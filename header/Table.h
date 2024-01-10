#pragma once

#define d_x (width/2 - 1)
#define d_y (height)

std::string text_table[4] {"Single","Double","Triple","Tetris"};
void qClear(std::queue<Block*>& q) {
	while(!q.empty()){
		delete q.front();
		q.pop();
	}
}// give the table a empty queue

//color table
const Color color_table[9] = {Color(0, 0, 0), Color(15, 155, 215), Color(34, 65, 197), Color(215, 16, 55), Color(227, 91, 2), Color(90, 177, 3), Color(227, 159, 0), Color(175, 41, 137), Color(187, 187, 187)};
const Color color_border[9] = {Color(80, 80, 80), Color(15, 155, 215), Color(34, 65, 197), Color(215, 16, 55), Color(227, 91, 2), Color(90, 177, 3), Color(227, 159, 0), Color(175, 41, 137), Color(187, 187, 187)};
const Color color_tBorder = Color(50, 50, 50);


//5-stage test for kick_table
const Point Kick_Table(bool isI,short start, const short& drc, const short& test){
    const static Point delta_notI[5] = { Point(0, 0), Point(-1, 0), Point(-1, 1), Point(0, -2), Point(-1,-2) };// for kick table try
    const static Point delta_I1[5] = { Point(0, 0), Point(-1, 0), Point(2, 0), Point(-1, 2), Point(2, -1) };// for kick table try
    const static Point delta_I2[5] = { Point(0, 0), Point(-2, 0), Point(1, 0), Point(-2, -1), Point(1, 2) };// for kick table try
    start = (start - drc + 4) %4;
    if(!isI){
        Point tmp; short factor;
        tmp = delta_notI[test];
        if(start == 2){
            tmp.x *= -drc;
        }
        else if(start == 0){
            tmp.x *= drc;
        }
        else{
            tmp.y *= -1;
            if(start == 1) tmp.x *= -1;
        }
        return tmp;
    }
    else{
        Point tmp; short factor;
        if(start % 2 && drc || !(start % 2) && drc == -1)
            if (start == 0 || start == 3)
                return delta_I2[test];
            else
                return delta_I2[test] * (-1);
        else
            if (start == 0 || start == 3)
                return delta_I1[test];
            else
                return delta_I1[test] * (-1);
    }
}

Block *id2block(const short& id){
	switch(id){
        case 1:{
            return new Block_T(Point(d_x, d_y));
        }
        case 2:{
            return new Block_L(Point(d_x, d_y));
        }
        case 3:{
            return new Block_J(Point(d_x, d_y));
        }
        case 4:{
            return new Block_S(Point(d_x, d_y));
        }
        case 5:{
            return new Block_Z(Point(d_x, d_y));
        }
        case 6:{
            return new Block_I(Point(d_x, d_y));
        }
        case 7:{
        	return new Block_O(Point(d_x, d_y));
        }
        default:{
            return nullptr;
        }
    }
}

class Table{
protected:
	short x, y, board[52][50]; //game id table, width range:(1,50) height range:(1,40)
    int score = 0, clear_line = 0, level = 0, garbage = 0, combo = 0;
public:
	Table() {}
	~Table() {}
	//move the table
	void set_position(const short& x, const short& y){this -> x = x, this -> y = y;}
	virtual void print_table() = 0; //print table on windows.h (x,y) is the origin of the table
	//return table data
    inline short get_x() const {return this -> x;}
    inline short get_y() const {return this -> y;}
    inline int get_score() const {return this -> score;}
    inline int get_line() const {return this -> clear_line;}
    inline int get_level() const {return this -> level;}
};

class Player:public Table{
private:
	Block *current = nullptr, *before = nullptr, *hold = nullptr;
    std::queue<Block*> next;
    bool tSpin = 0, pb2b = 0, b2b = 0, multi = 0, dis_clear = 0;
    int t_line, t_score;
    clock_t tStart, tClear;
public:
	Player() {}
	~Player() {
        delete current; delete before; delete hold;
        qClear(next);
    }
    // the initializer used to count the time passed
    void init(const clock_t& time, const bool& mut){
        std::queue<Block*> empty; std::swap(empty, next);
        tStart = time; multi = mut;
        pb2b = 0, b2b = 0, tSpin = 0, dis_clear = 0;
        score = 0, clear_line = 0, level = 0, garbage = 0, combo = 0; memset(board, 0, sizeof(board));
    }
    void new_block();
    // to fix the block
    void fix_block() {for(auto i : current -> block_position()) board[i.y][i.x] = current -> get_ID();}
    // to hold the block
    void hold_block();
    //block move
    void hard_drop();
    bool move_block(const short x, const short y);
    void rotate(const short direction);
    //printing
    void print_table() override; //print table on windows.h (x,y) is the origin of the table
    //checking
    bool isValid(const Block& tmp) const {for(auto i : tmp.block_position()) if(i.x < 0 || i.x >= width || i.y < 0 || (board[i.y][i.x]&7) ) return 0; return 1;}
    bool check_block(const Point &p) const;
    //filled check
    short chk_clear(int& line, int& score);
    //multi playing
    void SendTable(char str[]) const; //convert table into cstring (compression)
    void get_garbage(const short cnt) {garbage += cnt;}  
};
//block existence
void Player::new_block(){
    if(next.size() <= 1){
        //randomly generate
        short shuffle_block[7];
        for (int i = 0; i < 7; i++) shuffle_block[i] = (i + 1);
        std::random_shuffle(shuffle_block,shuffle_block+7);
        for (int i = 0; i < 7; i++)
        	next.push(id2block(shuffle_block[i]));
    }
    delete current;
    delete before;
    before = nullptr;
    current = nullptr;
    current = next.front();
    next.pop();
}
// hold the block
void Player::hold_block(){
    Block *tHold = id2block(current -> get_ID());
    (*tHold) = Point(0, 0);
    delete current;
    current = nullptr;
    if(hold){// see if there is already one block hold
        current = hold;
        (*current) = Point(d_x, d_y);
    }
    else
        this -> new_block();
    hold = tHold;
}
//block move
void Player::hard_drop(){
    Block *bTmp = current -> clone();// clone one to try if it works
    while(isValid(*bTmp)){
        (*bTmp) += Point(0, -1);
    }
    *bTmp += Point(0, 1);
    if(isValid(*bTmp)){
        *current = bTmp->get_location();
    }
    delete bTmp;
}
bool Player::move_block(const short x, const short y){
    Block *bTmp = current -> clone();// clone one to try if it works
    bool valid = isValid((*bTmp) += Point(x, y));
    if(valid){
        (*current) += Point(x, y);
        tSpin = 0;
    }
    delete bTmp;
    bTmp = nullptr;   
    return valid;
}
void Player::rotate(const short direction){
    Block *bTmp = current -> clone(); // clone one to try if it works
    bTmp -> rotate_set(direction);
    //current.rotate;
    for(int i = 0; i < 5; i++){
        Block *moveTmp = bTmp->clone();
        if(isValid((*moveTmp) += Kick_Table(bTmp -> isI(), bTmp -> get_direction(), direction, i))){
            current -> rotate_set(direction); 
            (*current) += Kick_Table(current -> isI(), current -> get_direction(), direction, i);
            break;
        }
        delete moveTmp;
    }
    tSpin = 0;
    if(current -> get_ID() == 1){
        Point tmp = current -> get_location();
        short tx, ty, tCnt = 0;
        for(int i = 0;i < 4;i++){
            tx = (tmp.x + ((i & 1)?1:-1)), ty = (tmp.y + ((i & 2)?1:-1));
            if(tx >= width || tx < 0 || ty < 0)
                tCnt++;
            else if(board[ty][tx])
                tCnt++;
        }
        if(tCnt >= 3)
            tSpin = 1;
    }
    delete bTmp;
    bTmp = nullptr;
}
//printing
void Player::print_table(){
    int tmp;
    //border:1 unit, side border: 4 unit, table border:6 unit, text&block:20 unit, empty block:18*18color, 1unit border
    Text text;
    text.setFont(font);
    RectangleShape BlockRect(Vector2f(unit * 18, unit * 18)), Side(Vector2f(unit * 80, unit * 80)), b_table(Vector2f(unit * width * 20, unit * height * 20));
    RectangleShape GarRect(Vector2f(unit * 3, unit * 19)), Garbage_can(Vector2f(unit * 5, unit * height * 20));
    BlockRect.setOutlineThickness(unit / 2.f); Side.setOutlineThickness(unit << 1); b_table.setOutlineThickness(unit << 2); Garbage_can.setOutlineThickness(unit << 2); GarRect.setOutlineThickness(unit);
    Side.setOutlineColor(color_tBorder); b_table.setOutlineColor(color_tBorder); Garbage_can.setOutlineColor(color_tBorder); GarRect.setOutlineColor(Color(0, 0, 0));
    Side.setFillColor(Color(0, 0, 0, 0)); b_table.setFillColor(Color(0, 0, 0, 0)); Garbage_can.setFillColor(Color(0, 0, 0)); GarRect.setFillColor(Color(195, 0, 0));
    //Draw table and side block table
    Side.setPosition(x + unit * 4, y + unit * 22); window.draw(Side); //hold
    Side.setPosition(x + unit * (111 + width * 20), y + unit * 22); window.draw(Side); //next
    if(multi) {Garbage_can.setPosition(x + unit * 92, y + unit * 4); window.draw(Garbage_can);} //garbage
    b_table.setPosition(x + unit * 101, y + unit * 4); window.draw(b_table); //table
    //hold label
    text.setColor(Color(255, 255, 255)); //white
    text.setCharacterSize(unit * 20);
    text.setString("HOLD");
    text.setOrigin(text.getGlobalBounds().left + round(text.getGlobalBounds().width / 2), text.getGlobalBounds().top + round(text.getGlobalBounds().height / 2)); //align center(4 word)
    text.setPosition(x + unit * 44, y + unit * 10);
    window.draw(text);
    //next label
    text.setString("NEXT");
    text.setPosition(x + unit * (151 + width * 20), y + unit * 10);
    window.draw(text);
    BlockRect.setFillColor(color_table[0]);
	BlockRect.setOutlineColor(color_border[0]);
    for(int i = 0;i < 4;i++) //hold board
    	for(int j = 0;j < 4;j++){
    		BlockRect.setPosition(x + unit * (5 + j * 20), y + unit * (23 + i * 20));
            window.draw(BlockRect);
		}
	for(int i = 0;i < 4;i++) //next board
    	for(int j = 0;j < 4;j++){
    		BlockRect.setPosition(x + unit * (112 + (j + width) * 20), y + unit * (23 + i * 20));
            window.draw(BlockRect);
		}
    if(hold){ //hold block
        tmp = hold -> get_ID();
        BlockRect.setFillColor(color_table[tmp]);
        BlockRect.setOutlineColor(color_border[tmp]);
        for(auto i:hold -> block_position()){
            BlockRect.setPosition(x + unit * (45 + i.x * 20), y + unit * (43 - i.y * 20));
            window.draw(BlockRect);
        }
    }
    if(!next.empty()){ //hold block
        tmp = next.front() -> get_ID();
        (*next.front()) = Point(0, 0);
        BlockRect.setFillColor(color_table[tmp]);
        BlockRect.setOutlineColor(color_border[tmp]);
        for(auto i:next.front() -> block_position()){
            BlockRect.setPosition(x + unit * (152 + (width + i.x) * 20), y + unit * (43 - i.y * 20));
            window.draw(BlockRect);
        }
        (*next.front()) = Point(d_x, d_y);
    }
    //board width:W+2, height:H+2
    for(int i = 0; i < height; ++i) { //row 1-20
        for (int j = 0; j < width; ++j) { //column 1-10
            BlockRect.setFillColor(color_table[board[i][j]]);
        	BlockRect.setOutlineColor(color_border[board[i][j]]);
        	BlockRect.setPosition(x + unit * (102 + j * 20), y + unit * ((height - i - 1) * 20 + 5));
        	window.draw(BlockRect);
        }
    }
    if(current)
	    for (auto i: current -> block_position())
	        if(i.y < height){
	        	tmp = current -> get_ID();
	            BlockRect.setFillColor(color_table[tmp]);
	        	BlockRect.setOutlineColor(color_border[tmp]);
	        	BlockRect.setPosition(x + unit * (102 + i.x * 20), y + unit * ((height - i.y - 1) * 20 + 5));
	        	window.draw(BlockRect);
	        }
	//set status
	text.setCharacterSize(unit * 15);
	text.setOrigin(0, 0);
	text.setString("Time:          " + to_string((clock() - tStart) / 1000));
	text.setPosition(x + unit * (111 + width * 20), y + unit * 108);
	window.draw(text);
	text.setString("Level:         " + to_string(level));
	text.setPosition(x + unit * (111 + width * 20), y + unit * 123);
	window.draw(text);
	text.setString("Score:        " + to_string(score));
	text.setPosition(x + unit * (111 + width * 20), y + unit * 138);
	window.draw(text);
	text.setString("Clear Line: " + to_string(clear_line));
	text.setPosition(x + unit * (111 + width * 20), y + unit * 153);
	window.draw(text);
	text.setString("Combo:      " + to_string(combo));
	text.setPosition(x + unit * (111 + width * 20), y + unit * 168);
	window.draw(text);
	//garbage
	if(multi){
        for(int i = 0;i < garbage;i++){
            GarRect.setPosition(x + unit * 93, y + unit * ((height - i - 1) * 20 + 5));
            window.draw(GarRect);
        }
    }
    if(dis_clear){
        string msg = "+\n";
        if(b2b)
            msg += "b2b\n";
        else
            msg += "\n";
        if(tSpin)
            msg += "T-Spin\n";
        else
            msg += "\n";
        msg += text_table[t_line-1];
        text.setString(msg);
        text.setPosition(x + unit * (111 + width * 20), y + unit * ((height - 4) * 20 + 15));
        text.setColor(Color(248, 247, 52)); //light yellow
        window.draw(text);
        if(clock() - tClear > 2000)
            dis_clear = 0;
    }
    return;
}

//checking
bool Player::check_block(const Point &p) const{
    Block *bTmp = current -> clone();
    bool result;
    *bTmp += p;
    result = isValid(*bTmp);
    delete bTmp;
    return result;
}
//line clear
short Player::chk_clear(int& line, int& tscore){
    bool allExist;// see if the whole row is filled
    int cnt = 0, point = 5, tGarbage = garbage;
    double multiplier = 1.0;// the total line num cleared
    for(int i=0; i<height; i++){
        allExist = true;
        for(int j=0; j<width; j++){
            if(!(this->board[i][j] & 7)){
                allExist = false;
                break;
            }
        }
        if(allExist){
            cnt++;
            for(int p=i;p<height; p++)
                for(int q=0; q<width; q++)
                    this->board[p][q] = this->board[p+1][q];
            i--; //check the line that have cleared. because move down
        }
    }
    if(garbage){
        if(cnt)
            garbage = std::max(0, garbage - cnt);
        else{
            int cleavage = rand() % 10;
            garbage = min(20, garbage);
            for(int i = height;i >= garbage;i--)
                for(int j = 0;j < width;j++)
                    board[i][j] = board[i-garbage][j];
            for(int i = 0;i < garbage;i++)
                for(int j = 0;j < width;j++)
                    board[i][j] = ((cleavage==j)?8:9);
            garbage = 0;
        }
    }
    // update the current condition
    tSpin = (tSpin && cnt);
    b2b = (pb2b && (cnt == 4 || tSpin));
    pb2b = (cnt?(cnt == 4 || tSpin):pb2b);
    this -> clear_line += cnt;
    combo = (cnt?(combo+1):0);
    point = ((cnt?(point<<cnt):0));
    multiplier *= pow(2,(tSpin + b2b * 3));
    multiplier *= combo?pow(1.1, combo-1):0;
    if(cnt){
        t_score += point * multiplier;
        t_line = cnt;
        dis_clear = 1;
        tClear = clock();
        this -> score += t_score;
        line = this -> clear_line;
        tscore = this -> score;
        level = min(29, clear_line/10);
    }
    tSpin = 0;
    for(int i = 0;i < width;i++)
        if(board[height][i])
            return -1; //game over token
    return (max(0, cnt-tGarbage));
}
//multi playing
void Player::SendTable(char str[]) const{
    short tx, ty, sTmp;
    int tlevel = (this -> level), tScore = (this -> score), tLine = (this -> clear_line);
    char tmp;
    for(int i = 0;i < 100;i++){
        tmp = 0;
        tx = (i * 2) % 10, ty = (i * 2) / 10;
        for(int j = 0;j < 2;j++)
            tmp |= (board[ty][tx + j] << (j * 4));
        str[i] = tmp;
    }
    for(auto i:current -> block_position()){
        if(i.y >= 20) continue;
        sTmp = i.y * 5 + (i.x >> 1);
        str[sTmp] |= ((current -> get_ID()) << ((i.x % 2)?4:0));
    }
    //str[100] last 3 bit => next
    str[100] = (next.front() -> get_ID());
    //str[100] first 3 bit => hold
    str[100] |= (hold?((hold -> get_ID())<<4):0);
    //str[101:103] level
    for(int i = 101;i < 103;i++){
        str[i] = (tlevel & 127);
        tlevel >>= 7;
    }
    //str[103:107] score
    for(int i = 103;i < 107;i++){
        str[i] = (tScore & 127);
        tScore >>= 7;
    }
    //str[107:110] clearline
    for(int i = 107;i < 110;i++){
        str[i] = (tLine & 127);
        tLine >>= 7;
    }
}

class Opponent:public Table{
private:
    short hold, next;
public:
    // initailize the table
    void init(){
        score = 0, clear_line = 0, level = 0, garbage = 0; memset(board, 0, sizeof(board));
        hold = 0, next = 0;
    }
    //printing
    void print_table() override;
    //multi playing
    void RecvTable(const char str[]);
};
//printing
void Opponent::print_table(){
    int tmp; Block *pTmp;
    //border:1 unit, side border: 4 unit, table border:6 unit, text&block:20 unit, empty block:18*18color, 1unit border
    Text text;
    text.setFont(font);
    RectangleShape BlockRect(Vector2f(unit * 18, unit * 18)), Side(Vector2f(unit * 80, unit * 80)), b_table(Vector2f(unit * width * 20, unit * height * 20));
    BlockRect.setOutlineThickness(unit / 2.f); Side.setOutlineThickness(unit << 1); b_table.setOutlineThickness(unit << 2);
    Side.setOutlineColor(color_tBorder); b_table.setOutlineColor(color_tBorder);
    Side.setFillColor(Color(0, 0, 0, 0)); b_table.setFillColor(Color(0, 0, 0, 0));
    //Draw table and side block table
    Side.setPosition(x + unit * 4, y + unit * 24); window.draw(Side); //hold
    Side.setPosition(x + unit * (104 + width * 20), y + unit * 24); window.draw(Side); //next
    b_table.setPosition(x + unit * 94, y + unit * 6); window.draw(b_table); //table
    //hold label
    text.setColor(Color(255, 255, 255)); //white
    text.setCharacterSize(unit * 20);
    text.setString("HOLD");
    text.setOrigin(text.getGlobalBounds().left + round(text.getGlobalBounds().width / 2), text.getGlobalBounds().top + round(text.getGlobalBounds().height / 2)); //align center(4 word)
    text.setPosition(x + unit * 44, y + unit * 10);
    window.draw(text);
    //next label
    text.setString("NEXT");
    text.setPosition(x + unit * (144 + width * 20), y + unit * 10);
    window.draw(text);
    BlockRect.setFillColor(color_table[0]);
	BlockRect.setOutlineColor(color_border[0]);
    for(int i = 0;i < 4;i++) //hold board
    	for(int j = 0;j < 4;j++){
    		BlockRect.setPosition(x + unit * (5 + j * 20), y + unit * (25 + i * 20));
            window.draw(BlockRect);
		}
	for(int i = 0;i < 4;i++) //next board
    	for(int j = 0;j < 4;j++){
    		BlockRect.setPosition(x + unit * (105 + (j + width) * 20), y + unit * (25 + i * 20));
            window.draw(BlockRect);
		}
    if(hold){ //hold block
    	pTmp = id2block(hold);
        (*pTmp) = Point(0, 0);
        BlockRect.setFillColor(color_table[hold]);
        BlockRect.setOutlineColor(color_border[hold]);
        for(auto i:pTmp -> block_position()){
            BlockRect.setPosition(x + unit * (45 + i.x * 20), y + unit * (45 - i.y * 20));
            window.draw(BlockRect);
        }
        delete pTmp;
    }
    if(next){
        pTmp = id2block(next);
        (*pTmp) = Point(0, 0);
        BlockRect.setFillColor(color_table[next]);
        BlockRect.setOutlineColor(color_border[next]);
        for(auto i:pTmp -> block_position()){
            BlockRect.setPosition(x + unit * (145 + (width + i.x) * 20), y + unit * (45 - i.y * 20));
            window.draw(BlockRect);
        }
        delete pTmp;
    }
    //board width:W+2, height:H+2
    for(int i = 0; i < height; ++i) { //row 1-20
        for (int j = 0; j < width; ++j) { //column 1-10
            BlockRect.setFillColor(color_table[board[i][j]]);
        	BlockRect.setOutlineColor(color_border[board[i][j]]);
        	BlockRect.setPosition(x + unit * (95 + j * 20), y + unit * ((height - i - 1) * 20 + 5));
        	window.draw(BlockRect);
        }
    }
	//set status
	text.setCharacterSize(unit * 15);
	text.setOrigin(0, 0);
	text.setString("Level:         " + to_string(level));
	text.setPosition(x + unit * (104 + width * 20), y + unit * 108);
	window.draw(text);
	text.setString("Score:        " + to_string(score));
	text.setPosition(x + unit * (104 + width * 20), y + unit * 123);
	window.draw(text);
	text.setString("Clear Line: " + to_string(clear_line));
	text.setPosition(x + unit * (104 + width * 20), y + unit * 138);
	window.draw(text);
    return;
}
//multi playing
void Opponent::RecvTable(const char str[]){
    short tx, ty;
    char tmp;
    //decompreesion
    //board
    for(int i = 0;i < 100;i++){
        tmp = str[i];
        tx = (i * 2) % 10, ty = (i * 2) / 10;
        for(int j = 0;j < 2;j++){
            board[ty][tx + j] = (tmp & 15);
            tmp >>= 4;
        }
    }
    //next 100 last 3
    next = (str[100] & 7);
    //hold 100 first 3
    hold = ((str[100] >> 3) & 7);
    //level [101,103)
    for(int i = 102;i >= 101;i--){
        level <<= 7;
        level |= (str[i] & 127);
    }
    //score [103,107)
    for(int i = 106;i >= 103;i--){
        score <<= 7;
    	score |= (str[i] & 127);
    }
    //line [107,110)
    for(int i = 109;i >= 107;i--){
        clear_line <<= 7;
        clear_line |= (str[i] & 127);
    }
}

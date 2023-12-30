#define DEFAULT_COLOR 112
#define d_x (width/2 - 1)
#define d_y (height-1)

std::string text_table[5] {"T-Spin","Single","Double","Triple","Tetris"};
void qClear(std::queue<Block*>& q) {
	while(!q.empty()){
		delete q.front();
		q.pop();
	}
}// give the table a empty queue

//allow various playing mode
short height = 20, width = 10;
const short color_table[9] = {DEFAULT_COLOR, 5*16, 0, 1*16, 2*16, 4*16, 3*16, 6*16, 240}; //[0] for none [8] for garbage(112+128, overflow is intentional)

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
	short x, y, board[42][50]; //game id table, width range:(1,50) height range:(1,40)
    int score = 0, clear_line = 0, level = 0, garbage = 0, combo = 0;
public:
	Table() {}
	~Table() {}
	//move the table
	void set_position(const short x, const short y){this -> x = x, this -> y = y;}
	virtual void print_table() = 0; //print table on windows.h (x,y) is the origin of the table
	//return table data
    inline short get_x() const {return this -> x;}
    inline short get_y() const {return this -> y;}
    inline int get_score() const {return this -> score;}
    inline int get_line() const {return this -> clear_line;}
    inline int get_level() const {return this -> level;}
    //multi playing
    void send_garbage(); //part of the code depending on socket can wait
    void get_garbage();  //part of the code depending on socket can wait
};

class Player:public Table{
private:
	Block *current = nullptr, *before = nullptr, *hold = nullptr;
    std::queue<Block*> next;
    bool tSpin = 0, pb2b = 0, b2b = 0;
    clock_t tStart;
public:
	Player() {}
	~Player() {
        delete current; delete before; delete hold;
        qClear(next);
    }
    // the initializer used to count the time passed
    void init(clock_t time){
        std::queue<Block*> empty; std::swap(empty, next);
        tStart = time;
        pb2b = 0, b2b = 0, tSpin = 0;
        score = 0, clear_line = 0, level = 0, garbage = 0, combo = 0; memset(board, 0, sizeof(board));
        current = nullptr, before = nullptr, hold = nullptr;
    }
    void new_block();
    // to fix the block
    void fix_block() {for (auto i : current -> block_position()) board[i.y][i.x] = current -> get_ID();}
    // to hold the block
    void hold_block();
    //block move
    void hard_drop();
    bool move_block(const short x, const short y);
    void rotate(const short direction);
    //printing
    void print_table() override; //print table on windows.h (x,y) is the origin of the table
    void print_block();
    //checking
    bool isValid(const Block& tmp) const {for(auto i : tmp.block_position()) if(i.x < 0 || i.x >= width || i.y < 0 || board[i.y][i.x]) return 0; return 1;}
    bool check_block(const Point &p) const;
    //filled check
    bool chk_clear(int& line, int& score);
    //multi playing
    void SendTable(char str[]) const; //convert table into cstring (compression)
};
//block existence
void Player::new_block(){
    if(next.size() <= 1){
        //randomly generate
        srand( time(NULL) );
        short shuffle_block[7];
        for (int i = 0; i < 7; i++) shuffle_block[i] = (i + 1);
        std::random_shuffle(shuffle_block,shuffle_block+7);
        for (int i = 0; i < 7; i++)
        	next.push(id2block(shuffle_block[i]));
    }
    delete before;
    before = nullptr;
    delete current;
    current = nullptr;
    current = next.front();
    before = current->clone();
    next.pop();
}
// hold the block
void Player::hold_block(){
    if(hold){// see if there is already one block hold
        std::swap(current, hold);
        (*hold) = Point(d_x, d_y);
    }
    else{
        hold = current;
        (*hold) = Point(d_x, d_y);
        current = nullptr;
        this -> new_block();
    }
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
    //hold width:5, height:6
    for(int i = 0; i < 4; ++i) {
        goto_xy(x, y + i + 1);
        set_color(color_table[0] + (bright?128:0));
        std::cout << '|';
        for(int j = 0;j < 4;j++) std::cout << ' ';
    }
    goto_xy(x, y + 5);
    for(int i = 0;i < 5;i++) std::cout << '-';
    if(hold){
        (*hold) = Point(0, 0);
        for(auto i:hold -> block_position()){
            goto_xy(x + 3 + i.x, y + 2 - i.y);
            set_color(color_table[(hold -> get_ID())] + (bright?128:0));
            std::cout << ' ';
        }
        (*hold) = Point(d_x, d_y);
    }
    goto_xy(x, y); //row 0
    set_color(color_table[0] + (bright?128:0));
    std::cout << "-HOLD-";
    for(int i = 0;i < width; i++) std::cout << '-'; 
    std::cout << "-NEXT-";
    //board width:W+2, height:H+2
    for(int i = 0; i < height; ++i) { //row 1-20
        goto_xy(x + 5, y + i + 1); //column 0
        std::cout << '|';
        for (int j = 0; j < width; ++j) { //column 1-10
            set_color(color_table[ board[height-1-i][j] ] + (board[height-1-i][j]?0:((j % 2)?128:0)) + (bright?128:0)); //inverse y-axis(19-i)
            std::cout << ' ';
        }
        set_color(color_table[0] + (bright?128:0));
        std::cout << '|'; //column 11
    }
    goto_xy(x + 5, y+height+1);
    for(int i = 0;i < width + 2; i++) std::cout << '-';//row 21
    //next
    for(int i = 0; i < 4; ++i) {
        goto_xy(x + width + 7, y + i + 1);
        set_color(color_table[0] + (bright?128:0));
        for(int j = 0;j < 4;j++) std::cout << ' ';
        std::cout << '|';
    }
    goto_xy(x + width + 7, y + 5);
    for(int i = 0;i < 5;i++) std::cout << '-';
    (*(next.front())) = Point(0, 0);
    for(auto i:next.front() -> block_position()){
        goto_xy(x + 9 + width + i.x, y + 2 - i.y);
        set_color(color_table[(next.front() -> get_ID())] + (bright?128:0));
        std::cout << ' ';
    }
    (*(next.front())) = Point(d_x, d_y);
    //set status
    set_color(color_table[0] + (bright?128:0));
    goto_xy(x + width + 7, y + 7);
    std::cout << "Level:     " << level;
    goto_xy(x + width + 7, y + 8);
    std::cout << "Score:     " << score;
    goto_xy(x + width + 7, y + 9);
    std::cout << "Clear Line:" << clear_line;
    goto_xy(x + width + 7, y + 10);
    set_color(0);
    std::cout << "               ";
    goto_xy(x + width + 7, y + 10);
    set_color(color_table[0] + (bright?128:0));
    std::cout << "Combo:     " << combo;
    return;
}

void Player::print_block() {
    if(current -> is_same_position(before)) return;
    short c = current -> get_ID(); // Change here
    for (auto i: before -> block_position())
        if(i.y < height){
            goto_xy(this->x + i.x + 6, this->y + height - i.y);
            set_color(color_table[0] + ((i.x + bright)%2?128:0));
            std::cout << ' ';
        }
    for (auto i: current -> block_position())
        if(i.y < height){
            goto_xy(this->x + i.x + 6, this->y + height - i.y);
            set_color(color_table[c] + (bright?128:0));
            std::cout << ' ';
        }
    set_color(color_table[0] + (bright?128:0));
    goto_xy(x + width + 7, y + 6);
    std::cout << "Time:      " << (clock() - tStart) / 1000;
    delete before;
    before = nullptr;
    before = current->clone();
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
bool Player::chk_clear(int& line, int& tscore){
    bool allExist;// see if the whole row is filled
    int cnt = 0, point = 5;
    double multiplier = 1.0;// the total line num cleared
    for(int i=0; i<height; i++){
        allExist = true;
        for(int j=0; j<width; j++){
            if(!this->board[i][j]){
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
    // update the current condition
    tSpin = (tSpin && cnt);
    b2b = (pb2b && (cnt == 4 || tSpin));
    pb2b = (cnt?(cnt == 4 || tSpin):pb2b);
    this -> clear_line += cnt;
    combo = (cnt?(combo+1):0);
    point = ((cnt?(point<<cnt):0));
    multiplier *= pow(2,(tSpin + b2b * 3));
    multiplier *= combo?pow(1.1, combo-1):0;
    this -> score += point * multiplier;
    set_color(14);
    if(cnt){
        goto_xy(x+width+8, y+height-3);
        std::cout << "+" << int(point * multiplier);
    }
    goto_xy(x+width+8, y+height-2);
    std::cout << (b2b?"b2b":"");
    goto_xy(x+width+8, y+height-1);
    std::cout << (tSpin?text_table[0]:"");
    goto_xy(x+width+8, y+height);
    std::cout << (cnt?text_table[cnt]:"");
    tSpin = 0;
    level = clear_line/10;
    if(level > 29) level = 29;
    line = this -> clear_line;
    tscore = this -> score;
    for(int i = 0;i < width;i++)
        if(board[height][i]){
            if(conn){
                if(server)
                    server_send("lose");
                else
                    client_send("lose");
                throw std::runtime_error("You lose!");
            }
            else
                throw std::runtime_error("Game over");
        }
    return cnt;
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
    //reset Board
    memset(board, 0, sizeof(board)); hold = 0, next = 0;
    //hold
    for(int i = 0; i < 4; ++i) {
        goto_xy(x, y + i + 1);
        set_color(color_table[0] + (bright?128:0));
        std::cout << '|';
        for(int j = 0;j < 4;j++) std::cout << ' ';
    }
    goto_xy(x, y + 5);
    for(int i = 0;i < 5;i++) std::cout << '-';
    //board
    goto_xy(x, y); //row 0
    set_color(color_table[0] + (bright?128:0));
    std::cout << "-HOLD-";
    for(int i = 0;i < width; i++) std::cout << '-'; 
    std::cout << "-NEXT-";
    for(int i = 0; i < height; ++i) { //row 1-20
        goto_xy(x + 5, y + i + 1); //column 0
        std::cout << '|';
        for (int j = 0; j < width; ++j) { //column 1-10
            set_color(color_table[0] + (j % 2?128:0) + (bright?128:0)); //inverse y-axis(19-i)
            std::cout << ' ';
        }
        set_color(color_table[0] + (bright?128:0));
        std::cout << '|'; //column 11
    }
    goto_xy(x + 5, y + 21);
    for(int i = 0;i < width + 2; i++) std::cout << '-';//row 21
    //next
    for(int i = 0; i < 4; ++i) {
        goto_xy(x + width + 7, y + i + 1);
        set_color(color_table[0] + (bright?128:0));
        for(int j = 0;j < 4;j++) std::cout << ' ';
        std::cout << '|';
    }
    goto_xy(x + width + 7, y + 5);
    for(int i = 0;i < 5;i++) std::cout << '-';
    set_color(color_table[0] + (bright?128:0));
    goto_xy(x + width + 7, y + 6);
    std::cout << "Level:     " << level;
    goto_xy(x + width + 7, y + 7);
    std::cout << "Score:     " << score;
    goto_xy(x + width + 7, y + 8);
    std::cout << "Clear Line:" << clear_line;
    return;
}
//multi playing
void Opponent::RecvTable(const char str[]){
    int tLevel = 0, tScore = 0, tLine = 0;
    short tx, ty;
    char tmp, tNext, tHold;
    Block *pTmp;
    //decompreesion
    //board
    for(int i = 0;i < 100;i++){
        tmp = str[i];
        tx = (i * 2) % 10, ty = (i * 2) / 10;
        for(int j = 0;j < 2;j++){
            if(board[ty][tx + j] ^ (tmp & 15)){ //if different
                goto_xy(x + tx + j + 6, y + (19 - ty) + 1);
                set_color(color_table[(tmp & 15)] + ((tmp & 15)?0:(((tx + j) % 2)?128:0)) + (bright?128:0)); //inverse y-axis(19-i)
                std::cout << ' ';
                board[ty][tx + j] = (tmp & 15);
            }
            tmp >>= 4;
        }
    }
    //next 100 last 3
    tNext = (str[100] & 7);
    //hold 100 first 3
    tHold = ((str[100] >> 3) & 7);
    if(tNext ^ next){ //if diff
        //reset board
        for(int i = 0; i < 4; ++i) {
            goto_xy(x + width + 7, y + i + 1);
            set_color(color_table[0] + (bright?128:0));
            for(int j = 0;j < 4;j++) std::cout << ' ';
        }
        pTmp = id2block(tNext);
        (*pTmp) = Point(0, 0);
        for(auto i:pTmp -> block_position()){
            goto_xy(x + 9 + width + i.x, y + 2 - i.y);
            set_color(color_table[(pTmp -> get_ID())] + (bright?128:0));
            std::cout << ' ';
        }
        next = tNext;
        delete pTmp;
    }
    if(tHold ^ hold){ //if diff
        for(int i = 0; i < 4; ++i) {
            goto_xy(x + 1, y + i + 1);
            set_color(color_table[0] + (bright?128:0));
            for(int j = 0;j < 4;j++) std::cout << ' ';
        }
        if(tHold){ //prevent from access nullptr
            pTmp = id2block(tHold);
            (*pTmp) = Point(0, 0);
            for(auto i:pTmp -> block_position()){
                goto_xy(x + 3 + i.x, y + 2 - i.y);
                set_color(color_table[(pTmp -> get_ID())] + (bright?128:0));
                std::cout << ' ';
            }
        }
        hold = tHold;
        delete pTmp;
    }
    //print status
    set_color(color_table[0] + (bright?128:0));
    //level [101,103)
    for(int i = 102;i >= 101;i--){
        tLevel <<= 7;
        tLevel |= (str[i] & 127);
    }
    if(tLevel ^ level){ //if diff
        goto_xy(x + width + 7, y + 6);
        std::cout << "Level:     " << tLevel;
        level = tLevel;
    }
    //score [103,107)
    for(int i = 106;i >= 103;i--){
        tScore <<= 7;
        tScore |= (str[i] & 127);
    }
    if(tScore ^ score){ //if diff
        goto_xy(x + width + 7, y + 7);
        std::cout << "Score:     " << tScore;
        score = tScore;
    }
    //line [107,110)
    for(int i = 109;i >= 107;i--){
        tLine <<= 7;
        tLine |= (str[i] & 127);
    }
    if(tLine ^ clear_line){ //if diff
        goto_xy(x + width + 7, y + 8);
        std::cout << "Clear Line:" << tLine;
        clear_line = tLine;
    }
}

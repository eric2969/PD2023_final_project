#define DEFAULT_COLOR 112
#define d_x 5
#define d_y 20

std::string text_table[5] {"T-Spin","Single","Double","Triple","Tetris"};
void goto_xy(int x, int y) {COORD pos = {x, y}; SetConsoleCursorPosition(hConsole, pos);}
void set_color(const unsigned short textColor) {SetConsoleTextAttribute(hConsole, textColor);}
void qClear(std::queue<Block*>& q) {std::queue<Block*> empty; std::swap(empty, q);}

const short color_table[8] = {DEFAULT_COLOR, 5*16, 0, 1*16, 2*16, 4*16, 3*16, 6*16}; //[0] for none

//5-stage test for kick_table
const Point Kick_Table(bool isI,short start, const short& drc, const short& test){
    const static Point delta_notI[5] = { Point(0, 0), Point(-1, 0), Point(-1, 1), Point(0, -2), Point(-1,-2) };
    const static Point delta_I1[5] = { Point(0, 0), Point(-1, 0), Point(2, 0), Point(-1, 2), Point(2, -1) };
    const static Point delta_I2[5] = { Point(0, 0), Point(-2, 0), Point(1, 0), Point(-2, -1), Point(1, 2) };
    start = (start - drc + 4) %4;
    if(!isI){
        Point tmp; short factor;
//        if(start % 2)
//            factor = (start>>1)?1:-1;
//            
//        else
//           factor = drc * ((start>>1)?-1:1);
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

class Table{
private:
    Block *current = nullptr, *before = nullptr, *hold = nullptr;
    std::queue<Block*> next;
    const static short height = 20, width = 10;
    short board[height+2][width]; //game id table
    short x, y;
    bool tSpin = 0, pb2b = 0, b2b = 0, bTime;
    int score = 0, clear_line = 0, level = 0, garbage = 0, combo = 0;
    clock_t tStart;
public:
    Table() : current(nullptr) { memset(board, 0, sizeof(board));}
    ~Table() {}
    void set_position(const short x, const short y){this -> x = x, this -> y = y;}
    void init(){
        pb2b = 0, b2b = 0;
        bTime = 0, combo = 0, tSpin = 0, score = 0, clear_line = 0; memset(board, 0, sizeof(board));
        current = nullptr, before = nullptr, hold = nullptr;
        qClear(next);
    }
    void init(clock_t time){
        tStart = time;
        pb2b = 0, b2b = 0;
        bTime = 1, combo = 0, tSpin = 0, score = 0, clear_line = 0; memset(board, 0, sizeof(board));
        current = nullptr, before = nullptr, hold = nullptr;
        qClear(next);
    }
    //block existence
    void new_block();
    void fix_block() {for (auto i : current -> block_position()) board[i.y][i.x] = current -> get_ID();}
    void hold_block();
    //block move
    void hard_drop();
    bool move_block(const short x, const short y);
    void rotate(const short direction);
    //printing
    void print_table() const; //print table on windows.h (x,y) is the origin of the table
    void print_block();
    //checking
    bool isValid(const Block& tmp) const {for(auto i : tmp.block_position()) if(i.x < 0 || i.x >= width || i.y < 0 || board[i.y][i.x]) return 0; return 1;}
    bool check_block(const Point &p) const;
    //filled check
    bool chk_clear(int& line, int& score);
    //return table data
    inline short get_x() const {return this -> x;}
    inline short get_y() const {return this -> y;}
    inline int get_score() const {return this -> score;}
    inline int get_line() const {return this -> clear_line;}
    inline int get_level() const {return this -> level;}
    //multi playing
    void SendTable(char str[]); //convert table into cstring (compression)
    void RecvTable(const char str[]); //convert cstring into table and print it(decompression)
    void send_garbage(); //part of the code depending on socket can wait
    void get_garbage();  //part of the code depending on socket can wait
};
bool Table::check_block(const Point &p) const{
    Block *bTmp = current -> clone();
    bool result;
    *bTmp += p;
    result = isValid(*bTmp);
    delete bTmp;
    return result;
}
//block existence
void Table::new_block(){
    if(next.size() <= 1){
        //randomly generate
        srand( time(NULL) );
        int shuffle_block[10];
        for (int i = 0; i < 7; i++) shuffle_block[i] = i;
        std::random_shuffle(shuffle_block,shuffle_block+7);
        for (int i = 0; i < 7; i++){
            switch(shuffle_block[i]){
                case 0:{
                    Block *I = new Block_I(Point(d_x, d_y));
                    next.push(I);
                    break;
                }
                case 1:{
                    Block *J = new Block_J(Point(d_x, d_y));
                    next.push(J);
                    break;
                }
                case 2:{
                    Block *L = new Block_L(Point(d_x, d_y));
                    next.push(L);
                    break;
                }
                case 3:{
                    Block *O = new Block_O(Point(d_x, d_y));
                    next.push(O);
                    break;
                }
                case 4:{
                    Block *S = new Block_S(Point(d_x, d_y));
                    next.push(S);
                    break;
                }
                case 5:{
                    Block *T = new Block_T(Point(d_x, d_y));
                    next.push(T);
                    break;
                }
                case 6:{
                    Block *Z = new Block_Z(Point(d_x, d_y));
                    next.push(Z);
                    break;
                }
                default:{
                    exit(1);
                    break;
                }
            }
        }
    }
    delete before;
    before = nullptr;
    delete current;
#ifdef DEBUG
    set_color(15);
    goto_xy(27,2);
    std::cout << current;
#endif // DEBUG
    current = nullptr;
    current = next.front();
    before = current->clone();
    next.pop();
}

void Table::hold_block(){
    if(hold){
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
void Table::hard_drop(){
    Block *bTmp = current -> clone();
    while(isValid(*bTmp)){
        (*bTmp) += Point(0, -1);
    }
    *bTmp += Point(0, 1);
    if(isValid(*bTmp)){
        *current = bTmp->get_location();
    }
    tSpin = 0;
    delete bTmp;
}

bool Table::move_block(const short x, const short y){
#ifdef DEBUG
    set_color(15);
    goto_xy(27, 3);
    std::cout << "Curr" << current;
#endif // DEBUG
    Block *bTmp = current -> clone();
#ifdef DEBUG
    set_color(15);
    goto_xy(27, 3);
    std::cout << "Current:" << current << " bTmp:" << bTmp;
#endif // DEBUG
    bool valid = isValid((*bTmp) += Point(x, y));
    if(valid){
        (*current) += Point(x, y);
        tSpin = 0;
    }
    delete bTmp;
    bTmp = nullptr;
#ifdef DEBUG
    set_color(15);
    goto_xy(27, 4);
    std::cout << "Current:" << current << " bTmp:" << bTmp;
#endif // DEBUG
    
    return valid;
}

void Table::rotate(const short direction){
    Block *bTmp = current -> clone(); // Change here
    bTmp -> rotate_set(direction);
    //current.rotate;
    
    
    for(int i = 0; i < 5; i++){
        
        Block *moveTmp = bTmp->clone();
        if(isValid((*moveTmp) += Kick_Table(bTmp -> isI(), bTmp -> get_direction(), direction, i))){
            current -> rotate_set(direction); // Change here
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

//line clear
bool Table::chk_clear(int& line, int& tscore){
    bool allExist;// see if the whole row is filled
    int cnt = 0, point = 5, multiplier = 1;// the total line num cleared
    for(int i=0; i<20; i++){
        allExist = true;
        for(int j=0; j<10; j++){
            if(!this->board[i][j]){
                allExist = false;
                break;
            }
        }
        if(allExist){
            cnt++;
            for(int p=i;p<20; p++)
                for(int q=0; q<10; q++)
                    this->board[p][q] = this->board[p+1][q];
            i--; //check the line that have cleared. because move down
        }
    }
    
    b2b = (pb2b && (cnt == 4 || tSpin));
    pb2b = (cnt == 4 || tSpin);
    set_color(14);
    goto_xy(x+18, y+18);
    std::cout << (b2b?"b2b":"");
    goto_xy(x+18, y+19);
    std::cout << (tSpin?text_table[0]:"");
    goto_xy(x+18, y+20);
    std::cout << (cnt?text_table[cnt]:"");
    this -> clear_line += cnt;
    combo = (cnt?(combo+1):0);
    point = ((cnt?(point<<cnt):0));
    multiplier <<= (combo + tSpin - 1 + b2b * 3);
    this -> score += point * multiplier;
    tSpin = 0;
    level = clear_line/10;
    if(level > 29) level = 29;
    line = this -> clear_line;
    tscore = this -> score;
    for(int i = 0;i < 10;i++)
        if(board[20][i])
            throw std::runtime_error("Game over");
    return cnt;
}

//printing
void Table::print_table() const{
    //hold
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
    //board
    goto_xy(x, y);
    set_color(color_table[0] + (bright?128:0));
    for(int i = 0;i < width + 12; i++) std::cout << '-'; //row 0
    for(int i = 0; i < height; ++i) { //row 1-20
        goto_xy(x + 5, y + i + 1); //column 0
        std::cout << '|';
        for (int j = 0; j < width; ++j) { //column 1-10
            set_color(color_table[ board[19-i][j] ] + (board[19-i][j]?0:((j % 2)?128:0)) + (bright?128:0)); //inverse y-axis(19-i)
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
    if(next.front()){
        (*(next.front())) = Point(0, 0);
        for(auto i:next.front() -> block_position()){
            goto_xy(x + 9 + width + i.x, y + 2 - i.y);
            set_color(color_table[(next.front() -> get_ID())] + (bright?128:0));
            std::cout << ' ';
        }
        (*(next.front())) = Point(d_x, d_y);
    }
    //status
    set_color(color_table[0] + (bright?128:0));
    goto_xy(x + width + 7, y + 7);
    std::cout << "Level:" << level;
    goto_xy(x + width + 7, y + 8);
    std::cout << "Score:" << score;
    goto_xy(x + width + 7, y + 9);
    std::cout << "Clear Line:" << clear_line;
    goto_xy(x + width + 7, y + 10);
    std::cout << "Combo:" << combo;
    return;
};

void Table::print_block() {
    if(current -> is_same_position(before)) return;
    short c = current -> get_ID(); // Change here
    for (auto i: before -> block_position())
        if(i.y < 20){
            goto_xy(this->x + i.x + 6, this->y + 20 - i.y);
            set_color(color_table[0] + ((i.x + bright)%2?128:0));
            std::cout << ' ';
        }
    for (auto i: current -> block_position())
        if(i.y < 20){
            goto_xy(this->x + i.x + 6, this->y + 20 - i.y);
            set_color(color_table[c] + (bright?128:0));
            std::cout << ' ';
        }
    if(bTime){
        set_color(color_table[0] + (bright?128:0));
        goto_xy(x + width + 7, y + 6);
        std::cout << "Time:" << (clock() - tStart) / 1000;
    }
    delete before;
    before = nullptr;
    before = current->clone();
}

//multi playing
void Table::SendTable(char str[]) {
    short tx, ty, sTmp;
    char tmp, snd[105] = "";
    for(int i = 0;i < 100;i++){
        tmp = 0;
        tx = (i * 2) % 10, ty = (i * 2) / 10;
        for(int j = 0;j < 2;j++)
            tmp |= (board[ty][tx + j] << (j * 3));
        snd[i] = tmp;
    }
    //for(auto i:current -> block_position()){
      //  sTmp = i.y * 5 + i.x / 5;
    //    snd[sTmp] |= ((current -> get_ID()) <<  ((i.x % 2)?3:0));
    //}
    strcpy(str, snd);
}
void Table::RecvTable(const char str[]) {
    short tx, ty;
    char tmp;
    for(int i = 0;i < 100;i++){
        tmp = str[i];
        tx = (i * 2) % 10, ty = (i * 2) / 10;
        for(int j = 0;j < 2;j++){
            board[ty][tx + j] = (tmp & 8);
            tmp >>= 3;
        }
    }
}
void Table::send_garbage() {}
void Table::get_garbage() {}

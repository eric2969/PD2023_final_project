#define DEFAULT_COLOR 0
#define BG 112
#define d_x 5
#define d_y 20

void goto_xy(int x, int y, HANDLE &hout) {COORD pos = {x, y}; SetConsoleCursorPosition(hout, pos);}
void set_color(const unsigned short textColor, HANDLE &hout) {SetConsoleTextAttribute(hout, textColor);}
void qClear(std::queue<Block*>& q) {std::queue<Block*> empty; std::swap(empty, q);}

const short color_table[8] = {DEFAULT_COLOR+BG, 0+BG, 1+BG, 2+BG, 3+BG, 4+BG, 5+BG, 6+BG}; //[0] for none
const char symbol_table[8] = {' ', '#', '@', '?', '$', '&', '%', '+'}; //[0] for none

//5-stage test for kick_table
const Point Kick_Table(bool isI, const short& start, const short& drc, const short& test){
    const static Point delta_notI[5] = { Point(0, 0), Point(-1, 0), Point(-1, 1), Point(0, -2), Point(-1,-2) };
    const static Point delta_I1[5] = { Point(0, 0), Point(-1, 0), Point(2, 0), Point(-1, 2), Point(2, -1) };
    const static Point delta_I2[5] = { Point(0, 0), Point(-2, 0), Point(1, 0), Point(-2, -1), Point(1, 2) };
    if(!isI){
        Point tmp; short factor;
        if(start % 2)
            factor = (start>>1)?1:-1;
        else
            factor = drc * ((start>>1)?-1:1);
        return delta_notI[test] * factor;
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
    bool bright_mode;
    int score = 0, clear_line = 0, level = 0, garbage = 0;
public:
    Table() : current(nullptr) { memset(board, 0, sizeof(board));}
    ~Table() {}
    void set_position(const short x, const short y){this -> x = x, this -> y = y;}
    void init(const bool& bri) {
        bright_mode = bri, score = 0, clear_line = 0; memset(board, 0, sizeof(board));
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
    void print_table(HANDLE &hConsole) const; //print table on windows.h (x,y) is the origin of the table
    void print_block(HANDLE &hConsole);
    //checking
    bool isValid(const Block& tmp) const {for(auto i : tmp.block_position()) if(i.x < 0 || i.x >= 10 || i.y < 0 || board[i.y][i.x]) return 0; return 1;}
    bool isT_Spin() const;
    //filled check
    bool chk_clear(const int& combo, const bool& tspin);
    //return table data
    inline int get_score() const {return this -> score;}
    inline int get_line() const {return this -> clear_line;}
    inline int get_level() const {return this -> level;}
    //multi playing
    void SendTable(char str[]); //convert table into cstring (compression)
    void RecvTable(HANDLE &hConsole, const char str[]); //convert cstring into table and print it(decompression)
    void send_garbage(); //part of the code depending on socket can wait
    void get_garbage();  //part of the code depending on socket can wait
};
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
    delete current;
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
    delete bTmp;
}

bool Table::move_block(const short x, const short y){
    Block *bTmp = current -> clone();
    bool valid = isValid((*bTmp) += Point(x, y));
    if(valid)// Change here
        (*current) += Point(x, y); // Change here
    delete bTmp;
    return valid;
}

void Table::rotate(const short direction){
    Block *bTmp = current -> clone(); // Change here
    bTmp -> rotate_set(direction);
    //current.rotate;
    for(int i = 0; i < 5; i++){
        if(isValid((*bTmp) += Kick_Table(bTmp -> isI(), bTmp -> get_direction(), direction, i))){
            current -> rotate_set(direction); // Change here
            (*current) += Kick_Table(current -> isI(), current -> get_direction(), direction, i);
            break;
        }
    }
    delete bTmp;
}

//line clear
bool Table::chk_clear(const int& combo = 0, const bool& tspin = 0){ //need to append b2b, t-spin, etc...
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
    this -> clear_line += cnt;
    point = ((cnt?(point<<cnt):0));
    multiplier <<= (combo + tspin);
    this -> score += point * multiplier;
    level = clear_line/10;
    if(level > 29) level = 29;
    for(int i = 0;i < 10;i++)
        if(board[20][i])
            throw std::runtime_error("game over");
    return cnt;
}

//printing
void Table::print_table(HANDLE &hConsole) const{
    //hold
    for(int i = 0; i < 4; ++i) {
        goto_xy(x, y + i + 1, hConsole);
        set_color(color_table[0], hConsole);
        std::cout << '|';
        for(int j = 0;j < 4;j++) std::cout << ' ';
    }
    goto_xy(x, y + 5, hConsole);
    for(int i = 0;i < 5;i++) std::cout << '-';
    if(hold){
        (*hold) = Point(0, 0);
        for(auto i:hold -> block_position()){
            goto_xy(x + 3 + i.x, y + 2 - i.y, hConsole);
            set_color(color_table[(hold -> get_ID())] + (bright_mode?8:0), hConsole);
            std::cout << symbol_table[(hold -> get_ID())];
        }
        (*hold) = Point(d_x, d_y);
    }
    //board
    goto_xy(x, y, hConsole);
    set_color(color_table[0], hConsole);
    for(int i = 0;i < width + 12; i++) std::cout << '-'; //row 0
    for(int i = 0; i < height; ++i) { //row 1-20
        goto_xy(x + 5, y + i + 1, hConsole); //column 0
        std::cout << '|';
        for (int j = 0; j < width; ++j) { //column 1-10
            set_color(color_table[ board[19-i][j] ] + (j%2?128:0) + ((bright_mode && board[19-i][j] != 7)?8:0), hConsole); //inverse y-axis(19-i)
            std::cout << symbol_table[ board[19-i][j] ];
        }
        set_color(color_table[0], hConsole);
        std::cout << '|'; //column 11
    }
    goto_xy(x + 5, y + 21, hConsole);
    for(int i = 0;i < width + 2; i++) std::cout << '-';//row 21
    //next
    for(int i = 0; i < 4; ++i) {
        goto_xy(x + width + 7, y + i + 1, hConsole);
        set_color(color_table[0], hConsole);
        for(int j = 0;j < 4;j++) std::cout << ' ';
        std::cout << '|';
    }
    goto_xy(x + width + 7, y + 5, hConsole);
    for(int i = 0;i < 5;i++) std::cout << '-';
    if(next.front()){
        (*(next.front())) = Point(0, 0);
        for(auto i:next.front() -> block_position()){
            goto_xy(x + 9 + width + i.x, y + 2 - i.y, hConsole);
            set_color(color_table[(next.front() -> get_ID())] + (bright_mode?8:0), hConsole);
            std::cout << symbol_table[(next.front() -> get_ID())];
        }
        (*(next.front())) = Point(d_x, d_y);
    }
    //status
    set_color(color_table[0], hConsole);
    goto_xy(x + width + 7, y + 6, hConsole);
    std::cout << "Level:" << level;
    goto_xy(x + width + 7, y + 7, hConsole);
    std::cout << "Score:" << score;
    goto_xy(x + width + 7, y + 8, hConsole);
    std::cout << "Clear Line:" << clear_line;
    return;
};

void Table::print_block(HANDLE &hConsole) {
    if(current -> is_same_position(before)) return;
    short c = current -> get_ID(); // Change here
    for (auto i: before -> block_position())
        if(i.y < 20){
            goto_xy(this->x + i.x + 6, this->y + 20 - i.y, hConsole);
            set_color(color_table[0] + (i.x%2?128:0), hConsole);
            std::cout << ' ';
        }
    for (auto i: current -> block_position())
        if(i.y < 20){
            goto_xy(this->x + i.x + 6, this->y + 20 - i.y, hConsole);
            set_color(color_table[c] + (i.x%2?128:0) + ((bright_mode && c != 7)?8:0), hConsole);
            std::cout << symbol_table[c];
        }
    delete before;
    before = current->clone();
}

bool Table::isT_Spin() const {
    // Implement the logic for T-Spin check
    return false;
}

//multi playing
void Table::SendTable(char str[]) {}
void Table::RecvTable(HANDLE &hConsole, const char str[]) {}
void Table::send_garbage() {}
void Table::get_garbage() {}

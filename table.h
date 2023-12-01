#define DEFAULT_COLOR 7
 
void goto_xy(int x, int y, HANDLE &hout) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(hout, pos);
}

void set_color(const unsigned short textColor, HANDLE &hout) {
    if (textColor >= 0 && textColor <= 15)
        SetConsoleTextAttribute(hout, textColor);
    else
        SetConsoleTextAttribute(hout, 7);
}

const short color_table[8] = {0, 1, 2, 3, 4, 5, 6, 7}; //[0] for none
const char symbol_table[8] = {' ', '#', '@', '?', '$', '&', '%', '+'}; //[0] for none

const Point Kick_Table(bool isI, const short& start, const short& drc, const short& test){
    const static Point delta[5] = { Point(0, 0), Point(-1, 0), Point(-1, 1), Point(0, -2), Point(-1,-2) };
    if(!isI){
        Point tmp; short factor;
        if(start % 2)
            factor = (start>>1)?1:-1;
        else
            factor = drc * ((start>>1)?-1:1);
        return delta[test] * factor;
    }
    else{

    }
}

class Table{
private:
    bool isI, n_isI; //for kick table
    Block current, next;
    const static short height = 20, width = 10;
    short id[height][width]; //game id table
    int score = 0, clear_line = 0, level = 0;
    int arr, gravity, das, multiplier = 0, garbage = 0, B2B = 0;
public:
    Table() { memset(id, 0, sizeof(id)); return;}
    ~Table() {}
    //block existence
    void fix_block();
    void del_block();
    void add_block(const Block& add);
    //block move
    void move_block(const short& x, const short& y);
    void rotate(const short& direction);
    //printing
    void print_table(const int& x, const int& y, HANDLE &hConsole) const; //print table on windows.h (x,y) is the origin of the table
    void print_block(HANDLE &hConsole) const;
    //set level
    void set_level(const int& level);
    //checking
    bool isValid(const Block& tmp) const;
    bool isT_Spin() const;
    //multi playing
    void getTable(); //part of the code depending on socket, only for opponent's table
    void send_garbage(); //part of the code depending on socket can wait
    void get_garbage();  //part of the code depending on socket can wait
};

//block existence
void Table::fix_block() {
    std::vector<Point> p = current.block_position();
    for (auto i : p)
        id[i.y][i.x] = current.get_ID();
}

void Table::del_block(){
    return;
}

void Table::add_block(const Block& add){
    current = next;
    next = add;
    return;
}

//block move
void Table::move_block(const short& x, const short& y){
    Point pTmp(x, y);
    if(isValid(current + pTmp)){
        std::vector<Point> p = current.block_position();
//        for(auto i : p)
//            id[i.y + y][i.x + x] = current.get_ID(), id[i.y][i.x] = 0;
        current += pTmp;
    }
    return;
}

void Table::rotate(const short& direction){
    current.rotate(direction);
    return;
}

//printing
void Table::print_table(const int& x, const int& y, HANDLE &hConsole) const{
    goto_xy(x, y, hConsole);
    set_color(DEFAULT_COLOR, hConsole);
    for(int i = 0;i < width + 2; i++)
        std::cout << '-';
    for (int i = 0; i < height; ++i) {
        goto_xy(x, y + i + 1, hConsole);
        std::cout << '|';
        for (int j = 0; j < width; ++j) {
            if (id[i][j] == 0) {
                std::cout << ' ';
                continue;
            }
            set_color(color_table[ id[i][j] ], hConsole);
            std::cout << symbol_table[ id[i][j] ];
            set_color(DEFAULT_COLOR, hConsole);
        }
        std::cout << '|';
    }
    goto_xy(x, y + 20, hConsole);
    for(int i = 0;i < width + 2; i++)
        std::cout << '-';
    return;
};

void Table::print_block(HANDLE &hConsole) const{
    std::vector<Point> p = current.block_position();
    short c = current.get_ID();
    for (int i = 0; i < 4; i++) {
        goto_xy(p[i].y, p[i].x, hConsole);
        set_color(color_table[c], hConsole);
        std::cout << symbol_table[c];
    }
}

void Table::set_level(const int& level) {
    this -> level = level;
    return;
}

//checking
bool Table::isValid(const Block& tmp) const{
    std::vector<Point> p = tmp.block_position();
    for(auto i : p)
        if(id[i.y][i.x])
            return 0;
    return 1;
}

bool Table::isT_Spin() const{

}

//multi playing
void Table::getTable(){
}
void Table::send_garbage(){
}
void Table::get_garbage(){
}

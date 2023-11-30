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

const Point Kick_Table(const short& start, const short& drc, const short& test){
    const static Point delta[5] = { Point(0, 0), Point(-1, 0), Point(-1, 1), Point(0, -2), Point(-1,-2) };
    Point tmp; short factor;
    if(start % 2)
        factor = (start>>1)?1:-1;
    else
        factor = drc * ((start>>1)?-1:1);
    return delta[test] * factor;
}

class Table{
private:
    Block current, next;
    const static short height = 20, width = 10;
    short color[height][width]; //game color table
    char symbol[height][width];
    int score = 0, clear_line = 0, level = 0;
    int arr, gravity, das, multiplier = 0, garbage = 0, B2B = 0;
public:
    Table();
    ~Table() {}
    void del_block();
    void add_block(const Block& add);
    void move_block(const short& x, const short& y);
    void fix_block();
    void rotate(const short& direction);
    void print_table(const int& x, const int& y, HANDLE &hConsole) const; //print table on windows.h (x,y) is the origin of the table
    void print_block(HANDLE &hConsole) const;
    void set_level(const int& level);
    bool isValid(const Block& tmp) const;
    bool isT_Spin() const;
    void getTable(); //part of the code depending on socket, only for opponent's table
    void send_garbage(); //part of the code depending on socket can wait
    void get_garbage();  //part of the code depending on socket can wait
};

Table::Table(){
    memset(symbol, ' ', sizeof(symbol));
    memset(color, 0, sizeof(color));
}

void Table::del_block(){
    return;
}

void Table::add_block(const Block& add){
    current = next;
    next = add;
    return;
}

void Table::move_block(const short& x, const short& y){
    Block tmp = current;
    tmp.move(x, y);
    if(!(this -> isValid(tmp)))
       return;
    std::vector<Point> p = current.block_position();
    for(auto i : p){
        color[i.y + y][i.x + x] = current.get_color(), symbol[i.y + y][i.x + x] = current.get_symbol();
        color[i.y][i.x] = 0, symbol[i.y][i.x] = ' ';
    }
    current.move(x, y);
    return;
}

void Table::fix_block() {
    std::vector<Point> p = current.block_position();
    for (auto i : p) {
        color[i.y][i.x] = current.get_color();
        symbol[i.y][i.x] = current.get_symbol();
    }
}

void Table::rotate(const short& direction){
    current.rotate(direction);
    return;
}

void Table::print_table(const int& x, const int& y, HANDLE &hConsole) const{
    goto_xy(x, y, hConsole);
    for(int i = 0;i < width + 2; i++)
        std::cout << '-';
    for (int i = 0; i < height; ++i) {
        goto_xy(x, y + i + 1, hConsole);
        std::cout << '|';
        for (int j = 0; j < width; ++j) {
            if (color[i][j] == 0) {
                std::cout << ' ';
                continue;
            }
            set_color(color[i][j], hConsole);
            std::cout << symbol[i][j];
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
    short c = current.get_color();
    for (int i = 0; i < 4; i++) {
        goto_xy(p[i].y, p[i].x, hConsole);
        set_color(c, hConsole);
        std::cout << current.get_symbol();
    }
}

void Table::set_level(const int& level) {
    this -> level = level;
    return;
}

bool Table::isValid(const Block& tmp) const{
    std::vector<Point> p = tmp.block_position();
    for(auto i : p)
        if(symbol[i.y][i.x])
            return 0;
    return 1;
}
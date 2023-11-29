void goto_xy(int x, int y, HANDLE &hout) {
    COORD pos = { x,y };
    SetConsoleCursorPosition(hout, pos);
}

void color(const unsigned short textColor, HANDLE &hout)
{
    if (textColor >= 0 && textColor <= 15) {
        SetConsoleTextAttribute(hout, textColor);
    }
    else {
        SetConsoleTextAttribute(hout, 7);
    }
}

class Table{
private:
    bool block_set = 0;
    short matrix[23][10] ; //game table
    int score = 0, clear_line = 0, level = 0;
    int height = 20, width = 10;
    int arr, gravity, das;
    int multiplier = 0;
    Block current;
    int garbage = 0;
    int B2B = 0;
public:
    Table(int ,int);
    ~Table() {};
    void del_block();
    void add_block(const Block& add);
    void move_block(const short& x, const short& y);
    void fix_block();
    void rotate(const short& direction);
    void print_table(const int& x, const int& y, HANDLE &hConsole); //print table on windows.h (x,y) is the origin of the table
    void print_block(HANDLE &hConsole);
    void set_level(const int& level);
    bool isValid();
    bool isT_Spin();
    void send_garbage(); //part of the code depending on socket can wait
    void get_garbage();  //part of the code depending on socket can wait
};

Table::Table(int height = 20, int width = 10){
    memset(matrix, 0, sizeof(matrix));
    this->width = width;
    this->height = height;
} 

void Table::del_block(){
    return;
}

void Table::add_block(const Block& add){
    block_set = 1;
    current = add;
    return;
}

void Table::move_block(const short& x, const short& y){
    current.move(x, y);
    return;
}

void Table::fix_block() {
    std::vector<Point> p = current.block_position();
    for (int i = 0; i < 4; i++) {
        matrix[p[i].y][p[i].x] = current.get_color();
    } 
}

void Table::rotate(const short& direction){
    current.rotate(direction);
    return;
}

void Table::print_table(const int& x, const int& y, HANDLE &hConsole){
    goto_xy(x, y, hConsole);
    for(int i=0;i<width+2; i++) std::cout << '-';
    std::cout << '\n';
    for (int i = 0; i < height; ++i) {
        goto_xy(x+i+1,y,hConsole);
        std::cout << '|';
        for (int j = 0; j < width; ++j) {
            /*if (board[i][j] == 'O') {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
            }
            else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
            }*/
            if (matrix[i][j] == 0) {
                std::cout << ' ';
                continue;
            }
            color(matrix[i][j], hConsole);
            std::cout << matrix[i][j];
        }
        std::cout << '|' << std::endl;
    }
    for(int i=0;i<width+2; i++) std::cout << '-';
    return;
};

void Table::print_block(HANDLE &hConsole) {
    std::vector<Point> p = current.block_position();
    short c = current.get_color();
    for (int i = 0; i < 4; i++) {
        goto_xy(p[i].y, p[i].x, hConsole);
        color(c, hConsole);
        std::cout << current.get_symbol();
    }
}

void Table::set_level(const int& level) {
    this -> level = level;
    return;
}

bool Table::isValid() {

}

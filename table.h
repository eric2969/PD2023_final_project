#include<block.h>
#include<windows.h>

class Table{
private:
    short matrix[10][23]; //game table
    int score = 0, clear_line = 0, level = 0;
    int height = 20, wide = 10;
    int arr, gravity, das;
    float multiplier = 0;
    Block current;
    int garbage = 0;
public:
    Table() {};
    ~Table() {};
    void del_block();
    void add_block(const Block& add);
    void move_block(const short& x, const short& y);
    void fix_block();
    void rotate(const short& direction);
    void print_table(const int& x, const int& y); //print table on windows.h (x,y) is the origin of the table
    void set_level(const int& level);
    bool isValid();
    void send_garbage(); //part of the code depending on socket can wait
    void get_garbage();  //part of the code depending on socket can wait
};

void Table::del_block(){
    return;
}

void Table::add_block(const Block& add){
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
        matrix[p.y][p.x] = current.get_color();
    } 
}

void Table::rotate(const short& direction){
    current.rotate(direction);
    return;
}

void Table::print_table(const int& x, const int& y, HANDLE &hConsole){

    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD savedAttributes = consoleInfo.wAttributes;
    goto_xy(x, y, hConsole);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < wide; ++j) {
            /*if (board[i][j] == 'O') {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
            }
            else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
            }*/
            if (board[i][j] == 0) continue;
            color(board[i][j], hConsole);
            std::cout << board[i][j];
        }
        std::cout << std::endl;
    }

    SetConsoleTextAttribute(hConsole, savedAttributes);
    return;
};

void Table::print_block(HANDLE &hConsole) {
    std::vector<Point> p = current.block_position();
    short c = current.get_color();
    for (int i = 0; i < 4; i++) {
        goto_xy(p[i].y, p[i].x);
        color(c, hConsole);
        cout << currnt.get_symbol();
    }
}

void Table::set_level(const int& level) {
    this -> level = level;
    return;
}

bool Table::isValid() {

}

void goto_xy(int x, int y, HANDLE &hout) {
    COORD pos = { x,y };
    SetConsoleCursorPosition(hout, pos);
}

void color(const unsigned short textColor, HANDLE &hout)
{
    if (textColor >= 0 && textColor <= 15) {
        //参数在0-15的范围颜色
        SetConsoleTextAttribute(hout, textColor);
    }
    else {
        //默认的字体颜色是白色
        SetConsoleTextAttribute(hout, 7);
    }
}

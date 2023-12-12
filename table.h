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

//5-stage test for kick_table
//deal with different state (e.g 0>>1, 1>>0, etc.)
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
    //I block special case
    //assume the center point is on the right center
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
    bool isI, n_isI; //for kick table
    Block *current; // Change here
    std::queue<Block*> next;
    const static short height = 20, width = 10;
    short id[height][width]; //game id table
    int score = 0, clear_line = 0, level = 0;
    int arr, gravity, das, multiplier = 0, garbage = 0, B2B = 0;
    int x,y;
public:
    Table() : current(nullptr) { memset(id, 0, sizeof(id)); return; }
    ~Table() {}
    void set_position(int x, int y);
    //block existence
    void fix_block();
    void del_block();
    void add_block(Block& add);
    //block move
    void move_block(const short x, const short y);
    void rotate(const short direction);
    //printing
    void print_table(HANDLE &hConsole) const; //print table on windows.h (x,y) is the origin of the table
    void print_block(HANDLE &hConsole) const;
    //set level
    void set_level(const int level);
    //checking
    bool isValid(const Block& tmp) const;
    bool isT_Spin() const;
    //multi playing
    void getTable(); //part of the code depending on socket, only for opponent's table
    void send_garbage(); //part of the code depending on socket can wait
    void get_garbage();  //part of the code depending on socket can wait
    void pop_block();
};

void Table::set_position(int x, int y){
    this->x = x;
    this->y = y; 
}

//block existence
void Table::fix_block() {
    std::vector<Point> p = current->block_position(); // Change here
    for (auto i : p)
        id[i.y][i.x] = current->get_ID(); // Change here
}

void Table::del_block(){
    return;
}

void Table::add_block(Block &add){
    next.push(&add);// Change here
    return;
}

void Table::pop_block(){
    current = next.front(); // Change here
    next.pop();
}

//block move
void Table::move_block(const short x, const short y){
    Point pTmp(x, y);
    if(isValid(current->move(pTmp))) // Change here
        current->move_set(pTmp); // Change here
    return;
}

void Table::rotate(const short direction){
    Block tmp(*current); // Change here
    tmp.rotate_set(direction);
    for(int i = 0; i < 5; i++){
        if(isValid(tmp + Kick_Table(isI, tmp.get_direction(), direction, i)))
            *current = (tmp + Kick_Table(isI, tmp.get_direction(), direction, i)); // Change here
            break;
    }
    return;
}

//printing
void Table::print_table(HANDLE &hConsole) const{
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
            }
            else{
                 set_color(color_table[ id[i][j] ], hConsole);
                 std::cout << symbol_table[ id[i][j] ];
                 set_color(DEFAULT_COLOR, hConsole);
            }
        }
        std::cout << '|';
    }
    goto_xy(x, y + 20, hConsole);
    for(int i = 0;i < width + 2; i++)
        std::cout << '-';
    return;
};

void Table::print_block(HANDLE &hConsole) const{
    std::vector<Point> p;
    p = current->block_position(); // Change here
    short c = current->get_ID(); // Change here
    set_color(color_table[c], hConsole);
    for (int i = 0; i < 4; i++) {
        goto_xy(this->x+1+p[i].x, this->y+1+p[i].y, hConsole);
        std::cout << symbol_table[c];
        //std::cout << this->x+1+p[i].x << ' ' << this->y+1+p[i].y << ',';
    }
    std::cout << std::endl;
}

void Table::set_level(const int level) {
    this->level = level;
    return;
}

//checking
bool Table::isValid(const Block& tmp) const{
    return 1;
    std::vector<Point> p = tmp.block_position();
    for(auto i : p)
        if(id[i.y][i.x])
            return false;
    return true;
}

bool Table::isT_Spin() const {
    // Implement the logic for T-Spin check
    return false;
}

//multi playing
void Table::getTable(){
    // Implement logic for getting table in multiplayer
}

void Table::send_garbage(){
    // Implement logic for sending garbage in multiplayer
}

void Table::get_garbage(){
    // Implement logic for receiving garbage in multiplayer
}

#define DEFAULT_COLOR 7

void goto_xy(int x, int y, HANDLE &hout) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(hout, pos);
}

void set_color(const unsigned short textColor, HANDLE &hout) {
    if (textColor >= 0 && textColor <= 15)
        SetConsoleTextAttribute(hout, textColor);
    else
        SetConsoleTextAttribute(hout, DEFAULT_COLOR);
}

const short color_table[8] = {DEFAULT_COLOR, 1, 2, 3, 4, 5, 6, 9}; //[0] for none
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
    Block *current; // Change here
    std::queue<Block*> next;
    const static short height = 20, width = 10;
    short board[height][width]; //game id table
    short x, y;
    int score = 0, clear_line = 0, level = 0, multiplier = 0, garbage = 0, B2B = 0, combo = 0;
public:
    Table() : current(nullptr) { memset(board, 0, sizeof(board)); return;}
    ~Table() {}
    void set_position(const short x, const short y){this -> x = x, this -> y = y;}
    //block existence
    void fix_block();
    void add_block(Block* add) {next.push(add);}
    int getNext() {return next.size();}
    //block move
    void hard_drop();
    bool move_block(const short x, const short y);
    void rotate(const short direction);
    //printing
    void print_table(HANDLE &hConsole) const; //print table on windows.h (x,y) is the origin of the table
    void print_block(HANDLE &hConsole) const;
    //set level
    void set_level(const int level) {this -> level = level;}
    //checking
    bool isValid(const Block& tmp) const;
    bool isT_Spin() const;
    //filled check
    void set_clear();
    int removeLine();
    //return table data
    int get_score() const {return this -> score;}
    int get_line() const {return this -> clear_line;}
    int get_level() const {return this -> level;}
    //multi playing
    void getTable(); //part of the code depending on socket, only for opponent's table
    void send_garbage(); //part of the code depending on socket can wait
    void get_garbage();  //part of the code depending on socket can wait
    void pop_block();
    void cancelLine(); //cancel the whole line
    void isGameover();
};
//block existence
void Table::fix_block() {
    std::vector<Point> p = current -> block_position(); // Change here
    for (auto i : p)
        board[i.y][i.x] = current -> get_ID(); // Change here
}

void Table::pop_block(){
    delete current;
    current = next.front(); // Change here
    next.pop();
}

//block move
void Table::hard_drop(){
    Block *bTmp = current -> clone();
    for(int i = 20;i >= 0;i--){
        (*bTmp) += Point(0, -i);
        if(isValid(*bTmp)){
            (*current) += Point(0,-i);
            break;
        }
        (*bTmp) += Point(0, i);
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

//filled check
void Table::set_clear(){ //need to append b2b, t-spin, etc...
    multiplier = 1;
    this -> level += (removeLine() * multiplier);
}

int Table::removeLine()
{
	bool allExist;// see if the whole row is filled
	int cnt=0;// the total line num cleared
	for(int i=0; i<20; i++){
        allExist = true;
		for(int j=0; j<10; j++){
			if(!this->board[i][j]){
				allExist = false ;
				break ;
			}
		}
		if(allExist){
			cnt++;
			for(int p=i;p>=1; p--)
				for(int q=0; q<10; q++)
					this->board[p][q] = this->board[p-1][q];
            i--; //check the line that have cleared. because move down
		}
	}
    this -> clear_line += cnt;
    switch (cnt){
        case 1:{
            return 10;
        }
        case 2:{
            return 20;
        }
        case 3:{
            return 40;
        }
        case 4:{
            return 80;
        }
        default:{
            return 0;
        }
    }
}

//printing
void Table::print_table(HANDLE &hConsole) const{
    goto_xy(x, y, hConsole);
    set_color(DEFAULT_COLOR, hConsole);
    for(int i = 0;i < width + 2; i++) //row 0
        std::cout << '-';
    for (int i = 0; i < height; ++i) { //row 1-20
        goto_xy(x, y + i + 1, hConsole); //column 0
        std::cout << '|';
        for (int j = 0; j < width; ++j) { //column 1-10
            if (board[i][j] == 0)
                std::cout << ' ';
            else{
                 set_color(color_table[ board[i][j] ], hConsole);
                 std::cout << symbol_table[ board[i][j] ];
                 set_color(DEFAULT_COLOR, hConsole);
            }
        }
        std::cout << '|'; //column 11
    }
    goto_xy(x, y + 21, hConsole); //row 21
    for(int i = 0;i < width + 2; i++)
        std::cout << '-';
    return;
};

void Table::print_block(HANDLE &hConsole) const{
    short c = current->get_ID(); // Change here
    set_color(color_table[c], hConsole);
    for (auto i: current -> block_position()) {
        if(i.y <= 20){
            goto_xy(this->x + i.x + 1, this->y + 21 - i.y, hConsole);
            std::cout << symbol_table[c];
        }
    }
}

//checking
bool Table::isValid(const Block& tmp) const{
    for(auto i : tmp.block_position())
        if(i.x < 0 || i.x >= 10 || i.y < 0 || board[i.y][i.x])
            return 0;
    return 1;
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

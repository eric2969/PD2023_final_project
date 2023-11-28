class Table{
private:
    short matrix[10][23]; //game table
    int score = 0, clear_line = 0, level = 0;
    float multiplier = 0;
    Block current;
    int garbage = 0;
public:
    Table() {};
    ~Table() {};
    void del_block();
    void add_block(Block add);
    void move_block(const short x, const short y);
    void fix_block();
    void rotate(const short direction);
    void print_table(const int x, const int y); //print table on windows.h (x,y) is the origin of the table
    void set_level(int level);
    bool isValid();
    void send_garbage(); //part of the code depending on socket can wait
    void get_garbage();  //part of the code depending on socket can wait
};

void table::del_block(){
    return;
}

void table::add_block(Block add){
    current = add;
    return;
}

void Table::move_block(const short x, const short y){
    current.move(x, y);
    return;
}

void Table::fix_block() {

}

void Table::rotate(const short direction){
    current.rotate(direction);
    return;
}

void Table::print_table(const int x, const int y){

    return;
};

void Table::set_level(int level) {
    this->level = level;
    return;
}

bool Table::isValid() {

}

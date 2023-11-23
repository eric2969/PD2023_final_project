class table{
private:
    short matrix[10][23]={0}; //game table
    int score = 0, clear_line = 0, level = 0;
    block current;
public:
    table() {};
    ~table() {};
    void del_block();
    void add_block(block add);
    void move_block(const short x, const short y);
    void rotate(const short direction);
    void print_table(const int x, const int y); //print table on windows.h (x,y) is the origin of the table
};

void table::del_block(){
    return;
}

void table::add_block(block add){
    current = add;
    return;
}

void table::move_block(const short x, const short y){
    current.move(x, y);
    return;
}

void table::rotate(const short direction){
    current.rotate(direction);
    return;
}

void table::print_table(const int x, const int y){

    return;
};

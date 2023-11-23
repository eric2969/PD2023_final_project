class table{
private:
    short matrix[10][23]={0}; //game table
    int score = 0, clear_line = 0, level = 0;
public:
    table() {};
    ~table() {};
    void del_block();
    void add_block(block add);
    void print_table(const int x, const int y); //print table on windows.h (x,y) is the origin of the table
};

void table::del_block(){
    return;
}

void table::add_block(block add){
    return;

}

void table::print_table(const int x, const int y){

    return;
};

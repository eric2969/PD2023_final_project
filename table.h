class table{
private:
    short matrix[10][23]={0}; //game table
    int score = 0, clear_line = 0, level = 0;

public:
    table() {};
    ~table() {};
    void print_table(const int x, const int y);
};

void table::print_table(const int x, const int y){
    return;
}

#define ff first
#define ss second

class block{
private:
    short rotation = 0; //positive is counterclockwise
    short x, y, x_delta[4][4], y_delta[4][4]; //first index is rotation, second one is the number of blocks
public:
    block() {};
    ~block() {};
    void set_position(const short x,const short y){
        (this -> x) = x;
        (this -> y) = y;
        return;
    }
    void rotating(short direction){
        this -> rotation = (this -> rotation + direction) % 4;
        return;
    }
    std::vector< std::pair<short, short> > blocks_position(){
        std::vector< std::pair<short, short> > position(0);
        for(int i = 0;i < 4;i++){
            position.emplace_back(std::make_pair(this -> x + this-> x_delta[this -> rotation][i], this -> y  + this -> y_delta[this -> rotation][i]));
        }
        return position;
    }
};

#define ff first
#define ss second

class block{
private:
    short rotation = 0; //positive is counterclockwise
    pair<short,short> location; //first is x-coordinate, second is y-coordinate
    vector<short> x_delta, y_delta;
public:
    block() {};
    ~block() {};
    void set_position(const short x,const short y){
        (this -> location).ff = x;
        (this -> location).ss = y;
        return;
    }
    void rotating(short direction){
        this -> rotation = (this -> rotation + direction) % 4;
        return;
    }
};

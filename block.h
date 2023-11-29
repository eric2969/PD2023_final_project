class Table;

struct Point{
    short x,y;
    Point():x(0), y(0) {}
    Point(const short& X, const short& Y):x(X), y(Y) {}
    Point(const Point& tmp):x(tmp.x), y(tmp.y) {}
    Point operator+(const Point& tmp) const{ return Point((this -> x) + tmp.x, (this -> y) + tmp.y);}
    Point operator*(const short& tmp) const{ return Point((this -> x) * tmp, (this -> y) * tmp);}
    bool operator==(const Point& tmp) const{return (this->x == tmp.x) && (this->y == tmp.y);}
    const Point& operator=(const Point& tmp){
        this -> x = tmp.x;
        this -> y = tmp.y;
        return *this;
    }
    Point& operator[](const int& index){
        if(index < 0)
            exit(1);
        return this[index];
    }
};

const Point Kick_Point(const short& start, const short& drc, const short& test){
    const static Point delta[5] = { Point(0, 0), Point(-1, 0), Point(-1, 1), Point(0, -2), Point(-1,-2) };
    Point tmp; short factor;
    if(!test)
        return tmp;
    if(start % 2)
        factor = (start>>1)?1:-1;
    else
        factor = drc * ((start>>1)?-1:1);
    return delta[test] * factor;
}

class Block{
protected:
    char symbol;
    short color, direction, x_delta[4], y_delta[4];
    Point location;
public:
    Block() {}
    Block(const Point& p):location(p) {}
    Block(const char& sym, const short& colour):symbol(sym), color(colour) {}
    Block(const Point& p, const char& sym, const short& colour):location(p), symbol(sym), color(colour) {}
    Block(const Block& b):symbol(b.get_symbol()), color(b.get_color()), direction(b.get_direction()), location(b.get_location()){
        for(int i = 0;i < 4;i++)
            x_delta[i] = get_xdelta(i), y_delta[i] = get_ydelta(i);
        return;
    }
    ~Block() {}
    char get_symbol() const {return symbol;}
    short get_color() const {return color;}
    short get_direction() const {return direction;}
    short get_xdelta(const short& index) const{return x_delta[index];}
    short get_ydelta(const short& index) const{return y_delta[index];}
    Point get_location() const {return location;}
    void set_location(const short& x,const short& y){ (this -> location.x) = x, (this -> location.y) = y; return; }
    void move(const short& x, const short& y){ (this -> location.x) += x, (this -> location.y) += y; return; }
    const Block& operator=(const Block& tmp){
        this -> direction = tmp.get_direction();
        this -> location = tmp.get_location();
        this -> symbol = tmp.get_symbol();
        this -> color = tmp.get_color();
        for(int i = 0;i < 4;i++)
            (this -> x_delta[i]) = tmp.get_xdelta(i), (this -> y_delta[i]) = tmp.get_ydelta(i);
        return *this;
    }
    virtual void rotate(const short& drc){ //positive is clockwise
        this -> direction = ( (this -> direction) + drc + 4) % 4;
        short x_tmp, y_tmp;
        for(int i = 0;i < 4;i++){
            x_tmp = x_delta[i], y_tmp = y_delta[i];
            x_delta[i] = drc * x_tmp, y_delta[i] = drc * (-1) * y_tmp;
        }
        return;
    }
    virtual void rotate(const Table& Tb, const short& drc);
    virtual std::vector<Point> block_position() const{
        std::vector<Point> tmp;
        Point ptmp;
        for(int i = 0;i < 4;i++){
            ptmp.x = (this -> location).x + (this -> x_delta)[i];
            ptmp.y = (this -> location).y + (this -> y_delta)[i];
            tmp.emplace_back(ptmp);
        }
        return tmp;
    }
};

/*all of the symbol and color is for temporary use, and color and symbol will be confirmed and modified later
 *rotate of block_o is complete*/

class Block_T : public Block{
public:
    Block_T(const Point& p) : Block(p, '#', 0) {}
    ~Block_T() {} 
};

class Block_L : public Block{
public:
    Block_L(const Point& p) : Block(p, '$', 1) {}
    ~Block_L() {} 
};

class Block_J : public Block{
public:
    Block_J(const Point& p) : Block(p, '@', 2) {}
    ~Block_J() {} 
};

class Block_S : public Block{
public:
    Block_S(const Point& p) : Block(p, '%', 3) {}
    ~Block_S() {}
};

class Block_Z : public Block{
public:
    Block_Z(const Point& p) : Block(p, '?', 4) {}
    ~Block_Z() {}
};

class Block_I : public Block{
public:
    Block_I(const Point& p) : Block(p, '&', 5) {}
    ~Block_I() {}    
};

class Block_O : public Block{
public:
    Block_O(const Point& p) : Block(p, '+', 6) {}
    ~Block_O() {}
    void rotate(const short& direction) override{}
};

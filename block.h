struct Point{
    short x,y;
    Point():x(0), y(0) {}
    Point(const Point& tmp):x(tmp.x), y(tmp.y) {}
    bool operator==(const Point& tmp) const{
        return (this->x == tmp.x) && (this->y == tmp.y);
    }
    const Point& operator=(const Point& tmp){
        this -> x = tmp.x;
        this -> y = tmp.y;
        return *this;
    }
};

class Block{
protected:
    char symbol;
    short color, direction;
    Point location;
    short x_delta[4], y_delta[4];
public:
    Block() {}
    Block(const char& sym, const short& colour):symbol(sym), color(colour) {}
    Block(const Point& p, const char& sym, const short& colour):location(p), symbol(sym), color(colour) {}
    ~Block() {}
    char get_symbol() const {return symbol;}
    short get_color() const {return color;}
    short get_xdelta(const short index) const{return x_delta[index];}
    short get_ydelta(const short index) const{return y_delta[index];}
    Point get_location() const {return location;}
    void set_location(const short x,const short y){ (this -> location.x) = x, (this -> location.y) = y; return; }
    void move(const short x, const short y){ (this -> location.x) += x, (this -> location.y) += y; return; }
    const Block& operator=(const Block& tmp){
        this -> location = tmp.get_location();
        this -> symbol = tmp.get_symbol();
        this -> color = tmp.get_color();
        for(int i = 0;i < 4;i++)
            (this -> x_delta[i]) = tmp.get_xdelta(i), (this -> y_delta[i]) = tmp.get_ydelta(i);
        return *this;
    }
    virtual void rotate(const Table t, const short drc){ //positive is clockwise
        this -> direction = ( (this -> direction) + drc ) % 4;
        short x_tmp, y_tmp;
        for(int i = 0;i < 4;i++){
            x_tmp = x_delta[i], y_tmp = y_delta[i];
            x_delta[i] = drc * y_tmp, y_delta[i] = -1 * drc * x_tmp; 
        }
        return;
    }
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
    //virtual const Point* get_kick() const = 0;
};

/*all of the symbol and color is for temporary use, and color and symbol will be confirmed and modified later
 *rotate of block_o is complete*/

class Block_T : public Block{
public:
    Block_T(const Point& p) : Block(p, '#', 0) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 0, y_delta[1] = 1 ;
		x_delta[2] = 1, y_delta[2] = 0 ;
		x_delta[3] = -1, y_delta[3] = 0 ;
	}
    ~Block_T() {} 
};

class Block_L : public Block{
public:
    Block_L(const Point& p) : Block(p, '$', 1) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 1, y_delta[1] = 0 ;
		x_delta[2] = 1, y_delta[2] = 1 ;
		x_delta[3] = -1, y_delta[3] = 0 ;
	}
    ~Block_L() {} 
};

class Block_J : public Block{
public:
    Block_J(const Point& p) : Block(p, '@', 2) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 1, y_delta[1] = 0 ;
		x_delta[2] = -1, y_delta[2] = 0 ;
		x_delta[3] = -1, y_delta[3] = 1 ;
	}
    ~Block_J() {} 
};

class Block_S : public Block{
public:
    Block_S(const Point& p) : Block(p, '%', 3) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 0, y_delta[1] = 1 ;
		x_delta[2] = 1, y_delta[2] = 1 ;
		x_delta[3] = -1, y_delta[3] = 0 ;
	}
    ~Block_S() {}
};

class Block_Z : public Block{
public:
    Block_Z(const Point& p) : Block(p, '?', 4) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 0, y_delta[1] = 1 ;
		x_delta[2] = 1, y_delta[2] = 0 ;
		x_delta[3] = -1, y_delta[3] = 1 ;
	}
    ~Block_Z() {}
};

class Block_I : public Block{
public:
    Block_I(const Point& p) : Block(p, '&', 5) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 1, y_delta[1] = 0 ;
		x_delta[2] = 2, y_delta[2] = 0 ;
		x_delta[3] = -1, y_delta[3] = 0 ;
	}
    ~Block_I() {}    
};

class Block_O : public Block{
public:
    Block_O(const Point& p) : Block(p, '+', 6) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 1, y_delta[1] = 0 ;
		x_delta[2] = 0, y_delta[2] = -1 ;
		x_delta[3] = 1, y_delta[3] = -1 ;
	}
    ~Block_O() {}
    void rotate(const short direction) override{}
};

class Table;

struct Point{
    short x,y;
    Point():x(0), y(0) {}
    Point(const short& X, const short& Y):x(X), y(Y) {}
    Point(const Point& tmp):x(tmp.x), y(tmp.y) {}
    inline Point operator+(const Point& tmp) const{ return Point((this -> x) + tmp.x, (this -> y) + tmp.y);}
    inline Point operator*(const short& tmp) const{ return Point((this -> x) * tmp, (this -> y) * tmp);}
    inline bool operator==(const Point& tmp) const{return (this->x == tmp.x) && (this->y == tmp.y);}
    inline const Point& operator=(const Point& tmp){ this -> x = tmp.x, this -> y = tmp.y; return *this;}
};

class Block{
protected:
    short ID, direction, x_delta[4], y_delta[4];
    Point location;
public:
    //constructor and destructor
    Block() {}
    Block(const Point& p):location(p) {}
    Block(const short& id):ID(id) {}
    Block(const Point& p, const short& id):location(p), ID(id) {}
    Block(const Block& b):ID(b.get_ID()), direction(b.get_direction()), location(b.get_location()){
        for(int i = 0; i < 4; i++)
            x_delta[i] = get_xdelta(i), y_delta[i] = get_ydelta(i);
        return;
    }
    ~Block() {}
    //for get block data (const)
    inline short get_ID() const {return ID;}
    inline short get_direction() const {return direction;}
    inline short get_xdelta(const short& index) const{return x_delta[index];}
    inline short get_ydelta(const short& index) const{return y_delta[index];}
    inline Point get_location() const {return location;}
    virtual std::vector<Point> block_position() const{ //need to override block_I
        std::vector<Point> tmp;
        Point ptmp;
        for(int i = 0;i < 4;i++){
            ptmp.x = (this -> location).x + (this -> x_delta)[i];
            ptmp.y = (this -> location).y + (this -> y_delta)[i];
            tmp.emplace_back(ptmp);
        }
        return tmp;
    }
    //return pre-modified data (const)
    Block move(const short& x, const short& y) const{ Block pTmp(*this); pTmp += Point(x, y); return pTmp;}
    Block move(const Point& tmp) const{ Block pTmp(*this); pTmp += tmp; return pTmp;}
    Block operator+(const Point& tmp) const{ Block pTmp(*this); pTmp += tmp; return pTmp;}

    virtual Block rotate(const short& drc) const{ Block pTmp(*this); pTmp.rotate_set(drc); return *this;}
    //data setting
    inline void move_set(const Point& tmp){ (this -> location.x) += tmp.x, (this -> location.y) += tmp.y; return;}
    inline void move_set(const short& x, const short& y){ (this -> location.x) += x, (this -> location.y) += y; return;}
    const Block& operator+=(const Point& tmp){ (this -> location.x) += tmp.x, (this -> location.y) += tmp.y; return *this;}

    inline void set_location(const short& x,const short& y){ (this -> location.x) = x, (this -> location.y) = y; return;}
    inline void set_location(const Point& tmp){ (this -> location.x) = tmp.x, (this -> location.y) = tmp.y; return;}
    const Block& operator=(const Point& tmp){ (this -> location.x) = tmp.x, (this -> location.y) = tmp.y; return *this;}

    virtual void rotate_set(const short& drc){ //positive is clockwise
        this -> direction = ( (this -> direction) + drc + 4) % 4;
        short x_tmp, y_tmp;
        for(int i = 0; i < 4; i++){
            x_tmp = x_delta[i], y_tmp = y_delta[i];
            x_delta[i] = drc * x_tmp, y_delta[i] = drc * (-1) * y_tmp;
        }
        return;
    }
    const Block& operator=(const Block& tmp){
        this -> direction = tmp.get_direction();
        this -> location = tmp.get_location();
        this -> ID = tmp.get_ID();
        for(int i = 0; i < 4; i++)
            (this -> x_delta[i]) = tmp.get_xdelta(i), (this -> y_delta[i]) = tmp.get_ydelta(i);
        return *this;
    }

};

/*all of the symbol and color is for temporary use, and color and symbol will be confirmed and modified later
 *rotate of block_o is complete*/

class Block_T : public Block{
public:
    Block_T(const Point& p = Point(0,0) ) : Block(p, 1) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 0, y_delta[1] = 1 ;
		x_delta[2] = 1, y_delta[2] = 0 ;
		x_delta[3] = -1, y_delta[3] = 0 ;
	}
    ~Block_T() {}
};

class Block_L : public Block{
public:
    Block_L(const Point& p = Point(0,0) ) : Block(p, 2) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 1, y_delta[1] = 0 ;
		x_delta[2] = 1, y_delta[2] = 1 ;
		x_delta[3] = -1, y_delta[3] = 0 ;
	}
    ~Block_L() {}
};

class Block_J : public Block{
public:
    Block_J(const Point& p = Point(0,0) ) : Block(p, 3) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 1, y_delta[1] = 0 ;
		x_delta[2] = -1, y_delta[2] = 0 ;
		x_delta[3] = -1, y_delta[3] = 1 ;
	}
    ~Block_J() {}
};

class Block_S : public Block{
public:
    Block_S(const Point& p = Point(0,0) ) : Block(p, 4) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 0, y_delta[1] = 1 ;
		x_delta[2] = 1, y_delta[2] = 1 ;
		x_delta[3] = -1, y_delta[3] = 0 ;
	}
    ~Block_S() {}
};

class Block_Z : public Block{
public:
    Block_Z(const Point& p = Point(0,0) ) : Block(p, 5) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 0, y_delta[1] = 1 ;
		x_delta[2] = 1, y_delta[2] = 0 ;
		x_delta[3] = -1, y_delta[3] = 1 ;
	}
    ~Block_Z() {}
};

class Block_I : public Block{
public:
    Block_I(const Point& p = Point(0,0) ) : Block(p, 6) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 1, y_delta[1] = 0 ;
		x_delta[2] = 2, y_delta[2] = 0 ;
		x_delta[3] = -1, y_delta[3] = 0 ;
	}
    ~Block_I() {}
    Block rotate(const short& drc) const override{ Block pTmp(*this); return *this;}
    void rotate_set(const short& direction, int drc) override{
        short x_tmp, y_tmp;
        for(int i = 0; i < 4; i++){
            x_tmp = x_delta[i], y_tmp = y_delta[i]; //swap
            x_delta[i] = drc * x_tmp, y_delta[i] = drc * (-1) * y_tmp;
        }
        if (direction == 0){
            
        }
        this -> direction = ( (this -> direction) + drc + 4) % 4; //update direction
        
        return;
    }
};

class Block_O : public Block{
public:
    Block_O(const Point& p = Point(0,0) ) : Block(p, 7) {
		x_delta[0] = 0, y_delta[0] = 0 ;
		x_delta[1] = 1, y_delta[1] = 0 ;
		x_delta[2] = 0, y_delta[2] = -1 ;
		x_delta[3] = 1, y_delta[3] = -1 ;
	}
    ~Block_O() {}
    Block rotate(const short& drc) const override{ Block pTmp(*this); return *this;}
    void rotate_set(const short& direction) override{}
};

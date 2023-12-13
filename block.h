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
    virtual void createKickTable() = 0;
public:
    //constructor and destructor
    Block() {}
    Block(const Point& p):location(p) {}
    Block(const short& id):ID(id) {}
    Block(const Point& p, const short& id):location(p), ID(id) {}
    Block(const Block& b):ID(b.get_ID()), direction(b.get_direction()), location(b.get_location()){}
    virtual ~Block() {}
    //for get block data (const)
    inline short get_ID() const {return ID;}
    inline short get_direction() const {return direction;}
    inline Point get_location() const {return location;}
    virtual std::vector<Point> block_position() const{ //need to override block_I
        std::vector<Point> tmp;
        for(int i = 0;i < 4;i++){
            Point ptmp;
            ptmp.x = (this -> location).x + (this -> x_delta)[i];
            ptmp.y = (this -> location).y + (this -> y_delta)[i];
            tmp.push_back(ptmp); 
        }
        return tmp;
    }
    //return pre-modified data (const)
    Block *move(const short& x, const short& y) const{ Block* pTmp = this->clone(); *pTmp += Point(x, y); return pTmp;}
    Block *move(const Point& tmp) const{ Block* pTmp = this->clone(); *pTmp += tmp; return pTmp;}
    Block &operator+(const Point& tmp) const{ Block *pTmp = this->clone(); *pTmp += tmp; return *pTmp;}

    virtual Block* rotate(const short& drc) const{ Block* pTmp = this->clone(); pTmp->rotate_set(drc); return pTmp;}
    //data setting
    inline void move_set(const Point& tmp){ (this -> location.x) += tmp.x, (this -> location.y) += tmp.y; return;}
    inline void move_set(const short& x, const short& y){ (this -> location.x) += x, (this -> location.y) += y; return;}
    Block& operator+=(const Point& tmp){ (this -> location.x) += tmp.x, (this -> location.y) += tmp.y; return *this;}

    inline void set_location(const short& x,const short& y){ (this -> location.x) = x, (this -> location.y) = y; return;}
    inline void set_location(const Point& tmp){ (this -> location.x) = tmp.x, (this -> location.y) = tmp.y; return;}
    Block& operator=(const Point& tmp){ (this -> location.x) = tmp.x, (this -> location.y) = tmp.y; return *this;}
    virtual Block* clone() const = 0;

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
            (this -> x_delta[i]) = tmp.x_delta[i], (this -> y_delta[i]) = tmp.y_delta[i];
        return *this;
    }

};

/*all of the symbol and color is for temporary use, and color and symbol will be confirmed and modified later
 *rotate of block_o is complete*/

class Block_T : public Block{
private:
    inline void createKickTable(){
        x_delta[0] = this->location.x, y_delta[0] = this->location.y ; // (0, 0)
		x_delta[1] = this->location.x, y_delta[1] = this->location.y+1 ;// (0,01)
		x_delta[2] = this->location.x+1, y_delta[2] = this->location.y ;//(1, 0)
		x_delta[3] = this->location.x-1, y_delta[3] = this->location.y ;// (-1, 0)
    }
    
public:
    
    Block_T(const Point& p = Point(0,0) ) : Block(p, 1) {
		createKickTable();
	}
	Block_T(const Block &b) : Block(b){
	    createKickTable();
    }
	
    ~Block_T() {}
    Block* clone() const{
        return new Block_T(*this);
    }
};

class Block_L : public Block{
private:
    inline void createKickTable(){
        x_delta[0] = this->location.x, y_delta[0] = this->location.y ;// (0, 0)
		x_delta[1] = this->location.x+1, y_delta[1] = this->location.y ;// (1, 0)
		x_delta[2] = this->location.x+1, y_delta[2] = this->location.y+1 ;// (1, 1)
		x_delta[3] = this->location.x-1, y_delta[3] = this->location.y ;// (-1, 0)
    }
    Block* clone(){
        return new Block_L(*this);
    }
public:
    Block_L(const Point& p = Point(0,0) ) : Block(p, 2) {
		createKickTable();
	}
	Block_L(const Block &b) : Block(b){
	    createKickTable();
    }
    ~Block_L() {}
    Block* clone() const{
        return new Block_L(*this);
    }
};

class Block_J : public Block{
private:
    inline void createKickTable(){
        x_delta[0] = this->location.x, y_delta[0] = this->location.y ;// (0, 0)
		x_delta[1] = this->location.x+1, y_delta[1] = this->location.y ;// (1, 0)
		x_delta[2] = this->location.x-1, y_delta[2] = this->location.y ;// (-1, 0)
		x_delta[3] = this->location.x-1, y_delta[3] = this->location.y+1 ;// (-1, 1)
    }
    
public:
    Block_J(const Point& p = Point(0,0) ) : Block(p, 3) {
		createKickTable();
	}
	Block_J(const Block &b) : Block(b){
	    createKickTable();
    }
    ~Block_J() {}
    Block* clone() const{
        return new Block_J(*this);
    }
};

class Block_S : public Block{
private:
    inline void createKickTable(){
        x_delta[0] = this->location.x, y_delta[0] = this->location.y ;// (0, 0)
		x_delta[1] = this->location.x, y_delta[1] = this->location.y+1 ;// (0, 1)
		x_delta[2] = this->location.x+1, y_delta[2] = this->location.y+1 ;// (1, 1)
		x_delta[3] = this->location.x-1, y_delta[3] = this->location.y ;// (-1, 0)
    }
    
public:
    Block_S(const Point& p = Point(0,0) ) : Block(p, 4) {
		createKickTable();
	}
	Block_S(const Block &b) : Block(b){
	    createKickTable();
    }
    ~Block_S() {}
    Block* clone() const{
        return new Block_S(*this);
    }
};

class Block_Z : public Block{
private:
    inline void createKickTable(){
        x_delta[0] = this->location.x, y_delta[0] = this->location.y ;// (0, 0)
		x_delta[1] = this->location.x, y_delta[1] = this->location.y+1 ;// (0, 1)
		x_delta[2] = this->location.x+1, y_delta[2] = this->location.y ;// (1, 0)
		x_delta[3] = this->location.x-1, y_delta[3] = this->location.y+1 ;// (-1, 1)
    }
    
public:
    Block_Z(const Point& p = Point(0,0) ) : Block(p, 5) {
		createKickTable();
	}
	Block_Z(const Block &b) : Block(b){
	    createKickTable();
    }
	
    ~Block_Z() {}
    
    Block* clone() const{
        return new Block_Z(*this);
    }
};

class Block_I : public Block{
private:
    inline void createKickTable(){
        x_delta[0] = this->location.x, y_delta[0] = this->location.y ;// (0, 0)
		x_delta[1] = this->location.x+1, y_delta[1] = this->location.y ;// (1, 0)
		x_delta[2] = this->location.x+2, y_delta[2] = this->location.y ;// (2, 0)
		x_delta[3] = this->location.x-1, y_delta[3] = this->location.y ;// (-1, 0)
    }
    
public:
    Block_I(const Point& p = Point(0,0) ) : Block(p, 6) {
		createKickTable();
	}
	Block_I(const Block &b) : Block(b){
	    createKickTable();
    }
    ~Block_I() {}
    Block* rotate(const short& drc) const override{ Block* pTmp = this->clone(); return pTmp;}
    void rotate_set(const short& direction, int drc) {
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
    Block* clone() const{
        return new Block_I(*this);
    }
};

class Block_O : public Block{
private:
    inline void createKickTable(){
        x_delta[0] = this->location.x, y_delta[0] = this->location.y ;// (0, 0)
		x_delta[1] = this->location.x+1, y_delta[1] = this->location.y ;// (1, 0)
		x_delta[2] = this->location.x, y_delta[2] = this->location.y-1 ;// (0, -1)
		x_delta[3] = this->location.x+1, y_delta[3] = this->location.y-1 ;// (1, -1)
    }
    
public:
    Block_O(const Point& p = Point(0,0) ) : Block(p, 7) {
		createKickTable();
	}
	Block_O(const Block &b) : Block(b){
	    createKickTable();
    }
    ~Block_O() {}
    Block *rotate(const short& drc) const override{ Block* pTmp = this->clone(); return pTmp;}
    void rotate_set(const short& direction) override{}
    Block* clone() const{
        return new Block_O(*this);
    }
};

class Table;

struct Point{
    short x,y;
    Point() {}
    Point(const short& X, const short& Y):x(X), y(Y) {}
    Point(const Point& tmp):x(tmp.x), y(tmp.y) {}
    Point operator+(const Point& tmp) const{ return Point((this -> x) + tmp.x, (this -> y) + tmp.y);}
    Point operator*(const short& tmp) const{ return Point((this -> x) * tmp, (this -> y) * tmp);}
    bool operator==(const Point& tmp) const{ return (this->x == tmp.x) && (this->y == tmp.y);}
    bool operator!=(const Point& tmp) const{ return !(*this == tmp);}
    const Point& operator=(const Point& tmp){ this -> x = tmp.x, this -> y = tmp.y; return *this;}
};

class Block{
protected:
    short ID, direction;
    Point location, delta[4];
    virtual void createDeltaTable() = 0;
public:
    //constructor and destructor (should be invoked by their derived classes)
    Block() {}
    Block(const Point& p, const short& id):ID(id), direction(0), location(p) {}
    Block(const Block& b):ID(b.ID), direction(b.direction), location(b.location) {
        for(int i = 0;i < 4;i++)
            this -> delta[i] = b.delta[i];
    }
    virtual ~Block() {}
    //for get block data (const)
    inline short get_ID() const {return ID;}
    inline short get_direction() const {return direction;}
    virtual std::vector<Point> block_position() const{ //need to override block_I
        std::vector<Point> tmp;
        for(int i = 0;i < 4;i++)
            tmp.push_back(location + delta[i]);
        return tmp;
    }
    //data setting
    const Block& operator+=(const Point& tmp){ (this -> location.x) += tmp.x, (this -> location.y) += tmp.y; return *this;}
    const Block& operator=(const Point& tmp){ (this -> location.x) = tmp.x, (this -> location.y) = tmp.y; return *this;}
    virtual Block *clone() const = 0;
    virtual void rotate_set(const short& drc){ //positive is clockwise
        this -> direction = ( (this -> direction) + drc + 4) % 4;
        short x_tmp, y_tmp;
        for(int i = 0; i < 4; i++){
            x_tmp = delta[i].x, y_tmp = delta[i].y;
            delta[i].x = drc * y_tmp, delta[i].y = drc * (-1) * x_tmp;
        }
        return;
    }
    const Block& operator=(const Block& tmp){
        this -> direction = tmp.direction;
        this -> location = tmp.location;
        this -> ID = tmp.ID;
        for(int i = 0; i < 4; i++)
            (this -> delta[i]) = tmp.delta[i];
        return *this;
    }
    virtual bool isI() {return 0;}
    virtual bool is_same_position(Block *right){
        if(location != right->location) return false;
        for(int i=0;i<4;i++){
            if(delta[i] != right->delta[i])return false;
        }
        return true;
    }
};

/*all of the symbol and color is for temporary use, and color and symbol will be confirmed and modified later
 *rotate of block_o is complete*/

class Block_T : public Block{
private:
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(1, 0), delta[2] = Point(-1, 0), delta[3] = Point(0, 1);}
public:
    Block_T(const Point &p = Point(0,0) ) : Block(p, 1) {createDeltaTable();}
	Block_T(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_T() {}
    Block *clone() const{return new Block_T(*this);}
};

class Block_L : public Block{
private:
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(1, 0), delta[2] = Point(-1, 0), delta[3] = Point(1, 1);}
public:
    Block_L(const Point& p = Point(0,0) ) : Block(p, 2) {createDeltaTable();}
	Block_L(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_L() {}
    Block *clone() const{return new Block_L(*this);}
};

class Block_J : public Block{
private:
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(1, 0), delta[2] = Point(-1, 0), delta[3] = Point(-1, 1);}
public:
    Block_J(const Point& p = Point(0,0) ) : Block(p, 3) {createDeltaTable();}
	Block_J(const Block& b) : Block(b) {createDeltaTable();}
    ~Block_J() {}
    Block *clone() const{Block *bTmp = new Block_J((*this));return bTmp;}
};

class Block_S : public Block{
private:
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(0, 1), delta[2] = Point(1, 1), delta[3] = Point(-1, 0);}
public:
    Block_S(const Point& p = Point(0,0) ) : Block(p, 4) {createDeltaTable();}
	Block_S(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_S() {}
    Block *clone() const{return new Block_S(*this);}
};

class Block_Z : public Block{
private:
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(0, 1), delta[2] = Point(1, 0), delta[3] = Point(-1, 1);}
public:
    Block_Z(const Point& p = Point(0,0) ) : Block(p, 5) {createDeltaTable();}
	Block_Z(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_Z() {}
    Block *clone() const{return new Block_Z(*this);}
};

class Block_I : public Block{
private:
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(1, 0), delta[2] = Point(2, 0), delta[3] = Point(-1, 0);}
public:
    Block_I(const Point& p = Point(0,0) ) : Block(p, 6) {createDeltaTable();}
    Block_I(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_I() {}
    Block *clone() const{return new Block_I(*this);}
    bool isI() override {return 1;}
    void rotate_set(const short& drc) override{ //positive is clockwise
        short x_tmp, y_tmp;
        for(int i = 0; i < 4; i++){
            x_tmp = delta[i].x, y_tmp = delta[i].y;
            delta[i].x = drc * y_tmp, delta[i].y = drc * (-1) * x_tmp;
        }
        this -> direction = ( (this -> direction) + drc + 4) % 4;
        return;
    }
};

class Block_O : public Block{
private:
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(1, 0), delta[2] = Point(0, -1), delta[3] = Point(1, -1);}
public:
    Block_O(const Point& p = Point(0,0) ) : Block(p, 7) {createDeltaTable();}
	Block_O(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_O() {}
    Block *clone() const{return new Block_O(*this);}
    void rotate_set(const short& direction) override{}
};

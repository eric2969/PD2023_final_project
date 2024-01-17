#pragma once
//struct for saving coordinate data
struct Point{
    //for saving data for x and y coordinate
    short x,y;
    //constructors for different use
    Point():x(0), y(0) {}
    Point(const short& X, const short& Y):x(X), y(Y) {}
    Point(const Point& tmp):x(tmp.x), y(tmp.y) {}
    //operator for add two Point together by add x and y coordinate independently
    Point operator+(const Point& tmp) const{ return Point((this -> x) + tmp.x, (this -> y) + tmp.y);}
    //opertaor for multiply a number to a Point by multiply x and y with the number independently
    Point operator*(const short& tmp) const{ return Point((this -> x) * tmp, (this -> y) * tmp);}
    //check whether two Point is the same
    bool operator==(const Point& tmp) const{ return (this->x == tmp.x) && (this->y == tmp.y);}
    //check whether two Point is different
    bool operator!=(const Point& tmp) const{ return !(*this == tmp);}
    //assign Point data to this by copy x and y data independently
    const Point& operator=(const Point& tmp){ this -> x = tmp.x, this -> y = tmp.y; return *this;}
};
//for output a Point in format
ostream &operator<< (ostream &out,Point right){ out << '(' << right.x << ',' << right.y << ')';}

//Use for create a container for save block data on x-y plane
class Block{
protected:
    //for saving block general data
    short ID, direction;
    Point location, delta[4];
    //pure virtual function for create relative position of other 3 point from the origin point
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
    inline Point get_location() const {return location;}
    //return a vector that contain four points of the block
    std::vector<Point> block_position() const{
        std::vector<Point> tmp;
        for(int i = 0;i < 4;i++)
            tmp.push_back(location + delta[i]);
        return tmp;
    }
    //data setting (+= :  move the block by Point(x,y) on x-y plane, = : move the block position to Point(x, y))
    const Block& operator+=(const Point& tmp){ (this -> location.x) += tmp.x, (this -> location.y) += tmp.y; return *this;}
    const Block& operator=(const Point& tmp){ (this -> location.x) = tmp.x, (this -> location.y) = tmp.y; return *this;}
    //create a pure virtual function for cloning a Block that is identical to itself and return the Block's reference
    virtual Block *clone() const = 0;
    //rotate the block on x-y plane
    virtual void rotate_set(const short& drc){ //positive is clockwise
        //record the direction data
        this -> direction = ( (this -> direction) + drc + 4) % 4;
        short x_tmp, y_tmp;
        //rotate each point of the block by pi/2 or -pi/2 (the formula can be calculated by rotation matrix)
        for(int i = 0; i < 4; i++){
            x_tmp = delta[i].x, y_tmp = delta[i].y;
            delta[i].x = drc * y_tmp, delta[i].y = drc * (-1) * x_tmp;
        }
        return;
    }
    //an operator can copy all the data from source
    const Block& operator=(const Block& tmp){
        this -> direction = tmp.direction;
        this -> location = tmp.location;
        this -> ID = tmp.ID;
        for(int i = 0; i < 4; i++)
            (this -> delta[i]) = tmp.delta[i];
        return *this;
    }
    //a virtual function that return whether this block is I (will be override by Block_I)
    virtual bool isI() {return 0;}
    //check the block whether is at the same place as itself
    bool is_same_position(Block *right){if(!right) return 0; return (ID == right -> ID && location == right -> location && direction == right -> direction);}
};

class Block_T : public Block{
private:
    //create unique delta table for specific block
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(1, 0), delta[2] = Point(-1, 0), delta[3] = Point(0, 1);}
public:
    //constructor for creating a Block at specific position
    Block_T(const Point &p = Point(0,0) ) : Block(p, 1) {createDeltaTable();}
	//copy constructor
    Block_T(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_T() {}
    //a fn for returning a Block reference that its value is identical to itself
    Block *clone() const{return new Block_T(*this);}
};

class Block_L : public Block{
private:
    //create unique delta table for specific block
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(1, 0), delta[2] = Point(-1, 0), delta[3] = Point(1, 1);}
public:
    //constructor for creating a Block at specific position
    Block_L(const Point& p = Point(0,0) ) : Block(p, 2) {createDeltaTable();}
	//copy constructor
    Block_L(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_L() {}
    //a fn for returning a Block reference that its value is identical to itself
    Block *clone() const{return new Block_L(*this);}
};

class Block_J : public Block{
private:
    //create unique delta table for specific block
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(1, 0), delta[2] = Point(-1, 0), delta[3] = Point(-1, 1);}
public:
    //constructor for creating a Block at specific position
    Block_J(const Point& p = Point(0,0) ) : Block(p, 3) {createDeltaTable();}
	//copy constructor
    Block_J(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_J() {}
    //a fn for returning a Block reference that its value is identical to itself
    Block *clone() const{return new Block_J(*this);}
};

class Block_S : public Block{
private:
    //create unique delta table for specific block
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(0, 1), delta[2] = Point(1, 1), delta[3] = Point(-1, 0);}
public:
    //constructor for creating a Block at specific position
    Block_S(const Point& p = Point(0,0) ) : Block(p, 4) {createDeltaTable();}
	//copy constructor
    Block_S(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_S() {}
    //a fn for returning a Block reference that its value is identical to itself
    Block *clone() const{return new Block_S(*this);}
};

class Block_Z : public Block{
private:
    //create unique delta table for specific block
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(0, 1), delta[2] = Point(1, 0), delta[3] = Point(-1, 1);}
public:
    //constructor for creating a Block at specific position
    Block_Z(const Point& p = Point(0,0) ) : Block(p, 5) {createDeltaTable();}
	//copy constructor
    Block_Z(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_Z() {}
    //a fn for returning a Block reference that its value is identical to itself
    Block *clone() const{return new Block_Z(*this);}
};

class Block_I : public Block{
private:
    //create unique delta table for specific block
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(1, 0), delta[2] = Point(-2, 0), delta[3] = Point(-1, 0);}
public:
    //constructor for creating a Block at specific position
    Block_I(const Point& p = Point(0,0) ) : Block(p, 6) {createDeltaTable();}
    //copy constructor
    Block_I(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_I() {}
    //a fn for returning a Block reference that its value is identical to itself
    Block *clone() const{return new Block_I(*this);}
    //a function that return true to identify this block is I block
    bool isI() override {return 1;}
    //rotate the I block under SRS rule
    void rotate_set(const short& drc) override{ //positive is clockwise
        //rotate I block using (0, 0) as pivot (not center, thus we need fixing position) 
        short x_tmp, y_tmp;
        for(int i = 0; i < 4; i++){
            x_tmp = delta[i].x, y_tmp = delta[i].y;
            delta[i].x = drc * y_tmp, delta[i].y = drc * (-1) * x_tmp;
        }
        //fixing position
        short x_delta = (direction%2) * ((direction>>1)?1:-1), y_delta = (direction%2 - 1) * ((direction>>1)?-1:1);
        if(drc == 1)
            (*this) += Point(x_delta, y_delta);
        else
            (*this) += Point(y_delta, (-1) * x_delta);
        //update direction data
        this -> direction = ( (this -> direction) + drc + 4) % 4;
        return;
    }
};

class Block_O : public Block{
private:
    //create unique delta table for specific block
    void createDeltaTable() {delta[0] = Point(0, 0), delta[1] = Point(-1, 0), delta[2] = Point(-1, -1), delta[3] = Point(0, -1);}
public:
    //constructor for creating a Block at specific position
    Block_O(const Point& p = Point(0,0) ) : Block(p, 7) {createDeltaTable();}
	//copy constructor
    Block_O(const Block &b) : Block(b) {createDeltaTable();}
    ~Block_O() {}
    //a fn for returning a Block reference that its value is identical to itself
    Block *clone() const{return new Block_O(*this);}
    void rotate_set(const short& drc) override{this -> direction = ( (this -> direction) + drc + 4) % 4; return;}
};

struct Point{
    short x,y;
    bool operator==(const Point& tmp) const{
        return (this->x == tmp.x) && (this->y == tmp.y);
    }
    /*point& operator[](int index){
        if(index < 0)
            exit(1);
        return this[index];
    }*/
    const Point& operator=(const Point& tmp){
        this -> x = tmp.x;
        this -> y = tmp.y;
        return *this;
    }
};

class Block{
protected:
    Point location;
    int direction;
    short x_delta[4], y_delta[4];
public:
    virtual char get_symbol() = 0;
    Block(const Point& p) {
    	location = p ;
	}
    ~Block() {};
    const Block& operator=(const Block& tmp){
        this -> symbol = tmp.symbol;
        this -> location = tmp.location;
        for(int i=0; i<4; i++){
            this -> x_delta[i] = tmp.x_delta[i];
            this -> y_delta[i] = tmp.y_delta[i];
        }
        return *this;
    }
    void set_position(const short x,const short y){
        (this -> location.x) = x;
        (this -> location.y) = y;
        return;
    }
    void move(const short x, const short y){
        (this -> location.x) += x;
        (this -> location.y) += y;
        return;
    }
    virtual void rotate(const short direction){ //positive is counterclockwise
        short x_tmp, y_tmp;
        for(int i = 0;i < 4;i++){
            x_tmp = x_delta[i], y_tmp = y_delta[i];
            x_delta[i] = (direction>0?(-1):(1)) * y_tmp;
            y_delta[i] = (direction>0?(1):(-1)) * x_tmp; 
            //a
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
    virtual const Point* get_kick() const = 0;
};

class Block_T : public Block{
	public:
		Block_T(const Point& p) : Block(p) {} ;
		~Block_T() {} ;
};

class Block_L : public Block{
	public:
		Block_L(const Point& p) : Block(p) {} ;
		~Block_L() {} ;
};

class Block_J : public Block{
	public:
		Block_J(const Point& p) : Block(p) {} ;
		~Block_J() {} ;
};

class Block_S : public Block{
	
};

class block_Z : public block{
	
};

class block_I : public block{
	
};

class block_O : public block{
public:
	block_O(const point& p) : block(p) {} ;
	~block_O() {} ;
    void rotate(const short direction) override{
        return;
    }
};




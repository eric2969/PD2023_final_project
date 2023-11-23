struct point{
    short x,y;
    bool operator==(const point& tmp) const{
        return (this->x == tmp.x) && (this->y == tmp.y);
    }
    point& operator[](int index){
        if(index < 0)
            exit(1);
        return this[index];
    }
    const point& operator=(const point& tmp){
        this -> x = tmp.x;
        this -> y = tmp.y;
        return *this;
    }
};

class block{
private:
    char symbol;
    point location;
protected:
    short x_delta[4], y_delta[4];
public:
    block() {};
    ~block() {};
    const block& operator=(const block& tmp){
        this -> symbol = tmp.symbol;
        this -> location = tmp.location;
        for(int i = 0;i < 4;i++){
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
    virtual std::vector<point> block_position() const{
        std::vector<point> tmp;
        point ptmp;
        for(int i = 0;i < 4;i++){
            ptmp.x = (this -> location).x + (this -> x_delta)[i];
            ptmp.y = (this -> location).y + (this -> y_delta)[i];
            tmp.emplace_back(ptmp);
        }
        return tmp;
    }
};

class block_T : public block{
		
};

class block_L : public block{
	
};

class block_J : public block{
	
};

class block_S : public block{
	
};

class block_Z : public block{
	
};

class block_I : public block{
	
};

class block_O : public block{
public:
    virtual void rotate(const short direction) override{
        return;
    }
};



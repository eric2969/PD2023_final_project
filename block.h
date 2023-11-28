struct point{
    short x,y;
    bool operator==(const point& tmp) const{
        return (this->x == tmp.x) && (this->y == tmp.y);
    }
    /*point& operator[](int index){
        if(index < 0)
            exit(1);
        return this[index];
    }*/
    const point& operator=(const point& tmp){
        this -> x = tmp.x;
        this -> y = tmp.y;
        return *this;
    }
};

class block{
protected:
    char symbol;
    short color;
    point location;
    short x_delta[4], y_delta[4];
public:
    block() {}
    block(const char& sym, const short& colour):symbol(sym), color(colour) {}
    ~block() {};
    short get_color() const {return color;}
    char get_symbol() const {return symbol;}
    point get_location() const {return location;}
    short get_xdelta(const short index) const{return x_delta[index];}
    short get_ydelta(const short index) const{return y_delta[index];}
    void set_location(const short x,const short y){ (this -> location.x) = x, (this -> location.y) = y; return; }
    void move(const short x, const short y){ (this -> location.x) += x, (this -> location.y) += y; return; }
    const block& operator=(const block& tmp){
        this -> location = tmp.get_location();
        this -> symbol = tmp.get_symbol();
        this -> color = tmp.get_color();
        for(int i=0; i<4; i++)
            (this -> x_delta[i]) = tmp.get_xdelta(i), (this -> y_delta[i]) = tmp.get_ydelta(i);
        return *this;
    }
    virtual void rotate(const short direction){ //positive is counterclockwise
        short x_tmp, y_tmp;
        for(int i = 0;i < 4;i++){
            x_tmp = x_delta[i], y_tmp = y_delta[i];
            x_delta[i] = (direction>0?(-1):(1)) * y_tmp;
            y_delta[i] = (direction>0?(1):(-1)) * x_tmp; 
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
    //virtual const point* get_kick() const = 0;
};

/*all of the symbol and color is for temporary use, and color and symbol will be confirmed and modified later
 *rotate of block_o is complete*/
class block_T : public block{
protected:
public:
    block_T():block('#', 0) {}
    ~block_T() {}
};

class block_L : public block{
protected:
public:
    block_L():block('$', 1) {}
    ~block_L() {}
};

class block_J : public block{
protected:
public:
    block_J():block('@', 2) {}
    ~block_J() {}
};

class block_S : public block{
protected:
public:
    block_S():block('%', 3) {}
    ~block_S() {}
};

class block_Z : public block{
protected:
public:
    block_Z():block('?', 4) {}
    ~block_Z() {}
};

class block_I : public block{
protected:
public:
    block_I():block('&', 5) {}
    ~block_I() {}
};

class block_O : public block{
protected:
public:
    block_O():block('+', 6) {}
    ~block_O() {}
    void rotate(const short direction) {return;}
};




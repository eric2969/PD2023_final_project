#ifndef MENU_H
#define MENU_H


//dwButtonState 鼠?按???

#define L_BUTTON 0x1
#define R_BUTTON 0x2
 
// dwEventFlags 鼠?事件??
#define MOUSE_CLICK 0x0
#define MOUSE_MOVED 0x1
#define DOUBLE_CLICK 0x2
 
//?色
#define COLOR_default SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
#define COLOR_Black_Cyan SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
#define COLOR_Yellow_Blue SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xe9);
 
// 移?光?位置
void setcursor(const COORD &temp);
void setcursor(short x, short y);
void hidecursor(bool hide = true);
void setmode();
void clean();
void clrscr();

class Menu
{
private:
    struct node
    {
        COORD pos_;
        std::string display_;
        const std::function<void(void)> &pf_;
        node(std::string d, const std::function<void(void)> &func) : display_(d), pf_(func){};
    };
    std::vector<node> nodes_;
    std::string title_;
 
protected:
    void recordposition();
    bool implement(COORD clickpos);
    void highlight(COORD hang);
 
public:
    //constructor
    Menu(){} 
    ~Menu(){}
    //methods
    Menu &settitle(std::string s);
    Menu &add(const std::function<void(void)> &p, std::string d);
    void start();
};

#endif

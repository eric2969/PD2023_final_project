#ifndef MENU_H
#define MENU_H


//dwButtonState for the mouse

#define L_BUTTON 0x1
#define R_BUTTON 0x2
 
// dwEventFlags for the mouse
#define MOUSE_CLICK 0x0
#define MOUSE_MOVED 0x1
#define DOUBLE_CLICK 0x2
 
//define colors
#define COLOR_default SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
#define COLOR_Black_Cyan SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
#define COLOR_Yellow_Blue SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xe9);
 
using std::ios_base;
using std::ostream;
using std::string;
using std::vector;
using std::cout;
using std::endl;

class Menu
{
private:
    struct node //menu's options
    {
        COORD pos_;
        std::string display_;
        const std::function<void(void)> &pf_;
        node(std::string d, const std::function<void(void)> &func) : display_(d), pf_(func){};
    };
    std::vector<node> nodes_;
    std::string title_; //title of the menu
 
protected:
    void recordposition(); //record the position of the mouse
    bool implement(COORD clickpos); //execute the selected option
    void highlight(COORD hang); //make the hovered option blue
 
public:
    //constructor
    Menu(){} 
    ~Menu(){}
    //methods
    Menu &settitle(std::string s);
    //add an option to the menu
    Menu &add(const std::function<void(void)> &p, std::string d);
    //run the menu
    void start();
};


//print cursor's location
ostream &operator<<(ostream &pout, const COORD &temp)
{
    pout.setf(ios_base::fixed);
    pout << "[Cursor Position] X: " << std::setw(2) << temp.X << "  Y: " << std::setw(2) << temp.Y;
    return pout;
}
 

//button - click
//check if cursor's location is in the affecting region (Same row and same column + 5 tiles)
bool operator-(const COORD &button, const COORD &click)
{
    if (button.Y == click.Y && button.X <= click.X && click.X <= button.X + 20)
        return true;
    else
        return false;
};
 
// reset the console mode
void setmode()
{
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE; //disable fast update mode
    mode &= ~ENABLE_INSERT_MODE;     //disable insert mode
    // mode &= ~ENABLE_MOUSE_INPUT;     //disable mouse input
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}
 
//wait before the mouse moves
MOUSE_EVENT_RECORD waitmouse(bool move = true)
{
    INPUT_RECORD record; //?入事件
    DWORD reg;           //??寄存
    while (1)
    {
        Sleep(10);
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &reg);                                  //??入事件存入record
        if (record.EventType == MOUSE_EVENT && (move | record.Event.MouseEvent.dwEventFlags != MOUSE_MOVED)) //是鼠?事件 && 移?事件与模式??
            return record.Event.MouseEvent;
    }
}

void Menu::recordposition(){
    for (auto p = nodes_.begin(); p != nodes_.end(); p++)
    {
        getcursor(p->pos_);
        cout << p->display_ << "\n\n";
    }
}
//execute
bool Menu::implement(COORD clickpos){
    for (auto p = nodes_.begin(); p != nodes_.end(); p++)
    {
        if (p->pos_ - clickpos) //if clicked at the right position
        {
            hidecursor(0);
            clean();
            try{p->pf_();}
            catch(std::runtime_error e){
                set_color(0);
                clrscr();
                set_color(7);
                cout << e.what() << endl;
                Sleep(2000);
                pause();
                
            }
            setmode();
            hidecursor(1);
            return true;
        }
    }
    return false;
}
void Menu::highlight(COORD hang){
    COLOR_default;
    clean();
    cout << title_ << endl //show title
         << "========================================" << endl;
    for (auto p = nodes_.begin(); p != nodes_.end(); p++)
    {
        if (p->pos_ - hang) //hovering
        {
            COLOR_Black_Cyan;
            cout << p->display_ << "\n\n";
            COLOR_default;
        }
        else
            cout << p->display_ << "\n\n";
    }
}

Menu& Menu::settitle(string s){
    title_ = s;
    return *this;
}
Menu& Menu::add(const std::function<void(void)> &p, string d = "Unkown."){
    nodes_.push_back(node(d, p));
    return *this;
}
void Menu::start(){
    setmode();
    MOUSE_EVENT_RECORD mouse;
    hidecursor(1);
    clean();
    cout << title_ << endl //show title
         << "========================================" << endl;
    recordposition(); //record mouse's position
    do
    {
        Sleep(10); //update ticks
        mouse = waitmouse();
        if (mouse.dwEventFlags == MOUSE_MOVED)
            highlight(mouse.dwMousePosition);
        else if (mouse.dwButtonState == L_BUTTON){
            implement(mouse.dwMousePosition);
            clrscr();
            highlight(mouse.dwMousePosition);
        }
            
    } while (mouse.dwButtonState != R_BUTTON);
    while(mouse.dwButtonState == R_BUTTON){
        mouse = waitmouse();
        Sleep(10);
    }
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}
#endif

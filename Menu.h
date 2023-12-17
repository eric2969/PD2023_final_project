#ifndef MENU_H
#define MENU_H


//dwButtonState ��?��???

#define L_BUTTON 0x1
#define R_BUTTON 0x2
 
// dwEventFlags ��?�ƥ�??
#define MOUSE_CLICK 0x0
#define MOUSE_MOVED 0x1
#define DOUBLE_CLICK 0x2
 
//?��
#define COLOR_default SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
#define COLOR_Black_Cyan SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0b);
#define COLOR_Yellow_Blue SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xe9);
 
using std::ios_base;
using std::ostream;
using std::string;
using std::vector;
using std::cout;
using std::endl;
 
// ��?��?��m
//void setcursor(const COORD &temp);
//void setcursor(short x, short y);
//void hidecursor(bool hide = true);
//void setmode();
//void clean();
//void clrscr();

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


//?�X��?��?
ostream &operator<<(ostream &pout, const COORD &temp)
{
    pout.setf(ios_base::fixed);
    pout << "[Cursor Position] X: " << std::setw(2) << temp.X << "  Y: " << std::setw(2) << temp.Y;
    return pout;
}
 

//button - click
//�P?��???��m�O�_�b���w�����ĭS?�]����A���C��5�C��?�^
bool operator-(const COORD &button, const COORD &click)
{
    if (button.Y == click.Y && button.X <= click.X && click.X <= button.X + 20)
        return true;
    else
        return false;
};
 
// ��ﱱ��x?�m�A�����x?��?�P���P�w��?
void setmode()
{
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE; //�����ֳt??�Ҧ�
    mode &= ~ENABLE_INSERT_MODE;     //�������J�Ҧ�
    // mode &= ~ENABLE_MOUSE_INPUT;     //������??�J
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}
 
//���ݹ�?�ƥ�A�i�ư���?��?
MOUSE_EVENT_RECORD waitmouse(bool move = true)
{
    INPUT_RECORD record; //?�J�ƥ�
    DWORD reg;           //??�H�s
    while (1)
    {
        Sleep(10);
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &reg);                                  //??�J�ƥ�s�Jrecord
        if (record.EventType == MOUSE_EVENT && (move | record.Event.MouseEvent.dwEventFlags != MOUSE_MOVED)) //�O��?�ƥ� && ��?�ƥ��O�Ҧ�??
            return record.Event.MouseEvent;
    }
}
// �M�̨�?

void Menu::recordposition(){
    for (auto p = nodes_.begin(); p != nodes_.end(); p++)
    {
        getcursor(p->pos_);
        cout << p->display_ << "\n\n";
    }
}
bool Menu::implement(COORD clickpos){
    for (auto p = nodes_.begin(); p != nodes_.end(); p++)
    {
        if (p->pos_ - clickpos) //??��
        {
            hidecursor(0);
            clean();
            try{p->pf_();}
            catch(std::runtime_error e){
                set_color(0);
                clrscr();
                set_color(7);
                cout << e.what() << endl;
            }
            //setmode();
            hidecursor(1);
            return true;
        }
    }
    return false;
}
void Menu::highlight(COORD hang){
    COLOR_default;
    clean();
    cout << title_ << endl //?��?�e
         << "========================================" << endl;
    for (auto p = nodes_.begin(); p != nodes_.end(); p++)
    {
        if (p->pos_ - hang) //?��?
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
    cout << title_ << endl //?��?�e
         << "========================================" << endl;
    recordposition(); //??��m
    do
    {
        Sleep(10); //?�֦�?
        mouse = waitmouse();
        if (mouse.dwEventFlags == MOUSE_MOVED)
            highlight(mouse.dwMousePosition);
        else if (mouse.dwButtonState == L_BUTTON)
            implement(mouse.dwMousePosition);
    } while (mouse.dwButtonState != R_BUTTON);
    Sleep(100); //??
}
#endif

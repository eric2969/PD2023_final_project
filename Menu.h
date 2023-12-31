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

using std::ostream;
using std::string;
using std::vector;
using std::cout;
using std::endl;

struct node { //menu's options
    COORD pos_;
    std::string display_;
    const std::function<void(void)> &pf_;
    node(std::string d, const std::function<void(void)> &func) : display_(d), pf_(func){};
};

class Menu
{
private:
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
    //menu init
    void init();
    //methods
    Menu &settitle(std::string s);
    //add an option to the menu
    Menu &add(const std::function<void(void)> &p, std::string d);
    //run the menu
    void start();
};

void Menu::init(){
    (this -> nodes_).clear();
    title_ = "";
}

//print cursor's location
ostream &operator<<(ostream &pout, const COORD &temp){
    pout.setf(ios_base::fixed);
    pout << "[Cursor Position] X: " << std::setw(2) << temp.X << "  Y: " << std::setw(2) << temp.Y;
    return pout;
}

//button - click
//check if cursor's location is in the affecting region (Same row and same column + 5 tiles)
bool operator-(const COORD &button, const COORD &click) {return (button.Y == click.Y && button.X <= click.X && click.X <= button.X + 20);}
// reset the console mode
void setmode(){
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE; //disable fast update mode
    mode &= ~ENABLE_INSERT_MODE;     //disable insert mode
    // mode &= ~ENABLE_MOUSE_INPUT;     //disable mouse input
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}

//wait before the mouse moves
MOUSE_EVENT_RECORD waitmouse(bool move = true){
    INPUT_RECORD record;
    DWORD reg;
    while (1){
        Sleep(10);
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &reg);
        if (record.EventType == MOUSE_EVENT && (move | record.Event.MouseEvent.dwEventFlags != MOUSE_MOVED))
            return record.Event.MouseEvent;
    }
}

void Menu::recordposition(){
    for (auto p = nodes_.begin(); p != nodes_.end(); p++){
        getcursor(p->pos_);
        cout << p->display_ << "\n\n";
    }
}
//execute
bool Menu::implement(COORD clickpos){
    for (auto p = nodes_.begin(); p != nodes_.end(); p++){
        if (p->pos_ - clickpos){ //if clicked at the right position
            hidecursor(0);
            clean();
            p->pf_();
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
    cout << title_ << "\n========================================\n"; //show title
    for (auto p = nodes_.begin(); p != nodes_.end(); p++){
        if (p->pos_ - hang) { //hovering
            COLOR_Black_Cyan;
            cout << p->display_ << "\n\n";
            COLOR_default;
        }
        else
            cout << p->display_ << "\n\n";
    }
}

Menu& Menu::settitle(string s){title_ = s; return *this;}
Menu& Menu::add(const std::function<void(void)> &p, string d){ nodes_.push_back(node(d, p)); return *this;}

void Menu::start(){
    setmode();
    MOUSE_EVENT_RECORD mouse;
    clrscr();
    hidecursor(1);
    COLOR_default;
    cout << title_ << "\n========================================\n"; //show title
    recordposition(); //record mouse's position
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    do{
        Sleep(flush_tick); //update ticks
        mouse = waitmouse();
        if (mouse.dwEventFlags == MOUSE_MOVED) //if mouse moves
            highlight(mouse.dwMousePosition);
        else if (mouse.dwButtonState == L_BUTTON){
            implement(mouse.dwMousePosition);
            clrscr();
            highlight(mouse.dwMousePosition);
        }
    }while (mouse.dwButtonState != R_BUTTON);
    while(mouse.dwButtonState == R_BUTTON){ //prevent the clicking affect the other page
        mouse = waitmouse();
        Sleep(10);
    }
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //prevent the clicking affect the other page
}



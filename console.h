void setcursor(short x = 0, short y = 0){
    COORD temp = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
}
//void goto_xy(int x, int y, HANDLE &hout) {COORD pos = {x, y}; SetConsoleCursorPosition(hout, pos);}
//void set_color(const unsigned short textColor, HANDLE &hout) {SetConsoleTextAttribute(hout, textColor);}
void setcursor(const COORD &temp){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
}
 
// ?得光?位置
void getcursor(COORD &other)
{
    CONSOLE_SCREEN_BUFFER_INFO temp;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &temp);
    other = temp.dwCursorPosition;
}
COORD getcursor()
{
    CONSOLE_SCREEN_BUFFER_INFO temp;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &temp);
    return temp.dwCursorPosition;
}
//是否?藏光?
void hidecursor(bool hide = true)
{
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
    CursorInfo.bVisible = !hide;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}
 
 void clean()
{
    //HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    //set_color(0,hout);
    COORD temp = getcursor();
    setcursor(0, 0);
    for (int i = 0; i <= temp.Y; i++)
        std::cout << "                                                                                                                                                                                                                                                                  \n";
    setcursor(0, 0);
    //system("cls");
}

void clrscr(){
    setcursor(0, 0);
    hidecursor(0);
    for (int i = 0; i <= 30; i++)
        std::cout << "                                                                                                                                       \n";
    setcursor(0, 0);
    hidecursor(1);
}
//?停
void pause(){
    std::cout << "?按任意???\n";
    std::cin.sync();
    std::cin.get();
}

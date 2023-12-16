#pragma comment(lib, "User32.lib")
#include <iostream>
#include <cstring>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <stdlib.h>
#include <functional>

#include "Menu.h"

using std::ios_base;
using std::ostream;
using std::string;
using std::vector;
using std::cout;
using std::endl;

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
//?出光?坐?
ostream &operator<<(ostream &pout, const COORD &temp)
{
    pout.setf(ios_base::fixed);
    pout << "[Cursor Position] X: " << std::setw(2) << temp.X << "  Y: " << std::setw(2) << temp.Y;
    return pout;
}
 
//是否?藏光?
void hidecursor(bool hide)
{
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
    CursorInfo.bVisible = !hide;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}
 
//button - click
//判?鼠???位置是否在指定的有效范?（本行，本列至5列之?）
bool operator-(const COORD &button, const COORD &click)
{
    if (button.Y == click.Y && button.X <= click.X && click.X <= button.X + 20)
        return true;
    else
        return false;
};
 
// 更改控制台?置，防止控制台?性?致的判定失?
void setmode()
{
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE; //移除快速??模式
    mode &= ~ENABLE_INSERT_MODE;     //移除插入模式
    // mode &= ~ENABLE_MOUSE_INPUT;     //移除鼠??入
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}
 
//等待鼠?事件，可排除鼠?移?
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
// 清屏函?
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
        if (p->pos_ - clickpos) //??成
        {
            hidecursor(0);
            clean();
            p->pf_();
            hidecursor(1);
            return true;
        }
    }
    return false;
}
void Menu::highlight(COORD hang){
    COLOR_default;
    clean();
    cout << title_ << endl //?示?容
         << "========================================" << endl;
    for (auto p = nodes_.begin(); p != nodes_.end(); p++)
    {
        if (p->pos_ - hang) //?停?
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
    cout << title_ << endl //?示?容
         << "========================================" << endl;
    recordposition(); //??位置
    do
    {
        Sleep(10); //?少次?
        mouse = waitmouse();
        if (mouse.dwEventFlags == MOUSE_MOVED)
            highlight(mouse.dwMousePosition);
        else if (mouse.dwButtonState == L_BUTTON)
            implement(mouse.dwMousePosition);
    } while (mouse.dwButtonState != R_BUTTON);
    Sleep(100); //??
}

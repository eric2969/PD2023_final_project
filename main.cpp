#pragma comment(lib, "User32.lib") //User32.lib library for event listener
#include <bits/stdc++.h>

#pragma comment (lib ,"imm32.lib") 
#include <windows.h>
#define _WIN32_WINNT 0x0601
#ifndef DEV 
#include <imm.h>
#include <immdev.h>  
typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT  FontFamily;
    UINT  FontWeight;
    WCHAR FaceName[LF_FACESIZE];
}CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;
#endif
//the function declaration begins
#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX
lpConsoleCurrentFontEx);
#ifdef __cplusplus
}
#endif

HANDLE hConsole;
bool bright;
int das, gravity;
const short flush_tick = 2;

#include "console.h"
#include "block.h"
#include "table.h"
#include "VK.h"
#include "game.h"
#include "Menu.h"

//#include "server.h"
//#include "client.h"

#define FONT
using namespace std;

#define SET_PATH "src/settings.txt"
#define RECORD_PATH "src/records.txt"

int playTimes, clearCnt, scoreCnt, highClear, highScore;

void game_init();
void game_exit();
void record_reset() {playTimes = 0, clearCnt = 0, scoreCnt = 0, highClear = 0, highScore = 0;}
void record_update(int& clr, int& score);

#ifdef FONT
void SetFont(int);
#endif

struct option1{
    int line, score;
    void operator() (){
         SetFont(25); 
         singlePlayer(line, score);
         record_update(line, score);
    }
};
struct option2{
    int line, score;
    void operator() (){
         SetFont(25); 
         multiPlayer(line, score);
         record_update(line, score);
    }
};
struct option3{
    void operator() (){
        set_color(7);
        setcursor(0,0);
        cout << playTimes << ' ' << clearCnt << ' ' << scoreCnt << ' ' << highClear << ' ' << highScore;
        setcursor(0,0);
        hidecursor();
        set_color(0);
        Sleep(2000);
        clrscr();
    }
};
struct option4{
    void operator() (){
        game_exit();
    }
};


signed main(){
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    FullScreen();
#ifdef FONT
    SetFont(25);
#endif
    game_init();
    Menu loop;
    string s = "Tetris";
    option1 opt1;
    option2 opt2;
    option3 opt3;
    option4 opt4;
    loop.settitle(s);
    loop.add(opt1, "Single Player").add(opt2, "Multi Player").add(opt3, "Settings").add(opt4, "Quit");
    loop.start();
    return 0;
}

void game_init(){
    ifstream setting(SET_PATH), record(RECORD_PATH);
    setting >> das >> gravity >> bright;
    record >> playTimes >> clearCnt >> scoreCnt >> highClear >> highScore;
    setting.close();
    record.close();
}

void game_exit(){
    ofstream setting(SET_PATH), record(RECORD_PATH);
    setting << das << ' ' << gravity << ' ' << bright;
    record << playTimes << ' ' << clearCnt << ' ' << scoreCnt << ' ' << highClear << ' ' << highScore;
    setting.close();
    record.close();
    exit(0);
}

void record_update(int& clr, int& score){
    playTimes++;
    clearCnt += clr;
    scoreCnt += score;
    highClear = max(highClear, clr);
    highScore = max(highScore, score);
}

#ifdef FONT
void SetFont(int size = 30) {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = size;
	cfi.dwFontSize.Y = size;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}
#endif // FONT

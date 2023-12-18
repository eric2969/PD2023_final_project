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
int das, arr, gravity;
const short flush_tick = 2;

#define DEBUG

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
#define PIC_PATH "src/pic.txt"
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
    static int line, score;
    static int gameMode;
    static void run(){
        try{
             singlePlayer(line, score);
         }
         catch(runtime_error e){
                set_color(0);
                clrscr();
                set_color(7);
                cout << e.what() << endl;
                Sleep(2000);
                pause();
         }
         record_update(line, score);
    }
    static void sub_option1(){
         gameMode = 1;
         run();
     }
    static void sub_option2(){
         gameMode = 2;
         run();
     }
    static void sub_option3(){
         gameMode = 3;
         run();
    }
    void operator() (){
         Menu sub_menu;
         sub_menu.settitle("Game Mode");
         sub_menu.add(sub_option1, "Mode 1").add(sub_option2, "Mode 2").add(sub_option3, "Mode 3");
         sub_menu.start();
    }
};
int option1::gameMode = 0;
int option1::line = 0;
int option1::score = 0;
struct option2{
    int line, score;
    void operator() (){
         SetFont(25); 
         try{
             multiPlayer(line, score);
         }
         catch(runtime_error e){
                set_color(0);
                clrscr();
                set_color(7);
                cout << e.what() << endl;
                Sleep(2000);
                pause();
         }
         record_update(line, score);
    }
};
struct option3{
    static void sub_option1(){
         cout << "Current ARR is: " << arr <<"\nPlease Type in the New ARR: ";
         cin >> arr;
     }
    static void sub_option2(){
         cout << "Current DAS is: " << das <<"\nPlease Type in the New DAS: ";
         cin >> das;
     }
    static void sub_option3(){
         cout << "Current Gravity Level is: " << gravity <<"\nPlease Type in the New Gravity Level: ";
         cin >> gravity;
    }
    static void sub_option4(){
         cout << "Current Bright Mode is: " << bright <<"\nPlease Type in the New Bright Mode: ";
         cin >> bright;
    }
    
    void operator() (){
        Menu sub_menu;
        sub_menu.settitle("Game Setting");
        sub_menu.add(sub_option1, "ARR").add(sub_option2, "DAS").add(sub_option3, "Gravity").add(sub_option4, "Bright");
        sub_menu.start();
        ofstream setting(SET_PATH);
        setting << das << ' ' << arr << ' ' << gravity << ' ' << bright;
        setting.close();
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
    option1 opt1;
    option2 opt2;
    option3 opt3;
    option4 opt4;
//    ifstream pic(PIC_PATH);
//    stringstream strStream;
//    strStream << pic.rdbuf(); //read the file
//    string title = strStream.str(); //str holds the content of the file
    loop.settitle("Tetris").add(opt1, "Single Player").add(opt2, "Multi Player").add(opt3, "Settings").add(opt4, "Quit");
    loop.start();
    return 0;
}

void game_init(){
    ifstream setting(SET_PATH), record(RECORD_PATH);
    setting >> arr >> das >> gravity >> bright;
    record >> playTimes >> clearCnt >> scoreCnt >> highClear >> highScore;
    setting.close();
    record.close();
}

void game_exit(){
    ofstream setting(SET_PATH), record(RECORD_PATH);
    setting << das << ' ' << arr << ' ' << gravity << ' ' << bright;
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

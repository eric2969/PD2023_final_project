#pragma comment(lib, "User32.lib") //User32.lib library for event listener
#include <bits/stdc++.h>
#include <windows.h>

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

#define nFONT
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
         singlePlayer(line, score);
         record_update(line, score);
    }
};
struct option2{
    int line, score;
    void operator() (){
         multiPlayer(line, score);
         record_update(line, score);
    }
};
struct option3{
    void operator() (){
        set_color(7);
        setcursor(0,0);
        cout << "hi";
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
#ifdef FONT
    SetFont(25);
#endif
    game_init();
    Menu loop;
    option1 opt1;
    option2 opt2;
    option3 opt3;
    option4 opt4;
    loop.settitle("Tetris").add(opt1, "Single Player").add(opt2, "Multi Player").add(opt3, "Settings").add(opt4, "Quit");
    loop.start();
    return 0;
}

void game_init(){
    streambuf *cinbuf = cin.rdbuf();
    ifstream setting(SET_PATH), record(RECORD_PATH);
    cin.rdbuf(setting.rdbuf());
    cin >> das >> arr >> gravity >> bright;
    cin.rdbuf(record.rdbuf());
    cin >> playTimes >> clearCnt >> scoreCnt >> highClear >> highScore;
    cin.rdbuf(cinbuf);
    setting.close();
    record.close();
}

void game_exit(){
    streambuf *coutbuf = cout.rdbuf();
    ofstream setting(SET_PATH), record(RECORD_PATH);
    cout.rdbuf(setting.rdbuf());
    cout << das << ' ' << arr << ' ' << gravity << ' ' << bright;
    cout.rdbuf(record.rdbuf());
    cout << playTimes << ' ' << clearCnt << ' ' << scoreCnt << ' ' << highClear << ' ' << highScore;
    cout.rdbuf(coutbuf);
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
	cfi.dwFontSize.X = 20;
	cfi.dwFontSize.Y = size;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}
#endif // FONT

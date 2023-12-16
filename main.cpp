#pragma comment(lib, "User32.lib") //User32.lib library for event listener
#include <bits/stdc++.h>
#include <windows.h>

#include "Menu.h"
#include "block.h"
#include "table.h"
#include "VK.h"
#include "game.h"

//#include "server.h"
//#include "client.h"

#define FONT
using namespace std;

#define SET_PATH "src/settings.txt"
#define RECORD_PATH "src/records.txt"
const short flush_tick = 2;
bool bright;
int das, gravity;
int playTimes, clearCnt, scoreCnt, highClear, highScore;

void game_init();
void game_exit();
void conf_set(int& tDas, int& tGrav, bool& bri) {das = tDas, gravity = tGrav;bright = bri;}
void record_reset() {playTimes = 0, clearCnt = 0, scoreCnt = 0, highClear = 0, highScore = 0;}
void record_update(int& clr, int& score);

#ifdef FONT
void SetFont(int);
#endif
struct option1{
    HANDLE hConsole;
    short flush_tick;
    int das;
    int gravity;
    bool bright;
    int line,score;
    option1(HANDLE hConsole,int flush_tick, int das, int gravity, bool bright, int line, int score):
        hConsole(hConsole), flush_tick(flush_tick), das(das), gravity(gravity), bright(bright), line(line), score(score){}
    void operator() (){
         singlePlayer(hConsole, flush_tick, das, gravity, bright, line, score);
    }
};
struct option2{
    void operator() (){
         Table player;
         player.set_position(2,2);
         player.new_block();
         HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
         player.print_table(hout);
         player.print_block(hout);
         Sleep(2000);
         goto_xy(0,0,hout);
         hidecursor();
         set_color(0,hout);
         clrscr();
         //system("cls");
    }
};
struct option3{
    void operator() (){
         cout << "hi";
         setcursor(0,0);
         HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
         hidecursor();
         set_color(0,hout);
         clrscr();
         //system("cls");
    }
};

signed main(){
    int tmpClear = 0, tmpScore = 0;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    game_init();
    Menu loop;
    SetFont(30);
    string s = "Tetris";
    option1 opt1(hConsole, flush_tick, das, gravity, bright, tmpClear, tmpScore);
    option2 opt2;
    
    loop.settitle(s);
    loop.add(opt1, "Single Player").add(opt2, "Multiplayer");
    loop.start();
    //singlePlayer(hConsole, flush_tick, das, gravity, bright, tmpClear, tmpScore);
    record_update(tmpClear, tmpScore);
    //multiPlayer(hConsole, flush_tick, das, gravity, bright, tmpClear, tmpScore);
    //record_update(tmpClear, tmpScore);
    game_exit();
    //system("pause");
}

void game_init(){
    streambuf *cinbuf = cin.rdbuf();
    ifstream setting(SET_PATH), record(RECORD_PATH);
    cin.rdbuf(setting.rdbuf());
    cin >> das >> gravity >> bright;
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
    cout << das << ' ' << gravity << ' ' << bright;
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

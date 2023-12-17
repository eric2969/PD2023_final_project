#pragma comment(lib, "User32.lib") //User32.lib library for event listener
#include <bits/stdc++.h>
#include <windows.h>

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
void conf_set(int& tDas, int& tGrav, bool& bri) {das = tDas, gravity = tGrav;bright = bri;}
void record_reset() {playTimes = 0, clearCnt = 0, scoreCnt = 0, highClear = 0, highScore = 0;}
void record_update(int& clr, int& score);

#ifdef FONT
void SetFont(int);
#endif

struct option1{
    int &line,&score;
    option1(int &line,int &score):line(line), score(score){}
    void operator() (){
         singlePlayer(line, score);
    }
};
struct option2{
    void operator() (){
         Table player;
         player.set_position(2,2);
         player.new_block();
         player.print_table();
         player.print_block();
         player.fix_block();
         Sleep(2000);
         goto_xy(0,0);
         hidecursor();
         set_color(0);
         clrscr();
    }
};
struct option3{
    void operator() (){
        //system("mode con cols=100 lines=50");
        set_color(7);
        setcursor(0,0);
        cout << "hi";
        setcursor(0,0);
        hidecursor();
        set_color(0);
        Sleep(2000);
        clrscr();
        //system("cls");
    }
};

signed main(){
    int tmpClear = 0, tmpScore = 0;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetFont(25);
    game_init();
    Menu loop;
    string s = "Tetris";
    option1 opt1(tmpClear, tmpScore);
    option2 opt2;
    option3 opt3;
    
    loop.settitle(s);
    loop.add(opt1, "Single Player").add(opt2, "Multiplayer").add(opt3, "Settings");
    loop.start();
    /*try{singlePlayer(tmpClear, tmpScore);}
    catch(runtime_error e){
        set_color(0);
        system("cls");
        set_color(7);
        cout << e.what() << endl;
    }*/
    record_update(tmpClear, tmpScore);
    //multiPlayer(hConsole, flush_tick, das, gravity, bright, tmpClear, tmpScore);
    //record_update(tmpClear, tmpScore);
    system("pause");
    game_exit();
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

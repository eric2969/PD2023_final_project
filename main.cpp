#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

HANDLE hConsole;
bool bright;
int das, arr, gravity;
string sbuff;
const short flush_tick = 2;

#define DEBUG
#define nFONT
#define SET_PATH "src/settings.txt"
#define PIC_PATH "src/pic.txt"
#define RECORD_PATH "src/records.txt"

//#include "server.h"
//#include "client.h"
#include "console.h"
#include "block.h"
#include "table.h"
#include "VK.h"
#include "game.h"
#include "Menu.h"

int playTimes, clearCnt, scoreCnt, highClear, highScore;

int stoi();
void game_init();
void game_exit();
void record_reset() {playTimes = 0, clearCnt = 0, scoreCnt = 0, highClear = 0, highScore = 0;}
void record_update(int& clr, int& score);

#ifdef FONT
void SetFont(int);
#endif

struct option1{
    static int line, score, gameMode, goal;
    static clock_t t;
    static void run(){
        t = clock();
        try{
             singlePlayer(line, score, gameMode, goal);
         }
         catch(runtime_error e){
                set_color(0);
                clrscr();
                set_color(7);
                cout << e.what() << endl;
                cout << "Used Time:" << (clock() - t)/1000 << endl;
                cout << "Clear Line:" << line << endl;
                cout << "Score:" << score << endl;
                Sleep(1000);
                pause();
         }
         record_update(line, score);
    }
    static void sub_option1(){
        gameMode = 0;
        run();
     }
    static void sub_option2(){
        cout << "Please input your goal (line)?" << endl;
        while(1){
            cin >> goal;
            if(goal > 0)
                break;
            else{
                cin.clear();
                fflush(stdin);
                cout << "Please input a number bigger than 0" << endl;
            }
        }
        gameMode = 1;
        run();
     }
    static void sub_option3(){
        cout << "Please input your time (second)?" << endl;
        while(1){
            cin >> goal;
            if(goal > 0)
                break;
            else{
                cin.clear();
                fflush(stdin);
                cout << "Please input a number bigger than 0" << endl;
            }
        }
        gameMode = 2;
        run();
    }
    void operator() (){
         Menu sub_menu;
         sub_menu.settitle("Choose a Game Mode, Right click for return to main menu");
         sub_menu.add(sub_option1, "Infinite Mode").add(sub_option2, "Clear Line Mode").add(sub_option3, "Time Mode");
         sub_menu.start();
    }
};
int option1::gameMode = 0;
int option1::line = 0;
int option1::score = 0;
int option1::goal = 0;
clock_t option1::t = 0;
struct option2{
    int line, score;
    void operator() (){
#ifdef FONT
    SetFont(25);
#endif
        try{
            multiPlayer(line, score);
        }
        catch(runtime_error e){
            set_color(0);
            clrscr();
            set_color(7);
            cout << e.what() << endl;
            pause();
        }
        record_update(line, score);
    }
};
struct option3{
    static void sub_option1(){
        cout << "Current ARR is: " << arr <<"\nPlease Type in the New ARR (1-500): ";
        while(1){
            cin >> arr;
            if(arr > 0 && arr <= 500)
                break;
            else{
                cin.clear();
                fflush(stdin);
                cout << "Please input a number between than 1 and 500: ";
            }
        }
     }
    static void sub_option2(){
        cout << "Current DAS is: " << das <<"\nPlease Type in the New DAS (1-1000): ";
        while(1){
            cin >> das;
            if(das > 0 && das <= 1000)
                break;
            else{
                cin.clear();
                fflush(stdin);
                cout << "Please input a number between than 1 and 1000: ";
            }
        }

     }
    static void sub_option3(){
         cout << "Current Gravity Level is: " << gravity <<"\nPlease Type in the New Gravity Level (1-50): ";
         while(1){
             cin >> gravity;
            if(gravity > 0 && gravity <= 50)
                break;
            else{
                cin.clear();
                fflush(stdin);
                cout << "Please input a number between than 1 and 50: ";
            }
         }
    }
    static void sub_option4(){
        cout << "Current Bright Mode is: " << bright <<"\nPlease Type in the New Bright Mode (0:dark/1:light): ";
        while(1){
            cin >> bright;
            if(!bright && bright == 1)
                break;
            else{
                cin.clear();
                fflush(stdin);
                cout << "Please input 0 or 1: ";
            }
        }
    }
    void operator() (){
        Menu sub_menu;
        sub_menu.settitle("Game Setting, Right click for return to main menu");
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
#ifdef FONT
    FullScreen();
    SetFont(25);
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

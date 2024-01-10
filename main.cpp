#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <functional>
#include <mutex>
#include <thread>
#include <ctime>
#include <cstring>
#include <vector>
#include <queue>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#if defined (__WIN32)
#include <windows.h>
#endif

using namespace std;
using namespace sf;

bool bright, server;
short das, arr, gravity, ResX, ResY, width, height, unit;
const short flush_tick = 5, DataSize = 130;
Mutex Thrd_lock;
RenderWindow window(VideoMode(VideoMode::getDesktopMode()), "Tetris!");
Event event;
Font font;

#define FONT_PATH "src/Arial.ttf"
#define SET_PATH "src/settings.txt"
#define RECORD_PATH "src/records.txt"


#include "header/Socket.h"
#include "header/Block.h"
#include "header/Table.h"
//#include "header/Menu.h"
#include "header/Game.h"

int playCnt, TimeCnt, clearCnt, scoreCnt, highClear, highScore;

void game_init();
void game_exit();
void set_unit(int cols, int lns){unit = min(ResX/cols,ResY/lns); return;}
void record_reset() {playCnt = 0, TimeCnt = 0, clearCnt = 0, scoreCnt = 0, highClear = 0, highScore = 0;}
void record_update(int& clr, int& score, const int& time);
#if defined (__WIN32)
void DisableIME(){
    HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
    HWND hWnd = GetConsoleWindow();
    PostMessage(hWnd, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)0x04090409);
    HKL keyboard = LoadKeyboardLayout("0x0409", KLF_ACTIVATE);
	ActivateKeyboardLayout(keyboard, 0);
}
#endif

void fetch(int &ret, int &tmp){
    while(1){
        cout << "input 1 to restart, 2 for quit: ";
        cin >> tmp;
        if(tmp == 1 || tmp == 2){
            ret = 0;
            break;
        }
    }
}

signed main(){
    //initialize the main menu
    int tLine, tScore, iTmp = 1;
    int ret;
    clock_t interval;
    game_init();
    set_unit(400, 500);
    Player player;
    player.set_position(20, 20);
    player.init(clock(), 0);
    while(window.isOpen()){
        interval = clock();
        try{singlePlayer(tLine, tScore);}
        catch(exception &e){
            record_update(tLine, tScore, (clock() - interval)/1000);
            cout << e.what() << endl;
            ret = 1;
            thread t1(fetch, ref(ret), ref(iTmp));
            while(ret){
                while (window.pollEvent(event)){
                    if(event.type == Event::Closed){
                        window.close();
                        game_exit();
                    }
                }
            }
            t1.join();
        }
        if(iTmp == 2)
			game_exit(); 
    }
    //socket_init();
    return 0;
}

void game_init(){
#if defined (__WIN32)
	DisableIME();
#endif
	srand(time(NULL));
	ResX = VideoMode::getDesktopMode().width;
	ResY = VideoMode::getDesktopMode().height;
	unit = 5; width = 10; height = 20;
    if(!font.loadFromFile(FONT_PATH)){
        cout << "Unable to load the font data\nPlease fixed it and open again!";
        exit(1);
    }
    // read user preference
    ifstream setting(SET_PATH), record(RECORD_PATH);
    if(setting.is_open())
        setting >> das >> arr >> gravity >> bright;
    else{
        cout << "Configuration data loaded fail, restore to default setting\nYou can modify it in the menu\n";
        das = 700; arr = 450; gravity = 45; bright = 1;
        ofstream set(SET_PATH);
        set << das << ' ' << arr << ' ' << gravity << ' ' << bright;
        set.close();
    }
    if(record.is_open())
        record >> playCnt >> TimeCnt >> clearCnt >> scoreCnt >> highClear >> highScore;
    else{
        cout << "Recode data loaded fail, record has reset\n";
        playCnt = 0, TimeCnt = 0, clearCnt = 0, scoreCnt = 0, highClear = 0, highScore = 0;
        ofstream rec(RECORD_PATH);
        rec << playCnt << ' ' << TimeCnt << ' ' << clearCnt << ' ' << scoreCnt << ' ' << highClear << ' ' << highScore;
        rec.close();
    }
    setting.close();
    record.close();
}

void game_exit(){
	//close socket
	/*if(conn){
		if(server)
			server_quit();
		else
			client_quit();
	}*/
    //save user preference
    ofstream setting(SET_PATH), record(RECORD_PATH);
    setting << das << ' ' << arr << ' ' << gravity << ' ' << bright;
    record << playCnt << ' ' << TimeCnt << ' ' << clearCnt << ' ' << scoreCnt << ' ' << highClear << ' ' << highScore;
    setting.close();
    record.close();
    exit(0);
}

void record_update(int& clr, int& score,const int& time){
    playCnt++;
    TimeCnt += time;
    clearCnt += clr;
    scoreCnt += score;
    highClear = max(highClear, clr);
    highScore = max(highScore, score);
}


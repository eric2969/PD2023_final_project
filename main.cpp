#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
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
#include <SFML/audio.hpp>
#include <SFML/Graphics.hpp>

#if defined (__WIN32)
#include <windows.h>
#endif

using namespace std;
using namespace sf;

const short flush_tick = 5, DataSize = 130;
bool conn, host;
short ResX, ResY, width, height, conf[4]; //0:das, 1:arr, 2:gravity
double unit;
int RecData[6], Thrd_ret, Thrd_token; //0: playcnt, 1:time, 2:line, 3:score, 4:h_line, 5:h_score
Mutex Thrd_lock;
RenderWindow window(VideoMode(VideoMode::getDesktopMode()), "Tetris!", Style::Fullscreen);
Music music, fx[2];
Event event;
Font font;
void set_unit(double cols, double lns){unit = min(ResX/(double)cols,ResY/(double)lns); return;}

#define FX_PATH "src/fx_"
#define BGM_PATH "src/bgm.flac"
#define FONT_PATH "src/font.ttf"
#define SET_PATH "src/settings.txt"
#define RECORD_PATH "src/records.txt"

#include "header/Socket.hpp"
#include "header/Block.hpp"
#include "header/Table.hpp"
#include "header/Object.hpp"
#include "header/Menu_instance.hpp"
#include "header/Game.hpp"

void game_init();
void game_exit();
#if defined (__WIN32)
void DisableIME(){
    HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
    HWND hWnd = GetConsoleWindow();
    PostMessage(hWnd, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)0x04090409);
    HKL keyboard = LoadKeyboardLayout("0x0409", KLF_ACTIVATE);
	ActivateKeyboardLayout(keyboard, 0);
}
#endif

signed main(){
    //game initialization
    game_init();
    main_menu();
    game_exit();
    return 0;
}

void game_init(){
#if defined (__WIN32)
	DisableIME();
#endif
	srand(time(NULL));
    conn = 0, Thrd_ret = 0, Thrd_token = 0;
	ResX = VideoMode::getDesktopMode().width;
	ResY = VideoMode::getDesktopMode().height;
	width = 10; height = 20;
    if(!font.loadFromFile(FONT_PATH)){
        if(window.isOpen()) window.close();
        char m;
        cout << "Unable to load the font data\nPlease fixed it and open again!\nPress Enter to Quit the game!";
        gets(&m);
        exit(1);
    }
    // read user preference
    ifstream setting(SET_PATH), record(RECORD_PATH);
    if(setting.is_open())
        for(int i = 0;i < 3;i++) setting >> conf[i];
    else{
        conf[0] = 700, conf[1] = 450, conf[2] = 45;
        ofstream set(SET_PATH);
        set << conf[0];
        for(int i = 1;i < 3;i++) set << ' ' << conf[i];
        set.close();
    }
    if(record.is_open())
        for(int i = 0;i < 6;i++) record >> RecData[i];
    else{
        for(int i = 0;i < 6;i++) RecData[i] = 0;
        ofstream rec(RECORD_PATH);
        rec << RecData[0];
        for(int i = 1;i < 6;i++) rec << ' ' << RecData[i];
        rec.close();
    }
    setting.close();
    record.close();
    if (music.openFromFile(BGM_PATH)){
        music.setVolume(10.f);
        music.play();
        music.setLoop(true);
    }
    for(int i = 0;i < 2;i++)
	    if (fx[i].openFromFile(FX_PATH + to_string(i) + ".flac")){
	        fx[i].setVolume(100.f);
	        fx[i].setLoop(false);
	    }
}

void game_exit(){
	//close socket
	socket_disconnect();
    //save user preference
    ofstream set(SET_PATH), rec(RECORD_PATH);
    set << conf[0];
    for(int i = 1;i < 3;i++) set << ' ' << conf[i];
    rec << RecData[0];
    for(int i = 1;i < 6;i++) rec << ' ' << RecData[i];
    set.close();
    rec.close();
    if(window.isOpen()) window.close();
    exit(0);
}

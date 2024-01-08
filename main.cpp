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

using namespace std;
using namespace sf;

bool bright, conn, server, multi;
short das, arr, gravity, ResX, ResY, width, height, unit;
const short flush_tick = 5, DataSize = 130;
RenderWindow window(VideoMode(VideoMode::getDesktopMode()), "Tetris!");
Event event;
Font font;

#define FONT_PATH "src/Arial.ttf"
#define SET_PATH "src/settings.txt"
#define RECORD_PATH "src/records.txt"

//#include "header/Socket.h"
#include "header/Block.h"
#include "header/Table.h"
//#include "header/Menu.h"
//#include "header/Game.h"

int playCnt, TimeCnt, clearCnt, scoreCnt, highClear, highScore;

void game_init();
void game_exit();
void set_unit(int cols, int lns){unit = min(ResX/cols,ResY/lns); return;}
void record_reset() {playCnt = 0, TimeCnt = 0, clearCnt = 0, scoreCnt = 0, highClear = 0, highScore = 0;}
void record_update(int& clr, int& score, const int& time);

signed main(){
    //initialize the main menu
    game_init();
    set_unit(400, 500);
    Player player;
    player.set_position(20, 20);
    player.init(clock());
    while(window.isOpen()){
    	while (window.pollEvent(event)){
            if(event.type == Event::Closed)
                window.close();
        }
        window.clear();
        player.print_table();
        window.display();
        sleep(milliseconds(flush_tick));
    }
    //socket_init();
    return 0;
}

void game_init(){
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


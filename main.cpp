#pragma comment(lib, "User32.lib") //User32.lib library for event listener
#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <windows.h>
#include <queue>
#include <algorithm>
#include <random>
#include <time.h>
#include "block.h"
#include "table.h"
#include "VK.h"
//#include "server.h"
//#include "client.h"

#define nDEBUG
#define nFONT
using namespace std;

const short flush_tick = 10;
void add_shuffle_block(Table&);
void game_exit();
#ifdef FONT
void SetFont(int);
#endif

signed main(){
    Table player, opponent;
    system("mode con cols=100 lines=50");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    #ifdef FONT
    SetFont(30);
    #endif // FONT
    bool isUpPressed = false;
    bool isDownPressed = false;
    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isZPressed = false;
    bool isXPressed = false;
    bool isSPACEPressed = false;
    clock_t before, now, down, left, right, tStuck;
    bool bUp, bDown, bLeft, bRight, bKeyZ, bKeyX, bSpace, stuck = 0;
    before = clock();
    short fwait = 100, wait = 700, das = 500, sWait = 700, sCnt, sTick = 70, sLimit = 10;
    short fall_tick = wait, fall_wait = sWait;
    player.set_position(2,2);
    opponent.set_position(60,2);
start:
    set_color(0, hConsole);
    system("cls");
    player.reset();
    add_shuffle_block(player);
    player.pop_block(); //move next to current
    player.print_table(hConsole);
    opponent.print_table(hConsole);
    while (1) {
        isUpPressed = GetAsyncKeyState(VK_UP) & 0x8000;
        isDownPressed = GetAsyncKeyState(VK_DOWN) & 0x8000;
        isLeftPressed = GetAsyncKeyState(VK_LEFT) & 0x8000;
        isRightPressed = GetAsyncKeyState(VK_RIGHT) & 0x8000;
        isZPressed = GetAsyncKeyState(VK_Z) & 0x8000; //counterclockwise rotate
        isXPressed = GetAsyncKeyState(VK_X) & 0x8000;
        isSPACEPressed = GetAsyncKeyState(VK_SPACE) & 0x8000;
        //down
        now = clock();
        if (clock() - before > fall_tick){
            if(stuck){
                if(clock() - tStuck > fall_wait){
                    player.fix_block();
                    player.print_table(hConsole);
                    try{player.set_clear();}
                    catch(runtime_error e){break;}
                    stuck = 0;
                    if(player.getNext() <= 1)
                        add_shuffle_block(player);
                    player.pop_block();
                }
                else
                    stuck = !player.move_block(0,-1);
            }
            else{
                stuck = !player.move_block(0,-1);
                before = clock();
                if(stuck)
                    fall_wait = sWait;
                    tStuck = clock();
                    sCnt = 0;
            }
        }
        //down arrow
        fall_tick = (isDownPressed)?fwait:wait;
        //left arrow
        if (isLeftPressed) {
            if (bLeft){
                if(clock() - left > das){
                    player.move_block(-1,0);
                    fall_wait += (sCnt++ >= sLimit)?0:sTick;
                }
            }
            else{
                player.move_block(-1,0);
                left = clock();
                fall_wait += (sCnt++ >= sLimit)?0:sTick;
            }
            bLeft = 1;
        }
        else
            bLeft = 0;
        //right arrow
        if (isRightPressed) {
            if(bRight){
                if(clock() - right > das){
                    player.move_block(1,0);
                    fall_wait += (sCnt++ >= sLimit)?0:sTick;
                }
            }
            else{
                player.move_block(1,0);
                right = clock();
                fall_wait += (sCnt++ >= sLimit)?0:sTick;
            }
            bRight = 1;
        }
        else
            bRight = 0;
        //key z
        if (isZPressed) {
            if(!bKeyZ){
                player.rotate(-1);
                fall_wait += (sCnt++ >= sLimit)?0:sTick;
            }
            bKeyZ = 1;
        }
        else
            bKeyZ = 0;
        //key x
        if (isXPressed) {
            if(!bKeyX){
                player.rotate(1);
                fall_wait += (sCnt++ >= sLimit)?0:sTick;
            }
            bKeyX = 1;
        }
        else
            bKeyX = 0;
        //Up
        if (isUpPressed) {
            if(!bUp){
                player.rotate(1);
                fall_wait += (sCnt++ >= sLimit)?0:sTick;
            }
            bUp = 1;
        }
        else
            bUp = 0;
        //space
        if (isSPACEPressed){
            if(!bSpace){
                player.hard_drop();
                player.fix_block();
                player.print_table(hConsole);
                try{player.set_clear();}
                catch(runtime_error e){break;}
                if(player.getNext() <= 1)
                    add_shuffle_block(player);
                player.pop_block();
                stuck = 0;
            }
            bSpace = 1;
        }
        else
            bSpace = 0;
#ifdef DEBUG
#else
        player.print_block(hConsole);
        opponent.print_table(hConsole);
        Sleep(flush_tick);
#endif
    }
    goto_xy(0, 25, hConsole);
    set_color(244, hConsole);
    short inTmp;
    cout << "   Game Over\n   Play again please input 1, Quit the game please input 2\n";
    while(1){
        cin >> inTmp;
        switch(inTmp){
            case 1:{
                goto start;
                break;
            }
            case 2:{
                game_exit();
                break;
            }
            default:{
                goto_xy(0, 27, hConsole);
                cout << "   Please input 1 or 2:  ";
                break;
            }
        }
    }
    return 0;
}

const short d_x = 5, d_y = 20;

void add_shuffle_block(Table &player){
    //randomly generate
    srand( time(NULL) );
    int shuffle_block[10];
    for (int i = 0; i < 7; i++) shuffle_block[i] = i;
    random_shuffle(shuffle_block,shuffle_block+7);
    for (int i = 0; i < 7; i++){
      switch(shuffle_block[i]){
        case 0:{
            Block *I = new Block_I(Point(d_x, d_y));
            player.add_block(I);
            break;
        }
        case 1:{
            Block *J = new Block_J(Point(d_x, d_y));
            player.add_block(J);
            break;
        }

        case 2:{
            Block *L = new Block_L(Point(d_x, d_y));
            player.add_block(L);
            break;
        }

        case 3:{
            Block *O = new Block_O(Point(d_x, d_y));
            player.add_block(O);
            break;
        }

        case 4:{
            Block *S = new Block_S(Point(d_x, d_y));
            player.add_block(S);
            break;
        }

        case 5:{
            Block *T = new Block_T(Point(d_x, d_y));
            player.add_block(T);
            break;
        }

        case 6:{
            Block *Z = new Block_Z(Point(d_x, d_y));
            player.add_block(Z);
            break;
        }

        default:
            exit(1);
            break;
      }
    }
}

void game_exit(){
    //for file i/o record data used;
    exit(0);
}

#ifdef FONT
void SetFont(int size = 30) {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = size;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

}
#endif // FONT


#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <windows.h>
#include <queue>
#include <algorithm>
#include <stdlib.h>
#include <random>
#include <time.h>
#include "block.h"
#include "table.h"
#include "VK.h"

#define nDEBUG
using namespace std;

const short flush_tick = 10;
void add_shuffle_block(Table&);
void SetFont(int);

signed main(){
    Table player, opponent;
    system("mode con cols=100 lines=50");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetFont(30);
    bool isUpPressed = false;
    bool isDownPressed = false;
    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isZPressed = false;
    bool isXPressed = false;
    bool isSPACEPressed = false;
    clock_t before, now, down, left, right;
    bool bDown, bLeft, bRight, bKeyZ, bKeyX, bSpace, stuck = 0;
    before = clock();
    int wait = 500;
    player.set_position(2,2);
    opponent.set_position(60,1);
    add_shuffle_block(player);
    player.pop_block(); //move next to current
    player.print_table(hConsole);
    while (1) {
        isDownPressed = GetAsyncKeyState(VK_DOWN) & 0x8000;
        isLeftPressed = GetAsyncKeyState(VK_LEFT) & 0x8000;
        isRightPressed = GetAsyncKeyState(VK_RIGHT) & 0x8000;
        isZPressed = GetAsyncKeyState(VK_Z) & 0x8000; //counterclockwise rotate
        isXPressed = GetAsyncKeyState(VK_X) & 0x8000;
        isSPACEPressed = GetAsyncKeyState(VK_SPACE) & 0x8000;
        //down
        now = clock();
        if (clock() - before > wait){
            stuck = !player.move_block(0,-1);
            before = clock();
            if(stuck){
                player.fix_block();
                player.set_clear();
                player.print_table(hConsole);
                stuck = 0;
                if(player.getNext() <= 1)
                    add_shuffle_block(player);
                player.pop_block();
            }
        }
        //down arrow
        wait = (isDownPressed)?100:1000;
        //left arrow
        if (isLeftPressed) {
            if (bLeft)
                if(clock() - left > 500)
                    player.move_block(-1,0);
            else{
                player.move_block(-1,0);
                left = clock();
            }
            bLeft = 1;
        }
        else
            bLeft = 0;
        //right arrow
        if (isRightPressed) {
            if(bRight)
                if(clock() - right > 500)
                    player.move_block(1,0);
            else{
                player.move_block(1,0);
                right = clock();
            }
            bRight = 1;
        }
        else
            bRight = 0;
        //key z
        if (isZPressed) {
            if(!bKeyZ)
                player.rotate(-1);
            bKeyZ = 1;
        }
        else
            bKeyZ = 0;
        //key x
        if (isXPressed) {
            if(!bKeyX)
                player.rotate(1);
            bKeyX = 1;
        }
        else
            bKeyX = 0;
        //space
        if (isSPACEPressed){
            if(!bSpace){
                player.hard_drop();
                player.fix_block();
                player.set_clear();
                player.print_table(hConsole);
                if(player.getNext() <= 1)
                    add_shuffle_block(player);
                player.pop_block();
            }
            bSpace = 1;
        }
        else
            bSpace = 0;
#ifdef DEBUG
#else
        //player.print_table(hConsole);
        player.print_block(hConsole);
        //opponent.print_table(hConsole);
        //system("cls");
        Sleep(flush_tick);
#endif 
    }

    return 0;
}

const short d_x = 5, d_y = 19;

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


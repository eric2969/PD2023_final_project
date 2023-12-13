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

#define notDEBUG
using namespace std;

const short flush_tick = 10;
void add_shuffle_block(Table&);


signed main(){
#ifdef DEBUG
    Block_Z tz(Point(0,0));
    
    vector<Block> q;
    q.push_back(tz);
    Block *z = &q.back();
    q.pop_back(); 
    for(int i=0;i<4;i++){
        cout << z->get_xdelta(i) << ' ' << z->get_ydelta(i) << ',';
    }
    cout << endl;
    cout << z->get_ID();
    return 0;
}
#else
    Table player, opponent;

    system("mode con cols=100 lines=50");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    bool isUpPressed = false;
    bool isDownPressed = false;
    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isZPressed = false;
    bool isXPressed = false;
    bool isSPACEPressed = false;
    clock_t before, now, down, left, right;
    bool bDown, bLeft, bRight, bKeyZ, bKeyX;
    before = clock();
    int wait = 500; 
    player.set_position(2,2);
    opponent.set_position(60,1); 
    add_shuffle_block(player);

    player.pop_block(); //move next to current
    //opponent.print_table(hConsole);
    //gameover
    while (1) {
        isDownPressed = GetAsyncKeyState(VK_DOWN) & 0x8000;
        isLeftPressed = GetAsyncKeyState(VK_LEFT) & 0x8000;
        isRightPressed = GetAsyncKeyState(VK_RIGHT) & 0x8000;
        isZPressed = GetAsyncKeyState(VK_Z) & 0x8000; //counterclockwise rotate
        isXPressed = GetAsyncKeyState(VK_X) & 0x8000;
        isSPACEPressed = GetAsyncKeyState(VK_SPACE) & 0x8000;
        
        
        now = clock();
        if (clock() - before > wait){
          player.move_block(0,1);
          before = clock();
        }
        

        // moving blocks
        // rotating blocks
        // the second parameter is drc, marking clockwise or counterclockwise, storing whether 1 or -1
        wait = (isDownPressed)?100:1000;

        if (isLeftPressed) {
          if (bLeft){
            if(clock() - left > 500){
                player.move_block(-1,0);
                player.move_block(-1,0);
            }
          }
          else{
            player.move_block(-1,0);
            left = clock();
          }
          bLeft = 1;
        }
        else
          bLeft = 0;
        
        if (isRightPressed) {
          if(bRight){
            if(clock() - right > 500){
                player.move_block(1,0);
                player.move_block(1,0);
            }
          }
          else{
            player.move_block(1,0);
            right = clock();
          }
          bRight = 1;
        } 
        else
          bRight = 0;
        
        if (isZPressed) {
          if(!bKeyZ)
            player.rotate(-1);
        }
        else
          bKeyZ = 0;
        
        if (isXPressed) {
          if(!bKeyX)
            player.rotate(1);
        }
        else
          bKeyX = 0;
        //fixed
        if (isSPACEPressed){
          
          player.pop_block();
          if (player.getNext() <= 1){
            add_shuffle_block(player);
          }
        }
        player.print_table(hConsole);
        player.print_block(hConsole);
        Sleep(flush_tick);
        system("cls");
    }

    return 0;
}

void add_shuffle_block(Table &player){
    //randomly generate 
    srand( time(NULL) );
    int shuffle_block[10];
    for (int i = 0; i < 7; i++) shuffle_block[i] = i;
    random_shuffle(shuffle_block,shuffle_block+7);
    for (int i = 0; i < 7; i++){
      switch(shuffle_block[i]){
        case 0:{
            Block *I = new Block_I(Point(5,1));
            player.add_block(I);
            break;
        }
        case 1:{
            Block *J = new Block_J(Point(5,1));
            player.add_block(J);
            break;
        }
          
        case 2:{
            Block *L = new Block_L(Point(5,1));
            player.add_block(L);
            break;
        }
          
        case 3:{
            Block *O = new Block_O(Point(5,1));
            player.add_block(O);
            break;
        }
          
        case 4:{
            Block *S = new Block_S(Point(5,1));
            player.add_block(S);
            break;
        }
          
        case 5:{
            Block *T = new Block_T(Point(5,1));
            player.add_block(T);
            break;
        }
          
        case 6:{
            Block *Z = new Block_Z(Point(5,1));
            player.add_block(Z);
            break;
        }
          
        default:
            exit(1);
            break;
      }
    }
}
#endif

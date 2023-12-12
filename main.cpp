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

#define notDEBUG
using namespace std;

const short flush_tick = 10;

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
    
    return 0;

#else
    Table player, opponent;

    system("mode con cols=100 lines=50");//?�m���f�j�p
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    bool isUpPressed = false;
    bool isDownPressed = false;
    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isZPressed = false;
    player.set_position(0,0);
    opponent.set_position(60,1); 
    
    player.add_block(z);
    
    
    player.print_table(hConsole);
    //opponent.print_table(hConsole);
    //gameover
    while (true) {
        isDownPressed = GetAsyncKeyState(VK_DOWN) & 0x8000;
        isLeftPressed = GetAsyncKeyState(VK_LEFT) & 0x8000;
        isRightPressed = GetAsyncKeyState(VK_RIGHT) & 0x8000;
        isZPressed = GetAsyncKeyState(VK_Z) & 0x8000; //counterclockwise rotate
        isXPressed = GetAsyncKeyState(VK_X) & 0x8000;
        isEnterPressed = GetAsyncKeyState(VK_SPACE) & 0x8000;
        
        
        // moving blocks
        // rotating blocks
        // the second parameter is drc, marking clockwise or counterclockwise, storing whether 1 or -1
        if (isDownPressed) {
          player.move_block(0,1); //change into 'move fast to the bottom'
        } else if (isLeftPressed) {
          player.move_block(-1,0);
        } else if (isRightPressed) {
          player.move_block(1,0);
        } else if (isZPressed) {
          player.rotate(-1);
        } else if (isXPressed) {
          player.rotate(1);
        }
        
        if (isEnterPressed){
          player.print_block(hConsole);
          player.pop_block();
          if
        }
        
        Sleep(flush_tick);
        //system("cls");

      //Gameover
      if (board)

    }

    return 0;
}
#endif

void add_shuffle_block(){
    //randomly generate 
    srand( time(NULL) );
    int shuffle_block[7];
    for (int i = 0; i < 7; i++) a[i] = i;
    random_shuffle(shuffle_block);
    for (int i = 0; i < 7; i++){
      switch(a[i]){
        case 0:
          Block_I i(Point{0,5});
          player.add_block(i);
        case 1:
          Block_J j(Point{0,5});
          player.add_block(j);
        case 2:
          Block_L l(Point{0,5});
          player.add_block(l);
        case 3:
          Block_O o(Point{0,5});
          player.add_block(o);
        case 4:
          Block_S s(Point{0,5});
          player.add_block(s);
        case 5:
          Block_T t(Point{0,5});
          player.add_block(t);
        case 6:
          Block_Z z(Point{0,5});
          player.add_block(z);
      }
    }
}

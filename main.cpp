#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <windows.h>
#include <queue>
#include <algorithm>
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
}
#else
    Table player, opponent;
    system("mode con cols=100 lines=50");//?�m���f�j�p
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    bool isUpPressed = false;
    bool isDownPressed = false;
    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isZPressed = false;
    player.set_position(20,5);
    opponent.set_position(60,1);
    Block_Z z(Point{0,0}); //randomize?
    player.add_block(z);
    player.pop_block();
    
    while (true) {
        isUpPressed = GetAsyncKeyState(VK_UP) & 0x8000;
        isDownPressed = GetAsyncKeyState(VK_DOWN) & 0x8000;
        isLeftPressed = GetAsyncKeyState(VK_LEFT) & 0x8000;
        isRightPressed = GetAsyncKeyState(VK_RIGHT) & 0x8000;
        isZPressed = GetAsyncKeyState(VK_UP) & 0x8000; //counterclockwise rotate
        
        
        // moving blocks
        if (isDownPressed) {
          player.move_block(0,1);
        } else if (isLeftPressed) {
          player.move_block(-1,0);
        } else if (isRightPressed) {
          player.move_block(1,0);
        }else if (isUpPressed) {
          player.move_block(0,-1);
        }

        // rotating blocks
        // the second parameter is drc, marking clockwise or counterclockwise, storing whether 1 or -1
//        if (isUpPressed) {
//          player.rotate(1);
//        } else if (isZPressed){
//          player.rotate(-1);
//        }

        
        player.print_table(hConsole);
        player.print_block(hConsole);
        //opponent.print_table(hConsole);
        Sleep(flush_tick);
        //system("cls");
    }

    return 0;
}
#endif

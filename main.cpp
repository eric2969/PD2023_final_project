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

const short flush_tick = 100;

signed main(){
#ifdef DEBUG
    Block_Z z(Point(0,0));
    for(int i=0;i<4;i++){
        cout << z.get_xdelta(i) << ' ' << z.get_ydelta(i) << ',';
    }
    cout << endl;
    cout << z.get_location().x << z.get_location().y << endl;
    vector<Point> v;
    v = z.block_position();
    cout << &v;
    for(auto p:v){
        cout << p.x << ' ' << p.y << ',';
    }
    cout << endl;
    z.move_set(1,0);
    v = z.block_position();
    cout << &v;
    for(auto p:v){
        cout << p.x << ' ' << p.y << ',';
    }
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

    //randomly generate 
    srand( time(NULL) );
    int shuffle_block[7];
    for (int i = 0; i < 7; i++) a[i] = i;
    random_shuffle(shuffle_block);
    for (int i = 0; i < 7; i++){
      switch(a[i]){
        case 0:
          Block_Z z
        case 1:
          Block_I 
        case 2:
          
      }
    }
    
    Block_Z z(Point{0,0}); //randomize?
    player.add_block(z);
    
    
    player.print_table(hConsole);
    //opponent.print_table(hConsole);
    //gameover
    while (true) {
        // �ˬd��V��O�_�Q���U
        //virtual key code
        //isUpPressed = GetAsyncKeyState(VK_UP) & 0x8000;
        isDownPressed = GetAsyncKeyState(VK_DOWN) & 0x8000;
        isLeftPressed = GetAsyncKeyState(VK_LEFT) & 0x8000;
        isRightPressed = GetAsyncKeyState(VK_RIGHT) & 0x8000;
        isZPressed = GetAsyncKeyState(VK_Z) & 0x8000; //counterclockwise rotate
        isXPressed = GetAsyncKeyState(VK_X) & 0x8000;
        isEnterPressed = GetAsyncKeyState(VK_SPACE) & 0x8000;
        
        
        // �B�z��V�䪺�ƥ�
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
        }
        
        
        Sleep(flush_tick);
        //system("cls");

      //Gameover
      if (board)

    }

    return 0;
}
#endif

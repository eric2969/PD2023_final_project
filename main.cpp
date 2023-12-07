#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <windows.h>

#include "block.h"
#include "table.h"

using namespace std;

const short flush_tick = 200;

signed main(){
    Table player, opponent;
    system("mode con cols=100 lines=50");//?�m���f�j�p
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    bool isUpPressed = false;
    bool isDownPressed = false;
    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isZPressed = false;

    while (true) {
        // �ˬd��V��O�_�Q���U
        isUpPressed = GetAsyncKeyState(VK_UP) & 0x8000;
        isDownPressed = GetAsyncKeyState(VK_DOWN) & 0x8000;
        isLeftPressed = GetAsyncKeyState(VK_LEFT) & 0x8000;
        isRightPressed = GetAsyncKeyState(VK_RIGHT) & 0x8000;
        isZPressed = GetAsyncKeyState(VK_RIGHT) & 0x8000; //counterclockwise rotate
        Block_Z z(Point{0,0}); //randomize?
        player.add_block(z);
        
        // �B�z��V�䪺�ƥ�
        // moving blocks
        if (isDownPressed) {
          player.move_block(0,-1);
        } else if (isLeftPressed) {
          player.move_block(-1,0);
        } else if (isRightPressed) {
          player.move_block(1,0);
        }

        // rotating blocks
        // the second parameter is drc, marking clockwise or counterclockwise, storing whether 1 or -1
        if (isUpPressed) {
          player.rotate(player.get_current().get_direction(), 1);
        } else if (isZPressed){
          player.rotate(player.get_current().get_direction(), -1);
        }

        
        player.print_table(0,1,hConsole);
        player.print_block(hConsole);
        opponent.print_table(60,1,hConsole);
        Sleep(flush_tick);
        //system("cls");
    }

    return 0;
}

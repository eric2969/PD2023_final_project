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
    system("mode con cols=100 lines=50");//?置窗口大小
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    bool isUpPressed = false;
    bool isDownPressed = false;
    bool isLeftPressed = false;
    bool isRightPressed = false;

    while (true) {
        // 檢查方向鍵是否被按下
        isUpPressed = GetAsyncKeyState(VK_UP) & 0x8000;
        isDownPressed = GetAsyncKeyState(VK_DOWN) & 0x8000;
        isLeftPressed = GetAsyncKeyState(VK_LEFT) & 0x8000;
        isRightPressed = GetAsyncKeyState(VK_RIGHT) & 0x8000;
        Block_Z z(Point{0,0});
        player.add_block(z);
        // 處理方向鍵的事件
        if (isUpPressed) {
          player.move_block(0,1);
        } else if (isDownPressed) {
          player.move_block(0,-1);
        } else if (isLeftPressed) {
          player.move_block(-1,0);
        } else if (isRightPressed) {
          player.move_block(1,0);
        }
        
        player.print_table(0,1,hConsole);
        player.print_block(hConsole);
        opponent.print_table(60,1,hConsole);
        Sleep(flush_tick);
        //system("cls");
    }

    return 0;
}

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
    system("mode con cols=100 lines=50");//?¸mµ¡¤f¤j¤p
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    while(1){
        player.print_table(0,1,hConsole);
        opponent.print_table(60,1,hConsole);
        Sleep(flush_tick);
        system("cls");
    }


    return 0;
}

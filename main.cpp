#pragma GCC optimize("O3")
#include <bits/stdc++.h>
#include <vector>
#include <utility>
#include <windows.h>
#include <stdio.h>

#include "block.h"
#include "table.h"

using namespace std;

#define IO ios_base::sync_with_stdio(0),cin.tie(0);
#define endl '\n'

signed main(){
    
    Table player;
    Block_Z z(Point(0,0));
    system("mode con cols=70 lines=50");//?¸mµ¡¤f¤j¤p
    //player.add_block(z);
    //player.move_block(2,5);
    while(1){
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        player.print_table(0,1,hConsole);
        player.print_table(20,1,hConsole);
        Sleep(1000);
        system("cls");
    }
    
    
    return 0;
}

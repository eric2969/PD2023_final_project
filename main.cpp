#pragma GCC optimize("O3")
#include <bits/stdc++.h>
#include <vector>
#include <utility>
#include <windows.h>

#include "block.h"
#include "table.h"

using namespace std;

#define IO ios_base::sync_with_stdio(0),cin.tie(0);
#define endl '\n'

signed main(){
    IO;
    Table player;
    Block_Z z(Point(0,0));
    //player.add_block(z);
    //player.move_block(2,5);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    player.print_table(0,0,hConsole);
    return 0;
}

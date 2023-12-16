//Up, Down, Left, Right, Space, Z, X, C, Shift
const int KeyCode[9] = {VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_SPACE, VK_Z, VK_X, VK_C, VK_SHIFT};
//hold keystate[7]
bool KeyPressed[9] = {}, KeyState[8] = {}, stuck;
const short fTick = 1000, sLimit = 10;
double speed;
short fall_tick, stuck_wait, sCnt;
//arrow:Left, Right
clock_t before, tStuck, tArrow[2];

void getKeyState() {for(int i = 0;i < 9;i++) KeyPressed[i] = GetAsyncKeyState(KeyCode[i]) & 0x8000;}
void game_cycle(Table& player, const int& das, const int& gravity, int& line, int& score);

void singlePlayer(HANDLE &hConsole, const int& flush, const int& das, const int& gravity, const bool& bri, int& line, int& score){
    Table player;
    speed = 1.0;
    system("mode con cols=100 lines=50");
    set_color(0, hConsole);
    system("cls");
    before = clock();
    player.set_position(2,2);
    player.init(hConsole, bri);
    player.new_block();
    player.print_table();
    while (1) {
        game_cycle(player, das, gravity, line, score);
        player.print_block();
        Sleep(flush);
    }
}

void multiPlayer(HANDLE &hConsole, const int& flush, const int& das, const int& gravity, const bool& bri, int& line, int& score){
    Table player, opponent;
    speed = 1.0;
    system("mode con cols=100 lines=50");
    set_color(0, hConsole);
    system("cls");
    before = clock();
    player.set_position(2,2);
    opponent.set_position(50,2);
    player.init(hConsole, bri);
    opponent.init(hConsole, bri);
    player.new_block();
    player.print_table();
    opponent.print_table();
    while (1) {
        game_cycle(player, das, gravity, line, score);
        player.print_block();
        opponent.RecvTable("");
        Sleep(flush);
    }
}

void game_cycle(Table& player, const int& flush, const int& das, const int& gravity, int& line, int& score){
    getKeyState();
    //fall
    if (clock() - before > fall_tick){
        if(stuck){
            if(clock() - tStuck > stuck_wait){
                player.fix_block();
                try{player.chk_clear();}
                catch(std::runtime_error e){line = player.get_line(); score = player.get_score();return;}
                player.new_block();
                player.print_table();
                stuck = 0;
                KeyState[7] = 0;
            }
            else
                stuck = !player.move_block(0,-1);
        }
        else{
            stuck = !player.move_block(0,-1);
            before = clock();
            if(stuck){
                stuck_wait = fTick * speed;
                tStuck = clock();
                sCnt = 0;
            }
        }
    }
    //down arrow
    fall_tick = (KeyPressed[1])?(fTick*speed*(21 - gravity)/20):(fTick*speed);
    //left/right arrow
    for(int i = 0;i < 2;i++){
        if (KeyPressed[2 + i]) {
            if (KeyState[2 + i]){
                if(clock() - tArrow[i] > das * speed){
                    player.move_block((i%2?1:-1), 0);
                    stuck_wait += (sCnt++ >= sLimit)?0:(fTick*speed/10);
                }
            }
            else{
                player.move_block((i%2?1:-1), 0);
                tArrow[i] = clock();
                stuck_wait += (sCnt++ >= sLimit)?0:(fTick*speed/10);
            }
            KeyState[2 + i] = 1;
        }
        else
            KeyState[2 + i] = 0;
    }
    //Up(0), Z(5), X(6) even:clockwise
    for(int i = 0;i < 3;i++){
        if(KeyPressed[i + (i?4:0)]){
            if(!KeyState[i + (i?4:0)]){
                player.rotate((i%2?-1:1));
                stuck_wait += (sCnt++ >= sLimit)?0:(fTick*speed/10);
            }
            KeyState[i + (i?4:0)] = 1;
        }
        else
            KeyState[i + (i?4:0)] = 0;
    }
    //C(7), Shift(8)
    for(int i = 0;i < 2;i++){
        if (KeyPressed[i + 7]) {
            if(!KeyState[7]){
                player.hold_block(); //keep
                KeyState[7] = 1;
                player.print_table();
            }
        }
    }
    //space(4)
    if (KeyPressed[4]){
        if(!KeyState[4]){
            player.hard_drop();
            player.fix_block();
            try{player.chk_clear();}
            catch(std::runtime_error e){line = player.get_line(); score = player.get_score();return;}
            player.new_block();
            player.print_table();
            stuck = 0;
            KeyState[7] = 0;
        }
        KeyState[4] = 1;
    }
    else
        KeyState[4] = 0;
    speed = (1.0 - 0.032 * player.get_level());
}
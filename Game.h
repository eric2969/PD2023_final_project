#define KeyCnt 12
//Up, Down, Left, Right, Space, Z, X, C, Shift, P(pause), Q(quit), R(resume)
const int KeyCode[KeyCnt] = {VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_SPACE, VK_Z, VK_X, VK_C, VK_SHIFT, VK_P, VK_Q, VK_R};
//keyState C,shift(7)
double speed;
bool KeyPressed[KeyCnt] = {}, KeyState[KeyCnt] = {}, stuck, clr;
const short fTick = 1000, sLimit = 10;
short fall_tick, stuck_wait, sCnt;
//arrow:Left, Right
clock_t before, tStuck, tClear, tStart, tArrow,tDas;

void quit();
void getKeyState() {for(int i = 0;i < KeyCnt;i++) KeyPressed[i] = GetAsyncKeyState(KeyCode[i]) & 0x8000;}
void game_cycle(Table& player, int& line, int& score, bool single);

void singlePlayer(int& line, int& score, int mode = 0, int goal = 40){ //mode:0(infinite), 1 (line, line), 2(time, second)
    Table player;
    speed = 1.0, line = 0, score = 0, stuck = 0;
    set_color(0);
    clrscr();
    before = clock(), tStart = clock(); //initialize time
    player.set_position(2,2);
    player.init(tStart);
    player.new_block();
    player.print_table();
    while (1) {
        if(mode == 1 && line >= goal)
            throw std::runtime_error("Goal Achieved");
        else if(mode == 2 && clock() - tStart >= goal * 1000)
            throw std::runtime_error("Goal Achieved");
        game_cycle(player, line, score, 1);
        Sleep(flush_tick);
    }
}

void multiPlayer(int& line, int& score){
    Table player, opponent;
    speed = 1.0, line = 0, score = 0;
    set_color(0);
    clrscr();
    before = clock(), tStart = clock();
    tStuck = clock();
    stuck_wait = fTick * 0.8;
    player.set_position(2, 2);
    opponent.set_position(35, 2);
    player.init(tStart);
    opponent.init();
    player.new_block();
    player.print_table();
    opponent.print_table();
    while (1) {
        game_cycle(player, line, score, 0);
        Sleep(flush_tick);
    }
}

void game_cycle(Table& player, int& line, int& score, bool single){
    getKeyState(); //get which key is pressed
    //fall
    if (clock() - before > fall_tick){
       stuck = !player.move_block(0,-1);
       before = clock();
       if(stuck){
          sCnt = 0;
          tStuck = clock();
       }
    }
    //if the block is stuck on the place for a long time
    if(stuck && clock() - tStuck > ((sCnt + 10) * fTick / 25) && !player.check_block(Point(0,-1))){
        //fix the block into place
        player.fix_block();
        if(player.chk_clear(line, score)){
            clr = 1;
            tClear = clock(); //reset timer
        }
        stuck = 0, sCnt = 0;
        player.new_block();
        player.print_table();
        KeyState[7] = 0; //reset the key state
    }
    //down arrow
    fall_tick = (KeyPressed[1])?(fTick*speed*(51 - gravity)/50):(fTick*speed);
    //left/right arrow
    for(int i = 0;i < 2;i++){
        if (KeyPressed[2 + i]) {
            if (KeyState[2 + i]){
                //the button haven't been released
                
                //if the arrow key is pressed for too long
                if(clock() - tDas > (1001 - das) * speed && clock() - tArrow > (501 - arr) * speed){
                    tArrow = clock(); //reset arr timer
                    player.move_block((i%2?1:-1), 0); //move according to the pressed button
                    //start counting down the lock timer if stuck
                    if(stuck)
                    	stuck_wait += (sCnt++ >= sLimit)?0:(fTick*2/25);
                }
            }
            else{
                //the button have been pressed just now
                player.move_block((i%2?1:-1), 0);
                tDas = clock(), tArrow = clock();
                //start counting down the lock timer if stuck
                if(stuck)
                	stuck_wait += (sCnt++ >= sLimit)?0:(fTick*2/25);
            }
            KeyState[2 + i] = 1; //change key state to prevent multiple move
        }
        else
            KeyState[2 + i] = 0; //change key state back
    }
    //Up(0), Z(5), X(6) even:clockwise
    for(int i = 0;i < 3;i++){
        if(KeyPressed[i + (i?4:0)]){
            if(!KeyState[i + (i?4:0)]){
                //the button have been pressed just now
                
                player.rotate((i%2?-1:1)); //rotate the block according to the button pressed
                //start counting down the lock timer if stuck
                if(stuck)
                	stuck_wait += (sCnt++ >= sLimit)?0:(fTick*2/25);
            }
            KeyState[i + (i?4:0)] = 1; //change the key state to prevent rotating for multiple times
        }
        else
            KeyState[i + (i?4:0)] = 0; //change the key state back
    }
    //C(7), Shift(8)
    for(int i = 0;i < 2;i++){
        if (KeyPressed[i + 7]) {
            if(!KeyState[7]){ //if not hold before
                player.hold_block(); //hold
                stuck = 0, sCnt = 0; //reset t-spin criteria and stuck state
                KeyState[7] = 1; //change the key state to prevent it being hold for multiple times
                player.print_table();
            }
        }
    }
    //space(4)
    if (KeyPressed[4]){
        if(!KeyState[4]){
            player.hard_drop();
            stuck = 0; //reset stuck state
            player.fix_block();
            if(player.chk_clear(line, score)){ //if clear any line
                clr = 1;
                tClear = clock();
            }
            player.new_block();
            player.print_table();
            KeyState[7] = 0; //reset hold state
        }
        KeyState[4] = 1; //change the key state to prevent multiple hard drop
    }
    else
        KeyState[4] = 0; //change the key state back
    
    //p:pause (9)
    if(single){
        if (KeyPressed[9]){
            if (!KeyState[9]){
                set_color(0);
                clrscr();
                SetFont(26);
                set_color(7);
                goto_xy(1, 1);
                std::cout << "You have paused the game";
                goto_xy(1, 2);
                std::cout << "Press R to resume the game";
                goto_xy(1, 3);
                std::cout << "Press Q to quit the game";
                while(1){
                    getKeyState();
                    if(KeyPressed[11])
                        break;
                    if(KeyPressed[10])
                        quit();
                    Sleep(flush_tick);
                }
                set_color(0);
                clrscr();
                SetFont(26, 1);
                player.print_table();
            }
            KeyState[9] = 1;
        }
        else
            KeyState[9] = 0;
    }
    //q:quit(10)
    if (KeyPressed[10]){
        if(!KeyState[10]){
            quit();
            set_color(0);
            clrscr();
            SetFont(26, 1);
            player.print_table();
        }
        KeyState[10] = 1;
    }
    else
        KeyState[10] = 0;
    player.print_block(); //print out the block
    if(clr && clock() - tClear >= 2 * fTick * speed){ //print out play time
        set_color(0);
        goto_xy(player.get_x() + 18, player.get_y() + 17);
        std::cout << "      ";
        goto_xy(player.get_x() + 18, player.get_y() + 18);
        std::cout << "      ";
        goto_xy(player.get_x() + 18, player.get_y() + 19);
        std::cout << "      ";
        goto_xy(player.get_x() + 18, player.get_y() + 20);
        std::cout << "      ";
        clr = 0;
    }
    speed = (1.0 - 0.032 * player.get_level()); //set the speed of the block
}

void quit(){
    KeyState[10] = 1;
    set_color(0);
    SetFont(26);
    clrscr();
    set_color(7);
    goto_xy(1, 1);
    std::cout << "Are you sure you want to quit the game?";
    goto_xy(1, 2);
    std::cout << "If yes please press Q";
    goto_xy(1, 3);
    std::cout << "If no please press R to resume the game";
    while(1){
        getKeyState();
        if(KeyPressed[10]){
            if(!KeyState[10])
                throw std::runtime_error("Quit");
            KeyState[10] = 1;
        }
        else
            KeyState[10] = 0;
        if(KeyPressed[11])
            return;
        Sleep(flush_tick);
    }
}

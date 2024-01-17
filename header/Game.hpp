#pragma once

#define KeyCnt 10
//Up, Down, Left, Right, Space, Z, X, C, Shift, ESC(pause)
const Keyboard::Key KeyCode[KeyCnt] = {Keyboard::Up, Keyboard::Down, Keyboard::Left, Keyboard::Right, Keyboard::Space, Keyboard::Z, Keyboard::X, Keyboard::C, Keyboard::LShift, Keyboard::Escape};
double speed;
bool KeyPressed[KeyCnt] = {}, KeyState[KeyCnt] = {}, stuck, clr, f_exit; //exit flag
const short fTick = 1000, sLimit = 10;
short fall_tick, stuck_wait, sCnt, ClearCnt, status;
//arrow:Left, Right
clock_t before, tStuck, tArrow, tDas, tStart;
//thread para and return value
int thrd_token, ret_thrd_val;

void pause_menu();
void ready_conn();
void wait_display();
void getKeyState() {for(int i = 0;i < KeyCnt;i++) KeyPressed[i] = Keyboard::isKeyPressed(KeyCode[i]);}
short game_cycle(Player& player, int& line, int& score,const bool& single);

void singlePlayer(int& line, int& score, const int& mode = 0, const int& goal = 60){ //mode:0(infinite), 1 (line, line), 2(time, second)
    Player player; //create new Table for player
    speed = 1.0, stuck = 0, line = 0, score = 0, status = 0, tStart = clock();
    //initialize the game
    player.set_position(2,2);
    player.init(clock(), 0);
    player.new_block();
    player.print_table();
    set_unit(20 * width + 200.f, 20 * height + 50.f);
    while (1) {
    	while (window.pollEvent(event)){
            if(event.type == Event::Closed)
                throw runtime_error("Quit");
        }
        //run the game
        if(mode == 1 && clock() - tStart >= goal * 1000)
            throw runtime_error("Goal Achieved");
        else if(mode == 2 && line >= goal)
            throw runtime_error("Goal Achieved");
        window.clear();
        status = game_cycle(player, line, score, 1);
        if(status < 0)
            throw runtime_error("Game over!");
        window.display();
        sleep(milliseconds(flush_tick));
    }
}

void multiPlayer(int& line, int& score){
    char BoardData[DataSize], RecvBoard[DataSize];
    width = 10, height = 20; //reset table size
    speed = 1.0, stuck = 0, line = 0, score = 0;
    tStart = before = clock();
    Thrd_token = 0, Thrd_ret = 0;
    thread wait_thrd(ready_conn);
    wait_display();
    wait_thrd.join();
    if(Thrd_ret == -1)
        throw runtime_error("Opponent Exit, you win!");
    else if(Thrd_ret == -2)
        throw runtime_error("Quit, you lose!");
    //initialize the game
    Player player; Opponent opponent; //create table for player and opponent
    set_unit(800, 450);
    player.set_position(2 * unit, 2 * unit);
    opponent.set_position(401 * unit, 2 * unit);
    player.init(clock(), 1);
    opponent.init();
    player.new_block();
    Thrd_token = 1, Thrd_ret = 0;
    player.print_table();
    opponent.print_table();
    memset(BoardData, 0, sizeof(BoardData));
    memset(RecvBoard, 0, sizeof(RecvBoard));
    thread Socket_thrd(Table_Trans, ref(BoardData), ref(RecvBoard));
    while (1) {
        while (window.pollEvent(event)){
            if(event.type == Event::Closed){
                Thrd_lock.lock();
                Thrd_token = -1;
                Thrd_lock.unlock();
                Socket_thrd.join();
                throw runtime_error("Quit, you lose!");
            }
        }
        //run the multi-player game
        window.clear();
        status = game_cycle(player, line, score, 0);
        if(status == -1){
            Thrd_lock.lock();
            Thrd_token = -1;
            Thrd_lock.unlock();
            socket_send("lose");
            Socket_thrd.join();
            throw runtime_error("You lose!");
        }
        Thrd_lock.lock();
        player.SendTable(BoardData);
        if(status > 0)
            BoardData[110] += status;
        if(Thrd_ret == 1){
            if(!strcmp(RecvBoard, "win")){
                Thrd_token = -1;
                Thrd_lock.unlock();
                Socket_thrd.join();
            	throw runtime_error("You lose!");
            }
            else if(!strcmp(RecvBoard, "lose")){
                Thrd_token = -1;
                Thrd_lock.unlock();
                Socket_thrd.join();
            	throw runtime_error("You win!");
            }
            player.get_garbage(RecvBoard[110]);
            RecvBoard[110] = 0;
            opponent.RecvTable(RecvBoard);
            Thrd_ret = 0;
        }
        else if(Thrd_ret < 0){
            Thrd_lock.unlock();
            Socket_thrd.join();
            throw runtime_error("Opponent Exit, you win!");
        }
        Thrd_lock.unlock();
        opponent.print_table();
        window.display();
        sleep(milliseconds(flush_tick));
    }
}

short game_cycle(Player& player, int& line, int& score, const bool& single){
    ClearCnt = 0;
    getKeyState(); //get which key is pressed
    //fall
    if (clock() - before > fall_tick){
        if(!player.move_block(0,-1) && !stuck){
            stuck = 1;
            sCnt = 0;
            tStuck = clock();
            stuck_wait = (fTick * 4 / 5);
        }
        before = clock();
    }
    //if the block is stuck on the place for a long time
    if(stuck && clock() - tStuck > stuck_wait && !player.check_block(Point(0,-1))){
        //fix the block into place
        player.fix_block();
        ClearCnt = player.chk_clear(line, score);
        stuck = 0, sCnt = 0;
        player.new_block();
        KeyState[7] = 0; //reset the key state
    }
    //down arrow
    fall_tick = (KeyPressed[1])?(fTick*speed*(51 - conf[2])/50):(fTick*speed);
    //left/right arrow
    for(int i = 0;i < 2;i++){
        if (KeyPressed[2 + i]) {
            if (KeyState[2 + i]){
                //the button haven't been released
                //if the arrow key is pressed for too long
                if(clock() - tDas > (1001 - conf[0]) * speed && clock() - tArrow > (501 - conf[1]) * speed){
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
                	stuck_wait += (sCnt++ >= sLimit)?0:(fTick/10);
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
                	stuck_wait += (sCnt++ >= sLimit)?0:(fTick/10);
            }
            KeyState[i + (i?4:0)] = 1; //change the key state to prevent rotating for multiple times
        }
        else
            KeyState[i + (i?4:0)] = 0; //change the key state back
    }
    //C(7), Shift(8)
    for(int i = 0;i < 2;i++){
        if(KeyPressed[i + 7]) {
            if(!KeyState[7]){ //if not hold before
                player.hold_block(); //hold
                stuck = 0, sCnt = 0; //reset t-spin criteria and stuck state
                KeyState[7] = 1; //change the key state to prevent it being hold for multiple times
            }
        }
    }
    //space(4)
    if (KeyPressed[4]){
        if(!KeyState[4]){
            player.hard_drop();
            stuck = 0; //reset stuck state
            player.fix_block();
            ClearCnt = player.chk_clear(line, score);
            player.new_block();
            KeyState[7] = 0; //reset hold state
        }
        KeyState[4] = 1; //change the key state to prevent multiple hard drop
    }
    else
        KeyState[4] = 0; //change the key state back
    //p:pause (9)
    if(single && KeyPressed[9]){
        pause_menu();
        set_unit(20 * width + 200.f, 20 * height + 50.f);
    }
    player.print_table(); //print out the block
    speed = (1.0 - 0.032 * player.get_level()); //set the speed of the block
    return ClearCnt;
}
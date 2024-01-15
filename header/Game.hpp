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
void getKeyState() {for(int i = 0;i < KeyCnt;i++) KeyPressed[i] = Keyboard::isKeyPressed(KeyCode[i]);}
void Table_Trans(char Snd[], char Rec[]);
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
    	window.clear();
        //run the game
        if(mode == 1 && clock() - tStart >= goal * 1000)
            throw runtime_error("Goal Achieved");
        else if(mode == 2 && line >= goal)
            throw runtime_error("Goal Achieved");
        status = game_cycle(player, line, score, 1);
        if(status < 0)
            throw runtime_error("Game over!");
        window.display();
        sleep(milliseconds(flush_tick));
    }
}
/*
void multiPlayer(int& line, int& score){
    int mode = 0, goal = 0, iTmp, status;
    width = 10, height = 20; //reset table size
    speed = 1.0, stuck = 0, line = 0, score = 0;
    tStart = before = clock();
    char BoardData[DataSize], RecvBoard[DataSize];
    //chk join
    cout << "Please wait for your opponent joining!\n";
    do{
        if(server){
            if(server_send("Ready"))
                throw runtime_error("Opponent have exited");
            server_recv(BoardData);
        }
        else{
            if(client_send("Ready"))
                throw runtime_error("Opponent have exited");
            client_recv(BoardData);
        }
    }while(strcmp(BoardData, "Ready"));
    //choose mode
    if(server){
        cout << "Please choose your mode!\n1(Infinite Mode)/2(Line Mode):";
        while(1){
            cin >> mode;
            if(mode == 1 || mode == 2){
                mode--;
                cout << "Mode choose : " << (mode?"Line Mode\n":"Infinite Mode\n");
                if(!mode) break;
                cout << "Please input your goal (line)? ";
                while(1){
                    cin >> goal;
                    if(goal > 0)
                        break;
                    else{
                        cin.clear();
                        fflush(stdin); //flush the cin buffer to prevent it from reading it again
                        cout << "Please input a number bigger than 0:\n";
                    }
                } 
                break;
            }
            else{
                cin.clear();
                fflush(stdin);
                cout << "Please input 1 or 2: ";
            }
        }
        strcpy(BoardData, "Mode set");
        BoardData[10] = mode;
        for(int i = 11, tGoal = goal;i < 15;i++,tGoal>>=7)
            BoardData[i] = (tGoal & 127);
        if(server_send(BoardData))
            throw runtime_error("Opponent have exited");
    }
    else{
        cout << "Please wait for the host select mode...\n";
        do{
            client_recv(BoardData);
            if(client_send("chk"))
                throw runtime_error("Opponent have exited");
        }while(strcmp(BoardData, "Mode set"));
        mode = BoardData[10];
        goal = 0;
        for(int i = 14;i >= 11;i--){
            goal <<= 7;
            goal |= BoardData[i];
        }
        cout << "========================\nMode set : " << (mode?"Line Mode\n":"Infinite Mode\n");
        if(mode)
            cout << "Goal set(line) : " << goal;
        cout << "========================\nIf you want to start, please input 1\nIf you want quit, please input 2\n";
        while(1){
            cin >> iTmp;
            if(iTmp == 1){
                client_send("Start");
                break;
            }
            else if(iTmp ==2){
                client_send("quit");
                client_disconn();
                throw runtime_error("Quit");
            }
            else{
                cin.clear();
                fflush(stdin);
                cout << "Please input 1 or 2: ";
            }
        }
    }
    //initialize the game
    Player player;Opponent opponent; //create table for player and opponent
    player.set_position(2, 2);
    opponent.set_position(35, 2);
    player.init(clock());
    opponent.init();
    player.new_block();
    thrd_token = 0, ret_thrd_val = 0;
    set_color(7);
    if(server){
        cout << "Please wait for your opponent starting!";
        do{
            server_recv(BoardData);
        	if(server_send("chk"))
        		throw runtime_error("Opponent have exited");
        }while(strcmp(BoardData, "Start"));
    }
    clrscr();
    SetFont(1, 68, 30);
    player.print_table();
    opponent.print_table();
    memset(BoardData, 0, sizeof(BoardData));
    memset(RecvBoard, 0, sizeof(RecvBoard));
    thread Socket_thrd(Table_Trans, ref(BoardData), ref(RecvBoard));
    while (1) {
        //run the multi-player game
        status = game_cycle(player, line, score, 0);
        if(mode == 1 && line >= goal){
        	if(server)
        		server_send("win");
        	else
        		client_send("win");
        	thrd_token = -1;
            Socket_thrd.join();
            throw runtime_error("You win!");
        }
        if(status == -10)
			opponent.print_table();
        else if(status == -1){
            thrd_token = -1;
            if(server)
                server_send("lose");
            else
                client_send("lose");
            Socket_thrd.join();
            throw runtime_error("You lose!");
        }
        Thrd_lock.lock();
        if(status > 0)
            BoardData[110] += status;
        player.SendTable(BoardData);
        Thrd_lock.unlock();
        if(ret_thrd_val == 1){
            Thrd_lock.lock();
            if(!strcmp(RecvBoard, "win")){
                thrd_token = -1;
                Socket_thrd.join();
            	throw runtime_error("You lose!");
            }
            else if(!strcmp(RecvBoard, "lose")){
                thrd_token = -1;
                Socket_thrd.join();
            	throw runtime_error("You win!");
            }
            player.get_garbage(RecvBoard[110]);
            opponent.RecvTable(RecvBoard);
            ret_thrd_val = 0;
            Thrd_lock.unlock();
        }
        else if(ret_thrd_val < 0){
            Socket_thrd.join();
            throw runtime_error("Opponent Exit, you win!");
        }
        thrd_token = 1;
    }
}*/

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

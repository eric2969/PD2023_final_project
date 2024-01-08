#pragma once

#define KeyCnt 11
//Up, Down, Left, Right, Space, Z, X, C, Shift, P(pause), Q(quit)
const int KeyCode[KeyCnt] = {VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_SPACE, VK_Z, VK_X, VK_C, VK_SHIFT, VK_P, VK_Q};
double speed;
bool KeyPressed[KeyCnt] = {}, KeyState[KeyCnt] = {}, stuck, clr;
const short fTick = 1000, sLimit = 10;
short fall_tick, stuck_wait, sCnt, ClearCnt, status;
//arrow:Left, Right
clock_t before, tStuck, tClear, tStart, tArrow, tDas;
//thread lock
mutex Thrd_lock;
//thread para and return value
int thrd_token, ret_thrd_val;

struct SettingMenu{ //settings
    static void sub_option1(){
        cout << "Current ARR is: " << arr <<"\nPlease Type in the New ARR\n1(slow)-500(fast): ";
        while(1){
            cin >> arr;
            if(arr > 0 && arr <= 500){  
                clrscr();
                set_color(7);
                cout << "Configuration Set\n\n";
                Sleep(800);
                pause();
                break;
            }
            else{
                cin.clear();
                fflush(stdin); //flush the cin buffer to prevent it from reading it again
                cout << "Please input a number between than 1 and 500: ";
            }
        }
    }
    static void sub_option2(){
        cout << "Current DAS is: " << das <<"\nPlease Type in the New DAS\n1(slow)-1000(fast): ";
        while(1){
            cin >> das;
            if(das > 0 && das <= 1000){
                clrscr();
                set_color(7);
                cout << "Configuration Set\n\n";
                Sleep(800);
                pause();
                break;
            }
            else{
                cin.clear();
                fflush(stdin); //flush the cin buffer to prevent it from reading it again
                cout << "Please input a number between than 1 and 1000: ";
            }
        }

    }
    static void sub_option3(){
         cout << "Current Gravity Level is: " << gravity <<"\nPlease Type in the New Gravity Level\n1(slow)-50(fast): ";
         while(1){
            cin >> gravity;
            if(gravity > 0 && gravity <= 50){
                clrscr();
                set_color(7);
                cout << "Configuration Set\n\n";
                Sleep(800);
                pause();
                break;
            }
            else{
                cin.clear();
                fflush(stdin);//flush the cin buffer to prevent it from reading it again
                cout << "Please input a number between than 1 and 50: ";
            }
         }
    }
    static void sub_option4(){
    	int iTmp;
        cout << "Current Bright Mode is: " << (bright?"Bright":"Dark") <<"\nPlease Type in the New Bright Mode\n1(Dark)/2(Bright): ";
        while(1){
            cin >> iTmp;
            if(iTmp == 1 || iTmp == 2){
                bright = iTmp - 1;
                clrscr();
                set_color(7);
                cout << "Configuration Set\n\n";
                Sleep(800);
                pause();
                break;
            }
            else{
                cin.clear();
                fflush(stdin);
                cout << "Please input 1 or 2: ";
            }
        }
    }
    static void sub_option5() {
    	int iTmp;
        cout << "Are you sure you want to reset the settings? 1(No)/2(Yes): ";
        while(1){
            cin >> iTmp;
            if(iTmp == 1)
                break;
            else if(iTmp == 2){
            	das = 700; arr = 450; gravity = 45; bright = 1;
            	cout << "Reset Completed\n";
            	Sleep(800);
            	pause();
            	break;
			}
            else{
                cin.clear();
                fflush(stdin);
                cout << "Please input 1 or 2: ";
            }
        }
	}
    void operator() (){
        Menu sub_menu;
        clrscr();
        sub_menu.settitle("Game Setting\nRight click for return to main menu").add(sub_option1, "ARR").add(sub_option2, "DAS").add(sub_option3, "Gravity").add(sub_option4, "Bright").add(sub_option5, "Reset");
        sub_menu.start();
        ofstream setting(SET_PATH);
        setting << das << ' ' << arr << ' ' << gravity << ' ' << bright;
        setting.close();
    }
};

static void Quit() {
    if(multi){
        if(server)
            server_send("lose");
        else
            client_send("lose");
        throw runtime_error("Quit, you lose!");
    }
    else
    	throw runtime_error("Quit");
}

void getKeyState() {for(int i = 0;i < KeyCnt;i++) KeyPressed[i] = GetAsyncKeyState(KeyCode[i]) & 0x8000;}
void Table_Trans(char Snd[], char Rec[]);
short game_cycle(Player& player, int& line, int& score,const bool& single);

void singlePlayer(int& line, int& score, const int& mode, const int& goal){ //mode:0(infinite), 1 (line, line), 2(time, second)
    Player player; //create new Table for player
    speed = 1.0, stuck = 0, line = 0, score = 0, status = 0;
    tStart = before = clock();
    clrscr();
    //initialize the game
    player.set_position(2,2);
    player.init(clock());
    player.new_block();
    SetFont(1, width + 30, height + 7);
    player.print_table();
    while (1) {
        //run the game
        if(mode == 1 && line >= goal)
            throw runtime_error("Goal Achieved");
        else if(mode == 2 && clock() - tStart >= goal * 1000)
            throw runtime_error("Goal Achieved");
        status = game_cycle(player, line, score, 1);
        if(status < 0)
            throw runtime_error("Game over!");
        Sleep(flush_tick);
    }
}

void multiPlayer(int& line, int& score){
    int mode = 0, goal = 0, iTmp, status;
    width = 10, height = 20; //reset table size
    speed = 1.0, stuck = 0, line = 0, score = 0;
    tStart = before = clock();
    char BoardData[DataSize], RecvBoard[DataSize];
    clrscr();
    set_color(7);
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
}

void Table_Trans(char Snd[], char Rec[]){
    while(thrd_token != 1) {Sleep(flush_tick);}
    char tmp[DataSize];
    while(thrd_token > 0){
        Thrd_lock.lock();
        for(int i = 0;i < DataSize;i++) tmp[i] = Snd[i];
        Snd[110] = 0;
        Thrd_lock.unlock();
        if(server){
            if(server_send(tmp)){
                ret_thrd_val = -1;
                break;
            }
            if(server_recv(tmp)){
                ret_thrd_val = -1;
                break;
            }
        }
        else{
            if(client_send(tmp)){
                ret_thrd_val = -1;
                break;
            }
            if(client_recv(tmp)){
                ret_thrd_val = -1;
                break;
            }
        }
        if(!strcmp(tmp, "chk"))
            continue;
        Thrd_lock.lock();
        for(int i = 0;i < DataSize;i++) Rec[i] = tmp[i];
        ret_thrd_val = 1;
        Thrd_lock.unlock();
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
        if(ClearCnt){
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
            ClearCnt = player.chk_clear(line, score);
	        if(ClearCnt){
	            clr = 1;
	            tClear = clock(); //reset timer
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
                clrscr();
                SetFont(0, 100);
                Menu PauseMenu; SettingMenu SM;//make the pause menu
                PauseMenu.settitle("Pause\nIf want to resume, please right click!").add(SM, "Settings").add(Quit, "Quit");
                PauseMenu.start(); //execute the menu
                clrscr();
                SetFont(1, width + 30, height + 7);
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
            clrscr();
            SetFont(0, 100);
            Menu QuitMenu;//make the quit menu
			QuitMenu.settitle("Are you sure you want to quit?\nIf no, please right click!").add(Quit, "Quit");
			QuitMenu.start(); // execute the menu
            clrscr();
            if(single)
            	SetFont(1, width + 20, height + 7);
            else
            	SetFont(1, 70, 30);
            player.print_table();
            return -10;
        }
        KeyState[10] = 1;
    }
    else
        KeyState[10] = 0;
    player.print_block(); //print out the block
    if(clr && clock() - tClear >= 2 * fTick * speed){ 
        //clear the message box to the left
        set_color(0);
        goto_xy(player.get_x() + width + 8, player.get_y() + height - 3);
        cout << "           ";
        goto_xy(player.get_x() + width + 8, player.get_y() + height - 2);
        cout << "      ";
        goto_xy(player.get_x() + width + 8, player.get_y() + height - 1);
        cout << "      ";
        goto_xy(player.get_x() + width + 8, player.get_y() + height);
        cout << "      ";
        clr = 0;
    }
    speed = (1.0 - 0.032 * player.get_level()); //set the speed of the block
    return ClearCnt;
}

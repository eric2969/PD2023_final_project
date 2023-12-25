#include <bits/stdc++.h>
#include <windows.h> 

using namespace std;

HANDLE hConsole;
bool bright;
int das, arr, gravity, addrlen;
const short flush_tick = 2;

#define SET_PATH "src/settings.txt"
#define PIC_PATH "src/pic.txt"
#define RECORD_PATH "src/records.txt"

#include "VK.h"
#include "Server.h"
#include "Client.h"
#include "Console.h"
#include "Block.h"
#include "Table.h"
#include "Menu.h"
#include "Game.h"

int playCnt, TimeCnt, clearCnt, scoreCnt, highClear, highScore;

void game_init();
void game_exit();
void record_reset() {playCnt = 0, TimeCnt = 0, clearCnt = 0, scoreCnt = 0, highClear = 0, highScore = 0;}
void record_update(int& clr, int& score, const int& time);
void print_pic();

struct option1{ //option from single player mode
    static int line, score, usedTime, gameMode, goal;
    static clock_t t;
    static void run(){ //execute the sub-menu
        t = clock();
        SetFont(26, 1);
        try{singlePlayer(line, score, gameMode, goal);}
        catch(runtime_error e){ 
                //game over
        		usedTime = (clock() - t) / 1000;
                print_pic();
                Sleep(1000);
                SetFont();
                clrscr();
                set_color(7);
                cout << e.what() << endl << endl;
                cout << "Used Time(s):  " << usedTime << endl;
                cout << "Clear Line:    " << line << endl;
                cout << "Score:         " << score << endl << endl;
                Sleep(800);
                pause();
        }
        record_update(line, score, usedTime); //update record
    }
    static void sub_option1(){ //infinite mode
        gameMode = 0;
        run();
    }
    static void sub_option2(){ //line mode
        cout << "Please input your goal (line)?" << endl;
        while(1){
            cin >> goal;
            if(goal > 0)
                break;
            else{
                cin.clear();
                fflush(stdin); //flush the cin buffer to prevent it from reading it again
                cout << "Please input a number bigger than 0" << endl;
            }
        }
        gameMode = 1;
        run();
    }
    static void sub_option3(){ //time mode
        cout << "Please input your time (second)?" << endl;
        while(1){
            cin >> goal;
            if(goal > 0)
                break;
            else{
                cin.clear();
                fflush(stdin); //flush the cin buffer to prevent it from reading it again
                cout << "Please input a number bigger than 0" << endl;
            }
        }
        gameMode = 2;
        run();
    }
    void operator() (){
        Menu sub_menu; //create sub-menu
        clrscr();
        sub_menu.settitle("Single Game\nChoose a Game Mode\nRight click for return to main menu").add(sub_option1, "Infinite Mode").add(sub_option2, "Clear Line Mode").add(sub_option3, "Time Mode");
        sub_menu.start();
    }
};
int option1::gameMode = 0;
int option1::usedTime = 0;
int option1::line = 0;
int option1::score = 0;
int option1::goal = 0;
clock_t option1::t = 0;

struct option2{ //option from multi-player
    static int line, score, usedTime;
    static clock_t t;
    static void run(){ //execute sub-menu
    	t = clock();
        SetFont(22, 1);
        try{multiPlayer(line, score);}
        catch(runtime_error e){ 
                //game over
        		usedTime = (clock() - t) / 1000;
                print_pic();
                Sleep(1000);
                SetFont();
                clrscr();
                set_color(7);
                cout << e.what() << endl << endl;
                cout << "Used Time(s):  " << usedTime << endl;
                cout << "Clear Line:    " << line << endl;
                cout << "Score:         " << score << endl << endl;
                Sleep(800);
                pause();
        }
        record_update(line, score, usedTime); //update record
	}
	static void sub_option1(){
        run();
    }
    void operator() (){
		Menu sub_menu; //create sub-menu
        clrscr();
        sub_menu.settitle("Multi Game\nChoose a Game Mode\nRight click for return to main menu").add(sub_option1, "Game Start");
        sub_menu.start();
    }
};
int option2::line = 0;
int option2::score = 0;
int option2::usedTime = 0;
clock_t option2::t = 0;

struct option3{ //record option
    static int iTmp;
    static void sub_option1(){ //show record
        clrscr();
        set_color(7);
        cout << "Records\n\n";
        cout << "Game Played:         " << playCnt << endl;
        cout << "Total Play Time(s):  " << TimeCnt << endl;
        cout << "Total Clear Line:    " << clearCnt << endl;
        cout << "Total Score:         " << scoreCnt << endl;
        cout << "Best Clear Line:     " << highClear << endl;
        cout << "Best Score:          " << highScore << endl << endl;
        Sleep(800);
        pause(); //press any key to continue
    }
    static void sub_option2(){ //reset record
        cout << "Are you sure you want to reset your record?\n[1(No)/2(Yes)]: ";
        while(1){
            cin >> iTmp;
            if(iTmp == 1)
                break;
            else if(iTmp == 2){
                record_reset();
                clrscr();
                set_color(7);
                cout << "Record Reset\n\n";
                Sleep(800);
                pause();
                break;
            }
            else{ //invalid value
                cin.clear();
                fflush(stdin); //flush the cin buffer to prevent it from reading it again
                cout << "Please input 1 or 2: ";
            }
        }
    }
    void operator() (){
        Menu sub_menu; //create sub-menu
        clrscr();
        sub_menu.settitle("Record\nRight click for return to main menu").add(sub_option1, "See Record").add(sub_option2, "Reset Record");
        sub_menu.start();
        ofstream record(RECORD_PATH);
        //write new record
        record << playCnt << ' ' << TimeCnt << ' ' << clearCnt << ' ' << scoreCnt << ' ' << highClear << ' ' << highScore;
        record.close(); //close file
    }
};
int option3::iTmp = 0;

struct option4{ //settings
    static int iTmp;
    static void sub_option1(){
        cout << "Current ARR is: " << arr <<"\nPlease Type in the New ARR\n[1(slow)-500(fast)]: ";
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
        cout << "Current DAS is: " << das <<"\nPlease Type in the New DAS\n[1(slow)-1000(fast)]: ";
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
         cout << "Current Gravity Level is: " << gravity <<"\nPlease Type in the New Gravity Level\n[1(slow)-50(fast)]: ";
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
        cout << "Current Bright Mode is: " << (bright?"Bright":"Dark") <<"\nPlease Type in the New Bright Mode\n[1(Dark)/2(Bright)]: ";
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
    void operator() (){
        Menu sub_menu;
        clrscr();
        sub_menu.settitle("Game Setting\nRight click for return to main menu").add(sub_option1, "ARR").add(sub_option2, "DAS").add(sub_option3, "Gravity").add(sub_option4, "Bright");
        sub_menu.start();
        ofstream setting(SET_PATH);
        setting << das << ' ' << arr << ' ' << gravity << ' ' << bright;
        setting.close();
    }
};
int option4::iTmp = 0;

struct option5{ //exit game
    void operator() (){
        game_exit();
    }
};

signed main(){
    //initialize the main menu
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    FullScreen();
    SetFont();
    game_init();
    Menu Main;
    option1 opt1;
    option2 opt2;
    option3 opt3;
    option4 opt4;
    option5 opt5;
    Main.settitle("Tetris").add(opt1, "Single Player").add(opt3, "Records").add(opt4, "Settings").add(opt5, "Quit");
    Main.start();
    return 0;
}

void game_init(){
    // read user preference
    ifstream setting(SET_PATH), record(RECORD_PATH);
    if(setting.is_open())
        setting >> arr >> das >> gravity >> bright;
    else{
        set_color(7);
        goto_xy(2, 2);
        cout << "Configuration data loaded fail, use default setting\nYou can modify it in the menu";
        pause();
    }
    if(record.is_open())
        record >> playCnt >> TimeCnt >> clearCnt >> scoreCnt >> highClear >> highScore;
    else{
        set_color(7);
        goto_xy(2, 2);
        cout << "Recode data loaded fail, record has reset";
        pause();
    }
    setting.close();
    record.close();
}

void game_exit(){
    //save user preference
    ofstream setting(SET_PATH), record(RECORD_PATH);
    setting << das << ' ' << arr << ' ' << gravity << ' ' << bright;
    record << playCnt << ' ' << TimeCnt << ' ' << clearCnt << ' ' << scoreCnt << ' ' << highClear << ' ' << highScore;
    setting.close();
    record.close();
    exit(0);
}

void record_update(int& clr, int& score,const int& time){
    playCnt++;
    TimeCnt += time;
    clearCnt += clr;
    scoreCnt += score;
    highClear = max(highClear, clr);
    highScore = max(highScore, score);
}

void print_pic(){
    clrscr();
    SetFont(5, 1);
    goto_xy(0,0);
    set_color(7);
    COORD buffersize;
    buffersize.X = 600;
    buffersize.Y = 600;
    HWND hwnd = GetConsoleWindow();
    SetConsoleScreenBufferSize(hConsole, buffersize); //resize console buffer
    SetConsoleSize(600, 600, 600, 600); //resize console
    ifstream pic(PIC_PATH);
    string str;
    while(getline(pic,str)) cout << str << endl; //print out the picture
    pic.close();
}


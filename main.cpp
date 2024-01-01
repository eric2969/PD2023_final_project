#include <bits/stdc++.h>
#include <windows.h> 

using namespace std;

HANDLE hConsole;
bool bright, pic_ava, conn, server, multi;
int das, arr, gravity, addrlen;
int ResX, ResY;
const short flush_tick = 2, port = 9487, DataSize = 130;

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
    static Menu sub_menu;
    static int line, score, usedTime, gameMode, goal;
    static clock_t t;
    static void run(){ //execute the sub-menu
        multi = 0;
        t = clock();
        try{singlePlayer(line, score, gameMode, goal);}
        catch(runtime_error e){ 
            //game over
    		usedTime = (clock() - t) / 1000;
    		if(pic_ava){
    			print_pic();
            	Sleep(1000);
			}
            clrscr();
            SetFont(0, 100);
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
        cout << "Please input your goal (line)?: ";
        while(1){
            cin >> goal;
            if(goal > 0)
                break;
            else{
                cin.clear();
                fflush(stdin); //flush the cin buffer to prevent it from reading it again
                cout << "Please input a number bigger than 0: ";
            }
        }
        gameMode = 1;
        run();
    }
    static void sub_option3(){ //time mode
        cout << "Please input your time (second)?: " ;
        while(1){
            cin >> goal;
            if(goal > 0)
                break;
            else{
                cin.clear();
                fflush(stdin); //flush the cin buffer to prevent it from reading it again
                cout << "Please input a number bigger than 0: ";
            }
        }
        gameMode = 2;
        run();
    }
    static void set_size(){
        cout << "Please input Table width (5-50, default:10)?: ";
        while(1){
            cin >> width;
            if(width >= 5 && width <= 50)
                break;
            else{
                cin.clear();
                fflush(stdin); //flush the cin buffer to prevent it from reading it again
                cout << "Please input a number between 5 and 50: ";
            }
        }
        cout << "Please input Table height (15-50, default:20)?: ";
        while(1){
            cin >> height;
            if(height >= 15 && height <= 50)
                break;
            else{
                cin.clear();
                fflush(stdin); //flush the cin buffer to prevent it from reading it again
                cout << "Please input a number between 15 and 50: ";
            }
        }
        cout << "Configuration Set...\n";
        char title[256], tmp[5];
        strcpy(title, "Single Game\nTable Width: ");
        itoa(width, tmp, 10);
        strcat(title, tmp);
        strcat(title, ", Height: ");
        itoa(height, tmp, 10);
        strcat(title, tmp);
        strcat(title, "\nChoose a Game Mode\nRight click for return to main menu");
        sub_menu.settitle(title);
        Sleep(800);
        pause();
    }
    void operator() (){
        clrscr();
        char title[256], tmp[5];
        sub_menu.init();
        strcpy(title, "Single Game\nTable Width: ");
        itoa(width, tmp, 10);
        strcat(title, tmp);
        strcat(title, ", Height: ");
        itoa(height, tmp, 10);
        strcat(title, tmp);
        strcat(title, "\nChoose a Game Mode\nRight click for return to main menu");
        sub_menu.settitle(title);
        sub_menu.add(sub_option1, "Infinite Mode").add(sub_option2, "Clear Line Mode").add(sub_option3, "Time Mode").add(set_size, "Set Table Size");
        sub_menu.start();
    }
};
int option1::gameMode = 0;
int option1::goal = 0;
int option1::usedTime = 0;
int option1::line = 0;
int option1::score = 0;
Menu option1::sub_menu;
clock_t option1::t = 0;

//under socket application
struct option2{ //option from multi-player
	static Menu sub_menu;
    static int line, score, usedTime;
    static clock_t t;
    static char ip[16];
    static void run(){ //execute sub-menu
        if(!conn){
            clrscr();
            set_color(7);
            cout << "Please connect to another player first\n";
            Sleep(800);
            pause();
            return;
        }
        multi = 1;
    	t = clock();
        try{multiPlayer(line, score);}
        catch(runtime_error e){ 
            //game over
        	usedTime = (clock() - t) / 1000;
            if(pic_ava){
        		print_pic();
               	Sleep(1000);
			}
			if(conn){
				if(server)
					server_disconn();
				else
					client_disconn();
			}
            sub_menu.settitle("Multi Game\nYou are disconnected\nRight click for return to main menu");
            SetFont(0, 100);
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
        clrscr();
        set_color(7);
        if(conn)
            cout << "You have connected\n";
        else{
            int status;
            cout << "Please input your ip(in setting)\n";
            cin >> ip;
            cout << "Waiting for connection...\n";
            status = server_connect(ip, port);
            if(status)
                cout << "Connect Error, error code : " << status << endl;
            else{
            	sub_menu.settitle("Multi Game\nYou are connected as a host!\nRight click for return to main menu");
                cout << "Connected successfully\n";
                conn = 1;
                server = 1;
            }
        }
        Sleep(800);
        pause();
    }
    static void sub_option2(){
        clrscr();
        set_color(7);
        if(conn)
            cout << "You have connected\n";
        else{
            int status;
            char title[256];
            cout << "Please input opponent's ip\n";
            cin >> ip;
            cout << "Waiting for connection...\n";
            status = client_connect(ip, port);
            if(status)
                cout << "Connect Error, error code : " << status << endl;
            else if(client_send("test"))
            	cout << "Failed, please connect after host starting connection!\n";
            else{
                strcpy(title, "Multi Game\nYou are guest!Host IP:");
                strcat(title, ip);
                strcat(title, "\nRight click for return to main menu");
                sub_menu.settitle(title);
                cout << "Connected successfully\n";
                conn = 1;
                server = 0;
            }
        }
        Sleep(800);
        pause();
    }
    static void sub_option3(){
        clrscr();
        set_color(7);
        if(conn){
            if(server)
                server_disconn();
            else
                client_disconn();
            sub_menu.settitle("Multi Game\nYou are disconnected\nRight click for return to main menu");
            cout << "Disconnected\n";
        }
        else
            cout << "No connection\n";
        Sleep(800);
        pause();
    }
    void operator() (){
        char title[256];
        clrscr();
        sub_menu.init();
        if(conn){
            if(server)
                if(server_send("test"))
                    sub_menu.settitle("Multi Game\nYou are disconnected\nRight click for return to main menu");
                else
                    sub_menu.settitle("Multi Game\nYou are connected as a host!\nRight click for return to main menu");
            else{
                if(client_send("test"))
                    sub_menu.settitle("Multi Game\nYou are disconnected\nRight click for return to main menu");
                else{
                    strcpy(title, "Multi Game\nYou are guest!Host IP:");
                    strcat(title, ip);
                    strcat(title, "\nRight click for return to main menu");
                    sub_menu.settitle(title);
                }
            }
        }
        else
            sub_menu.settitle("Multi Game\nYou are disconnected\nRight click for return to main menu");
        sub_menu.add(sub_option1, "Be a Host").add(sub_option2, "Connect to the Host").add(sub_option3, "Disconnect").add(run, "Game Start");
        sub_menu.start();
    }
};
Menu option2::sub_menu;
int option2::line = 0;
int option2::score = 0;
int option2::usedTime = 0;
char option2::ip[16];
clock_t option2::t = 0;

struct option3{ //record option
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
        int iTmp;
        cout << "Are you sure you want to reset your record?\n[1(No)/2(Yes)]: ";
        while(1){
            cin >> iTmp;
            if(iTmp == 1)
                break;
            else if(iTmp == 2){
                record_reset();
                clrscr();
                ofstream record(RECORD_PATH);
        		//write new record
        		record << playCnt << ' ' << TimeCnt << ' ' << clearCnt << ' ' << scoreCnt << ' ' << highClear << ' ' << highScore;
        		record.close(); //close file
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
    }
};

signed main(){
    //initialize the main menu
    DisableIME();
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    FullScreen();
    game_init();
    socket_init();
    SetFont(0, 100);
    Menu Main;
    option1 opt1;
    option2 opt2;
    option3 opt3;
    SettingMenu opt4;
    Main.settitle("Tetris").add(opt1, "Single Player").add(opt2, "Multi Player").add(opt3, "Records").add(opt4, "Settings").add(game_exit, "Quit");
    Main.start();
    return 0;
}

void game_init(){
	//load monitor resolution
	ResX = GetSystemMetrics(SM_CXSCREEN) - 30;
	ResY = GetSystemMetrics(SM_CYSCREEN) - 30;
    // read user preference
    ifstream setting(SET_PATH), record(RECORD_PATH), pic(PIC_PATH);
    if(setting.is_open())
        setting >> das >> arr >> gravity >> bright;
    else{
        set_color(7);
        cout << "Configuration data loaded fail, restore to default setting\nYou can modify it in the menu\n";
        das = 700; arr = 450; gravity = 45; bright = 1;
        ofstream set(SET_PATH);
        set << das << ' ' << arr << ' ' << gravity << ' ' << bright;
        set.close();
        pause();
        clrscr();
    }
    if(record.is_open())
        record >> playCnt >> TimeCnt >> clearCnt >> scoreCnt >> highClear >> highScore;
    else{
        set_color(7);
        cout << "Recode data loaded fail, record has reset\n";
        playCnt = 0, TimeCnt = 0, clearCnt = 0, scoreCnt = 0, highClear = 0, highScore = 0;
        ofstream rec(RECORD_PATH);
        rec << playCnt << ' ' << TimeCnt << ' ' << clearCnt << ' ' << scoreCnt << ' ' << highClear << ' ' << highScore;
        rec.close();
        pause();
        clrscr();
    }
    if(!(pic_ava = pic.is_open())){
    	pic_ava = 0;
    	set_color(7);
    	cout << "Picture file loaded fail, picture function disabled\n";
    	cout << "To restore, please download data from Github to src/pic.txt\n";
    	pause();
        clrscr();
	}
	pic.close();
    setting.close();
    record.close();
}

void game_exit(){
	//close socket
	if(conn){
		if(server)
			server_quit();
		else
			client_quit();
	}
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
	if(!pic_ava)
		return;
    clrscr();
    SetFont(1, 150, 150);
    goto_xy(0,0);
    set_color(7);
    SetConsoleSize(900, 900); //resize console
    ifstream pic(PIC_PATH);
    string str;
    while(getline(pic,str)) cout << str << endl; //print out the picture
    pic.close();
}


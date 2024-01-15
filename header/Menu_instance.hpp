#pragma once

#include "Object.hpp"
#include "Game.hpp"

const string RecTitle[6] = {
    "Game Played:      ",
    "Played Time(s):   ",
    "Total Clear Line: ",
    "Total Score:      ",
    "Best Clear Line:  ",
    "Best Score:       "
};

const string OverDisplay[3] = {
    "Duration(s):",
    "Clear Line: ",
    "Score:      "
};

void chk_hover(Button btns[], const int num){
    for(int i = 0;i < num;i++)
        if(btns[i].isMouseOver())
            btns[i].highlight();
        else
            btns[i].dishighlight();
}
int ston(string s){
    int ans = 0;
    for(int i = 0;i < s.length();i++) ans = ans * 10 + s[i] - '0';
    return ans;
}
short chk_over(Button btns[], const int num){ for(short i = 1;i <= num;i++) if(btns[i-1].isMouseOver()) return i; return 0;}
string itos_plus(int a, int width, char sym = ' '){
    int dig ; string tmp = "", ans = "";
    for(dig = 0;a;a/=10, dig++)
        tmp += (char)(a%10 + '0');
    if(!dig){
        dig = 1;
        tmp += '0';
    }
    for(int i = 0;i < max(0, width - dig);i++)
        ans += sym;
    for(int i = tmp.length() - 1;i >= 0;i--)
        ans += tmp[i];
    return ans;
}
void record_reset() {for(int i = 0;i < 6;i++) RecData[i] = 0;}
void record_update(const int& clr, const int& score, const int& time);
void score_display(const string& over, const int& time, const int& line, const int& score){
	fx[1].play();
    record_update(line, score, time);
    const short t_num = 3;
    set_unit(1, 700);
    TextBox title(unit * 50, over.length()), rec[t_num];
    title.setText(over);
    title.setMidPosition(Vector2f(ResX / 2.f, 100 * unit));
    Button button = Button("Return", unit * 20);
    for(int i = 0;i < t_num;i++){
        rec[i].setFontSize(unit * 20);
        rec[i].setColor(Color(255, 255, 255), Color(100, 100, 100));
        rec[i].setLimit(19);
        switch (i){
            case 0:{
                rec[i].setText(OverDisplay[i] + itos_plus(time, 7));
                break;
            }
            case 1:{
                rec[i].setText(OverDisplay[i] + itos_plus(line, 7));
                break;
            }
            case 2:{
                rec[i].setText(OverDisplay[i] + itos_plus(score, 7));
                break;
            }
        }
        rec[i].setMidPosition(Vector2f(ResX / 2.f, unit * (47 * i + 250)));
    }
    button.setMidPosition(Vector2f(ResX / 2.f, unit * 400));
    if(button.isMouseOver())
        button.highlight();
    else
        button.dishighlight();
    while(window.isOpen()){
        while(window.pollEvent(event)){
            switch (event.type){
                case Event::TextEntered:{
                    if(event.text.unicode == VK_ESC)
                        return;
                }
                case Event::Closed:{
                    window.close();
                    return;
                }
                case Event::MouseMoved:{
                    if(button.isMouseOver())
                        button.highlight();
                    else
                        button.dishighlight();
                    break;
                }
                case Event::MouseButtonPressed:{
                    if(button.isMouseOver()) return;
                    break;
                }
            }
        }
        window.clear();
        title.Draw();
        for(int i = 0;i < t_num;i++) rec[i].Draw();
        button.Draw();
        window.display();
        sleep(milliseconds(flush_tick));
    }
}

void single(){ //to be finished
    clock_t t_start;
    int tLine, tScore, goal;
    short sel = 0;
    const short opt = 6;
    set_unit(1, 700);
    TextBox title(unit * 50, 13), sub_title[3], input[4];
    title.setText("Single Player");
    title.setMidPosition(Vector2f(ResX / 2.f, 100 * unit));
    for(int i = 0;i < 3;i++){
        sub_title[i].setFontSize(unit * 20);
        sub_title[i].setLimit(25);
        sub_title[i].setMidPosition(Vector2f(ResX / 2.f, unit * (80 * (i + bool(i)) + 220)));
    }
    sub_title[0].setText("Width (5-50)  Height (15-40)");
    sub_title[1].setText("Goal: (Seconds)");
    sub_title[2].setText("Goal: (Lines)");
    for(int i = 0;i < 4;i++){
        input[i].setFontSize(unit * 17);
        input[i].setLimit(8);
        input[i].setColor(Color(255, 255, 255), Color(100, 100, 100));
        input[i].setLeftPosition(Vector2f(ResX / 2.f - unit * (120 - (i==1?140:0)), unit * (80 * (i - (i==1)) + 250)));
    }
    input[0].setText(to_string(width)); input[0].setRange(5, 50);
    input[1].setText(to_string(height)); input[1].setRange(15, 40);
    input[2].setText("60"); input[2].setRange(1, 99999999);
    input[3].setText("40"); input[3].setRange(1, 99999999);
    Button buttons[opt] = {Button("Set", unit * 20), Button("Infinite Mode", unit * 20), Button("Time Mode", unit * 20), Button("Clear Line Mode", unit * 20), Button("Return", unit * 20), Button("Reset", unit * 20)};
    buttons[0].setLeftPosition(Vector2f(ResX / 2.f + unit * 220, unit * 250));
    buttons[1].setMidPosition(Vector2f(ResX / 2.f, unit * 330));
    for(int i = 2;i < 4;i++)
        buttons[i].setLeftPosition(Vector2f(ResX / 2.f + unit * 70, unit * (80 * i + 250)));
    buttons[4].setMidPosition(Vector2f(ResX / 2.f, unit * 570));
    buttons[5].setLeftPosition(Vector2f(ResX / 2.f + unit * 150, unit * 250));
    chk_hover(buttons, opt);
    while(window.isOpen()){
        while(window.pollEvent(event)){
            switch (event.type){
                case Event::Closed:{
                    window.close();
                    return;
                }
                case Event::TextEntered:{
                    if(event.text.unicode == VK_ENTER){
                        if(sel == 1 || sel == 2){
                            input[0].setFontColor(Color(255, 255, 255));
                            width = ston(input[0].getText());
                            input[1].setFontColor(Color(255, 255, 255));
                            height = ston(input[1].getText());
                            input[0].setText(to_string(width));
                            input[1].setText(to_string(height));
                        }
                    }
                    else if(event.text.unicode == VK_ESC)
                        return;
                    else if(sel)
                        input[sel - 1].typedOn();
                    break;
                }
                case Event::MouseMoved:{
                    chk_hover(buttons, opt);
                    break;
                }
                case Event::MouseButtonPressed:{
                    sel = chk_over(buttons, opt);
                    if(sel == 1){
                        width = ston(input[0].getText());
                        height = ston(input[1].getText());
                        input[0].setFontColor(Color(255, 255, 255));
                        input[1].setFontColor(Color(255, 255, 255));
                        input[0].setText(to_string(width));
                        input[1].setText(to_string(height));
                    }
                    else if(sel == 2){
                        t_start = clock();
                        try{singlePlayer(tLine, tScore);}
                        catch(exception &e){
                            score_display(e.what(), (clock() - t_start) / 1000, tLine, tScore);
                        }
                    }
                    else if(sel == 3 || sel == 4){ //to be finished(time)
                        t_start = clock();
                        input[sel-1].setFontColor(Color(255, 255, 255));
                        goal = ston(input[sel-1].getText());
                        try{singlePlayer(tLine, tScore, sel - 2, goal);}
                        catch(exception &e){
                            score_display(e.what(), (clock() - t_start) / 1000, tLine, tScore);
                        }
                    }
                    else if(sel == 5)
                        return;
                    else if(sel == 6){
                        width = 10, height = 20;
                        input[0].setFontColor(Color(255, 255, 255));
                        input[1].setFontColor(Color(255, 255, 255));
                        input[0].setText(to_string(width));
                        input[1].setText(to_string(height));
                    }
                    sel = 0;
                    for(int i = 1;i <= 4;i++){
                        if(input[i-1].isMouseOver()){
                            sel = i;
                            input[i-1].setSelected(1);
                        }
                        else{
                            input[i-1].setSelected(0);
                            input[i-1].verifyRange();
                            if(i == 3 || i == 4)
                                input[i - 1].setFontColor(Color(255, 255, 255));
                        }
                    }
                    chk_hover(buttons, opt);
                    break;
                }
            }
        }
        window.clear();
        title.Draw();
        for(int i = 0;i < 3;i++) sub_title[i].Draw();
        for(int i = 0;i < 4;i++) input[i].Draw();
        for(int i = 0;i < opt;i++) buttons[i].Draw();
        window.display();
        sleep(milliseconds(flush_tick));
    }
}

void thrd_conn(const bool& isHost, const string& ip){
    Thrd_lock.lock();
    Thrd_ret = 0;
    Thrd_lock.unlock();
    int tmp;
    if(isHost)
        tmp = server_connect();
    else
        tmp = client_connect(ip.c_str());
    Thrd_lock.lock();
    Thrd_ret = tmp;
    Thrd_lock.unlock();
}

void conn_dis(const bool& isHost, const string& s = ""){
    clock_t t_dot = clock();
    int trd_fetch, dot_cnt = 0;
    set_unit(1, 700);
    TextBox title(unit * 50, 12), sub_title;
    title.setText("Multi Player");
    title.setMidPosition(Vector2f(ResX / 2.f, 100 * unit));
    sub_title.setContext("Connecting");
    sub_title.setLeftPosition(Vector2f(ResX / 2.f - unit * 100, 200 * unit));
    Button button = Button("OK", unit * 20);
    button.setMidPosition(Vector2f(ResX / 2.f, unit * 280));
    thread thrd(thrd_conn, isHost, s);
    if(button.isMouseOver())
        button.highlight();
    else
        button.dishighlight();
    while(window.isOpen()){
        Thrd_lock.lock();
        trd_fetch = Thrd_ret;
        Thrd_lock.unlock();
        while(window.pollEvent(event)){
            switch (event.type){
                case Event::TextEntered:{
                    if(event.text.unicode == VK_ESC)
                        return;
                }
                case Event::Closed:{
                    window.close();
                    return;
                }
                case Event::MouseMoved:{
                    if(button.isMouseOver())
                        button.highlight();
                    else
                        button.dishighlight();
                    break;
                }
                case Event::MouseButtonPressed:{
                    if(button.isMouseOver() && trd_fetch){
                        thrd.join();
                        return;
                    }
                    break;
                }
            }
        }
        if(trd_fetch){
            conn = (trd_fetch > 0);
            host = (conn && isHost);
            sub_title.setContext((trd_fetch>0?"Connecting Suceed!":"Connecting failed!"));
        }
        else{
            if(clock() - t_dot > 800){
                t_dot = clock();
                dot_cnt = (dot_cnt + 1) % 4;
            }
            sub_title.setContext(string("Connecting") + string(dot_cnt, '.'));
        }
        window.clear();
        title.Draw();
        sub_title.Draw();
        if(trd_fetch)
            button.Draw();
        window.display();
        sleep(milliseconds(flush_tick));
    }
}

void multi(){ //to be finished
    int tLine, tScore;
    clock_t t_start;
    short sel = 0;
    const short opt = 3;
    set_unit(1, 700);
    TextBox title(unit * 50, 12), sub_title[2], input;
    title.setText("Multi Player");
    title.setMidPosition(Vector2f(ResX / 2.f, 100 * unit));
    sub_title[0].setContext("You are disconnected!");
    sub_title[1].setContext("You are Temp !");
    for(int i = 0;i < 2;i++){
        sub_title[i].setFontSize(unit * 20);
        sub_title[i].setMidPosition(Vector2f(ResX / 2.f, unit * 200));
    }
    input.setFontSize(unit * 17);
    input.setLimit(16);
    input.setColor(Color(255, 255, 255), Color(100, 100, 100));
    input.setLeftPosition(Vector2f(ResX / 2.f - unit * 180, unit * 400));
    Button buttons[opt] = {Button("Be a Host", unit * 20), Button("Connect to the Host", unit * 20), Button("Return", unit * 20)};
    buttons[0].setMidPosition((Vector2f(ResX / 2.f, unit * 330)));
    buttons[1].setLeftPosition(Vector2f(ResX / 2.f + unit * 20, unit * 400));
    buttons[2].setMidPosition((Vector2f(ResX / 2.f, unit * 470)));
    chk_hover(buttons, opt);
    Button conn_but[2] = {Button("Start", unit * 20), Button("Return", unit * 20)};
    conn_but[0].setMidPosition((Vector2f(ResX / 2.f, unit * 330)));
    conn_but[1].setMidPosition((Vector2f(ResX / 2.f, unit * 400)));
    while(window.isOpen()){
        window.clear();
        if(conn){
            while(window.pollEvent(event)){
                switch (event.type){
                    case Event::Closed:{
                        window.close();
                        return;
                    }
                    case Event::MouseMoved:{
                        chk_hover(conn_but, 2);
                        break;
                    }
                    case Event::MouseButtonPressed:{
                        sel = chk_over(conn_but, 2);
                        if(sel == 1){
                            t_start = clock();
                            try{multiPlayer(tLine, tScore);}
                            catch(exception &e){
                                score_display(e.what(), (clock() - t_start) / 1000, tLine, tScore);
                            }
                        }
                        if(sel == 2)
                            return;
                        chk_over(conn_but, 2);
                        break;
                    }
                }
            }
            title.Draw();
            sub_title[1].Draw();
            for(int i = 0;i < 2;i++) conn_but[i].Draw();
        }
        else{
            while(window.pollEvent(event)){
                switch (event.type){
                    case Event::Closed:{
                        window.close();
                        return;
                    }
                    case Event::TextEntered:{
                        if(event.text.unicode == VK_ENTER && sel == 2){
                            conn_dis(0, input.getText());
                            sub_title[1].setContext(string("You are ") + (host?"Host !":"Guest!"));
                            chk_hover(conn_but, 2);
                        }
                        else if(event.text.unicode == VK_ESC)
                            return;
                        else
                            input.typedOn();
                        break;
                    }
                    case Event::MouseMoved:{
                        chk_hover(buttons, opt);
                        break;
                    }
                    case Event::MouseButtonPressed:{
                        sel = chk_over(buttons, opt);
                        if(sel == 1 || sel == 2){
                            conn_dis(sel - 2, input.getText());
                            sub_title[1].setContext(string("You are ") + (host?"Host !":"Guest!"));
                            chk_hover(conn_but, 2);
                        }
                        if(sel == 3)
                            return;
                        sel = 0;
                        if(input.isMouseOver()){
                            sel = 2;
                            input.setSelected(1);
                        }
                        else
                            input.setSelected(0);
                        chk_hover(buttons, opt);
                        break;
                    }
                }
            }
            title.Draw();
            input.Draw();
            sub_title[host].Draw();
            for(int i = 0;i < opt;i++) buttons[i].Draw();
        }
        window.display();
        sleep(milliseconds(flush_tick));
    }
}

void record(){
    const short opt = 2, t_num = 6;
    set_unit(1, 700);
    TextBox title(unit * 50, 6), rec[6];
    title.setText("Record");
    title.setMidPosition(Vector2f(ResX / 2.f, 100 * unit));
    Button buttons[opt] = {Button("Reset Record", unit * 20), Button("Return", unit * 20)};
    for(int i = 0;i < t_num;i++){
        rec[i].setFontSize(unit * 20);
        rec[i].setColor(Color(255, 255, 255), Color(100, 100, 100));
        rec[i].setLimit(25);
        rec[i].setText(RecTitle[i] + itos_plus(RecData[i], 7));
        rec[i].setMidPosition(Vector2f(ResX / 2.f, unit * (47 * i + 220)));
    }
    for(int i = 0;i < opt;i++) buttons[i].setMidPosition(Vector2f(ResX / 2.f, unit * (40 * i + 500)));
    chk_hover(buttons, opt);
    while(window.isOpen()){
        window.clear();
        while(window.pollEvent(event)){
            switch (event.type){
                case Event::TextEntered:{
                    if(event.text.unicode == VK_ESC)
                        return;
                }
                case Event::Closed:{
                    window.close();
                    return;
                }
                case Event::MouseMoved:{
                    chk_hover(buttons, opt);
                    break;
                }
                case Event::MouseButtonPressed:{
                    switch (chk_over(buttons, opt)){
                        case 1: {record_reset(); for(int i = 0;i < t_num;i++) rec[i].setText(RecTitle[i] + itos_plus(RecData[i], 7)); break;}
                        case 2: {return;}
                    }
                    chk_hover(buttons, opt);
                    break;
                }
            }
        }
        title.Draw();
        for(int i = 0;i < t_num;i++) rec[i].Draw();
        for(int i = 0;i < opt;i++) buttons[i].Draw();
        window.display();
        sleep(milliseconds(flush_tick));
    }
}

void setting(){
    short sel = 0;
    const short opt = 4;
    set_unit(1, 700);
    TextBox title(unit * 50, 8), sub_title[3], input[3];
    title.setText("Setting");
    title.setMidPosition(Vector2f(ResX / 2.f, 100 * unit));
    for(int i = 0;i < 3;i++){
        sub_title[i].setFontSize(unit * 20);
        sub_title[i].setLimit(25);
        sub_title[i].setMidPosition(Vector2f(ResX / 2.f, unit * (100 * i + 220)));
        input[i].setFontSize(unit * 17);
        input[i].setLimit(8);
        input[i].setColor(Color(255, 255, 255), Color(100, 100, 100));
        input[i].setLeftPosition(Vector2f(ResX / 2.f - unit * 120, unit * (100 * i + 250)));
        input[i].setText(to_string(conf[i]));
    }
    sub_title[0].setText("DAS:   1(slow)-1000(fast)");
    sub_title[1].setText("ARR:   1(slow)- 500(fast)");
    sub_title[2].setText("Gravity: 1(slow)-50(fast)");
    input[0].setRange(1, 1000);
    input[1].setRange(1, 500);
    input[2].setRange(1, 50);
    Button buttons[opt] = {Button("Set", unit * 20), Button("Set", unit * 20), Button("Set", unit * 20), Button("Return", unit * 20)};
    for(int i = 0;i < opt - 1;i++) buttons[i].setLeftPosition(Vector2f(ResX / 2.f + unit * 100, unit * (100 * i + 250)));
    buttons[3].setMidPosition((Vector2f(ResX / 2.f, unit * 550)));
    chk_hover(buttons, opt);
    while(window.isOpen()){
        while(window.pollEvent(event)){
            switch (event.type){
                case Event::Closed:{
                    window.close();
                    return;
                }
                case Event::TextEntered:{
                    if(event.text.unicode == VK_ENTER){
                        if(sel){
                            input[sel-1].setFontColor(Color(255, 255, 255));
                            conf[sel-1] = ston(input[sel-1].getText());
                            input[sel-1].setText(to_string(conf[sel-1]));
                        }
                    }
                    else if(event.text.unicode == VK_ESC)
                        return;
                    else if(sel)
                        input[sel - 1].typedOn();
                    break;
                }
                case Event::MouseMoved:{
                    chk_hover(buttons, opt);
                    break;
                }
                case Event::MouseButtonPressed:{
                    sel = chk_over(buttons, opt);
                    if(sel == 4)
                        return;
                    if(sel){
                        input[sel-1].setFontColor(Color(255, 255, 255));
                        conf[sel-1] = ston(input[sel-1].getText());
                        input[sel-1].setText(to_string(conf[sel-1]));
                    }
                    sel = 0;
                    for(int i = 1;i <= 3;i++){
                        if(input[i-1].isMouseOver()){
                            sel = i;
                            input[i-1].setSelected(1);
                        }
                        else{
                            input[i-1].setSelected(0);
                            input[i-1].verifyRange();
                        }
                    }
                    chk_hover(buttons, opt);
                    break;
                }
            }
        }
        window.clear();
        title.Draw();
        for(int i = 0;i < 3;i++){
            sub_title[i].Draw();
            input[i].Draw();
        }
        for(int i = 0;i < opt;i++) buttons[i].Draw();
        window.display();
        sleep(milliseconds(flush_tick));
    }
}

void main_menu(){ //700 row
    const short opt = 5;
    set_unit(1, 700);
    TextBox title(unit * 50, 6);
    title.setText("Tetris");
    title.setMidPosition(Vector2f(ResX / 2.f, 100 * unit));
    Button buttons[opt] = {Button("Single Player", unit * 20), Button("Multi Player", unit * 20), Button("Record", unit * 20), Button("Setting", unit * 20), Button("Exit", unit * 20)};
    for(int i = 0;i < opt;i++) buttons[i].setMidPosition(Vector2f(ResX / 2.f, unit * 70 * (i + 4)));
    chk_hover(buttons, opt);
    while(window.isOpen()){
        while(window.pollEvent(event)){
            switch (event.type){
                case Event::TextEntered:{
                    if(event.text.unicode == VK_ESC)
                        return;
                }
                case Event::Closed:{
                    window.close();
                    return;
                }
                case Event::MouseMoved:{
                    chk_hover(buttons, opt);
                    break;
                }
                case Event::MouseButtonPressed:{
                    switch (chk_over(buttons, opt)){
                        case 1: {single(); break;}
                        case 2: {multi(); break;}
                        case 3: {record(); break;}
                        case 4: {setting(); break;}
                        case 5: {return;}
                    }
                    chk_hover(buttons, opt);
                    break;
                }
            }
        }
        window.clear();
        title.Draw();
        for(int i = 0;i < opt;i++) buttons[i].Draw();
        window.display();
        sleep(milliseconds(flush_tick));
    }
}

void pause_menu(){
    const short opt = 3;
    set_unit(1, 700);
    TextBox title(unit * 50, 6);
    title.setText("Pause");
    title.setMidPosition(Vector2f(ResX / 2.f, 100 * unit));
    Button buttons[opt] = {Button("Rseume", unit * 20), Button("Setting", unit * 20), Button("Quit", unit * 20)};
    for(int i = 0;i < opt;i++) buttons[i].setMidPosition(Vector2f(ResX / 2.f, unit * 70 * (i + 4)));
    chk_hover(buttons, opt);
    while(window.isOpen()){
        while(window.pollEvent(event)){
            switch (event.type){
                case Event::TextEntered:{
                    if(event.text.unicode == VK_ESC)
                        return;
                }
                case Event::Closed:{
                    window.close();
                    return;
                }
                case Event::MouseMoved:{
                    chk_hover(buttons, opt);
                    break;
                }
                case Event::MouseButtonPressed:{
                    switch (chk_over(buttons, opt)){
                        case 1: {return; break;}
                        case 2: {setting(); break;}
                        case 3: {throw runtime_error("Quit"); break;}
                    }
                    chk_hover(buttons, opt);
                    break;
                }
            }
        }
        window.clear();
        title.Draw();
        for(int i = 0;i < opt;i++) buttons[i].Draw();
        window.display();
        sleep(milliseconds(flush_tick));
    }
}

void record_update(const int& clr, const int& score,const int& time){
    RecData[0]++;
    RecData[1] += time;
    RecData[2] += clr;
    RecData[3] += score;
    RecData[4] = max(RecData[4], clr);
    RecData[5] = max(RecData[5], score);
}

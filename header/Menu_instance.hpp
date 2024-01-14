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
void set_unit(double cols, double lns){unit = min(ResX/(double)cols,ResY/(double)lns); return;}

void single(){ //to be finished
    const short opt = 5;
    set_unit(1, 700);
    TextBox title(unit * 50, 13);
    title.setText("Single Player");
    title.setMidPosition(Vector2f(ResX / 2.f, 100 * unit));
    Button buttons[opt] = {Button("Set", unit * 20), Button("Infinite Mode", unit * 20), Button("Time Mode", unit * 20), Button("Clear Line Mode", unit * 20), Button("Return", unit * 20)};
    buttons[0].setLeftPosition(Vector2f(ResX / 2.f + unit * 100, unit * 280));
    for(int i = 1;i < opt;i++) buttons[i].setMidPosition(Vector2f(ResX / 2.f, unit * 70 * (i + 4)));
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
                    break;
                }
                case Event::MouseMoved:{
                    chk_hover(buttons, opt);
                    break;
                }
                case Event::MouseButtonPressed:{
                    switch (chk_over(buttons, opt)){
                        case 1: {single(); break;}
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

void multi(){ //to be finished

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
        while(window.pollEvent(event)){
            switch (event.type){
                case Event::TextEntered:{
                    if(event.text.unicode == VK_ESC)
                        return;
                }
                case Event::Closed:{
                    window.close();
                    break;
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
        window.clear();
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
                    break;
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

void record_update(const int& clr, const int& score,const int& time){
    RecData[0]++;
    RecData[1] += time;
    RecData[2] += clr;
    RecData[3] += score;
    RecData[4] = max(RecData[4], clr);
    RecData[5] = max(RecData[5], score);
}

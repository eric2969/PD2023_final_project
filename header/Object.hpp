#pragma once

#define VK_DEL 8
#define VK_ENTER 13
#define VK_ESC 27

class TextBox{
public:
	TextBox():isSel(0), Limit(0), Range(0){
		text.str(""); text.clear();
		textbox.setFont(font);
		textbox.setColor(Color(255, 255, 255));
		bg.setFillColor(Color(0, 0, 0));
		bg.setOutlineColor(Color(0, 0, 0));
	}
	TextBox(const int& size, const int& lim, const bool& sel = 0, const Color& tc = Color(255, 255, 255), const Color& bgc = Color(0, 0, 0)):isSel(sel), Limit(lim), Range(0){
		text.str(""); text.clear();
		textbox.setFont(font);
		textbox.setCharacterSize(size);
		textbox.setColor(tc);
		textbox.setString(string(max(1, Limit), 'A'));
		bg.setSize(Vector2f(textbox.getGlobalBounds().width, textbox.getGlobalBounds().height + (size >> 1)));
		bg.setFillColor(bgc);
		bg.setOutlineThickness(size / 2.f);
		bg.setOutlineColor(bgc);
		textbox.setString(sel?"_":"");
	}
	void setRange(const int& bottom, const int& top){Range = 1; Minimum = bottom; Maximum = top;}
	void typedOn(){
		if(isSel){
			int charTyped = event.text.unicode;
			if(charTyped < 128 && text.str().length() <= Limit)
				inputLog(charTyped);
		}
	}
	void setFontSize(const int& size){
		textbox.setCharacterSize(size);
		textbox.setString(string(max(1, Limit), '('));
		bg.setSize(Vector2f(textbox.getGlobalBounds().width, textbox.getGlobalBounds().height + (size >> 1)));
		bg.setOutlineThickness(size / 2.f);
		textbox.setString(text.str() + (isSel?"_":""));
	}
	void setFontColor(const Color& tc) {textbox.setColor(tc);}
	void setColor(const Color& tc, const Color& bgc){
		textbox.setColor(tc);
		bg.setFillColor(bgc);
		bg.setOutlineColor(bgc);
	}
	void setText(const string& s){text.str(""); text << s; textbox.setString(text.str() + (isSel?"_":""));}
	void setLimit(const int& lim){Limit = lim; textbox.setString(string(max(1,lim), 'A')); bg.setSize(Vector2f(textbox.getGlobalBounds().width, bg.getSize().y)); textbox.setString(text.str() + (isSel?"_":""));}
	void setLeftPosition(const Vector2f& pos) {textbox.setPosition(pos); bg.setPosition(pos);}
	void setMidPosition(const Vector2f& pos){
		textbox.setString(string(max(1, Limit), '('));
		textbox.setPosition(Vector2f(pos.x - textbox.getGlobalBounds().width / 2.f, pos.y - textbox.getGlobalBounds().height / 2.f));
		bg.setPosition(Vector2f(pos.x - textbox.getGlobalBounds().width / 2.f, pos.y - textbox.getGlobalBounds().height / 2.f));
		textbox.setString(text.str() + (isSel?"_":""));
	}
	void setSelected(const bool& sel){isSel = sel;textbox.setString(text.str() + (sel?"_":""));}
	void Draw() {window.draw(bg); window.draw(textbox);}
	bool isSelected() {return isSel;}
	bool isMouseOver(){
		float MX = Mouse::getPosition(window).x, MY = Mouse::getPosition(window).y;
		float btnX0 = bg.getPosition().x, btnY0 = bg.getPosition().y;
		float btnX1 = btnX0 + bg.getGlobalBounds().width, btnY1 = btnY0 + bg.getGlobalBounds().height;
		return (btnX0 <= MX && btnX1 >= MX && btnY0 <= MY && btnY1 >= MY); 
	}
	string getText() {return text.str();}
private:
	Text textbox;
	RectangleShape bg;
	stringstream text, sstmp;
	bool isSel, Range;
	int Limit, Maximum, Minimum, iTmp;
	void inputLog(int charTyped){
		iTmp = 0;
		if(charTyped != VK_DEL && charTyped != VK_ENTER && charTyped != VK_ESC && text.str().length() < Limit){
			if(Range && isdigit(charTyped)){
				text << static_cast<char>(charTyped);
				sstmp.str(""); sstmp.clear(); sstmp << text.str();
				sstmp >> iTmp;
				iTmp = min(Maximum, iTmp);
				iTmp = max(Minimum, iTmp);
				text.str(""); text << iTmp;
				textbox.setColor(Color(170, 170, 170));
			}
			else if(!Range){
				text << static_cast<char>(charTyped);
				textbox.setColor(Color(170, 170, 170));
			}
		}
		else if(charTyped == VK_DEL && text.str().length() > 0){
			delChar();
			textbox.setColor(Color(170, 170, 170));
		}
		textbox.setString(text.str() + "_");
	}
	void delChar(){
		string t = text.str(), tmp = ""; text.str("");
		for(int i = 0;i < t.length() - 1;i++)
			tmp += t[i];
		if(Range){
			sstmp.str(""); sstmp.clear(); sstmp << tmp;
			sstmp >> iTmp;
			iTmp = min(Maximum, iTmp);
			iTmp = max(Minimum, iTmp);
			text << iTmp;
		}
		else
			text << tmp;
	}
};

class Button{
public:
	Button(const string& textstr, const int& size, const Color& TC = Color(255, 255, 255), const Color& TCH = Color(255, 255, 255), const Color& BGC = Color(50, 50, 50), const Color& BGCH = Color(200, 200, 200)): bgc(BGC), bgch(BGCH), tc(TC), tch(TCH){
		text.setFont(font);
		text.setString(textstr);
		text.setColor(TC);
		text.setCharacterSize(size);
		button.setSize(Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height + size /4.f));
		button.setFillColor(BGC);
		button.setOutlineThickness(size / 4.f);
		button.setOutlineColor(BGC);
	}
	void setLeftPosition(const Vector2f& pos) {text.setPosition(pos); button.setPosition(Vector2f(pos.x, pos.y + text.getGlobalBounds().height / 4.f));}
	void setMidPosition(const Vector2f& pos){
		text.setPosition(Vector2f(pos.x - text.getGlobalBounds().width / 2.f, pos.y - text.getGlobalBounds().height / 2.f));
		button.setPosition(Vector2f(pos.x - text.getGlobalBounds().width / 2.f, pos.y - text.getGlobalBounds().height / 4.f));
	}
	void highlight() {text.setColor(tch); button.setFillColor(bgch); button.setOutlineColor(bgch);}
	void dishighlight() {text.setColor(tc); button.setFillColor(bgc); button.setOutlineColor(bgc);}
	void Draw() {window.draw(button); window.draw(text);}
	bool isMouseOver(){
		float MX = Mouse::getPosition(window).x, MY = Mouse::getPosition(window).y;
		float btnX0 = button.getGlobalBounds().left, btnY0 = button.getGlobalBounds().top;
		float btnX1 = btnX0 + button.getGlobalBounds().width, btnY1 = btnY0 + button.getGlobalBounds().height;
		return (btnX0 <= MX && btnX1 >= MX && btnY0 <= MY && btnY1 >= MY); 
	}
private:
	Text text;
	RectangleShape button;
	Color bgc, bgch, tc, tch;
};

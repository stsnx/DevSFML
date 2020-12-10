#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<sstream>
using namespace sf;
using namespace std;
#define DELETE 8
#define ENTER 13
#define ESCAPE 27
class textbox{
	public:
		textbox(int size, bool select) {
			Textbox.setCharacterSize(size);
			Textbox.setFillColor(Color::White);
			isSelected = select;
			if (select) {
				Textbox.setString("_");
			}
			else {
				Textbox.setString("");
			}
		}
		void setFont(Font& font) {
			Textbox.setFont(font);
		}
		void setPosition(Vector2f pos) {
			Textbox.setPosition(pos);
		}
		void setLimit(bool ToF,int lim) {
			haslimit = ToF;
			limit = lim;
		}
		void setSelected(bool sel) {
			isSelected = sel;
			if (!sel) {
				string t = text.str();
				string newText = "";
				for (int i = 0; i < t.length() - 1; i++) {
					newText += t[i];
				}
				Textbox.setString(newText);
			}
		}
		string getText() {
			return text.str();
		}
		void draw(RenderWindow & window) {
			window.draw(Textbox);
		}
		void typeOn(Event input) {
			if (isSelected) {
				int charTyped = input.text.unicode;
				std::cout << charTyped << std::endl;
				if (charTyped < 128) {
					if (haslimit) {
						if (text.str().length() <= limit) {
							inputLogic(charTyped);
						}
						else if (text.str().length() > limit && charTyped == DELETE) {
							delLastChar();
						}
					}
					else {
						inputLogic(charTyped);
					}
				}
				
			}
		}
		void cleartext() {
			Textbox.setString(' ');
			text.str("");
		}	
		sf::Text Textbox;
		ostringstream text;
	private:
	
		bool isSelected = false;
		bool haslimit = false;
		int limit=90;
		void inputLogic(int typed) {
			if (typed != DELETE && typed != ENTER && typed != ESCAPE) {
				text << static_cast<char>(typed);
			}
			else if (typed == DELETE) {
				if (text.str().length() > 0) {
					delLastChar();
				}
			}
			Textbox.setString(text.str() + "_");
		
		}
		void delLastChar() {
			string t = text.str();
			string newText = "";
			for (int i = 0; i < t.length() - 1; i++) {
				newText += t[i];
			}
			text.str("");
			text << newText;
			Textbox.setString(text.str());
		}
};


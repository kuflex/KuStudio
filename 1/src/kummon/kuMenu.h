#pragma once
//Меню

#include "ofMain.h"

class kuMenu
{
public:
	void setup( string fontFile, int fontSize, float X = 0, float Y = 0,
               float indentX = 10, float indentY = 5,
               float marginX = 10, float marginY = 5 );
	void update( float dt );
	void draw();
	string commandName(); //выдает команду которую нужно выполнить

	void addSubmenu( string name, string title );
	void addItem( string name, string title, string submenuName );

    //обработка событий мыши
    //type 0 - move, 1 - press, 2 - drag, 3 - release
    //mouseState - указатель, использует ли кто-то мышь
    void mouse( int x, int y, int button, void *&mouseState, int type );

private:
	vector< vector<string> > titles;
	vector< vector<string> > names;
	vector<string> subTitles;
	vector<string> subNames;
	int size;
	vector<int> sizes;

	int findSub( string name );

	ofTrueTypeFont font;

	void updateDrawProp();
	int lineH;
	vector<ofRectangle> subRects;
	vector< vector<ofRectangle> > rects;
    int findSubRect( int x, int y );
    int findRect( int x, int y, int sub );

	float X, Y;
    float indentX, indentY;
    float marginX, marginY;
    
    int state;  //0 - пусто, 1 - показ субменю
    int subItem, item;  //выбранное подменю
    int overSub, overItem;
    string commandName_;

    void cancel();
};


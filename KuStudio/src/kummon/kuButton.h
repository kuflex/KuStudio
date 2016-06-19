#pragma once
//Кнопка

#include "ofMain.h"

class kuButton
{
public:
	void setup( string iconFile, float X, float Y );
	void update( float dt );
	void draw();
    
    void setToggle( bool toggle );
    void setEnabled( bool enabled );
    void setDown( bool down );
    void setVisible( bool visible );
    void setPressed();  //эмуляция нажатой кнопки - только если она включена
    
    bool isToggle() { return toggle_; }
    bool isEnabled() { return enabled_; }
    bool isVisible() { return visible_; }
    
    bool isPressed() { return pressed_; } //очищается при update()
    bool isDown() { return down_; }

    //обработка событий мыши
    //type 0 - move, 1 - press, 2 - drag, 3 - release
    //mouseState - указатель, использует ли кто-то мышь
    void mouse( int x, int y, int button, void *&mouseState, int type );

private:
	float X_, Y_;
    ofTexture icon_;
    float W_, H_;
    bool toggle_;
    bool enabled_;
    bool visible_;
    bool down_;
    
    int state_;  //0 - пусто, 1 - нажата мышь
    int pressed_;
    
};

//набор кнопок
class kuButtons {
public:
    void setup( int maxButtons );
    kuButton *addButton( string iconFile, float X, float Y );
    void update( float dt );
    void draw();

    //обработка событий мыши
    //type 0 - move, 1 - press, 2 - drag, 3 - release
    //mouseState - указатель, использует ли кто-то мышь
    void mouse( int x, int y, int button, void *&mouseState, int type );
    
private:
    vector<kuButton> button_;
    int n_;
    
};
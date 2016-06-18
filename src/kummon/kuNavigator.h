#pragma once
//Навигатор по области обзора, одномерный, в секундах

#include "ofMain.h"

class kuNavigator
{
public:
	void setup( int x, int y, int w, int h );
    void resized( int w, int h );
	void update( float dt );
	void draw();
    
    float getX() { return x_; }
    float getY() { return y_; }
    float getW() { return w_; }
    float getH() { return h_; }
    
    void setDuration( float duration );
    void setSize( int w, int h );
    void setSegment( float time0, float time1 );
    
    bool isSegmentChanged();    //сбрасывается при каждом вызове update()
    bool isSegmentLeftChanged(); //сбрасывается также; указывает, нужно ли перепослать все значения
    float time0() { return time0_; }
    float time1() { return time1_; }
    
    float timeToPix( float time );
    float time0Pix();
    float time1Pix();
    float pixToTime( float pix );

    //обработка событий мыши
    //type 0 - move, 1 - press, 2 - drag, 3 - release
    //mouseState - указатель, использует ли кто-то мышь
    void mouse( int x, int y, int button, void *&mouseState, int type );

private:
    int x_, y_, w_, h_;
    int state;          //0 - ничего, 1 - обрабатываем мышь
	bool dragLeft, dragRight;
    float dragLeftDelta, dragRightDelta;
    
    
    float duration_;
    float time0_, time1_;
    
    
    bool isSegmentChanged_, isSegmentLeftChanged_;
};


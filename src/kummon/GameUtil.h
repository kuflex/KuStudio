#pragma once

//Вспомогательные компоненты для игры

#include "ofMain.h"

void ofLine3( const ofPoint &p0, const ofPoint &p1 );

void ofLine3Eps( const ofPoint &p0, const ofPoint &p1, const ofPoint eps );

void ofLines( const vector<float> &pointsXY, bool closed = false );	//рисует кривую, заданную линиями

void ofRect3( const ofPoint &p0, const ofPoint &p1, const ofPoint &p2, const ofPoint &p3,
			 bool filled = true );

void ofRect3Textured( const ofPoint &p0, const ofPoint &p1, const ofPoint &p2, const ofPoint &p3,
			 ofImage &texture );

void ofSetColor0( const ofColor &color, float alpha = 1.0f );

float ofDist( const ofPoint &p0, const ofPoint &p1 );

ofPoint ofLerp(ofPoint start, ofPoint stop, float amt);

ofPoint ofPointUnit( float angleDeg );	//единичный вектор

void ofTranslate( const ofPoint &p );

void drawOfImageCentered( ofImage &image, ofPoint center, float scale = 1, float w = -1, float h = -1 );

//Переворот картинки. Например, перед вызовом у imageOut можно поставить bUseTexture=false
void ofFlipImage( ofImage &image, ofImage &imageOut, bool horiz, bool vert ); 

ofColor makeOfColor( int r, int g, int b, int a = 255 );

//ofColor ofImagePixel( ofImage &image, int x, int y );
//ofColor ofImagePixelUnit( ofImage &image, float x, float y );

//Рисование без изменения пропорций
void pbPushMatrixFit( float W, float H, float w, float h, bool drawBorder );

//Поставить текстуре чтоб увеличивала в режиме NEAREST

//Рисование оптического потока, цвет не ставит
void pbDrawFlow( const vector<float> &flow, int w, int h, float eps = 0.01 );

#pragma once

#include "ofMain.h"

//Быстрое рисование и вспомогательные функции

//--------------------------------------------------------------
//огрубление с шагом size
float coarse( float x, float size );

//1, -1 равновероятно
int randomSign();

float sqr( float x );
//--------------------------------------------------------------


//цветные прямоугольники
void pbPushRect( float x, float y, float w, float h, const ofColor &color,
                vector<ofPoint> &points, vector<ofColor> &colors );
void pbPushRect( float x, float y, float z, float w, float h, const ofColor &color,
                vector<ofPoint> &points, vector<ofColor> &colors );

void pbPushRect( const ofPoint &p, const ofPoint &w, const ofPoint &h, const ofColor &color,
                vector<ofPoint> &points, vector<ofColor> &colors );
void pbDrawRects( vector<ofPoint> &points, vector<ofColor> &colors );

//текстурированные прямоугольники
void pbPushRect( const ofPoint &p, const ofPoint &w, const ofPoint &h,
                const ofRectangle &texRect,
                vector<ofPoint> &points, vector<ofVec2f> &texs );
void pbPushRect( float x, float y, float z, float w, float h,
                const ofRectangle &texRect,
                vector<ofPoint> &points, vector<ofVec2f> &texs );

void pbPushRect( const ofPoint &p1, const ofPoint &p2, const ofPoint &p3, const ofPoint &p4,
                const ofRectangle &texRect,
                vector<ofPoint> &points, vector<ofVec2f> &texs );

void pbDrawRects( vector<ofPoint> &points, vector<ofVec2f> &texs, ofTexture &texture );

//текстурированные прямоугольники с цветом
void pbPushRect( float x, float y, float z, float w, float h,
                const ofColor &color,
                const ofRectangle &texRect,
                vector<ofPoint> &points, vector<ofColor> &colors, vector<ofVec2f> &texs );

void pbPushRect( const ofPoint &p, const ofPoint &w, const ofPoint &h,
                const ofColor &color,
                const ofRectangle &texRect,
                vector<ofPoint> &points, vector<ofColor> &colors, vector<ofVec2f> &texs );

void pbDrawRects( vector<ofPoint> &points, vector<ofColor> &colors,
                 vector<ofVec2f> &texs, ofTexture &texture );

//цветные линии

void pbStartSmoothing();    //для сглаживания
void pbEndSmoothing();

void pbPushLine( const ofPoint &p1, const ofPoint &p2, const ofColor &color,
                vector<ofPoint> &points, vector<ofColor> &colors );
void pbPushLine( const ofPoint &p1, const ofPoint &p2, const ofColor &color1,
                const ofColor &color2,
                vector<ofPoint> &points, vector<ofColor> &colors );
void pbDrawLines( vector<ofPoint> &points, vector<ofColor> &colors );

//предполагается, что текстура уже подключена
void pbDrawLines_textured( vector<ofPoint> &points,
                 vector<ofVec2f> &texs );

//Вершины, рисуются текстурой
//Внимание: сами используют шейдеры, поэтому не должны вызываться при включенном шейдере
class pbVertexRender {
public:
    void setup( string pointImageFile, string shaderVert = "", string shaderFrag = "" );
    //если оставить пути к шейдеру пустыми - то нужно самим подключать шейдер,
    //после drawBegin и выключать после drawEnd
    
    void clearPoints();
    void addPoint( const ofPoint &p, float rad, float alpha );
    void setPoints( vector<ofPoint> &v, float minRad, float maxRad );
    
    //data - данные, которые передаются в шейдер как нормаль
    //например, data.x - size, data.y,z - текстурные координаты
    void setPoints( vector<ofPoint> &v, vector<ofPoint> &data );
    
    void draw();
    
    void drawBegin( float part = 1 );   //начало рисования, можно установить параметры в шейдер
    //part - доля сколько частиц рисовать, 0..1
    ofShader &shader() { return shader_; }
    void drawEnd( float part = 1 ); //конец рисования, part должен совпадать с началом
    
private:
    vector <ofVec3f> points;
    vector <ofVec3f> sizes;
    
    ofTexture texture;
    bool useShader_;
    ofShader shader_;
    ofVbo vbo;
    
};




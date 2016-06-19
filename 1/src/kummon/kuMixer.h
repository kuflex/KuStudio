#pragma once
//Микшер со слайдерами

#include "ofMain.h"
#include "ParamMap.h"

//Один слайдер
class kuMixerSlider
{
public:
	void setup( string name, float *value, float minV, float maxV,
               ofColor backColor,
               bool isInt );
    void setRect( float X, float Y, float W, float H );
	void update( float dt );
	void draw( ofTrueTypeFont &font, ofTrueTypeFont &fontSmall );
    
    float value() { return *value_; }
    void setValue( float v );
    const string &name() const { return name_; }
    
    //обработка событий мыши
    //type 0 - move, 1 - press, 2 - drag, 3 - release
    //mouseState - указатель, использует ли кто-то мышь
    void mouse( int x, int y, int button, void *&mouseState, int type );

private:
	float X_, Y_;
    float W_, H_;

    string name_;
    float *value_;
    float min_, max_;
    bool isInt_;
    
    ofColor backColor_;
    
    //int state_;
    //float y0_;
    void setValueFromYPixel( float y );
    
    float valueToPix();
    string stringValueTitle();

};


//Микшер со слайдерами
class kuMixer {
public:
    void setup( string id, string fontFile, int fontSize, int fontSizeSmall );
    void resize( float X, float Y, float W, float H );
    void load( ParamMap &ini );
    void save( ParamMap &ini );
    
    void addSlider( string name, float *value, float min, float max,
                   ofColor backColor = ofColor(0),
                   bool isInt = false );
    void addSliderInt( string name, float *value, float min, float max,
                   ofColor backColor = ofColor(0) );
    void addSlider( string name, float &value, float min, float max,
                   ofColor backColor = ofColor(0),
                   bool isInt = false );
    void addSliderInt( string name, float &value, float min, float max,
                      ofColor backColor = ofColor(0) );
    void update( float dt );
    void draw();

    void setValue( const string &name, float v );
    
    //обработка событий мыши
    //type 0 - move, 1 - press, 2 - drag, 3 - release
    //mouseState - указатель, использует ли кто-то мышь
    void mouse( int x, int y, int button, void *&mouseState, int type );
    
private:
    vector<kuMixerSlider> slider_;
    float X_, Y_, W_, H_;
    string id_;
    
    ofTrueTypeFont font_, fontSmall_;
    
    bool needUpdateRects_;
    void updateRects();
    
    int findSlider( const string &name ); //TODO по скорости оптимизировать ParamMap

};
#include "kuMixer.h"

//---------------------------------------------------------------------
void kuMixerSlider::setup( string name, float *value, float minV, float maxV,
                          ofColor backColor,
                          bool isInt ) {
    name_ = name;
    value_ = value;
    min_ = minV;
    max_ = maxV;
    isInt_ = isInt;
    
    backColor_ = backColor;
    
    X_ = Y_ = 0;
    W_ = H_ = 100;
    
    //state_ = 0;
    //y0_ = 0;
}

//---------------------------------------------------------------------
void kuMixerSlider::setRect( float X, float Y, float W, float H ) {
    X_ = X;
    Y_ = Y;
    W_ = W;
    H_ = H;
    
}

//---------------------------------------------------------------------
void kuMixerSlider::update( float dt ) {
    
}

//---------------------------------------------------------------------
float kuMixerSlider::valueToPix() {
    return ofMap( value(), min_, max_, Y_+H_-1, Y_+1 );
}

//---------------------------------------------------------------------
string kuMixerSlider::stringValueTitle() {
    float v = value();
    if ( isInt_ ) return ofToString( int(v) );
    else {
        int i = int(v);
        int a = abs(i);
        //if ( a > 999 || i < -99 ) return ofToString( v, 0 );
        if ( a > 99 || i < -9 ) return ofToString( v, 0 );
        if ( a > 9 || i < 0 ) return ofToString( v, 1 );
        return ofToString( v, 2 );
    }
}

//---------------------------------------------------------------------
void kuMixerSlider::draw( ofTrueTypeFont &font, ofTrueTypeFont &fontSmall ) {
    ofSetColor( backColor_ );
    ofFill();
    ofRect( X_, Y_, W_, H_ );
    ofSetColor( 255 );
    ofNoFill();
    ofRect( X_, Y_, W_, H_ );

    //значение
    float y = valueToPix();
    ofSetLineWidth( 3 );
    ofLine( X_+1, y, X_+W_-1, y );
    ofSetLineWidth( 1 );
    
    //текст - значение
    fontSmall.drawString( stringValueTitle(), X_ + 5, Y_ - 0.5 * fontSmall.getLineHeight() + 2 );

    //текст - имя
    ofPushMatrix();
    ofTranslate( X_ + W_/2, Y_ + H_/2 );
    ofRotate( -90 );
    font.drawString( name_, int(-font.stringWidth( name_)) / 2, font.getLineHeight()/2 );
    ofPopMatrix();  //TODO рисовать в FBO микшера для ускорения!!
    
}

//---------------------------------------------------------------------
void kuMixerSlider::setValue( float v ) {
    if ( isInt_ ) { v = int(v + 0.5); }
    *value_ = ofClamp( v, min_, max_ );
}

//---------------------------------------------------------------------
void kuMixerSlider::setValueFromYPixel( float y ) {
    setValue( ofMap( y, Y_+H_-1, Y_+1, min_, max_, true ) );
}

//---------------------------------------------------------------------
//обработка событий мыши
//type 0 - move, 1 - press, 2 - drag, 3 - release
//mouseState - указатель, использует ли кто-то мышь
void kuMixerSlider::mouse( int x, int y, int button, void *&mouseState, int type ) {
    
    bool mouseCaptured = ( mouseState != 0 );
    bool captured = mouseCaptured && (mouseState == this);
    if ( mouseCaptured && !captured ) { return; }
    
    if ( type == 0 ) {  //move
        return;
    }
    if ( type == 1 ) { //press
        if ( ofInRange( x, X_, X_ + W_ ) && ofInRange( y, Y_, Y_ + H_ ) ) {
            //state_ = 1;
            mouseState = this;
            setValueFromYPixel( y );
        }
        return;
    }
    if ( !captured ) { return; }
    if ( type == 2 ) { //drag
        setValueFromYPixel( y );
        return;
    }
    if ( type == 3 ) { //release
        setValueFromYPixel( y );
        //state_ = 0;
        mouseState = 0;
        return;
    }
    
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void kuMixer::setup( string id, string fontFile, int fontSize, int fontSizeSmall ) {
    id_ = id;
    font_.loadFont( fontFile, fontSize );
    fontSmall_.loadFont( fontFile, fontSizeSmall );
}

//---------------------------------------------------------------------
void kuMixer::resize( float X, float Y, float W, float H ) {
    X_ = X;
    Y_ = Y;
    W_ = W;
    H_ = H;
    needUpdateRects_ = true;
}

//---------------------------------------------------------------------
void kuMixer::load( ParamMap &ini ) {
    for (int i=0; i<slider_.size(); i++) {
        string s = id_ + "." + slider_[i].name();
        float v = ini.getFloat( s, slider_[i].value() );
        slider_[i].setValue( v );
    }
}

//---------------------------------------------------------------------
void kuMixer::save( ParamMap &ini ) {
    for (int i=0; i<slider_.size(); i++) {
        string s = id_ + "." + slider_[i].name();
        ini.setFloat( s, slider_[i].value() );
    }
}

//---------------------------------------------------------------------
void kuMixer::addSlider( string name, float *value, float min, float max,
                        ofColor backColor,
                        bool isInt ) {
    if ( value == 0 ) {
        cout << "Can't add slider " << name << " - value is a null pointer!" << endl;
        return;
    }
    kuMixerSlider slider;
    slider.setup( name, value, min, max, backColor, isInt );
    slider_.push_back( slider );
    needUpdateRects_ = true;
}

//---------------------------------------------------------------------
void kuMixer::addSliderInt( string name, float *value, float min, float max,
                           ofColor backColor ) {
    addSlider( name, value, min, max, backColor, true );
}

//---------------------------------------------------------------------
void kuMixer::addSlider( string name, float &value, float min, float max,
                        ofColor backColor,
                        bool isInt ) {
    addSlider( name, &value, min, max, backColor, isInt );
}
//---------------------------------------------------------------------
void kuMixer::addSliderInt( string name, float &value, float min, float max,
                           ofColor backColor ) {
    addSliderInt( name, &value, min, max, backColor );
}
//---------------------------------------------------------------------
void kuMixer::update( float dt ) {
    for (int i=0; i<slider_.size(); i++) {
        slider_[i].update( dt );
    }
}

//---------------------------------------------------------------------
void kuMixer::updateRects() {
    int n = slider_.size();
    if ( n == 0 ) return;
    float maxW = 60;
    if ( W_ / n <= maxW ) {
        //равномерно делим
        for (int i=0; i<n; i++) {
            float x0 = ofMap( i, 0, n, X_, X_+W_ );
            float x1 = ofMap( i+1, 0, n, X_, X_+W_ );
            slider_[i].setRect( x0, Y_, x1-x0, H_ );
        }
    }
    else {
        //делаем шириной maxW
        float x = 0;
        for (int i=0; i<n; i++) {
            slider_[i].setRect( x, Y_, maxW, H_ );
            x += maxW;
        }
    }
}

//---------------------------------------------------------------------
void kuMixer::draw() {
    if ( needUpdateRects_ ) {
        updateRects();
    }
    for (int i=0; i<slider_.size(); i++) {
        slider_[i].draw( font_, fontSmall_ );
    }
    ofSetColor( 255 );
    ofNoFill();
    ofRect( X_, Y_, W_, H_ );
}

//---------------------------------------------------------------------
//обработка событий мыши
//type 0 - move, 1 - press, 2 - drag, 3 - release
//mouseState - указатель, использует ли кто-то мышь
void kuMixer::mouse( int x, int y, int button, void *&mouseState, int type ) {
    for (int i=0; i<slider_.size(); i++) {
        slider_[i].mouse( x, y, button, mouseState, type );
    }
}

//---------------------------------------------------------------------
int kuMixer::findSlider( const string &name ) { //TODO по скорости оптимизировать ParamMap
    for (int i=0; i<slider_.size(); i++) {
        if ( slider_[i].name() == name ) return i;
    }
    return -1;
}

//---------------------------------------------------------------------
void kuMixer::setValue( const string &name, float v ) {
    int i = findSlider( name );
    if ( i >= 0) slider_[i].setValue( v );
}

//---------------------------------------------------------------------


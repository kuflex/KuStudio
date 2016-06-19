#include "kuNavigator.h"


//---------------------------------------------------------------------
void kuNavigator::setup( int x, int y, int w, int h ) {
    x_ = x;
    y_ = y;
    w_ = w;
    h_ = h;
    state = 0;
    
    time0_ = 0;
    time1_ = 1;
    duration_ = 1;
    
    isSegmentChanged_ = false;
    isSegmentLeftChanged_ = false;
}

//---------------------------------------------------------------------
void kuNavigator::resized( int w, int h ) {
    isSegmentChanged_ = true;
    w_ = w;
    h_ = h;
}


//---------------------------------------------------------------------
void kuNavigator::setDuration( float duration ) {
    if ( duration != duration_ ) {
        duration_ = duration;
        //setSegment( 0, duration );
        setSegment( duration * 0.25, duration * 0.5 );
    }
}

//---------------------------------------------------------------------
void kuNavigator::setSegment( float time0, float time1 ) {
    bool changed0 = (fabs(time0-time0_) > 0.0001); //изменился левый край
    time0_ = ofClamp( time0, 0, duration_ );
    time1_ = ofClamp( time1, 0, duration_ );
    isSegmentChanged_ = true;
    isSegmentLeftChanged_ = changed0;
}

//---------------------------------------------------------------------
bool kuNavigator::isSegmentChanged() {
    return isSegmentChanged_;
}

//---------------------------------------------------------------------
bool kuNavigator::isSegmentLeftChanged() {
    return isSegmentLeftChanged_;
}

//---------------------------------------------------------------------
void kuNavigator::update( float dt ) {
    isSegmentChanged_ = false;
    isSegmentLeftChanged_ = false;
}

//---------------------------------------------------------------------
float kuNavigator::timeToPix( float time ) {
    if ( duration_ > 0 ) return ofMap( time, 0, duration_, x_, x_+w_ );
    else return 0;
}

//---------------------------------------------------------------------
float kuNavigator::pixToTime( float pix ) {
    return ofMap( pix, x_, x_ + w_, 0, duration_ );
}

//---------------------------------------------------------------------
float kuNavigator::time0Pix() {
    return timeToPix( time0_ );
}

//---------------------------------------------------------------------
float kuNavigator::time1Pix() {
    return timeToPix( time1_ );
}

//---------------------------------------------------------------------
void kuNavigator::draw() {
    ofPushMatrix();
    ofTranslate( x_, y_ );
    ofNoFill();
    ofSetColor( 128 );
    
    ofRect( 0, 0, w_, h_ );
    
    float pix0 = time0Pix() - x_;
    float pix1 = time1Pix() - x_;

    ofFill();
    ofSetColor( 100 );
    ofEnableBlendMode( OF_BLENDMODE_ADD );
    ofRect( pix0, 1, pix1-pix0, h_-1 );
    ofEnableAlphaBlending();

    ofSetColor( 255 );
    ofLine( pix0, 0, pix0, h_ + 8 );
    ofLine( pix1, 0, pix1, h_ + 8 );

    ofPopMatrix();
}

//---------------------------------------------------------------------
//обработка событий мыши
//type 0 - move, 1 - press, 2 - drag, 3 - release
//mouseState - указатель, использует ли кто-то мышь
void kuNavigator::mouse( int x, int y, int button, void *&mouseState, int type ) {
    bool mouseCaptured = ( mouseState != 0 );
    bool captured = mouseCaptured && (mouseState == this);
    if ( mouseCaptured && !captured ) { return; }
    
    if ( type == 0 ) {  //move
        return;
    }
    
    float time0 = time0Pix();
    float time1 = time1Pix();
    if ( type == 1 ) { //press
        if ( ofInRange( x, time0 - 5, time1 + 5 ) && ofInRange( y, y_, y_+h_+8 ) ) {
            dragLeft = ( (time0 + 5 < time1 && x < time0 + 5 )
                        || (time0 + 5 >= time1 && x < time0 ) );
            dragRight = ( (time1 - 5 > time0 && x > time1 - 5 )
                                     || (time1 - 5 <= time0 && x > time1 ) );
            if ( !dragLeft && !dragRight ) {    //если очень узкая - тащим оба конца
                dragLeft = dragRight = true;
            }
            dragLeftDelta = ( time0 - x );
            dragRightDelta = ( time1 - x );
            state = 1;
            mouseState = this;
        }
        return;
    }
    if ( type == 2 ) { //drag
        if ( captured ) {
            float newtime0 = ( dragLeft ) ? ( pixToTime(dragLeftDelta + x)) : time0_;
            float newtime1 = ( dragRight ) ? ( pixToTime(dragRightDelta + x)) : time1_;
            
            if ( dragLeft && newtime0 > newtime1 ) newtime0 = newtime1;
            newtime1 = max( newtime0, newtime1 );
            float len = newtime1 - newtime0;
            if ( !ofInRange( newtime0, 0, duration_ ) ) {
                newtime0 = ofClamp( newtime0, 0, duration_ );
                if ( dragRight ) newtime1 = newtime0 + len;
            }
            if ( !ofInRange( newtime1, 0, duration_ ) ) {
                newtime1 = ofClamp( newtime1, 0, duration_ );
                if ( dragLeft ) newtime0 = newtime1 - len;
            }
            setSegment( newtime0, newtime1 );
        }
        return;
    }
    if ( type == 3 ) { //release
        if ( captured ) {
            mouseState = 0;
        }
        state = 0;
        return;
    }
}

//---------------------------------------------------------------------


#include "kuButton.h"


//---------------------------------------------------------------------
void kuButton::setup( string iconFile, float X, float Y ) {
	
	X_ = X;
	Y_ = Y;
    ofLoadImage( icon_, iconFile );
    W_ = icon_.getWidth();
    H_ = icon_.getHeight();
    
    toggle_ = false;
    enabled_ = true;
    visible_ = true;
    down_ = false;
    
    state_ = 0;
    pressed_ = 0;
}

//---------------------------------------------------------------------
void kuButton::update( float dt ) {
    pressed_ = 0;
}

//---------------------------------------------------------------------
void kuButton::draw() {
    if ( visible_ ) {
        ofColor mainColor = ( enabled_ ) ? ofColor( 255 ) : ofColor( 128 );
        ofSetColor( mainColor );
        icon_.draw(X_, Y_);
        
        if ( down_ || state_ ) {
            ofColor overColor = ( state_ ) ? ofColor( 160 ) : ofColor( 128 );
            ofSetColor( overColor );
            ofFill();
            ofEnableBlendMode( OF_BLENDMODE_ADD );
            ofRect( X_, Y_, W_, H_ );
            ofEnableAlphaBlending();
        }
    }
}

//---------------------------------------------------------------------
void kuButton::setToggle( bool toggle ) {
    toggle_ = toggle;
}

//---------------------------------------------------------------------
void kuButton::setEnabled( bool enabled ) {
    enabled_ = enabled;
}

//---------------------------------------------------------------------
void kuButton::setDown( bool down ) {
    down_ = down;
}

//---------------------------------------------------------------------
void kuButton::setVisible( bool visible ) {
    visible_ = visible;
}

//---------------------------------------------------------------------
void kuButton::setPressed() {  //эмуляция нажатой кнопки - только если она включена
    if ( visible_ && enabled_ ) {
        pressed_ = true;
        if ( toggle_ ) {
            down_ = !down_;
        }
    }
}

//---------------------------------------------------------------------
//type 0 - move, 1 - press, 2 - drag, 3 - release
void kuButton::mouse( int x, int y, int button, void *&mouseState, int type ) {
    bool mouseCaptured = ( mouseState != 0 );
    bool captured = mouseCaptured && (mouseState == this);
    if ( mouseCaptured && !captured ) { return; }

    if ( !visible_ || !enabled_ ) {
        if ( captured ) {
            mouseState = 0;
            state_ = 0;
        }
        return;
    }
    
    if ( type == 0 ) {  //move
        return;
    }
    if ( type == 1 ) { //press
        if ( ofInRange( x, X_, X_ + W_ ) && ofInRange( y, Y_, Y_ + H_ ) ) {
            state_ = 1;
            mouseState = this;
            pressed_ = true;
            if ( toggle_ ) {
                down_ = !down_;
            }
        }
        return;
    }
    if ( !captured ) { return; }

    if ( type == 2 ) { //drag
        return;
    }
    if ( type == 3 ) { //release
        state_ = 0;
        mouseState = 0;
        return;
    }
    
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void kuButtons::setup( int maxButtons ) {
    button_.resize( maxButtons );
    n_ = 0;
}

//---------------------------------------------------------------------
kuButton *kuButtons::addButton( string iconFile, float X, float Y ) {
    if ( n_ < button_.size() ) {
        int id = n_;
        n_++;
        button_[id].setup( iconFile, X, Y );
        return &button_[id];
    }
    else {
        return 0;
    }
}

//---------------------------------------------------------------------
void kuButtons::update( float dt ) {
    for (int i=0; i<n_; i++) {
        button_[i].update( dt );
    }
}

//---------------------------------------------------------------------
void kuButtons::draw() {
    for (int i=0; i<n_; i++) {
        button_[i].draw();
    }
}

//---------------------------------------------------------------------
//обработка событий мыши
//type 0 - move, 1 - press, 2 - drag, 3 - release
//mouseState - указатель, использует ли кто-то мышь
void kuButtons::mouse( int x, int y, int button, void *&mouseState, int type ) {
    for (int i=0; i<n_; i++) {
        button_[i].mouse( x, y, button, mouseState, type );
    }
}
    
//---------------------------------------------------------------------


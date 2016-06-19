#include "kuMenu.h"


//---------------------------------------------------------------------
void kuMenu::setup( string fontFile, int fontSize, float X0, float Y0,
                   float indentX0, float indentY0,
                   float marginX0, float marginY0 ) {
	font.loadFont( fontFile, fontSize );
	X = X0;
	Y = Y0;
    indentX = indentX0;
    indentY = indentY0;
    marginX = marginX0;
    marginY = marginY0;
    
	size = 0;
    state = 0;
    overSub = -1;
    overItem = -1;
    commandName_ = "";
    

	updateDrawProp();
}

//---------------------------------------------------------------------
void kuMenu::update( float dt ) {

}

//---------------------------------------------------------------------
void kuMenu::updateDrawProp() {
	size = subNames.size();
	sizes.resize( size );
	for (int i=0; i<size; i++) {
		sizes[i] = names[i].size();
	}

	lineH = font.getLineHeight();

	subRects.resize( size );
	rects.resize( size );
    float x = 0;
	for (int i=0; i<size; i++) {
        float w = font.stringWidth( subTitles[i] ) + 2*marginX;
        subRects[i] = ofRectangle( x + X, Y, w, lineH + 2 * marginY );
        x += w + indentX;
        
        //подменю
        int n1 = sizes[i];
        float w1 = 40;
		for (int j=0; j<n1; j++) {
			w1 = max( w1, font.stringWidth( titles[i][j] ) );
		}
        w1 += 2*marginX;
        rects[i].resize( n1 );
        float x1 = subRects[i].x + marginX;
        float y1 = subRects[i].y + lineH + 2*marginY + indentY;
		for (int j=0; j<n1; j++) {
            rects[i][j] = ofRectangle( x1, y1 + (lineH + 2*marginY + indentY) * j, w1, lineH + 2*marginY );
		}
        
	}
}

//---------------------------------------------------------------------
void kuMenu::draw() {
	ofPushMatrix();
	ofTranslate( X, Y );
	for ( int i=0; i<subTitles.size(); i++ ) {
        ofRectangle &r = subRects[i];
		ofSetColor( 64 );
		ofFill();
		ofRect( r );
		ofSetColor( 255 );
        ofNoFill();
		ofRect( r );
        font.drawString( subTitles[i], r.x + marginX, r.y + lineH + marginY );
        if ( state == 1 && i == subItem && !rects[i].empty()) {
            float x0 = rects[i][0].getMinX() - 5;
            float x1 = rects[i][0].getMaxX() + 5;
            float y0 = rects[i][0].getMinY() - 3;
            float y1 = rects[i][rects[i].size()-1].getMaxY() + 5;
            ofSetColor( 0, 128 );
            ofFill();
            ofRect( x0, y0, x1-x0, y1-y0 );
            for (int j=0; j<titles[i].size(); j++) {
                ofRectangle &r = rects[i][j];
                ofSetColor( 64 );
                ofFill();
                ofRect( r );
                ofSetColor( 255 );
                ofNoFill();
                ofRect( r );
                font.drawString( titles[i][j], r.x + marginX, r.y + lineH + marginY );
            }
        }
	}
	ofPopMatrix();
}

//---------------------------------------------------------------------
void kuMenu::addSubmenu( string name, string title ) {
	subNames.push_back( name );
	subTitles.push_back( title );
    names.push_back( vector<string>() );
    titles.push_back( vector<string>() );
	updateDrawProp();
}

//---------------------------------------------------------------------
int kuMenu::findSub( string name ) {
	for (int i=0; i<subNames.size(); i++) {
		if ( subNames[i] == name ) return i;
	}
	return -1;
}

//---------------------------------------------------------------------
void kuMenu::addItem( string name, string title, string submenuName ) {
    int i = findSub( submenuName );
    if ( i >= 0 ) {
        names[i].push_back( name );
        titles[i].push_back( title );
        updateDrawProp();
    }
}

//---------------------------------------------------------------------
void kuMenu::cancel() {
    state = 0;
    commandName_ = "";
}

//---------------------------------------------------------------------
int kuMenu::findSubRect( int x, int y ) {
    x -= X;
    y -= Y;
    for (int i=0; i<subRects.size(); i++) {
        if ( subRects[i].inside(x, y) ) {
            return i;
        }
    }
    return -1;
}

//---------------------------------------------------------------------
int kuMenu::findRect( int x, int y, int sub ) {
    x -= X;
    y -= Y;
    for (int j=0; j<rects[sub].size(); j++) {
        if ( rects[sub][j].inside(x, y) ) {
            return j;
        }
    }
    return -1;
}

//---------------------------------------------------------------------
string kuMenu::commandName() { //выдает команду которую нужно выполнить
    string res = commandName_;
    commandName_ = "";
    return res;
}

//---------------------------------------------------------------------
//type 0 - move, 1 - press, 2 - drag, 3 - release
void kuMenu::mouse( int x, int y, int button, void *&mouseState, int type ) {
    bool mouseCaptured = ( mouseState != 0 );
    bool captured = mouseCaptured && (mouseState == this);
    if ( mouseCaptured && !captured ) { return; }
    
    if ( type == 0 ) {  //move
        return;
    }
    if ( type == 1 ) { //press
        int sub = findSubRect( x, y );
        if ( state == 0 ) {
            if ( sub >= 0 ) {
                state = 1;
                mouseState = this;
                subItem = sub;
            }
        }
        else {
            bool newSub = false;
            if ( sub >= 0 ) {
                if ( sub == subItem ) { state = 0; }
                else { subItem = sub; newSub = true; }
            }
            if ( !newSub ) {
                int j = findRect(x, y, subItem);
                if ( j >= 0 ) {
                    state = 0;
                    commandName_ = names[subItem][j];
                }
                else {
                    state = 0;
                }
            }
        }
        return;
    }
    if ( !captured ) { return; }

    if ( type == 2 ) { //drag
        return;
    }
    if ( type == 3 ) { //release
        if ( state == 0 && captured )  {
            mouseState = 0;
        }

        return;
    }
    
}

//---------------------------------------------------------------------

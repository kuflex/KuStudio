#include "pbGraphicUtils.h"

//--------------------------------------------------------------
//огрубление с шагом size
float coarse( float x, float size ) {
    return int( x / size + 0.5 ) * size;
}

//--------------------------------------------------------------
//1, -1 равновероятно
int randomSign() {
    return ( ofRandom( 0, 1 ) < 0.5 ) ? 1 : (-1);
}

//--------------------------------------------------------------
float sqr( float x ) {
    return x * x;
}


//-------------------------------------------------------------- make rectangles for GL Drawing with drawRects
void pbPushRect( float x, float y, float w, float h, const ofColor &color, vector<ofPoint> &points, vector<ofColor> &colors )
{
    points.push_back( ofPoint( x, y ) );
    points.push_back( ofPoint( x + w, y ) );
    points.push_back( ofPoint( x + w, y + h ) );
    points.push_back( ofPoint( x, y + h ) );
    
    colors.push_back( color );
    colors.push_back( color );
    colors.push_back( color );
    colors.push_back( color );
}

//--------------------------------------------------------------
void pbPushRect( float x, float y, float z, float w, float h, const ofColor &color,
                vector<ofPoint> &points, vector<ofColor> &colors )
{
    points.push_back( ofPoint( x, y, z ) );
    points.push_back( ofPoint( x + w, y, z ) );
    points.push_back( ofPoint( x + w, y + h, z ) );
    points.push_back( ofPoint( x, y + h, z ) );
    
    colors.push_back( color );
    colors.push_back( color );
    colors.push_back( color );
    colors.push_back( color );
}


//--------------------------------------------------------------
void pbPushRect( const ofPoint &p, const ofPoint &w, const ofPoint &h, const ofColor &color,
                vector<ofPoint> &points, vector<ofColor> &colors )
{
    
    points.push_back( p );
    points.push_back( p + w );
    points.push_back( p + w + h );
    points.push_back( p + h );
    
    colors.push_back( color );
    colors.push_back( color );
    colors.push_back( color );
    colors.push_back( color );
}


//-------------------------------------------------------------- GL Drawing rectangles
void pbDrawRects( vector<ofPoint> &points, vector<ofColor> &colors ) {
    if ( points.empty() ) { return; }
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer( 3, GL_FLOAT, sizeof( ofPoint ), &points[0].x);
    glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( ofColor ), &colors[0].v[0]);
    
    glDrawArrays(GL_QUADS, 0, points.size());
    
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}


//--------------------------------------------------------------
//текстурированные прямоугольники
void pbPushRect( const ofPoint &p, const ofPoint &w, const ofPoint &h,
                const ofRectangle &texRect,
                vector<ofPoint> &points, vector<ofVec2f> &texs )
{
    points.push_back( p );
    points.push_back( p + w );
    points.push_back( p + w + h );
    points.push_back( p + h );
    
    texs.push_back( texRect.getTopLeft() );
    texs.push_back( texRect.getTopRight() );
    texs.push_back( texRect.getBottomRight() );
    texs.push_back( texRect.getBottomLeft() );
}

//--------------------------------------------------------------
void pbPushRect( float x, float y, float z, float w, float h,
                const ofRectangle &texRect,
                vector<ofPoint> &points, vector<ofVec2f> &texs )
{
    points.push_back( ofPoint( x, y, z ) );
    points.push_back( ofPoint( x + w, y, z ) );
    points.push_back( ofPoint( x + w, y + h, z ) );
    points.push_back( ofPoint( x, y + h, z ) );
    
    texs.push_back( texRect.getTopLeft() );
    texs.push_back( texRect.getTopRight() );
    texs.push_back( texRect.getBottomRight() );
    texs.push_back( texRect.getBottomLeft() );
    
}

//--------------------------------------------------------------
void pbPushRect( const ofPoint &p1, const ofPoint &p2, const ofPoint &p3, const ofPoint &p4,
                const ofRectangle &texRect,
                vector<ofPoint> &points, vector<ofVec2f> &texs ) {
    points.push_back( p1 );
    points.push_back( p2 );
    points.push_back( p3 );
    points.push_back( p4 );
    
    texs.push_back( texRect.getTopLeft() );
    texs.push_back( texRect.getTopRight() );
    texs.push_back( texRect.getBottomRight() );
    texs.push_back( texRect.getBottomLeft() );
}

//--------------------------------------------------------------
void pbDrawRects( vector<ofPoint> &points, vector<ofVec2f> &texs, ofTexture &texture )
{
    if ( points.empty() ) { return; }
   
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer( 3, GL_FLOAT, sizeof( ofPoint ), &points[0].x);
    glTexCoordPointer( 2, GL_FLOAT, sizeof( ofVec2f ), &texs[0].x );
    
    texture.bind();
    glDrawArrays(GL_QUADS, 0, points.size());
    texture.unbind();
		
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
}

//--------------------------------------------------------------
//текстурированные прямоугольники с цветом
void pbPushRect( float x, float y, float z, float w, float h,
                const ofColor &color,
                const ofRectangle &texRect,
                vector<ofPoint> &points, vector<ofColor> &colors, vector<ofVec2f> &texs )
{
    points.push_back( ofPoint( x, y, z ) );
    points.push_back( ofPoint( x + w, y, z ) );
    points.push_back( ofPoint( x + w, y + h, z ) );
    points.push_back( ofPoint( x, y + h, z ) );
    
    colors.push_back( color );
    colors.push_back( color );
    colors.push_back( color );
    colors.push_back( color );

    texs.push_back( texRect.getTopLeft() );
    texs.push_back( texRect.getTopRight() );
    texs.push_back( texRect.getBottomRight() );
    texs.push_back( texRect.getBottomLeft() );
}

//--------------------------------------------------------------
void pbPushRect( const ofPoint &p, const ofPoint &w, const ofPoint &h,
                const ofColor &color,
                const ofRectangle &texRect,
                vector<ofPoint> &points, vector<ofColor> &colors, vector<ofVec2f> &texs )
{
    points.push_back( p );
    points.push_back( p + w );
    points.push_back( p + w + h );
    points.push_back( p + h );
    
    colors.push_back( color );
    colors.push_back( color );
    colors.push_back( color );
    colors.push_back( color );
    
    texs.push_back( texRect.getTopLeft() );
    texs.push_back( texRect.getTopRight() );
    texs.push_back( texRect.getBottomRight() );
    texs.push_back( texRect.getBottomLeft() );
}

//--------------------------------------------------------------
void pbDrawRects( vector<ofPoint> &points, vector<ofColor> &colors,
                 vector<ofVec2f> &texs, ofTexture &texture )
{
    if ( points.empty() ) { return; }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glVertexPointer( 3, GL_FLOAT, sizeof( ofPoint ), &points[0].x);
    glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( ofColor ), &colors[0].v[0]);
    glTexCoordPointer( 2, GL_FLOAT, sizeof( ofVec2f ), &texs[0].x );
    
    texture.bind();
    glDrawArrays(GL_QUADS, 0, points.size());
    texture.unbind();
    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

//----------------------------------------------------------
void pbStartSmoothing(){
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
    
	//why do we need this?
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


//----------------------------------------------------------
void pbEndSmoothing(){
    glDisable(GL_LINE_SMOOTH);
}

//--------------------------------------------------------------
//цветные линии
void pbPushLine( const ofPoint &p1, const ofPoint &p2, const ofColor &color,
                vector<ofPoint> &points, vector<ofColor> &colors )
{
    points.push_back( p1 );
    points.push_back( p2 );
    
    colors.push_back( color );
    colors.push_back( color );
}


void pbPushLine( const ofPoint &p1, const ofPoint &p2,
                const ofColor &color1, const ofColor &color2,
                vector<ofPoint> &points, vector<ofColor> &colors )
{
    points.push_back( p1 );
    points.push_back( p2 );
    
    colors.push_back( color1 );
    colors.push_back( color2 );
}

//--------------------------------------------------------------
void pbDrawLines( vector<ofPoint> &points, vector<ofColor> &colors )
{
    if ( points.empty() ) { return; }

    pbStartSmoothing();     //включаем сглаживание
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer( 3, GL_FLOAT, sizeof( ofPoint ), &points[0].x);
    glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( ofColor ), &colors[0].v[0]);
    
    glDrawArrays(GL_LINES, 0, points.size());
    
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    //pbEndSmoothing();       //выключаем сглаживание

}

//--------------------------------------------------------------
//предполагается, что текстура уже подключена
void pbDrawLines_textured( vector<ofPoint> &points,
                          vector<ofVec2f> &texs ) {
    if ( points.empty() ) { return; }
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glVertexPointer( 3, GL_FLOAT, sizeof( ofPoint ), &points[0].x);
    glTexCoordPointer( 2, GL_FLOAT, sizeof( ofVec2f ), &texs[0].x );
    
    //texture.bind();
    glDrawArrays(GL_LINES, 0, points.size());
    //texture.unbind();
    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

//--------------------------------------------------------------

void pbVertexRender::setup( string pointImageFile, string shaderVert, string shaderFrag )
{
    ofDisableArbTex();
	ofLoadImage( texture, pointImageFile );
	ofEnableArbTex();

    useShader_ = (shaderVert!="" && shaderFrag!="");
    if ( useShader_ ) {
        shader_.load( shaderVert, shaderFrag );
    }
}

//--------------------------------------------------------------
void pbVertexRender::clearPoints()
{
    points.clear();
    sizes.clear();
}

//--------------------------------------------------------------
void pbVertexRender::addPoint( const ofPoint &p, float rad, float alpha )
{
    points.push_back( p );
    sizes.push_back( ofPoint( rad, alpha, 0 ) );
}

//--------------------------------------------------------------
void pbVertexRender::setPoints( vector<ofPoint> &v, float minRad, float maxRad ) {
    points = v;
    int n = v.size();
    sizes.resize( n );
    for (int i=0; i<n; i++) {
        sizes[i] = ofPoint( ofRandom( minRad, maxRad ), 1, 0 );
    }
}

//--------------------------------------------------------------
void pbVertexRender::setPoints( vector<ofPoint> &v, vector<ofPoint> &data ) {
    points = v;
    sizes = data;
}

//--------------------------------------------------------------
void pbVertexRender::drawBegin( float part ) {
    //начало рисования, можно установить параметры в шейдер
    if ( points.empty() ) { return; }
    part = ofClamp( part, 0, 1 );
    int total = (int)points.size() * part;
    if ( total == 0 ) return;
    
	vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
	
    //	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnablePointSprites();

    if ( useShader_ ) {
        shader_.begin();
    }
}

//--------------------------------------------------------------
void pbVertexRender::drawEnd( float part ){ //конец рисования
    if ( points.empty() ) { return; }
    part = ofClamp( part, 0, 1 );
    int total = (int)points.size() * part;
    if ( total == 0 ) return;
    
    texture.bind();
	vbo.draw(GL_POINTS, 0, total);
	texture.unbind();
	
    if ( useShader_ ) {
        shader_.end();
    }
	
	ofDisablePointSprites();
	//ofDisableBlendMode();
    
    //    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void pbVertexRender::draw()
{
	drawBegin();
    drawEnd();
	
}
    
//--------------------------------------------------------------


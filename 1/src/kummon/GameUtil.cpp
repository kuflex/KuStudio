#include "GameUtil.h"

//---------------------------------------------------------------------------
float linePoints3[6];
void ofLine3( const ofPoint &p0, const ofPoint &p1 )
{
	linePoints3[0] = p0.x;
	linePoints3[1] = p0.y;
	linePoints3[2] = p0.z;
	linePoints3[3] = p1.x;
	linePoints3[4] = p1.y;
	linePoints3[5] = p1.z;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &linePoints3[0]);
	glDrawArrays(GL_LINES, 0, 2);
}


//---------------------------------------------------------------------------
void ofLine3Eps( const ofPoint &p0, const ofPoint &p1, const ofPoint eps )
{
	 ofLine3( p0 + eps, p1 + eps );
}

//---------------------------------------------------------------------------
void ofLines( const vector<float> &pointsXY, bool closed )	//рисует кривую, заданную линиями
{
	int n = pointsXY.size() / 2;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &pointsXY[0]);
	glDrawArrays( (!closed) ? GL_LINES : GL_LINE_LOOP, 0, n);
}

//---------------------------------------------------------------------------
float rectPoints3[12];
void ofRect3( const ofPoint &p0, const ofPoint &p1, const ofPoint &p2, const ofPoint &p3,
			 bool filled )
{
	rectPoints3[0] = p0.x;
	rectPoints3[1] = p0.y;
	rectPoints3[2] = p0.z;

	rectPoints3[3] = p1.x;
	rectPoints3[4] = p1.y;
	rectPoints3[5] = p1.z;

	rectPoints3[6] = p2.x;
	rectPoints3[7] = p2.y;
	rectPoints3[8] = p2.z;

	rectPoints3[9] = p3.x;
	rectPoints3[10] = p3.y;
	rectPoints3[11] = p3.z;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &rectPoints3[0]);
	glDrawArrays( filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, 4);
}

//---------------------------------------------------------------------------
float rectTex2[8];
void ofRect3Textured( const ofPoint &p0, const ofPoint &p1, const ofPoint &p2, const ofPoint &p3,
			 ofImage &texture )
{
	rectPoints3[0] = p0.x;
	rectPoints3[1] = p0.y;
	rectPoints3[2] = p0.z;

	rectPoints3[3] = p1.x;
	rectPoints3[4] = p1.y;
	rectPoints3[5] = p1.z;

	rectPoints3[6] = p2.x;
	rectPoints3[7] = p2.y;
	rectPoints3[8] = p2.z;

	rectPoints3[9] = p3.x;
	rectPoints3[10] = p3.y;
	rectPoints3[11] = p3.z;
	
	float w = texture.getWidth();
	float h = texture.getHeight();
	rectTex2[0] = 0;
	rectTex2[1] = 0;

	rectTex2[2] = w-1;
	rectTex2[3] = 0;

	rectTex2[4] = w-1;
	rectTex2[5] = h-1;

	rectTex2[6] = 0;
	rectTex2[7] = h-1;

	texture.getTextureReference().bind();

	//вершины
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &rectPoints3[0]);

	//текстура
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer( 2, GL_FLOAT, 0, &rectTex2[0] );

	//рисуем
	glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	texture.getTextureReference().unbind();

}

//---------------------------------------------------------------------------
void ofSetColor0( const ofColor &color, float alpha )
{
	ofSetColor( color.r, color.g, color.b, color.a * alpha );
}

//---------------------------------------------------------------------------
float ofDist( const ofPoint &p0, const ofPoint &p1 ) 
{
	return ofDist( p0.x, p0.y, p1.x, p1.y );
}

//---------------------------------------------------------------------------
ofPoint ofLerp(ofPoint start, ofPoint stop, float amt)
{
	return start + (stop-start) * amt;
}

//---------------------------------------------------------------------------
ofPoint ofPointUnit( float angleDeg )	//единичный вектор
{
	return ofPoint( cos( angleDeg * DEG_TO_RAD ), sin( angleDeg * DEG_TO_RAD ) );
}

//---------------------------------------------------------------------------
//void ofTranslate( const ofPoint &p )
//{
//	ofTranslate( p.x, p.y, p.z );
//}

//---------------------------------------------------------------------------
void drawOfImageCentered( ofImage &image, ofPoint center, float scale, float w, float h )
{
	if ( w < 0 || h < 0 ) {
		w = image.getWidth() * scale;
		h = image.getHeight() * scale;
	}
	image.draw( center.x - w/2, center.y - h/2, w, h );

}

//---------------------------------------------------------------------------
//Переворот картинки. Например, перед вызовом у imageOut можно поставить bUseTexture=false
/*void ofFlipImage( ofImage &image, ofImage &imageOut, bool horiz, bool vert )
{
	int step = image.bpp / 8;

	int w = image.width;
	int W = w * step;
	int h = image.height;
	vector<unsigned char> data( W * h );
	unsigned char *input = image.getPixels();
	unsigned char *pixels = &data[0];

	if ( vert ) {
		for (int y=0; y<h; y++) {
			for (int x=0; x<W; x++) {
				pixels[ x + W * y ] = input[ x + W * ( h - 1 - y ) ];
			}
		}
	}
	if ( horiz ) {
		//TODO
	}

	imageOut.setFromPixels( pixels, w, h, image.type );


}*/

//---------------------------------------------------------------------------
ofColor makeOfColor( int r, int g, int b, int a )
{
	ofColor c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
}

//---------------------------------------------------------------------------
/*ofColor ofImagePixel( ofImage &image, int x, int y )
{
	ofColor color;
	if ( image.width > 0 && image.height > 0 ) {
		x = min( max( x, 0 ), min( x, image.width-1 ) );
		y = min( max( y, 0 ), min( y, image.height-1 ) );
		int step = image.bpp / 8;
		int index = step * (x + image.width * y);
		unsigned char *pixels = image.getPixels();
		if ( step >= 3 ) {
			color.r = pixels[ index + 0 ];
			color.g = pixels[ index + 1 ];
			color.b = pixels[ index + 2 ];
			color.a = ( step == 4 ) ? pixels[ index + 3 ] : 255;
		}
		if ( step == 1 ) {
			color.r = color.g = color.b = pixels[ index ];
			color.a = 255;
		}
	}
	return color;
}*/


//---------------------------------------------------------------------------
/*ofColor ofImagePixelUnit( ofImage &image, float x, float y )
{
	return ofImagePixel( image, x * image.width, y * image.height );
}*/

//---------------------------------------------------------------------------
//рисование без изменения пропорций
void pbPushMatrixFit( float W, float H, float w, float h, bool drawBorder )
{
	ofPushMatrix();
	ofTranslate( W/2, H/2 );
	float scl = min( W / w, H / h );
	ofScale( scl, scl );
	ofTranslate( -w/2, -h/2 );
	
	//границы
	if ( drawBorder ) {
		ofColor col = ofGetStyle().color;
		ofSetColor( 128, 128, 128 );
		ofNoFill();
		ofRect( 0, 0, w, h );
		ofSetColor0( col );
	}

}


//-------------------------------------------
void pbSetTextureNearestFilter( ofImage &image ) 
{
	ofTexture &tex = image.getTextureReference();

	tex.bind();		
	glTexParameterf(tex.texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(tex.texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	tex.unbind();

}

//-------------------------------------------
//Рисование оптического потока, цвет не ставит
void pbDrawFlow( const vector<float> &flow, int w, int h, float eps )
{
	//float *flowP = &_flow[0];
	for (int y=0; y<h; y++) {
		for (int x=0; x<w; x++) {
			float fx = flow[ (x + w * y) * 2 ];
			float fy = flow[ (x + w * y) * 2 + 1 ];
			if ( fabs( fx ) + fabs( fy ) > eps ) {
				ofLine( x, y, x + fx, y + fy );
			}
		}
	}
}

//-------------------------------------------

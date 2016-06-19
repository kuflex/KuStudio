#pragma once

//Noise generator

#include "ofMain.h"

class pbNoiseGenerator
{
public:
	pbNoiseGenerator(void) { pos = speed = value0 = value1 = 0; }

	void setup( float value00, float value10, float speed0 ) {
		setParams( value00, value10, speed0 );
		restart();
	}
	void setParams( float value00, float value10, float speed0 ) {
		value0 = value00;
		value1 = value10;
		speed = speed0;
	}

	void restart() {
			pos = ofRandom( 0, 1000 );
	}
	void update( float dt ) {
		pos += speed * dt;
	}
	float value() {
		return ofMap( ofNoise( pos ), 0, 1, value0, value1 );
	}

	float speed, pos;
	float value0, value1;
};

//случайная точка
class pbNoisePoint {
public:

	void setup( float x0, float x1, float y0, float y1, float z0, float z1, float speed ) {
		nx.setup( x0, x1, speed );
		ny.setup( y0, y1, speed );
		nz.setup( z0, z1, speed );
		restart();
	}
	void restart() {
		nx.restart(); ny.restart(); nz.restart(); 
	}
	void setSpeed( float speed ){
		nx.speed = ny.speed = nz.speed = speed;
	}
	void update( float dt ) {
		nx.update( dt ); 
		ny.update( dt );
		nz.update( dt );
	}

	ofPoint value() {
		return ofPoint( nx.value(), ny.value(), nz.value() );
	}

	pbNoiseGenerator nx, ny, nz;
};
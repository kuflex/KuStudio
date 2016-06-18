#pragma once

//Приём данных OSC в отдельном потоке

#include "ofMain.h"
#include "ofxOsc.h"

class pbOscReceiver : public ofThread{
public:
	pbOscReceiver() { _cb = 0; }
	void setup( int port, bool threaded = true );	
	//if threaded == true, it is asynchronous mode
	void setCallback( void (*cb)(ofxOscMessage &m) ) { _cb = cb; }

	void close();

	void threadedFunction();
	void receive();

	int count;
private:
	bool _threaded;

	void (*_cb)(ofxOscMessage &m);

	ofxOscReceiver oscReceiver;					

	void receive0();
};

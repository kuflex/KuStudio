#pragma once
//Получатель OSC

#include "ofMain.h"
#include "ofxOsc.h"

class kuOscReceive
{
public:
    kuOscReceive();
    ~kuOscReceive();
	void setup( int port ); //0 - значит не стартовать
    void setupFromString( const string &s );
    void setupFromDialog( const string &title );

    string toString();
    
    void start();
    void stop();
    
    bool hasWaitingMessages();
	bool getNextMessage( ofxOscMessage &m );
   
    int port() { return port_; }
private:
    int port_;
	ofxOscReceiver *receiver_;
    bool started_;
    
    void restartIfStarted();
    
    void startReceiver();
    void stopReceiver();
};


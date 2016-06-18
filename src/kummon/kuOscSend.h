#pragma once
//Посылатель OSC

#include "ofMain.h"
#include "ofxOsc.h"
#include "ParamMap.h"

class kuOscSend
{
public:
    kuOscSend();
    ~kuOscSend();
    void link( ParamMap &ini, string title );
	void setup( string host, int port );
    void setupFromString( const string &s );
    void setupFromDialog( const string &title );

    void params_updated();

    string toString();
    
    void start();
    void stop();
    
    void sendCommand( string address );
    void sendFloat( string address, float value );
    void sendString( string address, const string &value );
    void sendStrings( string address, const vector<string> &values );
   
private:
    string host_;
    int port_;
	ofxOscSender *sender_;
    
    bool linked_;
    
    void restartIfStarted();
    string host_start_; //с какими параметрами запущен
    int port_start_;
    
    void appendAddress( string &address );
};


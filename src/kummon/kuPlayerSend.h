#pragma once
//Посылатель OSC в kuPlayer

#include "ofMain.h"
#include "kuOscSend.h"

class kuPlayerSend : public kuOscSend
{
public:
    //kuPlayerSend();
    //~kuPlayerSend();
	//void setup( string host, int port );
    //void setupFromString( const string &s );
    //void setupFromDialog( const string &title );

    //string toString();
    
    //void start();
    //void stop();
    
    //void sendFloat( const string &address, float value );
    void sendStop();
    void sendTime( float time );
   
private:
    
};


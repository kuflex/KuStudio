#include "pbOscReceiver.h"

//-------------------------------------------------------------------
void pbOscReceiver::setup( int port, bool threaded )		
{
	_threaded = threaded;

	cout << "Starting OSC receiver, port " << port << endl;
	oscReceiver.setup( port );

	if ( _threaded ) {
		startThread( true, false );   //blocking, verbose
	}

	count = 0;
}

//-------------------------------------------------------------------
void pbOscReceiver::close()
{
	_cb = 0;
	if ( _threaded ) {
		stopThread();		//TODO - it crashes application!
	}
	//oscReceiver.close();
}

//-------------------------------------------------------------------
void pbOscReceiver::threadedFunction()
{
	while( isThreadRunning() != 0 ){
		//Receive
		receive0();
	}
}

//-------------------------------------------------------------------
void pbOscReceiver::receive()
{
	if ( !_threaded ) {
		receive0();
	}
}

//-------------------------------------------------------------------
void pbOscReceiver::receive0()
{
	while (oscReceiver.hasWaitingMessages() ) {
		count++;
        ofxOscMessage m;
        oscReceiver.getNextMessage( &m );
        if ( _cb ) {
			_cb( m );
		}
    }
}

//-------------------------------------------------------------------
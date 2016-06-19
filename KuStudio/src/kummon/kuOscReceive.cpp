#include "kuOscReceive.h"
#include "kuConsole.h"


//---------------------------------------------------------------------
kuOscReceive::kuOscReceive() {
    receiver_ = 0;
    port_ = 12340;
    started_ = false;
}

//---------------------------------------------------------------------
kuOscReceive::~kuOscReceive() {
    stop();
}

//---------------------------------------------------------------------
void kuOscReceive::setup( int port ) {
    port_ = port;
    restartIfStarted();
}

//---------------------------------------------------------------------
void kuOscReceive::setupFromString( const string &s ) {
    vector<string> list = ofSplitString( s, " " );
    if ( list.size() >= 1 ) {
        port_ = ofToInt(list[0]);
        restartIfStarted();
    }
}

//---------------------------------------------------------------------
void kuOscReceive::setupFromDialog( const string &title ) {
    string res = systemTextBoxDialog( title, toString() );
    if ( res != "" ) {
        setupFromString( res );
    }
}

//---------------------------------------------------------------------
string kuOscReceive::toString() {
    return ofToString( port_ );
}

//---------------------------------------------------------------------
void kuOscReceive::startReceiver() {
    if ( port_ != 0 ) {
        if ( !receiver_ ) { receiver_ = new ofxOscReceiver(); }
        receiver_->setup( port_ );
    }
    else {
        stopReceiver();
    }
}

//---------------------------------------------------------------------
void kuOscReceive::stopReceiver() {
    if ( receiver_ ) {
        delete receiver_;
        receiver_ = 0;
    }
}

//---------------------------------------------------------------------
void kuOscReceive::restartIfStarted() {
    if ( started_ ) {
        start();
    }
}
//---------------------------------------------------------------------
void kuOscReceive::start() {
    started_ = true;
    startReceiver();
}

//---------------------------------------------------------------------
void kuOscReceive::stop() {
    started_ = false;
    stopReceiver();
}

//---------------------------------------------------------------------
bool kuOscReceive::hasWaitingMessages() {
    if ( receiver_ ) {
        return receiver_->hasWaitingMessages();
    }
    else return false;
}

//---------------------------------------------------------------------
bool kuOscReceive::getNextMessage( ofxOscMessage &m ) {
    if ( receiver_ ) {
        return receiver_->getNextMessage( &m );
    }
    else return false;
}


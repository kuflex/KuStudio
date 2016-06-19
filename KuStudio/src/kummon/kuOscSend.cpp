#include "kuOscSend.h"
#include "kuErrors.h"
#include "kuConsole.h"

//---------------------------------------------------------------------
kuOscSend::kuOscSend() {
    linked_ = false;
    sender_ = 0;
    host_ = "localhost";
    port_ = 12345;

    host_start_ = "";
    port_start_ = 0;
}

//---------------------------------------------------------------------
kuOscSend::~kuOscSend() {
    stop();
}

//---------------------------------------------------------------------
void kuOscSend::link( ParamMap &ini, string title ) {
    linked_ = true;
    ini.addString( host_, title + "_host", host_ );
    ini.addInt( port_, title + "_port", port_ );
}

//---------------------------------------------------------------------
void kuOscSend::setup( string host, int port ) {
    host_ = host;
    port_ = port;
    restartIfStarted();
}

//---------------------------------------------------------------------
void kuOscSend::restartIfStarted() {
    if ( sender_ ) {
        start();
    }
}

//---------------------------------------------------------------------
void kuOscSend::params_updated() {
    if ( port_start_ != port_ || host_start_ != host_ ) {
        restartIfStarted();
    }
}

//---------------------------------------------------------------------
void kuOscSend::setupFromString( const string &s ) {
    vector<string> list = ofSplitString( s, " " );
    if ( list.size() != 2 ) {
        list = ofSplitString( s, ":" );
    }
    if ( list.size() >= 2 ) {
        host_ = list[0];
        port_ = ofToInt(list[1]);
        restartIfStarted();
    }
    else {
        kuAssertWarning(false, "kuOscSend::setupFromString - bad arg '"
                        + s + "', expect 'localhost 12349' or 'localhost:12349'");
    }
}

//---------------------------------------------------------------------
void kuOscSend::setupFromDialog( const string &title ) {
    string res = systemTextBoxDialog( title, toString() );
    if ( res != "" ) {
        setupFromString( res );
    }
}

//---------------------------------------------------------------------
string kuOscSend::toString() {
    return host_ + " " + ofToString( port_ );
}

//---------------------------------------------------------------------
void kuOscSend::start() {
    if ( !sender_ ) { sender_ = new ofxOscSender(); }
    else {
        if ( port_start_ == port_ && host_start_ == host_ ) {
            return;
        }
    }
    sender_->setup( host_, port_ );
    port_start_ = port_;
    host_start_ = host_;
}

//---------------------------------------------------------------------
void kuOscSend::stop() {
    if ( sender_ ) {
        delete sender_;
        sender_ = 0;
    }
}

//---------------------------------------------------------------------
void kuOscSend::appendAddress( string &address ) {
    if ( address != "" && address[0] != '/' ) address = '/' + address;
}

//---------------------------------------------------------------------
void kuOscSend::sendCommand( string address ) {
    if ( sender_ ) {
        appendAddress(address);
        ofxOscMessage m;
        m.setAddress( address );
        sender_->sendMessage( m );
    }
}

//---------------------------------------------------------------------
void kuOscSend::sendFloat( string address, float value ) {
    if ( sender_ ) {
        appendAddress(address);
        ofxOscMessage m;
        m.setAddress( address );
        m.addFloatArg( value );
        sender_->sendMessage( m );
    }
}

//---------------------------------------------------------------------
void kuOscSend::sendString( string address, const string &value ) {
    vector<string> values;
    values.push_back( value );
    sendStrings( address, values );
}

//---------------------------------------------------------------------
void kuOscSend::sendStrings( string address, const vector<string> &values ) {
    if ( sender_ ) {
        appendAddress(address);
        ofxOscMessage m;
        m.setAddress( address );
        for (int i=0; i<values.size(); i++) {
            m.addStringArg( values[i] );
        }
        sender_->sendMessage( m );
    }
}

//---------------------------------------------------------------------


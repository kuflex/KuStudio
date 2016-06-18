#include "kuPlayerSend.h"


//---------------------------------------------------------------------
void kuPlayerSend::sendStop() {
    sendFloat( "/stop", 0 );
}

//---------------------------------------------------------------------
void kuPlayerSend::sendTime( float time ) {
    sendFloat( "/time", time );
}

//---------------------------------------------------------------------


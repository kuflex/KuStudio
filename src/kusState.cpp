#include "kusState.h"
#include "pbFiles.h"
#include "ParamMap.h"

kusState shared_state;
kusCommon kus;


//---------------------------------------------------------------------
void kusState::setup() {
    settingsFile = ofToDataPath( "settings/settings.ini" );
    FPS = 60;
    editButton = false;
    recordButton = false;
    load();
    
}


//---------------------------------------------------------------------
void kusState::fpsChanged() {
    ofSetFrameRate( FPS );
}

//---------------------------------------------------------------------
void kusState::save() {
    vector<string> file;
    file.push_back( "projectFile=" + lastProjectFile );
    file.push_back( "FPS=" + ofToString( FPS ) );
    file.push_back( "editButton=" + ofToString( editButton ? 1 : 0 ) );
    file.push_back( "recordButton=" + ofToString( recordButton ? 1 : 0 ) );
    pbFiles::writeStrings( file, settingsFile );
    
}

//---------------------------------------------------------------------
void kusState::load() {
    ParamMap ini;
    ini.loadIni( settingsFile );
    
    lastProjectFile = ini.getString("projectFile", lastProjectFile);
    FPS = ini.getInt( "FPS", FPS );
    fpsChanged();
    editButton = ini.getInt( "editButton", editButton ? 1 : 0 );
    recordButton = ini.getInt( "recordButton", recordButton ? 1 : 0 );

}

//---------------------------------------------------------------------
void kusState::setFps( int fps ) {
    FPS = fps;
    fpsChanged();
    save();
}

//---------------------------------------------------------------------
void kusState::setProjectFile( string projectFile ) {
    lastProjectFile = projectFile;
    save();
}

//---------------------------------------------------------------------
void kusState::setEditButton( bool down ) {
    editButton = down;
    save();
}

//---------------------------------------------------------------------
void kusState::setEditLineButton( bool down ) {
    editLineButton = down;
    save();
}

//---------------------------------------------------------------------
void kusState::setRecordButton( bool down ) {
    recordButton = down;
    save();
}

//---------------------------------------------------------------------
kusCommon::kusCommon() {
    time0 = time1 = 0;
}

//---------------------------------------------------------------------

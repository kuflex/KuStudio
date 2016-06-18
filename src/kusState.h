#pragma once
//Параметры

#include "ofMain.h"
#include "kuOscSend.h"
#include "kuOscReceive.h"
#include "kuPlayerSend.h"

class kusState
{
public:
	void setup();
    void save();
    void load();

    void setFps( int fps );
    void setProjectFile( string projectFile );
    void setEditButton( bool down );
    void setEditLineButton( bool down );
    void setRecordButton( bool down );
    
    string settingsFile;
    string lastProjectFile;
    int FPS;
    
    bool editButton, editLineButton;
    bool recordButton;
    
private:
    void fpsChanged();

};

extern kusState shared_state;

//общие переменные
struct kusCommon {
    float time0, time1;
    ofTrueTypeFont trackFont1, trackFont2;
    ofTexture trackIcons;
    int trackIconN; //количество кнопок
    
    static const int OscOuts = 4;   //число OSC-выходов
    static const int PlayerOuts = 4;   //число Player-выходов
    vector<kuOscSend> oscOut; //посылатели OSC
    vector<kuPlayerSend> playerOut; //посылатели OSC в kuPlayer
    kuOscReceive oscIn;     //получатель OSC
    
    float leftX;    //левое значение для установки значений
    
    kusCommon();
    

};

extern kusCommon kus;
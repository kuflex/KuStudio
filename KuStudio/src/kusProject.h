#pragma once
//Проект

#include "ofMain.h"
#include "kusTrack.h"
#include "kuWavFile.h"
#include "kuOscSend.h"

const float kScrollDist = 20;   //порог, когда считать что не скролл, а выбор трека

const float kTracksAudioY = 100;
const float kTracksY = kTracksAudioY + 65;


class kusProject
{
public:
	void setup( int w, int h,
               string fontFile1, int fontSize1,
               string fontFile2, int fontSize2 );
    void createNew( bool rewrite = false );
    bool open( string fileName );
    bool openAs();
    bool save( bool saveAs = false, bool forceSave = true );
    bool exportRawText(); //экспортировать в текстовый файл, с разделением TAB
    void shiftAllTracks_sec(); //сдвинуть все треки во времени, в секундах
    void shiftAllTracks_samples(); //сдвинуть все треки во времени, в сэмплах
    
    bool openAudioAs();
    
	void update( float dt );
    void draw();
    
    void setWindowSize( int w, int h );
    void setThumbSize( int w, int h );
    void drawThumb( float x, float y );

    void setViewSegment( float time0, float time1 );
    float time0() { return time0_; }
    float time1() { return time1_; }
    float time() { return time_; }
    string timeString();
    
    void playAll();
    void playSegment();
    void stop();
    void requestSendTracksValues();

    bool isPlayingAll() { return playall_; }
    bool isPlayingSegment() { return play_; }
    bool isPlaying() { return playall_ || play_; }
    
    bool isEmpty() { return duration == 0; }
    
    void addTrack();
    void editTrackName();
    void editTrackOscOut();
    void editTrackRange();
    void editTrackSmooth();
    void editTrackType();
    void deleteTrack();
	bool checkTrackSelected();	//выбран ли какой-то трек, если нет - предупредить
    
    void editOscOut( int index ); //редактировать OSC-выход
    void editPlayerOut( int index ); //редактировать OSC-выход на kuPlayer
    void editOscIn(); //редактировать OSC-вход
    bool setTrackValue( const string &name, float value ); //установить значение в трек по имени
    
	string projectFile;
    string projectName;
    string audioFile;
    float duration;
    vector<kusTrack> tracks;
    int findTrackByName( const string &name, int noValue = -1); 

    ofSoundPlayer audio;
    kuWavFile audioData;
    
    //обработка событий мыши
    //type 0 - move, 1 - press, 2 - drag, 3 - release
    //mouseState - указатель, использует ли кто-то мышь
    void mouse( int x, int y, int button, void *&mouseState, int type );
    
    string name();  //имя файла проекта
    string name_without_extension();  //имя файла проекта без расширения
    
private:
    void setProjectFile( string fileName );
    bool loadAudio( string fileName, bool forceSelection = false,
                   string projecFile = "" );
    ofFileDialogResult loadAudioDialog( string expectedFile = "" );
    
    void durationWasUpdated();
    void afterProjectIsReady();
    
    int thumbw, thumbh;
    float winw_, winh_;
    
    float time0_, time1_;
    float time0Stored_, time1Stored_;
    
    bool playall_;
    bool play_;
    
    float time_;    //текущее время игры
    float timeToPix();
    
    float y0_;
    float audioH_;
    
    float posY_;    //положение треков по Y
    void setPosY( float posY );
    
    void updateTracksPos(); //обновить положение треков на экране
    void applyTrackButton( int ind, int button ); //на треке ind нажата кнопка button
    
    int findIndex( int x, int y ); //-2 - ничего, -1 - звуковой, 0..n - треки
    int findIndexLeft( int x, int y ); //поиск точки в зоне установки значений трека
    bool pointInAudio( int x, int y );
    
    int editTrack_;
    bool settingValue_, editingValue_;

    void tracksSetDirty();
    void audioDataRedraw();
    
    int selectedTrack_;  //выбранный трек
    void deselectTrack();   //сбросить пометку у выбранного трека
    void setSelectedTrack( int ind );
    
    bool scrolling_;
    int scrollTrack_;    //предварительный трек - если не будет скролла
    ofPoint scrollDelta_, scrollPos_;  //запомненное положение мыши - для скролла
    float scrollDist_;
    
    bool editing_, editing_line_;
    bool recording_;
    void setEditing( bool editing );    //режим редактирования - вызывается автоматически в update
    void setEditingLine( bool editingLine );    //режим редактирования линией
    void setRecording( bool recording ); //режим записи - вызывается автоматически в update

    void loopEnded();
    
    bool requestSendValue_;
    
    float last_sent_status_wait_;    //время от последней отправки статуса
};


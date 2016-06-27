#include "kusProject.h"
#include "pbFiles.h"
#include "kusState.h"
#include "converter.h"
#include "kusState.h"
#include "ParamMap.h"
#include "kuConsole.h"

//трек звука - всегда вверху показывается
//по пробелу - запускается с левого края страницы

//---------------------------------------------------------------------
void kusProject::setup( int winw, int winh,
                       string fontFile1, int fontSize1,
                       string fontFile2, int fontSize2 ) {
    winw_ = winw;
    winh_ = winh;
    kus.leftX = 10;
    
    kus.trackFont1.loadFont( fontFile1, fontSize1 );
    kus.trackFont2.loadFont( fontFile1, fontSize1 );
    
    //кнопки трека
    ofLoadImage( kus.trackIcons, "icons/trackIcons.png" );
    kus.trackIconN = 4; //количество кнопок
    
    
    thumbw = 100;
    thumbh = 10;

    
    time0_ = 0;
    time1_ = 1;
    
    playall_ = play_ = false;

    createNew();

    y0_ = kTracksAudioY;
    audioH_ = 50;
    posY_ = 0;
    
    editTrack_ = -2;
    settingValue_ = false;
    editingValue_ = false;
    
    selectedTrack_ = -2;
    
    scrolling_ = false;
    scrollTrack_ = -1;
    scrollDist_ = 0;
    
    editing_ = false;
    editing_line_ = false;
    recording_ = false;
    requestSendValue_ = false;
    
    kus.oscOut.resize( kus.OscOuts );
    for (int i=0; i<kus.OscOuts; i++) {
        kus.oscOut[i].setup("localhost", 12345 + i);
        kus.oscOut[i].start();
    }
    kus.playerOut.resize( kus.PlayerOuts );
    for (int i=0; i<kus.PlayerOuts; i++) {
        kus.playerOut[i].setup("localhost", 12360 + i);
        kus.playerOut[i].start();
    }

    kus.oscIn.setup( 0 );
    kus.oscIn.start();
 
    last_sent_status_wait_ = 0;
}

//---------------------------------------------------------------------
void kusProject::setWindowSize( int w, int h ) {
    if ( winw_ != w || winh_ != h ) {
        winw_ = w;
        winh_ = h;
        audioDataRedraw();
        tracksSetDirty();
        updateTracksPos();
        
    }
}

//---------------------------------------------------------------------
void kusProject::setThumbSize( int w, int h ) {
    thumbw = w;
    thumbh = h;
    audioData.renderThumbnail( thumbw, thumbh );
}

//---------------------------------------------------------------------
void kusProject::createNew( bool rewrite ) {
    stop();
    save( false, false );
    if ( rewrite ) {
        setProjectFile( "" );
    }
    else {
        projectFile = "";
    }
    audioFile = "";
    audio.unloadSound();
    audioData.free();
    
    duration = 0;
    time0_ = time1_ = 0;
    audioData.renderEmptyThumbnail( thumbw, thumbh );
    
    tracks.clear();
    
    durationWasUpdated();
}

//---------------------------------------------------------------------
void kusProject::setProjectFile( string fileName ) {
    projectFile = fileName;
    shared_state.setProjectFile( projectFile );
    
}

//---------------------------------------------------------------------
string kusProject::name() {
    return ofFilePath::getFileName( projectFile );
}

//---------------------------------------------------------------------
string kusProject::name_without_extension() {  //имя файла проекта без расширения
    return ofFilePath::removeExt(name());
}

//---------------------------------------------------------------------
bool kusProject::save( bool saveAs, bool forceSave ) {
    //getting file name
    if ( saveAs || projectFile == "") {
        if ( !forceSave ) { return false; }
        
        ofFileDialogResult res = ofSystemSaveDialog( "project.kus", "Save Project to...");
        if ( res.bSuccess ) {
            setProjectFile( res.filePath );
        }
        else {
            return false;
        }
    }
    
    vector<string> file;
    file.push_back("KuStudio project file ver. 1.0");
    file.push_back( "audioFile=" + audioFile );
    file.push_back( "duration=" + ofToString( duration ) );
    file.push_back( "time0=" + ofToString( time0_ ) );
    file.push_back( "time1=" + ofToString( time1_ ) );
    
    for (int i=0; i<kus.OscOuts; i++) {
        file.push_back( "oscOut" + ofToString(i+1) + "=" + kus.oscOut[i].toString() );
    }
    for (int i=0; i<kus.PlayerOuts; i++) {
        file.push_back( "playerOut" + ofToString(i+1) + "=" + kus.playerOut[i].toString() );
    }
    file.push_back( "oscIn=" + kus.oscIn.toString() );
    
    file.push_back( "tracks=" + ofToString( tracks.size() ) );
    
    string s;
    for (int i=0; i<tracks.size(); i++) {
        tracks[i].toString( s, 0 );
        file.push_back( "track" + ofToString(i+1) + "=" + s );
    }
    
    file.push_back( "[Data]" );
    for (int i=0; i<tracks.size(); i++) {
        tracks[i].toString( s, 1 );
        file.push_back( s );
    }
    
    pbFiles::writeStrings( file, projectFile );
    return true;
}

//---------------------------------------------------------------------
void kusProject::shiftAllTracks_samples() { //сдвинуть все треки влево в сэмплах
    string sec = systemTextBoxDialog("Shift all tracks by time (samples)", "0" );
    if ( sec != "" ) {
        int v = ofToInt(sec);
        if ( v != 0 ) {
            for (int i=0; i<tracks.size(); i++) {
                tracks[i].shift_time_samples(v);
            }
            tracksSetDirty();
        }
    }
}

//---------------------------------------------------------------------
void kusProject::shiftAllTracks_sec() { //сдвинуть все треки влево
    string sec = systemTextBoxDialog("Shift all tracks by time (sec)", "0" );
    if ( sec != "" ) {
        float v = ofToFloat(sec);
        if ( v != 0 ) {
            for (int i=0; i<tracks.size(); i++) {
                tracks[i].shift_time_sec(v);
            }
            tracksSetDirty();
        }
    }
}

//---------------------------------------------------------------------
//экспортировать в текстовый файл, с разделением TAB
bool kusProject::exportRawText() {
    ofFileDialogResult res = ofSystemSaveDialog( "tracks.txt", "Export tract to text file...");
    if ( res.bSuccess ) {
        string fileName = res.filePath;
        
        vector<string> file;
        int m = tracks.size();
        if (m==0) return false; //нет треков
        string s;
        for (int i=0; i<m; i++) {
            if ( i > 0 ) s += "\t";
            s += tracks[i].name();
        }
        file.push_back(s);
        int n = tracks[0].getDataRef().size();
        for (int k=0; k<n; k++) {
            string s;
            for (int i=0; i<m; i++) {
                if ( i > 0 ) s += "\t";
                s += ofToString( tracks[i].getDataRef()[k] );
            }
            file.push_back(s);
        }
        
        pbFiles::writeStrings( file, fileName );
        return true;
    }
    
    return false;
}


//---------------------------------------------------------------------
bool kusProject::open( string fileName ) {
	if (fileName == "") return false;
	string extension = "kus";
    string ext1 = ofToLower(ofFilePath::getFileExt(fileName));
	if ( ext1 != extension) {
		ofSystemAlertDialog("Can't load project '"
                            + fileName
                            + "': this is not KuStudio project file, because its extension '"
                            + ext1 + "' is not '" + extension + "'");
		return false;
	}

    stop();
    save( false, false );

    fileName = ofToDataPath( fileName );
    if ( !pbFiles::fileExists( fileName ) ) {
        return false;
    }
    vector<string> file = pbFiles::readStrings( fileName );
    ParamMap ini;
    int k = 0;
    while ( k < file.size() && file[k] != "[Data]") {
        ini.setPair( file[k] );
        k++;
    }
    
    loadAudio( ini.getString("audioFile"), true, fileName );
    setViewSegment( ini.getFloat("time0", 0), ini.getFloat("time1", duration) );
    
    for (int i=0; i<kus.OscOuts; i++) {
        kus.oscOut[i].setupFromString(
                                      ini.getString("oscOut" + ofToString(i+1), kus.oscOut[i].toString() ) );
    }
    for (int i=0; i<kus.PlayerOuts; i++) {
        kus.playerOut[i].setupFromString(
                                      ini.getString("playerOut" + ofToString(i+1), kus.playerOut[i].toString() ) );
    }
    kus.oscIn.setupFromString( ini.getString( "oscIn", kus.oscIn.toString() ) );
    
    int n = ini.getInt("tracks");
    tracks.resize( n );
    for (int i=0; i<n; i++) {
        tracks[i].fromString( ini.getString("track" + ofToString(i+1)), 0 );
    }
    //после [Data]
    k++;
    for (int i=0; i<n && k < file.size(); i++) {
        tracks[i].fromString( file[k++], 1 );
    }
    
    afterProjectIsReady();
    
    setProjectFile( fileName );
    cout << "Opened project " << fileName << endl;
    return true;
}

//---------------------------------------------------------------------
void kusProject::afterProjectIsReady() {
    durationWasUpdated();
    updateTracksPos();
    setRecording( false );
    setEditing( false );
}

//---------------------------------------------------------------------
bool kusProject::openAs() {
    ofFileDialogResult res = ofSystemLoadDialog( "Open Project" );
    if ( res.bSuccess ) return open( res.filePath );
    else return false;
}

//---------------------------------------------------------------------
bool kusProject::loadAudio( string fileName, bool forceSelection, string projectFile ) {
    stop();
    if ( forceSelection && !pbFiles::fileExists( ofToDataPath( fileName ) ) ) {
        //если нет файла - сначала пытаемся найти в папке проекта, где projectFile
        bool exists = false;
        if ( projectFile != "" ) {
            string projectPath = ofFilePath::getEnclosingDirectory( projectFile );
            string newFileName = projectPath + ofFilePath::getFileName( fileName );
            if ( pbFiles::fileExists( ofToDataPath( newFileName ) ) ) {
                fileName = newFileName;
                exists = true;
            }
        }
        
        //если в текущей папке не найден - то выдаём диалог
        if ( !exists ) {
            ofFileDialogResult res = loadAudioDialog( fileName );
            if ( res.bSuccess ) {
                fileName = res.filePath;
            }
        }
    }
    
    audio.loadSound( fileName );
    audioData.loadWav( audio );
    audioFile = fileName;
    if ( audio.isLoaded() ) {
        duration = audioData.durationSec();
        time0_ = 0;
        time1_ = duration;
        audioData.renderThumbnail( thumbw, thumbh );
        durationWasUpdated();
        tracksSetDirty();
        audioDataRedraw();
    }
    else {
        duration = 0;
    }
    return audio.isLoaded();
}

//---------------------------------------------------------------------
void kusProject::durationWasUpdated() {
    for ( int i=0; i<tracks.size(); i++) {
        tracks[i].setDuration( duration );
    }
}

//---------------------------------------------------------------------
ofFileDialogResult kusProject::loadAudioDialog( string expectedFile ) {
    string title = "Select audio track";
    if ( expectedFile != "" ) {
        expectedFile = ofFilePath::getFileName( expectedFile );
        title += ", expected " + expectedFile;
    }
    
    return ofSystemLoadDialog( title );
}

//---------------------------------------------------------------------
bool kusProject::openAudioAs() {
    ofFileDialogResult res = loadAudioDialog();
    if ( res.bSuccess ) {
        loadAudio( res.filePath );
        save(false,false);
    }
    else {
        return false;
    }
	return true;
}

//---------------------------------------------------------------------
void kusProject::requestSendTracksValues() {
    for (int i=0; i<tracks.size(); i++) {
        tracks[i].requestSendValue();
    }
    requestSendValue_ = true;
}


//---------------------------------------------------------------------
void kusProject::playAll() {
    stop();
    playall_ = true;
    time_ = 0;
    audio.setPositionMS( time_ * 1000.0 );
    audio.play();
    
    requestSendTracksValues();
    
    //запоминаем положение
    time0Stored_ = time0_;
    time1Stored_ = time1_;
    setViewSegment( 0, duration );
}

//---------------------------------------------------------------------
void kusProject::playSegment() {
    stop();
    play_ = true;
    
    audio.play();
    audio.setPaused( true );
    time_ = time0_;
    audio.setPositionMS( time_ * 1000.0 );
    //audio.play();
    audio.setPaused( false );
    requestSendTracksValues();
}

//---------------------------------------------------------------------
void kusProject::stop() {
    audio.stop();
    if ( playall_ ) {
        //восстанавливаем сегмент
        setViewSegment( time0Stored_, time1Stored_ );
    }
    playall_ = false;
    play_ = false;
    time_ = time0_;
    
    //посылка плеерам,
    //они могут быть еще не инициализированы - поэтому до playerOut.size()
    for (int i=0; i<kus.playerOut.size(); i++) {
        kus.playerOut[i].sendStop();
    }
}

//---------------------------------------------------------------------
void kusProject::loopEnded() {
    stop();
}

//---------------------------------------------------------------------
void kusProject::update( float dt ) {
    float time = isPlaying() ? (audio.getPositionMS() * 0.001) : time0_;
    //time == 0 && time_ > 0 - значит трек доиграл до конца и автоматически перемотался на начало
    if ( playall_ && (time > duration || (time == 0 && time_ > 0) ) ) { loopEnded(); }
    if ( play_ && (time > time1_ || (time == 0 && time_ > 0) ) ) { loopEnded(); }
    
    if ( editing_ != shared_state.editButton ) { setEditing( shared_state.editButton ); }
    if ( editing_line_ != shared_state.editLineButton ) {
        setEditingLine( shared_state.editLineButton );
    }
    if ( recording_ != shared_state.recordButton ) { setRecording( shared_state.recordButton ); }
    
    if ( isPlaying() || requestSendValue_ ) {
        time_ = time;
        for (int i=0; i<tracks.size(); i++) {
            tracks[i].updateValue( time );
        }
        
        //посылка плеерам
        //они могут быть еще не инициализированы - поэтому до playerOut.size()
        for (int i=0; i<kus.playerOut.size(); i++) {
            kus.playerOut[i].sendTime( time );
        }
        
        //посылка времени   //TODO только одному получателю
        //for (int i=0; i<kus.oscOut.size(); i++) {
        {
            int i=0;
            kus.oscOut[i].sendFloat( "/master_time", time );
        }
        
    }
    else { time_ = time0_; }
    
    for (int i=0; i<tracks.size(); i++) {
        tracks[i].update( dt, isPlaying(), time_ );
    }
    requestSendValue_ = false;
    
    //посылка статуса
    last_sent_status_wait_ += dt;
    if ( last_sent_status_wait_ > 1 ) { //TODO параметр - время между отправками статуса
        last_sent_status_wait_ = 0;
        int i=0;
        string id = "kustudio";
        string status = name_without_extension()
        + " " + ofToString( time_, 1 )
        + " " + ofToString( ofGetElapsedTimef(), 1 );
        status += (isPlaying())?(isPlayingAll() ? " playAll" : " playSegm") : " stop";
        kus.oscOut[i].sendString( "/" + id + ":" + "status", status );
    }

}

//---------------------------------------------------------------------
float kusProject::timeToPix() {
    if ( time0_ < time1_ ) {
        return ofMap( time_, time0_, time1_, kus.leftX, winw_ );
    }
    else return 0;
}

//---------------------------------------------------------------------
void kusProject::updateTracksPos() { //обновить положение треков на экране
    float trackY0 = kTracksY;
    float indentY = 5;

    float y = trackY0;
    for (int i=0; i<tracks.size(); i++) {
        kusTrack &track = tracks[i];
        track.setPos( 12, y, winw_-12, track.h() );
        y += track.h() + indentY;
    }
    tracksSetDirty();
}

//---------------------------------------------------------------------
void kusProject::applyTrackButton( int ind, int button ) //на треке ind нажата кнопка button
{
    //cout << "track button " << ind << " " << button << endl;
    const int TRACK_ZOOMIN = 0;
    const int TRACK_ZOOMOUT = 1;
    const int TRACK_UP = 2;
    const int TRACK_DOWN = 3;
    
    kusTrack &track = tracks[ ind ];
    float h = track.h();
    if ( button == TRACK_ZOOMIN ) {
        if ( h <= 20 ) h = 40;
        else {
            h += 40;
        }
    }
    if ( button == TRACK_ZOOMOUT ) {
        h -= 40;
        if ( h < 20 ) { h = 20; }
    }
    if ( h != track.h() ) {
        track.setH( h );
        updateTracksPos();
    }
    
    if ( button == TRACK_UP ) {
        if ( ind > 0 ) {
            swap( tracks[ ind ], tracks[ ind-1 ] );
            updateTracksPos();
        }
    }
    if ( button == TRACK_DOWN ) {
        if ( ind + 1 < tracks.size() ) {
            swap( tracks[ ind ], tracks[ ind+1 ] );
            updateTracksPos();
        }
    }

}

//---------------------------------------------------------------------
void kusProject::draw() {
    if ( isEmpty() ) { return; }

    //треки
    ofPushMatrix();
    ofTranslate( 0, posY_ );
    //область видимости - чтобы не рисовать невидимые треки
    float screenY0 = kTracksY - posY_;
    float screenY1 = winh_ - posY_;
    
    for (int i=0; i<tracks.size(); i++) {
        tracks[i].draw( i == selectedTrack_, screenY0, screenY1 );
    }
    ofPopMatrix();
    
    //затираем верх
    ofSetColor( 0 );
    ofFill();
    ofRect( 0, 0, winw_, kTracksY );
    ofSetColor( 64 );
    ofLine( 0, kTracksY, winw_, kTracksY );

    
    ofSetColor( 255 );
    audioData.draw( kus.leftX, y0_);
    
    
    //текущее время полосой
    ofSetColor( 255 );
    float timeX = timeToPix();
    ofLine( timeX, y0_ - 8, timeX, winh_ );
}

//---------------------------------------------------------------------
void kusProject::drawThumb( float x, float y ) {
    audioData.drawThumbnail( x, y );
}

//---------------------------------------------------------------------
void kusProject::tracksSetDirty() {
    for (int i=0; i<tracks.size(); i++) {
        tracks[i].setDirty();
    }
}

//---------------------------------------------------------------------
void kusProject::audioDataRedraw() {
    audioData.updateDraw( kuWavFileDrawParam( winw_ - kus.leftX, audioH_, time0_, time1_ ) );
}

//---------------------------------------------------------------------
void kusProject::setViewSegment( float time0, float time1 ) {
    time0_ = time0;
    time1_ = time1;
    kus.time0 = time0;
    kus.time1 = time1;
    tracksSetDirty();
    audioDataRedraw();
}

//---------------------------------------------------------------------
string kusProject::timeString() {
    int seconds = int( time_ );
    float frac = int((time_ - seconds) * 10);
    int minutes = seconds / 60;
    seconds %= 60;
    
    return Converter::intToStr(minutes, 2)
    + ":" + Converter::intToStr(seconds, 2)
    + ":" + Converter::intToStr(frac);
}

//---------------------------------------------------------------------
void kusProject::addTrack() {
    if ( isEmpty() ) { return; }

    string res = systemTextBoxDialog( "Add track", "trackName");
    if ( res != "" ) {
        string name = res;
        int oscOut = 1;
        vector<string> list = ofSplitString( res, " " );
        if ( list.size() >= 2 ) {
            name = list[0];
            oscOut = ofToInt( list[1] );
            oscOut = ofClamp( oscOut, 1, kus.OscOuts );
        }
        kusTrack track;
        track.setup();
        track.setName( name );
        track.setOscOut( oscOut );
        track.setDuration( duration );
        tracks.push_back( track );
        
        //ставим трек после редактируемого
        if ( selectedTrack_ >= 0 ) {
            for (int i=tracks.size()-1; i>=selectedTrack_+2; i--) {
                swap( tracks[i], tracks[i-1] );
            }
        }
        
        updateTracksPos();
    }
}


//---------------------------------------------------------------------
bool kusProject::checkTrackSelected() {	//выбран ли какой-то трек, если нет - предупредить
	if (isEmpty()) {
		return false;
	}
	if (selectedTrack_ >= 0) {
		return true;
	}
	else {
		ofSystemAlertDialog("Please, select a track by clicking it.");
		return false;
	}
}

//---------------------------------------------------------------------
void kusProject::editTrackName() {
	if (checkTrackSelected()) {
        tracks[ selectedTrack_ ].setNameDialog();
    }
}

//---------------------------------------------------------------------
void kusProject::editTrackOscOut() {
	if (checkTrackSelected()) {
        tracks[ selectedTrack_ ].setOscOutDialog();
    }
}

//---------------------------------------------------------------------
void kusProject::editTrackRange() {
	if (checkTrackSelected()) {
        tracks[ selectedTrack_ ].setRangeDialog();
    }
}

//---------------------------------------------------------------------
void kusProject::editTrackSmooth() {
	if (checkTrackSelected()) {
        tracks[ selectedTrack_ ].setSmoothDialog();
    }
}

//---------------------------------------------------------------------
void kusProject::editTrackType() {
	if (checkTrackSelected()) {
        tracks[ selectedTrack_ ].setTypeDialog();
    }
}

//---------------------------------------------------------------------
void kusProject::deleteTrack() {
	if (checkTrackSelected()) {
        if ( systemTextBoxDialog("Delete '"
                                   + tracks[selectedTrack_].name()
                                   + "'? Type 'yes'") == "yes" ) {
            tracks.erase( tracks.begin() + selectedTrack_ );
            selectedTrack_ = -2;
            updateTracksPos();
        }
    }
}

//---------------------------------------------------------------------
void kusProject::editOscOut( int index ) { //редактировать OSC-выход
    kus.oscOut[ index ].setupFromDialog( "OSC out " + ofToString( index+1 ) + " (localhost 12345)" );
}

//---------------------------------------------------------------------
void kusProject::editPlayerOut( int index ) { //редактировать OSC-выход на kuPlayer
    kus.playerOut[ index ].setupFromDialog( "kuPlayer out " + ofToString( index+1 ) );
}

//---------------------------------------------------------------------
void kusProject::editOscIn() { //редактировать OSC-вход
    kus.oscIn.setupFromDialog( "Osc in port (12340, 0-disable)" );
}

//---------------------------------------------------------------------
bool kusProject::pointInAudio( int x, int y ) {
    return ( ofInRange( x, 0, winw_ ) && ofInRange( y, y0_, y0_ + audioH_) );
}

//---------------------------------------------------------------------
int kusProject::findIndex( int x, int y ) { //-2 - ничего, -1 - звуковой, 0..n - треки
    if ( pointInAudio( x, y ) ) return -1;
    for (int i=0; i<tracks.size(); i++) {
        if ( tracks[i].pointInRect( x, y ) ) return i;
    }
    return -2;
}

//---------------------------------------------------------------------
int kusProject::findIndexLeft( int x, int y ) { //-2 - ничего, -1 - звуковой, 0..n - треки
    for (int i=0; i<tracks.size(); i++) {
        if ( tracks[i].pointInRectLeft( x, y ) ) return i;
    }
    return -2;
}

//---------------------------------------------------------------------
void kusProject::deselectTrack() {   //сбросить пометку у выбранного трека
    if ( selectedTrack_ >= 0 ) {
        tracks[selectedTrack_].setSelected( false );
    }
    selectedTrack_ = -2;
}

//---------------------------------------------------------------------
void kusProject::setSelectedTrack( int ind ) {
    if ( ind != selectedTrack_ ) {
        deselectTrack();
        selectedTrack_ = ind;
    }
    if ( ind >= 0 ) {
        tracks[ind].setSelected( editing_ || editing_line_ );
    }
}

//---------------------------------------------------------------------
void kusProject::setPosY( float posY ) {
    posY_ = posY;
    posY_ = min( posY_, 0.0f );
    
}

//---------------------------------------------------------------------
//type 0 - move, 1 - press, 2 - drag, 3 - release
//0 - левая кнопка мыши, 2 - правая кнопка мыши
void kusProject::mouse( int x, int y, int button, void *&mouseState, int type ) {
    bool mouseCaptured = ( mouseState != 0 );
    bool captured = mouseCaptured && (mouseState == this);
    if ( mouseCaptured && !captured ) { return; }
    
    int y0 = y;
    y -= posY_;
    //cout << "proj mouse "<< type << " button " << button << " captured " << captured << endl;
    
    if ( type == 0 ) {  //move
        return;
    }
    if ( type == 1 ) { //press
        if ( y0 <= kTracksY ) return;
        
        //установка значений - белый столбик слева от трека
        int indLeft = findIndexLeft( x, y );
        if ( indLeft >= 0 ) {
            tracks[indLeft].setValue( tracks[indLeft].yToValue( y ) );
            mouseState = this;
            editTrack_ = indLeft;
            settingValue_ = true;
            editingValue_ = false;
            return;
        }
        
        int ind = findIndex( x, y );
        //если левой кнопкой - то сначала выбор трека, затем редактирование,
        //либо скролл
        bool canEdit = false;
        if ( ind >= 0 ) {
            //проверка, нажата ли кнопка на треке
            //TODO ждать пока отпустит - чтобы убедиться что именно ее нажал
            int trackButton = tracks[ ind ].trackButtonIndex( x, y );
            if ( trackButton >= 0 ) {
                applyTrackButton( ind, trackButton );
                return;
            }
            if ( button == 0 ) {
                canEdit = ( ind == selectedTrack_ )
                && (shared_state.editButton || shared_state.editLineButton);
            }
        }
        if ( !canEdit ) {
            //начинаем скроллинг
            scrolling_ = true;
            scrollPos_ = ofPoint( x, y0 );
            scrollDelta_ = ofPoint( 0, posY_ ) - scrollPos_;
            scrollTrack_ = ind;
            scrollDist_ = 0;
            mouseState = this;
        }
        //setSelectedTrack( ind );
        else {
            //начинаем редактирование
            tracks[ ind ].mouseDownEdit( x, y, isPlaying() );
            mouseState = this;
            editTrack_ = ind;
            settingValue_ = false;
            editingValue_ = true;
        }
        return;
    }
    if ( type == 2 ) { //drag
        if ( captured ) {
            if ( editTrack_ >= 0 ) {
                if ( settingValue_ ) {
                    tracks[editTrack_].setValue( tracks[editTrack_].yToValue( y ) );
                }
                //во время таскания ставим, если просто edit
                if ( editingValue_ && shared_state.editButton && !shared_state.editLineButton ) {
                    if ( tracks[ editTrack_ ].pointInRectX( x ) ) {
                        tracks[ editTrack_ ].mouseMoveEdit( x, y, isPlaying() );
                    }
                }
            }
            if ( scrolling_ ) {
                //вычисление пройденного пути
                ofPoint pos( x, y0 );
                scrollDist_ += (pos - scrollPos_).length();
                scrollPos_ = pos;
                
                setPosY( scrollDelta_.y + pos.y );
                //cout << scrollDist_ << endl;
            }
            
        }
        return;
    }
    if ( type == 3 ) { //release
        if ( captured ) {
            if ( scrolling_ ) {
                //если мало проехали - то это выбор трека
                if ( scrollDist_ <= kScrollDist ) {
                    setSelectedTrack( scrollTrack_ );
                }
            }
            if ( editingValue_ ) {
                //во время таскания ставим, если просто edit
                if ( shared_state.editLineButton) {
                    tracks[ editTrack_ ].mouseMoveEdit( x, y, isPlaying() );
                }
                //if ( tracks[ editTrack_ ].pointInRectX( x ) ) {
                tracks[ editTrack_ ].mouseUpEdit( x, y, isPlaying() );
                //}
            }
        
            mouseState = 0;
            editTrack_ = -2;
            editingValue_ = false;
            settingValue_ = false;
            
            scrolling_ = false;
            scrollTrack_ = -1;
        }
        
        return;
    }
    
}

//---------------------------------------------------------------------
//режим редактирования - вызывается автоматически в update
void kusProject::setEditing( bool editing ) {
    editing_ = editing;
    setSelectedTrack( selectedTrack_ );     //чтобы поставить туда эту переменную
}

//---------------------------------------------------------------------
void kusProject::setEditingLine( bool editingLine ) {    //режим редактирования линией
    editing_line_ = editingLine;
    setSelectedTrack( selectedTrack_ );      //чтобы поставить туда эту переменную
}

//---------------------------------------------------------------------
//режим записи - вызывается автоматически в update
void kusProject::setRecording( bool recording ) {
    recording_ = recording;
    for (int i=0; i<tracks.size(); i++) {
        tracks[i].setRecording( recording );
    }
}

//---------------------------------------------------------------------
int kusProject::findTrackByName( const string &name, int noValue ) {
    //смотрим - если это "kus1, kus2..." - то тогда возвращаем трек
    if ( name.substr(0, 3) == "kus" ) {
        int i = ofToInt( name.substr(3) ) - 1;
        if ( i >= 0 && i < tracks.size() ) {
            return i;
        }
    }
    for (int i=0; i<tracks.size(); i++) {
        if ( tracks[i].name() == name ) { return i; }
    }
    return noValue;
}

//---------------------------------------------------------------------
//установить значение в трек по имени
bool kusProject::setTrackValue( const string &name, float value ) {
    int i = findTrackByName( name );
    if ( i >= 0 ) {
        tracks[i].setValue( value );
    }
	return true;
}

//---------------------------------------------------------------------


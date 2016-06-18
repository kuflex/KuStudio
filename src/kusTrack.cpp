#include "kusTrack.h"
#include "kusState.h"

//---------------------------------------------------------------------
void kusTrack::setup() {
    name_ = "track";
    oscOut_ = 1;
    x_ = y_ = 0;
    w_ = 100;
    h_ = 80;
    
    duration_ = 0;
    n_ = 0;
    
    rate_ = 60;     //TODO параметр
    dirty_ = false;
    
    value_ = 0;
    smoothValue_ = 0;
    sentValue_ = 0;
    needSend_ = false;
        
    range0_ = 0;
    range1_ = 1;
    
    smooth_ = 0;
    type_ = TYPE_FLOAT;
    
    selected_ = false;
    recStage_ = 0;
}

//---------------------------------------------------------------------
void kusTrack::shift_time_sec( float sec ) {   //сдвиг влево на заданное число секунд
    shift_time_samples( sec * rate_ );
}

//---------------------------------------------------------------------
void kusTrack::shift_time_samples( int k ) {   //сдвиг влево на заданное число сэмплов
    if ( k == 0 ) return;
    if ( k < 0 ) {  //влево
        int k1 = -k;
        for (int i=0; i<n_; i++) {
            int j = min( i+k1, n_-1 );   //чтобы правые записались верно
            data_[i] = data_[j];
        }
    }
    else {
        for (int i=n_-1; i>=0; i--) { //вправо
            int j = max( i-k, 0 );   //чтобы левые записались верно
            data_[i] = data_[j];
        }
        
        
    }
}

//---------------------------------------------------------------------
void kusTrack::exportRawBinary( string fileName ) { //записать как массив float на диск
    if ( data_.size() > 0 ) {
        int n = data_.size();
        FILE *file = fopen( fileName.c_str(), "wb" );
        if ( file ) {
            fwrite( &data_[0], sizeof(data_[0]), n, file );
            fclose( file );
        }
    }
}

//---------------------------------------------------------------------
void kusTrack::setPos( float x, float y, float w, float h ) {
    x_ = x; y_ = y; w_ = w; h_ = h;
    setDirty();
}

//---------------------------------------------------------------------
void kusTrack::setH( float h ) {
    if ( h != h_ ) {
        h_ = h;
        setDirty();
    }
}

//---------------------------------------------------------------------
void kusTrack::setName( string name ) {
    name_ = name;
    names_ = ofSplitString( name_, "," );
    requestSendValue();
}

//---------------------------------------------------------------------
void kusTrack::setOscOut( int oscOut ) {
    oscOut = ofClamp( oscOut, 1, kus.OscOuts );
    oscOut_ = oscOut;
    requestSendValue();
}

//---------------------------------------------------------------------
void kusTrack::setRange( float range0, float range1 ) {
    range0_ = range0;
    range1_ = range1;
    requestSendValue();
}

//---------------------------------------------------------------------
void kusTrack::setDuration( float duration ) {
    if ( duration > duration_ ) {
        int n0 = n_;
        n_ = rate_ * duration;
        data_.resize( n_ );
        for (int i=n0; i<n_; i++) {
            data_[i] = TR_EMPTY;
        }
    }
    duration_ = duration;
    setDirty();
    requestSendValue();
}

//---------------------------------------------------------------------
void kusTrack::setSmooth( float smooth ) {
    smooth_ = ofClamp( smooth, 0, 1 );
}

//---------------------------------------------------------------------
void kusTrack::setType( int type ) {
    type_ = type;
}

//---------------------------------------------------------------------
int kusTrack::stringToType( string ts ) {
    int t = TYPE_FLOAT;
    if ( ts == "int" ) t = TYPE_INT;
    return t;
}

//---------------------------------------------------------------------
string kusTrack::typeToString( int t ) {
    string s = "float";
    if ( t == TYPE_INT ) s = "int";
    return s;
}

//---------------------------------------------------------------------
float kusTrack::convertType( float v ) {
    //TODO сейчас правильно работает только для положительных целых
    return ( type_ == TYPE_INT ) ? (int(v+0.5)) : v;
}

//---------------------------------------------------------------------
void kusTrack::setNameDialog() {
    string res = ofSystemTextBoxDialog( "' " + name() + "' - name", name_ );
    if ( res != "" ) { setName( res ); }
}

//---------------------------------------------------------------------
void kusTrack::setOscOutDialog() {
    string res = ofSystemTextBoxDialog( "'" + name() + "' - Osc out", ofToString( oscOut_ ) );
    if ( res != "" ) { setOscOut( ofToInt( res ) ); }
}

//---------------------------------------------------------------------
void kusTrack::setRangeDialog() {
    string res = ofSystemTextBoxDialog( "'" + name() + "' - Range",
                                       ofToString( range0_ ) + " " + ofToString( range1_ ) );
    if ( res != "" ) {
        vector<string> list = ofSplitString( res, " " );
        if ( list.size() >= 2 ) {
            setRange( ofToFloat( list[0]), ofToFloat( list[1] ) );
        }
    }
}

//---------------------------------------------------------------------
void kusTrack::setSmoothDialog() {
    string res = ofSystemTextBoxDialog( "'" + name() + "' - Smooth (0..1)",
                                       ofToString( smooth_ ) );
    if ( res != "" ) {
        setSmooth( ofToFloat( res ) );
    }
}

//---------------------------------------------------------------------
void kusTrack::setTypeDialog() {
    string res = ofSystemTextBoxDialog( "'" + name() + "' - Type (float, int)",
                                       typeToString( type_ ) );
    if ( res != "" ) {
        setType( stringToType( res ) );
    }
}


//---------------------------------------------------------------------
void kusTrack::toString( string &s, int stage ) {
    //name_, oscOut_, duration_, rate_, n_,
    //data_
    if( stage == 0 ) {
        s = name_ + " " + ofToString( oscOut_ ) + " " + ofToString( duration_ )
        + " " + ofToString( rate_ ) + " " + ofToString( n_ ) + " "
        + ofToString( range0_ ) + " " + ofToString( range1_ ) + " " + ofToString( smooth_ )
        + " " + ofToString( h_ ) + " " + typeToString( type_ );
    }
    if ( stage == 1 ) {
        s = "";
        for (int i=0; i<data_.size(); i++) {
            if ( i > 0 ) s += " ";
            s += ofToString( data_[i] );
        }
    }
}

//---------------------------------------------------------------------
void kusTrack::fromString( const string &s, int stage ) {
    if ( stage == 0 ) {
        setup();
        vector<string> list = ofSplitString( s, " " );
        if ( list.size() >= 7 ) {
            int k = 0;
            name_ = list[k++];
            setName( name_ );
            oscOut_ = ofToInt(list[k++]);
            duration_ = ofToFloat(list[k++]);
            rate_ = ofToFloat(list[k++]);
            n_ = ofToInt(list[k++]);
            range0_ = ofToFloat(list[k++]);
            range1_ = ofToFloat(list[k++]);
            if ( k < list.size() ) {
                smooth_ = ofToFloat(list[k++]);
            }
            if ( k < list.size() ) {
                h_ = ofToFloat(list[k++]);
            }
            if ( k < list.size() ) {
                type_ = stringToType(list[k++]);
            }
            
            data_.resize( n_ );
            fill( data_.begin(), data_.end(), TR_EMPTY );
        }
    }
    if ( stage == 1 ) {
        vector<string> list = ofSplitString( s, " " );
        int N = min( n_, int( list.size() ) );
        for (int i=0; i<N; i++) {
            data_[i] = ofToFloat( list[i] );
        }
    }
    requestSendValue();
}

//---------------------------------------------------------------------
void kusTrack::updateValue( float time ) { //выставить значение переменной по времени
    //если идет запись - то игнорировать
    if ( recStage_ >= 2 ) { return; }
    
    float indf = timeToIndexFloat( time );
    int ix0 = max( int( indf ), 0 );
    if ( ix0 >=0 && ix0 < n_ ) {
        int ix1 = min( ix0+1, n_-1 );
        if ( ix0 < ix1 ) {
            value_ = ofMap( indf, ix0, ix1, data_[ix0], data_[ix1] );
        }
        else value_ = data_[ix0];
    }
}

//---------------------------------------------------------------------
void kusTrack::requestSendValue() {    //просьба выслать значения
    //sentValue_ = -1;
    needSend_ = true;
}

//---------------------------------------------------------------------
void kusTrack::setValue( float val ) { //значение для отправки извне
    value_ = convertType( val );
    if ( recStage_ == 1 ) {  //если ждали значения, вот оно пришло
        recStage_ = 2;
    }
}

//---------------------------------------------------------------------
void kusTrack::update( float dt, bool playing, float time ) {
    //запись
    if ( playing ) {
        if ( recStage_ == 3 ) {
            recordingMoveEdit( time, value_ );
        }
        if ( recStage_ == 2 ) {
            recordingDownEdit( time, value_ );
            recStage_ = 3;
        }
    }
    else {
        if ( recStage_ >= 2 ) { recStage_ = 1; } //если остановилась игра - то ждем сначала
    }
    
    //сглаживание
    if ( smooth_ > 0 && type_ == TYPE_FLOAT ) {
        smoothValue_ += (value_ - smoothValue_) * (1-smooth_);
    }
    else {
        smoothValue_ = value_;
    }
    
    //отправка
    if ( needSend_ || fabs(smoothValue_ - sentValue_) > 0.0001 ) { //TODO параметр
        sendValue( ofLerp( range0_, range1_, smoothValue_ ) );
    }
}

//---------------------------------------------------------------------
void kusTrack::sendValue( float v ) {
    //посылаем на несколько адресов
    for (int i=0; i<names_.size(); i++) {
        kus.oscOut[ oscOut_ - 1 ].sendFloat( "/" + names_[i], v );
    }
    sentValue_ = smoothValue_;
    needSend_ = false;
}

//---------------------------------------------------------------------
void kusTrack::draw( bool selected, float screenY0, float screenY1 ) {
    //проверка видимости трека
    if ( max(y_, screenY0) <= min( y_ + fbo_.getHeight(), screenY1 ) ) {
        render();

        ofSetColor( 255 );
        fbo_.draw( x_, y_ );
    
        if ( selected ) ofSetColor( 255, 255, 0 );
        else ofSetColor( 255 );
        kus.trackFont1.drawString( name_, x_ + 10, y_ + kus.trackFont1.getLineHeight() );

        //рисуем текущее значение
        //ofSetColor( 0 );
        ofFill();
        //ofRect( 0, y_+1, 4, h_-2 );
        ofSetColor( 255 );
        ofLine( 3, y_+h_-1, 3, valueToY( value_ ) );
        ofLine( 4, y_+h_-1, 4, valueToY( value_ ) );
    }
    
}

//---------------------------------------------------------------------
bool kusTrack::pointInRect( int x, int y ) {
    return ofInRange( x, x_, x_ + w_ )
    && ofInRange( y, y_, y_ + h_ );
}

//---------------------------------------------------------------------
bool kusTrack::pointInRectX( int x ) {
    return ofInRange( x, x_, x_ + w_ );
}

//---------------------------------------------------------------------
bool kusTrack::pointInRectLeft( int x, int y ) {
    return ofInRange( x, x_-kus.leftX, x_-1 )   //kus.leftX - параметр левой границы
    && ofInRange( y, y_, y_ + h_ );
}

//---------------------------------------------------------------------
float kusTrack::timeToIndexFloat( float time ) {
    return time * rate_;
}

//---------------------------------------------------------------------
float kusTrack::xToIndexFloat( float x ) {
    if ( kus.time0 < kus.time1 ) {
        return ofMap( x, x_, x_ + w_, kus.time0 * rate_, kus.time1 * rate_ );
    }
    else {
        return kus.time0 * rate_;
    }
}

//---------------------------------------------------------------------
float kusTrack::yToValue( float y ) {
    return ofMap( y, y_+h_-1, y_+2, 0, 1, true );
}

//---------------------------------------------------------------------
float kusTrack::valueToY( float val ) {
    return valueToY_rel( val ) + y_;
}

//---------------------------------------------------------------------
float kusTrack::valueToY_rel( float val ) {
    return ofMap( val, 0, 1, h_-1, 2, true );
}

//---------------------------------------------------------------------
//для записи - будут записываться значения value_
void kusTrack::setRecording( bool rec ) {
    if ( rec ) { recStage_ = 1; }
    else { recStage_ = 0; }
}

//---------------------------------------------------------------------
//для записи
void kusTrack::recordingDownEdit( float time, float value ) {
    float indf = timeToIndexFloat( time );
    int ix = int( indf );
    if ( ix >= 0 && ix < data_.size() ) {
        data_[ix] = value;
    }
    recLastInd = indf;
    recLastVal = value;
    
    setDirty();
    
    //value_ = recLastVal;
}

//---------------------------------------------------------------------
void kusTrack::recordingMoveEdit( float time, float value ) {
    float indf0 = recLastInd;
    int ix0 = int( recLastInd );
    float val0 = recLastVal;
    
    float indf1 = timeToIndexFloat( time );
    int ix1 = int( indf1 );
    float val1 = value;
    
    int dir = ( ix1 >= ix0 ) ? 1 : -1;
    int steps = abs( ix1-ix0 ) + 1;
    int ix_ = ix0;
    for (int k = 0; k < steps; k++ ) {
        if ( ix_ >= 0 && ix_ < data_.size() ) {
            if ( indf0 != indf1 ) {
                data_[ix_] = ofMap( ix_, indf0, indf1, val0, val1, true );
            }
            else {
                data_[ix_] = val1;
            }
        }
        ix_ += dir;
    }
    recLastInd = indf1;
    recLastVal = val1;
    
    setDirty();
    //value_ = value;
}

//---------------------------------------------------------------------
void kusTrack::mouseDownEdit( int x, int y, bool playing ) {
    //cout << "mouse down " << xToIndexFloat( x ) << " " << yToValue( y ) << endl;
    float indf = xToIndexFloat( x );
    int ix = int( indf );
    float val = yToValue( y );
    if ( ix >= 0 && ix < data_.size() ) {
        data_[ix] = convertType( val );
    }
    lastInd = indf;
    lastVal = val;
    
    setDirty();
    
    if ( !playing ) {
        value_ = lastVal;
    }
}

//---------------------------------------------------------------------
void kusTrack::mouseUpEdit( int x, int y, bool playing ) {
    
}

//---------------------------------------------------------------------
void kusTrack::mouseMoveEdit( int x, int y, bool playing ) {
    float indf0 = lastInd;
    int ix0 = int( lastInd );
    float val0 = lastVal;
    
    float indf1 = xToIndexFloat( x );
    int ix1 = int( indf1 );
    float val1 = yToValue( y );

    int dir = ( ix1 >= ix0 ) ? 1 : -1;
    int steps = abs( ix1-ix0 ) + 1;
    int ix_ = ix0;
    for (int k = 0; k < steps; k++ ) {
        if ( ix_ >= 0 && ix_ < data_.size() ) {
            if ( indf0 != indf1 ) {
                data_[ix_] = convertType( ofMap( ix_, indf0, indf1, val0, val1, true ) );
            }
            else {
                data_[ix_] = convertType( val1 );
            }
        }
        ix_ += dir;
    }
    lastInd = indf1;
    lastVal = val1;
    
    setDirty();
    if ( !playing ) {
        value_ = lastVal;
    }
}

//---------------------------------------------------------------------
void kusTrack::setDirty() {
    dirty_ = true;
}

//---------------------------------------------------------------------
void kusTrack::setSelected( bool selected ) {
    if ( selected != selected_ ) {
        selected_ = selected;
        setDirty();
    }
}

//---------------------------------------------------------------------
void kusTrack::render() {  //обновить картинку трека
    if ( !dirty_ ) return;
    dirty_ = false;
    if ( fbo_.getWidth() != w_ || fbo_.getHeight() != h_ ) {
        fbo_.allocate( w_, h_ );
    }
    fbo_.begin();
    //ofClear( 0 );
    ofSetColor( 40 );
    ofFill();
    ofRect( 1, 1, w_-1, h_-1 );
    
    ofNoFill();
    ofSetColor( 128 );
    ofRect( 1, 1, w_-1, h_-1 );
    
    //ofSetColor( 255, 0, 255 );
    if ( !selected_ ) { ofSetColor( 0, 255, 255 ); }
    else { ofSetColor( 255, 255, 0 ); }

    float lastv0 = 0;
    float lastv1 = 0;
    for (int x=0; x<w_; x++) {
        int ix0 = int(xToIndexFloat( x_ + x ));
        int ix1 = int(xToIndexFloat( x_ + x + 1 ));
        if ( ix0 >= 0 && ix1 < data_.size() ) {
            float v0 = data_[ix0];
            float v1 = v0;
            for (int i=ix0+1; i<ix1; i++) {
                v0 = min( v0, data_[i] );
                v1 = max( v1, data_[i] );
            }
            v0 = valueToY_rel(v0);
            v1 = valueToY_rel(v1)+1;
            ofLine( x, v0, x, v1 );   //TODO ускорить
            if ( x > 0 && v1 < lastv0 ) ofLine( x-1, lastv0, x, v1 );
            if ( x > 0 && v0 > lastv1 ) ofLine( x-1, lastv1, x, v0 );
            lastv0 = v0;
            lastv1 = v1;
        }
    }
    
    //ofSetColor( 255 );
    //kus.trackFont1.drawString( name_, 5, kus.trackFont1.getLineHeight() );
    
    //кнопки управления треком
    ofRectangle tr = trackButtonRect();
    ofSetColor( 255 );
    kus.trackIcons.draw( tr.x, tr.y );
    fbo_.end();
}

//---------------------------------------------------------------------
ofRectangle kusTrack::trackButtonRect() {
    return ofRectangle( w_ - kus.trackIcons.getWidth() - 10, 3,
                       kus.trackIcons.getWidth(), kus.trackIcons.getHeight() );
}

//---------------------------------------------------------------------
int kusTrack::trackButtonIndex( int x, int y )  //возвращает нажатую кнопку трека, или -1
{
    x -= x_;
    y -= y_;
    ofRectangle tr = trackButtonRect();
    if ( tr.inside( x, y ) ) {
        int i = ofMap( x, tr.x, tr.x + tr.width, 0, kus.trackIconN );
        i = ofClamp( i, 0, kus.trackIconN-1 );
        return i;
    }
    return -1;
}

//---------------------------------------------------------------------



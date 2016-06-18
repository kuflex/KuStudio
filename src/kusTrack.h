#pragma once
//Трек

#include "ofMain.h"

static const float TR_EMPTY = 0;  //значение, показывающее что нет значения

class kusTrack
{
public:
	void setup();
    void exportRawBinary( string fileName ); //записать как массив float на диск
    void shift_time_samples( int samples );   //сдвиг влево на заданное число сэмплов
    void shift_time_sec( float sec );   //сдвиг влево на заданное число секунд
    
    void setPos( float x, float y, float w, float h );
    void setH( float h );
    
    void setName( string name );
    void setOscOut( int oscOut );
    void setRange( float range0, float range1 );
    void setDuration( float duration );
    void setSmooth( float smooth );
    
    void setType( int type );
    static const int TYPE_FLOAT = 0;
    static const int TYPE_INT = 1;
    int stringToType( string ts );
    string typeToString( int t );
    float convertType( float v );   //если int - то делает целым
    
    string name() { return name_; }
    
    void setNameDialog();
    void setOscOutDialog();
    void setRangeDialog();
    void setSmoothDialog();
    void setTypeDialog();

	void toString( string &s, int stage );
    void fromString( const string &s, int stage );

    float value() { return value_; }
    void updateValue( float time ); //выставить значение переменной по времени, вызывать до update
    void requestSendValue();    //просьба выслать значения
    void setValue( float val ); //значение для отправки извне
    
    void update( float dt, bool playing, float time );    //высылает значение переменной
    void draw( bool selected, float screenY0, float screenY1 );
    //screenY0,Y1 - область видимости на экране, если трек вне её - не рисовать
    
    bool pointInRect( int x, int y ); //основной прямоугольник трека
    bool pointInRectX( int x );
    bool pointInRectLeft( int x, int y ); //левая область трека - для установки значений

    ofRectangle trackButtonRect(); //прямоугольник для кнопок
    int trackButtonIndex( int x, int y );  //возвращает нажатую кнопку трека, или -1

    //для записи - будут записываться значения value_
    void setRecording( bool rec );
    
    //для редактирования мышью
    void mouseDownEdit( int x, int y, bool playing );
    void mouseMoveEdit( int x, int y, bool playing );
    void mouseUpEdit( int x, int y, bool playing );

    void setDirty();    //для перерисовки

    void setSelected( bool selected );
    bool isSelected() { return selected_; }
    
    float x() { return x_; }
    float y() { return y_; }
    float w() { return w_; }
    float h() { return h_; }

    float timeToIndexFloat( float time );
    float xToIndexFloat( float x );
    float yToValue( float y );
    float valueToY( float val );
    float valueToY_rel( float val );

    vector<float> &getDataRef() { return data_; }
private:
    string name_;
    vector<string> names_;  //все адреса (полученные из name разбиение по запятым)
                            //например: eq1/e,eq2/e
    int oscOut_;    //число - номер выхода OSC, нумеруются с 1
    float x_, y_, w_, h_;

    float duration_;
    float rate_;
    
    float range0_, range1_;
    float smooth_;
    int type_;   //0 - float, 1 - int
    
    bool selected_;          //трек выделен для редактирования - его рисовать другим цветом
    
    int n_;
    vector<float> data_;
    
    float lastInd, lastVal; //для мыши
  
    int recStage_;  //0 -  не запись, 1 - старт записи и ждем первого значения,
    //2 - дождались значения, 3 - продолжаем
    float recLastInd, recLastVal; //для записи
    void recordingDownEdit( float time, float value );
    void recordingMoveEdit( float time, float value );

    
    void render();  //обновить картинку трека
    ofFbo fbo_;
    bool dirty_;    //надо ли обновить
    
    float value_;       //последнее установленное значение
    float smoothValue_; //сглаженное значение
    float sentValue_;   //последнее отправленное значение
    bool needSend_;     //нужно ли обязательно выслать
        
    void sendValue( float v );
};


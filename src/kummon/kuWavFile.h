#pragma once
//Рисование звукового файла

#include "ofMain.h"


struct kuWavFileDrawParam {
    int w, h;
    float time0, time1;
    kuWavFileDrawParam();
    kuWavFileDrawParam( int w, int h, float time0, float time1 );
    void free();
    bool isEqual( const kuWavFileDrawParam &param );
    bool isEmpty();
    
};

class kuWavFile
{
public:
    kuWavFile();
    bool loadWav( ofSoundPlayer &sound );
    void free();
    void printInfo();
    float durationSec();
    bool isEmpty();

    void updateDraw( const kuWavFileDrawParam &param );
    void draw( float x, float y );
	
    void renderThumbnail( int w, int h );
    void renderEmptyThumbnail( int w, int h );
    void drawThumbnail( float x, float y );
    
private:
    
    float vrate_; //разрешение (отсчетов в секунду) для визуализации
    vector<float> data_; //данные для рисования, от 0 до 1
    float duration_;
    
    
    int timeToIndex( float time );

    kuWavFileDrawParam param_;
    ofFbo fbo_;
    
    void render( const kuWavFileDrawParam &param, const ofColor &background );
    vector<float> renderVals_;   //временный массив для вычисления значений визуализации

    
    ofFbo thumb_;
};


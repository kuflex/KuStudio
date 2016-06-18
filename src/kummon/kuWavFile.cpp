#include "kuWavFile.h"
#include "pbFiles.h"


//---------------------------------------------------------------------
kuWavFileDrawParam::kuWavFileDrawParam() {
    free();
}

//---------------------------------------------------------------------
bool kuWavFileDrawParam::isEmpty() {
    return (time0 == time1) && ( time0 == -1 );
}


//---------------------------------------------------------------------
void kuWavFileDrawParam::free() {
    time0 = time1 = -1;
}

//---------------------------------------------------------------------
kuWavFileDrawParam::kuWavFileDrawParam( int _w, int _h, float _time0, float _time1 ) {
    w = _w;
    h = _h;
    time0 = _time0;
    time1 = _time1;
    
}

//---------------------------------------------------------------------
bool kuWavFileDrawParam::isEqual( const kuWavFileDrawParam &param ) {
    return
    w == param.w
    && h == param.h
    && time0 == param.time0
    && time1 == param.time1;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
kuWavFile::kuWavFile() {
    free();
}

//---------------------------------------------------------------------
bool kuWavFile::loadWav( ofSoundPlayer &sound ) {

    vrate_ = 120; //60.0;
    //считывание данных
    ofPtr<ofBaseSoundPlayer> player  = sound.getPlayer();
    
	ofFmodSoundPlayer *fmod = (ofFmodSoundPlayer *)player.get();
    
	int length = fmod->length;
	//FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_PCM);
    
	void *ptr1, *ptr2;
	unsigned int len1=0, len2=0;
	
	//https://www.fmod.org/docs/content/generated/FMOD_Sound_Lock.html
	FMOD_RESULT result;
	result = FMOD_Sound_Lock(fmod->sound, 0, length, &ptr1, &ptr2, &len1, &len2);
	if ( result == FMOD_OK ) {
		FMOD_SOUND_TYPE type;
		FMOD_SOUND_FORMAT format;
		int channels;
		int bits;
		result = FMOD_Sound_GetFormat(fmod->sound, &type, &format, &channels, &bits);
        
		unsigned int length_ms;
		result = FMOD_Sound_GetLength(fmod->sound, &length_ms, FMOD_TIMEUNIT_MS);
		duration_ = length_ms/1000.0;
		
		//заполнение массива
        int n = duration_ * vrate_;
		long long int N = ( (long long int) length) *channels;
        //cout << "n " << n << endl;
        data_.resize( n );
        fill( data_.begin(), data_.end(), 0 );
        
        for (int i=0; i<N; i++) {	//пробегаем все каналы - так как проецируем в моно
			float v = 0;
			//выбор типа данных
			switch ( format ) {
                case FMOD_SOUND_FORMAT_PCM8:	v = ((char *)ptr1)[i]; break;           /* 8bit integer PCM data. */
                case FMOD_SOUND_FORMAT_PCM16:	v = ((short *)ptr1)[i]; break;         /* 16bit integer PCM data.  */
                    //case FMOD_SOUND_FORMAT_PCM24: v = ((??? *)ptr1)[i];  break;        /* 24bit integer PCM data.  */
                case FMOD_SOUND_FORMAT_PCM32:	v = ((int *)ptr1)[i]; break;          /* 32bit integer PCM data.  */
                case FMOD_SOUND_FORMAT_PCMFLOAT: v = ((float *)ptr1)[i]; break;        /* 32bit floating point PCM data.  */
                default: v=0;
			}
			v = fabs(v);
            
            int j = (long long int)(i) * n / N;
            data_[j] = max( data_[j], v );
        }
		//нормализация громкости
		float maxVal = 0;
		for (int i=0; i<n; i++) {
			maxVal = max( maxVal, data_[i] );
		}
		if ( maxVal > 0 ) {
			for (int i=0; i<n; i++) {
				data_[i] /= maxVal;
			}
		}
        
		result = FMOD_Sound_Unlock(fmod->sound, &ptr1, &ptr2, len1, len2);
	}

    
    
    return true;
}

//---------------------------------------------------------------------
void kuWavFile::free() {
    duration_ = 0;
    data_.clear();
    param_.free();
}

//---------------------------------------------------------------------
bool kuWavFile::isEmpty() {
    return data_.size() == 0;
}

//---------------------------------------------------------------------
float kuWavFile::durationSec() {
    return duration_;
}

//---------------------------------------------------------------------
int kuWavFile::timeToIndex( float time ) {
    if ( !isEmpty() ) {
        return time * vrate_;
        //header.subchunk2Size / (header.bitsPerSample / 8) / header.sampleRate;
    }
    else return -1;
}

//---------------------------------------------------------------------
void kuWavFile::printInfo() {
    // Выводим полученные данные
    /*cout << "Sample rate: " << header.sampleRate << endl;
    cout << "Channels: " << header.numChannels << endl;
    cout << "Bits per sample: " << header.bitsPerSample << endl;
    
    // Посчитаем длительность воспроизведения в секундах
    float fDurationSeconds = durationSec();
    
    int iDurationMinutes = (int)floor(fDurationSeconds) / 60;
    fDurationSeconds = fDurationSeconds - (iDurationMinutes * 60);
//    printf_s("Duration: %02d:%02.f\n", iDurationMinutes, fDurationSeconds);
 
    cout << "Duration: " << iDurationMinutes << ":" << fDurationSeconds << endl;
*/
}

//---------------------------------------------------------------------
//рисует
void kuWavFile::render( const kuWavFileDrawParam &param, const ofColor &background ) {
    int w = param.w;
    int h = param.h;
    
    
    
    //вычисляем значения
    renderVals_.resize( w );
    fill( renderVals_.begin(), renderVals_.end(), 0 );
    
    if ( !data_.empty() ) {
        int j0 = 0;
        for (int i=0; i<w; i++) {
            int j1 = timeToIndex( ofLerp( param.time0, param.time1, 1.0*(i+1)/w ) );
            int j1_use = min( j1+1, int(data_.size()) );
            float &v = renderVals_[ i ];
        
            //уменьшение числа шагов для ускорения
            int steps = j1_use - j0;
            int step = ( steps <= 10 ) ? steps : steps / (steps/10);
        
            for (int j=j0; j<j1_use; j += step) {
                v = max( v, data_[j] );
            }
            j0 = j1;
        }
    }
    
    ofSetColor( background );
    ofFill();
    ofRect( 0, 0, w, h );
    
    ofSetColor( 255 );
    float scaleY = h/2;
    float Y0 = h/2;
    for (int i=0; i<w; i++) {
        int q = renderVals_[i] * scaleY;
        ofLine( i, Y0 - q, i, Y0 + q + 1 ); //TODO ускорить
        
        //cout << fboVals_[i] << " ";
    }
    //cout << endl;
}

//---------------------------------------------------------------------
void kuWavFile::updateDraw( const kuWavFileDrawParam &param ) {
    if ( !param_.isEqual( param ) ) {
        param_ = param;
        if ( fbo_.getWidth() != param.w || fbo_.getHeight() != param.h ) {
            fbo_.allocate( param.w, param.h );
        }
        fbo_.begin();
        render( param, ofColor( 100 ) );

        fbo_.end();
    }
}

//---------------------------------------------------------------------
void kuWavFile::draw( float x, float y ) {
    if ( !param_.isEmpty() && !isEmpty() > 0 ) {
        fbo_.draw( x, y );
    }
}

//---------------------------------------------------------------------
void kuWavFile::renderEmptyThumbnail( int w, int h ) {
    thumb_.allocate( w, h );
    thumb_.begin();
    ofFill();
    ofSetColor( 0 );
    ofRect( 0, 0, w, h );
    thumb_.end();
}

//---------------------------------------------------------------------
void kuWavFile::renderThumbnail( int w, int h ) {
    thumb_.allocate( w, h );
    thumb_.begin();
    render( kuWavFileDrawParam( w, h, 0, durationSec() ), ofColor( 64 ) );
    thumb_.end();
}

//---------------------------------------------------------------------
void kuWavFile::drawThumbnail( float x, float y ) {
    thumb_.draw( x, y );
}

//---------------------------------------------------------------------
/*vector<float> kuWavFile::getSamplesFloat() { //создает массив float-сэмплов
    int n = samples.size();
    vector<float> sampl(n);
    for (int i=0; i<n; i++) {
        sampl[i] = float(samples[i]) / 16384.0f;
    }
    return sampl;
}*/

//---------------------------------------------------------------------

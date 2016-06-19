#include "pbNetwork.h"

const string PacketMarker = "[Data packet]";
const int PacketMarkerSize = 13;
const int PacketHeaderSize = PacketMarkerSize + 3 * 4;	//frame, size, src32

struct PacketHeader {
	bool valid;
	int frame, size, src32;

	char buffer[ PacketHeaderSize ];
	int bufferSize;
	void setup( int frame0, int size0, int src320, bool fillBuffer )
	{
		valid = true;
		frame = frame0;
		size = size0;
		src32 = src320;
		if ( fillBuffer ) {
			//заполняем буфер
			int k=0;
			memcpy( buffer + k, &PacketMarker[0], PacketMarkerSize );
			k += PacketMarkerSize;
			memcpy( buffer + k, &frame, 4 );
			k+=4;
			memcpy( buffer + k, &size, 4 );
			k+=4;
			memcpy( buffer + k, &src32, 4 );
			k+=4;

			bufferSize = PacketHeaderSize;
		}

	}

	//поиск заголовка и его парсинг
	int findHeader( char *data, int len ) {
		for (int k=0; k<len - PacketHeaderSize; k++) {
			if ( data[k] == PacketMarker[0] ) {
				bool ok = true;
				for (int i=1; i<PacketMarkerSize; i++) {
					if ( data[k+i] != PacketMarker[i] ) {
						ok = false;
						break;
					}
				}
				if ( ok ) {	ok = parse( data + k ); }
				if ( ok ) {
					return k;
				}
			}
		}
		return -1;
	}

	bool parse( char *data )	//предполагается, что данных хватает для парсинга
	{
		int k=0;
		memcpy( buffer + k, &data + k, PacketMarkerSize );
		k += PacketMarkerSize;
		memcpy( &frame, data + k, 4 );
		k+=4;
		memcpy( &size, data + k, 4 );
		k+=4;
		memcpy( &src32, data + k, 4 );
		k+=4;
		return ( frame >= 0 && size >= 0 && size < 10000000 && src32 >= 0 );	//ПАРАМЕТР - ограничение максимальных данных
	}
};

//-------------------------------------------------------------------
void pbNetworkSenderSync::setup( const string &addr, int port, int packetSize )
{
	_addr = addr;
	_port = port;
	_packetSize = packetSize;

	tcpClient.TCPClientRef().SetTimeoutSend( 1 );	//ПАРАМЕТР ожидания отправки
	tcpClient.TCPClientRef().SetTimeoutAccept( 1 );
	reconnect();
}

//-------------------------------------------------------------------
void pbNetworkSenderSync::close()
{
	if ( connected() ) {
		tcpClient.close();
	}
}

//-------------------------------------------------------------------
void pbNetworkSenderSync::reconnect()
{
	cout << "Reconnect sender " << _addr << ", port " << _port << endl;
	bool blocking = true;
	_connected = tcpClient.setup( _addr, _port, blocking);		//"127.0.0.1", 11999
	_connectTime = ofGetElapsedTimef();
	if ( _connected ) { cout << " ok" << endl; }
	else { cout << " failed" << endl; }
}

//-------------------------------------------------------------------
void pbNetworkSenderSync::update()
{
	if ( !connected() ) {//переподключаемся	
		float deltaTime = ofGetElapsedTimef() - _connectTime;
		if( deltaTime > 1.0f ){			//ПАРАМЕТР
			reconnect();
		} 
	}
}

//-------------------------------------------------------------------
bool pbNetworkSenderSync::send( unsigned char *data, int size, int frameNumber )
{
	update();	//на всякий случай
	bool res = connected();
	//Заголовок
	if ( res ) {
		PacketHeader header;
		header.setup( frameNumber, size, 0, true );
		res = tcpClient.sendRawBytes( header.buffer, header.bufferSize );
	}
	//Данные
	int N = 0;
	if ( res ) {
		while ( N < size && res ) {
			int n = size - N;
			n = min( n, _packetSize );
			res = tcpClient.sendRawBytes( (char *)data + N, n );
			N += n;
		}
	}

	if ( !res && connected () ) {		//если не удалось отпраавить - надо переподключаться
		cout << "Data send error..." << endl;
		tcpClient.close();
		_connected = false;
	}

	//memcpy( _buffer, &pos, 4 );
	//memcpy( _buffer + 4, _data + pos, _packetSize );
	//bool res = tcpClient.sendRawBytes( (const char*)_buffer, _packetSizeFull );
	return res;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
pbNetworkReceiver::pbNetworkReceiver() {
    _threaded = false;
}

//-------------------------------------------------------------------

pbNetworkReceiver::~pbNetworkReceiver() {
    close();
}

//-------------------------------------------------------------------

void pbNetworkReceiver::setup( int port, int packetSize, bool threaded )		//размер изображения в байтах и размер пакета
{
	_threaded = threaded;
	_wantRestart = false;

	_port = port;
	_packetSize = packetSize;

	_frame = -1;
	_size = 0;

	maxN = 1000000;		//максимальный размер передаваемых данных
	_buffer.resize( maxN );
	_N = 0;

	_data.resize( maxN );

	startTCP();

	if ( _threaded ) {
		startThread( true, false );   // blocking, verbose
	}
}

//-------------------------------------------------------------------
void pbNetworkReceiver::startTCP()
{
	cout << "pbNetworkReceiver - start receiver, port " << _port << endl;
	bool blocking = true;
	TCP.setup(_port, blocking);
	TCP.TCPServerRef().SetTimeoutReceive( 1 );	//ПАРАМЕТР ожидания приема
}

//-------------------------------------------------------------------
void pbNetworkReceiver::close()
{
	//ВОЗМОЖНО, ПАДАЕТ!
	if ( _threaded ) {
		stopThread();
	}
	TCP.close();
}

//-------------------------------------------------------------------
void pbNetworkReceiver::restart()	//перезапуск получателя
{
	_wantRestart = true;
}

//-------------------------------------------------------------------
void pbNetworkReceiver::threadedFunction()
{

	//устанавливаем приоритет потока
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);			
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);			

	while( isThreadRunning() != 0 ){
		//принимаем
		receive0();

		//делаем перезапуск - просто отключаем всех клиентов
		if ( _wantRestart ) {
			_wantRestart = false;
			for(int k = 0; k < TCP.getNumClients(); k++){
				if ( TCP.isClientConnected(k) ) { 
					disconnectClient( k );
				}
			}
		}
	}

}


//-------------------------------------------------------------------
void pbNetworkReceiver::receive()
{
	if ( !_threaded ) {
		receive0();
	}
}

//-------------------------------------------------------------------
//укорачивание буфера до длины newLen
void shiftBuffer( char *buffer, int &len, int newLen )
{
	if ( newLen < len ) {
		int start = len - newLen;
		for (int i=0; i<newLen; i++) {
			buffer[ i ] = buffer[ i + start ];
		}
		len = newLen;
	}
}

//-------------------------------------------------------------------
void pbNetworkReceiver::receive0()
{
	char *buffer = &_buffer[0];
	for(int k = 0; k < TCP.getNumClients(); k++){
		if ( !TCP.isClientConnected(k) ) { 
			continue;
		}

		int free = maxN - _N;
		if ( free > 0 ) {
			free = min( free, _packetSize );
			int rec = TCP.receiveRawBytes( k, (char *)(buffer + _N), free ); 
			if ( rec >= 0 ) {
				_N += rec;		
				//cout << "rec " << rec << endl;
				//ищем заголовок
				PacketHeader header;
				int headerPos = header.findHeader( (char *)buffer, _N );
				if ( headerPos >= 0 ) {

					//cout << "reading frame " << header.frame << endl;
					//нашли заголовок, сейчас надо данные скачать

					int headerEnd = headerPos + PacketHeaderSize;
					int N = _N - headerEnd;

					//сдвигаем буфер до начала данных
					shiftBuffer( buffer, _N, N );

					int size = header.size;

					float lastTime = ofGetElapsedTimef();	//время последнего успешного приема
					while ( _N < size ) {
						rec = TCP.receiveRawBytes( k, (char *)(buffer + _N), free ); 
						if ( rec > 0 ) {
							_N += rec;
							lastTime = ofGetElapsedTimef();
						}
						else {
							float time = ofGetElapsedTimef();
							if ( time >= lastTime + 1.0 ) {		//ПАРАМЕТР обрыв связи
								cout << "reading data error frame " << header.frame << endl;
								break;
							}
						}
					}
					if ( _N >= size ) {
						//данные считаны успешно
						memcpy( &_data[0], buffer, size );
						_frame = header.frame;
						_size = header.size;
						//cout << _frame << endl;
					}

					//_frame = header.frame;
					//_size = header.size;
					//int src = header.src32;
					
					//стираем использованный буфер

				}
				else {
					//укорачиваем текущий буфер, чтоб дальше продолжить искать
					shiftBuffer( buffer, _N, PacketHeaderSize );
				}


			}
			else {
				cout << "Network: Receive error - no data" << endl;
				disconnectClient( k );
				_N = 0;						//сброс считанного
				return;						//выходим из процедуры
			}
		}
		 


		/*if ( rec == _packetSizeFull ) {
		int pos;
		memcpy( &pos, _buffer, 4 );
		if ( pos >= 0 && pos + _packetSize <= _sizeAlign ) {
		memcpy( _data + pos, _buffer + 4, _packetSize );
		}
		else {
		cout << "Error: Received bad header " << pos << endl;
		}
		}
		else {
		cout << "Error: Received bad packet length " << rec << endl;
		}*/
	}
}

//-------------------------------------------------------------------
void pbNetworkReceiver::disconnectClient( int id )	//отключить клиента
{
	cout << "\tDisconnect client " << id + 1 << endl;
	if ( id < TCP.getNumClients() ) {
		TCP.disconnectClient( id );
	}
}

//-------------------------------------------------------------------

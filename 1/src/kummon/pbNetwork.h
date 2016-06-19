#pragma once

//Классы для передачи изображений и других данных по сети
//ВАЖНО: packetSize=1024 - как базовый, затем можно попробовать повысить

//OF change:
//In ofxTCPServer.h add declaration to class:
//ofxTCPManager			&server() { return TCPServer; }


#include "ofMain.h"
#include "ofxNetwork.h"


//Посылка синхронная
class pbNetworkSenderSync 
{
public:
	void setup( const string &addr, int port, int packetSize );		//размер пакета
	void close();

	bool send( unsigned char *data, int size, int frameNumber );
	void update();

	bool connected() { return _connected; }
private:
	ofxTCPClient tcpClient;	//клиент - отправитель
	int _port;
	string _addr;
	int _packetSize;

	bool _connected;
	float _connectTime;
	void reconnect();


};



//Прием, с возможностью выбора - асинхронный или синхронный
class pbNetworkReceiver : public ofThread //ofxThread
{
public:
    pbNetworkReceiver();
    ~pbNetworkReceiver();
	void setup( int port, int packetSize, bool threaded );		//размер изображения в байтах и размер пакета
	void close();

	int frame() { return _frame; }		//последний считанный фрейм
	int size()  { return _size; }		//последние считанный размер
	vector<unsigned char> &data() { return _data; }	//данные, размер - может быть больше чем size()!

	void threadedFunction();

	void receive();

	void restart();	//перезапуск получателя

private:
	bool _threaded;

	int _port;
	int _packetSize;

	ofxTCPServer TCP;					//сервер - получатель
	void disconnectClient( int id );	//отключить клиента

	int _frame;
	int _size;
	vector<unsigned char> _data;

	void receive0();
	vector<char> _buffer;
	int maxN;
	int _N;	//сколько считано в буфер

	bool _wantRestart;
	void startTCP();

};

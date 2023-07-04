#pragma once
#include "Engine.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <thread>

#define PORT_NUMBER			8080
#define HOST_IP				"127.0.0.1"

using namespace std;

class ServerManager
{
	SINGLE(ServerManager)
public:
	void Initalize();
	void SocketCreate();
	void ConvertIP();
	void Connect();
	void Clear();
	void Receive();
	void PushSend(void* data);

	void Rlease();
	
	GETSET(const WSADATA,		mWSData, WSData)
	GETSET(const WORD,			mVersion, Version)
	GETSET(const SOCKET,		mSocket, Socket)
	GETSET(const sockaddr_in,	mServerAddr, ServerAddr)
	GETSET(const string& ,		mServerIP, ServerIP)
	GETSET(const string&,		mClientName, ClientName)

private:
	WSADATA				mWSData;
	WORD				mVersion;
	SOCKET				mSocket;
	sockaddr_in			mServerAddr;

	string				mClientName;
	string				mServerIP;

	void*				mSendData;
	void*				mReceiveData;
};


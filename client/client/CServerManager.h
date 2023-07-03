#pragma once
#include "def.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#define PORT_NUMBER 8080
#define HOST_IP "127.0.0.1"

using namespace std;

class ServerManager
{
	SINGLE(ServerManager)
public:
	void Initalize();
	void SocketCreate();
	void ConvertIP();
	void Connect();
	void Send();
	void Receive();
	void Clear();
	


	GETSET(const WSADATA, mWSData, WSData)
	GETSET(const WORD, mVersion, Version)
	GETSET(const SOCKET, mSocket, Socket)
	GETSET(const sockaddr_in, mServerAddr, ServerAddr)

	GETSET(const string& , mServerIP, ServerIP)

private:
	WSADATA mWSData;
	WORD mVersion;
	SOCKET mSocket;
	sockaddr_in mServerAddr;

	char mTextBuffer[256];
	string mServerIP;
};


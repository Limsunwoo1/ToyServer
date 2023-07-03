#pragma once
#include "def.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#define PORT_NUMBER 8080
#define HOST_IP "218.236.53.10"

using namespace std;

class ServerManager
{
	SINGLE(ServerManager)
public:
	void Initalize();
	void SocketCreate();
	void ConverIP();
	void Connect();
	void Send();
	void Receive(char* data);
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


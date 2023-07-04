#include <iostream>
#include <thread>
#include "def.h"
#include "CServerManager.h"

void SendFunc()
{
	while (1)
	{
		GETSINGLE(ServerManager)->Send();
	}
}

void ReceiveFunc()
{
	while (1)
	{
		GETSINGLE(ServerManager)->Receive();
	}
}


int main()
{
	GETSINGLE(ServerManager)->Initalize();
	GETSINGLE(ServerManager)->SocketCreate();
	GETSINGLE(ServerManager)->ConvertIP();
	GETSINGLE(ServerManager)->Connect();


	std::thread sendthread(SendFunc);
	std::thread receivethread(ReceiveFunc);

	sendthread.detach();
	receivethread.detach();

	while (1)
	{
		int a = 0;
	}

	return 0;
}
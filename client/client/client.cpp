#include <iostream>
#include "def.h"
#include "CServerManager.h"

int main()
{
	GETSINGLE(ServerManager)->Initalize();
	GETSINGLE(ServerManager)->SocketCreate();
	GETSINGLE(ServerManager)->ConverIP();
	GETSINGLE(ServerManager)->Connect();

	while (1)
	{
		GETSINGLE(ServerManager)->Send();
	}

	return 0;
}
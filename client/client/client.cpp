#include <iostream>
#include <thread>
#include "Engine.h"
#include "CServerManager.h"

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

	std::thread receivethread(ReceiveFunc);

	receivethread.detach();


	int a = 0;
	while (1)
	{
		
		// 나중에 함수화 하여 특정한 상황에
		// Send한다
		if (a >= 9999999)
		{
			ChatMassege_Packet chat = {};
			chat.type = ServerDataType::ChatMessege;
			chat.name = GETSINGLE(ServerManager)->GetClientName();

			cout << "메세지를 입력하세요 : ";
			char buf[256] = {};
			chat.Messege = string(gets_s(buf));

			if(chat.Messege.find("/") != string::npos)
			{
				int curNum = chat.Messege.find("/");
				int endNum = chat.Messege.find(" ");

				string name = chat.Messege.substr(curNum + 1, endNum - 1);
				string messege = chat.Messege.substr(endNum + 1, chat.Messege.size());

				WhisperMessege_Packet wPacket = {};
				wPacket.type = ServerDataType::WhisperMessege;
				wPacket.name = GETSINGLE(ServerManager)->GetClientName();
				wPacket.Messege = messege;
				wPacket.otherName = name;

				GETSINGLE(ServerManager)->PushSend((void*)&wPacket);
				continue;
			}

			GETSINGLE(ServerManager)->PushSend((void*)&chat);
		}
		a++;
	}

	// window Api 위에선 종료 시점에 호출 하면 된다.
	GETSINGLE(ServerManager)->Rlease();
	return 0;
}
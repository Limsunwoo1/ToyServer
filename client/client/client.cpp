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
		
		// ���߿� �Լ�ȭ �Ͽ� Ư���� ��Ȳ��
		// Send�Ѵ�
		if (a >= 9999999)
		{
			ChatMassege_Packet chat = {};
			chat.type = ServerDataType::ChatMessege;
			chat.name = GETSINGLE(ServerManager)->GetClientName();

			cout << "�޼����� �Է��ϼ��� : ";
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

	// window Api ������ ���� ������ ȣ�� �ϸ� �ȴ�.
	GETSINGLE(ServerManager)->Rlease();
	return 0;
}
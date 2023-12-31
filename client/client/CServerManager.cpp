#include "CServerManager.h"
#include <stdio.h>

ServerManager::ServerManager()
{
}

ServerManager::~ServerManager()
{
}

void ServerManager::Initalize()
{
	mVersion = MAKEWORD(2, 2);
	if (WSAStartup(mVersion, &mWSData) != 0)
	{
		std::cout << "InitError!" << std::endl;

		WSACleanup();
		exit(1);
	}
}

void ServerManager::SocketCreate()
{
	mSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mSocket == INVALID_SOCKET)
	{
		std::cout << "SocketCreateError!" << std::endl;

		WSACleanup();
		exit(1);
	}
}

void ServerManager::ConvertIP()
{
	mServerIP = "";

	std::cout << "연결 될 서버 IP 주소를 입력해주세요 : ";
	char serverIP[_MAX_PATH] = {};
	gets_s(serverIP);
	mServerIP = string(serverIP);

	
	if (mServerIP.find("local") != std::string::npos
		|| mServerIP.find("LOCAL") != std::string::npos
		|| mServerIP.find("Local") != std::string::npos)
	{
		mServerIP = HOST_IP;
	}
	//mServerIP = HOST_IP;

	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_port = htons(PORT_NUMBER);

	INT error = inet_pton(AF_INET, mServerIP.c_str(), &(mServerAddr.sin_addr));

	if (!error)
	{
		std::cout << "ConvertError!" << std::endl;

		closesocket(mSocket);
		WSACleanup();
	}
}

void ServerManager::Connect()
{
	socklen_t len = (socklen_t)sizeof(mServerAddr);
	int iConnect = connect(mSocket, (sockaddr*)&mServerAddr, len);
	if (iConnect == SOCKET_ERROR)
	{
		std::cout << "ConnectError!" << std::endl;

		closesocket(mSocket);
		WSACleanup();

		exit(1);
	}

	cout << "ID 를 입력하세요 : ";
	char nameData[MAX_NAME_SIZE] = {};
	gets_s(nameData);
	mClientName = string(nameData);
	

	std::cout << "Client Connect.." << std::endl;

	// 로그인 패킷 전송 람다 함수
	function<void()> loginFun = [this]() {
		Login_Packet packet = {};
		packet.type = ServerDataType::LoginData;
		packet.name = GetClientName();

		PushSend((void*)&packet);
	};
	loginFun();


	// 리시브 받을 준비
	std::thread receivethread([this]()
		{
			while (1)
			{
				Receive();
			}
		});

	receivethread.detach();
}

void ServerManager::Receive()
{
	// 서버에 소켓데이터 버퍼 수신
	char buf[1024] = {};

	int iRecv = recv(mSocket, reinterpret_cast<char*>(buf), MAX_DATA_SIZE, 0);
	if (iRecv == SOCKET_ERROR)
	{
		int test = WSAGetLastError();
		std::cout << test << std::endl;
		std::cout << "ReceiveError!" << std::endl;

		closesocket(mSocket);
		WSACleanup();

		exit(1);
	}

	if (iRecv != 0)
	{
		thread receiveThread([&buf,this]()
			{
				int* dataType = reinterpret_cast<int*>(buf);
				int type = *dataType;

				switch ((ServerDataType)type)
				{
				case ServerDataType::LoginData:
				{
					Login_Packet* chatData = reinterpret_cast<Login_Packet*>(buf);
					cout << endl;
					cout << chatData->name << " 님이 입장하였습니다" << endl;
					std::cout << "메세지 입력 : ";
				}
				break;
				case ServerDataType::LogoutData:
				{
					Logout_Packet* chatData = reinterpret_cast<Logout_Packet*>(buf);
					cout << endl;
					cout << chatData->name << " 님의 퇴장하였습니다" <<endl;
					std::cout << "메세지 입력 : ";
				}
				break;
				case ServerDataType::ChatMessege:
				{
					ChatMassege_Packet* chatData = reinterpret_cast<ChatMassege_Packet*>(buf);
					cout << endl;
					cout << chatData->name << " 님의 메세지 : " << chatData->Messege << endl;
					std::cout << "메세지 입력 : ";
				}
				break;

				case ServerDataType::WhisperMessege:
				{
					WhisperMessege_Packet* whisperData = reinterpret_cast<WhisperMessege_Packet*>(buf);
					cout << endl;
					cout << whisperData->name << " 님의 귓속말 메세지 : " << whisperData->Messege << endl;
					std::cout << "메세지 입력 : ";
				}
					break;

				case ServerDataType::DamegeData:

					break;

				case ServerDataType::PositionData:

					break;
				case ServerDataType::RigidbodyData:

					break;

				default:
					break;
				}

				return;
			});
		receiveThread.join();
	}
}

void ServerManager::PushSend(void* data)
{
	int* type = reinterpret_cast<int*> (data);

	int bufSize = 0;
	switch ((ServerDataType)*type)
	{
	case ServerDataType::LoginData:			bufSize = sizeof(Login_Packet);				break;
	case ServerDataType::LogoutData:		bufSize = sizeof(Logout_Packet);			break;
	case ServerDataType::ChatMessege:		bufSize = sizeof(ChatMassege_Packet);		break;
	case ServerDataType::WhisperMessege:	bufSize = sizeof(WhisperMessege_Packet);	break;
	case ServerDataType::PositionData:		bufSize = sizeof(Position_Packet);			break;
	case ServerDataType::AnimationData:		bufSize = sizeof(Animation_Packet);			break;
	case ServerDataType::DamegeData:
	{
	}
		break;
	case ServerDataType::RigidbodyData:
	{
	}
		break;
	};

	INT isend = send(mSocket, reinterpret_cast<char*>(data), bufSize, 0);
	if (isend == SOCKET_ERROR)
	{
		cout << "SendError...!" << endl;

		closesocket(mSocket);
		WSACleanup();

		exit(1);
		return;
	}
}

void ServerManager::Rlease()
{
	Logout_Packet packet = {};
	packet.type = ServerDataType::LogoutData;
	packet.name = GetClientName();

	PushSend((void*)&packet);

	Clear();
}

void ServerManager::Clear()
{
	closesocket(mSocket);
	WSACleanup();
}

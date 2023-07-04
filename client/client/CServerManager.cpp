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
		std::cout << "Error!" << std::endl;

		WSACleanup();
		exit(1);
	}
}

void ServerManager::SocketCreate()
{
	mSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mSocket == INVALID_SOCKET)
	{
		std::cout << "Error!" << std::endl;

		WSACleanup();
		exit(1);
	}
}

void ServerManager::ConvertIP()
{
	mServerIP = "";

	std::cout << "연결 될 서버 IP 주소를 입력해주세요 : ";
	char serverIP[_MAX_PATH];
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
		std::cout << "Error!" << std::endl;

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
		std::cout << "Error!" << std::endl;

		closesocket(mSocket);
		WSACleanup();

		exit(1);
	}

	cout << "ID 를 입력하세요 : ";
	char nameData[MAX_NAME_SIZE];
	gets_s(nameData);
	mClientName = nameData;
	

	std::cout << "Client Connect.." << std::endl;
	//if(getsockopt())
}

void ServerManager::Send()
{
	// 소켓 데이터 버퍼에 send
	eTCP_Data<char> testData = {};
	char chatBuf[256] = {};
	gets_s(chatBuf);

	testData.type = ServerDataType::ChatMessege;
	testData.Name = mClientName;
	testData.Data = chatBuf;
	PushData((void*)&testData);
	
	int iSent = send(mSocket, reinterpret_cast<char*>(mSendData), sizeof(mSendData), 0);
	if (iSent == SOCKET_ERROR)
	{
		std::cout << "SendError!" << std::endl;

		closesocket(mSocket);
		WSACleanup();

		exit(1);
	}

	ZeroMemory(mSendData, sizeof(mSendData));
}

void ServerManager::Receive()
{
	// 서버에 소켓데이터 버퍼 수신
	int iRecv = recv(mSocket, reinterpret_cast<char*>(mReceiveData), MAX_DATA_SIZE, 0);
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
		thread receiveThread(&ServerManager::ReceiveEvent, this);
		receiveThread.detach();
	}
}

void ServerManager::ReceiveEvent()
{
	if (mReceiveData == nullptr)
		return;


	int* dataType = reinterpret_cast<int*>(mReceiveData);
	int type = *dataType;

	switch ((ServerDataType)type)
	{
	case ServerDataType::ChatMessege :
	{
		eTCP_Data<char>* chatData = reinterpret_cast<eTCP_Data<char>*>(mReceiveData);
		cout << chatData->Name << " 님의 메세지 : " << chatData->Data << endl;
	}
		break;

	case ServerDataType::DamegeData:

		break;

	case ServerDataType::PostionData:

		break;
	case ServerDataType::RigidbodyData:

		break;

	default:
		break;
	}

	ZeroMemory(mReceiveData, sizeof(mReceiveData));
}

void ServerManager::Clear()
{
	
}

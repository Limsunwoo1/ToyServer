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

	std::cout << "���� �� ���� IP �ּҸ� �Է����ּ��� : ";
	std::cin >> mServerIP;
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
}

void ServerManager::Send()
{
	// ���� ������ ���ۿ� send
	char test[256];
	gets_s(test);

	int iSent = send(mSocket, test, sizeof(test), 0);
	if (iSent == SOCKET_ERROR)
	{
		std::cout << "Error!" << std::endl;

		closesocket(mSocket);
		WSACleanup();

		exit(1);
	}

	ZeroMemory(test, sizeof(test));
}

void ServerManager::Receive()
{
	// ������ ���ϵ����� ���� ����
	char buf[256] = {};
	int iRecv = recv(mSocket, buf, 256, 0);
	if (iRecv == SOCKET_ERROR)
	{
		int test = WSAGetLastError();
		std::cout << test << std::endl;
		std::cout << "Error!" << std::endl;

		closesocket(mSocket);
		WSACleanup();

		exit(1);
	}

	std::cout << "������ ���� : " << buf << std::endl;
}

void ServerManager::Clear()
{
	
}

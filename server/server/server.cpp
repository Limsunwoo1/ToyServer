// server.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>>
#include <list>
#include <thread>

#define PORT_NUMBER 8080
#define HOST_IP "127.0.0.1"


std::list<SOCKET> soketList;
SOCKET ServerSocket;

bool ClientHandler(SOCKET sock);
int main()
{
	// was 초기화
	WSADATA wsa;
	WORD serverVersion = MAKEWORD(2, 2);
	int error = WSAStartup(serverVersion, &wsa);
	if (error != 0)
	{
		std::cout << "Sever_error!" << std::endl;

		for (SOCKET socket : soketList)
			closesocket(socket);

		exit(1);
	}


	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		std::cout << "Sever_error!" << std::endl;

		for (SOCKET socket : soketList)
			closesocket(socket);

		exit(1);
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NUMBER);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	// 아이피 주소 설정 (INADDR_ANY 키워드로 모든 네트워크 인터페이스에서 들어오는 연결을 수락)

	if (bind(ServerSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::cout << "Sever_error!" << std::endl;

		for (SOCKET socket : soketList)
			closesocket(socket);

		exit(1);
	}

	if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Sever_error!" << std::endl;

		for (SOCKET socket : soketList)
			closesocket(socket);

		exit(1);
	}

	while (1)
	{
		sockaddr_in clientAddr;
		SOCKET clientSocket;
		socklen_t clientSize = (socklen_t)sizeof(clientAddr);

		clientSocket = accept(ServerSocket, (sockaddr*)&clientAddr, &clientSize);
		soketList.emplace_back(clientSocket);

		if (clientSocket == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			std::cout << error << std::endl;

			closesocket(clientSocket);

			exit(1);
		}
		
		std::thread clientHandle(ClientHandler, clientSocket);
		clientHandle.detach();

	}

	for (SOCKET socket : soketList)
	{
		closesocket(socket);
	}

	WSACleanup();
}

bool ClientHandler(SOCKET sock)
{
	char buf[256] = {};

	while (1)
	{
		INT iReceive = recv(sock, buf, 256, 0);

		if (iReceive == SOCKET_ERROR)
		{
			std::cout << "Error!" << std::endl;
			return false;
		}

		for (SOCKET sendsock : soketList)
		{
			INT iSend = send(sendsock, buf, 256, 0);
			if (iSend == SOCKET_ERROR)
			{
				std::cout << "Error!" << std::endl;
				return false;
			}
		}

		std::cout << buf << std::endl;
		ZeroMemory(buf, sizeof(buf), 0);
	}

	return true;
}
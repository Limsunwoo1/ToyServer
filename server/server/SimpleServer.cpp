#include "SimpleServer.h"
#include <bitset>

void SimpleServer::Initalize()
{
	WSAStartUp();
	CreateScoket();
	BindSocket();
	ListenSocket();
}

void SimpleServer::WSAStartUp()
{
	// 소켓 라이브러리 버젼 2.2
	mVersion = MAKEWORD(2, 2);
	INT iWSA = WSAStartup(mVersion, &mWSAData);
	if (iWSA != 0)
	{
		cout << "WSAStartup() Fail..!" << endl;
		exit(1);
	}

	cout << "WSAStartup() Access..!" << endl;
}

void SimpleServer::CreateScoket()
{
	// 소켓 생성
	mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mServerSocket == INVALID_SOCKET)
	{
		cout << "SocketCreateFail...!" << endl;
		EXIT();
	}

	cout << "SocketCreateAccess..!" << endl;
}

void SimpleServer::BindSocket()
{
	mSocketAddr.sin_family = AF_INET;
	mSocketAddr.sin_port = htons(PORT_NUMBER);
	mSocketAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	// 아이피 주소 설정 (INADDR_ANY 키워드로 모든 네트워크 인터페이스에서 들어오는 연결을 수락)

	if (bind(mServerSocket, (sockaddr*)&mSocketAddr, sizeof(mSocketAddr)) == SOCKET_ERROR)
	{
		std::cout << "BindFail...!" << std::endl;

		EXIT();
	}

	cout << "BindAccess..!" << endl;
}

void SimpleServer::ListenSocket()
{
	if (listen(mServerSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "ListenFail...!" << std::endl;

		EXIT();
	}

	std::cout << "Running..." << std::endl;
}

void SimpleServer::Accept()
{
	while (1)
	{
		sockaddr_in clientAddr;
		SOCKET clientSocket;
		socklen_t clientSize = (socklen_t)sizeof(clientAddr);

		clientSocket = accept(mServerSocket, (sockaddr*)&clientAddr, &clientSize);
		mAllSocketList.emplace_back(clientSocket);

		if (clientSocket == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			std::cout << error << std::endl;

			closesocket(clientSocket);

			exit(1);
		}

		thread clientHandle([this, &clientSocket]()
		{
			cout << clientSocket << " - 소켓번호 " << endl;
			ClientHandler(clientSocket);
		});
		clientHandle.detach();
	}
}

INT SimpleServer::SendData(SOCKET sock, void* data, int dataLen, int flags)
{
	INT iSend = send(sock, reinterpret_cast<char*>(data), dataLen, flags);
	if (iSend == SOCKET_ERROR)
	{
		std::cout << "SendError...!" << std::endl;
	}

	return iSend;
}

INT SimpleServer::ReceiveData(SOCKET sock, void* data, int dataLen, int flags)
{
	INT iReceive = recv(sock, reinterpret_cast<char*>(data), dataLen, flags);
	if (iReceive == SOCKET_ERROR)
	{
		std::cout << "ReceiveError...!" << std::endl;
		return SOCKET_ERROR;
	}
	
	int* dataType = reinterpret_cast<int*>(data);
	int type = *dataType;


	switch ((ServerDataType)type)
	{
	case ServerDataType::ChatMessege: 
	{
		eTCP_Data<char>* tctData = reinterpret_cast<eTCP_Data<char>*>(data);
		// 스레드로 함수 실행
		std::thread ChatThread([this,&tctData, sock, data, dataLen]() {
			std::cout << tctData->Name << " 님의 메세지 : " << tctData->Data << std::endl;

			for (SOCKET sock : mAllSocketList)
			{
				SendData(sock, data, dataLen);
			}
			});
		ChatThread.detach();
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
	

	
	

	return iReceive;
}

void SimpleServer::EXIT()
{
	cout << "EXIT..." << endl;

	mAllSocketList.clear();

	closesocket(mServerSocket);
	WSACleanup();
}

bool SimpleServer::ClientHandler(SOCKET sock)
{
	void* buf = nullptr;

	while (1)
	{
		INT iReceive = ReceiveData(sock, buf, 2048);

		if (iReceive == SOCKET_ERROR)
		{
			return false;
		}

		for (SOCKET sendsock : mAllSocketList)
		{
			INT iSend = SendData(sock, buf, 256);
			if (iSend == SOCKET_ERROR)
			{
				return false;
			}
		}

		std::cout << buf << std::endl;
		ZeroMemory(buf, sizeof(buf), 0);
	}

	return false;
}

#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <stdlib.h>
#include <WinSock2.h>

using namespace std;

void ErrorHandling(string_view message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[] = "Hello World!";
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <port>" << endl;
		exit(1);
	}

	// 20행 WSAStartup() 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	// 23행 socket() 소켓 생성
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	// 32행 bind() 소켓에 IP 주소와 PORT 번호 할당
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("bind() error");
	}

	// 35행 listen() 생성한 소켓을 서버 소켓으로 완성
	if (listen(hServSock, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() error");
	}

	// 39행 accept() 클라이언트의 연결요청 수락
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET) {
		ErrorHandling("accept() error");
	}

	// 43행 send() 데이터 전송
	send(hClntSock, message, sizeof(message), 0);
	closesocket(hClntSock);
	closesocket(hServSock);
	// 46행 소켓 라이브러리 해제
	WSACleanup();
	return 0;
}

void ErrorHandling(string_view message)
{
	cout << message << endl;
	exit(1);
}
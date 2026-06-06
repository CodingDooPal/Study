#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <winSock2.h>

using namespace std;

void ErrorHandling(string message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen;
	if (argc != 3) {
		cout << "Usage : " << argv[0] << " <IP> <port>" << endl;
		exit(1);
	}
	
	// 20행 WSAStartup() 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	// 23행 socket() 소켓 생성
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// 32행 connect() 서버에 연결요청
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error!");
	}

	// 35행 recv() 서버로부터 전송되는 데이터 수신
	strLen = recv(hSocket, message, sizeof(message) - 1, 0);
	if (strLen == -1) {
		ErrorHandling("read() error!");
	}
	cout << "Message from server: " << message << endl;

	closesocket(hSocket);
	// 41행 WSACleanup() 소켓 라이브러리 해제
	WSACleanup();
	return 0;
}

void ErrorHandling(string message) {
	cout << message << endl;
	exit(1);
}
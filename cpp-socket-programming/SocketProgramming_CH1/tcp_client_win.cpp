#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <stdlib.h>
#include <winsock2.h>

using namespace std;

void ErrorHandling(string_view message);

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    // 9행: 함수의 반환 값 저장을 위한 SOCKET형 변수
    SOCKET hSocket;
    SOCKADDR_IN servAddr;

    char message[30];
    int strLen = 0;
    int idx = 0, readLen = 0;

    if (argc != 3) {
        cout << "Usage : " << argv[0] << " <IP> <port>" << endl;
        exit(1); 
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        ErrorHandling("WSAStartup() error!");
    }

    // 25행: socket() TCP 소켓 생성
    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET) {
        ErrorHandling("socket() error");
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        ErrorHandling("connect() error!");
    }

    // 37행: recv() 수신된 데이터를 1바이트씩 읽음
    while (readLen = recv(hSocket, &message[idx++], 1, 0)) {
        if (readLen == -1) {
            ErrorHandling("read() error!");
        }

        // 1바이트씩 읽기 때문에 실제로 더해지는 값은 1(recv 함수의 호출횟수)
        strLen += readLen;
    }

    cout << "Message from server: " << message << endl;
    cout << "Function read call count: " << strLen << endl;

    closesocket(hSocket);
    WSACleanup();
    return 0;
}

void ErrorHandling(string_view message) {
    cout << message << endl;
    exit(1);
}
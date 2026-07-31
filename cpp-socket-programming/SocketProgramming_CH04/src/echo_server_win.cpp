#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>

#define BUF_SIZE 1024
using namespace std;

void ErrorHandling(string_view message);

int main(int argc, char* argv[])
{
    WSADATA wsaData{};
    SOCKET hServSock{}, hClntSock{};
    vector<char> message(BUF_SIZE);
    int strLen{}, i{};

    SOCKADDR_IN servAdr{}, clntAdr{};
    int clntAdrSize{};

    if (argc != 2) {
        cout << "Usage " << argv[0] << " <port>" << endl;
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        ErrorHandling("WSAStartup() error!");
    }

    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if (hServSock == INVALID_SOCKET) {
        ErrorHandling("socket() error!");
    }

    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(atoi(argv[1]));

    if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) {
        ErrorHandling("bind() error!");
    }

    if (listen(hServSock, 5) == SOCKET_ERROR) {
        ErrorHandling("listen() error!");
    }

    clntAdrSize = sizeof(clntAdr);

    for (i = 0; i < 5; ++i) {
        hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
        if (hClntSock == -1) {
            ErrorHandling("accept() error!");
        }
        else {
            cout << "Connect client " << i + 1 << endl;
        }

        while ((strLen = recv(hClntSock, message.data(), BUF_SIZE, 0)) != 0) {
            send(hClntSock, message.data(), strLen, 0);
        }

        closesocket(hClntSock);
    }
    closesocket(hServSock);
    WSACleanup();
    return 0;
}

void ErrorHandling(string_view message)
{
    cout << message << endl;
    exit(1);
}

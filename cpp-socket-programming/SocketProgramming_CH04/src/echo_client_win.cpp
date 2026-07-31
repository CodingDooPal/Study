#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#define BUF_SIZE 1024

using namespace std;

void ErrorHandling(string_view message);

int main(int argc, char* argv[])
{
    WSADATA wsaData{};
    SOCKET hSocket{};
    vector<char> message(BUF_SIZE);
    int strLen{};
    SOCKADDR_IN servAdr{};

    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        ErrorHandling("WSAStartup() error!");
    }

    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET) {
        ErrorHandling("socket() error");
    }

    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);
    servAdr.sin_port = htons(atoi(argv[2]));

    if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) {
        ErrorHandling("connect() error!");
    }
    else {
        cout << "Connected............" << endl;
    }

    while (true) {
        cout << "Input message(Q to quit): ";
        string input{};
        getline(cin, input);
        
        if (input == "q" || input == "Q") {
            break;
        }

        send(hSocket, input.c_str(), input.size(), 0);
        strLen = recv(hSocket, message.data(), BUF_SIZE - 1, 0);
        string received(message.data(), strLen);
        cout << "Message from server: " << received << endl;
    }
    closesocket(hSocket);
    WSACleanup();
    return 0;
}

void ErrorHandling(string_view message) {
    cout << message << endl;
    exit(1);
}

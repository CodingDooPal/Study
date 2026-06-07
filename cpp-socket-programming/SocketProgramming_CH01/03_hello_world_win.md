## 윈도우 기반의 Hello world! 프로그램 구현

### 1) 서버(hello_server_win.cpp)

```cpp
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
```
· 20행: 소켓 라이브러리를 초기화하고 있다.  
· 23, 32행: 23행에서 소켓을 생성하고, 32행에서 이 소켓에 IP 주소와 PORT 번호를 할당하고 있다.  
· 35행: listen 함수호출을 통해서 23행에서 생성한 소켓을 서버 소켓으로 완성하였다.  
· 39행: 클라이언트의 연결요청을 수락하기 위해서 accept 함수를 호출하고 있다.  
· 43행: send 함수 호출을 통해서 39행에서 연결된 클라이언트에 데이터를 전송하고 있다.
· 46행: 프로그램을 종료하기 전에 20행에서 초기화한 소켓 라이브러리를 해제하고 있다.  

---

### 2) 클라이언트(hello_client_win.cpp)

```cpp
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
```
· 20행: 소켓 라이브러리를 초기화하고 있다.  
· 23, 32행: 23행에서 소켓을 생성하고, 32행에서 생성된 소켓을 바탕으로 서버에 연결요청을 하고 있다.  
· 35행: recv 함수호출을 통해서 서버로부터 데이터를 수신하고 있다.  
· 41행: 20행에서 초기화한 소켓 라이브러리를 해제하고 있다.  

---

바로가기  
[서버 코드](./src/hello_server_win.cpp)  
[클라이언트 코드](./src/hello_client_win.cpp)  

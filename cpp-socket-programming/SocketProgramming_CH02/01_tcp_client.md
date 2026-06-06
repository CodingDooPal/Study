## 연결지향형 소켓, TCP 소켓의 예

### 1) 리눅스 버전

```cpp
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

void error_handling(string_view message);


int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0;
    int idx = 0, read_len = 0;

    if (argc != 3) {
        cout << "Usage: " << argv[0] << "<IP> <port>" << endl;
        exit(1);
    }
    
    // 17행 TCP 소켓 생성   
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        error_handling("socket() error!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1){
		error_handling("connect() error!");
	}

    // 29행 while문에서 read 함수 반복 호출, 1바이트씩 데이터를 읽어 들인다.
    while(read_len = read(sock, &message[idx++], 1)){
        if(read_len == -1) {
            error_handling("read() error!");
        }
        
        // 34행 str_len에 읽어들인 값을 더한다.(항상 1)
        str_len += read_len;
    }

    cout << "Message from server: " << message << endl;
    cout << "Function read call count: " << str_len << endl;
    close(sock);
    return 0;
}

void error_handling(string_view message)
{
	cout << message << endl;
	exit(1);
}
```
· 17행: TCP 소켓을 생성하고 있다. 첫 번째 인자와 두 번째 인자로 각각 PF_INET, SOCK_STREAM가 전달되면
세 번째 인자인 IPPROTO_TCP은 생략 가능하다.  
· 29행: while문 안에서 read 함수를 반복 호출하고 있다. 중요한 것은 이 함수가 호출될 때마다
1바이트씩 데이터를 읽어 들인다는 점이다. 그리고 read 함수가 0을 반환하면 이는 거짓을 의미하기 때문에
while문을 빠져나간다.  
· 34행: 이 문장이 실행될 때 변수 read_len에 저장되어 있는 값은 항상 1이다. 29행에서 1바이트씩 데이터를 읽고 있기 때문이다.
결국 while문을 빠져나간 이후에 str_len에는 읽어 들인 바이트 수가 저장된다.  

### 2) 윈도우 버전
```cpp
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
```
· 9, 25행:  9행에서는 socket 함수의 반환 값 저장을 위해서 SOCKET형 변수 하나를 선언하였다. 그리고 25행에서는
socket 함수호출을 통해서 TCP 소켓을 생성하고 있다. 이제 이 두 문장이 눈에 들어올 것이다.  
· 37행: while문 안에서 recv 함수호출을 통해, 수신된 데이터를 1바이트씩 읽고 있다.  
· 42행: 37행에서 1바이트씩 데이터를 읽고 있기 때문에 이 문장에서 변수 strLen에 실제로 더해지는 값은 1이며, 
이는 recv 함수의 호출횟수와 같다.  

---
바로가기  
[리눅스 코드로 이동](./src/tcp_client.cpp)  
[윈도우 코드로 이동](./src/tcp_client_win.cpp)

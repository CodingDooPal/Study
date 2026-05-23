# Study 소켓 프로그래밍 예제 코드

## 소켓 프로그래밍 Hello world! 프로그램 구현

### 들어가기에 앞서

예제 코드는 되도록  서버와 클라이언트 두 관점을 모두 고려할 것이다.
이 과정에서 서버는 Linux, 클라이언트는 Windows 기반으로 진행할 것이다.
Windows와 Linux에서 사용하는 라이브러리와 함수에는 차이점이 존재한다.
차이점 존재 시 예제 코드 앞에 기술하여 혼동을 방지할 생각이다.

1. include
- Linux에서 사용하는 소켓 라이브러리
```
#inlcude <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
```

- Windows 에서 사용하는 소켓 라이브러리
```
#include <WinSock2.h>
#include <WS2tcpip.h>
```

2. 함수
- Linux에서 사용하는 함수 
```
write(), close(), read()
// 모두 unistd.h에 정의되어 있다.
```

- Windows에서 사용하는 함수
```
send(), closesocket(), recv()
// 각각 write(), close(), read()를 대신하며 windock2.h에 정의되어 있다.
// 여기서 recv()는 뒤에 flag 인수를 하나 더 추가해야 하는데 보통 0으로 둔다.
```
### 1) 서버

```cpp
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
//#include <WinSock2.h>
//#include <WS2tcpip.h>
void error_handling(const char* message);

int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[] = "Hello World!";

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0); // 26행
	if (serv_sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) { // 35행
		error_handling("bind() error");
	}

	if (listen(serv_sock, 5) == -1) { // 38행
		error_handling("listen() error");
	}

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); // 42행
	if (clnt_sock == -1) {
		error_handling("accept() error");
	}

	write(clnt_sock, message, sizeof(message)); // 46행
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
```
· 26행: socket 함수호출을 통해서 소켓을 생성하고 있다.
· 35행: bind 함수호출을 통해서 IP 주소와 PORT 번호를 할당하고 있다.
· 38행: listen 함수를 호출하고 있다. 이로써 소켓은 연결요청을 받아들일 수 있는 상태가 된다.
· 42행: 연결요청의 수락을 위한 accept 함수를 호출하고 있다. 연결요청이 없는 상태에서 이 함수가 호출되면, 연결요청이 있을 때까지 함수는 반환하지 않는다.
· 46행: 잠시 후에 소개하는 write 함수는 데이턴를 전송하는 기능의 함수인데, 42행을 지나서 이 문장이 실행되었다는 것은 연결요청이 있었다는 뜻이 된다.

### 2) 클라이언트

```cpp
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
//#include <arpa/inet.h>
//#include <sys/socket.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
void error_handling(const char* message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0); // 22행
	if (sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) { // 31행
		error_handling("connect() error");
	}

	str_len = recv(sock, message, sizeof(message) - 1, 0);
	if (str_len == -1) {
		error_handling("read() error!");
	}

	printf("Message from server: %s\n", message);
	closesocket(sock);
	return 0;
}

void error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
```
· 22행: 소켓을 생성하고 있다. 소켓을 생성하는 순간에는 서버 소켓과 클라이언트 소켓으로 나뉘지 않는다. bind, listen 함수의 호출이 이어지면 서버 소켓이 되는 것이고, connect 함수의 호출로 이어지면 클라이언트 소켓이 되는 것이다.
· 31행: connect 함수호출을 통해서 서버 프로그램에 연결을 요청하고 있다.


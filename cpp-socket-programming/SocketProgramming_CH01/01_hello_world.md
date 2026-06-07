# Study 소켓 프로그래밍 예제 코드

## 리눅스 기반의 Hello world! 프로그램 구현

### 1) 서버

```cpp
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
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

	// 26행 socket() 소켓 생성
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	// 35행 bind() IP 주소와 PORT 번호 할당
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind() error");
	}

	// 38행 listen() 연결요청을 받아들이는 상태
	if (listen(serv_sock, 5) == -1) { 
		error_handling("listen() error");
	}

	clnt_addr_size = sizeof(clnt_addr);
	// 42행 accept() 연결 요청 수락
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1) {
		error_handling("accept() error");
	}

	// 46행 데이터 전송 및 소켓 종료
	write(clnt_sock, message, sizeof(message));
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
· 46행: 잠시 후에 소개하는 write 함수는 데이터를 전송하는 기능의 함수인데, 42행을 지나서 이 문장이 실행되었다는 것은 연결요청이 있었다는 뜻이 된다.  

### 2) 클라이언트

```cpp
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
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

	// 22행 소켓 생성 부분
	sock = socket(PF_INET, SOCK_STREAM, 0); 
	if (sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	// 31행 connect() 서버 연결 부분
	if (connect(sock, struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1){
		error_handling("connect() error!");
	}

	str_len = read(sock, message, sizeof(message) - 1);
	if (str_len == -1) {
		error_handling("read() error!");
	}

	printf("Message from server: %s\n", message);
	close(sock);
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

바로가기  
[서버 코드로 이동](./src/hello_server.cpp)  
[클라이언트 코드로 이동](./src/hello_client.cpp)


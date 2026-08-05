#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 30
void error_handling(string_view message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char *argv[]) {
    int sock{};
    pid_t pid{};
    char buf[BUF_SIZE]{};
    struct sockaddr_in serv_adr{};
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("connect() error!");
    }

    pid = fork();
    // 34~37행: 35행에서 호출하는 write_routine 함수에는 데이터 출력에 관련된 코드만 존재한다.
    // 37행에서 호출하는 read_routine 함수에는 데이터 입력에 관련된 코드만 존재한다.
    if (pid == 0) {
        write_routine(sock, buf);
    } else {
        read_routine(sock, buf);
    }

    close(sock);
    return 0;
}

void read_routine(int sock, char *buf) {
    while (true) {
        int str_len = read(sock, buf, BUF_SIZE);
        if (str_len == 0) {
            return;
        }

        buf[str_len] = 0;
        cout << "Message from server: " << buf << endl;
    }
}

void write_routine(int sock, char *buf) {
    while (true) {
        fgets(buf, BUF_SIZE, stdin);
        if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
            // 62행: 서버로의 EOF 전달을 위해서 shutdown 함수가 호출되었다.
            // fork 함수호출을 통해서 파일 디스크립터가 복사된 상황에서는 반드시 shutdown 함수호출을 통해서
            // EOF의 전달을 별도로 명시해야 한다.
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, strlen(buf));
    }
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

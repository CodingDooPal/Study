#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

using namespace std;

void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int serv_sock{}, clnt_sock{};
    string message(BUF_SIZE, '\0');
    int str_len{}, i{};

    struct sockaddr_in serv_adr{}, clnt_adr{};
    socklen_t clnt_adr_sz{};

    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <port>" << endl;
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error!");
    }

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind() error!");
    }

    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error!");
    }

    clnt_adr_sz = sizeof(clnt_adr);

    // 42~54행 총 5개의 클라이언트에게 서비스를 제공하기 위한 반복문이다.
    for (i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
        if (clnt_sock == -1) {
            error_handling("accept() error");
        } else {
            cout << "Connected client " << i + 1 << endl;
        }

        // 50, 51행 실제 에코 서비스가 이뤄지는 부분이다.
        while ((str_len = read(clnt_sock, message.data(), BUF_SIZE)) != 0) {
            write(clnt_sock, message.c_str(), str_len);
        }

        // 53행 소켓을 대상으로 close 함수가 호출, 클라이언트 소켓에 EOF가 전달된다.
        close(clnt_sock);
    }
    // 55행 서비스가 끝나면, 마지막으로 서버 소켓을 종료
    close(serv_sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

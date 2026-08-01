#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
using namespace std;

#define TRUE 1
#define FALSE 0
void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int serv_sock{}, clnt_sock{};
    char message[30];
    int option{}, str_len{};
    socklen_t optlen{}, clnt_adr_sz{};
    struct sockaddr_in serv_adr{}, clnt_adr{};
    if(argc != 2) {
        cout << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error");
    }

    // Time-wait 상태에 있는 소켓의 PORT 번호를 할당할 수 있게 한다.
    optlen = sizeof(option);
    option = TRUE;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))) {
        error_handling("bind() error");
    }
    if(listen(serv_sock, 5) == 1 ) {
        error_handling("listen() error");
    }
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

    while((str_len = read(clnt_sock, message, sizeof(message))) != 0) {
        write(clnt_sock, message, str_len);
        write(1, message, str_len);
    }

    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

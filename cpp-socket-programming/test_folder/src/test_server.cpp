#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 100
void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int serv_sock{};
    int clnt_sock{};
    char message[BUF_SIZE]{};
    int str_len{};
    socklen_t clnt_adr_sz{};

    struct sockaddr_in serv_addr{};
    struct sockaddr_in clnt_addr{};

    if (argc != 2) {
        cout << "Using: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    clnt_adr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
    
    if (clnt_sock == -1) {
        error_handling("accept() error");
    }
    else {
        cout << "User connected" << endl;
        string str = "Wellcom to my server!!";
        write(clnt_sock, str.data(), str.size());
    }

    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}
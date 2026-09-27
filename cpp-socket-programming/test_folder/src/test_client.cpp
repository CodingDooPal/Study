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
    int clnt_sock{};
    struct sockaddr_in clnt_addr{};
    char message[BUF_SIZE]{};
    int str_len{};
    socklen_t clnt_adr_sz{};

    if (argc != 3) {
        cout << "Using " << argv[0] << " <IP> <PORT>" << endl;
        exit(1);
    }

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (clnt_sock == -1) {
        error_handling("socket() error");
    }

    clnt_addr.sin_family = AF_INET;
    clnt_addr.sin_addr.s_addr = inet_addr(argv[1]);
    clnt_addr.sin_port = htons(atoi(argv[2]));

    if (connect(clnt_sock, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr)) == -1) {
        error_handling("connect() error");
    }

    str_len = read(clnt_sock, message, BUF_SIZE - 1);
    cout << message << endl;

    close(clnt_sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}
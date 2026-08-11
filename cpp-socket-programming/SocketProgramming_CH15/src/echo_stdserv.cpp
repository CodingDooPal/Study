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
    char message[BUF_SIZE]{};
    int str_len{}, i{};

    struct sockaddr_in serv_adr{}, clnt_adr{};
    socklen_t clnt_adr_sz{};
    FILE* readfp{nullptr};
    FILE* writefp{nullptr};

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

    for (i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
        if (clnt_sock == -1) {
            error_handling("accept() error");
        } else {
            cout << "Connected client " << i + 1 << endl;
        }

        readfp = fdopen(clnt_sock, "r");
        writefp = fdopen(clnt_sock, "w");
        while(!feof(readfp)) {
            fgets(message, BUF_SIZE, readfp);
            fputs(message, writefp);
            fflush(writefp);
        }

        fclose(readfp);
        fclose(writefp);
    }
    
    close(serv_sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

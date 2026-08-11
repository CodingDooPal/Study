#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

using namespace std;

void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int sock{};
    char message[BUF_SIZE]{};
    int str_len{};
    struct sockaddr_in serv_adr{};
    FILE* readfp{nullptr};
    FILE* writefp{nullptr};

    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <IP> <port>" << endl;
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error!");
    }

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("connect() error!");
    } else {
        cout << "Connected............." << endl;
    }

    readfp = fdopen(sock, "r");
    writefp = fdopen(sock, "w"); 
    while (true) {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }

        fputs(message, writefp);
        fflush(writefp);
        fgets(message, BUF_SIZE, readfp);
        cout << "Massage from server: " << message << endl;
    }
    fclose(writefp);
    fclose(readfp);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

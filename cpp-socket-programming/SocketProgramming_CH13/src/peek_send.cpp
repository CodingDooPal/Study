#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

void error_handling(string_view message);

int main(int argc, char* argv[]) {
    int sock{};
    struct sockaddr_in send_adr{};
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&send_adr, 0, sizeof(send_adr));
    send_adr.sin_family = AF_INET;
    send_adr.sin_addr.s_addr = inet_addr(argv[1]);
    send_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&send_adr, sizeof(send_adr)) == -1) {
        error_handling("connect() error!");
    }

    write(sock, "123", strlen("123"));
    sleep(5);
    close(sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

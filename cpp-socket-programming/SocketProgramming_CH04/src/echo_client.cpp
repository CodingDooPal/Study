#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

using namespace std;

void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int sock{};
    string message(BUF_SIZE, '\0');
    int str_len{};
    struct sockaddr_in serv_adr{};

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

    // 32행 connect 함수가 호출된다.
    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("connect() error!");
    } else {
        cout << "Connected............." << endl;
    }

    while (true) {
        cout << "Input message(Q to quit): ";
        getline(cin, message);

        if (message.compare("q") == 0 || message.compare("Q") == 0) {
            break;
        }

        write(sock, message.data(), message.size());
        str_len = read(sock, message.data(), BUF_SIZE - 1);
        message[str_len] = 0;
        cout << "Massage from server: " << message << endl;
    }
    // 50행 이렇게 close 함수가 호출되면 상대 소켓으로는 EOF가 전송된다.
    close(sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

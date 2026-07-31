#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define BUF_SIZE 1024

using namespace std;

void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int sock{};
    vector<char> message(BUF_SIZE);
    int str_len{}, recv_len{}, recv_cnt{};
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

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("connect() error!");
    } else {
        cout << "Connected............." << endl;
    }

    while (true) {
        cout << "Input message(Q to quit): ";
        string input{};
        getline(cin, input);

        if (input == "q" || input == "Q") {
            break;
        }

        str_len = write(sock, input.c_str(), input.size());

        recv_len = 0;
        while (recv_len < str_len) {
            recv_cnt = read(sock, message.data(), BUF_SIZE - 1);
            if (recv_cnt == -1) {
                error_handling("read() error!");
            }
            recv_len += recv_cnt;
        }

        string received(message.data(), str_len);
        cout << "Message from server: " << received << endl;
    }
    close(sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

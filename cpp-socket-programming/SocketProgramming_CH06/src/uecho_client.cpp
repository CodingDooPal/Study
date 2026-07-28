#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 30
void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int sock{};
    string message{BUF_SIZE, 0};
    int str_len{};
    socklen_t adr_sz{};

    struct sockaddr_in serv_adr{}, from_adr{};
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }

    // 18행: UDP 소켓을 생성한다.
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    // memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    while (true) {
        cout << "Insert message(q to quit): ";
        getline(cin, message);
        if(!message.compare("q") || !message.compare("Q")) {
            break;
        }

        // 34행: 서버로 데이터를 전송한다.
        sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
        adr_sz = sizeof(from_adr);
        // 37행: 서버로부터 데이터를 수신한다.
        str_len = recvfrom(sock, message.data(), BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);
        //message[str_len] = 0;
        cout << "Message from server: " << message << endl;
    }

    close(sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

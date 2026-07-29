#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 30
void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int sock{};
    string message{BUF_SIZE, 0};
    int str_len{};
    socklen_t adr_sz{}; // 불필요해진 변수

    struct sockaddr_in serv_adr{}, from_adr{}; // from_adr 불필요해짐
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    // memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));

    while (true) {
        cout << "Insert message(q to quit): ";
        getline(cin, message);
        if (!message.compare("q") || !message.compare("Q")) {
            break;
        }

        /*
        sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr*)&serv_adr,
        sizeof(serv_adr));
        */
        write(sock, message.c_str(), message.size());

        /*
        adr_sz = sizeof(from_adr);
        str_len =
            recvfrom(sock, message.data(), BUF_SIZE, 0, (struct sockaddr *)&from_adr, &adr_sz);
        */
        str_len = read(sock, message.data(), sizeof(message) - 1);
        
        // message[str_len] = 0;
        cout << "Message from server: " << message << endl;
    }

    close(sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

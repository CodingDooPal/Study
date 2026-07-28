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
    struct sockaddr_in my_adr{}, your_adr{};
    socklen_t adr_sz{};
    int str_len{}, i{};

    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    // memset(&my_adr, 0, sizeof(my_adr));
    my_adr.sin_family = AF_INET;
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_adr.sin_port = htons(atoi(argv[1]));

    if (bind(sock, (struct sockaddr *)&my_adr, sizeof(my_adr)) == -1) {
        error_handling("bind() error");
    }

    for (i = 0; i < 3; ++i) {
        sleep(5);
        adr_sz = sizeof(your_adr);
        str_len =
            recvfrom(sock, message.data(), BUF_SIZE, 0, (struct sockaddr *)&your_adr, &adr_sz);
        message.resize(str_len); // 실제 받은 길이로 크기를 맞춘다

        cout << "Message " << i + 1 << ": " << message << endl;

        message.resize(BUF_SIZE); // 다음 recvfrom을 위해 다시 원래 버퍼 크기로 복구
    }

    close(sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

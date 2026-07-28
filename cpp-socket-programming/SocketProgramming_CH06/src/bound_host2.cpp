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
    string msg1{"Hi!"};
    string msg2{"I'm another UDP host!"};
    string msg3{"Nice to meet you"};

    struct sockaddr_in your_adr{};
    socklen_t your_adr_sz{};
    if (argc != 3) {
        cout << "Usage :" << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
        error_handling("socket() error");
    }

    //memset(&your_adr, 0, sizeof(your_adr));
    your_adr.sin_family = AF_INET;
    your_adr.sin_addr.s_addr = inet_addr(argv[1]);
    your_adr.sin_port = htons(atoi(argv[2]));

    sendto(sock, msg1.c_str(), msg1.size(), 0, (struct sockaddr*)&your_adr, sizeof(your_adr));
    sendto(sock, msg2.c_str(), msg2.size(), 0, (struct sockaddr*)&your_adr, sizeof(your_adr));
    sendto(sock, msg3.c_str(), msg3.size(), 0, (struct sockaddr*)&your_adr, sizeof(your_adr));
    close(sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

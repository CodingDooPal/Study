#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
using namespace std;

void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int tcp_sock{}, udp_sock{};
    int sock_type{};
    socklen_t optlen{};
    int state{};

    optlen = sizeof(sock_type);
    // 15, 16행: TCP, UDP 소켓을 각각 생성
    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    // 17, 18행: TCP, UDP 소켓 생성시 인자로 전달하는 SOCK_STREAM, SOCK_DGRAM의 상수 값을 출력
    cout << "SOCK_STREAM: " << SOCK_STREAM << endl;
    cout << "SOCK_DGRAM: " << SOCK_DGRAM << endl;

    // 20, 25행: 소켓의 타입정보를 얻는다.
    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state) {
        error_handling("getsockopt() error!");
    }
    cout << "Socket type one: " << sock_type << endl;

    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state) {
        error_handling("getsockopt() error!");
    }
    cout << "Socket type two: " << sock_type << endl;

    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

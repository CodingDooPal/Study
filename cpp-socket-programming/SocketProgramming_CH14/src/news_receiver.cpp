#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 30
void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int recv_sock{};
    int str_len{};
    char buf[BUF_SIZE]{};
    struct sockaddr_in adr{};
    struct ip_mreq join_adr{};
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <Group IP> <PORT>" << endl;
        exit(1);
    }

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(atoi(argv[2]));

    if (bind(recv_sock, (struct sockaddr *)&adr, sizeof(adr)) == -1) {
        error_handling("bind() error");
    }

    // 26, 27행: 구조체 ip_merg형 변수를 초기화하고 있다. 멀티캐스트 그룹의 IP주소는 26행에서,
    // 그룹에 가입할 호스트의 IP 주소는 27행에서 초기화 된다.
    join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY);

    // 29행: 소켓옵션 IP_ADD_MEMBERSHIP을 이용해서 멀티캐스트 그룹에 가입하고 있다. 이제 26행에서
    // 지정한 멀티캐스트 그룹으로 전송되는 데이터를 수신할 준비는 모두 끝났다.
    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&join_adr, sizeof(join_adr));

    while (true) {
        // 34행: recvfrom 함수호출을 통해서 멀티캐스트 데이터를 수신하고 있다. 참고로 데이터를
        // 전송한 호스트의 주소정보를 알 필요가 없다면, 이렇듯 recvfrom 함수의 다섯 번째와 여섯 번째
        // 인자로 각각 NULL과 0을 전달하면 된다.
        str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0);
        if (str_len < 0) {
            break;
        }
        buf[str_len] = 0;
        cout << buf << endl;
    }

    close(recv_sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

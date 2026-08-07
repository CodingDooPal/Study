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
    int acpt_sock{}, recv_sock{};
    struct sockaddr_in acpt_adr{}, recv_adr{};
    int str_len{}, state{};
    socklen_t recv_adr_sz{};
    char buf[BUF_SIZE]{};
    if (argc != 2) {
        cout << "Usage : " << argv[0] << " <port>" << endl;
        exit(1);
    }

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&acpt_adr, 0, sizeof(acpt_adr));
    acpt_adr.sin_family = AF_INET;
    acpt_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_adr.sin_port = htons(atoi(argv[1]));

    if (bind(acpt_sock, (struct sockaddr *)&acpt_adr, sizeof(acpt_adr)) == -1) {
        error_handling("bind() error");
    }
    listen(acpt_sock, 5);

    recv_adr_sz = sizeof(recv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr *)&recv_adr, &recv_adr_sz);

    while (true) {
        // 38행: recv 함수를 호출하면서 MSG_PEEK을 옵션으로 전달하고 있다.
        // MSG_DONTWAIT 옵션을 함께 전달한 이유는 데이터가 존재하지 않아도 블로킹 상태에 두지 않기
        // 위해서다.
        str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK | MSG_DONTWAIT);
        if (str_len > 0) {
            break;
        }
    }

    buf[str_len] = 0;
    cout << "Buffering " << str_len << " bytes: " << buf << endl;

    // 46행: recv 함수를 한 번 더 호출하고 있다. 이번에 아무런 옵션도 설정하지 않았다. 때문에 이번에
    // 읽어 들인 데이터는 입력버퍼에서 지워진다.
    str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0);
    buf[str_len] = 0;
    cout << "Read again: " << buf << endl;
    close(acpt_sock);
    close(recv_sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

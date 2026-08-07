#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 30
void error_handling(string_view message);
void urg_handler(int signo);

int acpt_sock{};
int recv_sock{};

int main(int argc, char *argv[]) {
    struct sockaddr_in recv_adr{}, serv_adr{};
    int str_len{}, state{};
    socklen_t serv_adr_sz{};
    struct sigaction act{};
    char buf[BUF_SIZE]{};
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    // 29, 47행: 이 예제에서는 시그널 SIGURG와 관련된 부분을 주의 깊게 봐야 한다. MSG_OOB의 긴급 메시지를 수신하게 되면,
    // 운영체제는 SIGURG 시그널을 발생시켜서 프로세스가 등록한 시그널 핸들러가 호출되게 한다. 특히 61행에 정의되어 있는 핸들러 함수
    // 내부에서는 긴급 메시지의 수신을 위한 recv 함수의 호출문장도 삽입되어 있음에 주목하자.
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(acpt_sock, (struct sockaddr *)&recv_adr, sizeof(recv_adr)) == -1) {
        error_handling("bind() error");
    }
    listen(acpt_sock, 5);

    serv_adr_sz = sizeof(serv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr *)&serv_adr, &serv_adr_sz);

    // 46행: fcntl 함수가 호출되고 있다.
    fcntl(recv_sock, F_SETOWN, getpid());
    state = sigaction(SIGURG, &act, 0);

    while ((str_len = recv(recv_sock, buf, sizeof(buf), 0)) != 0) {
        if (str_len == -1) {
            continue;
        }
        buf[str_len] = 0;
        cout << buf << endl;
    }

    close(recv_sock);
    close(acpt_sock);
    return 0;
}

void urg_handler(int signo) {
    int str_len{};
    char buf[BUF_SIZE]{};
    str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
    buf[str_len] = 0;
    cout << "Urgent message: " << buf << endl;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

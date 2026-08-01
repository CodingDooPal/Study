#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int sock{};
    int snd_buf = 1024 * 3, rcv_buf = 1024 * 3;
    int state{};
    socklen_t len{};

    // 12, 16행: 입력버퍼와 출력버퍼의 크기를 각각 3K Byte로 변경한다.
    sock = socket(PF_INET, SOCK_STREAM, 0);
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));
    if(state) {
        error_handling("setsockopt() error");
    }

    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));
    if(state) {
        error_handling("setsockopt() error");
    }

    // 21, 26행: 입출력 버퍼의 변경요청에 따른 결과를 확인하기 위해서 입출력 버퍼의 크기를 참조하고 있다.
    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if(state) {
        error_handling("getsockopt() error");
    }

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if(state) {
        error_handling("getsockopt() error");
    }

    cout << "Input buffer size: " << rcv_buf << endl;
    cout << "Output buffer size: " << snd_buf << endl;
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
using namespace std;

void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int sock{};
    int snd_buf{}, rcv_buf{}, state{};
    socklen_t len{};

    sock = socket(PF_INET, SOCK_STREAM, 0);
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

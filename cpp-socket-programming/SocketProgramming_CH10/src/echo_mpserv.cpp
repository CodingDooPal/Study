#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUF_SIZE 30
void error_handling(string_view message);
void read_childproc(int sig);

int main(int argc, char *argv[]) {
    int serv_sock{}, clnt_sock{};
    struct sockaddr_in serv_adr{}, clnt_adr{};

    pid_t pid{};
    struct sigaction act{};
    socklen_t adr_sz{};
    int str_len{}, state{};
    char buf[BUF_SIZE]{};
    if(argc != 2) {
        cout << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    // 29~32행: 좀비 프로세스의 생성을 막기 위한 코드 구성
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind() error");
    }
    if(listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    while(1) {
        adr_sz = sizeof(clnt_adr);
        // 47, 52행: 47행에서 accept 함수를 호출한 이후에 52행에서 fork 함수를 호출
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock == -1) {
            continue;
        }
        else {
            cout << "new client connected..." << endl;
        }
        pid = fork();
        if(pid == -1) {
            close(clnt_sock);
            continue;
        }
        // 58~66행: 자식 프로세스에 의해 실행되는 영역이다. 이 부분에 의해서 클라이언트에게 에코 서비스가 제공된다.
        if(pid == 0) { // 자식 프로세스 실행 영역
            close(serv_sock);
            while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) {
                write(clnt_sock, buf, str_len);
            }
            close(clnt_sock);
            cout << "Client disconnected..." << endl;
            return 0;
        }
        else {
           // 69행: 47행의 accept 함수호출을 통해서 만들어진 소켓의 파일 디스크립터가 자식 프로세스에게 복사되었으니, 
           // 서버는 자신이 소유하고 있는 파일 디스크립터를 소멸 
            close(clnt_sock);
        }
    }

    close(serv_sock);
    return 0;
}

void read_childproc(int sig) {
    pid_t pid{};
    int status{};
    pid = waitpid(-1, &status, WNOHANG);
    cout << "removed proc id: " << pid << endl;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

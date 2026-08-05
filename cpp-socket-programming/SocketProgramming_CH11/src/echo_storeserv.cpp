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

#define BUF_SIZE 100
void error_handling(string_view message);
void read_childproc(int sig);

int main(int argc, char *argv[]) {
    int serv_sock{}, clnt_sock{};
    struct sockaddr_in serv_adr{}, clnt_adr{};
    int fds[2]{};

    pid_t pid{};
    struct sigaction act{};
    socklen_t adr_sz{};
    int str_len{}, state{};
    char buf[BUF_SIZE]{};
    if(argc != 2) {
        cout << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

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
    
    // 38, 39행: 38행에서 파이프를 생성하고 39행에서는 파일의 데이터 저장을 담당할 프로세스를 생성하고 있다.
    pipe(fds);
    pid = fork();
    // 40~53행: 39행에서 생성한 자식 프로세스에 의해 실행되는 영역
    if(pid == 0) {
        FILE* fp = fopen("echomsg.txt" , "wt");
        char msgbuf[BUF_SIZE];
        int i{}, len{};

        for(i = 0; i < 10; ++i) {
            len = read(fds[0], msgbuf, BUF_SIZE);
            fwrite((void*)msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }

    while(1) {
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock == -1) {
            continue;
        }
        else {
            cout << "new client connected..." << endl;
        }
        pid = fork();
        /*
        if(pid == -1) {
            close(clnt_sock);
            continue;
        }
        */
        if(pid == 0) {
            close(serv_sock);
            while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) {
                write(clnt_sock, buf, str_len);
                // 71행: 64행의 fork 함수호출로 생성되는 모든 자식 프로세스는 38행에서 생성한 파이프의 파일 디스크립터를 복사
                write(fds[1], buf, str_len);
            }
            close(clnt_sock);
            cout << "Client disconnected..." << endl;
            return 0;
        }
        else {
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

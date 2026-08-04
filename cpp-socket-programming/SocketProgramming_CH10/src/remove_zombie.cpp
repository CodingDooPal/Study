#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

void read_childproc(int sig) {
    int status{};
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        cout << "Removed proc id: " << id << endl;
        cout << "Child send: " << WEXITSTATUS(status) << endl;
    }
}

int main(int argc, char *argv[]) {
    pid_t pid{};
    // 21~25행: 시그널 SIGCHLD에 대한 시그널 핸들러의 등록과정을 보이고 있다.
    // 자식 프로세스가 종료되면 7행에 정의된 함수가 호출된다.
    struct sigaction act{};
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    // 27, 37행: 부모 프로세스를 통해서 총 두 개의 자식 프로세스를 생성
    pid = fork();
    if (pid == 0) { // 자식 프로세스 실행영역
        cout << "Hi! I'm child process" << endl;
        sleep(10);
        return 12;
    } 
    else { // 부모 프로세스 실행영역
        cout << "Child proc id: " << pid << endl;
        pid = fork();
        if (pid == 0) { // 또 다른 자식 프로세스 실행영역
            cout << "Hi! I'm child process" << endl;
            sleep(10);
            exit(24);
        }
        else {
            int i{};
            cout << "Child proc id: " << pid << endl;
            // 48, 51행: 시그널 SIGCHLD의 발생을 대기하기 위해 부모 프로세스를 5초간 5회 멈춤
            for(i = 0; i < 5; ++i) {
                cout << "wait..." << endl;
                sleep(5);
            }
        }
    }

    return 0;
}

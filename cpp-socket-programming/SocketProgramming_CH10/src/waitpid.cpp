#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    int status{};
    pid_t pid = fork();

    if(pid == 0) {
        // 12행: 자식 프로세스의 종료를 늦추기 위해서 sleep 함수를 호출
        sleep(15);
        return 24;
    }
    else {
        // 17행: while문 내에서 waitpid 함수를 호출, 종료된 자식 프로세스가 없으면 0을 반환
        while(!waitpid(-1, &status, WNOHANG)) {
            sleep(3);
            cout << "sleep 3sec." << endl;
        }

        if(WIFEXITED(status)) {
            cout << "Child send " << WEXITSTATUS(status) << endl;
        }
    }

    return 0;
}
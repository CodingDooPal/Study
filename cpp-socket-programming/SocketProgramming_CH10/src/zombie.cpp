#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]) {
    pid_t pid = fork();

    if(pid == 0) { // if Child Process
        cout << "Hi, I am a child process" << endl;
    }
    else {
        // 14행: 자식 프로세스의 ID를 출력
        cout << "Child Process ID: " << pid << endl;
        // 15행: 30초간 부모 프로세스를 멈추기 위한 코드
        sleep(30);
    }

    if(pid == 0) {
        cout << "End child process" << endl;
    }
    else {
        cout << "End parent process" << endl;
    }

    return 0;
}
#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    int status{};
    // 9, 13행: 9행에서 생성된 자식 프로세스는 13행에서 보이듯이 return문 실행을 통해서 종료
    pid_t pid = fork();

    if (pid == 0) {
        return 3;
    }
    else {
        cout << "Child PID: " << pid << endl;
        // 18, 21행: 18행에서 생성된 자식 프로세스는 21행에서 보이듯이 exit 함수호출을 통해서 종료
        pid = fork();
        if (pid == 0) {
            exit(7);
        }
        else {
            cout << "Child PID: " << pid << endl;
            // 26행: 종료된 프로세스 관련 정보는 status에 담기게 되고, 해당 정보의 프로세스는 완전히 소멸
            wait(&status);
            // 27, 28행: 매크로 함수 WIFEXITED를 통해서 자식 프로세스의 정상종료 여부를 확인
            // 정상종료일 경우 WEXITSTATUS 함수를 호출하여 자식 프로세스가 전달한 값을 출력
            if(WIFEXITED(status)) {
                cout << "Child send ONE: " << WEXITSTATUS(status) << endl;
            }

            // 30~32행: 자식 프로세스가 두 개이므로 한 번 더 wait 함수호출과 매크로 함수의 호출을 진행
            wait(&status);
            if(WIFEXITED(status)) {
                cout << "Child send ONE: " << WEXITSTATUS(status) << endl;
            }
            // 33행: 부모 프로세스의 종료를 멈추기 위해서 삽입한 코드
            sleep(30); // Sleep 30 sec.
        }
    }

    return 0;
}
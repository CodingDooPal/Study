#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

void timeout(int sig) {
    if (sig == SIGALRM) {
        cout << "Time out" << endl;
    }
    alarm(2);
}

int main(int argc, char* argv[]) {
    int i{};
    // 15, 16행: 시그널 발생시 호출될 함수의 등록을 위해서 sigaction 구조체 변수를 선언
    // 구조체 멤버 sa_handler에 함수 포인터 값을 저장
    struct sigaction act{};
    act.sa_handler = timeout;
    // 17행: sigemptyset 함수를 사용하여 sigaction 구조체의 멤버 sa_mask의 모든 비트를 0으로 초기화
    sigemptyset(&act.sa_mask);
    // 18행: sa_flags 멤버도 모든 비트를 0으로 초기화
    act.sa_flags = 0;
    // 19, 21행: 시그널 SIGALRM에 대한 핸들러를 지정하고, alarm 함수호출을 통해서 2초 뒤에
    // 시그널 SIGALRM의 발생을 예약
    sigaction(SIGALRM, &act, 0);

    alarm(5);

    for (i = 0; i < 3; ++i){
        cout << "wait..." << endl;
        sleep(100);
    }

    return 0;
}

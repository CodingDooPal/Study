#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

// 5, 11행: 시그널이 발생했을 때 호출되어야 할 함수가 각각 정의
void timeout(int sig) {
    if (sig == SIGALRM) {
        cout << "Time out!" << endl;
    }
    // 9행: 2초 간격으로 SIGALRM 시그널을 반복 발생시키기 위해 alarm 함수를 호출
    alarm(2);
}

void keycontrol(int sig) {
    if(sig == SIGINT) {
        cout << "CTRL+C pressed" << endl;
    }
}

int main(int argc, char* argv[]) {
    int i{};
    // 20, 21행: 시그널 SIGALRM, SIGINT에 대한 시그널 핸들러를 등록
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    // 22행: 시그널 SIGALRM의 발생을 2초 뒤로 예약
    alarm(2);
    
    for(i = 0; i < 3; ++i) {
        cout << "wait..." << endl;
        // 27행: 시그널의 발생과 시그널 핸들러의 실행을 확인하기 위해서 100초간 총 3회의 대기시간을 갖도록 한다.
        sleep(100);
    }

    return 0;
}
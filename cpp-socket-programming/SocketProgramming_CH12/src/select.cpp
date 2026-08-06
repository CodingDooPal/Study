#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

using namespace std;

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
    fd_set reads{}, temps{};
    int result{}, str_len{};
    char buf[BUF_SIZE]{};
    struct timeval timeout{};

    // 14, 15행: 14행에서 fd_set형 변수를 초기화
    // 15행에서 파일 디스크립터 0의 위치를 1로 설정
    FD_ZERO(&reads);
    FD_SET(0, &reads); // 0 is standard input(console)

    /*
    // 18, 19행: 이이는 select 함수의 타임아웃 설정을 위한 코드이다. 그런데 이 위치에서 설정하면 안 된다.
    // select 함수호출 후에는 구조체 timeval의 멤버 tv_sec와 tv_usec에 저장된 값이 타임아웃이 발생하기까지 남았던 시간이 바뀌기 때문이다.
    // 따라서 select 함수를 호출하기 전에 매번 timeval 구조체 변수의 초기화를 반복해야 한다.
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;
    */

    while(true) {
        // 24행: 미리 준비해 둔 fd_set형 변수 reads의 내용을 변수 temps에 복사
        // select 함수호출이 끝나면 변화가 생긴 파일 디스크립터의 위치를 제외한 나머지 위치의 비트들을 0으로 초기화 된다.
        // 따라서 원본 유지를 위해서 복사 과정을 거쳐야 한다.
        temps = reads;
        // 25, 26행: timeval 구조체 변수의 초기화 코드를 반복문 안에 삽입해서 select 함수가 호출되기 전에 매번 새롭게 값이 초기화되도록 한다.
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        // 27행: 콘솔로부터 입력된 데이터가 있다면 0보다 큰 수가 반횐되며, 입력된 데이터가 없어서 타임아웃이 발생하는 경우에는 0이 반환된다.
        result = select(1, &temps, 0, 0, &timeout);
        if(result == -1) {
            cout << "select() error!" << endl;
            break;
        }
        else if (result == 0) {
            cout << "Time out!" << endl;
        }
        else {
            // 39~44행: select 함수가 0보다 큰 수를 반환했을 때 실행되는 영역이다.
            // 변화를 보인 파일 디스크립터가 표준입력이 맞는지 확인하고, 맞으면 표준입력으로부터 데이터를 읽어서 콘솔로 데이터를 출력한다.
            if(FD_ISSET(0, &temps)) {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                cout << "message from console: " << buf << endl;
            }
        }
    }

    return 0;
}
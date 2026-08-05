#include <iostream>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
    int fds1[2]{}, fds2[2]{};
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZE]{};
    pid_t pid{};

    // 13행: 두 개의 파이프를 생성
    pipe(fds1), pipe(fds2);
    pid = fork();
    if(pid == 0) {
        // 17, 23행: 자식 프로세스에서 부모 프로세스로의 데이터 전송은 배열 fds1이 참조하는 파이프를 사용
        write(fds1[1], str1, sizeof(str1));
        // 18, 25행: 부모 프로세스에서 자식 프로세스로의 데이터 전송은 배열 fds2가 참조하는 파이프를 사용
        read(fds2[0], buf, BUF_SIZE);
        cout << "Child proc output: " << buf << endl;
    }
    else {
        read(fds1[0], buf, BUF_SIZE);
        cout << "Parent proc output: " << buf << endl;
        write(fds2[1], str2, sizeof(str2));
        // 26행: 큰 의미 없다. 다만 부모 프로세스의 종료를 지연시키기 위해 삽입했다.
        sleep(3);
    }

    return 0;
}
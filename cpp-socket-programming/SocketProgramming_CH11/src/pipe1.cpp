#include <iostream>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
    int fds[2]{};
    char str[] = "Who are you?";
    char buf[BUF_SIZE]{};
    pid_t pid{};

    // 12행: pipe 함수호출을 통해서 파이프를 생성
    pipe(fds);
    // 13행: fork 함수를 호출하여 자식 프로세스 생성
    // 12행의 함수호출을 통해서 얻게 된 두 개의 파일 디스크립터를 함께 소유
    pid = fork();
    if (pid == 0) {
        // 16, 20행: 자식 프로세스는 16행의 실행을 통해서 파이프로 문자열을 전달
        // 부모 프로세스는 20행의 실행을 통해서 파이프로부터 문자열을 수신
        write(fds[1], str, sizeof(str));
    } 
    else {
        read(fds[0], buf, BUF_SIZE);
        cout << buf << endl;
    }

    return 0;
}
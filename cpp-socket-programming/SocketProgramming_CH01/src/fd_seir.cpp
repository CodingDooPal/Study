#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

using namespace std;

int main(void)
{
    int fd1, fd2, fd3;
    // 9~11행 하나의 파일과 두 개의 소켓을 생성
    fd1 = socket(PF_INET, SOCK_STREAM, 0);
    fd2 = open("test.dat", O_CREAT|O_WRONLY|O_TRUNC);
    fd3 = socket(PF_INET, SOCK_DGRAM, 0);

    // 13~15행: 파일 디스크립터 정수 값을 출력
    cout << "file descriptor 1: " << fd1 << endl;
    cout << "file descriptor 2: " << fd2 << endl;
    cout << "file descriptor 3: " << fd3 << endl;

    close(fd1); close(fd2); close(fd3);
    return 0;
}

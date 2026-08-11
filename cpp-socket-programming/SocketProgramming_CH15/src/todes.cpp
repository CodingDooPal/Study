#include <fcntl.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    FILE* fp{nullptr};
    int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);
    if(fd == -1) {
        fputs("file open error", stdout);
        return -1;
    }

    // 14행: 7행에서 반환된 파일 디스크립터의 정수 값을 출력하고 있다.
    cout << "First file descriptor: " << fd << endl;
    // 15, 17행: 15행에서는 fdopen 함수호출을 통해서 파일 디스크립터를 FILE 포인터로
    // 17행에서는 fileno 함수호출을 통해서 이를 다시 파일 디스크립터로 변환하였다. 그리고 이 정수 값을 출력하고 있다.
    fp = fdopen(fd, "w");
    fputs("TCP/IP SOCKET PROGRAMMING\n", fp);
    cout << "Second file descriptor: " << fileno(fp) << endl;
    fclose(fp);
    return 0;
}

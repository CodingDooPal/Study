#include <iostream>
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 100

using namespace std;

void error_handling(string_view message);

int main(void)
{
    int fd;
    char buf[BUF_SIZE];

    // 13행 open() data.txt를 읽기 전용으로 열기
    fd = open("data.txt", O_RDONLY);
    if (fd == -1) {
        error_handling("open() error!");
    }
    cout << "file descriptor: " << fd << endl;

    // 18행 read() buf에 fd로 읽어 들인 데이터를 저장
    if (read(fd, buf, sizeof(buf)) == -1) {
        error_handling("read() error!");
    }
    cout << "file data: " << buf << endl;
    close(fd);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

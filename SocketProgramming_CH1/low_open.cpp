#include <iostream>
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

void error_handling(string_view message);

int main(void)
{
    int fd;
    char buf[] = "Let's go!\n";

    // 12행 open() data.txt 오픈
    fd = open("data.txt", O_CREAT|O_WRONLY|O_TRUNC);
    if (fd == -1) {
        error_handling("open() error!");
    }
    cout << "file descriptor: " << fd << endl;

    // 17행 write() fd에 저장된 파일에 buf에 저장된 데이터 전송
    if(write(fd, buf, sizeof(buf)) == -1) {
        error_handling("write() error!");
    }    
    close(fd);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

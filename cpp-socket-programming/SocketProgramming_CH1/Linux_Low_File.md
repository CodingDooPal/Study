## 리눅스 기반의 저 수준 파일 입출력 예제

### 1) 파일에 데이터 쓰기
```cpp
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
```
· 12행: 파일 오픈 모드가 O_CREAT, O_WRONLY, 그리고 O_TRUNC의 조합이니, 아무것도 저장되어있지 않은 
새로운 파일이 생성되어 쓰기만 가능하게 된다. 물론 이미 data.txt라는 이름의 파일이 존재한다면, 
이 파일의 데이터는 모두 지워져 버린다.  
· 17행: fd에 저장된 파일 디스크립터에 해당하는 파일에 buf에 저장된 데이터를 전송하고 있다.  

### 2) 파일에 저장된 데이터 읽기
```cpp
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
```
· 13행: 파일 data.txt를 읽기 전용으로 열고 있다.  
· 18행: read 함수를 이용해서 11행에 선언된 배열 buf에 읽어 들인 데이터를 저장하고 있다.  

### 3) 파일 디스크립터와 소켓
```cpp
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
```
· 9~11행: 하나의 파일과 두 개의 소켓을 생성하고 있다.  
· 13~15행: 앞서 생성한 파일 디스크립터의 정수 값을 출력하고 있다.  

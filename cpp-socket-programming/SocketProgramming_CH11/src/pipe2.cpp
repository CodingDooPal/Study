#include <iostream>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
    int fds[2]{};
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZE]{};
    pid_t pid{};

    pipe(fds);
    pid = fork();
    if (pid == 0) {
        // 17~20행: 자식 프로세스의 실행영역이다.
        write(fds[1], str1, sizeof(str1));
        sleep(2);
        read(fds[0], buf, BUF_SIZE);
        cout << "Child proc output " << buf << endl;
    }
    else { 
        // 24~26행: 부모 프로세스의 실행영역이다.
        read(fds[0], buf, BUF_SIZE);
        cout << "Parent proc output " << buf << endl;
        write(fds[1], str2, sizeof(str2));
        // 27행: 부모 프로세스가 먼저 종료되면 명령 프롬프트가 떠버린다. 그래도 자식 프로세스는 자신의 일을 한다.
        // 이 문장은 자식 프로세스가 끝나기 전에 명령 프롬프트가 뜨는 어색한 상황을 여러분에게 보이지 않기 위한 문장이다.
        sleep(3);
    }

    return 0;
}
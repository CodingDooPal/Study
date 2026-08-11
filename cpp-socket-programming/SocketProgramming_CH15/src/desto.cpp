#include <fcntl.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    FILE *fp{nullptr};
    // 7행: open 함수를 사용해서 파일을 생성했으므로 파일 디스크립터가 반환된다.
    int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1) {
        fputs("file open error", stdout);
        return -1;
    }

    // 14행: fdopen 함수호출을 통해서 파일 디스크립터를 FILE 포인터로 변환하고 있다. 이때 두 번째
    // 인자로 "w"가 전달되었으니, 출력모드 FILE 포인터가 반환된다.
    fp = fdopen(fd, "w");
    // 15행: 14행을 통해서 얻은 포인터를 기반으로 표준출력 함수인 fputs 함수를 호출하고 있다.
    fputs("Network C programming \n", fp);
    // 16행: FILE 포인터를 이용해서 파일을 닫고 있다. 이 경우 파일자체가 완전히 종료되기 때문에 파일
    // 디스크립를 이용해서 또 다시 종료할 필요는 없다. 뿐만 아니라, fclose ㅎ마수호출 이후부터는
    // 파일 디스크립터도 의미 없는 정수에 지나지 않는다.
    fclose(fp);
    return 0;
}

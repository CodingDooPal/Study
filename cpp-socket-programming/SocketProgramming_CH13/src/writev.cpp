#include <iostream>
#include <sys/uio.h>

using namespace std;

int main(int argc, char *argv[]) {
    struct iovec vec[2]{};
    char buf1[] = "ABCDEFG";
    char buf2[] = "1234567";
    int str_len{};

    // 11, 12행: 첫 번째로 전송할 데이터가 저장된 위치와 크기정보를 담고 있다.
    vec[0].iov_base = buf1;
    vec[0].iov_len = 3;
    // 13, 14행: 두 번째로 전송할 데이터가 저당된 위치와 크기정보를 담고 있다.
    vec[1].iov_base = buf2;
    vec[1].iov_len = 4;

    // 16행: writev 함수의 첫 번째 전달인자가 1이므로 콘솔로 출력이 이뤄진다.
    str_len = writev(1, vec, 2);
    cout << "" << endl;
    cout << "Write bytes: " << str_len << endl;
    return 0;
}

#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define BUF_SIZE 1024
// 3, 4행 피연산자의 바이트 수와 연산결과의 바이트 수를 상수화
#define RLT_SIZE 4
#define OPSZ 4
using namespace std;

void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int sock{};
    // 데이터의 송수신을 위한 메모리 공간(배열 기반 누적)
    char opmsg[BUF_SIZE];
    int result{}, opnd_cnt{}, i{};
    struct sockaddr_in serv_adr{};

    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error!");
    }

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("connect() error!");
    } else {
        cout << "Connected..........." << endl;
    }

    cout << "Operand count: ";
    // 33, 34행 피연산자의 개수정보를 입력 받은 후 배열에 저장
    cin >> opnd_cnt;
    opmsg[0] = static_cast<char>(opnd_cnt);

    // 36~40행 정수를 입력 받아서 배열에 저장(int -> char로 형변환)
    for (i = 0; i < opnd_cnt; ++i) {
        cout << "Operand " << i + 1 << ": ";
        cin >> *reinterpret_cast<int*>(&opmsg[i * OPSZ + 1]);
    }

    cout << "Operator: ";
    // 43행 마지막으로 연산자 정보를 입력 받은 후 배열에 저장
    cin >> &opmsg[opnd_cnt * OPSZ + 1];
    // 44행 write 함수호출을 통해서 opmsg에 저장되어 있는 연산과 관련된 정보를 한번에 전송
    write(sock, opmsg, opnd_cnt * OPSZ + 2);
    // 45행 서버가 전송해주는 연산결과의 저장
    read(sock, &result, RLT_SIZE);

    cout << "Operation result: " << result << endl;
    close(sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

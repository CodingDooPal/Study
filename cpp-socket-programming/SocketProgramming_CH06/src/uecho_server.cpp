#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 30
void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int serv_sock{};
    char message[BUF_SIZE]{};
    int str_len{};
    socklen_t clnt_adr_sz{};

    struct sockaddr_in serv_adr{}, clnt_adr{};
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    // 24행: UDP 소켓의 생성을 위해서 socket 함수의 두 번째 인자로 SOCK_DGRAM을 전달하고 있다.
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (serv_sock == -1) {
        error_handling("UDP socket creation error");
    }

    // memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind() error");
    }

    while (true) {
        clnt_adr_sz = sizeof(clnt_adr);
        // 39행: 33행에 할당된 주소로 전달되는 모든 데이터를 수신하고 있다.
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        // 41행: 39행의 함수호출을 통해 얻은 주소정보를 이용해서 수신된 데이터를 역으로 재전송한다.
        sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
    }

    // 44행: 37행의 while문이 무한루프이고, 이 루프를 빠져나가기 위한 break문이 삽입되지 않았기 때문에 사실상 실행되지는 않는다.
    close(serv_sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

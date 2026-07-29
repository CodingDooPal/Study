#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 30
void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int serv_sd{}, clnt_sd{};
    FILE *fp{};
    char buf[BUF_SIZE]{};
    int read_cnt{};

    struct sockaddr_in serv_adr{}, clnt_adr{};
    socklen_t clnt_adr_sz{};

    if (argc != 2) {
        cout << "Using: " << argv[0] << "<port>" << endl;
        exit(1);
    }

    // 26행: 서버의 소스파일인 file_server.cpp를 클라이언트에게 전송하기 위해서 파일을 열고 있다.
    fp = fopen("file_server.cpp", "rb");
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    bind(serv_sd, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
    listen(serv_sd, 5);

    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sd = accept(serv_sd, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);

    // 40~49행: 38행의 accept 함수호출을 통해서 연결된 클라이언트에게 파일 데이터를 전송하기 위한 반복문
    while (true) {
        read_cnt = fread((void *)buf, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE) {
            write(clnt_sd, buf, read_cnt);
            break;
        }
        write(clnt_sd, buf, BUF_SIZE);
    }

    // 51행: 파일전송 후에 출력 스트림에 대한 Half-close를 진행한다.
    shutdown(clnt_sd, SHUT_WR);
    // 52행: 출력 스트림만 닫았기 때문에 입력 스트림을 통한 데이터의 수신은 여전히 가능하다.
    read(clnt_sd, buf, BUF_SIZE);
    cout << "Message from client: " << buf << endl;

    fclose(fp);
    close(clnt_sd);
    close(serv_sd);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

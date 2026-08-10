#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define TTL 64
#define BUF_SIZE 30
void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int send_sock{};
    struct sockaddr_in mul_adr{};
    int time_live = TTL;
    FILE *fp{};
    char buf[BUF_SIZE]{};
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <GroupIP> <PORT>" << endl;
        exit(1);
    }

    // 24행: 멀티캐스트 데이터의 송수신은 UDP 소켓을 대상으로 하기 때문에, 이 문장에서는 UDP 소켓을
    // 생성하고 있다.
    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&mul_adr, 0, sizeof(mul_adr));
    // 26~28행: 데이터를 전송할 주소정보를 설정하고 있다. 중요한 것은 반드시 IP주소를 멀티캐스트
    // 주소로 설정해야 한다는 사실이다.
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]); // Multicast IP
    mul_adr.sin_port = htons(atoi(argv[2]));      // Multicast Port

    // 30행: 소켓의 TTL 정보를 저장하고 있다. Sender에서 반드시 해야 할 일이다.
    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&time_live, sizeof(time_live));
    if ((fp = fopen("news.txt", "r")) == NULL) {
        error_handling("fopen() error");
    }

    // 35~41행: 실제 데이터 전송이 이뤄지는 영역이다. UDP 소켓을 기반으로 데이터 전송이 이뤄지므로
    // sendto 함수를 사용하는 것은 당연하다. 그리고 40행의 sleep 함수호출은 데이터 전송에 약간의
    // 시간간격을 두기 위함일 뿐, 별 다른 의미를 갖진 않는다.
    while (!feof(fp)) { // Broadcasting
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr *)&mul_adr, sizeof(mul_adr));
        sleep(2);
    }

    fclose(fp);
    close(send_sock);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

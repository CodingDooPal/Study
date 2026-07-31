#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <string>
using namespace std;

void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int i{};
    struct hostent *host{nullptr};
    if(argc != 2){
        cout << "Usage: " << argv[0] << " <addr>" << endl;
        exit(1);
    }

    // 17행: main 함수를 통해서 전달된 문자열을 인자로 gethostbyname 함수를 호출
    host = gethostbyname(argv[1]);
    if(!host) {
        error_handling("gethost... error");
    }

    // 21행: 공식 도메인 이름을 출력
    cout << "Official name: " << host->h_name << endl;
    // 22~23행: 공식 도메인 이름 이외의 도메인 이름을 출력
    for(i = 0; host->h_aliases[i]; ++i) {
        cout << "Aliases: " << i + 1 << ' ' << host->h_aliases[i] << endl;
    }
    // 26~28행: IP 주소정보를 출력
    cout << "Address type: " << (host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6") << endl;
    for(i = 0; host->h_addr_list[i]; ++i) {
        cout << "IP addr: " << i + 1 << ' ' << inet_ntoa(*(struct in_addr*) host->h_addr_list[i]) << endl;
    }

    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

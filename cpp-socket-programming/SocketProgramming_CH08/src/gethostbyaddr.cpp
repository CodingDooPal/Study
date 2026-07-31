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
    struct sockaddr_in addr{};
    if (argc != 2) {
        cout << "Usaged: " << argv[0] << " <IP>" << endl;
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    host = gethostbyaddr((char *)&addr.sin_addr, 4, AF_INET);
    if (!host) {
        error_handling("gethost... error");
    }

    cout << "Official name: " << host->h_name << endl;
    for(i = 0; host->h_aliases[i]; ++i) {
        cout << "Aliases: " << i + 1 << ' ' << host->h_aliases[i] << endl;
    }
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

#include <arpa/inet.h>
#include <iostream>
#include <string.h>
using namespace std;

void error_handling(string_view message);


int main(int argc, char *argv[]) {
    string addr{"127.232.124.79"};
    struct sockaddr_in addr_inet;

    if(!inet_aton(addr.c_str(), &addr_inet.sin_addr)){
        error_handling("Conversion error");
    }
    else {
        cout << "Network ordered integer addr: 0x" << uppercase << hex 
        << addr_inet.sin_addr.s_addr << endl;
    }

    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}

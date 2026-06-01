#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char *argv[]) {
    struct sockaddr_in addr1{}, addr2{};
    char* str_ptr{nullptr};
    string str_arr{};

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101);

    str_ptr = inet_ntoa(addr1.sin_addr);
    str_arr = str_ptr;
    cout << "Dotted-Decimal notation1: " << str_ptr << endl; 

    inet_ntoa(addr2.sin_addr);
    cout << "Dotted-Decimal notation2: " << str_ptr << endl; 
    cout << "Dotted-Decimal notation3: " << str_arr << endl; 

    return 0;
}

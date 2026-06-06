#include <arpa/inet.h>
#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char *argv[]) {
    const char *addr1 = "1.2.3.4";
    const char *addr2 = "1.2.3.256";

    unsigned long conv_addr = inet_addr(addr1);
    if (conv_addr == INADDR_NONE) {
        cout << "Error occured!" << endl;
    } else {
        cout << "Network ordered integer addr: 0x" << uppercase << hex << conv_addr << endl;
    }

    conv_addr = inet_addr(addr2);
    if (conv_addr == INADDR_NONE) {
        cout << "Error occured!" << endl;
    } else {
        cout << "Network ordered integer addr: 0x" << uppercase << hex << conv_addr << endl;
    }

    return 0;
}

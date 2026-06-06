#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#undef UNICODE
#undef _UNICODE
#include <iostream>
#include <string>
#include <winsock2.h>
using namespace std;

int main()
{
    string strAddr{ "203.211.218.102:9190" };

    string strAddrBuf{};
    SOCKADDR_IN servAddr{};
    int strToAddrSize{};
    DWORD addrToStrSize{};

    WSADATA wsaData{};
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    strToAddrSize = sizeof(servAddr);
    WSAStringToAddress(
        (LPSTR)strAddr.c_str(), AF_INET, NULL, (SOCKADDR*)&servAddr, &strToAddrSize
    );

    strAddrBuf.resize(50);
    addrToStrSize = strAddrBuf.size();
    WSAAddressToString(
        (SOCKADDR*)&servAddr, sizeof(servAddr), NULL, (LPSTR)strAddrBuf.data(), &addrToStrSize
    );

    cout << "Second conv result: " << strAddrBuf << endl;

    WSACleanup();
    return 0;
}

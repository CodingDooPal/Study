#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <winsock2.h>
using namespace std;
void ErrorHandling(string_view message);

int main(int argc, char* argv[])
{
	WSADATA wsaData{};
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	/* inet_addr 함수의 호출 예 */
	{
		string addr = "127.212.124.78";
		unsigned long conv_addr = inet_addr(addr.c_str());
		if (conv_addr == INADDR_NONE) {
			cout << "Error occured!" << endl;
		}
		else {
			cout << "Network ordered integer addr: 0x" << uppercase << hex << conv_addr << endl;
		}
	}

	/* inet_ntoa 함수의 호출 예 */
	{
		struct sockaddr_in addr {};
		char* strPtr{ nullptr };
		string strArr{};

		addr.sin_addr.s_addr = htonl(0x1020304);
		strPtr = inet_ntoa(addr.sin_addr);
		strArr = strPtr;
		cout << "Dotted-Decimal notation3 " << strArr << endl;
	}

	WSACleanup();
	return 0;
}

void ErrorHandling(string_view message) {
	cout << message << endl;
	exit(1);
}

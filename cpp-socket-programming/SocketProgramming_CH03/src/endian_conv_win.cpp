#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <winsock2.h>
using namespace std;
void ErrorHandling(string_view message);

int main(int argc, char* argv[])
{
	WSADATA wsaData{};
	unsigned short host_port = 0x1234;
	unsigned short net_port{};
	unsigned long host_addr = 0x12345678;
	unsigned long net_addr{};

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	cout << "Host ordered port: 0x" << uppercase << hex << host_port << endl;
	cout << "Network ordered port: 0x" << uppercase << hex << net_port << endl;
	cout << "Host ordered Address: 0x" << uppercase << hex << host_addr << endl;
	cout << "Network ordered Address: 0x" << uppercase << hex << net_addr << endl;
	WSACleanup();
	return 0;
}

void ErrorHandling(string_view message) {
	cout << message << endl;
	exit(1);
}

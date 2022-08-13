#include <iostream>
#include <thread>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

using namespace std;

void SendMsg(SOCKET sock) {
	while (true) {
		char sbuff[100] = { 0 };
		cin.getline(sbuff, 100);
		send(sock, sbuff, 100, 0);
	}
}

void RecvMsg(SOCKET sock) {
	while (true) {
		char rbuff[115] = { 0 };
		recv(sock, rbuff, 115, 0);
		cout << rbuff << endl;
	}
}

int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return -1;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR) {
		WSACleanup();
		return -1;
	}
	
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9090);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (connect(sock, (sockaddr*)&addr, sizeof(addr)) != 0) {
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	char sbuff[10] = { 0 };
	cout << "닉네임을 입력해 주세요 : ";
	cin.getline(sbuff, 10);
	send(sock, sbuff, 10, 0);
	

	cout << "\n채팅 서버 접속 완료!\n" << endl;
	thread t1(RecvMsg, sock);
	thread t2(SendMsg, sock);

	t1.join();
	t2.join();
	closesocket(sock);
	WSACleanup();
	return 0;
}

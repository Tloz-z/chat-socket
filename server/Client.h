#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

class Client {
	private:
		SOCKET mSock;
		char mName[10];
		bool isInit;
		bool isQuit;

	public:
		Client(SOCKET sock);
		~Client();

		void SetName(char* name);
		void Quit();

		SOCKET GetSocket();
		char* GetName();
		bool IsInit();
		bool IsQuit();
};
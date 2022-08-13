#include "Client.h"

Client::Client(SOCKET sock)
	: mSock(sock)
	, isInit(false)
	, isQuit(false)
{
}

Client::~Client()
{
}

void Client::SetName(char* name) {
	int i = 0;
	while (name[i] != '\0' && i < 9) {
		mName[i] = name[i];
		i++;
	}
	mName[i] = '\0';
	isInit = true;
}

void Client::Quit() {
	closesocket(mSock);
	isQuit = true;
}

SOCKET Client::GetSocket() {
	return mSock;
}

char* Client::GetName() {
	return mName;
}

bool Client::IsInit() {
	return isInit;
}

bool Client::IsQuit() {
	return isQuit;
}

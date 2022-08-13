#include "Client.h"

using namespace std;

void FormatChat(char* data, const char* name, const char* chat) {
	*data++ = '[';
	while (*name != '\0') {
		*data++ = *name++;
	}
	strcpy(data, "] : ");
	data += 4;
	strcpy(data, chat);
}

void FormatChatGreeting(char* data, const char* name, bool isVisit) {
	while (*name != '\0') {
		*data++ = *name++;
	}
	if (isVisit) {
		strcpy(data, "¥‘¿Ã ¡¢º”«ﬂΩ¿¥œ¥Ÿ.");
	}
	else {
		strcpy(data, "¥‘¿Ã ≥™∞¨Ω¿¥œ¥Ÿ.");
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
	addr.sin_addr.S_un.S_addr = ADDR_ANY;
	if (bind(sock, (sockaddr*)&addr, sizeof(addr)) != 0) {
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	listen(sock, 5);

	fd_set recv_fds;
	sockaddr_in accept_addr;
	int accept_addr_size;
	char chat_data[100];
	char send_data[115];

	vector<Client> client_list;
	client_list.reserve(5);

	vector<Client> temp_list;
	temp_list.reserve(5);

	cout <<
	"   _______      _______      ______        __   __      _______      ______\n  |       |    |       |    |    _ |      |  | |  |    |       |    |    _ |\n  |  _____|    |    ___|    |   | ||      |  |_|  |    |    ___|    |   | ||\n  | |_____     |   |___     |   |_||_     |       |    |   |___     |   |_||_\n  |_____  |    |    ___|    |    __  |    |       |    |    ___|    |    __  |\n   _____| |    |   |___     |   |  | |     |     |     |   |___     |   |  | |\n  |_______|    |_______|    |___|  |_|      |___|      |_______|    |___|  |_|\n"
	<< endl;

	while (true) {
		FD_ZERO(&recv_fds);
		FD_SET(sock, &recv_fds);
		for (int i = 0; i < client_list.size(); ++i) {
			FD_SET(client_list[i].GetSocket(), &recv_fds);
		}

		select(0, &recv_fds, 0, 0, 0);

		if (FD_ISSET(sock, &recv_fds)) {
			accept_addr_size = sizeof(accept_addr);
			SOCKET csock = accept(sock, (sockaddr*)&accept_addr, &accept_addr_size);
			if (csock != SOCKET_ERROR) {
				client_list.push_back(Client(csock));
			}
		}

		vector<Client>::iterator iter;
		for (iter = client_list.begin(); iter != client_list.end(); ++iter) {
			if (FD_ISSET(iter->GetSocket(), &recv_fds) == false) {
				continue;
			}
			memset(chat_data, 0, sizeof(chat_data));
			recv(iter->GetSocket(), chat_data, sizeof(chat_data), 0);

			memset(send_data, 0, sizeof(send_data));

			if (*chat_data == '\0') {
				iter->Quit();
				if (iter->IsInit() == false) {
					continue;
				}
				cout << iter->GetName() << " ¥‘¿Ã ≥™∞¨Ω¿¥œ¥Ÿ." << endl;
				FormatChatGreeting(send_data, iter->GetName(), false);
			} else if (iter->IsInit() == false) {
				iter->SetName(chat_data);
				cout << chat_data << " ¥‘¿Ã ¡¢º”«ﬂΩ¿¥œ¥Ÿ." << endl;
				FormatChatGreeting(send_data, chat_data, true);
			}
			else {
				FormatChat(send_data, iter->GetName(), chat_data);
			}

			for (int j = 0; j < client_list.size(); ++j) {
				if (iter->GetSocket() == client_list[j].GetSocket() || client_list[j].IsInit() == false || client_list[j].IsQuit() == true) {
					continue;
				}
				send(client_list[j].GetSocket(), send_data, sizeof(send_data), 0);
			}
		}

		for (int i = 0; i < client_list.size(); ++i) {
			if (client_list[i].IsQuit() == false) {
				temp_list.push_back(client_list[i]);
			}
		}
		client_list.clear();
		client_list = temp_list;
		temp_list.clear();
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}

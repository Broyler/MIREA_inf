#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void clear() {
#ifdef _WIN32
	system("cls");
#endif
#ifdef linux
	system("clear");
#endif
}

int main(void) {
	char lane[80];
	int ptr = 0;
	int space = 5;
	float timeInterval = 1;
	printf("Enter the ticker: ");
	scanf("%s", lane);
	int len = strlen(lane);

	for (;;) {
		for (int i = len - ptr + space; i < len; ++i) {
			printf("%c", lane[i]);
		}
		for (int i = 0; i < (ptr > space ? space : ptr); ++i) printf(" ");
		for (int i = 0; i < len - (ptr - space >= 0 ? ptr - space : 0); ++i) {
			printf("%c", lane[i]);
		}
        printf("\n");
		sleep(timeInterval);
		ptr++;
		if (ptr >= space + len) ptr = 0;
		clear();
	}

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear() {
#ifdef _WIN32
	system("cls");
#endif
#ifdef linux
	system("clear");
#endif
}

int isLetterGuessed(char letter, char guessed[26]) {
	for (int i = 0; i < 26; ++i) {
		if (guessed[i] == letter) return 1;
	}
	return 0;
}

int isLetterSecret(char letter, char word[80]) {
	for (int i = 0; i < 80; ++i) {
		if (word[i] == letter) return 1;
	}
	return 0;
}

void printWord(char word[80], char guessed[26], int wordLen) {
	int flag;
	for (int i = 0; i < wordLen; ++i) {
		flag = 0;
		for (int j = 0; j < 26; ++j) {
			if (word[i] == guessed[j]) {
				printf("%c", word[i]);
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			printf("_");
		}
		printf(" ");
	}
	printf("\n");
}

int main(void) {
	char word[80];
	int players;
	int currentPlayer = 1;
	char guess;
	char guessed[26];
	int guessedAmount = 0;
	printf("Input the secret word: ");
	scanf("%s", word);
	int wordLen = strlen(word);
	clear();
	printf("Enter the amount of players: ");
	scanf("%d", &players);

	for (;;) {
		printWord(word, guessed, wordLen);
		printf("\nPlayer %d, guess a letter: ", currentPlayer);
		scanf("%c", &guess);
		printf("GDX: %c\n", (int)guess);

		if (isLetterGuessed(guess, guessed) == 1) continue;
		guessed[guessedAmount] = guess;
		guessedAmount++;
		if (isLetterSecret(guess, word) == 0) {
			currentPlayer++;
			if (currentPlayer > players) currentPlayer = 1;
		}

	}

	return 0;
}
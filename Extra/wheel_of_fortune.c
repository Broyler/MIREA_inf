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

int printWord(char word[80], char guessed[26], int wordLen) {
	int correct = 0;
	int flag;
	for (int i = 0; i < wordLen; ++i) {
		flag = 0;
		for (int j = 0; j < 26; ++j) {
			if (word[i] == guessed[j]) {
				printf("%c", word[i]);
				correct++;
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
	return correct == wordLen ? 1 : 0;
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
		if (printWord(word, guessed, wordLen) == 1) {
			printf("Player %d wins! The word was %s.\n", currentPlayer, word);
			return 0;
		}
		printf("\nPlayer %d, guess a letter: ", currentPlayer);
		scanf("%c", &guess);
		if ((int)guess == 10) continue;

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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
	srand(time(NULL));

	printf("Guess the number!\n");
	int secretNumber = rand() % 100 + 1;
	int guess;
	int attempts = 10;

	for (;;) {
		printf("Pick a number between 1 and 100: ");
		scanf("%d", &guess);
		if (guess == secretNumber) break;
		else {
			attempts--;
			if (guess > secretNumber) printf("Too high!\n");
			else printf("Too low!\n");
			if (attempts <= 0) {
				printf("You lost.\nThe number was %d.\n", secretNumber);
				return 0;
			}
		}
	}

	printf("You won!\nThe number was %d\n", secretNumber);
	return 0;
}
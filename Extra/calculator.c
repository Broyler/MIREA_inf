#include <stdio.h>
#include <math.h>

void infoMsg() {
	printf("Basic C calculator. Supported operations:"
		"\n + Addition\n - Subtraction\n / Division\n * Multiplication\n"
		" ^ Power\n % Modulo\n ? Help\n q Quit\n");
}

float calcf(float a, float b, char operation) {
	switch (operation) {
	case '+':
		return a + b;

	case '-':
		return a - b;

	case '/':
		return a / b;

	case '*':
		return a * b;

	case '^':
		return pow(a, b);

	case '%':
		return fmod(a, b);
	}
	printf("Unsupported operation!\n");
	return 0;
}

int main(void) {
	infoMsg();
	float a;
	float b;
	char op;

	for (;;) {
		printf("Enter number a: ");
		scanf("%f", &a);
		printf("Enter number b: ");
		scanf("%f", &b);
		printf("Select operation: ");
		scanf("%s", &op);

		if (op == 'q') break;
		else if (op == '?') infoMsg();
		else printf("The result is: %f\n", calcf(a, b, op));
	}

	printf("Thanks for using this calculator! :)\n");

	return 0;
}

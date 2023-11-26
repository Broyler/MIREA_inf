#include <stdio.h>

int main() {
	int cats, penguins;
	printf("Enter the amount of cats to display: ");
	scanf("%d", &cats);
	printf("Enter the amount of penguins to display: ");
	scanf("%d", &penguins);

	for (int p = 0; p < penguins; ++p) {
		printf(" (o o)  ");
	}
	for (int c = 0; c < cats; ++c) {
		printf("  ^_^   ");
	}
	printf("\n");

	for (int p = 0; p < penguins; ++p) {
		printf(" / V \\  ");
	}
	for (int c = 0; c < cats; ++c) {
		printf(" /@ @\\  ");
	}
	printf("\n");

	for (int p = 0; p < penguins; ++p) {
		printf("/( _ )\\ ");
	}
	for (int c = 0; c < cats; ++c) {
		printf("(~~0~~) ");
	}
	printf("\n");

	for (int p = 0; p < penguins; ++p) {
		printf(" ^^ ^^  ");
	}

	return 0;
}
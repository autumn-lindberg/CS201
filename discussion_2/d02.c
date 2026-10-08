#include <stdio.h>
#include <stdlib.h>

// TWO'S COMPLEMENT PROGRAM
//
// this program takes in an integer and walks the user through
// how the two's complement of it is calculated. two's complement
// is used in the computer's hardware to represent negative numbers.

// print the number bit by bit
void print_bits(int num) {
	// 8 bits per byte
	// go from 31 to 0
	for (int i = (8 * sizeof(int)) - 1; i >= 0; i--) {
		printf("%d", (num >> i) & 1 );
		// print a space every 4 bits
		if (i % 4 == 0) printf(" ");
	}
	printf("\n");
}

int flip_bits(int num) {
	for (int i = 0; i < 8 * sizeof(int); i++) {
		num ^= (1 << i);
	}
	return num;
}

// get input from user
int get_int(char * message) {
	int input;
	// returns 0 if invalid input
	printf("%s", message);
	while (scanf("%d", &input) == 0) {
		// absorb until newline
		while (getchar() != '\n') {}
		printf("Invalid input. try again: ");
	}
	return input;
}

int main() {
	int num;

	printf("Welcome to my 2's complement program!\n\n");
	num = get_int("Enter an integer to start: ");

	printf("\nYour number in binary: ");
	print_bits(num);
	printf("\nNow let's flip all the bits!\n\n");
	num = flip_bits(num);
	printf("Your number flipped: ");
	print_bits(num);
	printf("\nNow it's time to add 1\n\n");
	num++;
	printf("Your number after adding 1: ");
	print_bits(num);
	printf("\n...and it's decimal value is %d !\n", num);
	
	// zero means "no notes"
	return 0;
}

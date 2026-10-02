#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int MAX_LENGTH = 100;

struct Rect {
	long width;
	long height;
};

int main() {

	int n;             // %d
	long l;            // %ld
	char c;            // %c
	unsigned int i;    // %u
	float f;           // %f
	double d;          // %lf
	int * p = &n;      // %x (hex address)

	char * s = malloc(sizeof(char) * MAX_LENGTH);          // %s

	// sizes
	printf("The size of an int is: %ld\n", sizeof(n));
	printf("The size of a long is: %ld\n", sizeof(l));
	printf("The size of a char is: %ld\n", sizeof(c));
	printf("The size of a float is: %ld\n", sizeof(f));
	printf("The size of a double is: %ld\n", sizeof(d));
	printf("The size of an int * is: %ld\n", sizeof(p));
	printf("The size of a Rect is: %ld\n", sizeof(struct Rect));
	printf("\n");
	printf("The highest possible integer (using macro MAX_INT):");
	printf("%d\n", INT_MAX);
	printf("The lowest possible integer is: %d\n", INT_MIN);
	printf("INT_MAX + 1 = %d (overflows to negative due to leading 1)\n", INT_MAX + 1);
	printf("UINT_MAX + 1 = %d (ignores leading 1, rest of bits are 0)\n", UINT_MAX + 1);

	// SCANNING / PRINTING
	//
	// scanf takes in the address which to store
	printf("Enter an integer: ");
	while (scanf("%d", &n) ==  0) {
		getchar();
		printf("Invalid input. Enter an integer: ");
	}
	printf("You entered %d\n", n);
	// clear newline from buffer
	getchar();

	// for words, use match case to read until new line ( %[^\n] )
	// address not needed for strings
	printf("Enter your name: ");
	scanf("%[^\n]", s);
	getchar();
	printf("you entered %s\n", s);

	// MALLOC
	//
	// malloc always returns a pointer, needs an explicit size
	int * num = malloc(sizeof(int));
	int * arr = malloc(10 * sizeof(int));

	*num = 10;
	
	// reset pointer to 0 (an invalid address)
	// after freeing, c uses it for other purposes,
	// and *n would give a bogus value
	num = 0;
	free(0);
}

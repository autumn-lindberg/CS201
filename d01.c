#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// DOT MATRIX BIT FLIPPING PROGRAM
// 
// this program reads in a file of comma and semicolon separated touples 
// places them in an array, and uses them as coordinates for which bits to flip
//
// y
//
// 0000000000000000000000000000000000000000  nums[0]
// 0000000000000000000000000000000000000000  nums[1]
// 0000000000000000000000000000000000000000	 nums[2]	
// 0000000000000000000000000000000000000000  ...
// 0000000000000000000000000000000000000000
// 0000000000000000000000000000000000000000
// 0000000000000000000000000000000000000000
// 0000000000000000000000000000000000000000
// 0000000000000000000000000000000000000000
// 0000000000000000000000000000000000000000  x

#define LETTER_WIDTH 18
#define DISPLAY_WIDTH 32
#define DISPLAY_HEIGHT 10

// https://stackoverflow.com/questions/74993010/how-to-create-an-array-of-arrays-in-c
// example uses two arrays holding up to 10 items each
// this program uses a variable length array (*big_arr)
// each with 2 items [2]
// without (), it evaluates to int * (big_arr[2])

// this function returns a pointer to the first element of touple array
int (*read_line(char * filename))[2] {
	int (*big_arr)[2];
	int (*big_arr_copy)[2];
	// 1 char for null terminator
	int chars = 1;
	int little_arr[2];
	int val;
	int touples = 1;
	char current;
	char * stream;
	char * copy;
	FILE * fptr;

	// open file in read mode
	fptr = fopen(filename, "r");	
	if (fptr == NULL) {
		printf("unable to open file\n");
		return 1;
	}
	printf("reading file...\n\n");
	// start instantiated
	stream = malloc(chars);
	// two ints to each touple
	big_arr = malloc(touples * sizeof(*little_arr));

	while((current = fgetc(fptr)) != EOF) {
		// if it's not a semicolon, keep reading
		if(current != ';') {
			// if it's not a comma, keep reading
			if(current != ',') {
				
				// create a bigger copy string
				copy = malloc(chars + 1);
				// put contents of stream into it
				strcpy(copy, stream);
				// add to end of copy string
				copy[chars] = "\0";
				copy[chars - 1] = current;
				// clear and copy back into stream
				free(stream);
				stream = malloc(chars + 1);
				strcpy(stream, copy);
				// clear copy for next use
				free(copy);
				chars++;
			// else it's a comma, add first part to touple and reset stream
			} else {	
				little_arr[0] = atoi(stream);
				//free(stream);
				chars = 1;
			}
		// else it's a semicolon, add to second part of touple
		// then add touple to big array,
		// then reset stream
		} else {
			little_arr[1] = atoi(stream);
			// add touple to big array 
			// put current into a copy
			// fixed seg fault by just giving it a shitload of memory
			big_arr_copy = malloc((touples + 1) * sizeof(*big_arr_copy));
			for (int i = 0; i < touples; i++) {
				big_arr_copy[i][0] = big_arr[i][0];
				big_arr_copy[i][1] = big_arr[i][1];
			}
			// add to end of array
			big_arr_copy[touples - 1][0] = little_arr[0];
			big_arr_copy[touples - 1][1] = little_arr[1];

			touples++;
			// copy back into array
			big_arr = malloc(touples * sizeof(int) * 2);
			for (int i = 0; i < touples; i++) {
				big_arr[i][0] = big_arr_copy[i][0];
				big_arr[i][1] = big_arr_copy[i][1];
			}
			//free(stream);
			chars = 1;
		}
	}

	fclose(fptr);
	return big_arr;
}

void print_bits(int * nums) {

	printf("\n");
	for (int y = 0; y < 10; y++) {
		printf("  ");
		for (int x = 31; x >=0; x--) {
			// shift to get to the right place, & 1 prints either 1 or 0 (T/F)
			printf("%d", (nums[y] >> x) & 1);
		}
		printf("\n");
	}
	printf("\n\n");
}

void flip_bit(int * nums, int x, int y) {
	// shift the 1 to proper place
	// xor flips just that bit
	nums[10 - y] = nums[10 - y] ^ (1 << (32 - x));
}

void mask_letter(int * nums, int (*array)[2]) {
	int i = 0;
	while(array[i][0] != 0 && array[i][1] != 0) {
		flip_bit(nums, array[i][0], array[i][1]);
		i++;
	}
}

void reset_bits(int * nums) {
	for (int y = 0; y < 10; y++) {
		nums[y] = 0;
	}
}

void slide_letter(int * nums) {
	struct timespec remaining, request = { 0, 100000000 };
	int numsCopy[10]; 

	// right shift all nums by 32 (off the screen)
	for (int x = 31; x >= 0; x--) {
		for (int y = 10; y >= 0; y--) {
			numsCopy[y] = nums[y] >> x;
		}
		print_bits(numsCopy);
    nanosleep(&request, &remaining);
		// sleep(1);
		system("clear");
	}
	// shift left off the screen
	for (int x = 0; x < LETTER_WIDTH + 6; x++) {
		for (int y = 10; y >= 0; y--) {
			numsCopy[y] = nums[y] << x;
		}
		print_bits(numsCopy);
    nanosleep(&request, &remaining);
		// sleep(1);
		system("clear");
	}
}

int main() {

	int nums[10]; 
	for (int y = 0; y < 10; y++) {
		nums[y] = 0;
	}

	int (*coords_a)[2] = read_line("a");
	int (*coords_b)[2] = read_line("b");
	int (*coords_c)[2] = read_line("c");

	mask_letter(&nums, coords_a);
	slide_letter(&nums);
	reset_bits(nums);

	mask_letter(&nums, coords_b); 
	slide_letter(&nums);
	reset_bits(nums);

	mask_letter(&nums, coords_c);
	slide_letter(&nums);
	reset_bits(nums);
}

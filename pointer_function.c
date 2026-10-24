#include <stdio.h>
#include <stdlib.h>

int mult(int n, int x) {
	return  n * x;
}

// function takes in two ints, and returns an int
void maparr(int arr[], int s, int (*func)(int, int)) {
	for (int i = 0; i < s; i++) {
		arr[i] = func(arr[i], 3);
	}
}

int main() {
	int arr[5] = {0, 1, 2, 3, 4};

	maparr(arr, 5, mult);

	for (int i = 0; i < 5; i++) {
		printf("%d\n", arr[i]);
	}

}

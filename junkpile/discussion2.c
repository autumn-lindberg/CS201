/*
 *
 * A computer stores an int as a fixed number of bits. Two's complement reads
 * those bits like normal binary, except the leftmost bit counts as NEGATIVE
 * (-2^31 in a 32-bit int). Example with 4 bits: 1011 = -8 + 2 + 1 = -5.
 *
 * To negate a number: flip every bit, then add 1.  (0101 -> 1010 -> 1011)
 *
 * Read as unsigned, the leftmost bit counts as positive instead, so a negative
 * numbers unsigned value is its signed value + 2^32.  (-1 -> 4294967295)
 *
 * Try: 0, 1, -1, 5, -5, 127, -128, 2147483647, -2147483648
 */

#include <limits.h>
#include <stdio.h>

#include "discussion2.h"

void print_twos_complement(int n)
{
    unsigned int bits = (unsigned int)n;   /* same bits, read as unsigned */
    int width = sizeof(int) * CHAR_BIT;

    printf("bits:     ");
    for (int i = width - 1; i >= 0; i--) {
        printf("%u", (bits >> i) & 1u);
        if (i % 4 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\nunsigned: %u\n", bits);
}

int main(void)
{
    int n;

    printf("Enter an int: ");
    if (scanf("%d", &n) != 1) {
        printf("That isn't an int.\n");
        return 1;
    }

    print_twos_complement(n);
    return 0;
}

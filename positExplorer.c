/*
 * Explore the generalized posit<8,3> teaching format from positTutorial.org.
 * This is NOT the 2022 standard posit8 format. It decodes bit patterns;
 * it does not implement posit arithmetic or rounding to a posit.
 *
 * Build: cc -std=c11 -Wall -Wextra -Wpedantic positExplorer.c -lm -o positExplorer
 * Run:   ./positExplorer
 * Or:    ./positExplorer 01000000 01000001
 * Table: ./positExplorer --table
 */
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* All finite values in this format are exact in a binary double with
 * at least three significant bits and exponents covering 2^-48 to 2^48. */
_Static_assert(FLT_RADIX == 2 && DBL_MANT_DIG >= 3 &&
               DBL_MIN_EXP <= -47 && DBL_MAX_EXP >= 49,
               "This explorer needs a suitable binary double type.");

enum { ES = 3 };

typedef struct {
    unsigned int raw, magnitude;
    int negative, zero, nar;
    int k, e;
    int regime_bits, exponent_bits, fraction_bits;
    double mantissa, value;
} Posit;

static int getBit(unsigned int n, int p)
{
    return (int)((n >> p) & 1u); /* Call only with positions 0 through 7. */
}

/* p always identifies the first unread bit; -1 means none remain. */
static int regime(unsigned int n, int *p)
{
    int first = getBit(n, *p);
    int count = 0;
    while (*p >= 0 && getBit(n, *p) == first) {
        ++count;
        --*p;
    }
    if (*p >= 0) --*p; /* Consume the terminator, if there is one. */
    return first ? count - 1 : -count;
}

static int exponent(unsigned int n, int *p)
{
    int e = 0;
    int count = 0;
    while (*p >= 0 && count < ES) {
        e = (e << 1) | getBit(n, *p);
        --*p;
        ++count;
    }
    /* Available bits are the high bits; missing low bits are zero. */
    return e << (ES - count);
}

static double mantissa(unsigned int n, int *p)
{
    double m = 1.0;
    double weight = 0.5;
    while (*p >= 0) {
        m += getBit(n, *p) * weight;
        weight *= 0.5;
        --*p;
    }
    return m;
}

static Posit decode(unsigned int raw)
{
    Posit d = {0};
    d.raw = raw;
    d.zero = raw == 0;
    d.nar = raw == 128;
    if (d.zero || d.nar) return d;

    d.negative = getBit(raw, 7);
    /* Take the entire byte's two's complement for a negative value. */
    d.magnitude = d.negative ? (256u - raw) : raw;
    int p = 6;
    d.k = regime(d.magnitude, &p);
    d.regime_bits = 6 - p;
    int start = p;
    d.e = exponent(d.magnitude, &p);
    d.exponent_bits = start - p;
    d.fraction_bits = p + 1;
    d.mantissa = mantissa(d.magnitude, &p);

    /* useed = 2^(2^3) = 256, so 256^k * 2^e = 2^(8*k + e).
     * ldexp multiplies by that power of two directly. */
    d.value = ldexp(d.mantissa, 8 * d.k + d.e);
    if (d.negative) d.value = -d.value;
    return d;
}

static void bits(unsigned int n, char out[9])
{
    for (int p = 7; p >= 0; --p) out[7 - p] = (char)('0' + getBit(n, p));
    out[8] = '\0';
}

static void show(unsigned int raw)
{
    Posit d = decode(raw);
    char original[9], positive[9];
    bits(raw, original);
    printf("\nBits: %s (byte value %u)\n", original, raw);
    if (d.zero) {
        puts("Value: 0 (special encoding; no fields to decode)");
        return;
    }
    if (d.nar) {
        puts("Value: NaR (Not a Real; special encoding, not a number)");
        return;
    }
    bits(d.magnitude, positive);
    printf("Sign: %s\n", d.negative ? "negative" : "positive");
    if (d.negative)
        printf("Two's complement for decoding the magnitude: %s\n", positive);
    int offset = 1;
    printf("Magnitude fields: sign=0 | regime=%.*s", d.regime_bits, positive + offset);
    offset += d.regime_bits;
    if (d.exponent_bits)
        printf(" | exponent=%.*s", d.exponent_bits, positive + offset);
    else
        printf(" | exponent=(none)");
    offset += d.exponent_bits;
    if (d.fraction_bits)
        printf(" | fraction=%.*s\n", d.fraction_bits, positive + offset);
    else
        puts(" | fraction=(none)");
    printf("Regime k=%d (%d bits, including terminator when present)\n",
           d.k, d.regime_bits);
    printf("Exponent e=%d (%d stored bits; missing low bits are zero)\n",
           d.e, d.exponent_bits);
    printf("Mantissa=%.17g (%d fraction bits; includes the leading 1)\n",
           d.mantissa, d.fraction_bits);
    printf("Value: %s256^(%d) * 2^(%d) * %.17g = %.17g\n",
           d.negative ? "-" : "", d.k, d.e, d.mantissa, d.value);
}

/* Strictly accept eight binary digits. This input is a bit pattern,
 * not a decimal number that will be rounded into the format. */
static int parseBits(const char *text, unsigned int *value)
{
    if (strlen(text) != 8) return 0;
    unsigned int n = 0;
    for (int i = 0; i < 8; ++i) {
        if (text[i] != '0' && text[i] != '1') return 0;
        n = (n << 1) | (unsigned int)(text[i] - '0');
    }
    *value = n;
    return 1;
}

static void table(void)
{
    puts("bits\tbyte\tk\te\tregime_bits\texponent_bits\tfraction_bits\tmantissa\tvalue");
    for (unsigned int n = 0; n < 256; ++n) {
        Posit d = decode(n);
        char text[9];
        bits(n, text);
        if (d.zero || d.nar)
            printf("%s\t%u\t-\t-\t-\t-\t-\t-\t%s\n", text, n, d.nar ? "NaR" : "0");
        else
            printf("%s\t%u\t%d\t%d\t%d\t%d\t%d\t%.17g\t%.17g\n",
                   text, n, d.k, d.e, d.regime_bits, d.exponent_bits,
                   d.fraction_bits, d.mantissa, d.value);
    }
}

int main(int argc, char **argv)
{
    if (argc == 2 && strcmp(argv[1], "--table") == 0) {
        table();
        return 0;
    }
    if (argc > 1) {
        /* Validate the whole request before showing any results. */
        for (int i = 1; i < argc; ++i) {
            unsigned int n;
            if (!parseBits(argv[i], &n)) {
                fprintf(stderr, "Use eight binary digits per argument, or --table.\n");
                return 1;
            }
        }
        puts("Generalized posit<8,3> teaching format (not standard posit8)");
        for (int i = 1; i < argc; ++i) {
            unsigned int n = 0;
            (void)parseBits(argv[i], &n);
            show(n);
        }
        return 0;
    }

    unsigned int current = 64; /* 01000000 represents 1. */
    char line[128];
    puts("Generalized posit<8,3> teaching format (not standard posit8)");
    puts("Enter eight bits, flip N (N from 0 to 7), or q to quit.");
    puts("Bit 7 is the leftmost bit; bit 0 is the rightmost.");
    show(current);
    for (;;) {
        printf("\nEight bits / flip N / q: ");
        fflush(stdout);
        if (!fgets(line, sizeof line, stdin)) break;
        if (!strchr(line, '\n') && !feof(stdin)) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            puts("Input too long. Enter eight bits, flip N, or q.");
            continue;
        }
        char *start = line;
        while (isspace((unsigned char)*start)) ++start;
        size_t length = strlen(start);
        while (length && isspace((unsigned char)start[length - 1])) start[--length] = '\0';
        if (strcmp(start, "q") == 0) break;
        if (parseBits(start, &current)) {
            show(current);
        } else if (length == 6 && strncmp(start, "flip ", 5) == 0 &&
                   start[5] >= '0' && start[5] <= '7') {
            current ^= 1u << (start[5] - '0');
            show(current);
        } else {
            puts("Please enter exactly eight binary digits, flip N (0-7), or q.");
        }
    }
    return 0;
}

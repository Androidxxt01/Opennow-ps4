#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ps4_entropy.h"

static void print_hex(const char *label, const unsigned char *buf, size_t len) {
    printf("%s", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x", buf[i]);
    }
    printf("\n");
}

static int run_test(FILE *out) {
    unsigned char a[32];
    unsigned char b[32];
    int r1 = -999;
    int r2 = -999;
    int e1 = 0;
    int e2 = 0;
    int same;

    memset(a, 0, sizeof(a));
    memset(b, 0, sizeof(b));

    errno = 0;
    r1 = ps4_entropy_fill(a, sizeof(a));
    e1 = errno;
    errno = 0;
    r2 = ps4_entropy_fill(b, sizeof(b));
    e2 = errno;

    same = memcmp(a, b, sizeof(a)) == 0;

    fprintf(out, "ps4_entropy_fill r1=%d errno1=%d r2=%d errno2=%d\n", r1, e1, r2, e2);
    fprintf(out, "a=");
    for (size_t i = 0; i < sizeof(a); i++) fprintf(out, "%02x", a[i]);
    fprintf(out, "\n");
    fprintf(out, "b=");
    for (size_t i = 0; i < sizeof(b); i++) fprintf(out, "%02x", b[i]);
    fprintf(out, "\n");
    fprintf(out, "%s\n", same ? "IGUAIS" : "DIFERENTES");

    printf("ps4_entropy_fill r1=%d errno1=%d r2=%d errno2=%d\n", r1, e1, r2, e2);
    print_hex("a=", a, sizeof(a));
    print_hex("b=", b, sizeof(b));
    printf("%s\n", same ? "IGUAIS" : "DIFERENTES");

    return same ? 1 : 0;
}

int main(void) {
    FILE *out = fopen("/data/opennow-entropy.txt", "wb");
    int same = 0;
    if (out != NULL) {
        same = run_test(out);
        fclose(out);
    } else {
        same = run_test(stdout);
    }
    return 0;
}

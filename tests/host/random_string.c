#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Stubs: utils.c compila funções HMAC/MD5 que dependem de mbedtls, mas este
 * teste só exercita utils_random_string. */
typedef struct { int dummy; } mbedtls_md_context_t;
typedef int mbedtls_md_type_t;
typedef struct { int dummy; } mbedtls_md_info_t;
#define MBEDTLS_MD_SHA1 0
#define MBEDTLS_MD_MD5 0
void mbedtls_md_init(mbedtls_md_context_t* ctx) { (void)ctx; }
const mbedtls_md_info_t* mbedtls_md_info_from_type(mbedtls_md_type_t t) { (void)t; return (const mbedtls_md_info_t*)1; }
int mbedtls_md_setup(mbedtls_md_context_t* ctx, const mbedtls_md_info_t* info, int hmac) { (void)ctx; (void)info; (void)hmac; return 0; }
int mbedtls_md_hmac_starts(mbedtls_md_context_t* ctx, const unsigned char* key, size_t keylen) { (void)ctx; (void)key; (void)keylen; return 0; }
int mbedtls_md_hmac_update(mbedtls_md_context_t* ctx, const unsigned char* input, size_t ilen) { (void)ctx; (void)input; (void)ilen; return 0; }
int mbedtls_md_hmac_finish(mbedtls_md_context_t* ctx, unsigned char* output) { (void)ctx; (void)output; return 0; }
void mbedtls_md_free(mbedtls_md_context_t* ctx) { (void)ctx; }
int mbedtls_md_starts(mbedtls_md_context_t* ctx) { (void)ctx; return 0; }
int mbedtls_md_update(mbedtls_md_context_t* ctx, const unsigned char* input, size_t ilen) { (void)ctx; (void)input; (void)ilen; return 0; }
int mbedtls_md_finish(mbedtls_md_context_t* ctx, unsigned char* output) { (void)ctx; (void)output; return 0; }

void utils_random_string(char* s, int len);

int main(void) {
    enum { N = 1000, UFRAG_LEN = 4, UPWD_LEN = 24 };
    char ufrags[N][UFRAG_LEN + 1];
    char upwds[N][UPWD_LEN + 1];
    int prefix_hits = 0;
    int dup_pwds = 0;

    for (int i = 0; i < N; i++) {
        utils_random_string(ufrags[i], UFRAG_LEN);
        utils_random_string(upwds[i], UPWD_LEN);
        if (strncmp(upwds[i], ufrags[i], UFRAG_LEN) == 0) {
            prefix_hits++;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (strcmp(upwds[i], upwds[j]) == 0) {
                dup_pwds++;
            }
        }
    }

    printf("pairs=%d\n", N);
    printf("ufrag_prefix_of_upwd=%d\n", prefix_hits);
    printf("duplicate_upwd_pairs=%d\n", dup_pwds);
    printf("::notice::random_string pairs=%d ufrag_prefix_of_upwd=%d duplicate_upwd_pairs=%d\n", N, prefix_hits, dup_pwds);
    return 0;
}

/* Teste de host: CTR_DRBG sem semente, com a configuracao do build do Switch.
 * MBEDTLS_NO_PLATFORM_ENTROPY ligado, sem hardware alt, sem NULL_ENTROPY.
 */
#include <stdio.h>

#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>

#if !defined(MBEDTLS_NO_PLATFORM_ENTROPY)
#error "teste exige MBEDTLS_NO_PLATFORM_ENTROPY"
#endif
#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT) || \
    defined(MBEDTLS_ENTROPY_NV_SEED) || \
    defined(MBEDTLS_TEST_NULL_ENTROPY)
#error "config de teste nao pode ter fonte de entropia ativa"
#endif

static void print_hex(const unsigned char *buf, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        printf("%02x", buf[i]);
    }
}

int main(void)
{
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const unsigned char pers[] = "dtls_srtp";
    unsigned char out[32];
    int ret;

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                                pers, sizeof(pers) - 1);
    printf("seed=%d (-0x%04x)\n", ret, (unsigned int)-ret);

    /* Ignora o retorno do seed, como dtls_srtp.c:88 faz. */
    for (int i = 1; i <= 2; i++) {
        const int r = mbedtls_ctr_drbg_random(&ctr_drbg, out, sizeof(out));
        printf("random%d=%d ", i, r);
        print_hex(out, sizeof(out));
        printf("\n");
    }

    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    return 0;
}

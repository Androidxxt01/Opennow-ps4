#include <stdio.h>
#include <string.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
static void hex(const unsigned char *b, size_t n){for(size_t i=0;i<n;i++) printf("%02x",b[i]);}
int main(void){
  unsigned char out[32], out2[32];
  mbedtls_entropy_context e; mbedtls_ctr_drbg_context d;
  mbedtls_entropy_init(&e); mbedtls_ctr_drbg_init(&d);
  int ret = mbedtls_ctr_drbg_seed(&d, mbedtls_entropy_func, &e, (const unsigned char*)"t",1);
  int r = mbedtls_ctr_drbg_random(&d,out,sizeof(out));
  printf("seed=%d (-0x%04x) rand=%d ", ret, (unsigned int)(-ret), r); hex(out,32); printf("\n");
  mbedtls_entropy_context e2; mbedtls_ctr_drbg_context d2;
  mbedtls_entropy_init(&e2); mbedtls_ctr_drbg_init(&d2);
  int ret2 = mbedtls_ctr_drbg_seed(&d2, mbedtls_entropy_func, &e2, (const unsigned char*)"t",1);
  int r2 = mbedtls_ctr_drbg_random(&d2,out2,sizeof(out2));
  printf("seed2=%d rand2=%d ",ret2,r2); hex(out2,32); printf("\n");
  printf("%s\n", memcmp(out,out2,32)==0?"IGUAIS":"DIFERENTES");
  return 0;
}

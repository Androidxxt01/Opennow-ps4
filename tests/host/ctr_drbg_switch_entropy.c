#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>

void randomGet(void *buf, size_t len) {
  int fd = open("/dev/urandom", O_RDONLY);
  size_t off = 0;
  if (fd < 0) abort();
  while (off < len) {
    ssize_t n = read(fd, (unsigned char*)buf + off, len - off);
    if (n <= 0) { close(fd); abort(); }
    off += (size_t)n;
  }
  close(fd);
}
static void hex(const unsigned char *b, size_t n){for(size_t i=0;i<n;i++) printf("%02x",b[i]);}
int main(void){
  unsigned char a[32], b[32];
  mbedtls_entropy_context e1,e2; mbedtls_ctr_drbg_context d1,d2;
  mbedtls_entropy_init(&e1); mbedtls_ctr_drbg_init(&d1);
  mbedtls_entropy_init(&e2); mbedtls_ctr_drbg_init(&d2);
  int s1 = mbedtls_ctr_drbg_seed(&d1, mbedtls_entropy_func, &e1, (const unsigned char*)"t",1);
  int r1 = mbedtls_ctr_drbg_random(&d1,a,sizeof(a));
  int s2 = mbedtls_ctr_drbg_seed(&d2, mbedtls_entropy_func, &e2, (const unsigned char*)"t",1);
  int r2 = mbedtls_ctr_drbg_random(&d2,b,sizeof(b));
  printf("seed1=%d rand1=%d a=",s1,r1); hex(a,32); printf("\n");
  printf("seed2=%d rand2=%d b=",s2,r2); hex(b,32); printf("\n");
  printf("%s\n", memcmp(a,b,32)==0?"IGUAIS":"DIFERENTES");
  mbedtls_ctr_drbg_free(&d1); mbedtls_entropy_free(&e1);
  mbedtls_ctr_drbg_free(&d2); mbedtls_entropy_free(&e2);
  return (s1==0 && s2==0 && memcmp(a,b,32)!=0) ? 0 : 1;
}

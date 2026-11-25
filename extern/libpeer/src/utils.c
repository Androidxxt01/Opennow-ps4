#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mbedtls/md.h"

#if defined(__PS4__)
#include "ps4_entropy.h"
#elif defined(__SWITCH__)
#include <switch.h>
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#if defined(__linux__) || defined(__FreeBSD__)
#include <sys/random.h>
#endif
#endif

static int utils_secure_random(void* buf, size_t len) {
#if defined(__PS4__)
  return ps4_entropy_fill((unsigned char*)buf, len);
#elif defined(__SWITCH__)
  randomGet(buf, len);
  return 0;
#elif defined(__linux__) || defined(__FreeBSD__)
  size_t off = 0;
  while (off < len) {
    ssize_t n = getrandom((unsigned char*)buf + off, len - off, 0);
    if (n < 0 && errno == EINTR) continue;
    if (n <= 0) return -1;
    off += (size_t)n;
  }
  return 0;
#else
  int fd = open("/dev/urandom", O_RDONLY);
  size_t off = 0;
  if (fd < 0) return -1;
  while (off < len) {
    ssize_t n = read(fd, (unsigned char*)buf + off, len - off);
    if (n < 0 && errno == EINTR) continue;
    if (n <= 0) { close(fd); return -1; }
    off += (size_t)n;
  }
  close(fd);
  return 0;
#endif
}

void utils_random_string(char* s, const int len) {
  int i;
  unsigned char* rnd;

  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  const size_t n = sizeof(alphanum) - 1;

  if (len <= 0) {
    if (s) s[0] = '\0';
    return;
  }

  rnd = (unsigned char*)malloc((size_t)len);
  if (rnd == NULL) {
    abort();
  }
  if (utils_secure_random(rnd, (size_t)len) != 0) {
    free(rnd);
    abort();
  }

  for (i = 0; i < len; ++i) {
    s[i] = alphanum[rnd[i] % n];
  }
  free(rnd);

  s[len] = '\0';
}

void utils_get_hmac_sha1(const char* input, size_t input_len, const char* key, size_t key_len, unsigned char* output) {
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA1;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
  mbedtls_md_hmac_starts(&ctx, (const unsigned char*)key, key_len);
  mbedtls_md_hmac_update(&ctx, (const unsigned char*)input, input_len);
  mbedtls_md_hmac_finish(&ctx, output);
  mbedtls_md_free(&ctx);
}

void utils_get_md5(const char* input, size_t input_len, unsigned char* output) {
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_MD5;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char*)input, input_len);
  mbedtls_md_finish(&ctx, output);
  mbedtls_md_free(&ctx);
}

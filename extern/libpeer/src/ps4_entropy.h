#ifndef OPENNOW_PS4_ENTROPY_H
#define OPENNOW_PS4_ENTROPY_H

#include <stddef.h>

#if defined(__PS4__)
#ifdef __cplusplus
extern "C" {
#endif
int ps4_entropy_fill(unsigned char *buf, size_t len);
#ifdef __cplusplus
}
#endif
#endif

#endif

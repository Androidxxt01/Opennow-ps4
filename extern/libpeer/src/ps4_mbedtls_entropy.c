#include <stddef.h>

#if defined(__PS4__)
#include <stdlib.h>

#include "ps4_entropy.h"

int mbedtls_hardware_poll(void *data,
                          unsigned char *output,
                          size_t len,
                          size_t *olen);

int mbedtls_hardware_poll(void *data,
                          unsigned char *output,
                          size_t len,
                          size_t *olen)
{
	(void)data;
	if (olen == NULL) {
		abort();
	}
	*olen = 0;
	ps4_entropy_fill(output, len);
	*olen = len;
	return 0;
}
#else
typedef int opennow_ps4_mbedtls_entropy_empty_translation_unit;
#endif

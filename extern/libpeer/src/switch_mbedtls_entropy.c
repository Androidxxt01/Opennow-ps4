#include <stddef.h>

#if defined(__SWITCH__)
#include <stdlib.h>
#include <switch.h>

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
	if (olen == NULL || (output == NULL && len != 0)) {
		abort();
	}
	randomGet(output, len);
	*olen = len;
	return 0;
}
#else
typedef int opennow_switch_mbedtls_entropy_empty_translation_unit;
#endif

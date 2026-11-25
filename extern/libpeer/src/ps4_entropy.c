#include "ps4_entropy.h"

#if defined(__PS4__)
#include <errno.h>
#include <stdlib.h>
#include <sys/random.h>

/*
 * ATENCAO: a implementacao de getrandom() no fork musl do OpenOrbis NAO foi
 * verificada. A declaracao existe em sys/random.h, mas o musl original usa
 * syscall do Linux e o PS4 e FreeBSD. Isto precisa de teste em hardware.
 * Falha fechada: EINTR tenta de novo (no maximo 100 vezes); -1 por outro
 * motivo, 0 ou falta de progresso aborta o processo. Retorno parcial avanca o
 * buffer e continua. Nao ha fallback para rand(), time(), enderecos de memoria
 * ou /dev/urandom.
 */
int ps4_entropy_fill(unsigned char *buf, size_t len)
{
	size_t position = 0;
	int attempts = 0;

	if (buf == NULL && len != 0) {
		abort();
	}
	while (position < len) {
		const size_t remaining = len - position;
		ssize_t n;

		if (++attempts >= 100) {
			abort();
		}
		n = getrandom(buf + position, remaining, 0);
		if (n < 0 && errno == EINTR) {
			continue;
		}
		if (n <= 0) {
			abort();
		}
		position += (size_t)n;
	}
	return 0;
}
#else
typedef int opennow_ps4_entropy_empty_translation_unit;
#endif

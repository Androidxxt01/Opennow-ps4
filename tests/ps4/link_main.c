/* Main mínimo para o alvo link do ps4-check: força referências a símbolos
 * das quatro bibliotecas e deixa o linker listar o que falta. */
#include <cstdint>

extern "C" {
#include "ps4_entropy.h"
#include "usrsctp.h"
#include "peer_connection.h"
}

namespace opennow::gfn::detail
{
std::int64_t NowMs();
}

int main()
{
    unsigned char seed[32];

    ps4_entropy_fill(seed, sizeof(seed));
    usrsctp_init(0, nullptr, nullptr);
    (void)opennow::gfn::detail::NowMs();
    peer_connection_set_diagnostics_enabled(0);
    return 0;
}

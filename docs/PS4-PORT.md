# Porte PS4 — notas de validação

## TODO: teste em hardware

O ramo `__PS4__` de `user_environment.c` usa `getrandom(buf, size, 0)` declared
em `sys/random.h` do OpenOrbis. A declaração existe, mas a implementação do
fork musl do SDK **não foi verificada**: no musl original essa função é um
wrapper de syscall do Linux, enquanto o PS4 é FreeBSD. O código falha fechado
(`abort()`) em `-1`, `0` ou retorno parcial; não há fallback para `rand()`,
`time()`, endereços de memória ou `/dev/urandom`.

Critérios de aprovação, a validar em hardware PS4:

1. `getrandom(buf, n, 0)` retorna exatamente `n` para os tamanhos usados pelo
   usrsctp, sem retorno parcial.
2. Duas chamadas seguidas com o mesmo `n` produzem saídas diferentes.
3. A saída não é inteiramente zero.
4. `errno` fica limpo (valor `0` antes e depois da chamada).

Enquanto esses critérios não forem confirmados, a fonte de entropia do PS4
deve ser tratada como **não comprovada**.

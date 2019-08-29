#include "cipher_def.h"

void xor_buf(const uint8_t in[], uint8_t out[], size_t len)
{
    size_t idx;

    for (idx = 0; idx < len; idx++)
        out[idx] ^= in[idx];
}

#ifndef SHA1_H
#define SHA1_H

/*
   SHA-1 in C
   By Steve Reid <steve@edmweb.com>
   100% Public Domain
 */

#if _MSC_VER < 1600
typedef unsigned int uint32_t;
#else
#include "stdint.h"
#endif

typedef struct
{
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
    unsigned char digest[20];
} SHA1_CTX;

#ifdef __cplusplus
extern "C" {
#endif

void SHA1Init(SHA1_CTX* context);
void SHA1Update(SHA1_CTX* context, const unsigned char* data, uint32_t len);
void SHA1Final(SHA1_CTX* context);

#ifdef __cplusplus
};
#endif

#endif /* SHA1_H */

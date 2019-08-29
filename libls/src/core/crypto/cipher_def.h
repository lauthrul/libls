#ifndef TYPEDEF_H
#define TYPEDEF_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

#if _MSC_VER < 1600
typedef unsigned char   uint8_t;        // 8-bit byte
typedef int             int32_t;        // 32-bit word
typedef unsigned int    uint32_t;       // 32-bit unsigned word
#else
#include "stdint.h"
#endif

/****************************** MACROS ******************************/
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*********************** FUNCTION DEFINITIONS ***********************/
#ifdef __cplusplus
extern "C" {
#endif

    // XORs the in and out buffers, storing the result in out. Length is in bytes.
    void xor_buf(const uint8_t in[], uint8_t out[], size_t len);

#ifdef __cplusplus
}
#endif

#endif   // TYPEDEF_H

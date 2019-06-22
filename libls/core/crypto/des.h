/*********************************************************************
* Filename:   des.h
* Author:     Brad Conte (brad AT bradconte.com)
* Contributor: lauthrul (lauthrul@foxmail.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding DES implementation.
              Note that encryption and decryption are defined by how
              the key setup is performed, the actual en/de-cryption is
              performed by the same function.
*********************************************************************/

#ifndef DES_H
#define DESH

/*************************** HEADER FILES ***************************/
#include <stddef.h>
#include "cipher_def.h"

/****************************** MACROS ******************************/
#define DES_BLOCK_SIZE 8                // DES operates on 8 bytes at a time

/**************************** DATA TYPES ****************************/
typedef enum
{
    DES_ENCRYPT,
    DES_DECRYPT
} DES_MODE;

/*********************** FUNCTION DECLARATIONS **********************/
#ifdef __cplusplus
extern "C" {
#endif

///////////////////
// AES - ECB
///////////////////

void des_key_setup(const uint8_t key[DES_BLOCK_SIZE],       // The key must be 64 bits (8 bytes)
                   uint8_t schedule[][6],                   // Output key schedule to be used later
                   DES_MODE mode);                          // Encrypt or decrypt

void des_crypt(const uint8_t in[DES_BLOCK_SIZE],            // Plaintext or ciphertext, must be 64 bits (8 bytes)
               uint8_t out[DES_BLOCK_SIZE],                 // Out, same length as in
               const uint8_t key[][6]);                     // From the key setup

///////////////////
// AES - CBC
///////////////////

int32_t des_encrypt_cbc(const uint8_t in[],                 // Plaintext, must be a multiple of DES_BLOCK_SIZE
                        size_t in_len,                      // Must be a multiple of DES_BLOCK_SIZE
                        uint8_t out[],                      // Ciphertext, same length as plaintext
                        const uint8_t key[][6],             // From the key setup
                        const uint8_t iv[DES_BLOCK_SIZE]);  // IV, must be DES_BLOCK_SIZE bytes long

int32_t des_decrypt_cbc(const uint8_t in[],                 // Ciphertext, must be a multiple of DES_BLOCK_SIZE
                        size_t in_len,                      // Must be a multiple of DES_BLOCK_SIZE
                        uint8_t out[],                      // Plaintext, same length as ciphertext
                        const uint8_t key[][6],             // From the key setup
                        const uint8_t iv[DES_BLOCK_SIZE]);  // IV, must be DES_BLOCK_SIZE bytes long


///////////////////
// THREE AES - ECB
///////////////////

void three_des_key_setup(const uint8_t key[],
                         uint8_t schedule[][16][6],
                         DES_MODE mode);

void three_des_crypt(const uint8_t in[DES_BLOCK_SIZE],
                     uint8_t out[DES_BLOCK_SIZE],
                     const uint8_t key[][16][6]);

int32_t three_des_encrypt_cbc(const uint8_t in[],                 // Plaintext, must be a multiple of DES_BLOCK_SIZE
                              size_t in_len,                      // Must be a multiple of DES_BLOCK_SIZE
                              uint8_t out[],                      // Ciphertext, same length as plaintext
                              const uint8_t key[][16][6],         // From the key setup
                              const uint8_t iv[DES_BLOCK_SIZE]);  // IV, must be DES_BLOCK_SIZE bytes long

int32_t three_des_decrypt_cbc(const uint8_t in[],                 // Ciphertext, must be a multiple of DES_BLOCK_SIZE
                              size_t in_len,                      // Must be a multiple of DES_BLOCK_SIZE
                              uint8_t out[],                      // Plaintext, same length as ciphertext
                              const uint8_t key[][16][6],         // From the key setup
                              const uint8_t iv[DES_BLOCK_SIZE]);  // IV, must be DES_BLOCK_SIZE bytes long


///////////////////
// THREE AES - CBC
///////////////////



#ifdef __cplusplus
}
#endif

#endif   // DES_H

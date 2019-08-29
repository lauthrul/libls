#pragma once
#ifndef URL_ENCODE_H
#define URL_ENCODE_H

#ifdef __cplusplus
extern "C" {
#endif

// return encoded result length
    int url_encode(const char* s, int len, char* buff, int buff_size);

// return decoded result length, *param* [str] is output as well as input.
    int url_decode(char* str, int len);

#ifdef __cplusplus
}
#endif

#endif /* URL_ENCODE_H */

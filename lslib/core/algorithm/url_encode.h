#pragma once
#ifndef URL_ENCODE_H
#define URL_ENCODE_H

#ifdef __cplusplus
extern "C" {
#endif

// return decoded result length, *param* [str] is output as well as input.
int ls_url_decode(__inout char* str, int len);

// return encoded result length
int ls_url_encode(char const* s, int len, char* buff, int buff_size);

#ifdef __cplusplus
}
#endif

#endif /* URL_ENCODE_H */
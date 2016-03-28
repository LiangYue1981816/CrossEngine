#pragma once


#include <stdio.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif

int base64_encode(const char *incode, int insize, char *outcode, int outsize);
int base64_decode(const char *incode, int insize, char *outcode, int outsize);

#ifdef __cplusplus
}
#endif

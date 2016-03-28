#pragma once


#include <stdio.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif

void md5_init ();
void md5_update (unsigned char *inbuf, unsigned int inlen);
void md5_final (unsigned char *sign);
void md5_sign (unsigned char *inbuf, unsigned int inlen, unsigned char *sign);

#ifdef __cplusplus
}
#endif

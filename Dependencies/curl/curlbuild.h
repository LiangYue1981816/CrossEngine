#ifndef __CURL_CURLBUILD_H
#define __CURL_CURLBUILD_H

#if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32)) && !defined(__SYMBIAN32__)
#include "curlbuild_win32.h"
#else
#include "curlbuild_other.h"
#endif

#endif /* __CURL_CURLBUILD_H */

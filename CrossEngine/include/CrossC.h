/****************************************************************************
Copyright (c) 2015 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#pragma once


#include "CrossEngine.h"


_CrossExport unsigned int tick(void);                                                              // 时钟滴答, 精确到1微秒, 1s = 1000ms = 1000000μ
_CrossExport void splitfilename(const char *name, char *fname, char *ext);                         // 分隔文件名


_CrossExport size_t fsize(FILE *stream);                                                           // 文件大小
_CrossExport size_t freadline(char *buffer, size_t size, FILE *stream);                            // 读入文件行
_CrossExport size_t freadstring(char *buffer, size_t size, FILE *stream);                          // 读入字符串
_CrossExport size_t fwritestring(const char *buffer, size_t size, FILE *stream);                   // 保存字符串


_CrossExport int wcstoi(const wchar_t *wsz);                                                       // 宽子节字符串转换为整数
_CrossExport float wcstof(const wchar_t *wsz);                                                     // 宽子节字符串转换为浮点数


#if !defined _PLATFORM_WINDOWS_

#if defined _PLATFORM_IOS_

_CrossExport size_t mbstowcs(wchar_t *wsz, const char *sz, size_t count);                          // 多字节字符串转换为宽子节字符串
_CrossExport size_t wcstombs(char *sz, const wchar_t *wsz, size_t count);                          // 宽子节字符串转换为多字节字符串

#endif

_CrossExport int stricmp(const char *src, const char *dst);                                        // 字符串比较
_CrossExport int strnicmp(const char *src, const char *dst, int count);                            // 字符串比较

#define _A_NORMAL 0x00
#define _A_RDONLY 0x01
#define _A_HIDDEN 0x02
#define _A_SYSTEM 0x04
#define _A_SUBDIR 0x10
#define _A_ARCH   0x20

struct _finddata_t {
	char name[260];
	unsigned int attrib;
};

_CrossExport long _findfirst(const char *pattern, struct _finddata_t *data);                       // 查找首个匹配的文件
_CrossExport long _findnext(long id, struct _finddata_t *data);                                    // 查找下一个匹配的文件
_CrossExport long _findclose(long id);                                                             // 结束查找

#endif

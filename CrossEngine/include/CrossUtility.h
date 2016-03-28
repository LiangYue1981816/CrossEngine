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


namespace CrossEngine {

	_CrossExport DWORD HashValue(const CHAR *szString);                                            // 哈希值

	_CrossExport VOID PseudoRandomSeed(DWORD dwSeed);                                              // 伪随机数种子
	_CrossExport DWORD PseudoRandomValue(VOID);                                                    // 伪随机数

	_CrossExport size_t UnicodeToUTF8(const WCHAR *wszSrc, INT nSrcLen, CHAR *szDest, INT nDestLen); // UNICODE转换为UTF-8
	_CrossExport size_t UTF8ToUnicode(const CHAR *szSrc, INT nSrcLen, WCHAR *wszDest, INT nDestLen); // UTF-8转换为UNICODE

}

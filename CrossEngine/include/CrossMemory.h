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

	enum MEMTYPE { MEMTYPE_HEAP = 0, MEMTYPE_STACK };                                              // 内存类型

	_CrossExport VOID* Malloc(size_t size, MEMTYPE memType);                                       // 分配内存
	_CrossExport VOID  Free(VOID *ptr);                                                            // 释放内存

	_CrossExport VOID** MallocMatrix(size_t nx, size_t ny, size_t nsize, MEMTYPE memType);         // 分配二维内存
	_CrossExport VOID   FreeMatrix(VOID **ptr);                                                    // 释放二维内存

	_CrossExport VOID*** MallocVolume(size_t nx, size_t ny, size_t nz, size_t nsize, MEMTYPE memType); // 分配三维内存
	_CrossExport VOID    FreeVolume(VOID ***ptr);                                                  // 释放三维内存

	_CrossExport DWORD GetMaxPointerCount(VOID);                                                   // 获得峰值指针数
	_CrossExport DWORD GetMaxAllocatedSize(VOID);                                                  // 获得峰值内存大小

	_CrossExport DWORD GetTotalPointerCount(VOID);                                                 // 获得累计指针数
	_CrossExport DWORD GetTotalAllocatedSize(VOID);                                                // 获得累计内存大小

	_CrossExport DWORD GetPointerCount(VOID);                                                      // 获得当前指针数
	_CrossExport DWORD GetAllocatedSize(VOID);                                                     // 获得当前内存大小

}

//
// C 内存分配重载
//
#define SAFE_MALLOC(s, t) ::CrossEngine::Malloc((s), (t))
#define SAFE_FREE(p) do { if ((p)) { ::CrossEngine::Free((p)); (p) = NULL; } } while (0);

//
// C++ 内存分配重载
//
// 使用方法: 
//     BYTE *p = SAFE_NEW(MEMTYPE_HEAP) BYTE;
//     SAFE_DELETE(p);
//
#define SAFE_NEW(t) new (t)
#define SAFE_DELETE(p) do { if ((p)) { delete (p); (p) = NULL; } } while (0);
#define SAFE_DELETE_ARRAY(p) do { if ((p)) { delete [] (p); (p) = NULL; } } while (0);

//
// 内存大小
//
#define MEM_SIZE(ptr) *((DWORD *)(ptr) - 1)

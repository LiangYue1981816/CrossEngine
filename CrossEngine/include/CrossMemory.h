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

	enum MEMTYPE { MEMTYPE_HEAP = 0, MEMTYPE_STACK };                                              // �ڴ�����

	_CrossExport VOID* Malloc(size_t size, MEMTYPE memType);                                       // �����ڴ�
	_CrossExport VOID  Free(VOID *ptr);                                                            // �ͷ��ڴ�

	_CrossExport VOID** MallocMatrix(size_t nx, size_t ny, size_t nsize, MEMTYPE memType);         // �����ά�ڴ�
	_CrossExport VOID   FreeMatrix(VOID **ptr);                                                    // �ͷŶ�ά�ڴ�

	_CrossExport VOID*** MallocVolume(size_t nx, size_t ny, size_t nz, size_t nsize, MEMTYPE memType); // ������ά�ڴ�
	_CrossExport VOID    FreeVolume(VOID ***ptr);                                                  // �ͷ���ά�ڴ�

	_CrossExport DWORD GetMaxPointerCount(VOID);                                                   // ��÷�ֵָ����
	_CrossExport DWORD GetMaxAllocatedSize(VOID);                                                  // ��÷�ֵ�ڴ��С

	_CrossExport DWORD GetTotalPointerCount(VOID);                                                 // ����ۼ�ָ����
	_CrossExport DWORD GetTotalAllocatedSize(VOID);                                                // ����ۼ��ڴ��С

	_CrossExport DWORD GetPointerCount(VOID);                                                      // ��õ�ǰָ����
	_CrossExport DWORD GetAllocatedSize(VOID);                                                     // ��õ�ǰ�ڴ��С

}

//
// C �ڴ��������
//
#define SAFE_MALLOC(s, t) ::CrossEngine::Malloc((s), (t))
#define SAFE_FREE(p) do { if ((p)) { ::CrossEngine::Free((p)); (p) = NULL; } } while (0);

//
// C++ �ڴ��������
//
// ʹ�÷���: 
//     BYTE *p = SAFE_NEW(MEMTYPE_HEAP) BYTE;
//     SAFE_DELETE(p);
//
#define SAFE_NEW(t) new (t)
#define SAFE_DELETE(p) do { if ((p)) { delete (p); (p) = NULL; } } while (0);
#define SAFE_DELETE_ARRAY(p) do { if ((p)) { delete [] (p); (p) = NULL; } } while (0);

//
// �ڴ��С
//
#define MEM_SIZE(ptr) *((DWORD *)(ptr) - 1)

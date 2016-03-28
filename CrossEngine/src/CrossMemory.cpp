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

#include "_CrossEngine.h"


namespace CrossEngine {

	//////////////////////////////////////////////////////////////////////////
	// 系统内存分配
	//========================================================================
	static inline VOID* _malloc(size_t size)
	{
		ASSERT(size > 0);

		VOID *ptr = NULL;

#if defined _PLATFORM_WINDOWS_

		ptr = _aligned_malloc(size, 16);

#elif defined _PLATFORM_ANDROID_

		ptr = memalign(16, size);

#else

		ptr = malloc(size);

#endif

		if (ptr == NULL) {
			WriteLogE("Error: malloc system memory failed.\n");
		}

		return ptr;
	}

	static inline VOID _free(VOID *ptr)
	{
		ASSERT(ptr);

#if defined _PLATFORM_WINDOWS_

		_aligned_free(ptr);

#else

		free(ptr);

#endif
	}


	//////////////////////////////////////////////////////////////////////////
	// 堆内存分配器
	//
	// MemPool
	//  _______       _________________       _______
	// |       |     |     |     |     |     |       |
	// |  ...  | --> | Mem | ... | Mem | --> |  ...  |
	// |       | <-- |Block|     |Block| <-- |       |
	// |       |     |     |     |     |     |       |
	//  -------       -----------------       -------
	//   128k          128k   ...  128k         128k
	//========================================================================
	class CMemHeap
	{
	private:
		static const DWORD POOL_SIZE = 128 * 1024;
		static const DWORD POOL_COUNT = 64 * 1024;

		struct MEMBLOCK;
		struct MEMPOOL;
		struct MEMPOOLHEAD;

		struct MEMBLOCK {
			MEMPOOL *pPool;
			MEMBLOCK *pNext;

			DWORD dwReserve0;
#ifdef __LP64__
			DWORD dwReserve1;
			DWORD dwReserve2;
#endif
			DWORD dwAddress;
		};

		struct MEMPOOL {
			DWORD dwBlockIndex;
			DWORD dwBlockCount;

			MEMPOOL *pPrev;
			MEMPOOL *pNext;

			MEMBLOCK *pFree;
			MEMBLOCK *pBlockBase;
		};

		struct MEMPOOLHEAD {
			MEMPOOL *pMemPoolHead;
			MEMPOOL *pMemPoolActive;
		};

		MEMPOOLHEAD m_memPools[POOL_COUNT];

	public:
		CMemHeap(VOID)
		{
			memset(m_memPools, 0, sizeof(m_memPools));
		}

		~CMemHeap(VOID)
		{
			for (INT indexMemPool = 0; indexMemPool < POOL_COUNT; indexMemPool++) {
				if (m_memPools[indexMemPool].pMemPoolHead) {
					FreeMemPool(m_memPools[indexMemPool].pMemPoolHead);
				}
			}
		}

	private:
		inline MEMPOOL* AllocMemPool(DWORD dwMemSize)
		{
			DWORD dwBlockSize = ALIGN_16BYTE(sizeof(MEMBLOCK)) + ALIGN_16BYTE(dwMemSize);
			DWORD dwBlockCount = POOL_SIZE / dwBlockSize;

			MEMPOOL *pMemPool = (MEMPOOL *)_malloc(ALIGN_16BYTE(sizeof(MEMPOOL)) + ALIGN_16BYTE(dwBlockSize*dwBlockCount));
			MEMBLOCK *pMemBlock = pMemPool->pBlockBase = (MEMBLOCK *)(((BYTE *)pMemPool) + ALIGN_16BYTE(sizeof(MEMPOOL)));
			{
				for (DWORD indexBlock = 0; indexBlock < dwBlockCount; indexBlock++) {
					pMemBlock->pPool = pMemPool;
					pMemBlock->pNext = (MEMBLOCK *)(((BYTE *)pMemBlock) + dwBlockSize);
					pMemBlock = pMemBlock->pNext;
				}
			}

			pMemPool->pNext = NULL;
			pMemPool->pPrev = NULL;
			pMemPool->pFree = pMemPool->pBlockBase;

			pMemPool->dwBlockIndex = 0;
			pMemPool->dwBlockCount = dwBlockCount;

			return pMemPool;
		}

		inline VOID FreeMemPool(MEMPOOL *pMemPool)
		{
			if (pMemPool->pNext) {
				FreeMemPool(pMemPool->pNext);
			}

			_free(pMemPool);
		}

	public:
		inline VOID* Malloc(size_t size)
		{
			DWORD *pPointer = NULL;
			DWORD dwMemSize = (DWORD)ALIGN_16BYTE(size);

			if (dwMemSize < POOL_COUNT) {
				if (m_memPools[dwMemSize].pMemPoolHead == NULL) {
					m_memPools[dwMemSize].pMemPoolHead = m_memPools[dwMemSize].pMemPoolActive = AllocMemPool(dwMemSize);
				}

				MEMPOOL *pMemPool = m_memPools[dwMemSize].pMemPoolActive;
				{
					do {
						if (pMemPool->dwBlockIndex < pMemPool->dwBlockCount) {
							pMemPool->dwBlockIndex++;

							pPointer = &pMemPool->pFree->dwAddress;
							pMemPool->pFree = pMemPool->pFree->pNext;

							break;
						}

						if (pMemPool->pNext == NULL) {
							pMemPool->pNext = AllocMemPool(dwMemSize);
							pMemPool->pNext->pPrev = pMemPool;
						}

						pMemPool = pMemPool->pNext;
					} while (TRUE);
				}
				m_memPools[dwMemSize].pMemPoolActive = pMemPool;
			}
			else {
				pPointer = (DWORD *)_malloc(dwMemSize + 16);

				*pPointer++ = 0xcccccccc;
				*pPointer++ = 0xcccccccc;
				*pPointer++ = 0xcccccccc;
			}

			*pPointer++ = dwMemSize;

			return pPointer;
		}

		inline VOID Free(VOID *ptr)
		{
			DWORD dwMemSize = *(DWORD *)(((DWORD *)ptr) - 1);

			if (dwMemSize < POOL_COUNT) {
				MEMBLOCK *pMemBlock = (MEMBLOCK *)(((BYTE *)ptr) - sizeof(MEMBLOCK));
				MEMPOOL *pMemPool = pMemBlock->pPool;

				if (--pMemPool->dwBlockIndex == 0) {
					if (m_memPools[dwMemSize].pMemPoolHead == pMemPool) {
						m_memPools[dwMemSize].pMemPoolHead = pMemPool->pNext;
					}

					if (pMemPool->pPrev) pMemPool->pPrev->pNext = pMemPool->pNext;
					if (pMemPool->pNext) pMemPool->pNext->pPrev = pMemPool->pPrev;

					pMemPool->pPrev = NULL;
					pMemPool->pNext = NULL;

					FreeMemPool(pMemPool);
				}
				else {
					pMemBlock->pNext = pMemPool->pFree;
					pMemPool->pFree = pMemBlock;
				}

				m_memPools[dwMemSize].pMemPoolActive = m_memPools[dwMemSize].pMemPoolHead;
			}
			else {
				_free((BYTE *)ptr - 16);
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// 栈内存分配器
	//
	// MemPool 8MB
	//  ________________________________________
	// |     |         |     |         |        |
	// | Mem |  Memory | Mem |  Memory |   ...  |
	// |Size |         |Size |         |        |
	// |     |         |     |         |        |
	//  ----------------------------------------
	//========================================================================
	class CMemStack
	{
	private:
		static const DWORD POOL_SIZE = 8 * 1024 * 1024;

		struct MEMPOOL {
			DWORD *pAddress;
			DWORD *pPointer;

			DWORD dwFullSize;
			DWORD dwFreeSize;
		};

		MEMPOOL m_memPool;

	public:
		CMemStack(VOID)
		{
			m_memPool.pAddress = m_memPool.pPointer = (DWORD *)_malloc(POOL_SIZE);
			m_memPool.dwFullSize = POOL_SIZE;
			m_memPool.dwFreeSize = POOL_SIZE;
		}

		~CMemStack(VOID)
		{
			_free(m_memPool.pAddress);
		}

	public:
		inline VOID* Malloc(size_t size)
		{
			DWORD *pPointer = NULL;
			DWORD dwMemSize = (DWORD)ALIGN_16BYTE(size);

			if ((((BYTE *)m_memPool.pPointer) + dwMemSize + 16) < (((BYTE *)m_memPool.pAddress) + m_memPool.dwFullSize)) {
				pPointer = m_memPool.pPointer;

				*pPointer++ = 0xcccccccc;
				*pPointer++ = 0xcccccccc;
				*pPointer++ = 0xcccccccc;
				*pPointer++ = (DWORD)dwMemSize;

				m_memPool.pPointer = (DWORD *)(((BYTE *)m_memPool.pPointer) + dwMemSize + 16);
				m_memPool.dwFreeSize = (DWORD)(((BYTE *)m_memPool.pAddress) + m_memPool.dwFullSize - ((BYTE *)m_memPool.pPointer));
			}

			return pPointer;
		}

		inline VOID Free(VOID *ptr)
		{
			DWORD dwMemSize = *(DWORD *)(((DWORD *)ptr) - 1);

			m_memPool.pPointer = (DWORD *)(((BYTE *)m_memPool.pPointer) - dwMemSize - 16);
			m_memPool.dwFreeSize = (DWORD)(((BYTE *)m_memPool.pAddress) + m_memPool.dwFullSize - ((BYTE *)m_memPool.pPointer));
		}

		inline BOOL IsStackMemory(VOID *ptr)
		{
			return ptr >= m_memPool.pAddress && ptr < m_memPool.pPointer ? TRUE : FALSE;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// 内存分配器
	//========================================================================
	class CMemAllocator
	{
	private:
		struct MEMMONITOR {
			DWORD dwPointerCount;
			DWORD dwAllocatedSize;

			DWORD dwMaxPointerCount;
			DWORD dwMaxAllocatedSize;

			DWORD dwTotalPointerCount;
			DWORD dwTotalAllocatedSize;
		};

		CMemHeap m_memHeap;
		CMemStack m_memStack;
		pthread_mutex_t m_mutex;

		MEMMONITOR m_memMonitor;

	public:
		CMemAllocator(VOID)
		{
			pthread_mutex_init(&m_mutex, NULL);
			memset(&m_memMonitor, 0, sizeof(m_memMonitor));
		}

		~CMemAllocator(VOID)
		{
			WriteLogI("\n");
			WriteLogI("================= Memory Information =================\n");
			WriteLogI("\tLeak memory size = %2.2fMB (%dBytes)\n", m_memMonitor.dwAllocatedSize/(1024.0f*1024.0f), m_memMonitor.dwAllocatedSize);
			WriteLogI("\tLeak pointer number = %d\n", m_memMonitor.dwPointerCount);
			WriteLogI("\tMax pointer number = %d\n", m_memMonitor.dwMaxPointerCount);
			WriteLogI("\tMax allocated size = %2.2fMB (%dBytes)\n", m_memMonitor.dwMaxAllocatedSize/(1024.0f*1024.0f), m_memMonitor.dwMaxAllocatedSize);
			WriteLogI("======================================================\n");

			pthread_mutex_destroy(&m_mutex);
		}

	public:
		inline VOID* Malloc(size_t size, MEMTYPE memType)
		{
			ASSERT(size > 0);

			VOID *pPointer = NULL;
			pthread_mutex_lock(&m_mutex);
			{
				switch (memType) {
				case MEMTYPE_HEAP: pPointer = m_memHeap.Malloc(size); break;
				case MEMTYPE_STACK: pPointer = m_memStack.Malloc(size); break;
				}

				ASSERT(pPointer);
				ASSERT((UINT64)pPointer == ((UINT64)pPointer / 16) * 16);

				m_memMonitor.dwPointerCount++;
				m_memMonitor.dwAllocatedSize += GetMemSize(pPointer);

				m_memMonitor.dwTotalPointerCount++;
				m_memMonitor.dwTotalAllocatedSize += GetMemSize(pPointer);

				if (m_memMonitor.dwMaxPointerCount < m_memMonitor.dwPointerCount) m_memMonitor.dwMaxPointerCount = m_memMonitor.dwPointerCount;
				if (m_memMonitor.dwMaxAllocatedSize < m_memMonitor.dwAllocatedSize) m_memMonitor.dwMaxAllocatedSize = m_memMonitor.dwAllocatedSize;
			}
			pthread_mutex_unlock(&m_mutex);
			return pPointer;
		}

		inline VOID Free(VOID *ptr)
		{
			ASSERT(ptr);

			pthread_mutex_lock(&m_mutex);
			{
				if (ptr) {
					m_memMonitor.dwPointerCount--;
					m_memMonitor.dwAllocatedSize -= GetMemSize(ptr);

					if (m_memStack.IsStackMemory(ptr)) {
						m_memStack.Free(ptr);
					}
					else {
						m_memHeap.Free(ptr);
					}
				}
			}
			pthread_mutex_unlock(&m_mutex);
		}

		inline DWORD GetMemSize(VOID *ptr)
		{
			return MEM_SIZE(ptr);
		}

		inline DWORD GetPointerCount(VOID)
		{
			DWORD dwPointerCount;
			pthread_mutex_lock(&m_mutex);
			{
				dwPointerCount = m_memMonitor.dwPointerCount;
			}
			pthread_mutex_unlock(&m_mutex);
			return dwPointerCount;
		}

		inline DWORD GetAllocatedSize(VOID)
		{
			DWORD dwAllocatedSize;
			pthread_mutex_lock(&m_mutex);
			{
				dwAllocatedSize = m_memMonitor.dwAllocatedSize;
			}
			pthread_mutex_unlock(&m_mutex);
			return dwAllocatedSize;
		}

		inline DWORD GetMaxPointerCount(VOID)
		{
			DWORD dwMaxPointerCount;
			pthread_mutex_lock(&m_mutex);
			{
				dwMaxPointerCount = m_memMonitor.dwMaxPointerCount;
			}
			pthread_mutex_unlock(&m_mutex);
			return dwMaxPointerCount;
		}

		inline DWORD GetMaxAllocatedSize(VOID)
		{
			DWORD dwMaxAllocatedSize;
			pthread_mutex_lock(&m_mutex);
			{
				dwMaxAllocatedSize = m_memMonitor.dwMaxAllocatedSize;
			}
			pthread_mutex_unlock(&m_mutex);
			return dwMaxAllocatedSize;
		}

		inline DWORD GetTotalPointerCount(VOID)
		{
			DWORD dwTotalPointerCount;
			pthread_mutex_lock(&m_mutex);
			{
				dwTotalPointerCount = m_memMonitor.dwTotalPointerCount;
			}
			pthread_mutex_unlock(&m_mutex);
			return dwTotalPointerCount;
		}

		inline DWORD GetTotalAllocatedSize(VOID)
		{
			DWORD dwTotalAllocatedSize;
			pthread_mutex_lock(&m_mutex);
			{
				dwTotalAllocatedSize = m_memMonitor.dwTotalAllocatedSize;
			}
			pthread_mutex_unlock(&m_mutex);
			return dwTotalAllocatedSize;
		}
	};


	//////////////////////////////////////////////////////////////////////////
	// 对外接口
	//========================================================================
	static CMemAllocator memAllocator;

	//
	// 分配内存
	//
	_CrossExport VOID* Malloc(size_t size, MEMTYPE memType)
	{
		return memAllocator.Malloc(size, memType);
	}

	//
	// 释放内存
	//
	_CrossExport VOID Free(VOID *ptr)
	{
		memAllocator.Free(ptr);
	}

	//
	// 分配二维内存
	//
	_CrossExport VOID** MallocMatrix(size_t nx, size_t ny, size_t nsize, MEMTYPE memType)
	{
		ASSERT(nx);
		ASSERT(ny);
		ASSERT(nsize);

		DWORD dwHeaderSize = (DWORD)(nx*POINTER_SIZE);
		DWORD dwBufferSize = (DWORD)(dwHeaderSize + nx*ny*nsize);

		BYTE *pBuffer = (BYTE *)SAFE_MALLOC(dwBufferSize, memType);
		if (pBuffer == NULL) return NULL;

		BYTE **ppLine = (BYTE **)pBuffer;
		BYTE *pBody = pBuffer + dwHeaderSize;

		for (DWORD x = 0; x < nx; x++) {
			*ppLine = pBody; pBody += ny*nsize; ppLine++;
		}

		return (VOID **)pBuffer;
	}

	//
	// 释放二维内存
	//
	_CrossExport VOID FreeMatrix(VOID **ptr)
	{
		SAFE_FREE(ptr);
	}

	//
	// 分配三维内存
	//
	_CrossExport VOID*** MallocVolume(size_t nx, size_t ny, size_t nz, size_t nsize, MEMTYPE memType)
	{
		ASSERT(nx);
		ASSERT(ny);
		ASSERT(nz);
		ASSERT(nsize);

		DWORD dwHeaderSize = (DWORD)(nx*POINTER_SIZE + nx*ny*POINTER_SIZE);
		DWORD dwBufferSize = (DWORD)(dwHeaderSize + nx*ny*nz*nsize);

		BYTE *pBuffer = (BYTE *)SAFE_MALLOC(dwBufferSize, memType);
		if (pBuffer == NULL) return NULL;

		BYTE ***pppLine = (BYTE ***)pBuffer;
		BYTE **ppLine = (BYTE **)(pBuffer + nx*POINTER_SIZE);
		BYTE *pBody = pBuffer + dwHeaderSize;

		for (DWORD x = 0; x < nx; x++) {
			*pppLine = ppLine;

			for (DWORD y = 0; y < ny; y++) {
				*ppLine = pBody; pBody += nz*nsize; ppLine++;
			}

			pppLine++;
		}

		return (VOID ***)pBuffer;
	}

	//
	// 释放三维内存
	//
	_CrossExport VOID FreeVolume(VOID ***ptr)
	{
		SAFE_FREE(ptr);
	}

	//
	// 获得峰值指针数
	//
	_CrossExport DWORD GetMaxPointerCount(VOID)
	{
		return memAllocator.GetMaxPointerCount();
	}

	//
	// 获得峰值内存大小
	//
	_CrossExport DWORD GetMaxAllocatedSize(VOID)
	{
		return memAllocator.GetMaxAllocatedSize();
	}

	//
	// 获得累计指针数
	//
	_CrossExport DWORD GetTotalPointerCount(VOID)
	{
		return memAllocator.GetTotalPointerCount();
	}

	//
	// 获得累计内存大小
	//
	_CrossExport DWORD GetTotalAllocatedSize(VOID)
	{
		return memAllocator.GetTotalAllocatedSize();
	}

	//
	// 获得当前指针数
	//
	_CrossExport DWORD GetPointerCount(VOID)
	{
		return memAllocator.GetPointerCount();
	}

	//
	// 获得当前内存大小
	//
	_CrossExport DWORD GetAllocatedSize(VOID)
	{
		return memAllocator.GetAllocatedSize();
	}

}

//
// C++ 全局内存分配重载
//
VOID* operator new (std::size_t s) throw (std::bad_alloc)
{
	return ::CrossEngine::Malloc(s, ::CrossEngine::MEMTYPE_HEAP);
}

VOID* operator new [](std::size_t s) throw (std::bad_alloc)
{
	return ::CrossEngine::Malloc(s, ::CrossEngine::MEMTYPE_HEAP);
}

VOID* operator new (std::size_t s, ::CrossEngine::MEMTYPE t) throw (std::bad_alloc)
{
	return ::CrossEngine::Malloc(s, t);
}

VOID* operator new [](std::size_t s, ::CrossEngine::MEMTYPE t) throw (std::bad_alloc)
{
	return ::CrossEngine::Malloc(s, t);
}

VOID operator delete (VOID *p) throw ()
{
	::CrossEngine::Free(p);
}

VOID operator delete [](VOID *p) throw ()
{
	::CrossEngine::Free(p);
}

VOID operator delete (VOID *p, ::CrossEngine::MEMTYPE) throw ()
{
	ASSERT(FALSE);
}

VOID operator delete [](VOID *p, ::CrossEngine::MEMTYPE) throw ()
{
	ASSERT(FALSE);
}

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

	template<class T> class _CrossExport CListNode
	{
	public:
		CListNode(T *ptr)
			: pNext(NULL)
			, pPrev(NULL)
			, pPointer(ptr)
		{

		}

	public:
		T *pPointer;

	public:
		CListNode *pNext;
		CListNode *pPrev;
	};

	template<class T> class _CrossExport CList
	{
	public:
		CList(VOID)
			: pHead(NULL)
		{

		}

		~CList(VOID)
		{
			RemoveAll();
		}

	public:
		inline VOID Add(T *pPointer, MEMTYPE memType = MEMTYPE_HEAP)
		{
			ASSERT(pPointer);

			CListNode<T> *pNode = SAFE_NEW(memType) CListNode<T>(pPointer);

			if (pHead) {
				pHead->pPrev = pNode;
				pNode->pNext = pHead;
			}

			pHead = pNode;
		}

		inline VOID Remove(const T *pPointer)
		{
			ASSERT(pPointer);

			if (CListNode<T> *pNode = pHead) {
				if (pHead->pPointer == pPointer) {
					pHead = pHead->pNext;
					SAFE_DELETE(pNode);
				}
				else {
					do {
						if (pNode->pPointer == pPointer) {
							if (pNode->pPrev) pNode->pPrev->pNext = pNode->pNext;
							if (pNode->pNext) pNode->pNext->pPrev = pNode->pPrev;

							SAFE_DELETE(pNode);
							break;
						}
					} while (pNode = pNode->pNext);
				}
			}
		}

		inline VOID RemoveAll(VOID)
		{
			if (CListNode<T> *pNode = pHead) {
				CListNode<T> *pNodeNext = NULL;

				do {
					pNodeNext = pNode->pNext;
					SAFE_DELETE(pNode);
				} while (pNode = pNodeNext);
			}

			pHead = NULL;
		}

	public:
		CListNode<T> *pHead;
	};

}

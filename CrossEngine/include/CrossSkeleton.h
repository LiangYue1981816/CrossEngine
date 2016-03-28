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

	typedef struct {
		BOOL bUpdate;                                                                              // ���±��

		VEC3 scale;                                                                                // ����
		VEC3 position;                                                                             // λ��
		QUAT orientation;                                                                          // ��ת
	} BONE;

	typedef struct {
		FLOAT weightScale;                                                                         // ����Ȩ��
		FLOAT weightPosition;                                                                      // λ��Ȩ��
		FLOAT weightOrientation;                                                                   // ��תȨ��

		VEC3 scale;                                                                                // ����
		VEC3 position;                                                                             // λ��
		QUAT orientation;                                                                          // ��ת
	} BONE_BLEND;

	typedef struct {
		CHAR szName[64];                                                                           // ������
		DWORD dwName;                                                                              // ������
	} BONE_NAME;

	typedef struct {
		INT *pParentIDs;                                                                           // �������ڵ���������
		BONE_NAME *pBoneNames;                                                                     // ����������
		BONE *pBoneWorld2Locals;                                                                   // ��ʼ������������任���������꼯��
		BONE *pBoneLocal2Parents;                                                                  // ��ʼ������������任���������꼯��
	} BONE_HIERARCHY;

	class _CrossExport CAnimSkeleton
	{
		friend class CSkeleton;


		// ����/��������
	protected:
		CAnimSkeleton(VOID);
		virtual ~CAnimSkeleton(VOID);


		// ����
	public:
		const CHAR* GetName(VOID) const;                                                           // �������
		DWORD GetHashName(VOID) const;                                                             // �������

		FLOAT GetLength(VOID) const;                                                               // ���ʱ��

		INT GetKeyFrameCount(VOID) const;                                                          // ��ùؼ�֡��
		FLOAT GetKeyFrameTime(INT indexFrame) const;                                               // ��ùؼ�֡ʱ��
		const BONE* GetKeyFrameData(INT indexFrame) const;                                         // ��ùؼ�֡����


		// ����
	protected:
		CHAR *m_szName;                                                                            // ����
		DWORD m_dwName;                                                                            // ����

		FLOAT m_length;                                                                            // ʱ��(��)

		INT m_numKeyFrames;                                                                        // �ؼ�֡��
		FLOAT *m_pTimeKeyFrames;                                                                   // �ؼ�֡ʱ��
		BONE **m_ppDataKeyFrames;                                                                  // �ؼ�֡����
	};

	class _CrossExport CSkeleton : public CResource
	{
		friend class CSkeletonPtr;
		friend class CSkeletonManager;


		// ����/��������
	protected:
		CSkeleton(CResourceManager *pResourceManager);
		virtual ~CSkeleton(VOID);


		// ����
	public:
		virtual VOID Init(VOID);                                                                   // ��ʼ����Դ
		virtual VOID Free(VOID);                                                                   // �ͷ���Դ

	public:
		virtual TYPE GetType(VOID) const;                                                          // �������
		virtual BOOL IsValid(VOID) const;                                                          // ��Ч�Լ��

	public:
		virtual BOOL CopyFrom(const CResource *pCopyFrom);                                         // ������Դ
		virtual BOOL LoadFromStream(CStream *pStream);                                             // ��������������Դ

	protected:
		VOID LoadHierarchy(CStream *pStream);                                                      // ���عǼ�
		VOID LoadAnimation(CStream *pStream);                                                      // ���ض���

	public:
		INT GetBoneCount(VOID) const;                                                              // ��ù�����
		INT GetAnimationCount(VOID) const;                                                         // ��ö�����

		const CAnimSkeleton* GetAnimation(INT index) const;                                        // ��ö���
		const BONE_HIERARCHY* GetBoneHierarchy(VOID) const;                                        // ��ùǼ�


		// ����
	protected:
		INT m_numBones;                                                                            // ������
		INT m_numAnimations;                                                                       // ������

		BONE_HIERARCHY m_hierarchy;                                                                // �Ǽ�
		CAnimSkeleton *m_pAnimations;                                                              // ��������
	};

	class _CrossExport CSkeletonPtr : public CSharedPtr<CSkeleton>
	{
		// ����/��������
	public:
		CSkeletonPtr(VOID) : CSharedPtr<CSkeleton>() {}
		CSkeletonPtr(const CSkeleton *pSkeleton) : CSharedPtr<CSkeleton>(pSkeleton) {}
		CSkeletonPtr(const CSkeletonPtr &ptrSkeleton) : CSharedPtr<CSkeleton>(ptrSkeleton) {}
		CSkeletonPtr(const CResourcePtr &ptrResource) { Set(static_cast<CSkeleton*>(ptrResource.GetPointer()), ptrResource.GetRefCount()); }
		virtual ~CSkeletonPtr(VOID) {}


		// ����
	protected:
		virtual VOID FreePointer(VOID)
		{
			if (m_pPointer) {
				m_pPointer->GetResourceManager()->DestroyResource(m_pPointer);
			}
		}

	public:
		CSkeletonPtr& operator = (const CResourcePtr &ptrResource)
		{
			Set(static_cast<CSkeleton*>(ptrResource.GetPointer()), ptrResource.GetRefCount());
			return *this;
		}

		operator CResource* ()
		{
			return (CResource *)m_pPointer;
		}

		operator const CResource* () const
		{
			return (const CResource *)m_pPointer;
		}
	};

}

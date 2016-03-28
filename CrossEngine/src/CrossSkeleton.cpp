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

	CAnimSkeleton::CAnimSkeleton(VOID)
		: m_szName(NULL)
		, m_dwName(INVALID_HASHNAME)

		, m_length(0.0f)

		, m_numKeyFrames(0)
		, m_pTimeKeyFrames(NULL)
		, m_ppDataKeyFrames(NULL)
	{

	}

	CAnimSkeleton::~CAnimSkeleton(VOID)
	{

	}

	//
	// �������
	//
	const CHAR* CAnimSkeleton::GetName(VOID) const
	{
		return m_szName;
	}

	//
	// �������
	//
	DWORD CAnimSkeleton::GetHashName(VOID) const
	{
		return m_dwName;
	}

	//
	// ���ʱ��
	//
	FLOAT CAnimSkeleton::GetLength(VOID) const
	{
		return m_length;
	}

	//
	// ��ùؼ�֡��
	//
	INT CAnimSkeleton::GetKeyFrameCount(VOID) const
	{
		return m_numKeyFrames;
	}

	//
	// ��ùؼ�֡ʱ��
	//
	FLOAT CAnimSkeleton::GetKeyFrameTime(INT indexFrame) const
	{
		ASSERT(indexFrame >= 0 && indexFrame < m_numKeyFrames);
		return m_pTimeKeyFrames[indexFrame];
	}

	//
	// ��ùؼ�֡����
	//
	const BONE* CAnimSkeleton::GetKeyFrameData(INT indexFrame) const
	{
		ASSERT(indexFrame >= 0 && indexFrame < m_numKeyFrames);
		return m_ppDataKeyFrames[indexFrame];
	}

	CSkeleton::CSkeleton(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{
		Init();
	}

	CSkeleton::~CSkeleton(VOID)
	{
		Free();
	}

	//
	// ��ʼ����Դ
	//
	VOID CSkeleton::Init(VOID)
	{
		m_numBones = 0;
		m_numAnimations = 0;
		
		m_hierarchy.pParentIDs = NULL;
		m_hierarchy.pBoneNames = NULL;
		m_hierarchy.pBoneWorld2Locals = NULL;
		m_hierarchy.pBoneLocal2Parents = NULL;

		m_pAnimations = NULL;

		CResource::Init();
	}

	//
	// �ͷ���Դ
	//
	VOID CSkeleton::Free(VOID)
	{
		SAFE_DELETE(m_pAnimations);
		CResource::Free();
	}

	//
	// �������
	//
	CResource::TYPE CSkeleton::GetType(VOID) const
	{
		return CResource::SKELETON;
	}

	//
	// ��Ч�Լ��
	//
	BOOL CSkeleton::IsValid(VOID) const
	{
		return m_numBones != 0 || m_numAnimations != 0 ? TRUE : FALSE;
	}

	//
	// ������Դ
	// ע��: ��ֹ������Դ!!!
	//
	BOOL CSkeleton::CopyFrom(const CResource *pCopyFrom)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//
	// ��������������Դ
	//
	BOOL CSkeleton::LoadFromStream(CStream *pStream)
	{
		//
		// 1. ��������Ч�Լ��
		//
		if (pStream == NULL) {
			return FALSE;
		}

		if (pStream->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. �ͷŹ���
		//
		Free();

		//
		// 3. ���ع���
		//
		try {
			//
			// 3.1. ����Ǽ�����
			//
			LoadHierarchy(pStream);

			//
			// 3.2. ���������������
			//
			LoadAnimation(pStream);

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CSkeleton::LoadFromStream: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ���عǼ�
	//
	VOID CSkeleton::LoadHierarchy(CStream *pStream)
	{
		ASSERT(pStream);
		ASSERT(pStream->IsValid());

		//
		// 1. ���������
		//
		m_numBones = (INT)pStream->ReadDWORD();

		//
		// 2. ���븸����ID����
		//
		m_hierarchy.pParentIDs = (INT *)pStream->GetCurrentAddress();
		pStream->Seek(sizeof(*m_hierarchy.pParentIDs) * m_numBones, CStream::SEEK_MODE_CUR);

		//
		// 3. �������������
		//
		m_hierarchy.pBoneNames = (BONE_NAME *)pStream->GetCurrentAddress();
		pStream->Seek(sizeof(*m_hierarchy.pBoneNames) * m_numBones, CStream::SEEK_MODE_CUR);

		//
		// 4. ������������ϵ����������ϵ�任����
		//
		m_hierarchy.pBoneWorld2Locals = (BONE *)pStream->GetCurrentAddress();
		pStream->Seek(sizeof(*m_hierarchy.pBoneWorld2Locals) * m_numBones, CStream::SEEK_MODE_CUR);

		//
		// 5. ���뱾������ϵ��������ϵ�任����
		//
		m_hierarchy.pBoneLocal2Parents = (BONE *)pStream->GetCurrentAddress();
		pStream->Seek(sizeof(*m_hierarchy.pBoneLocal2Parents) * m_numBones, CStream::SEEK_MODE_CUR);
	}

	//
	// ���ض���
	//
	VOID CSkeleton::LoadAnimation(CStream *pStream)
	{
		ASSERT(pStream);
		ASSERT(pStream->IsValid());

		//
		// 1. ���붯����
		//
		m_numAnimations = (INT)pStream->ReadDWORD();
		if (m_numAnimations == 0) return;

		//
		// 2. ���ض���
		//
		m_pAnimations = SAFE_NEW(MEMTYPE_HEAP) CAnimSkeleton[m_numAnimations];
		ASSERT(m_pAnimations);

		for (INT indexAnimation = 0; indexAnimation < m_numAnimations; indexAnimation++) {
			CAnimSkeleton *pAnimation = &m_pAnimations[indexAnimation];
			ASSERT(pAnimation);

			//
			// 2.1. ���붯����Ϣ
			//
			DWORD dwNameLen = pStream->ReadDWORD();
			pAnimation->m_szName = (CHAR *)pStream->GetCurrentAddress();
			pStream->Seek(dwNameLen, CStream::SEEK_MODE_CUR);

			pAnimation->m_dwName = pStream->ReadDWORD();
			pAnimation->m_length = pStream->ReadFLOAT();
			pAnimation->m_numKeyFrames = (INT)pStream->ReadDWORD();

			//
			// 2.2. ����ؼ�֡ʱ������
			//
			pAnimation->m_pTimeKeyFrames = (FLOAT *)pStream->GetCurrentAddress();
			pStream->Seek(sizeof(*pAnimation->m_pTimeKeyFrames) * pAnimation->m_numKeyFrames, CStream::SEEK_MODE_CUR);

			//
			// 2.3. ����ؼ�֡��������
			//
			DWORD dwLineSize = sizeof(**pAnimation->m_ppDataKeyFrames) * m_numBones;
			DWORD dwHeaderSize = sizeof(UINT64) * pAnimation->m_numKeyFrames;
			DWORD dwBufferSize = dwHeaderSize + dwLineSize * pAnimation->m_numKeyFrames;

			pAnimation->m_ppDataKeyFrames = (BONE **)pStream->GetCurrentAddress();
			{
				BYTE **ppLine = (BYTE **)pAnimation->m_ppDataKeyFrames;
				BYTE *pBody = (BYTE *)pAnimation->m_ppDataKeyFrames + dwHeaderSize;

				for (INT indexFrame = 0; indexFrame < pAnimation->m_numKeyFrames; indexFrame++) {
					*ppLine = pBody; pBody += dwLineSize; ppLine++;
				}
			}
			pStream->Seek(dwBufferSize, CStream::SEEK_MODE_CUR);
		}
	}

	//
	// ��ù�����
	//
	INT CSkeleton::GetBoneCount(VOID) const
	{
		return m_numBones;
	}

	//
	// ��ö�����
	//
	INT CSkeleton::GetAnimationCount(VOID) const
	{
		return m_numAnimations;
	}

	//
	// ��ö���
	//
	const CAnimSkeleton* CSkeleton::GetAnimation(INT index) const
	{
		ASSERT(index >= 0 && index < m_numAnimations);
		return &m_pAnimations[index];
	}

	//
	// ��ùǼ�
	//
	const BONE_HIERARCHY* CSkeleton::GetBoneHierarchy(VOID) const
	{
		return &m_hierarchy;
	}

}

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

	CSound::CSound(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{
		Init();
	}

	CSound::~CSound(VOID)
	{
		Free();
	}

	//
	// ��ʼ����Դ
	//
	VOID CSound::Init(VOID)
	{
		m_pFModSound = NULL;
		CResource::Init();
	}

	//
	// �ͷ���Դ
	//
	VOID CSound::Free(VOID)
	{
		SoundSystem()->ReleaseSound(m_pFModSound);
		CResource::Free();
	}

	//
	// �������
	//
	CResource::TYPE CSound::GetType(VOID) const
	{
		return CResource::SOUND;
	}

	//
	// ��Ч�Լ��
	//
	BOOL CSound::IsValid(VOID) const
	{
		return m_pFModSound ? TRUE : FALSE;
	}

	//
	// ������Դ
	// ע��: ��ֹ������Դ!!!
	//
	BOOL CSound::CopyFrom(const CResource *pCopyFrom)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//
	// ���ļ�������Դ
	//
	BOOL CSound::LoadFromFile(const CHAR *szFileName)
	{
		//
		// 1. ��������
		//
		BOOL rcode = CResource::LoadFromFile(szFileName);

		//
		// 2. �ͷ�������
		//
		m_stream.FreeData();

		return rcode;
	}

	//
	// ��ѹ����������Դ
	//
	BOOL CSound::LoadFromZip(const CHAR *szZipName, const CHAR *szFileName)
	{
		//
		// 1. ��������
		//
		BOOL rcode = CResource::LoadFromZip(szZipName, szFileName);

		//
		// 2. �ͷ�������
		//
		m_stream.FreeData();

		return rcode;
	}

	//
	// ��ѹ����������Դ
	//
	BOOL CSound::LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName)
	{
		//
		// 1. ��������
		//
		BOOL rcode = CResource::LoadFromZip(pZipPack, szFileName);

		//
		// 2. �ͷ�������
		//
		m_stream.FreeData();

		return rcode;
	}

	//
	// ��������������Դ
	//
	BOOL CSound::LoadFromStream(CStream *pStream)
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
		// 2. ��������
		//
		Free();

		//
		// 3. ��������
		//
		try {
			m_pFModSound = SoundSystem()->CreateSound((BYTE *)pStream->GetCurrentAddress(), pStream->GetFreeSize(), TRUE);
			if (m_pFModSound == NULL) throw "Create sound fail.";

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CTexture::LoadFromStream: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// �������
	//
	FMOD_SOUND* CSound::GetSound(VOID) const
	{
		return m_pFModSound;
	}

}

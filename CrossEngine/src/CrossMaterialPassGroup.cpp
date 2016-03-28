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

	CMatPassGroup::CMatPassGroup(const CHAR *szName, CMaterial *pMaterial)
		: m_pParentMaterial(pMaterial)
	{
		Init(szName);
	}

	CMatPassGroup::~CMatPassGroup(VOID)
	{
		Free();
	}

	//
	// ��ʼ��
	//
	VOID CMatPassGroup::Init(VOID)
	{
		m_passes.clear();
	}

	//
	// ��ʼ����Դ
	//
	VOID CMatPassGroup::Init(const CHAR *szName)
	{
		ASSERT(szName);

		Init();
		strcpy(m_szName, szName);
	}

	//
	// �ͷ���Դ
	//
	VOID CMatPassGroup::Free(VOID)
	{
		//
		// 1. �ͷ���Ⱦͨ·
		//
		for (PassSet::const_iterator itPass = m_passes.begin(); itPass != m_passes.end(); ++itPass) {
			const CMatPass *pPass = *itPass;
			ASSERT(pPass);

			SAFE_DELETE(pPass);
		}

		//
		// 2. ��ʼ��
		//
		Init();
	}

	//
	// �������
	//
	const CHAR* CMatPassGroup::GetName(VOID) const
	{
		return m_szName;
	}

	//
	// ��ø�����
	//
	const CMaterial* CMatPassGroup::GetParentMaterial(VOID) const
	{
		return m_pParentMaterial;
	}

	//
	// �����Ⱦͨ·��
	//
	INT CMatPassGroup::GetPassCount(VOID) const
	{
		return (INT)m_passes.size();
	}

	//
	// �����Ⱦͨ·
	//
	const CMatPass* CMatPassGroup::GetPass(INT index) const
	{
		return index >= 0 && index < (INT)m_passes.size() ? m_passes[index] : NULL;
	}

	//
	// �����Ⱦͨ·����
	//
	const CMatPassGroup::PassSet& CMatPassGroup::GetPasses(VOID) const
	{
		return m_passes;
	}

	//
	// ����XML
	//
	VOID CMatPassGroup::LoadXML(TiXmlNode *pPassGroupNode)
	{
		ASSERT(pPassGroupNode);

		//
		// 1. �ͷ���Ⱦͨ·��
		//
		Free();

		//
		// 2. ������Ⱦͨ·��
		//
		if (TiXmlNode *pPassNode = pPassGroupNode->FirstChild("Pass")) {
			do {
				CMatPass *pPass = SAFE_NEW(MEMTYPE_HEAP) CMatPass(m_pParentMaterial);
				ASSERT(pPass);

				m_passes.push_back(pPass); // ע��: ����ӵ���Ⱦͨ·����ټ�����Ⱦͨ·, 
						                   //       �����ڴ�й©!!!

				pPass->LoadXML(pPassNode);
			} while (pPassNode = pPassGroupNode->IterateChildren("Pass", pPassNode));
		}
	}

	//
	// ����XML
	//
	VOID CMatPassGroup::SaveXML(TiXmlNode *pPassGroupNode) const
	{
		ASSERT(pPassGroupNode);

		for (PassSet::const_iterator itPass = m_passes.begin(); itPass != m_passes.end(); ++itPass) {
			const CMatPass *pPass = *itPass;
			ASSERT(pPass);

			TiXmlElement *pPassNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Pass"); ASSERT(pPassNode);
			{
				pPass->SaveXML(pPassNode);
			}
			pPassGroupNode->LinkEndChild(pPassNode);
		}
	}

}

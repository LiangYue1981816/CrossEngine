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

	CMatSolution::CMatSolution(const CHAR *szName, CMaterial *pMaterial)
		: m_pParentMaterial(pMaterial)
	{
		Init(szName);
	}

	CMatSolution::~CMatSolution(VOID)
	{
		Free();
	}

	//
	// ��ʼ��
	//
	VOID CMatSolution::Init(VOID)
	{
		m_groups.clear();
	}

	//
	// ��ʼ����Դ
	//
	VOID CMatSolution::Init(const CHAR *szName)
	{
		ASSERT(szName);

		Init();
		strcpy(m_szName, szName);
	}

	//
	// �ͷ���Դ
	//
	VOID CMatSolution::Free(VOID)
	{
		//
		// 1. �ͷ���Ⱦͨ·��
		//
		for (PassGroupMap::const_iterator itPassGroup = m_groups.begin(); itPassGroup != m_groups.end(); ++itPassGroup) {
			const CMatPassGroup *pPassGroup = itPassGroup->second;
			ASSERT(pPassGroup);

			SAFE_DELETE(pPassGroup);
		}

		//
		// 2. ��ʼ��
		//
		Init();
	}

	//
	// �������
	//
	const CHAR* CMatSolution::GetName(VOID) const
	{
		return m_szName;
	}

	//
	// ��ø�����
	//
	const CMaterial* CMatSolution::GetParentMaterial(VOID) const
	{
		return m_pParentMaterial;
	}

	//
	// �����Ⱦͨ·��
	//
	const CMatPassGroup* CMatSolution::GetPassGroup(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetPassGroup(HashValue(szName));
	}

	//
	// �����Ⱦͨ·��
	//
	const CMatPassGroup* CMatSolution::GetPassGroup(DWORD dwName) const
	{
		PassGroupMap::const_iterator itPassGroup = m_groups.find(dwName);
		return itPassGroup != m_groups.end() ? itPassGroup->second : NULL;
	}

	//
	// �����Ⱦͨ·�鼯��
	//
	const CMatSolution::PassGroupMap& CMatSolution::GetPassGroups(VOID) const
	{
		return m_groups;
	}

	//
	// ����XML
	//
	VOID CMatSolution::LoadXML(TiXmlNode *pSolutionNode)
	{
		ASSERT(pSolutionNode);

		//
		// 1. �ͷ���Ⱦ����
		//
		Free();

		//
		// 2. ������Ⱦ����
		//
		if (TiXmlNode *pPassGroupNode = pSolutionNode->FirstChild("PassGroup")) {
			do {
				//
				// 2.1. �����Ⱦͨ·����
				//
				const CHAR *szName = pPassGroupNode->ToElement()->AttributeString("name");
				if (szName == NULL) throw "No group name.";

				//
				// 2.2. ����Ƿ��Ѿ�����
				//
				DWORD dwName = HashValue(szName);

				PassGroupMap::const_iterator itGroup = m_groups.find(dwName);
				if (itGroup != m_groups.end()) continue;

				//
				// 2.3. ������Ⱦͨ·��
				//
				CMatPassGroup *pPassGroup = SAFE_NEW(MEMTYPE_HEAP) CMatPassGroup(szName, m_pParentMaterial);
				ASSERT(pPassGroup);

				m_groups[dwName] = pPassGroup; // ע��: ����ӵ���Ⱦͨ·�鼯�Ϻ��ټ�����Ⱦͨ·��, 
				                               //       �����ڴ�й©!!!

				pPassGroup->LoadXML(pPassGroupNode);
			} while (pPassGroupNode = pSolutionNode->IterateChildren("PassGroup", pPassGroupNode));
		}
	}

	//
	// ����XML
	//
	VOID CMatSolution::SaveXML(TiXmlNode *pSolutionNode) const
	{
		ASSERT(pSolutionNode);

		for (PassGroupMap::const_iterator itPassGroup = m_groups.begin(); itPassGroup != m_groups.end(); ++itPassGroup) {
			const CMatPassGroup *pPassGroup = itPassGroup->second;
			ASSERT(pPassGroup);

			TiXmlElement *pPassGroupNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("PassGroup"); ASSERT(pPassGroupNode);
			{
				pPassGroupNode->SetAttributeString("name", pPassGroup->GetName());
				pPassGroup->SaveXML(pPassGroupNode);
			}
			pSolutionNode->LinkEndChild(pPassGroupNode);
		}
	}

}

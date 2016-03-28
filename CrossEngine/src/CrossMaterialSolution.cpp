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
	// 初始化
	//
	VOID CMatSolution::Init(VOID)
	{
		m_groups.clear();
	}

	//
	// 初始化资源
	//
	VOID CMatSolution::Init(const CHAR *szName)
	{
		ASSERT(szName);

		Init();
		strcpy(m_szName, szName);
	}

	//
	// 释放资源
	//
	VOID CMatSolution::Free(VOID)
	{
		//
		// 1. 释放渲染通路组
		//
		for (PassGroupMap::const_iterator itPassGroup = m_groups.begin(); itPassGroup != m_groups.end(); ++itPassGroup) {
			const CMatPassGroup *pPassGroup = itPassGroup->second;
			ASSERT(pPassGroup);

			SAFE_DELETE(pPassGroup);
		}

		//
		// 2. 初始化
		//
		Init();
	}

	//
	// 获得名称
	//
	const CHAR* CMatSolution::GetName(VOID) const
	{
		return m_szName;
	}

	//
	// 获得父材质
	//
	const CMaterial* CMatSolution::GetParentMaterial(VOID) const
	{
		return m_pParentMaterial;
	}

	//
	// 获得渲染通路组
	//
	const CMatPassGroup* CMatSolution::GetPassGroup(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetPassGroup(HashValue(szName));
	}

	//
	// 获得渲染通路组
	//
	const CMatPassGroup* CMatSolution::GetPassGroup(DWORD dwName) const
	{
		PassGroupMap::const_iterator itPassGroup = m_groups.find(dwName);
		return itPassGroup != m_groups.end() ? itPassGroup->second : NULL;
	}

	//
	// 获得渲染通路组集合
	//
	const CMatSolution::PassGroupMap& CMatSolution::GetPassGroups(VOID) const
	{
		return m_groups;
	}

	//
	// 加载XML
	//
	VOID CMatSolution::LoadXML(TiXmlNode *pSolutionNode)
	{
		ASSERT(pSolutionNode);

		//
		// 1. 释放渲染方案
		//
		Free();

		//
		// 2. 加载渲染方案
		//
		if (TiXmlNode *pPassGroupNode = pSolutionNode->FirstChild("PassGroup")) {
			do {
				//
				// 2.1. 获得渲染通路组名
				//
				const CHAR *szName = pPassGroupNode->ToElement()->AttributeString("name");
				if (szName == NULL) throw "No group name.";

				//
				// 2.2. 检查是否已经加载
				//
				DWORD dwName = HashValue(szName);

				PassGroupMap::const_iterator itGroup = m_groups.find(dwName);
				if (itGroup != m_groups.end()) continue;

				//
				// 2.3. 加载渲染通路组
				//
				CMatPassGroup *pPassGroup = SAFE_NEW(MEMTYPE_HEAP) CMatPassGroup(szName, m_pParentMaterial);
				ASSERT(pPassGroup);

				m_groups[dwName] = pPassGroup; // 注意: 先添加到渲染通路组集合后再加载渲染通路组, 
				                               //       避免内存泄漏!!!

				pPassGroup->LoadXML(pPassGroupNode);
			} while (pPassGroupNode = pSolutionNode->IterateChildren("PassGroup", pPassGroupNode));
		}
	}

	//
	// 保存XML
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

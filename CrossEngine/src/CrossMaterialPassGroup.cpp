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
	// 初始化
	//
	VOID CMatPassGroup::Init(VOID)
	{
		m_passes.clear();
	}

	//
	// 初始化资源
	//
	VOID CMatPassGroup::Init(const CHAR *szName)
	{
		ASSERT(szName);

		Init();
		strcpy(m_szName, szName);
	}

	//
	// 释放资源
	//
	VOID CMatPassGroup::Free(VOID)
	{
		//
		// 1. 释放渲染通路
		//
		for (PassSet::const_iterator itPass = m_passes.begin(); itPass != m_passes.end(); ++itPass) {
			const CMatPass *pPass = *itPass;
			ASSERT(pPass);

			SAFE_DELETE(pPass);
		}

		//
		// 2. 初始化
		//
		Init();
	}

	//
	// 获得名称
	//
	const CHAR* CMatPassGroup::GetName(VOID) const
	{
		return m_szName;
	}

	//
	// 获得父材质
	//
	const CMaterial* CMatPassGroup::GetParentMaterial(VOID) const
	{
		return m_pParentMaterial;
	}

	//
	// 获得渲染通路数
	//
	INT CMatPassGroup::GetPassCount(VOID) const
	{
		return (INT)m_passes.size();
	}

	//
	// 获得渲染通路
	//
	const CMatPass* CMatPassGroup::GetPass(INT index) const
	{
		return index >= 0 && index < (INT)m_passes.size() ? m_passes[index] : NULL;
	}

	//
	// 获得渲染通路集合
	//
	const CMatPassGroup::PassSet& CMatPassGroup::GetPasses(VOID) const
	{
		return m_passes;
	}

	//
	// 加载XML
	//
	VOID CMatPassGroup::LoadXML(TiXmlNode *pPassGroupNode)
	{
		ASSERT(pPassGroupNode);

		//
		// 1. 释放渲染通路组
		//
		Free();

		//
		// 2. 加载渲染通路组
		//
		if (TiXmlNode *pPassNode = pPassGroupNode->FirstChild("Pass")) {
			do {
				CMatPass *pPass = SAFE_NEW(MEMTYPE_HEAP) CMatPass(m_pParentMaterial);
				ASSERT(pPass);

				m_passes.push_back(pPass); // 注意: 先添加到渲染通路组后再加载渲染通路, 
						                   //       避免内存泄漏!!!

				pPass->LoadXML(pPassNode);
			} while (pPassNode = pPassGroupNode->IterateChildren("Pass", pPassNode));
		}
	}

	//
	// 保存XML
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

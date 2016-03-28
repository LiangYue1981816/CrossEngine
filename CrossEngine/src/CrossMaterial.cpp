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

	CMaterial::CMaterial(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
		, m_physics(this)
	{
		Init();
	}

	CMaterial::~CMaterial(VOID)
	{
		Free();
	}

	//
	// 初始化资源
	//
	VOID CMaterial::Init(VOID)
	{
		m_solutions.clear();
		CResource::Init();
	}

	//
	// 释放资源
	//
	VOID CMaterial::Free(VOID)
	{
		//
		// 1. 释放物理材质
		//
		m_physics.Free();

		//
		// 2. 释放渲染方案
		//
		for (SolutionMap::const_iterator itSolution = m_solutions.begin(); itSolution != m_solutions.end(); ++itSolution) {
			const CMatSolution *pSolution = itSolution->second;
			ASSERT(pSolution);

			SAFE_DELETE(pSolution);
		}

		CResource::Free();
	}

	//
	// 获得类型
	//
	CResource::TYPE CMaterial::GetType(VOID) const
	{
		return CResource::MATERIAL;
	}

	//
	// 有效性检查
	//
	BOOL CMaterial::IsValid(VOID) const
	{
		return TRUE;
	}

	//
	// 获得物理
	//
	const CMatPhysics* CMaterial::GetPhysics(VOID) const
	{
		return &m_physics;
	}

	//
	// 获得材质方案
	//
	const CMatSolution* CMaterial::GetSolution(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetSolution(HashValue(szName));
	}

	//
	// 获得材质方案
	//
	const CMatSolution* CMaterial::GetSolution(DWORD dwName) const
	{
		SolutionMap::const_iterator itSolution = m_solutions.find(dwName);
		return itSolution != m_solutions.end() ? itSolution->second : NULL;
	}

	//
	// 获得材质方案集合
	//
	const CMaterial::SolutionMap& CMaterial::GetSolutions(VOID) const
	{
		return m_solutions;
	}

	//
	// 从数据流加载资源
	//
	BOOL CMaterial::LoadFromStream(CStream *pStream)
	{
		//
		// 1. 数据流有效性检查
		//
		if (pStream == NULL) {
			return FALSE;
		}

		if (pStream->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 释放材质
		//
		Free();

		//
		// 3. 加载材质
		//
		try {
			//
			// 3.1. 打开XML
			//
			TiXmlDocument doc;
			if (doc.LoadFile((CHAR *)pStream->GetCurrentAddress(), pStream->GetFreeSize()) == FALSE) {
				throw "Invalid xml format.";
			}

			//
			// 3.2. 获得节点
			//
			TiXmlNode *pMaterialNode = doc.FirstChild("Material");
			if (pMaterialNode == NULL) throw "Invalid material format.";

			//
			// 3.3. 加载物理材质
			//
			LoadPhysics(pMaterialNode);

			//
			// 3.4. 加载材质方案
			//
			LoadSolutions(pMaterialNode);

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CMaterial::LoadFromStream: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// 加载物理
	//
	VOID CMaterial::LoadPhysics(TiXmlNode *pMaterialNode)
	{
		ASSERT(pMaterialNode);

		if (TiXmlNode *pPhysicsNode = pMaterialNode->FirstChild("Physics")) {
			m_physics.LoadXML(pPhysicsNode);
		}
	}

	//
	// 加载材质方案
	//
	VOID CMaterial::LoadSolutions(TiXmlNode *pMaterialNode)
	{
		ASSERT(pMaterialNode);

		if (TiXmlNode *pSolutionNode = pMaterialNode->FirstChild("Solution")) {
			do {
				//
				// 1. 获得材质方案名
				//
				const CHAR *szName = pSolutionNode->ToElement()->AttributeString("name");
				if (szName == NULL) throw "No solution name.";

				//
				// 2. 检查是否已经加载
				//
				DWORD dwName = HashValue(szName);

				SolutionMap::const_iterator itSolution = m_solutions.find(dwName);
				if (itSolution != m_solutions.end()) continue;;

				//
				// 3. 加载材质方案
				//
				CMatSolution *pSolution = SAFE_NEW(MEMTYPE_HEAP) CMatSolution(szName, this);
				ASSERT(pSolution);

				m_solutions[dwName] = pSolution; // 注意: 先添加到材质方案集合后再加载材质方案, 
				                                 //       避免内存泄漏!!!

				pSolution->LoadXML(pSolutionNode);
			} while (pSolutionNode = pMaterialNode->IterateChildren("Solution", pSolutionNode));
		}
	}

	//
	// 保存到文件数据流
	//
	BOOL CMaterial::SaveToFileStream(FILE *pFile)
	{
		//
		// 1. 数据流有效性检查
		//
		if (pFile == NULL) {
			return FALSE;
		}

		//
		// 2. 保存材质
		//
		TiXmlDocument doc;
		TiXmlElement *pMaterialNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Material"); ASSERT(pMaterialNode);
		{
			SavePhysics(pMaterialNode);
			SaveSolutions(pMaterialNode);
		}
		doc.LinkEndChild(pMaterialNode);
		return doc.SaveFile(pFile) ? TRUE : FALSE;
	}

	//
	// 保存物理
	//
	VOID CMaterial::SavePhysics(TiXmlNode *pMaterialNode)
	{
		ASSERT(pMaterialNode);

		TiXmlElement *pPhysicsNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Physics"); ASSERT(pPhysicsNode);
		{
			m_physics.SaveXML(pPhysicsNode);
		}
		pMaterialNode->LinkEndChild(pPhysicsNode);
	}

	//
	// 保存渲染方案
	//
	VOID CMaterial::SaveSolutions(TiXmlNode *pMaterialNode)
	{
		ASSERT(pMaterialNode);

		for (SolutionMap::const_iterator itSolution = m_solutions.begin(); itSolution != m_solutions.end(); ++itSolution) {
			const CMatSolution *pSolution = itSolution->second;
			ASSERT(pSolution);

			TiXmlElement *pSolutionNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Solution"); ASSERT(pSolutionNode);
			{
				pSolutionNode->SetAttributeString("name", pSolution->GetName());
				pSolution->SaveXML(pSolutionNode);
			}
			pMaterialNode->LinkEndChild(pSolutionNode);
		}
	}

}

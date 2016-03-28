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

	CRenderSolution::CRenderSolution(CRenderFlow *pRenderFlow)
		: m_pRenderFlow(pRenderFlow)
	{
		Init();
	}

	CRenderSolution::~CRenderSolution(VOID)
	{
		Free();
	}

	//
	// ��ʼ��
	//
	VOID CRenderSolution::Init(VOID)
	{
		m_renderStageSet.clear();
		m_renderStageMap.clear();
	}

	//
	// �ͷ�
	//
	VOID CRenderSolution::Free(VOID)
	{
		//
		// 1. �ͷ���Ⱦ����
		//
		for (RenderStageSet::const_iterator itRenderStage = m_renderStageSet.begin(); itRenderStage != m_renderStageSet.end(); ++itRenderStage) {
			const CRenderStage *pRenderStage = *itRenderStage;
			ASSERT(pRenderStage);

			SAFE_DELETE(pRenderStage);
		}

		//
		// 2. ��ʼ��
		//
		Init();
	}

	//
	// ������Ⱦ����
	//
	VOID CRenderSolution::LoadFromStream(TiXmlNode *pRenderSolutionNode)
	{
		ASSERT(pRenderSolutionNode);

		if (TiXmlNode *pRenderStageNode = pRenderSolutionNode->FirstChild("RenderStage")) {
			do {
				//
				// 1. �����Ⱦ������
				//
				const CHAR *szName = pRenderStageNode->ToElement()->AttributeString("name");
				if (szName == NULL) throw "No render stage name.";

				//
				// 2. ����Ƿ��Ѿ�����
				//
				DWORD dwName = HashValue(szName);
				RenderStageMap::const_iterator itRenderStage = m_renderStageMap.find(dwName);
				if (itRenderStage != m_renderStageMap.end()) continue;

				//
				// 3. ������Ⱦ����
				//
				CRenderStage *pRenderStage = SAFE_NEW(MEMTYPE_HEAP) CRenderStage(m_pRenderFlow);
				ASSERT(pRenderStage);

				m_renderStageSet.push_back(pRenderStage);
				m_renderStageMap[dwName] = pRenderStage; // ע��: ����ӵ���Ⱦ���̼��Ϻ��ټ�����Ⱦ����, 
				                                         //�����ڴ�й©!!!

				pRenderStage->LoadFromStream(pRenderStageNode);
			} while (pRenderStageNode = pRenderSolutionNode->IterateChildren("RenderStage", pRenderStageNode));
		}
	}

	//
	// ����
	//
	VOID CRenderSolution::Create(VOID)
	{
		for (RenderStageSet::const_iterator itRenderStage = m_renderStageSet.begin(); itRenderStage != m_renderStageSet.end(); ++itRenderStage) {
			CRenderStage *pRenderStage = *itRenderStage;
			ASSERT(pRenderStage);

			pRenderStage->Create();
		}
	}

	//
	// ����
	//
	VOID CRenderSolution::Destroy(VOID)
	{
		for (RenderStageSet::const_iterator itRenderStage = m_renderStageSet.begin(); itRenderStage != m_renderStageSet.end(); ++itRenderStage) {
			CRenderStage *pRenderStage = *itRenderStage;
			ASSERT(pRenderStage);

			pRenderStage->Destroy();
		}
	}

	//
	// �����Ⱦ����
	//
	CRenderStage* CRenderSolution::GetRenderStage(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetRenderStage(HashValue(szName));
	}

	//
	// �����Ⱦ����
	//
	CRenderStage* CRenderSolution::GetRenderStage(DWORD dwName) const
	{
		RenderStageMap::const_iterator itRenderStage = m_renderStageMap.find(dwName);
		return itRenderStage != m_renderStageMap.end() ? itRenderStage->second : NULL;
	}

	//
	// ��Ⱦ
	//
	VOID CRenderSolution::Render(DWORD dwSceneManager)
	{
		for (RenderStageSet::const_iterator itRenderStage = m_renderStageSet.begin(); itRenderStage != m_renderStageSet.end(); ++itRenderStage) {
			CRenderStage *pRenderStage = *itRenderStage;
			ASSERT(pRenderStage);

			pRenderStage->Render(dwSceneManager);
		}
	}

}

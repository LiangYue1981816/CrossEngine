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

	CRenderFlow::CRenderFlow(VOID)
	{
		Init();
	}

	CRenderFlow::~CRenderFlow(VOID)
	{
		Destroy();
		Free();
	}

	//
	// ��ʼ����Դ
	//
	VOID CRenderFlow::Init(VOID)
	{
		m_scalePPI = 1.0f;

		m_renderTargets.clear();
		m_renderSolutions.clear();
		m_renderCallbacks.clear();
	}

	//
	// �ͷ���Դ
	//
	VOID CRenderFlow::Free(VOID)
	{
		//
		// 1. �ͷ���ȾĿ��
		//
		for (RenderTargetMap::const_iterator itRenderTarget = m_renderTargets.begin(); itRenderTarget != m_renderTargets.end(); ++itRenderTarget) {
			const CRenderTarget *pRenderTarget = itRenderTarget->second;
			ASSERT(pRenderTarget);

			SAFE_DELETE(pRenderTarget);
		}

		//
		// 2. �ͷ���Ⱦ����
		//
		for (RenderSolutionMap::const_iterator itRenderSolution = m_renderSolutions.begin(); itRenderSolution != m_renderSolutions.end(); ++itRenderSolution) {
			const CRenderSolution *pRenderSolution = itRenderSolution->second;
			ASSERT(pRenderSolution);

			SAFE_DELETE(pRenderSolution);
		}

		//
		// 3. ��ʼ��
		//
		Init();
	}

	//
	// ����
	//
	VOID CRenderFlow::Create(VOID)
	{
		//
		// 1. ������ȾĿ��
		//
		for (RenderTargetMap::const_iterator itRenderTarget = m_renderTargets.begin(); itRenderTarget != m_renderTargets.end(); ++itRenderTarget) {
			CRenderTarget *pRenderTarget = itRenderTarget->second;
			ASSERT(pRenderTarget);

			pRenderTarget->Create();
		}

		//
		// 2. ������Ⱦ����
		//
		for (RenderSolutionMap::const_iterator itRenderSolution = m_renderSolutions.begin(); itRenderSolution != m_renderSolutions.end(); ++itRenderSolution) {
			CRenderSolution *pRenderSolution = itRenderSolution->second;
			ASSERT(pRenderSolution);

			pRenderSolution->Create();
		}
	}

	//
	// ����
	//
	VOID CRenderFlow::Destroy(VOID)
	{
		//
		// 1. ������ȾĿ��
		//
		for (RenderTargetMap::const_iterator itRenderTarget = m_renderTargets.begin(); itRenderTarget != m_renderTargets.end(); ++itRenderTarget) {
			CRenderTarget *pRenderTarget = itRenderTarget->second;
			ASSERT(pRenderTarget);

			pRenderTarget->Destroy();
		}

		//
		// 2. ������Ⱦ����
		//
		for (RenderSolutionMap::const_iterator itRenderSolution = m_renderSolutions.begin(); itRenderSolution != m_renderSolutions.end(); ++itRenderSolution) {
			CRenderSolution *pRenderSolution = itRenderSolution->second;
			ASSERT(pRenderSolution);

			pRenderSolution->Destroy();
		}
	}

	//
	// ���ļ�������Դ
	//
	BOOL CRenderFlow::LoadFromFile(const CHAR *szFileName, FLOAT scalePPI)
	{
		//
		// 1. ������ȫ���
		//
		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. �ͷ���Դ
		//
		Free();

		//
		// 3. ������Դ
		//
		try {
			WriteLogI("Load RenderFlow from file: %s\n", szFileName);

			if (m_stream.LoadFromFile(szFileName) == FALSE) throw "Load file failed.";
			if (LoadFromStream(&m_stream, scalePPI) == FALSE) throw "Load from stream failed.";

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CRenderFlow::LoadFromFile: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ��ѹ����������Դ
	//
	BOOL CRenderFlow::LoadFromZip(const CHAR *szZipName, const CHAR *szFileName, FLOAT scalePPI)
	{
		//
		// 1. ������ȫ���
		//
		if (szZipName == NULL) {
			return FALSE;
		}

		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. �ͷ���Դ
		//
		Free();

		//
		// 3. ������Դ
		//
		try {
			WriteLogI("Load RenderFlow from zip: %s<%s>\n", szZipName, szFileName);

			if (m_stream.LoadFromZip(szZipName, szFileName) == FALSE) throw "Load zip failed.";
			if (LoadFromStream(&m_stream, scalePPI) == FALSE) throw "Load from stream failed.";

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CRenderFlow::LoadFromZip: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ��ѹ�����ļ�����������
	//
	BOOL CRenderFlow::LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName, FLOAT scalePPI)
	{
		//
		// 1. ������ȫ���
		//
		if (pZipPack == NULL) {
			return FALSE;
		}

		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. �ͷ���Դ
		//
		Free();

		//
		// 3. ������Դ
		//
		try {
			WriteLogI("Load RenderFlow from zip: <%s>\n", szFileName);

			if (m_stream.LoadFromZip(pZipPack, szFileName) == FALSE) throw "Load zip failed.";
			if (LoadFromStream(&m_stream, m_scalePPI) == FALSE) throw "Load from stream failed.";

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CRenderFlow::LoadFromZip: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ��������������Դ
	//
	BOOL CRenderFlow::LoadFromStream(CStream *pStream, FLOAT scalePPI)
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
		// 2. �ͷ���Ⱦ����
		//
		Destroy();
		Free();

		//
		// 3. ������Ⱦ����
		//
		try {
			//
			// 3.1. �������ű�
			//
			m_scalePPI = scalePPI;

			//
			// 3.2. ��XML
			//
			TiXmlDocument doc;
			if (doc.LoadFile((CHAR *)pStream->GetCurrentAddress(), pStream->GetFreeSize()) == FALSE) {
				throw "Invalid xml format.";
			}

			//
			// 3.3. �����Ⱦ���̽ڵ�
			//
			TiXmlNode *pRenderFlowNode = doc.FirstChild("RenderFlow");
			if (pRenderFlowNode == NULL) throw "Invalid render flow format.";

			//
			// 3.4. ������Ⱦ����
			//
			LoadRenderTargets(pRenderFlowNode);
			LoadRenderSolutions(pRenderFlowNode);

			//
			// 3.5. ������Ⱦ����
			//
			Create();

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CRenderFlow::LoadFromStream: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ������Ⱦ����
	//
	VOID CRenderFlow::LoadRenderTargets(TiXmlNode *pRenderFlowNode)
	{
		ASSERT(pRenderFlowNode);

		if (TiXmlNode *pRenderTargetNode = pRenderFlowNode->FirstChild("RenderTarget")) {
			do {
				//
				// 1. �����Ⱦ������
				//
				const CHAR *szName = pRenderTargetNode->ToElement()->AttributeString("category");
				if (szName == NULL) throw "No render target category.";

				//
				// 2. ����Ƿ��Ѿ�����
				//
				DWORD dwName = HashValue(szName);
				RenderTargetMap::const_iterator itRenderTarget = m_renderTargets.find(dwName);
				if (itRenderTarget != m_renderTargets.end()) continue;

				//
				// 3. ������Ⱦ����
				//
				CRenderTarget *pRenderTarget = SAFE_NEW(MEMTYPE_HEAP) CRenderTarget(this);
				ASSERT(pRenderTarget);

				m_renderTargets[dwName] = pRenderTarget; // ע��: ����ӵ���Ⱦ���󼯺Ϻ��ټ�����Ⱦ����, 
				                                         //       �����ڴ�й©!!!

				pRenderTarget->LoadFromStream(pRenderTargetNode);
			} while (pRenderTargetNode = pRenderFlowNode->IterateChildren("RenderTarget", pRenderTargetNode));
		}
	}

	//
	// ������Ⱦ����
	//
	VOID CRenderFlow::LoadRenderSolutions(TiXmlNode *pRenderFlowNode)
	{
		ASSERT(pRenderFlowNode);

		if (TiXmlNode *pRenderSolutionNode = pRenderFlowNode->FirstChild("RenderSolution")) {
			do {
				//
				// 1. �����Ⱦ������
				//
				const CHAR *szName = pRenderSolutionNode->ToElement()->AttributeString("name");
				if (szName == NULL) throw "No render solution name.";

				//
				// 2. ����Ƿ��Ѿ�����
				//
				DWORD dwName = HashValue(szName);
				RenderSolutionMap::const_iterator itRenderSolution = m_renderSolutions.find(dwName);
				if (itRenderSolution != m_renderSolutions.end()) continue;

				//
				// 3. ������Ⱦ����
				//
				CRenderSolution *pRenderSolution = SAFE_NEW(MEMTYPE_HEAP) CRenderSolution(this);
				ASSERT(pRenderSolution);

				m_renderSolutions[dwName] = pRenderSolution; // ע��: ����ӵ���Ⱦ�������Ϻ��ټ�����Ⱦ����, 
				                                             //       �����ڴ�й©!!!

				pRenderSolution->LoadFromStream(pRenderSolutionNode);
			} while (pRenderSolutionNode = pRenderFlowNode->IterateChildren("RenderSolution", pRenderSolutionNode));
		}
	}

	//
	// ���¼�����Դ
	//
	BOOL CRenderFlow::Reload(FLOAT scalePPI)
	{
		//
		// 1. �ض�λ������
		//
		if (m_stream.Seek(0, CStream::SEEK_MODE_SET) == FALSE) {
			return FALSE;
		}

		//
		// 2. ���¼���
		//
		return LoadFromStream(&m_stream, scalePPI);
	}

	//
	// ���PPI���ű�
	//
	FLOAT CRenderFlow::GetScalePPI(VOID) const
	{
		return m_scalePPI;
	}

	//
	// �����Ⱦ����
	//
	CRenderSolution* CRenderFlow::GetRenderSolution(DWORD dwName) const
	{
		RenderSolutionMap::const_iterator itRenderSolution = m_renderSolutions.find(dwName);
		return itRenderSolution != m_renderSolutions.end() ? itRenderSolution->second: NULL;
	}

	//
	// �����Ⱦ����
	//
	CRenderTarget* CRenderFlow::GetRenderTarget(DWORD dwName) const
	{
		RenderTargetMap::const_iterator itRenderTarget = m_renderTargets.find(dwName);
		return itRenderTarget != m_renderTargets.end() ? itRenderTarget->second : NULL;
	}

	//
	// ������Ⱦ�ص�����
	//
	BOOL CRenderFlow::SetRenderCallback(DWORD dwName, RenderCallback fnCallback)
	{
		m_renderCallbacks[dwName] = fnCallback;
		return TRUE;
	}

	//
	// �����Ⱦ�ص�����
	//
	RenderCallback CRenderFlow::GetRenderCallback(DWORD dwName)
	{
		RenderCallbackMap::const_iterator itRenderCallback = m_renderCallbacks.find(dwName);
		return itRenderCallback != m_renderCallbacks.end() ? itRenderCallback->second : NULL;
	}

	//
	// ��Ⱦ
	//
	VOID CRenderFlow::Render(DWORD dwSceneManager, DWORD dwSolution)
	{
		//
		// 1. ������Ⱦ����
		//
		RenderSolutionMap::const_iterator itRenderSolution = m_renderSolutions.find(dwSolution);
		if (itRenderSolution == m_renderSolutions.end()) return;

		//
		// 2. ִ����Ⱦ����
		//
		CRenderSolution *pRenderSolution = itRenderSolution->second;
		ASSERT(pRenderSolution);

		pRenderSolution->Render(dwSceneManager);
	}

}

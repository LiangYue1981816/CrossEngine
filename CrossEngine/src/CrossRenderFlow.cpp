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
	// 初始化资源
	//
	VOID CRenderFlow::Init(VOID)
	{
		m_scalePPI = 1.0f;

		m_renderTargets.clear();
		m_renderSolutions.clear();
		m_renderCallbacks.clear();
	}

	//
	// 释放资源
	//
	VOID CRenderFlow::Free(VOID)
	{
		//
		// 1. 释放渲染目标
		//
		for (RenderTargetMap::const_iterator itRenderTarget = m_renderTargets.begin(); itRenderTarget != m_renderTargets.end(); ++itRenderTarget) {
			const CRenderTarget *pRenderTarget = itRenderTarget->second;
			ASSERT(pRenderTarget);

			SAFE_DELETE(pRenderTarget);
		}

		//
		// 2. 释放渲染方案
		//
		for (RenderSolutionMap::const_iterator itRenderSolution = m_renderSolutions.begin(); itRenderSolution != m_renderSolutions.end(); ++itRenderSolution) {
			const CRenderSolution *pRenderSolution = itRenderSolution->second;
			ASSERT(pRenderSolution);

			SAFE_DELETE(pRenderSolution);
		}

		//
		// 3. 初始化
		//
		Init();
	}

	//
	// 创建
	//
	VOID CRenderFlow::Create(VOID)
	{
		//
		// 1. 创建渲染目标
		//
		for (RenderTargetMap::const_iterator itRenderTarget = m_renderTargets.begin(); itRenderTarget != m_renderTargets.end(); ++itRenderTarget) {
			CRenderTarget *pRenderTarget = itRenderTarget->second;
			ASSERT(pRenderTarget);

			pRenderTarget->Create();
		}

		//
		// 2. 创建渲染方案
		//
		for (RenderSolutionMap::const_iterator itRenderSolution = m_renderSolutions.begin(); itRenderSolution != m_renderSolutions.end(); ++itRenderSolution) {
			CRenderSolution *pRenderSolution = itRenderSolution->second;
			ASSERT(pRenderSolution);

			pRenderSolution->Create();
		}
	}

	//
	// 销毁
	//
	VOID CRenderFlow::Destroy(VOID)
	{
		//
		// 1. 销毁渲染目标
		//
		for (RenderTargetMap::const_iterator itRenderTarget = m_renderTargets.begin(); itRenderTarget != m_renderTargets.end(); ++itRenderTarget) {
			CRenderTarget *pRenderTarget = itRenderTarget->second;
			ASSERT(pRenderTarget);

			pRenderTarget->Destroy();
		}

		//
		// 2. 销毁渲染方案
		//
		for (RenderSolutionMap::const_iterator itRenderSolution = m_renderSolutions.begin(); itRenderSolution != m_renderSolutions.end(); ++itRenderSolution) {
			CRenderSolution *pRenderSolution = itRenderSolution->second;
			ASSERT(pRenderSolution);

			pRenderSolution->Destroy();
		}
	}

	//
	// 从文件加载资源
	//
	BOOL CRenderFlow::LoadFromFile(const CHAR *szFileName, FLOAT scalePPI)
	{
		//
		// 1. 参数安全检查
		//
		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. 释放资源
		//
		Free();

		//
		// 3. 加载资源
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
	// 从压缩包加载资源
	//
	BOOL CRenderFlow::LoadFromZip(const CHAR *szZipName, const CHAR *szFileName, FLOAT scalePPI)
	{
		//
		// 1. 参数安全检查
		//
		if (szZipName == NULL) {
			return FALSE;
		}

		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. 释放资源
		//
		Free();

		//
		// 3. 加载资源
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
	// 从压缩包文件加载数据流
	//
	BOOL CRenderFlow::LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName, FLOAT scalePPI)
	{
		//
		// 1. 参数安全检查
		//
		if (pZipPack == NULL) {
			return FALSE;
		}

		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. 释放资源
		//
		Free();

		//
		// 3. 加载资源
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
	// 从数据流加载资源
	//
	BOOL CRenderFlow::LoadFromStream(CStream *pStream, FLOAT scalePPI)
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
		// 2. 释放渲染流程
		//
		Destroy();
		Free();

		//
		// 3. 加载渲染流程
		//
		try {
			//
			// 3.1. 设置缩放比
			//
			m_scalePPI = scalePPI;

			//
			// 3.2. 打开XML
			//
			TiXmlDocument doc;
			if (doc.LoadFile((CHAR *)pStream->GetCurrentAddress(), pStream->GetFreeSize()) == FALSE) {
				throw "Invalid xml format.";
			}

			//
			// 3.3. 获得渲染流程节点
			//
			TiXmlNode *pRenderFlowNode = doc.FirstChild("RenderFlow");
			if (pRenderFlowNode == NULL) throw "Invalid render flow format.";

			//
			// 3.4. 加载渲染流程
			//
			LoadRenderTargets(pRenderFlowNode);
			LoadRenderSolutions(pRenderFlowNode);

			//
			// 3.5. 创建渲染流程
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
	// 加载渲染对象
	//
	VOID CRenderFlow::LoadRenderTargets(TiXmlNode *pRenderFlowNode)
	{
		ASSERT(pRenderFlowNode);

		if (TiXmlNode *pRenderTargetNode = pRenderFlowNode->FirstChild("RenderTarget")) {
			do {
				//
				// 1. 获得渲染对象名
				//
				const CHAR *szName = pRenderTargetNode->ToElement()->AttributeString("category");
				if (szName == NULL) throw "No render target category.";

				//
				// 2. 检查是否已经加载
				//
				DWORD dwName = HashValue(szName);
				RenderTargetMap::const_iterator itRenderTarget = m_renderTargets.find(dwName);
				if (itRenderTarget != m_renderTargets.end()) continue;

				//
				// 3. 加载渲染对象
				//
				CRenderTarget *pRenderTarget = SAFE_NEW(MEMTYPE_HEAP) CRenderTarget(this);
				ASSERT(pRenderTarget);

				m_renderTargets[dwName] = pRenderTarget; // 注意: 先添加到渲染对象集合后再加载渲染对象, 
				                                         //       避免内存泄漏!!!

				pRenderTarget->LoadFromStream(pRenderTargetNode);
			} while (pRenderTargetNode = pRenderFlowNode->IterateChildren("RenderTarget", pRenderTargetNode));
		}
	}

	//
	// 加载渲染方案
	//
	VOID CRenderFlow::LoadRenderSolutions(TiXmlNode *pRenderFlowNode)
	{
		ASSERT(pRenderFlowNode);

		if (TiXmlNode *pRenderSolutionNode = pRenderFlowNode->FirstChild("RenderSolution")) {
			do {
				//
				// 1. 获得渲染方案名
				//
				const CHAR *szName = pRenderSolutionNode->ToElement()->AttributeString("name");
				if (szName == NULL) throw "No render solution name.";

				//
				// 2. 检查是否已经加载
				//
				DWORD dwName = HashValue(szName);
				RenderSolutionMap::const_iterator itRenderSolution = m_renderSolutions.find(dwName);
				if (itRenderSolution != m_renderSolutions.end()) continue;

				//
				// 3. 加载渲染方案
				//
				CRenderSolution *pRenderSolution = SAFE_NEW(MEMTYPE_HEAP) CRenderSolution(this);
				ASSERT(pRenderSolution);

				m_renderSolutions[dwName] = pRenderSolution; // 注意: 先添加到渲染方案集合后再加载渲染方案, 
				                                             //       避免内存泄漏!!!

				pRenderSolution->LoadFromStream(pRenderSolutionNode);
			} while (pRenderSolutionNode = pRenderFlowNode->IterateChildren("RenderSolution", pRenderSolutionNode));
		}
	}

	//
	// 重新加载资源
	//
	BOOL CRenderFlow::Reload(FLOAT scalePPI)
	{
		//
		// 1. 重定位数据流
		//
		if (m_stream.Seek(0, CStream::SEEK_MODE_SET) == FALSE) {
			return FALSE;
		}

		//
		// 2. 重新加载
		//
		return LoadFromStream(&m_stream, scalePPI);
	}

	//
	// 获得PPI缩放比
	//
	FLOAT CRenderFlow::GetScalePPI(VOID) const
	{
		return m_scalePPI;
	}

	//
	// 获得渲染方案
	//
	CRenderSolution* CRenderFlow::GetRenderSolution(DWORD dwName) const
	{
		RenderSolutionMap::const_iterator itRenderSolution = m_renderSolutions.find(dwName);
		return itRenderSolution != m_renderSolutions.end() ? itRenderSolution->second: NULL;
	}

	//
	// 获得渲染对象
	//
	CRenderTarget* CRenderFlow::GetRenderTarget(DWORD dwName) const
	{
		RenderTargetMap::const_iterator itRenderTarget = m_renderTargets.find(dwName);
		return itRenderTarget != m_renderTargets.end() ? itRenderTarget->second : NULL;
	}

	//
	// 设置渲染回调函数
	//
	BOOL CRenderFlow::SetRenderCallback(DWORD dwName, RenderCallback fnCallback)
	{
		m_renderCallbacks[dwName] = fnCallback;
		return TRUE;
	}

	//
	// 获得渲染回调函数
	//
	RenderCallback CRenderFlow::GetRenderCallback(DWORD dwName)
	{
		RenderCallbackMap::const_iterator itRenderCallback = m_renderCallbacks.find(dwName);
		return itRenderCallback != m_renderCallbacks.end() ? itRenderCallback->second : NULL;
	}

	//
	// 渲染
	//
	VOID CRenderFlow::Render(DWORD dwSceneManager, DWORD dwSolution)
	{
		//
		// 1. 查找渲染方案
		//
		RenderSolutionMap::const_iterator itRenderSolution = m_renderSolutions.find(dwSolution);
		if (itRenderSolution == m_renderSolutions.end()) return;

		//
		// 2. 执行渲染方案
		//
		CRenderSolution *pRenderSolution = itRenderSolution->second;
		ASSERT(pRenderSolution);

		pRenderSolution->Render(dwSceneManager);
	}

}

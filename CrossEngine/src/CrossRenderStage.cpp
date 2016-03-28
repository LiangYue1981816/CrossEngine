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

	CRenderStage::CRenderStage(CRenderFlow *pRenderFlow)
		: m_pRenderFlow(pRenderFlow)

		, m_lastViewportX(0)
		, m_lastViewportY(0)
		, m_lastViewportWidth(0)
		, m_lastViewportHeight(0)
		, m_lastFrameBuffer(0)

		, m_dwRenderTime(0)
	{
		Init();
	}

	CRenderStage::~CRenderStage(VOID)
	{
		Free();
	}

	//
	// 初始化
	//
	VOID CRenderStage::Init(VOID)
	{
		memset(m_szName, 0, sizeof(m_szName));
		m_type = SCENE;
		m_bEnable = FALSE;

		m_dwBeginRenderCallback = INVALID_HASHNAME;
		m_dwEndRenderCallback = INVALID_HASHNAME;

		m_clearDepth = 1.0f;
		m_clearColor[0] = 0.0f;
		m_clearColor[1] = 0.0f;
		m_clearColor[2] = 0.0f;

		m_bClearColorBuffer = FALSE;
		m_bClearDepthBuffer = FALSE;

		m_dwMatBeginRenderID = INVALID_VALUE;
		m_dwMatEndRenderID = INVALID_VALUE;
		m_dwMatSolution = INVALID_HASHNAME;
		m_dwMatPassGroup = INVALID_HASHNAME;

		m_inputTextures.clear();
		m_outputTextures.clear();

		m_vertex = 0;
		m_fragment = 0;
		m_params.clear();

		m_frameBuffer = 0;
		m_frameBufferWidth = 0;
		m_frameBufferHeight = 0;

		m_numDraws = 0;
		memset(m_drawBuffers, 0, sizeof(m_drawBuffers));
	}

	//
	// 释放
	//
	VOID CRenderStage::Free(VOID)
	{
		//
		// 1. 释放纹理
		//
		for (TextureSet::const_iterator itTexture = m_inputTextures.begin(); itTexture != m_inputTextures.end(); ++itTexture) {
			const TEXTURE *pTexture = *itTexture;
			ASSERT(pTexture);

			SAFE_DELETE(pTexture);
		}

		for (TextureSet::const_iterator itTexture = m_outputTextures.begin(); itTexture != m_outputTextures.end(); ++itTexture) {
			const TEXTURE *pTexture = *itTexture;
			ASSERT(pTexture);

			SAFE_DELETE(pTexture);
		}

		//
		// 2. 初始化
		//
		Init();
	}

	//
	// 加载渲染过程
	//
	VOID CRenderStage::LoadFromStream(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		LoadName(pRenderStageNode);
		LoadType(pRenderStageNode);
		LoadEnable(pRenderStageNode);
		LoadCallback(pRenderStageNode);
		LoadClearBuffer(pRenderStageNode);
		LoadMaterial(pRenderStageNode);
		LoadInputTextures(pRenderStageNode);
		LoadOutputTextures(pRenderStageNode);
		LoadShaderVertex(pRenderStageNode);
		LoadShaderFragment(pRenderStageNode);
	}

	//
	// 加载名称
	//
	VOID CRenderStage::LoadName(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		const CHAR* szName = pRenderStageNode->ToElement()->AttributeString("name");
		if (szName == NULL) throw "No render state name.";

		strcpy(m_szName, szName);
	}

	//
	// 加载类型
	//
	VOID CRenderStage::LoadType(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		const CHAR *szType = pRenderStageNode->ToElement()->AttributeString("type");
		if (szType == NULL) throw "No render stage type.";

		if      (stricmp(szType, "SCENE") == 0) m_type = SCENE;
		else if (stricmp(szType, "LIGHT") == 0) m_type = LIGHT;
		else if (stricmp(szType, "SCREEN") == 0) m_type = SCREEN;
		else if (stricmp(szType, "SHADOW_CAST") == 0) m_type = SHADOW_CAST;
		else if (stricmp(szType, "SHADOW_RECEIVE") == 0) m_type = SHADOW_RECEIVE;
		else throw "Invalid render stage type.";
	}

	//
	// 加载状态
	//
	VOID CRenderStage::LoadEnable(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);
		m_bEnable = cglStringToAttrib(pRenderStageNode->ToElement()->AttributeString("enable"));
	}

	//
	// 加载回调
	//
	VOID CRenderStage::LoadCallback(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		if (TiXmlNode *pBeginRenderCallbackNode = pRenderStageNode->FirstChild("BeginRenderCallback")) {
			m_dwBeginRenderCallback = HashValue(pBeginRenderCallbackNode->ToElement()->AttributeString("name"));
		}

		if (TiXmlNode *pEndRenderCallbackNode = pRenderStageNode->FirstChild("EndRenderCallback")) {
			m_dwEndRenderCallback = HashValue(pEndRenderCallbackNode->ToElement()->AttributeString("name"));
		}
	}

	//
	// 加载清理缓存
	//
	VOID CRenderStage::LoadClearBuffer(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		if (TiXmlNode *pClearColorBufferNode = pRenderStageNode->FirstChild("ClearColorBuffer")) {
			m_bClearColorBuffer = cglStringToAttrib(pClearColorBufferNode->ToElement()->AttributeString("enable"));
			sscanf(pClearColorBufferNode->ToElement()->AttributeString("value"), "%f %f %f", &m_clearColor[0], &m_clearColor[1], &m_clearColor[2]);
		}

		if (TiXmlNode *pClearDepthBufferNode = pRenderStageNode->FirstChild("ClearDepthBuffer")) {
			m_bClearDepthBuffer = cglStringToAttrib(pClearDepthBufferNode->ToElement()->AttributeString("enable"));
			m_clearDepth = pClearDepthBufferNode->ToElement()->AttributeFloat("value");
		}
	}

	//
	// 加载材质
	//
	VOID CRenderStage::LoadMaterial(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		if (TiXmlNode *pBeginRenderIDNode = pRenderStageNode->FirstChild("BeginRenderID")) {
			m_dwMatBeginRenderID = pBeginRenderIDNode->ToElement()->AttributeInt("id");
		}

		if (TiXmlNode *pEndRenderIDNode = pRenderStageNode->FirstChild("EndRenderID")) {
			m_dwMatEndRenderID = pEndRenderIDNode->ToElement()->AttributeInt("id");
		}

		if (TiXmlNode *pSolutionNode = pRenderStageNode->FirstChild("Solution")) {
			m_dwMatSolution = HashValue(pSolutionNode->ToElement()->AttributeString("name"));
		}

		if (TiXmlNode *pPassGroupNode = pRenderStageNode->FirstChild("PassGroup")) {
			m_dwMatPassGroup = HashValue(pPassGroupNode->ToElement()->AttributeString("name"));
		}
	}

	//
	// 加载输入纹理
	//
	VOID CRenderStage::LoadInputTextures(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		if (TiXmlNode *pInputTextureNode = pRenderStageNode->FirstChild("Input")) {
			do {
				const CHAR *szCategroy = pInputTextureNode->ToElement()->AttributeString("category");
				const CHAR *szName = pInputTextureNode->ToElement()->AttributeString("name");
				if (szCategroy == NULL) throw "No input texture category.";
				if (szName == NULL) throw "No input texture name.";

				TEXTURE *pTexture = SAFE_NEW(MEMTYPE_HEAP) TEXTURE;
				ASSERT(pTexture);

				pTexture->dwCategoryName = HashValue(szCategroy);
				pTexture->dwTextureName = HashValue(szName);
				pTexture->channel = pInputTextureNode->ToElement()->AttributeInt("channel");
				pTexture->minFilter = cglStringToAttrib(pInputTextureNode->ToElement()->AttributeString("minfilter"));
				pTexture->magFilter = cglStringToAttrib(pInputTextureNode->ToElement()->AttributeString("magfilter"));
				pTexture->wrapu = cglStringToAttrib(pInputTextureNode->ToElement()->AttributeString("wrapu"));
				pTexture->wrapv = cglStringToAttrib(pInputTextureNode->ToElement()->AttributeString("wrapv"));
				pTexture->attachmentType = COLOR;

				m_inputTextures.push_back(pTexture);
			} while (pInputTextureNode = pRenderStageNode->IterateChildren("Input", pInputTextureNode));
		}
	}

	//
	// 加载输出纹理
	//
	VOID CRenderStage::LoadOutputTextures(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		if (TiXmlNode *pOutputTextureNode = pRenderStageNode->FirstChild("Output")) {
			do {
				const CHAR *szCategroy = pOutputTextureNode->ToElement()->AttributeString("category");
				const CHAR *szName = pOutputTextureNode->ToElement()->AttributeString("name");
				if (szCategroy == NULL) throw "No output texture category.";
				if (szName == NULL) throw "No output texture name.";

				TEXTURE *pTexture = SAFE_NEW(MEMTYPE_HEAP) TEXTURE;
				ASSERT(pTexture);

				pTexture->dwCategoryName = HashValue(szCategroy);
				pTexture->dwTextureName = HashValue(szName);
				pTexture->channel = pOutputTextureNode->ToElement()->AttributeInt("channel");
				pTexture->minFilter = GL_LINEAR;
				pTexture->magFilter = GL_LINEAR;
				pTexture->wrapu = GL_CLAMP_TO_EDGE;
				pTexture->wrapv = GL_CLAMP_TO_EDGE;
				pTexture->attachmentType = COLOR;

				if      (stricmp(pOutputTextureNode->ToElement()->AttributeString("attachment"), "color") == 0) pTexture->attachmentType = COLOR;
				else if (stricmp(pOutputTextureNode->ToElement()->AttributeString("attachment"), "depth") == 0) pTexture->attachmentType = DEPTH; 

				m_outputTextures.push_back(pTexture);
			} while (pOutputTextureNode = pRenderStageNode->IterateChildren("Output", pOutputTextureNode));
		}
	}

	//
	// 加载顶点着色器
	//
	VOID CRenderStage::LoadShaderVertex(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		if (TiXmlNode *pShaderNode = pRenderStageNode->FirstChild("ShaderVertex")) {
			//
			// 1. 获得着色器
			//
			const CShaderPtr &ptrShaderVertex = ShaderManager()->QueryResource(pShaderNode->ToElement()->AttributeString("name"));
			if (ptrShaderVertex.IsNull() || ptrShaderVertex->IsValid() == FALSE) throw "Invalid vertex shader.";

			//
			// 2. 设置着色器默认参数
			//
			SetShaderParams(ptrShaderVertex->GetParams());

			//
			// 3. 设置着色器材质参数
			//
			LoadShaderParams(pShaderNode);

			//
			// 4. 保存着色器
			//
			m_vertex = ptrShaderVertex->GetShaderGL();
		}
	}

	//
	// 加载片段着色器
	//
	VOID CRenderStage::LoadShaderFragment(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		if (TiXmlNode *pShaderNode = pRenderStageNode->FirstChild("ShaderFragment")) {
			//
			// 1. 获得着色器
			//
			const CShaderPtr &ptrShaderFragment = ShaderManager()->QueryResource(pShaderNode->ToElement()->AttributeString("name"));
			if (ptrShaderFragment.IsNull() || ptrShaderFragment->IsValid() == FALSE)  throw "Invalid fragment shader.";

			//
			// 2. 设置着色器默认参数
			//
			SetShaderParams(ptrShaderFragment->GetParams());

			//
			// 3. 设置着色器材质参数
			//
			LoadShaderParams(pShaderNode);

			//
			// 4. 保存着色器
			//
			m_fragment = ptrShaderFragment->GetShaderGL();
		}
	}

	//
	// 加载着色器参数
	//
	VOID CRenderStage::LoadShaderParams(TiXmlNode *pShaderNode)
	{
		ASSERT(pShaderNode);

		if (TiXmlNode *pParamsNode = pShaderNode->FirstChild("Parameters")) {
			if (TiXmlNode *pParamNode = pParamsNode->FirstChild()) {
				do {
					const CHAR *type = pParamNode->Value();
					const CHAR *name = pParamNode->ToElement()->AttributeString("name");
					const CHAR *value = pParamNode->ToElement()->AttributeString("value");
					if (type == NULL || name == NULL || value == NULL) throw "Invalid shader param format.";
					if (strlen(type) == 0) throw "Invalid shader param type.";
					if (strlen(name) == 0) throw "Invalid shader param name.";
					if (strlen(value) == 0) throw "Invalid shader param value.";

					CRenderer::UNIFORM_LOCATION param;
					memset(&param, 0, sizeof(param));

					strcpy(param.name, name);
					param.location = -1;

					if      (stricmp(type, "Uniform1i") == 0) { param.type = CRenderer::UNIFORM_TYPE_INT; sscanf(value, "%d", &param.value.nvalue[0]); }
					else if (stricmp(type, "Uniform2i") == 0) { param.type = CRenderer::UNIFORM_TYPE_INT2; sscanf(value, "%d %d", &param.value.nvalue[0], &param.value.nvalue[1]); }
					else if (stricmp(type, "Uniform3i") == 0) { param.type = CRenderer::UNIFORM_TYPE_INT3; sscanf(value, "%d %d %d", &param.value.nvalue[0], &param.value.nvalue[1], &param.value.nvalue[2]); }
					else if (stricmp(type, "Uniform4i") == 0) { param.type = CRenderer::UNIFORM_TYPE_INT4; sscanf(value, "%d %d %d %d", &param.value.nvalue[0], &param.value.nvalue[1], &param.value.nvalue[2], &param.value.nvalue[3]); }
					else if (stricmp(type, "Uniform1f") == 0) { param.type = CRenderer::UNIFORM_TYPE_FLOAT; sscanf(value, "%f", &param.value.fvalue[0]); }
					else if (stricmp(type, "Uniform2f") == 0) { param.type = CRenderer::UNIFORM_TYPE_FLOAT2; sscanf(value, "%f %f", &param.value.fvalue[0], &param.value.fvalue[1]); }
					else if (stricmp(type, "Uniform3f") == 0) { param.type = CRenderer::UNIFORM_TYPE_FLOAT3; sscanf(value, "%f %f %f", &param.value.fvalue[0], &param.value.fvalue[1], &param.value.fvalue[2]); }
					else if (stricmp(type, "Uniform4f") == 0) { param.type = CRenderer::UNIFORM_TYPE_FLOAT4; sscanf(value, "%f %f %f %f", &param.value.fvalue[0], &param.value.fvalue[1], &param.value.fvalue[2], &param.value.fvalue[3]); }
					else if (stricmp(type, "UniformMatrix2x2") == 0) { param.type = CRenderer::UNIFORM_TYPE_MATRIX2; sscanf(value, "%f %f %f %f", &param.value.fvalue[0], &param.value.fvalue[1], &param.value.fvalue[2], &param.value.fvalue[3]); }
					else if (stricmp(type, "UniformMatrix3x3") == 0) { param.type = CRenderer::UNIFORM_TYPE_MATRIX3; sscanf(value, "%f %f %f %f %f %f %f %f %f", &param.value.fvalue[0], &param.value.fvalue[1], &param.value.fvalue[2], &param.value.fvalue[3], &param.value.fvalue[4], &param.value.fvalue[5], &param.value.fvalue[6], &param.value.fvalue[7], &param.value.fvalue[8]); }
					else if (stricmp(type, "UniformMatrix4x4") == 0) { param.type = CRenderer::UNIFORM_TYPE_MATRIX4; sscanf(value, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", &param.value.fvalue[0], &param.value.fvalue[1], &param.value.fvalue[2], &param.value.fvalue[3], &param.value.fvalue[4], &param.value.fvalue[5], &param.value.fvalue[6], &param.value.fvalue[7], &param.value.fvalue[8], &param.value.fvalue[9], &param.value.fvalue[10], &param.value.fvalue[11], &param.value.fvalue[12], &param.value.fvalue[13], &param.value.fvalue[14], &param.value.fvalue[15]); }
					else throw "Invalid shader param type.";

					m_params[HashValue(name)] = param;
				} while (pParamNode = pParamsNode->IterateChildren(pParamNode));
			}
		}
	}

	//
	// 设置着色器参数
	//
	VOID CRenderStage::SetShaderParams(const CRenderer::UniformLocationMap& params)
	{
		for (CRenderer::UniformLocationMap::const_iterator itParam = params.begin(); itParam != params.end(); ++itParam) {
			m_params[itParam->first] = itParam->second;
		}
	}

	//
	// 创建
	//
	VOID CRenderStage::Create(VOID)
	{
		//
		// 1. 初始化
		//
		m_frameBuffer = 0;
		m_frameBufferWidth = 0;
		m_frameBufferHeight = 0;

		m_numDraws = 0;
		memset(m_drawBuffers, 0, sizeof(m_drawBuffers));

		//
		// 2. 检查纹理渲染对象集合
		//
		if (m_outputTextures.empty()) {
			return;
		}

		//
		// 3. 检查纹理渲染对象
		//    1. 输出纹理数量不超过最大FBO输出数
		//    2. 输出纹理数量不超过最大纹理单元数
		//    3. 要求所有纹理渲染对象来源同一个渲染对象集(确保纹理分辨率一直)
		//
		CRenderTarget *pRenderTarget = NULL;

		if (m_outputTextures.size() > Renderer()->GetMaxFBODrawBuffers()) {
			throw "Output textures out range of draw buffers.";
		}

		for (TextureSet::const_iterator itTexture = m_outputTextures.begin(); itTexture != m_outputTextures.end(); ++itTexture) {
			const TEXTURE *pTexture = *itTexture;
			ASSERT(pTexture);

			// 3.1. 检查纹理通道
			if (pTexture->channel >= Renderer()->GetMaxTexUnits()) {
				throw "Output texture channel out range of texture units.";
			}

			// 3.2. 检查渲染对象集一致性
			if (pRenderTarget == NULL) {
				pRenderTarget =  m_pRenderFlow->GetRenderTarget(pTexture->dwCategoryName);
			}

			if (pRenderTarget != m_pRenderFlow->GetRenderTarget(pTexture->dwCategoryName)) {
				throw "Output texture come from different render target set.";
			}
		}

		//
		// 4. 创建帧缓冲
		//
		m_frameBuffer = Renderer()->CreateFrameBuffer();
		if (m_frameBuffer == 0) throw "CreateFrameBuffer failed.";

		Renderer()->BindFrameBuffer(m_frameBuffer);
		{
			Renderer()->FrameBufferRenderBuffer(GL_DEPTH_ATTACHMENT, pRenderTarget->GetDepthRenderBuffer());

			for (TextureSet::const_iterator itTexture = m_outputTextures.begin(); itTexture != m_outputTextures.end(); ++itTexture) {
				const TEXTURE *pTexture = *itTexture;
				ASSERT(pTexture);

				const CRenderTarget::TEXTURE_TARGET *pTextureTarget = pRenderTarget->GetTextureTarget(pTexture->dwTextureName);
				ASSERT(pTextureTarget);

				switch (pTexture->attachmentType) {
				case DEPTH:
					Renderer()->FrameBufferTexture2D(GL_DEPTH_ATTACHMENT, pTextureTarget->texture, 0);
					break;
				case COLOR:
					Renderer()->FrameBufferTexture2D(GL_COLOR_ATTACHMENT0 + pTexture->channel, pTextureTarget->texture, 0);
					m_drawBuffers[m_numDraws++] = GL_COLOR_ATTACHMENT0 + pTexture->channel;
					break;
				}
			}

			Renderer()->SetReadBuffer(GL_NONE);
			Renderer()->SetDrawBuffer(GL_NONE);

			if (Renderer()->CheckFrameBufferStatus() == FALSE) {
				throw "CheckFrameBufferStatus failed.";
			}
		}
		Renderer()->BindFrameBuffer(0);

		//
		// 5. 设置帧缓冲分辨率
		//
		m_frameBufferWidth = pRenderTarget->GetWidth();
		m_frameBufferHeight = pRenderTarget->GetHeight();
	}

	//
	// 销毁
	//
	VOID CRenderStage::Destroy(VOID)
	{
		Renderer()->DestroyFrameBuffer(&m_frameBuffer);
	}

	//
	// 设置启用状态
	//
	VOID CRenderStage::SetEnable(BOOL bEnable)
	{
		m_bEnable = bEnable;
	}

	//
	// 获得启用状态
	//
	BOOL CRenderStage::IsEnable(VOID) const
	{
		return m_bEnable;
	}

	//
	// 获得名称
	//
	const CHAR* CRenderStage::GetName(VOID) const
	{
		return m_szName;
	}

	//
	// 获得渲染时间
	//
	DWORD CRenderStage::GetRenderTime(VOID) const
	{
		return m_dwRenderTime;
	}

	//
	// 渲染
	//
	VOID CRenderStage::Render(DWORD dwSceneManager)
	{
		if (const CSceneManager *pSceneManager = SceneManager(dwSceneManager)) {
			switch (m_type) {
			case SCENE: RenderScene(pSceneManager->GetMainCameraEntity()); break;
			case LIGHT: RenderLighting(pSceneManager->GetMainCameraEntity()); break;
			case SHADOW_CAST: RenderCastShadow(pSceneManager->GetMainShadowEntity()); break;
			case SHADOW_RECEIVE: RenderReceiveShadow(pSceneManager->GetMainCameraEntity(), pSceneManager->GetMainShadowEntity()); break;
			case SCREEN: RenderScreen(); break;
			}
		}
	}

	//
	// 开始渲染
	//
	VOID CRenderStage::Begin(VOID)
	{
		//
		// 1. 执行回调函数
		//
		RenderCallback fnBeginRenderCallback = m_pRenderFlow->GetRenderCallback(m_dwBeginRenderCallback);
		if (fnBeginRenderCallback) (*fnBeginRenderCallback)(this);

		//
		// 2. 保存状态
		//
		m_lastViewportX = Renderer()->GetViewportX();
		m_lastViewportY = Renderer()->GetViewportY();
		m_lastViewportWidth = Renderer()->GetViewportWidth();
		m_lastViewportHeight = Renderer()->GetViewportHeight();
		m_lastFrameBuffer = Renderer()->GetInteger(GL_FRAMEBUFFER_BINDING);

		//
		// 3. 设置输入纹理
		//
		for (TextureSet::const_iterator itTexture = m_inputTextures.begin(); itTexture != m_inputTextures.end(); ++itTexture) {
			const TEXTURE *pTexture = *itTexture;
			ASSERT(pTexture);

			if (const CRenderTarget *pRenderTarget = m_pRenderFlow->GetRenderTarget(pTexture->dwCategoryName)) {
				if (const CRenderTarget::TEXTURE_TARGET *pTextureTarget = pRenderTarget->GetTextureTarget(pTexture->dwTextureName)) {
					Renderer()->ActiveTexture(GL_TEXTURE0 + pTexture->channel);
					{
						Renderer()->Enable(GL_TEXTURE_2D);
						Renderer()->BindTexture(GL_TEXTURE_2D, pTextureTarget->texture);

						Renderer()->TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pTexture->minFilter);
						Renderer()->TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pTexture->magFilter);
						Renderer()->TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, pTexture->wrapu);
						Renderer()->TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, pTexture->wrapv);
					}
					Renderer()->ActiveTexture(GL_TEXTURE0);
				}
			}
		}

		//
		// 4. 设置FBO
		//
		if (m_frameBuffer) {
			Renderer()->BindFrameBuffer(m_frameBuffer);
			Renderer()->SetViewport(0, 0, m_frameBufferWidth, m_frameBufferHeight);
			Renderer()->SetDrawBuffers(m_numDraws, m_drawBuffers);
		}
	}

	//
	// 结束渲染
	//
	VOID CRenderStage::End(VOID)
	{
		//
		// 1. 恢复状态
		//
		Renderer()->BindFrameBuffer(m_lastFrameBuffer);
		Renderer()->SetViewport(m_lastViewportX, m_lastViewportY, m_lastViewportWidth, m_lastViewportHeight);

		//
		// 2. 执行回调函数
		//
		RenderCallback fnEndRenderCallback = m_pRenderFlow->GetRenderCallback(m_dwEndRenderCallback);
		if (fnEndRenderCallback) (*fnEndRenderCallback)(this);
	}

	//
	// 渲染场景
	//
	VOID CRenderStage::RenderScene(const CEntityCamera *pCamera)
	{
		ASSERT(pCamera);

		DWORD dwBegin = tick();
		{
			if (m_bEnable) {
				Begin();
				{
					//
					// 1. 清空缓冲
					//
					if (m_bClearColorBuffer) Renderer()->SetClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
					if (m_bClearDepthBuffer) Renderer()->SetClearDepth(m_clearDepth);
					Renderer()->ClearBuffer(m_bClearColorBuffer, m_bClearDepthBuffer, FALSE);

					//
					// 2. 渲染
					//
					RenderQuene()->RenderOcclude(pCamera);
					RenderQuene()->RenderQuery(pCamera);

					if (m_dwMatSolution != INVALID_HASHNAME && m_dwMatPassGroup != INVALID_HASHNAME && 
						m_dwMatBeginRenderID != INVALID_VALUE && m_dwMatEndRenderID != INVALID_VALUE) {
						RenderQuene()->RenderOpaque(pCamera, m_dwMatSolution, m_dwMatPassGroup, m_dwMatBeginRenderID, m_dwMatEndRenderID);
						RenderQuene()->RenderTransparence(pCamera, m_dwMatSolution, m_dwMatPassGroup, m_dwMatBeginRenderID, m_dwMatEndRenderID);
					}
				}
				End();
			}
		}
		DWORD dwEnd = tick();
		m_dwRenderTime = dwEnd - dwBegin;
	}

	//
	// 渲染产生阴影
	//
	VOID CRenderStage::RenderCastShadow(const CEntityShadow *pShadow)
	{
		ASSERT(pShadow);

		DWORD dwBegin = tick();
		{
			if (m_bEnable) {
				Begin();
				{
					//
					// 1. 清空缓冲
					//
					if (m_bClearColorBuffer) Renderer()->SetClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
					if (m_bClearDepthBuffer) Renderer()->SetClearDepth(m_clearDepth);
					Renderer()->ClearBuffer(m_bClearColorBuffer, m_bClearDepthBuffer, FALSE);

					//
					// 2. 渲染
					//
					if (m_dwMatSolution != INVALID_HASHNAME && m_dwMatPassGroup != INVALID_HASHNAME && 
						m_dwMatBeginRenderID != INVALID_VALUE && m_dwMatEndRenderID != INVALID_VALUE) {
						RenderQuene()->RenderCastShadow(pShadow, m_dwMatSolution, m_dwMatPassGroup, m_dwMatBeginRenderID, m_dwMatEndRenderID);
					}
				}
				End();
			}
		}
		DWORD dwEnd = tick();
		m_dwRenderTime = dwEnd - dwBegin;
	}

	//
	// 渲染接收阴影
	//
	VOID CRenderStage::RenderReceiveShadow(const CEntityCamera *pCamera, const CEntityShadow *pShadow)
	{
		ASSERT(pCamera);
		ASSERT(pShadow);

		DWORD dwBegin = tick();
		{
			if (m_bEnable) {
				Begin();
				{
					//
					// 1. 清空缓冲
					//
					if (m_bClearColorBuffer) Renderer()->SetClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
					if (m_bClearDepthBuffer) Renderer()->SetClearDepth(m_clearDepth);
					Renderer()->ClearBuffer(m_bClearColorBuffer, m_bClearDepthBuffer, FALSE);

					//
					// 2. 渲染
					//
					if (m_dwMatSolution != INVALID_HASHNAME && m_dwMatPassGroup != INVALID_HASHNAME && 
						m_dwMatBeginRenderID != INVALID_VALUE && m_dwMatEndRenderID != INVALID_VALUE) {
						RenderQuene()->RenderReceiveShadow(pCamera, pShadow, m_dwMatSolution, m_dwMatPassGroup, m_dwMatBeginRenderID, m_dwMatEndRenderID);
					}
				}
				End();
			}
		}
		DWORD dwEnd = tick();
		m_dwRenderTime = dwEnd - dwBegin;
	}

	//
	// 渲染光照
	//
	VOID CRenderStage::RenderLighting(const CEntityCamera *pCamera)
	{
		ASSERT(pCamera);

		DWORD dwBegin = tick();
		{
			if (m_bEnable) {
				Begin();
				{
					//
					// 1. 设置着色器
					//
					if (m_vertex && m_fragment) {
						Renderer()->UseProgram(m_vertex, m_fragment, m_params);
					}

					//
					// 2. 清空缓冲
					//
					if (m_bClearColorBuffer) Renderer()->SetClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
					if (m_bClearDepthBuffer) Renderer()->SetClearDepth(m_clearDepth);
					Renderer()->ClearBuffer(m_bClearColorBuffer, m_bClearDepthBuffer, FALSE);

					//
					// 3. 渲染
					//
					RenderQuene()->RenderLighting(pCamera);
				}
				End();
			}
		}
		DWORD dwEnd = tick();
		m_dwRenderTime = dwEnd - dwBegin;
	}

	//
	// 渲染屏幕
	//
	VOID CRenderStage::RenderScreen(VOID)
	{
		DWORD dwBegin = tick();
		{
			if (m_bEnable) {
				Begin();
				{
					//
					// 1. 设置着色器
					//
					if (m_vertex && m_fragment) {
						Renderer()->UseProgram(m_vertex, m_fragment, m_params);
					}

					//
					// 2. 清空缓冲
					//
					if (m_bClearColorBuffer) Renderer()->SetClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
					if (m_bClearDepthBuffer) Renderer()->SetClearDepth(m_clearDepth);
					Renderer()->ClearBuffer(m_bClearColorBuffer, m_bClearDepthBuffer, FALSE);

					//
					// 3. 渲染
					//
					Renderer()->RenderViewport();
				}
				End();
			}
		}
		DWORD dwEnd = tick();
		m_dwRenderTime = dwEnd - dwBegin;
	}

}

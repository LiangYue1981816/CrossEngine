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
	// ��ʼ��
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
	// �ͷ�
	//
	VOID CRenderStage::Free(VOID)
	{
		//
		// 1. �ͷ�����
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
		// 2. ��ʼ��
		//
		Init();
	}

	//
	// ������Ⱦ����
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
	// ��������
	//
	VOID CRenderStage::LoadName(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		const CHAR* szName = pRenderStageNode->ToElement()->AttributeString("name");
		if (szName == NULL) throw "No render state name.";

		strcpy(m_szName, szName);
	}

	//
	// ��������
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
	// ����״̬
	//
	VOID CRenderStage::LoadEnable(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);
		m_bEnable = cglStringToAttrib(pRenderStageNode->ToElement()->AttributeString("enable"));
	}

	//
	// ���ػص�
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
	// ����������
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
	// ���ز���
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
	// ������������
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
	// �����������
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
	// ���ض�����ɫ��
	//
	VOID CRenderStage::LoadShaderVertex(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		if (TiXmlNode *pShaderNode = pRenderStageNode->FirstChild("ShaderVertex")) {
			//
			// 1. �����ɫ��
			//
			const CShaderPtr &ptrShaderVertex = ShaderManager()->QueryResource(pShaderNode->ToElement()->AttributeString("name"));
			if (ptrShaderVertex.IsNull() || ptrShaderVertex->IsValid() == FALSE) throw "Invalid vertex shader.";

			//
			// 2. ������ɫ��Ĭ�ϲ���
			//
			SetShaderParams(ptrShaderVertex->GetParams());

			//
			// 3. ������ɫ�����ʲ���
			//
			LoadShaderParams(pShaderNode);

			//
			// 4. ������ɫ��
			//
			m_vertex = ptrShaderVertex->GetShaderGL();
		}
	}

	//
	// ����Ƭ����ɫ��
	//
	VOID CRenderStage::LoadShaderFragment(TiXmlNode *pRenderStageNode)
	{
		ASSERT(pRenderStageNode);

		if (TiXmlNode *pShaderNode = pRenderStageNode->FirstChild("ShaderFragment")) {
			//
			// 1. �����ɫ��
			//
			const CShaderPtr &ptrShaderFragment = ShaderManager()->QueryResource(pShaderNode->ToElement()->AttributeString("name"));
			if (ptrShaderFragment.IsNull() || ptrShaderFragment->IsValid() == FALSE)  throw "Invalid fragment shader.";

			//
			// 2. ������ɫ��Ĭ�ϲ���
			//
			SetShaderParams(ptrShaderFragment->GetParams());

			//
			// 3. ������ɫ�����ʲ���
			//
			LoadShaderParams(pShaderNode);

			//
			// 4. ������ɫ��
			//
			m_fragment = ptrShaderFragment->GetShaderGL();
		}
	}

	//
	// ������ɫ������
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
	// ������ɫ������
	//
	VOID CRenderStage::SetShaderParams(const CRenderer::UniformLocationMap& params)
	{
		for (CRenderer::UniformLocationMap::const_iterator itParam = params.begin(); itParam != params.end(); ++itParam) {
			m_params[itParam->first] = itParam->second;
		}
	}

	//
	// ����
	//
	VOID CRenderStage::Create(VOID)
	{
		//
		// 1. ��ʼ��
		//
		m_frameBuffer = 0;
		m_frameBufferWidth = 0;
		m_frameBufferHeight = 0;

		m_numDraws = 0;
		memset(m_drawBuffers, 0, sizeof(m_drawBuffers));

		//
		// 2. ���������Ⱦ���󼯺�
		//
		if (m_outputTextures.empty()) {
			return;
		}

		//
		// 3. ���������Ⱦ����
		//    1. ��������������������FBO�����
		//    2. ������������������������Ԫ��
		//    3. Ҫ������������Ⱦ������Դͬһ����Ⱦ����(ȷ������ֱ���һֱ)
		//
		CRenderTarget *pRenderTarget = NULL;

		if (m_outputTextures.size() > Renderer()->GetMaxFBODrawBuffers()) {
			throw "Output textures out range of draw buffers.";
		}

		for (TextureSet::const_iterator itTexture = m_outputTextures.begin(); itTexture != m_outputTextures.end(); ++itTexture) {
			const TEXTURE *pTexture = *itTexture;
			ASSERT(pTexture);

			// 3.1. �������ͨ��
			if (pTexture->channel >= Renderer()->GetMaxTexUnits()) {
				throw "Output texture channel out range of texture units.";
			}

			// 3.2. �����Ⱦ����һ����
			if (pRenderTarget == NULL) {
				pRenderTarget =  m_pRenderFlow->GetRenderTarget(pTexture->dwCategoryName);
			}

			if (pRenderTarget != m_pRenderFlow->GetRenderTarget(pTexture->dwCategoryName)) {
				throw "Output texture come from different render target set.";
			}
		}

		//
		// 4. ����֡����
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
		// 5. ����֡����ֱ���
		//
		m_frameBufferWidth = pRenderTarget->GetWidth();
		m_frameBufferHeight = pRenderTarget->GetHeight();
	}

	//
	// ����
	//
	VOID CRenderStage::Destroy(VOID)
	{
		Renderer()->DestroyFrameBuffer(&m_frameBuffer);
	}

	//
	// ��������״̬
	//
	VOID CRenderStage::SetEnable(BOOL bEnable)
	{
		m_bEnable = bEnable;
	}

	//
	// �������״̬
	//
	BOOL CRenderStage::IsEnable(VOID) const
	{
		return m_bEnable;
	}

	//
	// �������
	//
	const CHAR* CRenderStage::GetName(VOID) const
	{
		return m_szName;
	}

	//
	// �����Ⱦʱ��
	//
	DWORD CRenderStage::GetRenderTime(VOID) const
	{
		return m_dwRenderTime;
	}

	//
	// ��Ⱦ
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
	// ��ʼ��Ⱦ
	//
	VOID CRenderStage::Begin(VOID)
	{
		//
		// 1. ִ�лص�����
		//
		RenderCallback fnBeginRenderCallback = m_pRenderFlow->GetRenderCallback(m_dwBeginRenderCallback);
		if (fnBeginRenderCallback) (*fnBeginRenderCallback)(this);

		//
		// 2. ����״̬
		//
		m_lastViewportX = Renderer()->GetViewportX();
		m_lastViewportY = Renderer()->GetViewportY();
		m_lastViewportWidth = Renderer()->GetViewportWidth();
		m_lastViewportHeight = Renderer()->GetViewportHeight();
		m_lastFrameBuffer = Renderer()->GetInteger(GL_FRAMEBUFFER_BINDING);

		//
		// 3. ������������
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
		// 4. ����FBO
		//
		if (m_frameBuffer) {
			Renderer()->BindFrameBuffer(m_frameBuffer);
			Renderer()->SetViewport(0, 0, m_frameBufferWidth, m_frameBufferHeight);
			Renderer()->SetDrawBuffers(m_numDraws, m_drawBuffers);
		}
	}

	//
	// ������Ⱦ
	//
	VOID CRenderStage::End(VOID)
	{
		//
		// 1. �ָ�״̬
		//
		Renderer()->BindFrameBuffer(m_lastFrameBuffer);
		Renderer()->SetViewport(m_lastViewportX, m_lastViewportY, m_lastViewportWidth, m_lastViewportHeight);

		//
		// 2. ִ�лص�����
		//
		RenderCallback fnEndRenderCallback = m_pRenderFlow->GetRenderCallback(m_dwEndRenderCallback);
		if (fnEndRenderCallback) (*fnEndRenderCallback)(this);
	}

	//
	// ��Ⱦ����
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
					// 1. ��ջ���
					//
					if (m_bClearColorBuffer) Renderer()->SetClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
					if (m_bClearDepthBuffer) Renderer()->SetClearDepth(m_clearDepth);
					Renderer()->ClearBuffer(m_bClearColorBuffer, m_bClearDepthBuffer, FALSE);

					//
					// 2. ��Ⱦ
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
	// ��Ⱦ������Ӱ
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
					// 1. ��ջ���
					//
					if (m_bClearColorBuffer) Renderer()->SetClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
					if (m_bClearDepthBuffer) Renderer()->SetClearDepth(m_clearDepth);
					Renderer()->ClearBuffer(m_bClearColorBuffer, m_bClearDepthBuffer, FALSE);

					//
					// 2. ��Ⱦ
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
	// ��Ⱦ������Ӱ
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
					// 1. ��ջ���
					//
					if (m_bClearColorBuffer) Renderer()->SetClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
					if (m_bClearDepthBuffer) Renderer()->SetClearDepth(m_clearDepth);
					Renderer()->ClearBuffer(m_bClearColorBuffer, m_bClearDepthBuffer, FALSE);

					//
					// 2. ��Ⱦ
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
	// ��Ⱦ����
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
					// 1. ������ɫ��
					//
					if (m_vertex && m_fragment) {
						Renderer()->UseProgram(m_vertex, m_fragment, m_params);
					}

					//
					// 2. ��ջ���
					//
					if (m_bClearColorBuffer) Renderer()->SetClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
					if (m_bClearDepthBuffer) Renderer()->SetClearDepth(m_clearDepth);
					Renderer()->ClearBuffer(m_bClearColorBuffer, m_bClearDepthBuffer, FALSE);

					//
					// 3. ��Ⱦ
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
	// ��Ⱦ��Ļ
	//
	VOID CRenderStage::RenderScreen(VOID)
	{
		DWORD dwBegin = tick();
		{
			if (m_bEnable) {
				Begin();
				{
					//
					// 1. ������ɫ��
					//
					if (m_vertex && m_fragment) {
						Renderer()->UseProgram(m_vertex, m_fragment, m_params);
					}

					//
					// 2. ��ջ���
					//
					if (m_bClearColorBuffer) Renderer()->SetClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
					if (m_bClearDepthBuffer) Renderer()->SetClearDepth(m_clearDepth);
					Renderer()->ClearBuffer(m_bClearColorBuffer, m_bClearDepthBuffer, FALSE);

					//
					// 3. ��Ⱦ
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

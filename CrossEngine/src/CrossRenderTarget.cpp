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

	CRenderTarget::CRenderTarget(CRenderFlow *pRenderFlow)
		: m_pRenderFlow(pRenderFlow)
	{
		Init();
	}

	CRenderTarget::~CRenderTarget(VOID)
	{
		Free();
	}

	//
	// ��ʼ��
	//
	VOID CRenderTarget::Init(VOID)
	{
		m_width = 0;
		m_height = 0;
		m_depthRenderBuffer = 0;
		m_textureTargets.clear();
	}

	//
	// �ͷ�
	//
	VOID CRenderTarget::Free(VOID)
	{
		//
		// 1. �ͷ�������Ⱦ����
		//
		for (TextureTargetMap::const_iterator itTextureTarget = m_textureTargets.begin(); itTextureTarget != m_textureTargets.end(); ++itTextureTarget) {
			const TEXTURE_TARGET *pTextureTarget = itTextureTarget->second;
			ASSERT(pTextureTarget);

			SAFE_DELETE(pTextureTarget);
		}

		//
		// 2. ��ʼ��
		//
		Init();
	}

	//
	// ��ÿ�
	//
	INT CRenderTarget::GetWidth(VOID) const
	{
		return m_width;
	}

	//
	// ��ø�
	//
	INT CRenderTarget::GetHeight(VOID) const
	{
		return m_height;
	}

	//
	// ��������Ⱦ����
	//
	UINT CRenderTarget::GetDepthRenderBuffer(VOID) const
	{
		return m_depthRenderBuffer;
	}

	//
	// ���������Ⱦ����
	//
	CRenderTarget::TEXTURE_TARGET* CRenderTarget::GetTextureTarget(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetTextureTarget(HashValue(szName));
	}

	//
	// ���������Ⱦ����
	//
	CRenderTarget::TEXTURE_TARGET* CRenderTarget::GetTextureTarget(DWORD dwName) const
	{
		TextureTargetMap::const_iterator itTextureTarget = m_textureTargets.find(dwName);
		return itTextureTarget != m_textureTargets.end() ? itTextureTarget->second : NULL;
	}

	//
	// ������Ⱦ����
	//
	VOID CRenderTarget::LoadFromStream(TiXmlNode *pRenderTargetNode)
	{
		ASSERT(pRenderTargetNode);

		LoadResolution(pRenderTargetNode);
		LoadTextureTargets(pRenderTargetNode);
	}

	//
	// ���طֱ���
	//
	VOID CRenderTarget::LoadResolution(TiXmlNode *pRenderTargetNode)
	{
		ASSERT(pRenderTargetNode);

		BOOL bViewport = cglStringToAttrib(pRenderTargetNode->ToElement()->AttributeString("viewport"));

		//
		// 1. �����ӿڷֱ�������
		//
		if (bViewport) {
			FLOAT scaleWidth = pRenderTargetNode->ToElement()->AttributeFloat("width");
			FLOAT scaleHeight = pRenderTargetNode->ToElement()->AttributeFloat("height");

			m_width = (INT)(Renderer()->GetViewportWidth()*m_pRenderFlow->GetScalePPI()*scaleWidth + 0.5f);
			m_height = (INT)(Renderer()->GetViewportHeight()*m_pRenderFlow->GetScalePPI()*scaleHeight + 0.5f);
		}
		//
		// 2. ���Էֱ���
		//
		else {
			m_width = pRenderTargetNode->ToElement()->AttributeInt("width");
			m_height = pRenderTargetNode->ToElement()->AttributeInt("height");
		}

		//
		// 4. ���ֱ���
		//
		if (m_width <= 0 || m_height <= 0) {
			throw "Invalid render target resolution.";
		}
	}

	//
	// �����������
	//
	VOID CRenderTarget::LoadTextureTargets(TiXmlNode *pRenderTargetNode)
	{
		ASSERT(pRenderTargetNode);

		if (TiXmlNode *pTextureTargetNode = pRenderTargetNode->FirstChild("TextureTarget")) {
			do {
				//
				// 1. ������������
				//
				const CHAR *szName = pTextureTargetNode->ToElement()->AttributeString("name");
				if (szName == NULL) throw "No texture target name.";

				//
				// 2. ����Ƿ��Ѿ�����
				//
				DWORD dwName = HashValue(szName);
				TextureTargetMap::const_iterator itTextureTarget = m_textureTargets.find(dwName);
				if (itTextureTarget != m_textureTargets.end()) continue;

				//
				// 3. �����������
				//
				TEXTURE_TARGET *pTextureTarget = SAFE_NEW(MEMTYPE_HEAP) TEXTURE_TARGET;
				ASSERT(pTextureTarget);

				pTextureTarget->texture = 0;
				pTextureTarget->type = cglStringToAttrib(pTextureTargetNode->ToElement()->AttributeString("type"));
				pTextureTarget->format = cglStringToAttrib(pTextureTargetNode->ToElement()->AttributeString("format"));
				pTextureTarget->internalformat = cglStringToAttrib(pTextureTargetNode->ToElement()->AttributeString("internalformat"));

				m_textureTargets[dwName] = pTextureTarget;
			} while (pTextureTargetNode = pRenderTargetNode->IterateChildren("TextureTarget", pTextureTargetNode));
		}
	}

	//
	// ����
	//
	VOID CRenderTarget::Create(VOID)
	{
		//
		// 1. ���������Ⱦ����
		//
		m_depthRenderBuffer = Renderer()->CreateRenderBuffer();
		if (m_depthRenderBuffer == 0) throw "CreateRenderBuffer failed.";

		Renderer()->BindRenderBuffer(m_depthRenderBuffer);
		{
			const CHAR *szRenderer = Renderer()->GetString(GL_RENDERER);
			if (stricmp(szRenderer, "NVIDIA Tegra2") == 0 ||
				stricmp(szRenderer, "NVIDIA Tegra3") == 0) {
				Renderer()->RenderBufferFormat(GL_DEPTH_COMPONENT16, m_width, m_height);
			}
			else {
				Renderer()->RenderBufferFormat(GL_DEPTH_COMPONENT24, m_width, m_height);
			}
		}
		Renderer()->BindRenderBuffer(0);

		//
		// 2. ����������Ⱦ����
		//
		for (TextureTargetMap::const_iterator itTextureTarget = m_textureTargets.begin(); itTextureTarget != m_textureTargets.end(); ++itTextureTarget) {
			TEXTURE_TARGET *pTextureTarget = itTextureTarget->second;
			ASSERT(pTextureTarget);

			pTextureTarget->texture = Renderer()->CreateTexture2D(GL_TEXTURE_2D, 0, pTextureTarget->internalformat, m_width, m_height, 0, pTextureTarget->format, pTextureTarget->type, NULL);
			if (pTextureTarget->texture == 0) throw "CreateTexture2D failed.";

			Renderer()->BindTexture(GL_TEXTURE_2D, pTextureTarget->texture);
			{
				Renderer()->TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				Renderer()->TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				if (pTextureTarget->internalformat == GL_DEPTH_COMPONENT || pTextureTarget->format == GL_DEPTH_COMPONENT) {
					Renderer()->TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
					Renderer()->TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
					Renderer()->TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					Renderer()->TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
				else {
					Renderer()->TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					Renderer()->TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}
			}
			Renderer()->BindTexture(GL_TEXTURE_2D, 0);
		}
	}

	//
	// ����
	//
	VOID CRenderTarget::Destroy(VOID)
	{
		//
		// 1. �ͷ������Ⱦ����
		//
		Renderer()->DestroyRenderBuffer(&m_depthRenderBuffer);

		//
		// 2. ����������Ⱦ����
		//
		for (TextureTargetMap::const_iterator itTextureTarget = m_textureTargets.begin(); itTextureTarget != m_textureTargets.end(); ++itTextureTarget) {
			TEXTURE_TARGET *pTextureTarget = itTextureTarget->second;
			ASSERT(pTextureTarget);

			Renderer()->DestroyTexture(&pTextureTarget->texture);
		}
	}

}

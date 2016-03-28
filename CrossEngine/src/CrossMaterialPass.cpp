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

	CMatPass::CMatPass(CMaterial *pMaterial)
		: m_pParentMaterial(pMaterial)

		, pRenderNext(NULL)
		, pEntityHead(NULL)
	{
		Init();
	}

	CMatPass::~CMatPass(VOID)
	{
		Free();
	}

	//
	// 初始化资源
	//
	VOID CMatPass::Init(VOID)
	{
		m_id = 0;

		m_pass.lighting.bEnableLighting = GL_FALSE;
		m_pass.lighting.matAmbient[0] = 0.2f;
		m_pass.lighting.matAmbient[1] = 0.2f;
		m_pass.lighting.matAmbient[2] = 0.2f;
		m_pass.lighting.matAmbient[3] = 1.0f;
		m_pass.lighting.matDiffuse[0] = 0.8f;
		m_pass.lighting.matDiffuse[1] = 0.8f;
		m_pass.lighting.matDiffuse[2] = 0.8f;
		m_pass.lighting.matDiffuse[3] = 1.0f;
		m_pass.lighting.matEmission[0] = 0.0f;
		m_pass.lighting.matEmission[1] = 0.0f;
		m_pass.lighting.matEmission[2] = 0.0f;
		m_pass.lighting.matEmission[3] = 1.0f;
		m_pass.lighting.matSpecular[0] = 0.0f;
		m_pass.lighting.matSpecular[1] = 0.0f;
		m_pass.lighting.matSpecular[2] = 0.0f;
		m_pass.lighting.matSpecular[3] = 1.0f;
		m_pass.lighting.matShininess = 0.0f;

		m_pass.cull.bEnableCullFace = GL_TRUE;
		m_pass.cull.cullFaceMode = GL_BACK;
		m_pass.cull.frontFaceMode = GL_CCW;

		m_pass.depth.bEnableDepthTest = GL_TRUE;
		m_pass.depth.depthMask = GL_TRUE;
		m_pass.depth.depthFunc = GL_LESS;

		m_pass.alpha.bEnableAlphaTest = GL_FALSE;
		m_pass.alpha.alphaFunc = GL_ALWAYS;
		m_pass.alpha.alphaRef = 0.0f;

		m_pass.blend.bEnableBlend = GL_FALSE;
		m_pass.blend.blendModeColor = m_pass.blend.blendModeAlpha = GL_FUNC_ADD;
		m_pass.blend.blendSrcColor = m_pass.blend.blendSrcAlpha = GL_ONE;
		m_pass.blend.blendDesColor = m_pass.blend.blendDesAlpha = GL_ZERO;

		m_pass.program.vertex = 0;
		m_pass.program.fragment = 0;
		m_pass.program.uniformExternalLocations.clear();

		m_pass.textures[0].texture = 0;
		m_pass.textures[1].texture = 0;
		m_pass.textures[2].texture = 0;
		m_pass.textures[3].texture = 0;
		m_pass.textures[4].texture = 0;
		m_pass.textures[5].texture = 0;
		m_pass.textures[6].texture = 0;
		m_pass.textures[7].texture = 0;
	}

	//
	// 释放资源
	//
	VOID CMatPass::Free(VOID)
	{
		//
		// 1. 释放着色器资源
		//
		m_ptrShaderVertex.SetNull();
		m_ptrShaderFragment.SetNull();

		//
		// 2. 释放纹理资源
		//
		m_ptrTextures[0].SetNull();
		m_ptrTextures[1].SetNull();
		m_ptrTextures[2].SetNull();
		m_ptrTextures[3].SetNull();
		m_ptrTextures[4].SetNull();
		m_ptrTextures[5].SetNull();
		m_ptrTextures[6].SetNull();
		m_ptrTextures[7].SetNull();

		//
		// 3. 初始化
		//
		Init();
	}

	//
	// 获得父材质
	//
	const CMaterial* CMatPass::GetParentMaterial(VOID) const
	{
		return m_pParentMaterial;
	}

	//
	// 获得渲染通路
	//
	CRenderer::PASS* CMatPass::GetPass(VOID) const
	{
		return const_cast<CRenderer::PASS*>(&m_pass);
	}

	//
	// 设置渲染ID
	//
	VOID CMatPass::SetRenderID(DWORD id)
	{
		m_id = id;
	}

	//
	// 获得渲染ID
	//
	DWORD CMatPass::GetRenderID(VOID) const
	{
		return m_id;
	}

	//
	// 设置面剔除状态
	//
	VOID CMatPass::SetEnableCullFace(BOOL bEnable)
	{
		m_pass.cull.bEnableCullFace = bEnable;
	}

	//
	// 设置面剔除模式
	//
	VOID CMatPass::SetCullFaceMode(UINT mode)
	{
		m_pass.cull.cullFaceMode = mode;
	}

	//
	// 设置面顶点顺序模式
	//
	VOID CMatPass::SetFrontFaceMode(UINT mode)
	{
		m_pass.cull.frontFaceMode = mode;
	}

	//
	// 获得面剔除状态
	//
	BOOL CMatPass::IsEnableCullFace(VOID) const
	{
		return m_pass.cull.bEnableCullFace;
	}

	//
	// 获得面剔除模式
	//
	UINT CMatPass::GetCullFaceMode(VOID) const
	{
		return m_pass.cull.cullFaceMode;
	}

	//
	// 获得面顶点顺序模式
	//
	UINT CMatPass::GetFrontFaceMode(VOID) const
	{
		return m_pass.cull.frontFaceMode;
	}

	//
	// 设置深度测试状态
	//
	VOID CMatPass::SetEnableDepthTest(BOOL bEnable)
	{
		m_pass.depth.bEnableDepthTest = bEnable;
	}

	//
	// 设置写深度状态
	//
	VOID CMatPass::SetEnableDepthWrite(BOOL bEnable)
	{
		m_pass.depth.depthMask = bEnable;
	}

	//
	// 设置深度测试方法
	//
	VOID CMatPass::SetDepthFunc(UINT func)
	{
		m_pass.depth.depthFunc = func;
	}

	//
	// 获得深度测试状态
	//
	BOOL CMatPass::IsEnableDepthTest(VOID) const
	{
		return m_pass.depth.bEnableDepthTest;
	}

	//
	// 获得写深度状态
	//
	BOOL CMatPass::IsEnableDepthWrite(VOID) const
	{
		return m_pass.depth.depthMask;
	}

	//
	// 获得深度测试方法
	//
	UINT CMatPass::GetDepthFunc(VOID) const
	{
		return m_pass.depth.depthFunc;
	}

	//
	// 设置透明测试状态
	//
	VOID CMatPass::SetEnableAlphaTest(BOOL bEnable)
	{
		m_pass.alpha.bEnableAlphaTest = bEnable;
	}

	//
	// 设置透明测试方法
	//
	VOID CMatPass::SetAlphaFunc(UINT func)
	{
		m_pass.alpha.alphaFunc = func;
	}

	//
	// 设置透明测试及参数
	//
	VOID CMatPass::SetAlphaRef(FLOAT ref)
	{
		m_pass.alpha.alphaRef = ref;
	}

	//
	// 获得透明测试状态
	//
	BOOL CMatPass::IsEnableAlphaTest(VOID) const
	{
		return m_pass.alpha.bEnableAlphaTest;
	}

	//
	// 获得透明测试方法
	//
	UINT CMatPass::GetAlphaFunc(VOID) const
	{
		return m_pass.alpha.alphaFunc;
	}

	//
	// 获得透明测试参数
	//
	FLOAT CMatPass::GetAlphaRef(VOID) const
	{
		return m_pass.alpha.alphaRef;
	}

	//
	// 设置混合状态
	//
	VOID CMatPass::SetEnableBlend(BOOL bEnable)
	{
		m_pass.blend.bEnableBlend = bEnable;
	}

	//
	// 设置混合模式
	//
	VOID CMatPass::SetBlendMode(UINT color, UINT alpha)
	{
		m_pass.blend.blendModeColor = color;
		m_pass.blend.blendModeAlpha = alpha;
	}

	//
	// 设置混合参数
	//
	VOID CMatPass::SetBlendFunc(UINT srcColor, UINT srcAlpha, UINT desColor, UINT desAlpha)
	{
		m_pass.blend.blendSrcColor = srcColor;
		m_pass.blend.blendSrcAlpha = srcAlpha;
		m_pass.blend.blendDesColor = desColor;
		m_pass.blend.blendDesAlpha = desAlpha;
	}

	//
	// 获得混合状态
	//
	BOOL CMatPass::IsEnableBlend(VOID) const
	{
		return m_pass.blend.bEnableBlend;
	}

	//
	// 获得颜色混合模式
	//
	UINT CMatPass::GetBlendModeColor(VOID) const
	{
		return m_pass.blend.blendModeColor;
	}

	//
	// 获得透明混合模式
	//
	UINT CMatPass::GetBlendModeAlpha(VOID) const
	{
		return m_pass.blend.blendModeAlpha;
	}

	//
	// 获得颜色混合参数
	//
	UINT CMatPass::GetBlendFuncSrcColor(VOID) const
	{
		return m_pass.blend.blendSrcColor;
	}

	//
	// 获得透明混合参数
	//
	UINT CMatPass::GetBlendFuncSrcAlpha(VOID) const
	{
		return m_pass.blend.blendSrcAlpha;
	}

	//
	// 获得颜色混合参数
	//
	UINT CMatPass::GetBlendFuncDesColor(VOID) const
	{
		return m_pass.blend.blendDesColor;
	}

	//
	// 获得目标混合参数
	//
	UINT CMatPass::GetBlendFuncDesAlpha(VOID) const
	{
		return m_pass.blend.blendDesAlpha;
	}

	//
	// 设置光照状态
	//
	VOID CMatPass::SetEnableLighting(BOOL bEnable)
	{
		m_pass.lighting.bEnableLighting = bEnable;
	}

	//
	// 设置环境光
	//
	VOID CMatPass::SetAmbient(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
	{
		m_pass.lighting.matAmbient[0] = r;
		m_pass.lighting.matAmbient[1] = g;
		m_pass.lighting.matAmbient[2] = b;
		m_pass.lighting.matAmbient[3] = a;
	}

	//
	// 设置漫反射光
	//
	VOID CMatPass::SetDiffuse(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
	{
		m_pass.lighting.matDiffuse[0] = r;
		m_pass.lighting.matDiffuse[1] = g;
		m_pass.lighting.matDiffuse[2] = b;
		m_pass.lighting.matDiffuse[3] = a;
	}

	//
	// 设置自发光
	//
	VOID CMatPass::SetEmission(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
	{
		m_pass.lighting.matEmission[0] = r;
		m_pass.lighting.matEmission[1] = g;
		m_pass.lighting.matEmission[2] = b;
		m_pass.lighting.matEmission[3] = a;
	}

	//
	// 设置高光
	//
	VOID CMatPass::SetSpecular(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
	{
		m_pass.lighting.matSpecular[0] = r;
		m_pass.lighting.matSpecular[1] = g;
		m_pass.lighting.matSpecular[2] = b;
		m_pass.lighting.matSpecular[3] = a;
	}

	//
	// 设置高光级别
	//
	VOID CMatPass::SetShininess(FLOAT shininess)
	{
		m_pass.lighting.matShininess = shininess;
	}

	//
	// 获得光照状态
	//
	BOOL CMatPass::IsEnableLighting(VOID) const
	{
		return m_pass.lighting.bEnableLighting;
	}

	//
	// 获得环境光
	//
	const FLOAT* CMatPass::GetAmbient(VOID) const
	{
		return m_pass.lighting.matAmbient;
	}

	//
	// 获得漫反射光
	//
	const FLOAT* CMatPass::GetDiffuse(VOID) const
	{
		return m_pass.lighting.matDiffuse;
	}

	//
	// 获得自发光
	//
	const FLOAT* CMatPass::GetEmission(VOID) const
	{
		return m_pass.lighting.matEmission;
	}

	//
	// 获得高光
	//
	const FLOAT* CMatPass::GetSpecular(VOID) const
	{
		return m_pass.lighting.matSpecular;
	}

	//
	// 获得高光级别
	//
	FLOAT CMatPass::GetShininess(VOID) const
	{
		return m_pass.lighting.matShininess;
	}

	//
	// 设置纹理单元
	//
	BOOL CMatPass::SetTexture(INT indexTexUnit, const CHAR *szName)
	{
		ASSERT(szName);
		const CTexturePtr &ptrTexture = TextureManager()->QueryResource(szName);

		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return FALSE;
		}

		if (ptrTexture.IsNull() || ptrTexture->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 设置纹理单元
		//
		m_ptrTextures[indexTexUnit] = ptrTexture;
		m_pass.textures[indexTexUnit].target = ptrTexture->GetTargetGL();
		m_pass.textures[indexTexUnit].texture = ptrTexture->GetTextureGL();
	
		return TRUE;
	}

	//
	// 设置纹理参数
	//
	VOID CMatPass::SetTextureParam(INT indexTexUnit, UINT minFilter, UINT magFilter, UINT warpu, UINT warpv)
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return;
		}

		//
		// 2. 设置纹理参数
		//
		m_pass.textures[indexTexUnit].texMinFliter = minFilter;
		m_pass.textures[indexTexUnit].texMagFilter = magFilter;
		m_pass.textures[indexTexUnit].texWrapU = warpu;
		m_pass.textures[indexTexUnit].texWrapV = warpv;
	}

	//
	// 设置纹理滚动
	//
	VOID CMatPass::SetTextureScrollU(INT indexTexUnit, FLOAT scrollu)
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return;
		}

		//
		// 2. 设置纹理滚动
		//
		m_pass.textures[indexTexUnit].texAnimation.texScrollU = scrollu;
	}

	//
	// 设置纹理滚动
	//
	VOID CMatPass::SetTextureScrollV(INT indexTexUnit, FLOAT scrollv)
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return;
		}

		//
		// 2. 设置纹理滚动
		//
		m_pass.textures[indexTexUnit].texAnimation.texScrollV = scrollv;
	}

	//
	// 设置纹理动画
	//
	VOID CMatPass::SetTextureSequence(INT indexTexUnit, INT cols, INT rows, FLOAT duration)
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return;
		}

		//
		// 2. 设置纹理动画
		//
		m_pass.textures[indexTexUnit].texAnimation.texSequenceCols = cols;
		m_pass.textures[indexTexUnit].texAnimation.texSequenceRows = rows;
		m_pass.textures[indexTexUnit].texAnimation.texSequenceDurationTime = duration;
	}

	//
	// 获得纹理
	//
	const CTexturePtr& CMatPass::GetTexture(INT indexTexUnit) const
	{
		static CTexturePtr ptrTexture;

		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return ptrTexture;
		}

		//
		// 2. 获得纹理
		//
		return m_ptrTextures[indexTexUnit];
	}

	//
	// 获得纹理参数
	//
	UINT CMatPass::GetTextureMinFilter(INT indexTexUnit) const
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return GL_INVALID_ENUM;
		}

		//
		// 2. 获得纹理参数
		//
		return m_pass.textures[indexTexUnit].texMinFliter;
	}

	//
	// 获得纹理参数
	//
	UINT CMatPass::GetTextureMagFilter(INT indexTexUnit) const
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return GL_INVALID_ENUM;
		}

		//
		// 2. 获得纹理参数
		//
		return m_pass.textures[indexTexUnit].texMagFilter;
	}

	//
	// 获得纹理参数
	//
	UINT CMatPass::GetTextureWarpU(INT indexTexUnit) const
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return GL_INVALID_ENUM;
		}

		//
		// 2. 获得纹理参数
		//
		return m_pass.textures[indexTexUnit].texWrapU;
	}

	//
	// 获得纹理参数
	//
	UINT CMatPass::GetTextureWarpV(INT indexTexUnit) const
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return GL_INVALID_ENUM;
		}

		//
		// 2. 获得纹理参数
		//
		return m_pass.textures[indexTexUnit].texWrapV;
	}

	//
	// 获得纹理滚动参数
	//
	FLOAT CMatPass::GetTextureScrollU(INT indexTexUnit) const
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return 0.0f;
		}

		//
		// 2. 获得纹理滚动参数
		//
		return m_pass.textures[indexTexUnit].texAnimation.texScrollU;
	}

	//
	// 获得纹理滚动参数
	//
	FLOAT CMatPass::GetTextureScrollV(INT indexTexUnit) const
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return 0.0f;
		}

		//
		// 2. 获得纹理滚动参数
		//
		return m_pass.textures[indexTexUnit].texAnimation.texScrollV;
	}

	//
	// 获得纹理序列帧参数
	//
	INT CMatPass::GetTextureSequenceCols(INT indexTexUnit) const
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return 1;
		}

		//
		// 2. 获得纹理序列帧参数
		//
		return m_pass.textures[indexTexUnit].texAnimation.texSequenceCols;
	}

	//
	// 获得纹理序列帧参数
	//
	INT CMatPass::GetTextureSequenceRows(INT indexTexUnit) const
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return 1;
		}

		//
		// 2. 获得纹理序列帧参数
		//
		return m_pass.textures[indexTexUnit].texAnimation.texSequenceRows;
	}

	//
	// 获得纹理序列帧参数
	//
	FLOAT CMatPass::GetTextureSequenceDuration(INT indexTexUnit) const
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return 0.0f;
		}

		//
		// 2. 获得纹理序列帧参数
		//
		return m_pass.textures[indexTexUnit].texAnimation.texSequenceDurationTime;
	}

	//
	// 设置顶点着色器
	//
	BOOL CMatPass::SetShaderVertex(const CHAR *szName)
	{
		ASSERT(szName);
		const CShaderPtr &ptrShader = ShaderManager()->QueryResource(szName);

		//
		// 1. 参数安全检查
		//
		if (ptrShader.IsNull() || ptrShader->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 设置顶点着色器
		//
		m_ptrShaderVertex = ptrShader;
		m_pass.program.vertex = ptrShader->GetShaderGL();

		return TRUE;
	}

	//
	// 设置片源着色器
	//
	BOOL CMatPass::SetShaderFragment(const CHAR *szName)
	{
		ASSERT(szName);
		const CShaderPtr &ptrShader = ShaderManager()->QueryResource(szName);

		//
		// 1. 参数安全检查
		//
		if (ptrShader.IsNull() || ptrShader->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 设置片源着色器
		//
		m_ptrShaderFragment = ptrShader;
		m_pass.program.fragment = ptrShader->GetShaderGL();

		return TRUE;
	}

	//
	// 获得顶点着色器
	//
	const CShaderPtr& CMatPass::GetShaderVertex(VOID) const
	{
		return m_ptrShaderVertex;
	}

	//
	// 获得片源着色器
	//
	const CShaderPtr& CMatPass::GetShaderFragment(VOID) const
	{
		return m_ptrShaderFragment;
	}

	//
	// 计算评价值
	//
	UINT64 CMatPass::Evaluate(VOID) const
	{
		UINT64 vertex = m_pass.program.vertex;
		UINT64 fragment = m_pass.program.fragment;
		UINT64 texture0 = m_pass.textures[0].texture;
		UINT64 texture1 = m_pass.textures[1].texture;

		return 0xffffffffffffffff - ((vertex << 48) | (fragment << 32) | (texture0 << 16) | texture1);
	}

	//
	// 获得透明状态
	//
	BOOL CMatPass::IsTransparence(VOID) const
	{
		return m_pass.blend.bEnableBlend &&
			(GL_SRC_ALPHA == m_pass.blend.blendSrcAlpha || GL_ONE_MINUS_SRC_ALPHA == m_pass.blend.blendSrcAlpha || GL_DST_ALPHA == m_pass.blend.blendSrcAlpha || GL_ONE_MINUS_DST_ALPHA == m_pass.blend.blendSrcAlpha ||
			 GL_SRC_ALPHA == m_pass.blend.blendDesAlpha || GL_ONE_MINUS_SRC_ALPHA == m_pass.blend.blendDesAlpha || GL_DST_ALPHA == m_pass.blend.blendDesAlpha || GL_ONE_MINUS_DST_ALPHA == m_pass.blend.blendDesAlpha) ? TRUE : FALSE;
	}

	//
	// 加载XML
	//
	VOID CMatPass::LoadXML(TiXmlNode *pPassNode)
	{
		ASSERT(pPassNode);

		//
		// 1. 释放渲染通路
		//
		Free();

		//
		// 2. 加载渲染通路
		//
		LoadRenderID(pPassNode);
		LoadLighting(pPassNode);
		LoadCull(pPassNode);
		LoadBlend(pPassNode);
		LoadDepth(pPassNode);
		LoadAlpha(pPassNode);
		LoadTextureUnits(pPassNode);
		LoadShaderVertex(pPassNode);
		LoadShaderFragment(pPassNode);

		//
		// 2. 创建着色器程序
		//
		CreateProgram();
	}

	//
	// 加载渲染ID
	//
	VOID CMatPass::LoadRenderID(TiXmlNode *pPassNode)
	{
		ASSERT(pPassNode);

		if (TiXmlNode *pRenderIDNode = pPassNode->FirstChild("RenderID")) {
			SetRenderID(pRenderIDNode->ToElement()->AttributeInt("id"));
		}
	}

	//
	// 加载光照参数
	//
	VOID CMatPass::LoadLighting(TiXmlNode *pPassNode)
	{
		ASSERT(pPassNode);

		if (TiXmlNode *pLightingNode = pPassNode->FirstChild("Lighting")) {
			m_pass.lighting.bEnableLighting = cglStringToAttrib(pLightingNode->ToElement()->AttributeString("enable"));

			if (TiXmlNode *pAmbientNode = pLightingNode->FirstChild("Ambient")) {
				SetAmbient(
					pAmbientNode->ToElement()->AttributeFloat("r"),
					pAmbientNode->ToElement()->AttributeFloat("g"),
					pAmbientNode->ToElement()->AttributeFloat("b"),
					pAmbientNode->ToElement()->AttributeFloat("a"));
			}

			if (TiXmlNode *pDiffuseNode = pLightingNode->FirstChild("Diffuse")) {
				SetDiffuse(
					pDiffuseNode->ToElement()->AttributeFloat("r"),
					pDiffuseNode->ToElement()->AttributeFloat("g"),
					pDiffuseNode->ToElement()->AttributeFloat("b"),
					pDiffuseNode->ToElement()->AttributeFloat("a"));
			}

			if (TiXmlNode *pEmissionNode = pLightingNode->FirstChild("Emission")) {
				SetEmission(
					pEmissionNode->ToElement()->AttributeFloat("r"),
					pEmissionNode->ToElement()->AttributeFloat("g"),
					pEmissionNode->ToElement()->AttributeFloat("b"),
					pEmissionNode->ToElement()->AttributeFloat("a"));
			}

			if (TiXmlNode *pSpecularNode = pLightingNode->FirstChild("Specular")) {
				SetSpecular(
					pSpecularNode->ToElement()->AttributeFloat("r"),
					pSpecularNode->ToElement()->AttributeFloat("g"),
					pSpecularNode->ToElement()->AttributeFloat("b"),
					pSpecularNode->ToElement()->AttributeFloat("a"));
			}

			if (TiXmlNode *pShininessNode = pLightingNode->FirstChild("Shininess")) {
				SetShininess(pShininessNode->ToElement()->AttributeFloat("level"));
			}
		}
	}

	//
	// 加载剔除参数
	//
	VOID CMatPass::LoadCull(TiXmlNode *pPassNode)
	{
		ASSERT(pPassNode);

		if (TiXmlNode *pCullNode = pPassNode->FirstChild("Cull")) {
			SetEnableCullFace(cglStringToAttrib(pCullNode->ToElement()->AttributeString("enable")));

			if (TiXmlNode *pCullFaceModeNode = pCullNode->FirstChild("CullFaceMode")) {
				SetCullFaceMode(cglStringToAttrib(pCullFaceModeNode->ToElement()->AttributeString("mode")));
			}

			if (TiXmlNode *pFrontFaceModeNode = pCullNode->FirstChild("FrontFaceMode")) {
				SetFrontFaceMode(cglStringToAttrib(pFrontFaceModeNode->ToElement()->AttributeString("mode")));
			}
		}
	}

	//
	// 加载混合参数
	//
	VOID CMatPass::LoadBlend(TiXmlNode *pPassNode)
	{
		ASSERT(pPassNode);

		if (TiXmlNode *pBlendNode = pPassNode->FirstChild("Blend")) {
			SetEnableBlend(cglStringToAttrib(pBlendNode->ToElement()->AttributeString("enable")));

			if (TiXmlNode *pBlendEquationNode = pBlendNode->FirstChild("BlendMode")) {
				SetBlendMode(
					cglStringToAttrib(pBlendEquationNode->ToElement()->AttributeString("mode_color")),
					cglStringToAttrib(pBlendEquationNode->ToElement()->AttributeString("mode_alpha")));
			}

			if (TiXmlNode *pBlendFuncNode = pBlendNode->FirstChild("BlendFunc")) {
				SetBlendFunc(
					cglStringToAttrib(pBlendFuncNode->ToElement()->AttributeString("src_color")),
					cglStringToAttrib(pBlendFuncNode->ToElement()->AttributeString("src_alpha")),
					cglStringToAttrib(pBlendFuncNode->ToElement()->AttributeString("des_color")),
					cglStringToAttrib(pBlendFuncNode->ToElement()->AttributeString("des_alpha")));
			}
		}
	}

	//
	// 加载深度参数
	//
	VOID CMatPass::LoadDepth(TiXmlNode *pPassNode)
	{
		ASSERT(pPassNode);

		if (TiXmlNode *pDepthNode = pPassNode->FirstChild("Depth")) {
			SetEnableDepthTest(cglStringToAttrib(pDepthNode->ToElement()->AttributeString("enable")));

			if (TiXmlNode *pEnableDepthWriteNode = pDepthNode->FirstChild("DepthWrite")) {
				SetEnableDepthWrite(cglStringToAttrib(pEnableDepthWriteNode->ToElement()->AttributeString("enable")));
			}

			if (TiXmlNode *pDepthFuncNode = pDepthNode->FirstChild("DepthFunc")) {
				SetDepthFunc(cglStringToAttrib(pDepthFuncNode->ToElement()->AttributeString("func")));
			}
		}
	}

	//
	// 加载透明参数
	//
	VOID CMatPass::LoadAlpha(TiXmlNode *pPassNode)
	{
		ASSERT(pPassNode);

		if (TiXmlNode *pAlphaNode = pPassNode->FirstChild("Alpha")) {
			SetEnableAlphaTest(cglStringToAttrib(pAlphaNode->ToElement()->AttributeString("enable")));

			if (TiXmlNode *pAlphaFuncNode = pAlphaNode->FirstChild("AlphaFunc")) {
				SetAlphaFunc(cglStringToAttrib(pAlphaFuncNode->ToElement()->AttributeString("func")));
			}

			if (TiXmlNode *pAlphaRefNode = pAlphaNode->FirstChild("AlphaRef")) {
				SetAlphaRef(pAlphaRefNode->ToElement()->AttributeFloat("ref"));
			}
		}
	}

	//
	// 加载纹理单元
	//
	VOID CMatPass::LoadTextureUnits(TiXmlNode *pPassNode)
	{
		ASSERT(pPassNode);

		if (TiXmlNode *pTextureNode = pPassNode->FirstChild("Texture")) {
			do {
				//
				// 1. 设置纹理
				//
				INT indexChannel = pTextureNode->ToElement()->AttributeInt("channel");
				const CHAR *szName = pTextureNode->ToElement()->AttributeString("name");
				if (szName == NULL) throw "No texture name.";
				if (indexChannel < 0 || indexChannel >= MAX_TEXUNITS) throw "Invalid texture channel.";
				if (SetTexture(indexChannel, szName) == FALSE) throw "Invalid texture.";

				//
				// 2. 设置纹理参数
				//
				SetTextureParam(indexChannel, 
					cglStringToAttrib(pTextureNode->ToElement()->AttributeString("min_filter")),
					cglStringToAttrib(pTextureNode->ToElement()->AttributeString("mag_filter")),
					cglStringToAttrib(pTextureNode->ToElement()->AttributeString("wrapu")),
					cglStringToAttrib(pTextureNode->ToElement()->AttributeString("wrapv")));

				//
				// 3. 设置纹理动画参数
				//
				if (pTextureNode->ToElement()->AttributeString("scrollu")) {
					SetTextureScrollU(indexChannel, 
						pTextureNode->ToElement()->AttributeFloat("scrollu"));
				}

				if (pTextureNode->ToElement()->AttributeString("scrollv")) {
					SetTextureScrollV(indexChannel, 
						pTextureNode->ToElement()->AttributeFloat("scrollv"));
				}

				if (pTextureNode->ToElement()->AttributeString("cols") && 
					pTextureNode->ToElement()->AttributeString("rows") && 
					pTextureNode->ToElement()->AttributeString("duration")) {
					SetTextureSequence(indexChannel, 
						pTextureNode->ToElement()->AttributeInt("cols"), 
						pTextureNode->ToElement()->AttributeInt("rows"), 
						pTextureNode->ToElement()->AttributeFloat("duration"));
				}
			} while (pTextureNode = pPassNode->IterateChildren("Texture", pTextureNode));
		}
	}

	//
	// 加载顶点着色器
	//
	VOID CMatPass::LoadShaderVertex(TiXmlNode *pPassNode)
	{
		ASSERT(pPassNode);

		if (TiXmlNode *pShaderNode = pPassNode->FirstChild("ShaderVertex")) {
			//
			// 1. 获得着色器
			//
			const CHAR *szName = pShaderNode->ToElement()->AttributeString("name");
			if (szName == NULL) throw "No vertex shader name.";
			if (SetShaderVertex(szName) == FALSE) throw "Invalid vertex shader.";

			//
			// 2. 设置着色器默认参数
			//
			SetShaderParams(m_ptrShaderVertex->GetParams());

			//
			// 3. 设置着色器材质参数
			//
			LoadShaderParams(pShaderNode);

			//
			// 4. 保存着色器
			//
			m_pass.program.vertex = m_ptrShaderVertex->GetShaderGL();
		}
	}

	//
	// 加载片段着色器
	//
	VOID CMatPass::LoadShaderFragment(TiXmlNode *pPassNode)
	{
		ASSERT(pPassNode);

		if (TiXmlNode *pShaderNode = pPassNode->FirstChild("ShaderFragment")) {
			//
			// 1. 获得着色器
			//
			const CHAR *szName = pShaderNode->ToElement()->AttributeString("name");
			if (szName == NULL) throw "No fragment shader name.";
			if (SetShaderFragment(szName) == FALSE) throw "Invalid fragment shader.";

			//
			// 2. 设置着色器默认参数
			//
			SetShaderParams(m_ptrShaderFragment->GetParams());

			//
			// 3. 设置着色器材质参数
			//
			LoadShaderParams(pShaderNode);

			//
			// 4. 保存着色器
			//
			m_pass.program.fragment = m_ptrShaderFragment->GetShaderGL();
		}
	}

	//
	// 加载着色器参数
	//
	VOID CMatPass::LoadShaderParams(TiXmlNode *pShaderNode)
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

					m_pass.program.uniformExternalLocations[HashValue(name)] = param;
				} while (pParamNode = pParamsNode->IterateChildren(pParamNode));
			}
		}
	}

	//
	// 设置着色器参数
	//
	VOID CMatPass::SetShaderParams(const CRenderer::UniformLocationMap& params)
	{
		for (CRenderer::UniformLocationMap::const_iterator itParam = params.begin(); itParam != params.end(); ++itParam) {
			m_pass.program.uniformExternalLocations[itParam->first] = itParam->second;
		}
	}

	//
	// 创建着色器程序
	//
	VOID CMatPass::CreateProgram(VOID)
	{
		if (m_pass.program.vertex != 0 && m_pass.program.fragment != 0) {
			if (Renderer()->CreateProgram(m_pass.program.vertex, m_pass.program.fragment, m_pass.program.uniformExternalLocations) == FALSE) {
				throw "CreateProgram failed.";
			}
		}
	}

	//
	// 保存XML
	//
	VOID CMatPass::SaveXML(TiXmlNode *pPassNode) const
	{
		ASSERT(pPassNode);
		static const CMatPass defaultPass(NULL);

		SaveRenderID(pPassNode);
		SaveLighting(pPassNode, &defaultPass.GetPass()->lighting);
		SaveCull(pPassNode, &defaultPass.GetPass()->cull);
		SaveBlend(pPassNode, &defaultPass.GetPass()->blend);
		SaveDepth(pPassNode, &defaultPass.GetPass()->depth);
		SaveAlpha(pPassNode, &defaultPass.GetPass()->alpha);
		SaveTextureUnits(pPassNode);
		SaveShaderVertex(pPassNode);
		SaveShaderFragment(pPassNode);
	}

	//
	// 保存渲染ID
	//
	VOID CMatPass::SaveRenderID(TiXmlNode *pPassNode) const
	{
		ASSERT(pPassNode);

		TiXmlElement *pRenderIDNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("RenderID"); ASSERT(pRenderIDNode);
		{
			pRenderIDNode->SetAttributeInt("id", m_id);
		}
		pPassNode->LinkEndChild(pRenderIDNode);
	}

	//
	// 保存光照参数
	//
	VOID CMatPass::SaveLighting(TiXmlNode *pPassNode, const CRenderer::PASS_LIGHTING *pDefault) const
	{
		ASSERT(pPassNode);
		ASSERT(pDefault);

		TiXmlElement *pLightingNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Lighting"); ASSERT(pLightingNode);
		{
			pLightingNode->SetAttributeString("enable", cglAttribToString(m_pass.lighting.bEnableLighting));

			if (m_pass.lighting.bEnableLighting) {
				if (m_pass.lighting.matAmbient[0] != pDefault->matAmbient[0] ||
					m_pass.lighting.matAmbient[1] != pDefault->matAmbient[1] ||
					m_pass.lighting.matAmbient[2] != pDefault->matAmbient[2] ||
					m_pass.lighting.matAmbient[3] != pDefault->matAmbient[3]) {
					TiXmlElement *pAmbientNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Ambient"); ASSERT(pAmbientNode);
					{
						pAmbientNode->ToElement()->SetAttributeFloat("r", m_pass.lighting.matAmbient[0]);
						pAmbientNode->ToElement()->SetAttributeFloat("g", m_pass.lighting.matAmbient[1]);
						pAmbientNode->ToElement()->SetAttributeFloat("b", m_pass.lighting.matAmbient[2]);
						pAmbientNode->ToElement()->SetAttributeFloat("a", m_pass.lighting.matAmbient[3]);
					}
					pLightingNode->LinkEndChild(pAmbientNode);
				}

				if (m_pass.lighting.matDiffuse[0] != pDefault->matDiffuse[0] ||
					m_pass.lighting.matDiffuse[1] != pDefault->matDiffuse[1] ||
					m_pass.lighting.matDiffuse[2] != pDefault->matDiffuse[2] ||
					m_pass.lighting.matDiffuse[3] != pDefault->matDiffuse[3]) {
					TiXmlElement *pDiffuseNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Diffuse"); ASSERT(pDiffuseNode);
					{
						pDiffuseNode->ToElement()->SetAttributeFloat("r", m_pass.lighting.matDiffuse[0]);
						pDiffuseNode->ToElement()->SetAttributeFloat("g", m_pass.lighting.matDiffuse[1]);
						pDiffuseNode->ToElement()->SetAttributeFloat("b", m_pass.lighting.matDiffuse[2]);
						pDiffuseNode->ToElement()->SetAttributeFloat("a", m_pass.lighting.matDiffuse[3]);
					}
					pLightingNode->LinkEndChild(pDiffuseNode);
				}

				if (m_pass.lighting.matEmission[0] != pDefault->matEmission[0] ||
					m_pass.lighting.matEmission[1] != pDefault->matEmission[1] ||
					m_pass.lighting.matEmission[2] != pDefault->matEmission[2] ||
					m_pass.lighting.matEmission[3] != pDefault->matEmission[3]) {
					TiXmlElement *pEmissionNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Emission"); ASSERT(pEmissionNode);
					{
						pEmissionNode->ToElement()->SetAttributeFloat("r", m_pass.lighting.matEmission[0]);
						pEmissionNode->ToElement()->SetAttributeFloat("g", m_pass.lighting.matEmission[1]);
						pEmissionNode->ToElement()->SetAttributeFloat("b", m_pass.lighting.matEmission[2]);
						pEmissionNode->ToElement()->SetAttributeFloat("a", m_pass.lighting.matEmission[3]);
					}
					pLightingNode->LinkEndChild(pEmissionNode);
				}

				if (m_pass.lighting.matSpecular[0] != pDefault->matSpecular[0] ||
					m_pass.lighting.matSpecular[1] != pDefault->matSpecular[1] ||
					m_pass.lighting.matSpecular[2] != pDefault->matSpecular[2] ||
					m_pass.lighting.matSpecular[3] != pDefault->matSpecular[3]) {
					TiXmlElement *pSpecularNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Specular"); ASSERT(pSpecularNode);
					{
						pSpecularNode->ToElement()->SetAttributeFloat("r", m_pass.lighting.matSpecular[0]);
						pSpecularNode->ToElement()->SetAttributeFloat("g", m_pass.lighting.matSpecular[1]);
						pSpecularNode->ToElement()->SetAttributeFloat("b", m_pass.lighting.matSpecular[2]);
						pSpecularNode->ToElement()->SetAttributeFloat("a", m_pass.lighting.matSpecular[3]);
					}
					pLightingNode->LinkEndChild(pSpecularNode);
				}

				if (m_pass.lighting.matShininess != pDefault->matShininess) {
					TiXmlElement *pShininessNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Shininess"); ASSERT(pShininessNode);
					{
						pShininessNode->ToElement()->SetAttributeFloat("level", m_pass.lighting.matShininess);
					}
					pLightingNode->LinkEndChild(pShininessNode);
				}
			}
		}
		pPassNode->LinkEndChild(pLightingNode);
	}

	//
	// 保存剔除参数
	//
	VOID CMatPass::SaveCull(TiXmlNode *pPassNode, const CRenderer::PASS_CULL *pDefault) const
	{
		ASSERT(pPassNode);
		ASSERT(pDefault);

		TiXmlElement *pCullNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Cull"); ASSERT(pCullNode);
		{
			pCullNode->SetAttributeString("enable", cglAttribToString(m_pass.cull.bEnableCullFace));

			if (m_pass.cull.bEnableCullFace) {
				if (m_pass.cull.cullFaceMode != pDefault->cullFaceMode) {
					TiXmlElement *pCullFaceModeNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("CullFaceMode"); ASSERT(pCullFaceModeNode);
					{
						pCullFaceModeNode->SetAttributeString("mode", cglAttribToString(m_pass.cull.cullFaceMode));
					}
					pCullNode->LinkEndChild(pCullFaceModeNode);
				}

				if (m_pass.cull.frontFaceMode != pDefault->frontFaceMode) {
					TiXmlElement *pFrontFaceModeNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("FrontFaceMode"); ASSERT(pFrontFaceModeNode);
					{
						pFrontFaceModeNode->SetAttributeString("mode", cglAttribToString(m_pass.cull.frontFaceMode));
					}
					pCullNode->LinkEndChild(pFrontFaceModeNode);
				}
			}
		}
		pPassNode->LinkEndChild(pCullNode);
	}

	//
	// 保存混合参数
	//
	VOID CMatPass::SaveBlend(TiXmlNode *pPassNode, const CRenderer::PASS_BLEND *pDefault) const
	{
		ASSERT(pPassNode);
		ASSERT(pDefault);

		TiXmlElement *pBlendNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Blend"); ASSERT(pBlendNode);
		{
			pBlendNode->SetAttributeString("enable", cglAttribToString(m_pass.blend.bEnableBlend));

			if (m_pass.blend.bEnableBlend) {
				if (m_pass.blend.blendModeColor != pDefault->blendModeColor ||
					m_pass.blend.blendModeAlpha != pDefault->blendModeAlpha) {
					TiXmlElement *pBlendEquationNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("BlendMode"); ASSERT(pBlendEquationNode);
					{
						pBlendEquationNode->SetAttributeString("mode_color", cglAttribToString(m_pass.blend.blendModeColor));
						pBlendEquationNode->SetAttributeString("mode_alpha", cglAttribToString(m_pass.blend.blendModeAlpha));
					}
					pBlendNode->LinkEndChild(pBlendEquationNode);
				}

				if (m_pass.blend.blendSrcColor != pDefault->blendSrcColor ||
					m_pass.blend.blendDesColor != pDefault->blendDesColor ||
					m_pass.blend.blendSrcAlpha != pDefault->blendSrcAlpha ||
					m_pass.blend.blendDesAlpha != pDefault->blendDesAlpha) {
					TiXmlElement *pBlendFuncNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("BlendFunc"); ASSERT(pBlendFuncNode);
					{
						pBlendFuncNode->SetAttributeString("src_color", cglAttribToString(m_pass.blend.blendSrcColor));
						pBlendFuncNode->SetAttributeString("des_color", cglAttribToString(m_pass.blend.blendDesColor));
						pBlendFuncNode->SetAttributeString("src_alpha", cglAttribToString(m_pass.blend.blendSrcAlpha));
						pBlendFuncNode->SetAttributeString("des_alpha", cglAttribToString(m_pass.blend.blendDesAlpha));
					}
					pBlendNode->LinkEndChild(pBlendFuncNode);
				}
			}
		}
		pPassNode->LinkEndChild(pBlendNode);
	}

	//
	// 保存深度参数
	//
	VOID CMatPass::SaveDepth(TiXmlNode *pPassNode, const CRenderer::PASS_DEPTH *pDefault) const
	{
		ASSERT(pPassNode);
		ASSERT(pDefault);

		TiXmlElement *pDepthNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Depth"); ASSERT(pDepthNode);
		{
			pDepthNode->SetAttributeString("enable", cglAttribToString(m_pass.depth.bEnableDepthTest));

			if (m_pass.depth.bEnableDepthTest) {
				if (m_pass.depth.depthMask != pDefault->depthMask) {
					TiXmlElement *pEnableDepthWriteNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("DepthWrite"); ASSERT(pEnableDepthWriteNode);
					{
						pEnableDepthWriteNode->SetAttributeString("enable", cglAttribToString(m_pass.depth.depthMask));
					}
					pDepthNode->LinkEndChild(pEnableDepthWriteNode);
				}

				if (m_pass.depth.depthFunc != pDefault->depthFunc) {
					TiXmlElement *pDepthFuncNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("DepthFunc"); ASSERT(pDepthFuncNode);
					{
						pDepthFuncNode->SetAttributeString("func", cglAttribToString(m_pass.depth.depthFunc));
					}
					pDepthNode->LinkEndChild(pDepthFuncNode);
				}
			}
		}
		pPassNode->LinkEndChild(pDepthNode);
	}

	//
	// 保存透明参数
	//
	VOID CMatPass::SaveAlpha(TiXmlNode *pPassNode, const CRenderer::PASS_ALPHA *pDefault) const
	{
		ASSERT(pPassNode);
		ASSERT(pDefault);

		TiXmlElement *pAlphaNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Alpha"); ASSERT(pAlphaNode);
		{
			pAlphaNode->SetAttributeString("enable", cglAttribToString(m_pass.alpha.bEnableAlphaTest));

			if (m_pass.alpha.bEnableAlphaTest) {
				if (m_pass.alpha.alphaFunc != pDefault->alphaFunc) {
					TiXmlElement *pAlphaFuncNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("AlphaFunc"); ASSERT(pAlphaFuncNode);
					{
						pAlphaFuncNode->SetAttributeString("func", cglAttribToString(m_pass.alpha.alphaFunc));
					}
					pAlphaNode->LinkEndChild(pAlphaFuncNode);
				}

				if (m_pass.alpha.alphaRef != pDefault->alphaRef) {
					TiXmlElement *pAlphaRefNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("AlphaRef"); ASSERT(pAlphaRefNode);
					{
						pAlphaRefNode->SetAttributeFloat("ref", m_pass.alpha.alphaRef);
					}
					pAlphaNode->LinkEndChild(pAlphaRefNode);
				}
			}
		}
		pPassNode->LinkEndChild(pAlphaNode);
	}

	//
	// 保存纹理单元
	//
	VOID CMatPass::SaveTextureUnits(TiXmlNode *pPassNode) const
	{
		ASSERT(pPassNode);

		for (INT indexTexUnit = 0; indexTexUnit < MAX_TEXUNITS; indexTexUnit++) {
			if (m_pass.textures[indexTexUnit].texture == 0) continue;
			if (m_ptrTextures[indexTexUnit].IsNull() || m_ptrTextures[indexTexUnit]->IsValid() == FALSE) continue;

			TiXmlElement *pTextureNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Texture"); ASSERT(pTextureNode);
			{
				pTextureNode->SetAttributeInt("channel", indexTexUnit);
				pTextureNode->SetAttributeString("name", m_ptrTextures[indexTexUnit]->GetName());

				pTextureNode->SetAttributeString("min_filter", cglAttribToString(m_pass.textures[indexTexUnit].texMinFliter));
				pTextureNode->SetAttributeString("mag_filter", cglAttribToString(m_pass.textures[indexTexUnit].texMagFilter));
				pTextureNode->SetAttributeString("wrapu", cglAttribToString(m_pass.textures[indexTexUnit].texWrapU));
				pTextureNode->SetAttributeString("wrapv", cglAttribToString(m_pass.textures[indexTexUnit].texWrapV));

				pTextureNode->SetAttributeFloat("scrollu", m_pass.textures[indexTexUnit].texAnimation.texScrollU);
				pTextureNode->SetAttributeFloat("scrollv", m_pass.textures[indexTexUnit].texAnimation.texScrollV);

				pTextureNode->SetAttributeInt("cols", m_pass.textures[indexTexUnit].texAnimation.texSequenceCols);
				pTextureNode->SetAttributeInt("rows", m_pass.textures[indexTexUnit].texAnimation.texSequenceRows);
				pTextureNode->SetAttributeFloat("duration", m_pass.textures[indexTexUnit].texAnimation.texSequenceDurationTime);
			}
			pPassNode->LinkEndChild(pTextureNode);
		}
	}

	//
	// 保存顶点着色器
	//
	VOID CMatPass::SaveShaderVertex(TiXmlNode *pPassNode) const
	{
		ASSERT(pPassNode);

		TiXmlElement *pShaderNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("ShaderVertex"); ASSERT(pShaderNode);
		{
			pShaderNode->SetAttributeString("name", m_ptrShaderVertex->GetName());
			SaveShaderParams(pShaderNode, m_ptrShaderVertex->GetParams());
		}
		pPassNode->LinkEndChild(pShaderNode);
	}

	//
	// 保存片段着色器
	//
	VOID CMatPass::SaveShaderFragment(TiXmlNode *pPassNode) const
	{
		ASSERT(pPassNode);

		TiXmlElement *pShaderNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("ShaderFragment"); ASSERT(pShaderNode);
		{
			pShaderNode->SetAttributeString("name", m_ptrShaderFragment->GetName());
			SaveShaderParams(pShaderNode, m_ptrShaderFragment->GetParams());
		}
		pPassNode->LinkEndChild(pShaderNode);
	}

	//
	// 保存着色器参数
	//
	VOID CMatPass::SaveShaderParams(TiXmlNode *pShaderNode, const CRenderer::UniformLocationMap& refParams) const
	{
		ASSERT(pShaderNode);

		TiXmlElement *pParamsNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Parameters"); ASSERT(pParamsNode);
		{
			for (CRenderer::UniformLocationMap::const_iterator itParam = m_pass.program.uniformExternalLocations.begin(); itParam != m_pass.program.uniformExternalLocations.end(); ++itParam) {
				CRenderer::UniformLocationMap::const_iterator itRefParam = refParams.find(itParam->first);
				if (itRefParam == refParams.end()) continue;
				if (memcmp(itRefParam->second.value.data, itParam->second.value.data, sizeof(itParam->second.value.data)) == 0) continue;

				TiXmlElement *pParamNode = NULL;

				switch (itParam->second.type) {
				case CRenderer::UNIFORM_TYPE_INT:
					pParamNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Uniform1i"); ASSERT(pParamNode);
					pParamNode->SetAttributeString("name", itParam->second.name);
					pParamNode->SetAttributeString("value", "%d",
						itParam->second.value.nvalue[0]);
					break;
				case CRenderer::UNIFORM_TYPE_INT2:
					pParamNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Uniform2i"); ASSERT(pParamNode);
					pParamNode->SetAttributeString("name", itParam->second.name);
					pParamNode->SetAttributeString("value", "%d %d",
						itParam->second.value.nvalue[0], itParam->second.value.nvalue[1]);
					break;
				case CRenderer::UNIFORM_TYPE_INT3:
					pParamNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Uniform3i"); ASSERT(pParamNode);
					pParamNode->SetAttributeString("name", itParam->second.name);
					pParamNode->SetAttributeString("value", "%d %d %d",
						itParam->second.value.nvalue[0], itParam->second.value.nvalue[1], itParam->second.value.nvalue[2]);
					break;
				case CRenderer::UNIFORM_TYPE_INT4:
					pParamNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Uniform4i"); ASSERT(pParamNode);
					pParamNode->SetAttributeString("name", itParam->second.name);
					pParamNode->SetAttributeString("value", "%d %d %d %d",
						itParam->second.value.nvalue[0], itParam->second.value.nvalue[1], itParam->second.value.nvalue[2], itParam->second.value.nvalue[3]);
					break;
				case CRenderer::UNIFORM_TYPE_FLOAT:
					pParamNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Uniform1f"); ASSERT(pParamNode);
					pParamNode->SetAttributeString("name", itParam->second.name);
					pParamNode->SetAttributeString("value", "%f",
						itParam->second.value.fvalue[0]);
					break;
				case CRenderer::UNIFORM_TYPE_FLOAT2:
					pParamNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Uniform2f"); ASSERT(pParamNode);
					pParamNode->SetAttributeString("name", itParam->second.name);
					pParamNode->SetAttributeString("value", "%f %f",
						itParam->second.value.fvalue[0], itParam->second.value.fvalue[1]);
					break;
				case CRenderer::UNIFORM_TYPE_FLOAT3:
					pParamNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Uniform3f"); ASSERT(pParamNode);
					pParamNode->SetAttributeString("name", itParam->second.name);
					pParamNode->SetAttributeString("value", "%f %f %f",
						itParam->second.value.fvalue[0], itParam->second.value.fvalue[1], itParam->second.value.fvalue[2]);
					break;
				case CRenderer::UNIFORM_TYPE_FLOAT4:
					pParamNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Uniform4f"); ASSERT(pParamNode);
					pParamNode->SetAttributeString("name", itParam->second.name);
					pParamNode->SetAttributeString("value", "%f %f %f %f",
						itParam->second.value.fvalue[0], itParam->second.value.fvalue[1], itParam->second.value.fvalue[2], itParam->second.value.fvalue[3]);
					break;
				case CRenderer::UNIFORM_TYPE_MATRIX2:
					pParamNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("UniformMatrix2x2"); ASSERT(pParamNode);
					pParamNode->SetAttributeString("name", itParam->second.name);
					pParamNode->SetAttributeString("value", "%f %f %f %f",
						itParam->second.value.fvalue[0], itParam->second.value.fvalue[1], itParam->second.value.fvalue[2], itParam->second.value.fvalue[3]);
					break;
				case CRenderer::UNIFORM_TYPE_MATRIX3:
					pParamNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("UniformMatrix3x3"); ASSERT(pParamNode);
					pParamNode->SetAttributeString("name", itParam->second.name);
					pParamNode->SetAttributeString("value", "%f %f %f %f %f %f %f %f %f",
						itParam->second.value.fvalue[0], itParam->second.value.fvalue[1], itParam->second.value.fvalue[2], itParam->second.value.fvalue[3],
						itParam->second.value.fvalue[4], itParam->second.value.fvalue[5], itParam->second.value.fvalue[6], itParam->second.value.fvalue[7], 
						itParam->second.value.fvalue[8]);
					break;
				case CRenderer::UNIFORM_TYPE_MATRIX4:
					pParamNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("UniformMatrix4x4"); ASSERT(pParamNode);
					pParamNode->SetAttributeString("name", itParam->second.name);
					pParamNode->SetAttributeString("value", "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
						itParam->second.value.fvalue[0], itParam->second.value.fvalue[1], itParam->second.value.fvalue[2], itParam->second.value.fvalue[3],
						itParam->second.value.fvalue[4], itParam->second.value.fvalue[5], itParam->second.value.fvalue[6], itParam->second.value.fvalue[7], 
						itParam->second.value.fvalue[8], itParam->second.value.fvalue[9], itParam->second.value.fvalue[10], itParam->second.value.fvalue[11],
						itParam->second.value.fvalue[12], itParam->second.value.fvalue[13], itParam->second.value.fvalue[14], itParam->second.value.fvalue[15]);
					break;
				}

				pParamsNode->LinkEndChild(pParamNode);
			}
		}
		pShaderNode->LinkEndChild(pParamsNode);
	}

}

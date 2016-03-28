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

	CEntityMesh::CEntityMesh(DWORD dwName, CSceneManager *pSceneManager)
		: CEntityRenderable(dwName, pSceneManager)
		, m_dwRenderMeshName(INVALID_HASHNAME)
		, m_dwPhysicsMeshName(INVALID_HASHNAME)

		, m_bEnablePhysicsQuery(FALSE)
		, m_bEnablePhysicsSimulation(FALSE)
		, m_nPhysicsSimulationTypeIndex(0)
	{

	}

	CEntityMesh::~CEntityMesh(VOID)
	{
		m_ptrMesh.SetNull();
		m_ptrDiffuseMaterial.SetNull();
		m_ptrPhysicsMaterial.SetNull();
		m_ptrLightMapTexture.SetNull();
	}

	//
	// 获得类型
	//
	CEntity::TYPE CEntityMesh::GetType(VOID) const
	{
		return CEntity::MESH;
	}

	//
	// 获得可渲染
	//
	BOOL CEntityMesh::IsRenderable(VOID) const
	{
		return TRUE;
	}

	//
	// 设置网格
	//
	BOOL CEntityMesh::SetMesh(const CHAR *szName)
	{
		ASSERT(szName);
		return SetMesh(HashValue(szName));
	}

	//
	// 设置网格
	//
	BOOL CEntityMesh::SetMesh(DWORD dwName)
	{
		return SetMesh(MeshManager()->QueryResource(dwName));
	}

	//
	// 设置网格
	//
	BOOL CEntityMesh::SetMesh(const CMeshPtr &ptrMesh)
	{
		m_ptrMesh.SetNull();
		m_ptrDiffuseMaterial.SetNull();
		m_ptrPhysicsMaterial.SetNull();
		m_ptrLightMapTexture.SetNull();

		m_dwRenderMeshName = INVALID_HASHNAME;
		m_dwPhysicsMeshName = INVALID_HASHNAME;

		m_bEnablePhysicsQuery = FALSE;
		m_bEnablePhysicsSimulation = FALSE;
		m_nPhysicsSimulationTypeIndex = 0;

		//
		// 1. 参数安全检查
		//
		if (ptrMesh.IsNull() || ptrMesh->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 设置网格
		//
		m_ptrMesh = ptrMesh;

		return TRUE;
	}

	//
	// 设置渲染子网格
	//
	BOOL CEntityMesh::SetRenderSubMesh(const CHAR *szName)
	{
		ASSERT(szName);
		return SetRenderSubMesh(HashValue(szName));
	}

	//
	// 设置渲染子网格
	//
	BOOL CEntityMesh::SetRenderSubMesh(DWORD dwName)
	{
		m_ptrDiffuseMaterial.SetNull();
		m_dwRenderMeshName = INVALID_HASHNAME;

		//
		// 1. 参数安全检查
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return FALSE;
		}

		const CSubMesh *pRenderMesh = m_ptrMesh->GetSubMesh(dwName);
		if (pRenderMesh == NULL) return FALSE;

		//
		// 2. 设置渲染子网格
		//
		m_dwRenderMeshName = dwName;
		m_ptrDiffuseMaterial = pRenderMesh->GetMaterial();

		return TRUE;
	}

	//
	// 设置物理子网格
	//
	BOOL CEntityMesh::SetPhysicsSubMesh(const CHAR *szName)
	{
		ASSERT(szName);
		return SetPhysicsSubMesh(HashValue(szName));
	}

	//
	// 设置物理子网格
	//
	BOOL CEntityMesh::SetPhysicsSubMesh(DWORD dwName)
	{
		m_ptrPhysicsMaterial.SetNull();
		m_dwPhysicsMeshName = INVALID_HASHNAME;

		//
		// 1. 参数安全检查
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return FALSE;
		}

		const CSubMesh *pPhysicsMesh = m_ptrMesh->GetSubMesh(dwName);
		if (pPhysicsMesh == NULL) return FALSE;

		//
		// 2. 设置物理子网格
		//
		m_dwPhysicsMeshName = dwName;
		m_ptrPhysicsMaterial = pPhysicsMesh->GetMaterial();

		return TRUE;
	}

	//
	// 获得网格
	//
	const CMeshPtr& CEntityMesh::GetMesh(VOID) const
	{
		return m_ptrMesh;
	}

	//
	// 获得渲染子网格
	//
	const CSubMesh* CEntityMesh::GetRenderSubMesh(VOID) const
	{
		return m_ptrMesh.IsNull() == FALSE && m_ptrMesh->IsValid() ? m_ptrMesh->GetSubMesh(m_dwRenderMeshName) : NULL;
	}

	//
	// 获得物理子网格
	//
	const CSubMesh* CEntityMesh::GetPhysicsSubMesh(VOID) const
	{
		return m_ptrMesh.IsNull() == FALSE && m_ptrMesh->IsValid() ? m_ptrMesh->GetSubMesh(m_dwPhysicsMeshName) : NULL;
	}

	//
	// 设置漫反射材质
	//
	BOOL CEntityMesh::SetDiffuseMaterial(const CHAR *szName)
	{
		ASSERT(szName);
		return SetDiffuseMaterial(HashValue(szName));
	}

	//
	// 设置漫反射材质
	//
	BOOL CEntityMesh::SetDiffuseMaterial(DWORD dwName)
	{
		return SetDiffuseMaterial(MaterialManager()->QueryResource(dwName));
	}

	//
	// 设置漫反射材质
	//
	BOOL CEntityMesh::SetDiffuseMaterial(const CMaterialPtr &ptrMaterial)
	{
		m_ptrDiffuseMaterial.SetNull();

		//
		// 1. 参数安全检查
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return FALSE;
		}

		if (ptrMaterial.IsNull() || ptrMaterial->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 设置漫反射材质
		//
		m_ptrDiffuseMaterial = ptrMaterial;

		return TRUE;
	}

	//
	// 设置物理材质
	//
	BOOL CEntityMesh::SetPhysicsMaterial(const CHAR *szName)
	{
		ASSERT(szName);
		return SetPhysicsMaterial(HashValue(szName));
	}

	//
	// 设置物理材质
	//
	BOOL CEntityMesh::SetPhysicsMaterial(DWORD dwName)
	{
		return SetPhysicsMaterial(MaterialManager()->QueryResource(dwName));
	}

	//
	// 设置物理材质
	//
	BOOL CEntityMesh::SetPhysicsMaterial(const CMaterialPtr &ptrMaterial)
	{
		m_ptrPhysicsMaterial.SetNull();

		//
		// 1. 参数安全检查
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return FALSE;
		}

		if (ptrMaterial.IsNull() || ptrMaterial->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 设置物理材质
		//
		m_ptrPhysicsMaterial = ptrMaterial;

		return TRUE;
	}

	//
	// 设置光照贴图纹理
	//
	BOOL CEntityMesh::SetLightMapTexture(const CHAR *szName, FLOAT offsetu, FLOAT offsetv, FLOAT scaleu, FLOAT scalev)
	{
		ASSERT(szName);
		return SetLightMapTexture(HashValue(szName), offsetu, offsetv, scaleu, scalev);
	}

	//
	// 设置光照贴图纹理
	//
	BOOL CEntityMesh::SetLightMapTexture(DWORD dwName, FLOAT offsetu, FLOAT offsetv, FLOAT scaleu, FLOAT scalev)
	{
		return SetLightMapTexture(TextureManager()->QueryResource(dwName), offsetu, offsetv, scaleu, scalev);
	}

	//
	// 设置光照贴图纹理
	//
	BOOL CEntityMesh::SetLightMapTexture(const CTexturePtr &ptrTexture, FLOAT offsetu, FLOAT offsetv, FLOAT scaleu, FLOAT scalev)
	{
		m_ptrLightMapTexture.SetNull();
		MtxIdentity(&m_mtxTextures[1]);

		//
		// 1. 参数安全检查
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return FALSE;
		}

		if (ptrTexture.IsNull() || ptrTexture->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 设置光照贴图纹理
		//
		MATRIX4 mtxScale;
		MATRIX4 mtxTranslate;

		MtxDefScale(&mtxScale, scaleu, scalev, 1.0f);
		MtxDefTranslate(&mtxTranslate, offsetu, offsetv, 0.0f);
		MtxMul(&m_mtxTextures[1], &mtxScale, &mtxTranslate);

		m_ptrLightMapTexture = ptrTexture;

		return TRUE;
	}

	//
	// 获得漫反射材质
	//
	const CMaterialPtr& CEntityMesh::GetMaterial(VOID) const
	{
		return m_ptrDiffuseMaterial;
	}

	//
	// 获得物理材质
	//
	const CMaterialPtr& CEntityMesh::GetPhysicsMaterial(VOID) const
	{
		return m_ptrPhysicsMaterial;
	}

	//
	// 获得光照贴图纹理
	//
	const CTexturePtr& CEntityMesh::GetLightMapTexture(VOID) const
	{
		return m_ptrLightMapTexture;
	}

	//
	// 设置纹理矩阵
	//
	BOOL CEntityMesh::SetTextureMatrix(INT indexTexUnit, const MATRIX4 *matrix)
	{
		ASSERT(matrix);

		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return FALSE;
		}

		//
		// 2. 设置纹理矩阵
		//
		MtxCopy(&m_mtxTextures[indexTexUnit], matrix);

		return TRUE;
	}

	//
	// 获得纹理矩阵
	//
	const MATRIX4* CEntityMesh::GetTextureMatrix(INT indexTexUnit) const
	{
		//
		// 1. 参数安全检查
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return NULL;
		}

		//
		// 2. 获得纹理矩阵
		//
		return &m_mtxTextures[indexTexUnit];
	}

	//
	// 获得光照贴图变换矩阵
	//
	const MATRIX4* CEntityMesh::GetLightMapTextureMatrix(VOID) const
	{
		return &m_mtxTextures[1];
	}

	//
	// 设置物理查询状态
	//
	VOID CEntityMesh::SetEnablePhysicsQuery(BOOL bEnable)
	{
		m_bEnablePhysicsQuery = bEnable;
	}

	//
	// 设置物理模拟状态
	//
	VOID CEntityMesh::SetEnablePhysicsSimulation(BOOL bEnable)
	{
		m_bEnablePhysicsSimulation = bEnable;
	}

	//
	// 设置物理模拟类型索引
	//
	VOID CEntityMesh::SetPhysicsSimulationTypeIndex(INT index)
	{
		m_nPhysicsSimulationTypeIndex = index;
	}

	//
	// 获得物理查询状态
	//
	BOOL CEntityMesh::IsEnablePhysicsQuery(VOID) const
	{
		return m_bEnablePhysicsQuery;
	}

	//
	// 获得物理模拟状态
	//
	BOOL CEntityMesh::IsEnablePhysicsSimulation(VOID) const
	{
		return m_bEnablePhysicsSimulation;
	}

	//
	// 获得物理模拟类型索引
	//
	INT CEntityMesh::GetPhysicsSimulationTypeIndex(VOID) const
	{
		return m_nPhysicsSimulationTypeIndex;
	}

	//
	// 渲染
	//
	VOID CEntityMesh::Render(const CMaterial *pMaterial) const
	{
		//
		// 1. 参数安全检查
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return;
		}

		//
		// 2. 渲染
		//
		if (const CSubMesh *pRenderMesh = m_ptrMesh->GetSubMesh(m_dwRenderMeshName)) {
			Renderer()->PushMatrix();
			Renderer()->PushTextureMatrix();
			{
				//
				// 2.1. 设置光照贴图纹理
				//
				if (m_ptrLightMapTexture.IsNull() || m_ptrLightMapTexture->IsValid() == FALSE) {
					Renderer()->SetLightMapTexture(0);
				}
				else {
					Renderer()->SetLightMapTexture(m_ptrLightMapTexture->GetTextureGL());
				}

				//
				// 2.2. 设置动态光源
				//
				VEC4 direction;
				VEC3 ambient, diffuse, specular;
				VEC3 rim, skyLower, skyUpper;
				VEC3 indirectUp, indirectDown, indirectLeft, indirectRight, indirectFront, indirectBack;
				FLOAT attenuation[] = { 1.0f, 0.0f, 0.0f };

				if (m_pSceneManager->GetLightParams(m_dwLightChannel, &m_aabbWorld.center, &direction, &ambient, &diffuse, &specular, &rim, &skyLower, &skyUpper, &indirectUp, &indirectDown, &indirectLeft, &indirectRight, &indirectFront, &indirectBack)) {
					Renderer()->SetLight(direction, ambient, diffuse, specular, rim, skyLower, skyUpper, attenuation);
				}

				//
				// 2.3. 设置变换矩阵
				//
				MATRIX4 mtxTransform;

				if (m_pSceneNode) {
					MtxCopy(&mtxTransform, m_pSceneNode->GetWorldMatrix());
				}
				else {
					MtxIdentity(&mtxTransform);
				}

				Renderer()->SetModelWorldMatrix(mtxTransform);

				//
				// 2.4. 设置纹理矩阵
				//
				for (INT indexTexUnit = 0; indexTexUnit < MAX_TEXUNITS; indexTexUnit++) {
					Renderer()->SetTextureMatrix(indexTexUnit, m_mtxTextures[indexTexUnit]);
				}

				//
				// 2.5. 渲染
				//
				Renderer()->BindVBO(GL_ARRAY_BUFFER, pRenderMesh->GetVBO());
				Renderer()->BindVBO(GL_ELEMENT_ARRAY_BUFFER, pRenderMesh->GetIBO());
				Renderer()->SubmitVertexBuffer(CRenderer::VERTEX_FORMAT_STATICMESH, NULL);
				Renderer()->Render(NULL, pRenderMesh->GetFaceCount() * 3);
			}
			Renderer()->PopTextureMatrix();
			Renderer()->PopMatrix();
		}
	}

	//
	// 渲染遮挡
	//
	VOID CEntityMesh::RenderOcclude(VOID) const
	{
		//
		// 1. 参数安全检查
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return;
		}

		//
		// 2. 渲染
		//
		if (const CSubMesh *pRenderMesh = m_ptrMesh->GetSubMesh(m_dwRenderMeshName)) {
			Renderer()->PushMatrix();
			{
				//
				// 2.1. 设置变换矩阵
				//
				MATRIX4 mtxTransform;

				if (m_pSceneNode) {
					MtxCopy(&mtxTransform, m_pSceneNode->GetWorldMatrix());
				}
				else {
					MtxIdentity(&mtxTransform);
				}

				Renderer()->SetModelWorldMatrix(mtxTransform);

				//
				// 2.2. 渲染
				//
				Renderer()->BindVBO(GL_ARRAY_BUFFER, pRenderMesh->GetVBO());
				Renderer()->BindVBO(GL_ELEMENT_ARRAY_BUFFER, pRenderMesh->GetIBO());
				Renderer()->SubmitVertexBuffer(CRenderer::VERTEX_FORMAT_POSITION, NULL);
				Renderer()->Render(NULL, pRenderMesh->GetFaceCount() * 3);
			}
			Renderer()->PopMatrix();
		}
	}

}

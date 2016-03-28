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
	// �������
	//
	CEntity::TYPE CEntityMesh::GetType(VOID) const
	{
		return CEntity::MESH;
	}

	//
	// ��ÿ���Ⱦ
	//
	BOOL CEntityMesh::IsRenderable(VOID) const
	{
		return TRUE;
	}

	//
	// ��������
	//
	BOOL CEntityMesh::SetMesh(const CHAR *szName)
	{
		ASSERT(szName);
		return SetMesh(HashValue(szName));
	}

	//
	// ��������
	//
	BOOL CEntityMesh::SetMesh(DWORD dwName)
	{
		return SetMesh(MeshManager()->QueryResource(dwName));
	}

	//
	// ��������
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
		// 1. ������ȫ���
		//
		if (ptrMesh.IsNull() || ptrMesh->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. ��������
		//
		m_ptrMesh = ptrMesh;

		return TRUE;
	}

	//
	// ������Ⱦ������
	//
	BOOL CEntityMesh::SetRenderSubMesh(const CHAR *szName)
	{
		ASSERT(szName);
		return SetRenderSubMesh(HashValue(szName));
	}

	//
	// ������Ⱦ������
	//
	BOOL CEntityMesh::SetRenderSubMesh(DWORD dwName)
	{
		m_ptrDiffuseMaterial.SetNull();
		m_dwRenderMeshName = INVALID_HASHNAME;

		//
		// 1. ������ȫ���
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return FALSE;
		}

		const CSubMesh *pRenderMesh = m_ptrMesh->GetSubMesh(dwName);
		if (pRenderMesh == NULL) return FALSE;

		//
		// 2. ������Ⱦ������
		//
		m_dwRenderMeshName = dwName;
		m_ptrDiffuseMaterial = pRenderMesh->GetMaterial();

		return TRUE;
	}

	//
	// ��������������
	//
	BOOL CEntityMesh::SetPhysicsSubMesh(const CHAR *szName)
	{
		ASSERT(szName);
		return SetPhysicsSubMesh(HashValue(szName));
	}

	//
	// ��������������
	//
	BOOL CEntityMesh::SetPhysicsSubMesh(DWORD dwName)
	{
		m_ptrPhysicsMaterial.SetNull();
		m_dwPhysicsMeshName = INVALID_HASHNAME;

		//
		// 1. ������ȫ���
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return FALSE;
		}

		const CSubMesh *pPhysicsMesh = m_ptrMesh->GetSubMesh(dwName);
		if (pPhysicsMesh == NULL) return FALSE;

		//
		// 2. ��������������
		//
		m_dwPhysicsMeshName = dwName;
		m_ptrPhysicsMaterial = pPhysicsMesh->GetMaterial();

		return TRUE;
	}

	//
	// �������
	//
	const CMeshPtr& CEntityMesh::GetMesh(VOID) const
	{
		return m_ptrMesh;
	}

	//
	// �����Ⱦ������
	//
	const CSubMesh* CEntityMesh::GetRenderSubMesh(VOID) const
	{
		return m_ptrMesh.IsNull() == FALSE && m_ptrMesh->IsValid() ? m_ptrMesh->GetSubMesh(m_dwRenderMeshName) : NULL;
	}

	//
	// �������������
	//
	const CSubMesh* CEntityMesh::GetPhysicsSubMesh(VOID) const
	{
		return m_ptrMesh.IsNull() == FALSE && m_ptrMesh->IsValid() ? m_ptrMesh->GetSubMesh(m_dwPhysicsMeshName) : NULL;
	}

	//
	// �������������
	//
	BOOL CEntityMesh::SetDiffuseMaterial(const CHAR *szName)
	{
		ASSERT(szName);
		return SetDiffuseMaterial(HashValue(szName));
	}

	//
	// �������������
	//
	BOOL CEntityMesh::SetDiffuseMaterial(DWORD dwName)
	{
		return SetDiffuseMaterial(MaterialManager()->QueryResource(dwName));
	}

	//
	// �������������
	//
	BOOL CEntityMesh::SetDiffuseMaterial(const CMaterialPtr &ptrMaterial)
	{
		m_ptrDiffuseMaterial.SetNull();

		//
		// 1. ������ȫ���
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return FALSE;
		}

		if (ptrMaterial.IsNull() || ptrMaterial->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. �������������
		//
		m_ptrDiffuseMaterial = ptrMaterial;

		return TRUE;
	}

	//
	// �����������
	//
	BOOL CEntityMesh::SetPhysicsMaterial(const CHAR *szName)
	{
		ASSERT(szName);
		return SetPhysicsMaterial(HashValue(szName));
	}

	//
	// �����������
	//
	BOOL CEntityMesh::SetPhysicsMaterial(DWORD dwName)
	{
		return SetPhysicsMaterial(MaterialManager()->QueryResource(dwName));
	}

	//
	// �����������
	//
	BOOL CEntityMesh::SetPhysicsMaterial(const CMaterialPtr &ptrMaterial)
	{
		m_ptrPhysicsMaterial.SetNull();

		//
		// 1. ������ȫ���
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return FALSE;
		}

		if (ptrMaterial.IsNull() || ptrMaterial->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. �����������
		//
		m_ptrPhysicsMaterial = ptrMaterial;

		return TRUE;
	}

	//
	// ���ù�����ͼ����
	//
	BOOL CEntityMesh::SetLightMapTexture(const CHAR *szName, FLOAT offsetu, FLOAT offsetv, FLOAT scaleu, FLOAT scalev)
	{
		ASSERT(szName);
		return SetLightMapTexture(HashValue(szName), offsetu, offsetv, scaleu, scalev);
	}

	//
	// ���ù�����ͼ����
	//
	BOOL CEntityMesh::SetLightMapTexture(DWORD dwName, FLOAT offsetu, FLOAT offsetv, FLOAT scaleu, FLOAT scalev)
	{
		return SetLightMapTexture(TextureManager()->QueryResource(dwName), offsetu, offsetv, scaleu, scalev);
	}

	//
	// ���ù�����ͼ����
	//
	BOOL CEntityMesh::SetLightMapTexture(const CTexturePtr &ptrTexture, FLOAT offsetu, FLOAT offsetv, FLOAT scaleu, FLOAT scalev)
	{
		m_ptrLightMapTexture.SetNull();
		MtxIdentity(&m_mtxTextures[1]);

		//
		// 1. ������ȫ���
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return FALSE;
		}

		if (ptrTexture.IsNull() || ptrTexture->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. ���ù�����ͼ����
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
	// ������������
	//
	const CMaterialPtr& CEntityMesh::GetMaterial(VOID) const
	{
		return m_ptrDiffuseMaterial;
	}

	//
	// ����������
	//
	const CMaterialPtr& CEntityMesh::GetPhysicsMaterial(VOID) const
	{
		return m_ptrPhysicsMaterial;
	}

	//
	// ��ù�����ͼ����
	//
	const CTexturePtr& CEntityMesh::GetLightMapTexture(VOID) const
	{
		return m_ptrLightMapTexture;
	}

	//
	// �����������
	//
	BOOL CEntityMesh::SetTextureMatrix(INT indexTexUnit, const MATRIX4 *matrix)
	{
		ASSERT(matrix);

		//
		// 1. ������ȫ���
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return FALSE;
		}

		//
		// 2. �����������
		//
		MtxCopy(&m_mtxTextures[indexTexUnit], matrix);

		return TRUE;
	}

	//
	// ����������
	//
	const MATRIX4* CEntityMesh::GetTextureMatrix(INT indexTexUnit) const
	{
		//
		// 1. ������ȫ���
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return NULL;
		}

		//
		// 2. ����������
		//
		return &m_mtxTextures[indexTexUnit];
	}

	//
	// ��ù�����ͼ�任����
	//
	const MATRIX4* CEntityMesh::GetLightMapTextureMatrix(VOID) const
	{
		return &m_mtxTextures[1];
	}

	//
	// ���������ѯ״̬
	//
	VOID CEntityMesh::SetEnablePhysicsQuery(BOOL bEnable)
	{
		m_bEnablePhysicsQuery = bEnable;
	}

	//
	// ��������ģ��״̬
	//
	VOID CEntityMesh::SetEnablePhysicsSimulation(BOOL bEnable)
	{
		m_bEnablePhysicsSimulation = bEnable;
	}

	//
	// ��������ģ����������
	//
	VOID CEntityMesh::SetPhysicsSimulationTypeIndex(INT index)
	{
		m_nPhysicsSimulationTypeIndex = index;
	}

	//
	// ��������ѯ״̬
	//
	BOOL CEntityMesh::IsEnablePhysicsQuery(VOID) const
	{
		return m_bEnablePhysicsQuery;
	}

	//
	// �������ģ��״̬
	//
	BOOL CEntityMesh::IsEnablePhysicsSimulation(VOID) const
	{
		return m_bEnablePhysicsSimulation;
	}

	//
	// �������ģ����������
	//
	INT CEntityMesh::GetPhysicsSimulationTypeIndex(VOID) const
	{
		return m_nPhysicsSimulationTypeIndex;
	}

	//
	// ��Ⱦ
	//
	VOID CEntityMesh::Render(const CMaterial *pMaterial) const
	{
		//
		// 1. ������ȫ���
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return;
		}

		//
		// 2. ��Ⱦ
		//
		if (const CSubMesh *pRenderMesh = m_ptrMesh->GetSubMesh(m_dwRenderMeshName)) {
			Renderer()->PushMatrix();
			Renderer()->PushTextureMatrix();
			{
				//
				// 2.1. ���ù�����ͼ����
				//
				if (m_ptrLightMapTexture.IsNull() || m_ptrLightMapTexture->IsValid() == FALSE) {
					Renderer()->SetLightMapTexture(0);
				}
				else {
					Renderer()->SetLightMapTexture(m_ptrLightMapTexture->GetTextureGL());
				}

				//
				// 2.2. ���ö�̬��Դ
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
				// 2.3. ���ñ任����
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
				// 2.4. �����������
				//
				for (INT indexTexUnit = 0; indexTexUnit < MAX_TEXUNITS; indexTexUnit++) {
					Renderer()->SetTextureMatrix(indexTexUnit, m_mtxTextures[indexTexUnit]);
				}

				//
				// 2.5. ��Ⱦ
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
	// ��Ⱦ�ڵ�
	//
	VOID CEntityMesh::RenderOcclude(VOID) const
	{
		//
		// 1. ������ȫ���
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return;
		}

		//
		// 2. ��Ⱦ
		//
		if (const CSubMesh *pRenderMesh = m_ptrMesh->GetSubMesh(m_dwRenderMeshName)) {
			Renderer()->PushMatrix();
			{
				//
				// 2.1. ���ñ任����
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
				// 2.2. ��Ⱦ
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

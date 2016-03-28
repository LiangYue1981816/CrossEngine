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

	CSceneManager::CSceneManager(VOID)
		: m_dwVisibleFlags(0xffffffff)

		, m_pRootNode(NULL)
		, m_pMainCameraNode(NULL)
		, m_pMainShadowNode(NULL)
		, m_pMainCameraEntity(NULL)
		, m_pMainShadowEntity(NULL)

		, m_pLightGrid(NULL)
		, m_pOctreeCulling(NULL)
		, m_pOcclusionCulling(NULL)
	{
		//
		// 1. �����������ڵ�
		//
		m_pRootNode = SAFE_NEW(MEMTYPE_HEAP) CSceneNode(HashValue("Root"), this); ASSERT(m_pRootNode);

		//
		// 2. ����������ڵ㼰ʵ��
		//
		m_pMainCameraEntity = SAFE_NEW(MEMTYPE_HEAP) CEntityCamera(HashValue("MainCamera"), this); ASSERT(m_pMainCameraEntity);
		m_pMainCameraNode = SAFE_NEW(MEMTYPE_HEAP) CSceneNode(HashValue("MainCamera"), this); ASSERT(m_pMainCameraNode);
		m_pMainCameraNode->AttachEntity(m_pMainCameraEntity);

		//
		// 3. ��������Ӱ�ڵ㼰ʵ��
		//
		m_pMainShadowEntity = SAFE_NEW(MEMTYPE_HEAP) CEntityShadow(HashValue("MainShadow"), this); ASSERT(m_pMainShadowEntity);
		m_pMainShadowNode = SAFE_NEW(MEMTYPE_HEAP) CSceneNode(HashValue("MainShadow"), this); ASSERT(m_pMainShadowNode);
		m_pMainShadowNode->AttachEntity(m_pMainShadowEntity);

		//
		// 4. ��������ģ��
		//
		m_pLightGrid = SAFE_NEW(MEMTYPE_HEAP) CLightGrid(this); ASSERT(m_pLightGrid);
		m_pOctreeCulling = SAFE_NEW(MEMTYPE_HEAP) COctreeCulling(this); ASSERT(m_pOctreeCulling);
		m_pOcclusionCulling = SAFE_NEW(MEMTYPE_HEAP) COcclusionCulling(this); ASSERT(m_pOcclusionCulling);

		//
		// 5. ��ʼ��
		//
		Init();
	}

	CSceneManager::~CSceneManager(VOID)
	{
		//
		// 1. �ͷ�
		//
		Free();

		//
		// 2. �ͷ�����ģ��
		//
		SAFE_DELETE(m_pLightGrid);
		SAFE_DELETE(m_pOctreeCulling);
		SAFE_DELETE(m_pOcclusionCulling);

		//
		// 3. �ͷ�������ڵ㼰ʵ��
		//
		SAFE_DELETE(m_pMainCameraEntity);
		SAFE_DELETE(m_pMainCameraNode);

		//
		// 4. �ͷ�����Ӱ�ڵ㼰ʵ��
		//
		SAFE_DELETE(m_pMainShadowEntity);
		SAFE_DELETE(m_pMainShadowNode);

		//
		// 5. �ͷŸ��ڵ�
		//
		SAFE_DELETE(m_pRootNode);
	}

	//
	// ��ʼ������������
	//
	VOID CSceneManager::Init(VOID)
	{
		//
		// 1. ��ʼ���ɼ��Ա�ʶ
		//
		m_dwVisibleFlags = 0xffffffff;

		//
		// 2. ��ʼ���ڵ㲢�ҽ������������Ӱ
		//
		m_pRootNode->AttachNode(m_pMainCameraNode);
		m_pRootNode->AttachNode(m_pMainShadowNode);
		m_pRootNode->Identity();
		m_pMainCameraNode->Identity();
		m_pMainShadowNode->Identity();

		//
		// 3. ��ʼ��������
		//
		m_stream.Init();
	}

	//
	// �ͷų���������
	//
	VOID CSceneManager::Free(VOID)
	{
		//
		// 1. �ͷ�������
		//
		m_stream.Free();

		//
		// 2. ��ո��ڵ�
		//
		m_pRootNode->DetachAllNode();
		m_pRootNode->DetachAllEntity();

		//
		// 3. �ͷŽڵ㼯��
		//
		FreeNodes();

		//
		// 4. �ͷ�ʵ�弯��
		//
		FreeEntities(m_meshs);
		FreeEntities(m_skins);
		FreeEntities(m_effects);
		FreeEntities(m_halos);
		FreeEntities(m_lights);

		//
		// 5. �ͷ�����ģ��
		//
		m_pLightGrid->Free();
		m_pOctreeCulling->Free();
		m_pOcclusionCulling->Free();
		m_physics.DestroyScene();

		//
		// 6. ��ʼ��
		//
		Init();
	}

	//
	// �ͷŽ�㼯��
	//
	VOID CSceneManager::FreeNodes(VOID)
	{
		for (SceneNodeMap::const_iterator itNode = m_nodes.begin(); itNode != m_nodes.end(); ++itNode) {
			const CSceneNode *pNode = itNode->second;
			ASSERT(pNode);

			SAFE_DELETE(pNode);
		}

		m_nodes.clear();
	}

	//
	// �ͷ�ʵ�弯��
	//
	VOID CSceneManager::FreeEntities(EntityMap &entities)
	{
		for (EntityMap::const_iterator itEntity = entities.begin(); itEntity != entities.end(); ++itEntity) {
			const CEntity *pEntity = itEntity->second;
			ASSERT(pEntity);

			SAFE_DELETE(pEntity);
		}

		entities.clear();
	}

	//
	// ���ļ�������Դ
	//
	BOOL CSceneManager::LoadFromFile(const CHAR *szFileName)
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
			WriteLogI("Load from file: %s\n", szFileName);

			if (m_stream.LoadFromFile(szFileName) == FALSE) throw "Load file failed.";
			if (LoadFromStream(&m_stream) == FALSE) throw "Load from stream failed.";

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CSceneManager::LoadFromFile: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ��ѹ����������Դ
	//
	BOOL CSceneManager::LoadFromZip(const CHAR *szZipName, const CHAR *szFileName)
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
			WriteLogI("Load from zip: %s<%s>\n", szZipName, szFileName);

			if (m_stream.LoadFromZip(szZipName, szFileName) == FALSE) throw "Load zip failed.";
			if (LoadFromStream(&m_stream) == FALSE) throw "Load from stream failed.";

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CSceneManager::LoadFromZip: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ��ѹ�����ļ�����������
	//
	BOOL CSceneManager::LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName)
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
			WriteLogI("Load from zip: <%s>\n", szFileName);

			if (m_stream.LoadFromZip(pZipPack, szFileName) == FALSE) throw "Load zip failed.";
			if (LoadFromStream(&m_stream) == FALSE) throw "Load from stream failed.";

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CSceneManager::LoadFromZip: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ��������������Դ
	//
	BOOL CSceneManager::LoadFromStream(CStream *pStream)
	{
		return TRUE;
	}

	//
	// ������Ⱦ����
	//
	BOOL CSceneManager::CreateRenderScene(VOID)
	{
		return TRUE;
	}

	//
	// ����������
	//
	BOOL CSceneManager::CreatePhysicsScene(VOID)
	{
		for (EntityMap::const_iterator itEntity = m_meshs.begin(); itEntity != m_meshs.end(); ++itEntity) {
			const CEntityMesh *pEntityMesh = (const CEntityMesh *)itEntity->second;
			ASSERT(pEntityMesh);

			const CSubMesh *pPhysicsMesh = pEntityMesh->GetPhysicsSubMesh();
			if (pPhysicsMesh == NULL) continue;
			if (pEntityMesh->IsEnablePhysicsQuery() == FALSE && pEntityMesh->IsEnablePhysicsSimulation() == FALSE) continue;

			const VEC3 *position = pEntityMesh->GetSceneNode()->GetWorldPosition();
			const QUAT *orientation = pEntityMesh->GetSceneNode()->GetWorldOrientation();
			const PxTransform pxLocalPose(PxVec3(0.0f, 0.0f, 0.0f));
			const PxTransform pxGlobalPose(PxVec3((*position)[0], (*position)[1], (*position)[2]), PxQuat((*orientation)[0], (*orientation)[1], (*orientation)[2], (*orientation)[3]));

			const PxMaterial &pxMaterial = GetPhysicsStandardMaterial((MATERIAL_TYPE)pEntityMesh->GetPhysicsMaterial()->GetPhysics()->GetType());
			const PxFilterData pxQueryFilterData = PxFilterData(QUERY_FLAGS_SCENE_MESH, 0, 0, 0);
			const PxFilterData pxSimulationFilterData = PxFilterData(SIMULATION_FLAGS_WORD0[pEntityMesh->GetPhysicsSimulationTypeIndex()], SIMULATION_FLAGS_WORD1[pEntityMesh->GetPhysicsSimulationTypeIndex()], 0, 0);

			PxRigidActor *pPxRigidActor = (PxRigidActor *)m_physics.CreateRigidActor(pEntityMesh->GetSceneNode()->GetName(), pxGlobalPose, FALSE, TRUE, TRUE);
			PxShape *pPxShape = PxCreateShapeMesh(pPxRigidActor, pPhysicsMesh->GetVertexBuffer(), pPhysicsMesh->GetVertexCount(), pPhysicsMesh->GetFaceBuffer(), pPhysicsMesh->GetFaceCount(), pxMaterial, pxLocalPose, pEntityMesh->IsEnablePhysicsQuery(), pEntityMesh->IsEnablePhysicsSimulation(), pxQueryFilterData, pxSimulationFilterData);
			pPxShape->userData = pEntityMesh->userData;
		}

		return TRUE;
	}

	//
	// ���ÿɼ��Ա�ʶ
	//
	VOID CSceneManager::SetVisibleFlags(DWORD dwFlags)
	{
		m_dwVisibleFlags = dwFlags;
	}

	//
	// ��ÿɼ��Ա�ʶ
	//
	DWORD CSceneManager::GetVisibleFlags(VOID) const
	{
		return m_dwVisibleFlags;
	}

	//
	// ��ø��ڵ�
	//
	CSceneNode* CSceneManager::GetRootNode(VOID) const
	{
		return m_pRootNode;
	}

	//
	// ���������ڵ�
	//
	CSceneNode* CSceneManager::GetMainCameraNode(VOID) const
	{
		return m_pMainCameraNode;
	}

	//
	// ��������ʵ��
	//
	CEntityCamera* CSceneManager::GetMainCameraEntity(VOID) const
	{
		return m_pMainCameraEntity;
	}

	//
	// �������Ӱ�ڵ�
	//
	CSceneNode* CSceneManager::GetMainShadowNode(VOID) const
	{
		return m_pMainShadowNode;
	}

	//
	// �������Ӱʵ��
	//
	CEntityShadow* CSceneManager::GetMainShadowEntity(VOID) const
	{
		return m_pMainShadowEntity;
	}

	//
	// �����������
	//
	CPhysicsManager& CSceneManager::GetPhysics(VOID)
	{
		return m_physics;
	}

	//
	// �����ڵ�
	//
	CSceneNode* CSceneManager::CreateNode(const CHAR *szName)
	{
		ASSERT(szName);
		return CreateNode(HashValue(szName));
	}

	//
	// �����ڵ�
	//
	CSceneNode* CSceneManager::CreateNode(DWORD dwName)
	{
		CSceneNode *pNode = NULL;

		if (m_nodes.find(dwName) == m_nodes.end()) {
			m_nodes[dwName] = pNode = SAFE_NEW(MEMTYPE_HEAP) CSceneNode(dwName, this); ASSERT(pNode);
		}
		else {
			pNode = m_nodes[dwName];
		}

		return pNode;
	}

	//
	// ���ٽڵ�
	//
	VOID CSceneManager::DestroyNode(CSceneNode *pNode)
	{
		//
		// 1. ������ȫ���
		//
		if (pNode == NULL) {
			return;
		}

		//
		// 2. �ӽڵ㼯�����Ƴ�
		//
		SceneNodeMap::const_iterator itNode = m_nodes.find(pNode->GetName());
		if (itNode != m_nodes.end()) m_nodes.erase(itNode);

		//
		// 3. ���ٽڵ�
		//
		SAFE_DELETE(pNode);
	}

	//
	// ��ýڵ�
	//
	CSceneNode* CSceneManager::GetNode(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetNode(HashValue(szName));
	}

	//
	// ��ýڵ�
	//
	CSceneNode* CSceneManager::GetNode(DWORD dwName) const
	{
		SceneNodeMap::const_iterator itNode = m_nodes.find(dwName);
		return itNode != m_nodes.end() ? itNode->second : NULL;
	}

	//
	// ����ʵ��
	//
	CEntity* CSceneManager::CreateEntity(const CHAR *szName, CEntity::TYPE type)
	{
		ASSERT(szName);
		return CreateEntity(HashValue(szName), type);
	}

	//
	// ����ʵ��
	//
	CEntity* CSceneManager::CreateEntity(DWORD dwName, CEntity::TYPE type)
	{
		CEntity *pEntity = NULL;

		switch (type) {
		case CEntity::MESH:
			if (m_meshs.find(dwName) == m_meshs.end()) {
				m_meshs[dwName] = pEntity = SAFE_NEW(MEMTYPE_HEAP) CEntityMesh(dwName, this); ASSERT(pEntity);
			}
			else {
				pEntity = m_meshs[dwName];
			}

			break;
		case CEntity::SKIN:
			if (m_skins.find(dwName) == m_skins.end()) {
				m_skins[dwName] = pEntity = SAFE_NEW(MEMTYPE_HEAP) CEntitySkin(dwName, this); ASSERT(pEntity);
			}
			else {
				pEntity = m_skins[dwName];
			}

			break;
		case CEntity::EFFECT:
			if (m_effects.find(dwName) == m_effects.end()) {
				m_effects[dwName] = pEntity = SAFE_NEW(MEMTYPE_HEAP) CEntityEffect(dwName, this); ASSERT(pEntity);
			}
			else {
				pEntity = m_effects[dwName];
			}

			break;
		case CEntity::HALO:
			if (m_halos.find(dwName) == m_halos.end()) {
				m_halos[dwName] = pEntity = SAFE_NEW(MEMTYPE_HEAP) CEntityHalo(dwName, this); ASSERT(pEntity);
			}
			else {
				pEntity = m_halos[dwName];
			}

			break;
		case CEntity::LIGHT:
			if (m_lights.find(dwName) == m_lights.end()) {
				m_lights[dwName] = pEntity = SAFE_NEW(MEMTYPE_HEAP) CEntityLight(dwName, this); ASSERT(pEntity);
			}
			else {
				pEntity = m_lights[dwName];
			}

			break;
		}

		return pEntity;
	}

	//
	// ����ʵ��
	//
	VOID CSceneManager::DestroyEntity(CEntity *pEntity)
	{
		//
		// 1. ������ȫ���
		//
		if (pEntity == NULL) {
			return;
		}

		//
		// 2. ����ʵ��
		//
		EntityMap::const_iterator itEntity;

		switch (pEntity->GetType()) {
		case CEntity::MESH:
			itEntity = m_meshs.find(pEntity->GetName());
			if (itEntity != m_meshs.end()) m_meshs.erase(itEntity);

			break;
		case CEntity::SKIN:
			itEntity = m_skins.find(pEntity->GetName());
			if (itEntity != m_skins.end()) m_skins.erase(itEntity);

			break;
		case CEntity::EFFECT:
			itEntity = m_effects.find(pEntity->GetName());
			if (itEntity != m_effects.end()) m_effects.erase(itEntity);

			break;
		case CEntity::HALO:
			itEntity = m_halos.find(pEntity->GetName());
			if (itEntity != m_halos.end()) m_halos.erase(itEntity);

			break;
		case CEntity::LIGHT:
			itEntity = m_lights.find(pEntity->GetName());
			if (itEntity != m_lights.end()) m_lights.erase(itEntity);

			break;
		}

		SAFE_DELETE(pEntity);
	}

	//
	// ���ʵ��
	//
	CEntity* CSceneManager::GetEntity(const CHAR *szName, CEntity::TYPE type) const
	{
		ASSERT(szName);
		return GetEntity(HashValue(szName), type);
	}

	//
	// ���ʵ��
	//
	CEntity* CSceneManager::GetEntity(DWORD dwName, CEntity::TYPE type) const
	{
		CEntity *pEntity = NULL;
		EntityMap::const_iterator itEntity;

		switch (type) {
		case CEntity::MESH:
			itEntity = m_meshs.find(pEntity->GetName());
			pEntity = m_meshs.end() != itEntity ? itEntity->second : NULL;

			break;
		case CEntity::SKIN:
			itEntity = m_skins.find(pEntity->GetName());
			pEntity = m_skins.end() != itEntity ? itEntity->second : NULL;

			break;
		case CEntity::EFFECT:
			itEntity = m_effects.find(pEntity->GetName());
			pEntity = m_effects.end() != itEntity ? itEntity->second : NULL;

			break;
		case CEntity::HALO:
			itEntity = m_halos.find(pEntity->GetName());
			pEntity = m_halos.end() != itEntity ? itEntity->second : NULL;

			break;
		case CEntity::LIGHT:
			itEntity = m_lights.find(pEntity->GetName());
			pEntity = m_lights.end() != itEntity ? itEntity->second : NULL;

			break;
		}

		return pEntity;
	}

	//
	// �ڵ��ж�
	//
	BOOL CSceneManager::IsOccluded(INT index) const
	{
		ASSERT(m_pOcclusionCulling);
		return m_pOcclusionCulling->IsOccluded(index);
	}

	//
	// ��ù��ղ���
	//
	BOOL CSceneManager::GetLightParams(DWORD dwChannel, const VEC3 *position, VEC4 *direction, VEC3 *ambient, VEC3 *diffuse, VEC3 *specular, VEC3 *rim, VEC3 *skyLower, VEC3 *skyUpper, VEC3 *indirectUp, VEC3 *indirectDown, VEC3 *indirectLeft, VEC3 *indirectRight, VEC3 *indirectFront, VEC3 *indirectBack) const
	{
		ASSERT(m_pLightGrid);
		return m_pLightGrid->GetLightParams(dwChannel, position, direction, ambient, diffuse, specular, rim, skyLower, skyUpper, indirectUp, indirectDown, indirectLeft, indirectRight, indirectFront, indirectBack);
	}

	//
	// ��ӳ�����Ⱦ����
	//
	VOID CSceneManager::AddSceneRenderQuene(const CEntityCamera *pCamera, CEntityRenderable *pEntity)
	{
		ASSERT(pCamera);
		ASSERT(pEntity);

		AddSceneRenderQuene(pCamera, pEntity, pEntity->GetMaterial().GetPointer());
	}

	//
	// ��ӳ�����Ⱦ����
	//
	VOID CSceneManager::AddSceneRenderQuene(const CEntityCamera *pCamera, CEntityRenderable *pEntity, CMaterial *pMaterial)
	{
		ASSERT(pCamera);
		ASSERT(pEntity);
		ASSERT(pMaterial);

		//
		// 1. ��ӳ�����Ⱦ����
		//
		RenderQuene()->AddSceneRenderQuene(pCamera, pMaterial, pEntity);

		//
		// 2. ��ӳ����ڵ���Ⱦ����
		//
		if (pEntity->IsEnableOcclude()) {
			RenderQuene()->AddSceneOccludeRenderQuene(pCamera, pEntity);
		}

		//
		// 2. �����Ӱ��Ⱦ����
		//
		BOOL bIsShadowMapCastShadow = pEntity->IsShadowMapCastShadow(pCamera);
		BOOL bIsShadowMapReceiveShadow = pEntity->IsEnableShadowMapReceiveShadow();

		if (bIsShadowMapCastShadow || bIsShadowMapReceiveShadow) {
			if (m_pMainShadowEntity->IsVisible(pEntity->GetAABB())) {
				//
				// 2.1. ��ӵ�������Ӱ��Ⱦ����
				//
				if (bIsShadowMapCastShadow) {
					RenderQuene()->AddCastShadowRenderQuene(m_pMainShadowEntity, pMaterial, pEntity);
				}

				//
				// 2.2. ��ӽ�����Ӱ��Ⱦ����
				//
				if (bIsShadowMapReceiveShadow) {
					RenderQuene()->AddReceiveShadowRenderQuene(m_pMainShadowEntity, pMaterial, pEntity);
				}
			}
		}
	}

	//
	// ��ӹ�Դ��Ⱦ����
	//
	VOID CSceneManager::AddLightRenderQuene(const CEntityCamera *pCamera, CEntityLight *pEntity)
	{
		ASSERT(pCamera);
		ASSERT(pEntity);

		RenderQuene()->AddSceneLightRenderQuene(pCamera, pEntity);
	}

	//
	// ����
	//
	VOID CSceneManager::Update(FLOAT deltaTime)
	{
		m_pRootNode->UpdateTransform();
		m_pRootNode->UpdateAABB();
		m_pRootNode->UpdateEntity(deltaTime);
	}

	//
	// ��Ⱦ
	//
	VOID CSceneManager::Render(CEntityCamera *pCamera)
	{
		//
		// 1. �������
		//
		pCamera = pCamera ? pCamera : m_pMainCameraEntity;
		pCamera->Update(0.0f);

		//
		// 2. ������Ⱦ����
		//
		m_pRootNode->SetRenderable(FALSE, TRUE);
		m_pOctreeCulling->UpdateCulling(pCamera);
		m_pOcclusionCulling->UpdateCulling(pCamera);
		m_pRootNode->UpdateRenderQuene(pCamera);
	}

}

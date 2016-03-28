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
		// 1. 创建场景根节点
		//
		m_pRootNode = SAFE_NEW(MEMTYPE_HEAP) CSceneNode(HashValue("Root"), this); ASSERT(m_pRootNode);

		//
		// 2. 创建主相机节点及实体
		//
		m_pMainCameraEntity = SAFE_NEW(MEMTYPE_HEAP) CEntityCamera(HashValue("MainCamera"), this); ASSERT(m_pMainCameraEntity);
		m_pMainCameraNode = SAFE_NEW(MEMTYPE_HEAP) CSceneNode(HashValue("MainCamera"), this); ASSERT(m_pMainCameraNode);
		m_pMainCameraNode->AttachEntity(m_pMainCameraEntity);

		//
		// 3. 创建主阴影节点及实体
		//
		m_pMainShadowEntity = SAFE_NEW(MEMTYPE_HEAP) CEntityShadow(HashValue("MainShadow"), this); ASSERT(m_pMainShadowEntity);
		m_pMainShadowNode = SAFE_NEW(MEMTYPE_HEAP) CSceneNode(HashValue("MainShadow"), this); ASSERT(m_pMainShadowNode);
		m_pMainShadowNode->AttachEntity(m_pMainShadowEntity);

		//
		// 4. 创建其他模块
		//
		m_pLightGrid = SAFE_NEW(MEMTYPE_HEAP) CLightGrid(this); ASSERT(m_pLightGrid);
		m_pOctreeCulling = SAFE_NEW(MEMTYPE_HEAP) COctreeCulling(this); ASSERT(m_pOctreeCulling);
		m_pOcclusionCulling = SAFE_NEW(MEMTYPE_HEAP) COcclusionCulling(this); ASSERT(m_pOcclusionCulling);

		//
		// 5. 初始化
		//
		Init();
	}

	CSceneManager::~CSceneManager(VOID)
	{
		//
		// 1. 释放
		//
		Free();

		//
		// 2. 释放其他模块
		//
		SAFE_DELETE(m_pLightGrid);
		SAFE_DELETE(m_pOctreeCulling);
		SAFE_DELETE(m_pOcclusionCulling);

		//
		// 3. 释放主相机节点及实体
		//
		SAFE_DELETE(m_pMainCameraEntity);
		SAFE_DELETE(m_pMainCameraNode);

		//
		// 4. 释放主阴影节点及实体
		//
		SAFE_DELETE(m_pMainShadowEntity);
		SAFE_DELETE(m_pMainShadowNode);

		//
		// 5. 释放根节点
		//
		SAFE_DELETE(m_pRootNode);
	}

	//
	// 初始化场景管理器
	//
	VOID CSceneManager::Init(VOID)
	{
		//
		// 1. 初始化可见性标识
		//
		m_dwVisibleFlags = 0xffffffff;

		//
		// 2. 初始化节点并挂接主相机和主阴影
		//
		m_pRootNode->AttachNode(m_pMainCameraNode);
		m_pRootNode->AttachNode(m_pMainShadowNode);
		m_pRootNode->Identity();
		m_pMainCameraNode->Identity();
		m_pMainShadowNode->Identity();

		//
		// 3. 初始化数据流
		//
		m_stream.Init();
	}

	//
	// 释放场景管理器
	//
	VOID CSceneManager::Free(VOID)
	{
		//
		// 1. 释放数据流
		//
		m_stream.Free();

		//
		// 2. 清空根节点
		//
		m_pRootNode->DetachAllNode();
		m_pRootNode->DetachAllEntity();

		//
		// 3. 释放节点集合
		//
		FreeNodes();

		//
		// 4. 释放实体集合
		//
		FreeEntities(m_meshs);
		FreeEntities(m_skins);
		FreeEntities(m_effects);
		FreeEntities(m_halos);
		FreeEntities(m_lights);

		//
		// 5. 释放其他模块
		//
		m_pLightGrid->Free();
		m_pOctreeCulling->Free();
		m_pOcclusionCulling->Free();
		m_physics.DestroyScene();

		//
		// 6. 初始化
		//
		Init();
	}

	//
	// 释放结点集合
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
	// 释放实体集合
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
	// 从文件加载资源
	//
	BOOL CSceneManager::LoadFromFile(const CHAR *szFileName)
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
	// 从压缩包加载资源
	//
	BOOL CSceneManager::LoadFromZip(const CHAR *szZipName, const CHAR *szFileName)
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
	// 从压缩包文件加载数据流
	//
	BOOL CSceneManager::LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName)
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
	// 从数据流加载资源
	//
	BOOL CSceneManager::LoadFromStream(CStream *pStream)
	{
		return TRUE;
	}

	//
	// 创建渲染场景
	//
	BOOL CSceneManager::CreateRenderScene(VOID)
	{
		return TRUE;
	}

	//
	// 创建物理场景
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
	// 设置可见性标识
	//
	VOID CSceneManager::SetVisibleFlags(DWORD dwFlags)
	{
		m_dwVisibleFlags = dwFlags;
	}

	//
	// 获得可见性标识
	//
	DWORD CSceneManager::GetVisibleFlags(VOID) const
	{
		return m_dwVisibleFlags;
	}

	//
	// 获得根节点
	//
	CSceneNode* CSceneManager::GetRootNode(VOID) const
	{
		return m_pRootNode;
	}

	//
	// 获得主相机节点
	//
	CSceneNode* CSceneManager::GetMainCameraNode(VOID) const
	{
		return m_pMainCameraNode;
	}

	//
	// 获得主相机实体
	//
	CEntityCamera* CSceneManager::GetMainCameraEntity(VOID) const
	{
		return m_pMainCameraEntity;
	}

	//
	// 获得主阴影节点
	//
	CSceneNode* CSceneManager::GetMainShadowNode(VOID) const
	{
		return m_pMainShadowNode;
	}

	//
	// 获得主阴影实体
	//
	CEntityShadow* CSceneManager::GetMainShadowEntity(VOID) const
	{
		return m_pMainShadowEntity;
	}

	//
	// 获得物理引擎
	//
	CPhysicsManager& CSceneManager::GetPhysics(VOID)
	{
		return m_physics;
	}

	//
	// 创建节点
	//
	CSceneNode* CSceneManager::CreateNode(const CHAR *szName)
	{
		ASSERT(szName);
		return CreateNode(HashValue(szName));
	}

	//
	// 创建节点
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
	// 销毁节点
	//
	VOID CSceneManager::DestroyNode(CSceneNode *pNode)
	{
		//
		// 1. 参数安全检查
		//
		if (pNode == NULL) {
			return;
		}

		//
		// 2. 从节点集合中移除
		//
		SceneNodeMap::const_iterator itNode = m_nodes.find(pNode->GetName());
		if (itNode != m_nodes.end()) m_nodes.erase(itNode);

		//
		// 3. 销毁节点
		//
		SAFE_DELETE(pNode);
	}

	//
	// 获得节点
	//
	CSceneNode* CSceneManager::GetNode(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetNode(HashValue(szName));
	}

	//
	// 获得节点
	//
	CSceneNode* CSceneManager::GetNode(DWORD dwName) const
	{
		SceneNodeMap::const_iterator itNode = m_nodes.find(dwName);
		return itNode != m_nodes.end() ? itNode->second : NULL;
	}

	//
	// 创建实体
	//
	CEntity* CSceneManager::CreateEntity(const CHAR *szName, CEntity::TYPE type)
	{
		ASSERT(szName);
		return CreateEntity(HashValue(szName), type);
	}

	//
	// 创建实体
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
	// 销毁实体
	//
	VOID CSceneManager::DestroyEntity(CEntity *pEntity)
	{
		//
		// 1. 参数安全检查
		//
		if (pEntity == NULL) {
			return;
		}

		//
		// 2. 销毁实体
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
	// 获得实体
	//
	CEntity* CSceneManager::GetEntity(const CHAR *szName, CEntity::TYPE type) const
	{
		ASSERT(szName);
		return GetEntity(HashValue(szName), type);
	}

	//
	// 获得实体
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
	// 遮挡判断
	//
	BOOL CSceneManager::IsOccluded(INT index) const
	{
		ASSERT(m_pOcclusionCulling);
		return m_pOcclusionCulling->IsOccluded(index);
	}

	//
	// 获得光照参数
	//
	BOOL CSceneManager::GetLightParams(DWORD dwChannel, const VEC3 *position, VEC4 *direction, VEC3 *ambient, VEC3 *diffuse, VEC3 *specular, VEC3 *rim, VEC3 *skyLower, VEC3 *skyUpper, VEC3 *indirectUp, VEC3 *indirectDown, VEC3 *indirectLeft, VEC3 *indirectRight, VEC3 *indirectFront, VEC3 *indirectBack) const
	{
		ASSERT(m_pLightGrid);
		return m_pLightGrid->GetLightParams(dwChannel, position, direction, ambient, diffuse, specular, rim, skyLower, skyUpper, indirectUp, indirectDown, indirectLeft, indirectRight, indirectFront, indirectBack);
	}

	//
	// 添加场景渲染队列
	//
	VOID CSceneManager::AddSceneRenderQuene(const CEntityCamera *pCamera, CEntityRenderable *pEntity)
	{
		ASSERT(pCamera);
		ASSERT(pEntity);

		AddSceneRenderQuene(pCamera, pEntity, pEntity->GetMaterial().GetPointer());
	}

	//
	// 添加场景渲染队列
	//
	VOID CSceneManager::AddSceneRenderQuene(const CEntityCamera *pCamera, CEntityRenderable *pEntity, CMaterial *pMaterial)
	{
		ASSERT(pCamera);
		ASSERT(pEntity);
		ASSERT(pMaterial);

		//
		// 1. 添加场景渲染队列
		//
		RenderQuene()->AddSceneRenderQuene(pCamera, pMaterial, pEntity);

		//
		// 2. 添加场景遮挡渲染队列
		//
		if (pEntity->IsEnableOcclude()) {
			RenderQuene()->AddSceneOccludeRenderQuene(pCamera, pEntity);
		}

		//
		// 2. 添加阴影渲染队列
		//
		BOOL bIsShadowMapCastShadow = pEntity->IsShadowMapCastShadow(pCamera);
		BOOL bIsShadowMapReceiveShadow = pEntity->IsEnableShadowMapReceiveShadow();

		if (bIsShadowMapCastShadow || bIsShadowMapReceiveShadow) {
			if (m_pMainShadowEntity->IsVisible(pEntity->GetAABB())) {
				//
				// 2.1. 添加到产生阴影渲染队列
				//
				if (bIsShadowMapCastShadow) {
					RenderQuene()->AddCastShadowRenderQuene(m_pMainShadowEntity, pMaterial, pEntity);
				}

				//
				// 2.2. 添加接收阴影渲染队列
				//
				if (bIsShadowMapReceiveShadow) {
					RenderQuene()->AddReceiveShadowRenderQuene(m_pMainShadowEntity, pMaterial, pEntity);
				}
			}
		}
	}

	//
	// 添加光源渲染队列
	//
	VOID CSceneManager::AddLightRenderQuene(const CEntityCamera *pCamera, CEntityLight *pEntity)
	{
		ASSERT(pCamera);
		ASSERT(pEntity);

		RenderQuene()->AddSceneLightRenderQuene(pCamera, pEntity);
	}

	//
	// 更新
	//
	VOID CSceneManager::Update(FLOAT deltaTime)
	{
		m_pRootNode->UpdateTransform();
		m_pRootNode->UpdateAABB();
		m_pRootNode->UpdateEntity(deltaTime);
	}

	//
	// 渲染
	//
	VOID CSceneManager::Render(CEntityCamera *pCamera)
	{
		//
		// 1. 更新相机
		//
		pCamera = pCamera ? pCamera : m_pMainCameraEntity;
		pCamera->Update(0.0f);

		//
		// 2. 更新渲染队列
		//
		m_pRootNode->SetRenderable(FALSE, TRUE);
		m_pOctreeCulling->UpdateCulling(pCamera);
		m_pOcclusionCulling->UpdateCulling(pCamera);
		m_pRootNode->UpdateRenderQuene(pCamera);
	}

}

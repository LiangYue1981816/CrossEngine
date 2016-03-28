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

	static PxFilterFlags PxCCDSimulationFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
	{
		//
		// 1. Let triggers through
		//
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1)) {
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlags();
		}

		//
		// 2. Use a group-based mechanism for all other pairs:
		// - Objects within the default group (mask 0) always collide
		// - By default, objects of the default group do not collide
		//   with any other group. If they should collide with another
		//   group then this can only be specified through the filter
		//   data of the default group objects (objects of a different
		//   group can not choose to do so)
		// - For objects that are not in the default group, a bitmask
		//   is used to define the groups they should collide with
		//
		if ((filterData0.word0 != 0 || filterData1.word0 != 0) && !(filterData0.word0&filterData1.word1 || filterData1.word0&filterData0.word1)) {
			return PxFilterFlag::eSUPPRESS;
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT;

		//
		// 3. Enable CCD
		//
		if (!(filterData0.word2&SIMULATION_FLAGS_DISABLE_PAIR_CCD) || !(filterData1.word2&SIMULATION_FLAGS_DISABLE_PAIR_CCD)) {
			pairFlags |= PxPairFlag::eRESOLVE_CONTACTS | PxPairFlag::eCCD_LINEAR;
		}

		//
		// 4. Trigger the contact callback for pairs (A,B) where 
		//    the filter mask of A contains the ID of B and vice versa.
		//
		if ((filterData0.word0&filterData1.word1) && (filterData1.word0&filterData0.word1)) {
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
		}

		//
		// 5. The pairFlags for each object are stored in word3 of the filter data. Combine them.
		//
		pairFlags |= PxPairFlags(PxU16(filterData0.word3 | filterData1.word3));
		return PxFilterFlags();
	}

	CPhysicsManager::CPhysicsManager(VOID)
		: m_pPxScene(NULL)
		, m_pPxControllerManager(NULL)
		, m_pPxCpuDispatcher(NULL)
	{
		CreateScene();
	}

	CPhysicsManager::~CPhysicsManager(VOID)
	{
		DestroyScene();
	}

	//
	// 创建场景
	//
	VOID CPhysicsManager::CreateScene(VOID)
	{
		//
		// 1. 创建物理场景
		//
		PxSceneDesc sceneDesc(GetPhysics().getTolerancesScale());

		sceneDesc.gravity = PxVec3(0.0f, -GRAVITY, 0.0f);
		sceneDesc.cpuDispatcher = m_pPxCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		sceneDesc.filterShader = PxCCDSimulationFilterShader;
		sceneDesc.flags = PxSceneFlag::eENABLE_CCD | PxSceneFlag::eREQUIRE_RW_LOCK;

		m_pPxScene = GetPhysics().createScene(sceneDesc);
		ASSERT(m_pPxScene);

		//
		// 2. 创建控制器管理器
		//
		m_pPxControllerManager = PxCreateControllerManager(*m_pPxScene);
		ASSERT(m_pPxControllerManager);
	}

	//
	// 销毁场景
	//
	VOID CPhysicsManager::DestroyScene(VOID)
	{
		ASSERT(m_pPxScene);
		ASSERT(m_pPxCpuDispatcher);
		ASSERT(m_pPxControllerManager);

		//
		// 1. 释放刚体集合
		//
		for (PxActorMap::const_iterator itPxActor = m_actors.begin(); itPxActor != m_actors.end(); ++itPxActor) {
			PxActor *pPxActor = itPxActor->second;
			ASSERT(pPxActor);

			m_pPxScene->removeActor(*pPxActor);
			pPxActor->release();
		}

		m_actors.clear();

		//
		// 2. 释放物理场景
		//
		m_pPxControllerManager->release();
		m_pPxScene->release();
		m_pPxCpuDispatcher->release();

		m_pPxScene = NULL;
		m_pPxControllerManager = NULL;
		m_pPxCpuDispatcher = NULL;
	}

	//
	// 获得场景
	//
	PxScene* CPhysicsManager::GetScene(VOID) const
	{
		ASSERT(m_pPxScene);
		return m_pPxScene;
	}

	//
	// 获得控制器管理器
	//
	PxControllerManager* CPhysicsManager::GetControllerManager(VOID) const
	{
		ASSERT(m_pPxControllerManager);
		return m_pPxControllerManager;
	}

	//
	// 创建刚体角色
	//
	PxActor* CPhysicsManager::CreateRigidActor(const CHAR *szName, const PxTransform &pxGlobalPose, BOOL bDynamic, BOOL bKinematic, BOOL bEnableCCD)
	{
		ASSERT(szName);

		PxActor *pPxActor = CreateRigidActor(HashValue(szName), pxGlobalPose, bDynamic, bKinematic, bEnableCCD);
		pPxActor->setName(szName);

		return pPxActor;
	}

	PxActor* CPhysicsManager::CreateRigidActor(DWORD dwName, const PxTransform &pxGlobalPose, BOOL bDynamic, BOOL bKinematic, BOOL bEnableCCD)
	{
		ASSERT(m_pPxScene);

		//
		// 1. 销毁同名角色
		//
		DestroyActor(dwName);

		//
		// 2. 创建刚体角色
		//
		PxActor *pPxActor = PxCreateRigidActor(pxGlobalPose, bDynamic, bKinematic, bEnableCCD);
		ASSERT(pPxActor);

		pPxActor->userData = NULL;
		m_actors[dwName] = pPxActor;

		return pPxActor;
	}

	//
	// 销毁角色
	//
	VOID CPhysicsManager::DestroyActor(DWORD dwName)
	{
		ASSERT(m_pPxScene);

		//
		// 1. 查找角色
		//
		PxActorMap::const_iterator itPxActor = m_actors.find(dwName);
		if (itPxActor == m_actors.end()) return;

		//
		// 2. 销毁角色
		//
		PxActor *pPxActor = itPxActor->second;
		ASSERT(pPxActor);

		m_pPxScene->removeActor(*pPxActor);
		pPxActor->release();

		m_actors.erase(itPxActor);
	}

	//
	// 获得角色
	//
	PxActor* CPhysicsManager::GetActor(DWORD dwName)
	{
		PxActorMap::const_iterator itPxActor = m_actors.find(dwName);
		return itPxActor != m_actors.end() ? itPxActor->second : NULL;
	}

	//
	// 设置查询状态
	//
	VOID CPhysicsManager::EnableQuery(DWORD dwName, BOOL bEnable)
	{
		//
		// 1. 查找刚体角色
		//
		PxActorMap::const_iterator itPxActor = m_actors.find(dwName);
		if (itPxActor == m_actors.end()) return;
		if (itPxActor->second == NULL) return;
		if (itPxActor->second->isRigidActor() == false) return;

		//
		// 2. 对所有形状设置查询状态
		//
		PxRigidActor *pPxRigidActor = (PxRigidActor *)itPxActor->second;

		if (PxU32 numShapes = pPxRigidActor->getNbShapes()) {
			PxShape **ppPxShapes = SAFE_NEW(MEMTYPE_STACK) PxShape*[numShapes];
			{
				pPxRigidActor->getShapes(ppPxShapes, numShapes);

				for (PxU32 indexShape = 0; indexShape < numShapes; indexShape++) {
					ppPxShapes[indexShape]->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, bEnable ? true : false);
				}
			}
			SAFE_DELETE_ARRAY(ppPxShapes);
		}
	}

	//
	// 设置模拟状态
	//
	VOID CPhysicsManager::EnableSimulation(DWORD dwName, BOOL bEnable)
	{
		//
		// 1. 查找刚体角色
		//
		PxActorMap::const_iterator itPxActor = m_actors.find(dwName);
		if (itPxActor == m_actors.end()) return;
		if (itPxActor->second == NULL) return;
		if (itPxActor->second->isRigidActor() == false) return;

		//
		// 2. 对所有形状设置模拟状态
		//
		PxRigidActor *pPxRigidActor = (PxRigidActor *)itPxActor->second;

		if (PxU32 numShapes = pPxRigidActor->getNbShapes()) {
			PxShape **ppPxShapes = SAFE_NEW(MEMTYPE_STACK) PxShape*[numShapes];
			{
				pPxRigidActor->getShapes(ppPxShapes, numShapes);

				for (PxU32 indexShape = 0; indexShape < numShapes; indexShape++) {
					ppPxShapes[indexShape]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, bEnable ? true : false);
				}
			}
			SAFE_DELETE_ARRAY(ppPxShapes);
		}
	}

	//
	// 设置运动状态
	//
	VOID CPhysicsManager::EnableKinematic(DWORD dwName, BOOL bEnable)
	{
		//
		// 1. 查找刚体角色
		//
		PxActorMap::const_iterator itPxActor = m_actors.find(dwName);
		if (itPxActor == m_actors.end()) return;
		if (itPxActor->second == NULL) return;
		if (itPxActor->second->isRigidActor() == false) return;

		//
		// 2. 设置运动状态
		//
		PxRigidDynamic *pPxRigidDynamic = (PxRigidDynamic *)itPxActor->second;
		pPxRigidDynamic->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, bEnable ? true : false);
	}

	//
	// 设置CCD状态
	//
	VOID CPhysicsManager::EnableCCD(DWORD dwName, BOOL bEnable)
	{
		//
		// 1. 查找刚体角色
		//
		PxActorMap::const_iterator itPxActor = m_actors.find(dwName);
		if (itPxActor == m_actors.end()) return;
		if (itPxActor->second == NULL) return;
		if (itPxActor->second->isRigidDynamic() == false) return;

		//
		// 2. 设置CCD状态
		//
		PxRigidDynamic *pPxRigidDynamic = (PxRigidDynamic *)itPxActor->second;
		pPxRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, bEnable ? true : false);
	}

	//
	// 射线检测场景
	//
	DWORD CPhysicsManager::RayCastScene(DWORD dwQueryFlags, const RAY *pRay, FLOAT *length, VEC3 *position, VEC3 *normal, VOID **ppActorData, VOID **ppShapeData) const
	{
		ASSERT(pRay);
		ASSERT(m_pPxScene);

		DWORD dwHitData = 0x00000000;

		PxReal maxDistance = length ? *length : FLT_MAX;
		PxVec3 origin(pRay->origin.x, pRay->origin.y, pRay->origin.z);
		PxVec3 direction(pRay->direction.x, pRay->direction.y, pRay->direction.z);

		PxRaycastHit pxHit;
		const PxSceneQueryFlags pxOutputFlags = PxSceneQueryFlag::eDISTANCE | PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL;
		const PxSceneQueryFilterData pxFilterData(PxFilterData(dwQueryFlags, 0, 0, 0), PxSceneQueryFilterFlag::eSTATIC | PxSceneQueryFilterFlag::eDYNAMIC);

		if (m_pPxScene->raycastSingle(origin, direction, maxDistance, pxOutputFlags, pxHit, pxFilterData)) {
			if (pxHit.shape) {
				dwHitData = pxHit.shape->getQueryFilterData().word0;

				if (length) *length = pxHit.distance;
				if (position) Vec3Set(position, pxHit.position.x, pxHit.position.y, pxHit.position.z);
				if (normal) Vec3Set(normal, pxHit.normal.x, pxHit.normal.y, pxHit.normal.z);
				if (ppActorData) *ppActorData = pxHit.shape->getActor()->userData;
				if (ppShapeData) *ppShapeData = pxHit.shape->userData;
			}
		}

		return dwHitData;
	}

	//
	// 物理开始
	//
	VOID CPhysicsManager::Begin(VOID)
	{
		ASSERT(m_pPxScene);

		m_pPxScene->fetchResults(true);
		PX_SCENE_LOCK_RW_BEGIN(m_pPxScene);
	}

	//
	// 物理结束
	//
	VOID CPhysicsManager::End(PxReal deltaTime)
	{
		ASSERT(m_pPxScene);

		PX_SCENE_LOCK_RW_END(m_pPxScene);
		m_pPxScene->simulate(deltaTime);
	}

}

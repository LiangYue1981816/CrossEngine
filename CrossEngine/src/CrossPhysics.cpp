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

	class PxAllocator : public PxAllocatorCallback
	{
		virtual void* allocate(size_t size, const char* typeName, const char* filename, int line)
		{
			return SAFE_MALLOC(size, MEMTYPE_HEAP);
		}

		virtual void deallocate(void* ptr)
		{
			SAFE_FREE(ptr);
		}
	};

	class PxError : public PxErrorCallback
	{
	public:
		virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
		{
			WriteLogE("PhysX: code = %d, message = %s, file = %s, line = %d\n", code, message, file, line);
		}
	};

	static PxPhysics *pPxPhysics = NULL;
	static PxCooking *pPxCooking = NULL;
	static PxFoundation *pPxFoundation = NULL;
	static PxMaterial *pPxStandardMaterials[MATERIAL_TYPE_COUNT] = { NULL };

	static PxError pxErrorCallback;
	static PxAllocator pxAllocatorCallback;

	static VOID PVDCreateConnection(PxPhysics& physics)
	{
		if (physics.getPvdConnectionManager() == NULL) {
			return;
		}

		PxVisualDebuggerConnectionFlags pvdConnectionFlags(PxVisualDebuggerExt::getAllConnectionFlags());
		PxVisualDebuggerConnection *pvdConnection = PxVisualDebuggerExt::createConnection(physics.getPvdConnectionManager(), "127.0.0.1", 5425, 10, pvdConnectionFlags);
		if (pvdConnection) pvdConnection->release();
	}

	static VOID PVDToggleConnection(PxPhysics& physics)
	{
		if (physics.getPvdConnectionManager() == NULL) return;
		if (physics.getPvdConnectionManager()->isConnected()) physics.getPvdConnectionManager()->disconnect();
		else PVDCreateConnection(physics);

		physics.getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
		physics.getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
		physics.getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_CONSTRAINTS, true);
	}

	static PxTolerancesScale PxCreateScale(VOID)
	{
		PxTolerancesScale scale;
		scale.length = 1.0f;  // 1米
		scale.speed = 10.0f;  // 重力加速度作用下自由下落1秒后的速度
		scale.mass = 1000.0f; // 1立方米1000公斤(1吨)
		return scale;
	}

	static BOOL PxInitPhysics(VOID)
	{
		ASSERT(pPxFoundation);

		pPxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pPxFoundation, PxCreateScale());
		return pPxPhysics ? TRUE : FALSE;
	}

	static BOOL PxInitCooking(VOID)
	{
		ASSERT(pPxFoundation);

		PxCookingParams params(PxCreateScale());
		params.meshWeldTolerance = EPSILON_E3;
		params.meshPreprocessParams = PxMeshPreprocessingFlags(
			PxMeshPreprocessingFlag::eWELD_VERTICES | 
			PxMeshPreprocessingFlag::eREMOVE_UNREFERENCED_VERTICES | 
			PxMeshPreprocessingFlag::eREMOVE_DUPLICATED_TRIANGLES | 
			PxMeshPreprocessingFlag::eFORCE_32BIT_INDICES);

		pPxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *pPxFoundation, params);
		return pPxCooking ? TRUE : FALSE;
	}

	static BOOL PxCreateStandardMaterials(PxPhysics& physics)
	{
		pPxStandardMaterials[MATERIAL_TYPE_DEFAULT] = physics.createMaterial(0.5f, 0.5f, 0.1f); // 默认
		pPxStandardMaterials[MATERIAL_TYPE_MUD] = physics.createMaterial(0.5f, 0.5f, 0.1f);     // 泥土路
		pPxStandardMaterials[MATERIAL_TYPE_TARMAC] = physics.createMaterial(0.5f, 0.5f, 0.1f);  // 柏油路
		pPxStandardMaterials[MATERIAL_TYPE_SNOW] = physics.createMaterial(0.5f, 0.5f, 0.1f);    // 雪路
		pPxStandardMaterials[MATERIAL_TYPE_ICE] = physics.createMaterial(0.5f, 0.5f, 0.1f);     // 冰
		pPxStandardMaterials[MATERIAL_TYPE_WATER] = physics.createMaterial(0.5f, 0.5f, 0.1f);   // 水
		pPxStandardMaterials[MATERIAL_TYPE_GRASS] = physics.createMaterial(0.5f, 0.5f, 0.1f);   // 草
		pPxStandardMaterials[MATERIAL_TYPE_CEMENT] = physics.createMaterial(0.5f, 0.5f, 0.1f);  // 水泥
		pPxStandardMaterials[MATERIAL_TYPE_METAL] = physics.createMaterial(0.5f, 0.5f, 0.1f);   // 金属
		pPxStandardMaterials[MATERIAL_TYPE_WOOD] = physics.createMaterial(0.5f, 0.5f, 0.1f);    // 木头
		pPxStandardMaterials[MATERIAL_TYPE_CLOTH] = physics.createMaterial(0.5f, 0.5f, 0.1f);   // 布料
		pPxStandardMaterials[MATERIAL_TYPE_GLASS] = physics.createMaterial(0.5f, 0.5f, 0.1f);   // 玻璃

		return TRUE;
	}

	static VOID PxDestroyStandardMaterials(VOID)
	{
		for (INT indexMaterial = 0; indexMaterial < MATERIAL_TYPE_COUNT; indexMaterial++) {
			if (pPxStandardMaterials[indexMaterial]) {
				pPxStandardMaterials[indexMaterial]->release();
				pPxStandardMaterials[indexMaterial] = NULL;
			}
		}
	}

	//
	// 初始化物理引擎
	//
	_CrossExport BOOL InitPhysics(VOID)
	{
		ASSERT(pPxPhysics == NULL);
		ASSERT(pPxCooking == NULL);
		ASSERT(pPxFoundation == NULL);

		//
		// 1. 创建物理引擎基础
		//
		pPxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, pxAllocatorCallback, pxErrorCallback);
		if (pPxFoundation == NULL) {
			WriteLogE("PhysX: PxCreateFoundation failed.");
			return FALSE;
		}

		//
		// 2. 初始化物理引擎
		//
		if (PxInitPhysics() == FALSE) {
			WriteLogE("PhysX: PxInitPhysics failed.");
			return FALSE;
		}

		//
		// 3. 初始化物理引擎烹调
		//
		if (PxInitCooking() == FALSE) {
			WriteLogE("PhysX: PxInitCooking failed.");
			return FALSE;
		}

		//
		// 4. 初始化物理引擎扩展
		//
		if (PxInitExtensions(*pPxPhysics) == FALSE) {
			WriteLogE("PhysX: PxInitExtensions failed.");
			return FALSE;
		}

		//
		// 5. 创建物理引擎标准材质
		//
		if (PxCreateStandardMaterials(*pPxPhysics) == FALSE) {
			WriteLogE("PhysX: PxCreateStandardMaterials failed.");
			return FALSE;
		}

		//
		// 6. 创建调试器链接
		//
#if defined _PLATFORM_WINDOWS_

		PVDToggleConnection(*pPxPhysics);

#endif

		return TRUE;
	}

	//
	// 退出物理引擎
	//
	_CrossExport VOID ExitPhysics(VOID)
	{
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		PxCloseExtensions();
		PxDestroyStandardMaterials();

		pPxCooking->release();
		pPxPhysics->release();
		pPxFoundation->release();

		pPxPhysics = NULL;
		pPxCooking = NULL;
		pPxFoundation = NULL;
	}

	//
	// 获得物理引擎
	//
	_CrossExport PxPhysics& GetPhysics(VOID)
	{
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		return *pPxPhysics;
	}

	//
	// 获得物理引擎烹调
	//
	_CrossExport PxCooking& GetPhysicsCooking(VOID)
	{
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		return *pPxCooking;
	}

	//
	// 获得物理引擎默认材质
	//
	_CrossExport PxMaterial& GetPhysicsDefaultMaterial(VOID)
	{
		return GetPhysicsStandardMaterial(MATERIAL_TYPE_DEFAULT);
	}

	//
	// 获得物理引擎标准材质
	//
	_CrossExport PxMaterial& GetPhysicsStandardMaterial(MATERIAL_TYPE type)
	{
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);
		ASSERT(pPxStandardMaterials[type]);

		return *pPxStandardMaterials[type];
	}

	//
	// 创建物理凸包网格
	//
	_CrossExport PxConvexMesh* PxCreateConvexMesh(const VERTEX *pVertexBuffer, INT numVertices)
	{
		ASSERT(pVertexBuffer);
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		PxConvexMesh *pPxMesh = NULL;

		PxVec3 *pVertices = SAFE_NEW(MEMTYPE_STACK) PxVec3[numVertices];
		{
			for (INT indexVertex = 0; indexVertex < numVertices; indexVertex++) {
				pVertices[indexVertex].x = pVertexBuffer[indexVertex].position[0];
				pVertices[indexVertex].y = pVertexBuffer[indexVertex].position[1];
				pVertices[indexVertex].z = pVertexBuffer[indexVertex].position[2];
			}

			pPxMesh = createConvexMesh(*pPxPhysics, *pPxCooking, pVertices, numVertices, PxConvexFlag::eCOMPUTE_CONVEX /*| PxConvexFlag::eINFLATE_CONVEX*/);
		}
		SAFE_DELETE_ARRAY(pVertices);

		return pPxMesh;
	}

	//
	// 创建物理三角网格
	//
	_CrossExport PxTriangleMesh* PxCreateTriangleMesh(const VERTEX *pVertexBuffer, INT numVertices, const FACE *pFaceBuffer, INT numFaces)
	{
		ASSERT(pFaceBuffer);
		ASSERT(pVertexBuffer);
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		PxTriangleMesh *pPxMesh = NULL;

		PxU32 *pIndices = SAFE_NEW(MEMTYPE_STACK) PxU32[numFaces * 3];
		PxVec3 *pVertices = SAFE_NEW(MEMTYPE_STACK) PxVec3[numVertices];
		{
			for (INT indexFace = 0; indexFace < numFaces; indexFace++) {
				pIndices[indexFace * 3 + 0] = pFaceBuffer[indexFace].index0;
				pIndices[indexFace * 3 + 1] = pFaceBuffer[indexFace].index1;
				pIndices[indexFace * 3 + 2] = pFaceBuffer[indexFace].index2;
			}

			for (INT indexVertex = 0; indexVertex < numVertices; indexVertex++) {
				pVertices[indexVertex].x = pVertexBuffer[indexVertex].position[0];
				pVertices[indexVertex].y = pVertexBuffer[indexVertex].position[1];
				pVertices[indexVertex].z = pVertexBuffer[indexVertex].position[2];
			}

			pPxMesh = createTriangleMesh32(*pPxPhysics, *pPxCooking, pVertices, numVertices, pIndices, numFaces);
		}
		SAFE_DELETE_ARRAY(pVertices);
		SAFE_DELETE_ARRAY(pIndices);

		return pPxMesh;
	}

	//
	// 创建立方体物理网格
	//
	_CrossExport PxConvexMesh* PxCreateCubeConvexMesh(PxF32 width, PxF32 height, PxF32 length)
	{
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		PxVec3 vertices[8] = {
			PxVec3(-width / 2.0f, -height / 2.0f, -length / 2.0f),
			PxVec3(-width / 2.0f, -height / 2.0f, +length / 2.0f),
			PxVec3(-width / 2.0f, +height / 2.0f, -length / 2.0f),
			PxVec3(-width / 2.0f, +height / 2.0f, +length / 2.0f),
			PxVec3(+width / 2.0f, -height / 2.0f, -length / 2.0f),
			PxVec3(+width / 2.0f, -height / 2.0f, +length / 2.0f),
			PxVec3(+width / 2.0f, +height / 2.0f, -length / 2.0f),
			PxVec3(+width / 2.0f, +height / 2.0f, +length / 2.0f)
		};

		return createConvexMesh(*pPxPhysics, *pPxCooking, vertices, 8, PxConvexFlag::eCOMPUTE_CONVEX);
	}

	//
	// 创建圆柱体物理网格
	//
	_CrossExport PxConvexMesh* PxCreateCylinderConvexMesh(PxF32 length, PxF32 radius)
	{
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		#define MAX_NUM_VERTS_IN_CIRCLE 16

		PxVec3 vertices[2 * MAX_NUM_VERTS_IN_CIRCLE];
		PxU32 numVertices = 2 * MAX_NUM_VERTS_IN_CIRCLE;
		PxF32 delta = PxTwoPi / MAX_NUM_VERTS_IN_CIRCLE;

		for (PxU32 indexVertex = 0; indexVertex < MAX_NUM_VERTS_IN_CIRCLE; indexVertex++) {
			PxF32 theta = delta * indexVertex;
			PxF32 sinTheta = radius * PxSin(theta);
			PxF32 cosTheta = radius * PxCos(theta);
			vertices[2 * indexVertex + 0] = PxVec3(sinTheta, cosTheta, +length / 2.0f);
			vertices[2 * indexVertex + 1] = PxVec3(sinTheta, cosTheta, -length / 2.0f);
		}

		return createConvexMesh(*pPxPhysics, *pPxCooking, vertices, numVertices, PxConvexFlag::eCOMPUTE_CONVEX);
	}

	//
	// 设置物理图形
	//
	static void PxSetShape(PxShape *pPxShape, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData)
	{
		ASSERT(pPxShape);
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		pPxShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, bEnableQuery ? true : false);
		pPxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, bEnableSimulation ? true : false);
		pPxShape->setQueryFilterData(pxQueryFilterData);
		pPxShape->setSimulationFilterData(pxSimulationFilterData);
	}

	//
	// 创建物理立方体图形
	//
	_CrossExport PxShape* PxCreateShapeBox(PxRigidActor *pPxRigidActor, PxReal width, PxReal height, PxReal length, const PxMaterial& pxMaterial, const PxTransform& pxLocalPose, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData)
	{
		ASSERT(pPxRigidActor);
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		PxShape *pPxShape = pPxRigidActor->createShape(PxBoxGeometry(width / 2.0f, height / 2.0f, length / 2.0f), pxMaterial, pxLocalPose);
		PxSetShape(pPxShape, bEnableQuery, bEnableSimulation, pxQueryFilterData, pxSimulationFilterData);

		return pPxShape;
	}

	//
	// 创建物理球体图形
	//
	_CrossExport PxShape* PxCreateShapeSphere(PxRigidActor *pPxRigidActor, PxReal radius, const PxMaterial& pxMaterial, const PxTransform& pxLocalPose, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData)
	{
		ASSERT(pPxRigidActor);
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		PxShape *pPxShape = pPxRigidActor->createShape(PxSphereGeometry(radius), pxMaterial, pxLocalPose);
		PxSetShape(pPxShape, bEnableQuery, bEnableSimulation, pxQueryFilterData, pxSimulationFilterData);

		return pPxShape;
	}

	//
	// 创建物理胶囊体图形
	//
	_CrossExport PxShape* PxCreateShapeCapsule(PxRigidActor *pPxRigidActor, PxReal radius, PxReal height, const PxMaterial& pxMaterial, const PxTransform& pxLocalPose, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData)
	{
		ASSERT(pPxRigidActor);
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		PxShape *pPxShape = pPxRigidActor->createShape(PxCapsuleGeometry(radius, height / 2.0f), pxMaterial, pxLocalPose);
		PxSetShape(pPxShape, bEnableQuery, bEnableSimulation, pxQueryFilterData, pxSimulationFilterData);

		return pPxShape;
	}

	//
	// 创建物理网格图形
	//
	_CrossExport PxShape* PxCreateShapeMesh(PxRigidActor *pPxRigidActor, const VERTEX *pVertexBuffer, INT numVertices, const FACE *pFaceBuffer, INT numFaces, const PxMaterial& pxMaterial, const PxTransform& pxLocalPose, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData)
	{
		ASSERT(pFaceBuffer);
		ASSERT(pVertexBuffer);
		ASSERT(pPxRigidActor);
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		PxShape *pPxShape = pPxRigidActor->createShape(PxTriangleMeshGeometry(PxCreateTriangleMesh(pVertexBuffer, numVertices, pFaceBuffer, numFaces)), pxMaterial, pxLocalPose);
		PxSetShape(pPxShape, bEnableQuery, bEnableSimulation, pxQueryFilterData, pxSimulationFilterData);

		return pPxShape;
	}

	//
	// 创建刚体角色
	//
	_CrossExport PxRigidActor* PxCreateRigidActor(const PxTransform& pxGlobalPose, BOOL bDynamic, BOOL bKinematic, BOOL bEnableCCD)
	{
		ASSERT(pPxPhysics);
		ASSERT(pPxCooking);
		ASSERT(pPxFoundation);

		PxRigidActor *pPxRigidActor = NULL;

		if (bDynamic) {
			pPxRigidActor = pPxPhysics->createRigidDynamic(pxGlobalPose);

			((PxRigidDynamic *)pPxRigidActor)->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, bEnableCCD ? true : false);
			((PxRigidDynamic *)pPxRigidActor)->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, bKinematic ? true : false);

		}
		else {
			pPxRigidActor = pPxPhysics->createRigidStatic(pxGlobalPose);
		}

		return pPxRigidActor;
	}

}

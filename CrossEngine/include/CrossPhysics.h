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

#pragma once


#include "CrossEngine.h"

#include "PxPhysicsAPI.h"
#include "PxToolkit/PxToolkit.h"

using namespace physx;
using namespace PxToolkit;

#define GRAVITY                        10.0f                                                       // 重力加速度

#define PX_SCENE_LOCK_RW_BEGIN(scene)  do { if (scene) scene->lockWrite();  } while (0);           // 场景读写锁开始
#define PX_SCENE_LOCK_RW_END(scene)    do { if (scene) scene->unlockWrite();} while (0);           // 场景读写锁结束

namespace CrossEngine {

	typedef enum {
		MATERIAL_TYPE_DEFAULT = 0,                                                                 // 默认
		MATERIAL_TYPE_MUD,                                                                         // 泥土路
		MATERIAL_TYPE_TARMAC,                                                                      // 柏油路
		MATERIAL_TYPE_SNOW,                                                                        // 雪
		MATERIAL_TYPE_ICE,                                                                         // 冰
		MATERIAL_TYPE_WATER,                                                                       // 水
		MATERIAL_TYPE_GRASS,                                                                       // 草
		MATERIAL_TYPE_CEMENT,                                                                      // 水泥
		MATERIAL_TYPE_METAL,                                                                       // 金属
		MATERIAL_TYPE_WOOD,                                                                        // 木头
		MATERIAL_TYPE_CLOTH,                                                                       // 布料
		MATERIAL_TYPE_GLASS,                                                                       // 玻璃
		MATERIAL_TYPE_COUNT
	} MATERIAL_TYPE;

	enum {
		SIMULATION_FLAGS_NONE                              = 0x00000000,                           // 模拟空标识
		SIMULATION_FLAGS_VEHICLE_GROUND                    = 0x00000001,                           // 模拟车辆系统地面
		SIMULATION_FLAGS_VEHICLE_WHEEL                     = 0x00000002,                           // 模拟车辆系统车轮
		SIMULATION_FLAGS_VEHICLE_CHASSIS                   = 0x00000004,                           // 模拟车辆系统车身
		SIMULATION_FLAGS_VEHICLE_OBSTACLE                  = 0x00000008,                           // 模拟车辆系统不可驾驶障碍
		SIMULATION_FLAGS_VEHICLE_DRIVABLE_OBSTACLE         = 0x00000010,                           // 模拟车辆系统可驾驶障碍

		SIMULATION_VEHICLE_FLAGS_GROUND_AGAINST            = SIMULATION_FLAGS_VEHICLE_CHASSIS | SIMULATION_FLAGS_VEHICLE_OBSTACLE | SIMULATION_FLAGS_VEHICLE_DRIVABLE_OBSTACLE,
		SIMULATION_VEHICLE_FLAGS_WHEEL_AGAINST             = SIMULATION_FLAGS_VEHICLE_WHEEL | SIMULATION_FLAGS_VEHICLE_CHASSIS | SIMULATION_FLAGS_VEHICLE_OBSTACLE,
		SIMULATION_VEHICLE_FLAGS_CHASSIS_AGAINST           = SIMULATION_FLAGS_VEHICLE_GROUND | SIMULATION_FLAGS_VEHICLE_WHEEL | SIMULATION_FLAGS_VEHICLE_CHASSIS | SIMULATION_FLAGS_VEHICLE_OBSTACLE | SIMULATION_FLAGS_VEHICLE_DRIVABLE_OBSTACLE,
		SIMULATION_VEHICLE_FLAGS_OBSTACLE_AGAINST          = SIMULATION_FLAGS_VEHICLE_GROUND | SIMULATION_FLAGS_VEHICLE_WHEEL | SIMULATION_FLAGS_VEHICLE_CHASSIS | SIMULATION_FLAGS_VEHICLE_OBSTACLE | SIMULATION_FLAGS_VEHICLE_DRIVABLE_OBSTACLE,
		SIMULATION_VEHICLE_FLAGS_DRIVABLE_OBSTACLE_AGAINST = SIMULATION_FLAGS_VEHICLE_GROUND | SIMULATION_FLAGS_VEHICLE_CHASSIS | SIMULATION_FLAGS_VEHICLE_OBSTACLE | SIMULATION_FLAGS_VEHICLE_DRIVABLE_OBSTACLE,
	};

	// PxFilterData::word0
	static const DWORD QUERY_FLAGS_NONE                    = 0x00000000;                           // 查询空标识
	static const DWORD QUERY_FLAGS_SCENE_MESH              = 0x00000001;                           // 查询场景网格标识
	static const DWORD QUERY_FLAGS_SCENE_VOLUME            = 0x00000002;                           // 查询场景包围体标识
	static const DWORD QUERY_FLAGS_ACTOR_HITBOX            = 0x00010000;                           // 查询角色碰撞盒标识
	static const DWORD QUERY_FLAGS_ACTOR_CAPSULE           = 0x00020000;                           // 查询角色胶囊体标识

	// PxFilterData::word0
	static const DWORD SIMULATION_FLAGS_WORD0[] = {
		SIMULATION_FLAGS_NONE,                                                                     // 模拟空标识
		SIMULATION_FLAGS_VEHICLE_GROUND,                                                           // 模拟车辆系统地面
		SIMULATION_FLAGS_VEHICLE_WHEEL,                                                            // 模拟车辆系统车轮
		SIMULATION_FLAGS_VEHICLE_CHASSIS,                                                          // 模拟车辆系统车身
		SIMULATION_FLAGS_VEHICLE_OBSTACLE,                                                         // 模拟车辆系统不可驾驶障碍
		SIMULATION_FLAGS_VEHICLE_DRIVABLE_OBSTACLE,                                                // 模拟车辆系统可驾驶障碍
	};

	// PxFilterData::word1
	static const DWORD SIMULATION_FLAGS_WORD1[] = {
		SIMULATION_FLAGS_NONE,                                                                     // 模拟空标识
		SIMULATION_VEHICLE_FLAGS_GROUND_AGAINST,                                                   // 模拟车辆系统地面
		SIMULATION_VEHICLE_FLAGS_WHEEL_AGAINST,                                                    // 模拟车辆系统车轮
		SIMULATION_VEHICLE_FLAGS_CHASSIS_AGAINST,                                                  // 模拟车辆系统车身
		SIMULATION_VEHICLE_FLAGS_OBSTACLE_AGAINST,                                                 // 模拟车辆系统不可驾驶障碍
		SIMULATION_VEHICLE_FLAGS_DRIVABLE_OBSTACLE_AGAINST,                                        // 模拟车辆系统可驾驶障碍
	};

	// PxFilterData::word2
	static const DWORD SIMULATION_FLAGS_DISABLE_PAIR_CCD = 0x00000001;                             // 模拟禁用CCD(说明: 当两物体同时具有此属性时禁用他们之间的CCD)


	_CrossExport BOOL InitPhysics(VOID);                                                           // 初始化物理引擎
	_CrossExport VOID ExitPhysics(VOID);                                                           // 退出物理引擎

	_CrossExport PxPhysics& GetPhysics(VOID);                                                      // 获得物理引擎
	_CrossExport PxCooking& GetPhysicsCooking(VOID);                                               // 获得物理引擎烹调
	_CrossExport PxMaterial& GetPhysicsDefaultMaterial(VOID);                                      // 获得物理引擎默认材质
	_CrossExport PxMaterial& GetPhysicsStandardMaterial(MATERIAL_TYPE type);                       // 获得物理引擎标准材质

	_CrossExport PxConvexMesh* PxCreateConvexMesh(const VERTEX *pVertexBuffer, INT numVertices);   // 创建物理凸包网格
	_CrossExport PxTriangleMesh* PxCreateTriangleMesh(const VERTEX *pVertexBuffer, INT numVertices, const FACE *pFaceBuffer, INT numFaces); // 创建物理三角网格

	_CrossExport PxConvexMesh* PxCreateCubeConvexMesh(PxF32 width, PxF32 height, PxF32 length);    // 创建立方体物理网格
	_CrossExport PxConvexMesh* PxCreateCylinderConvexMesh(PxF32 length, PxF32 radius);             // 创建圆柱体物理网格

	_CrossExport PxShape* PxCreateShapeBox(PxRigidActor *pPxRigidActor, PxReal width, PxReal height, PxReal length, const PxMaterial& pxMaterial, const PxTransform& pxLocalPose, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData); // 创建物理立方体图形
	_CrossExport PxShape* PxCreateShapeSphere(PxRigidActor *pPxRigidActor, PxReal radius, const PxMaterial& pxMaterial, const PxTransform& pxLocalPose, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData); // 创建物理球体图形
	_CrossExport PxShape* PxCreateShapeCapsule(PxRigidActor *pPxRigidActor, PxReal radius, PxReal height, const PxMaterial& pxMaterial, const PxTransform& pxLocalPose, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData); // 创建物理胶囊体图形
	_CrossExport PxShape* PxCreateShapeMesh(PxRigidActor *pPxRigidActor, const VERTEX *pVertexBuffer, INT numVertices, const FACE *pFaceBuffer, INT numFaces, const PxMaterial& pxMaterial, const PxTransform& pxLocalPose, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData); // 创建物理网格图形

	_CrossExport PxRigidActor* PxCreateRigidActor(const PxTransform& pxGlobalPose, BOOL bDynamic, BOOL bKinematic, BOOL bEnableCCD); // 创建刚体角色

}

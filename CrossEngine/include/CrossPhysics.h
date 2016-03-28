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

#define GRAVITY                        10.0f                                                       // �������ٶ�

#define PX_SCENE_LOCK_RW_BEGIN(scene)  do { if (scene) scene->lockWrite();  } while (0);           // ������д����ʼ
#define PX_SCENE_LOCK_RW_END(scene)    do { if (scene) scene->unlockWrite();} while (0);           // ������д������

namespace CrossEngine {

	typedef enum {
		MATERIAL_TYPE_DEFAULT = 0,                                                                 // Ĭ��
		MATERIAL_TYPE_MUD,                                                                         // ����·
		MATERIAL_TYPE_TARMAC,                                                                      // ����·
		MATERIAL_TYPE_SNOW,                                                                        // ѩ
		MATERIAL_TYPE_ICE,                                                                         // ��
		MATERIAL_TYPE_WATER,                                                                       // ˮ
		MATERIAL_TYPE_GRASS,                                                                       // ��
		MATERIAL_TYPE_CEMENT,                                                                      // ˮ��
		MATERIAL_TYPE_METAL,                                                                       // ����
		MATERIAL_TYPE_WOOD,                                                                        // ľͷ
		MATERIAL_TYPE_CLOTH,                                                                       // ����
		MATERIAL_TYPE_GLASS,                                                                       // ����
		MATERIAL_TYPE_COUNT
	} MATERIAL_TYPE;

	enum {
		SIMULATION_FLAGS_NONE                              = 0x00000000,                           // ģ��ձ�ʶ
		SIMULATION_FLAGS_VEHICLE_GROUND                    = 0x00000001,                           // ģ�⳵��ϵͳ����
		SIMULATION_FLAGS_VEHICLE_WHEEL                     = 0x00000002,                           // ģ�⳵��ϵͳ����
		SIMULATION_FLAGS_VEHICLE_CHASSIS                   = 0x00000004,                           // ģ�⳵��ϵͳ����
		SIMULATION_FLAGS_VEHICLE_OBSTACLE                  = 0x00000008,                           // ģ�⳵��ϵͳ���ɼ�ʻ�ϰ�
		SIMULATION_FLAGS_VEHICLE_DRIVABLE_OBSTACLE         = 0x00000010,                           // ģ�⳵��ϵͳ�ɼ�ʻ�ϰ�

		SIMULATION_VEHICLE_FLAGS_GROUND_AGAINST            = SIMULATION_FLAGS_VEHICLE_CHASSIS | SIMULATION_FLAGS_VEHICLE_OBSTACLE | SIMULATION_FLAGS_VEHICLE_DRIVABLE_OBSTACLE,
		SIMULATION_VEHICLE_FLAGS_WHEEL_AGAINST             = SIMULATION_FLAGS_VEHICLE_WHEEL | SIMULATION_FLAGS_VEHICLE_CHASSIS | SIMULATION_FLAGS_VEHICLE_OBSTACLE,
		SIMULATION_VEHICLE_FLAGS_CHASSIS_AGAINST           = SIMULATION_FLAGS_VEHICLE_GROUND | SIMULATION_FLAGS_VEHICLE_WHEEL | SIMULATION_FLAGS_VEHICLE_CHASSIS | SIMULATION_FLAGS_VEHICLE_OBSTACLE | SIMULATION_FLAGS_VEHICLE_DRIVABLE_OBSTACLE,
		SIMULATION_VEHICLE_FLAGS_OBSTACLE_AGAINST          = SIMULATION_FLAGS_VEHICLE_GROUND | SIMULATION_FLAGS_VEHICLE_WHEEL | SIMULATION_FLAGS_VEHICLE_CHASSIS | SIMULATION_FLAGS_VEHICLE_OBSTACLE | SIMULATION_FLAGS_VEHICLE_DRIVABLE_OBSTACLE,
		SIMULATION_VEHICLE_FLAGS_DRIVABLE_OBSTACLE_AGAINST = SIMULATION_FLAGS_VEHICLE_GROUND | SIMULATION_FLAGS_VEHICLE_CHASSIS | SIMULATION_FLAGS_VEHICLE_OBSTACLE | SIMULATION_FLAGS_VEHICLE_DRIVABLE_OBSTACLE,
	};

	// PxFilterData::word0
	static const DWORD QUERY_FLAGS_NONE                    = 0x00000000;                           // ��ѯ�ձ�ʶ
	static const DWORD QUERY_FLAGS_SCENE_MESH              = 0x00000001;                           // ��ѯ���������ʶ
	static const DWORD QUERY_FLAGS_SCENE_VOLUME            = 0x00000002;                           // ��ѯ������Χ���ʶ
	static const DWORD QUERY_FLAGS_ACTOR_HITBOX            = 0x00010000;                           // ��ѯ��ɫ��ײ�б�ʶ
	static const DWORD QUERY_FLAGS_ACTOR_CAPSULE           = 0x00020000;                           // ��ѯ��ɫ�������ʶ

	// PxFilterData::word0
	static const DWORD SIMULATION_FLAGS_WORD0[] = {
		SIMULATION_FLAGS_NONE,                                                                     // ģ��ձ�ʶ
		SIMULATION_FLAGS_VEHICLE_GROUND,                                                           // ģ�⳵��ϵͳ����
		SIMULATION_FLAGS_VEHICLE_WHEEL,                                                            // ģ�⳵��ϵͳ����
		SIMULATION_FLAGS_VEHICLE_CHASSIS,                                                          // ģ�⳵��ϵͳ����
		SIMULATION_FLAGS_VEHICLE_OBSTACLE,                                                         // ģ�⳵��ϵͳ���ɼ�ʻ�ϰ�
		SIMULATION_FLAGS_VEHICLE_DRIVABLE_OBSTACLE,                                                // ģ�⳵��ϵͳ�ɼ�ʻ�ϰ�
	};

	// PxFilterData::word1
	static const DWORD SIMULATION_FLAGS_WORD1[] = {
		SIMULATION_FLAGS_NONE,                                                                     // ģ��ձ�ʶ
		SIMULATION_VEHICLE_FLAGS_GROUND_AGAINST,                                                   // ģ�⳵��ϵͳ����
		SIMULATION_VEHICLE_FLAGS_WHEEL_AGAINST,                                                    // ģ�⳵��ϵͳ����
		SIMULATION_VEHICLE_FLAGS_CHASSIS_AGAINST,                                                  // ģ�⳵��ϵͳ����
		SIMULATION_VEHICLE_FLAGS_OBSTACLE_AGAINST,                                                 // ģ�⳵��ϵͳ���ɼ�ʻ�ϰ�
		SIMULATION_VEHICLE_FLAGS_DRIVABLE_OBSTACLE_AGAINST,                                        // ģ�⳵��ϵͳ�ɼ�ʻ�ϰ�
	};

	// PxFilterData::word2
	static const DWORD SIMULATION_FLAGS_DISABLE_PAIR_CCD = 0x00000001;                             // ģ�����CCD(˵��: ��������ͬʱ���д�����ʱ��������֮���CCD)


	_CrossExport BOOL InitPhysics(VOID);                                                           // ��ʼ����������
	_CrossExport VOID ExitPhysics(VOID);                                                           // �˳���������

	_CrossExport PxPhysics& GetPhysics(VOID);                                                      // �����������
	_CrossExport PxCooking& GetPhysicsCooking(VOID);                                               // ��������������
	_CrossExport PxMaterial& GetPhysicsDefaultMaterial(VOID);                                      // �����������Ĭ�ϲ���
	_CrossExport PxMaterial& GetPhysicsStandardMaterial(MATERIAL_TYPE type);                       // ������������׼����

	_CrossExport PxConvexMesh* PxCreateConvexMesh(const VERTEX *pVertexBuffer, INT numVertices);   // ��������͹������
	_CrossExport PxTriangleMesh* PxCreateTriangleMesh(const VERTEX *pVertexBuffer, INT numVertices, const FACE *pFaceBuffer, INT numFaces); // ����������������

	_CrossExport PxConvexMesh* PxCreateCubeConvexMesh(PxF32 width, PxF32 height, PxF32 length);    // ������������������
	_CrossExport PxConvexMesh* PxCreateCylinderConvexMesh(PxF32 length, PxF32 radius);             // ����Բ������������

	_CrossExport PxShape* PxCreateShapeBox(PxRigidActor *pPxRigidActor, PxReal width, PxReal height, PxReal length, const PxMaterial& pxMaterial, const PxTransform& pxLocalPose, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData); // ��������������ͼ��
	_CrossExport PxShape* PxCreateShapeSphere(PxRigidActor *pPxRigidActor, PxReal radius, const PxMaterial& pxMaterial, const PxTransform& pxLocalPose, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData); // ������������ͼ��
	_CrossExport PxShape* PxCreateShapeCapsule(PxRigidActor *pPxRigidActor, PxReal radius, PxReal height, const PxMaterial& pxMaterial, const PxTransform& pxLocalPose, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData); // ������������ͼ��
	_CrossExport PxShape* PxCreateShapeMesh(PxRigidActor *pPxRigidActor, const VERTEX *pVertexBuffer, INT numVertices, const FACE *pFaceBuffer, INT numFaces, const PxMaterial& pxMaterial, const PxTransform& pxLocalPose, BOOL bEnableQuery, BOOL bEnableSimulation, const PxFilterData& pxQueryFilterData, const PxFilterData& pxSimulationFilterData); // ������������ͼ��

	_CrossExport PxRigidActor* PxCreateRigidActor(const PxTransform& pxGlobalPose, BOOL bDynamic, BOOL bKinematic, BOOL bEnableCCD); // ���������ɫ

}

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


namespace CrossEngine {

	class _CrossExport CPhysicsManager
	{
		friend class CSceneManager;


		// 数据结构
	public:
		typedef std::map<DWORD, PxActor*> PxActorMap;                                              // 角色集合


		// 构造/析构函数
	protected:
		CPhysicsManager(VOID);
		virtual ~CPhysicsManager(VOID);


		// 方法
	public:
		VOID CreateScene(VOID);                                                                    // 创建场景
		VOID DestroyScene(VOID);                                                                   // 销毁场景

		PxScene* GetScene(VOID) const;                                                             // 获得场景
		PxControllerManager* GetControllerManager(VOID) const;                                     // 获得控制器管理器

	public:
		PxActor* CreateRigidActor(const CHAR *szName, const PxTransform &pxGlobalPose, BOOL bDynamic, BOOL bKinematic = FALSE, BOOL bEnableCCD = FALSE); // 创建刚体角色
		PxActor* CreateRigidActor(DWORD dwName, const PxTransform &pxGlobalPose, BOOL bDynamic, BOOL bKinematic = FALSE, BOOL bEnableCCD = FALSE); // 创建刚体角色
		VOID DestroyActor(DWORD dwName);                                                           // 销毁角色
		PxActor* GetActor(DWORD dwName);                                                           // 获得角色

	public:
		VOID EnableQuery(DWORD dwName, BOOL bEnable);                                              // 设置查询状态
		VOID EnableSimulation(DWORD dwName, BOOL bEnable);                                         // 设置模拟状态
		VOID EnableKinematic(DWORD dwName, BOOL bEnable);                                          // 设置运动状态
		VOID EnableCCD(DWORD dwName, BOOL bEnable);                                                // 设置CCD状态

	public:
		DWORD RayCastScene(DWORD dwQueryFlags, const RAY *pRay, FLOAT *length, VEC3 *position, VEC3 *normal, VOID **ppActorData, VOID **ppShapeData) const; // 射线检测场景

	public:
		VOID Begin(VOID);                                                                          // 物理开始
		VOID End(PxReal deltaTime);                                                                // 物理结束

		
		// 属性
	protected:
		PxActorMap m_actors;                                                                       // 角色集合

	protected:
		PxScene *m_pPxScene;                                                                       // 物理场景
		PxControllerManager *m_pPxControllerManager;                                               // 物理控制管理器
		PxDefaultCpuDispatcher *m_pPxCpuDispatcher;                                                // 调度器
	};

}

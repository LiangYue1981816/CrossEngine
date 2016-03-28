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

	class _CrossExport CEntity;
	class _CrossExport CSceneNode
	{
		friend class CEntity;
		friend class CEntityLight;
		friend class CEntityRenderable;
		friend class CSceneManager;


		// 数据结构
	public:
		typedef std::map<DWORD, CEntity*> EntityMap;                                               // 实体集合
		typedef std::map<DWORD, CSceneNode*> SceneNodeMap;                                         // 场景节点集合


		// 构造/析构函数
	protected:
		CSceneNode(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CSceneNode(VOID);


		// 方法
	public:
		DWORD GetName(VOID) const;                                                                 // 获得名称

		CSceneNode* GetParentNode(VOID) const;                                                     // 获得父节点
		CSceneManager* GetSceneManager(VOID) const;                                                // 获得场景管理器

		const AABB* GetAABBCurrent(VOID) const;                                                    // 获得当前包围盒
		const AABB* GetAABBCascade(VOID) const;                                                    // 获得级联包围盒

	public:
		VOID SetVisible(BOOL bVisible, BOOL bCascade);                                             // 设置可见性
		BOOL IsVisible(VOID) const;                                                                // 获得可见性

		VOID SetRenderable(BOOL bNeedRender, BOOL bCascade);                                       // 设置可渲染性
		BOOL IsRenderable(VOID) const;                                                             // 获得可渲染性

	public:
		BOOL AttachEntity(CEntity *pEntity);                                                       // 挂入实体
		VOID DetachEntity(CEntity *pEntity);                                                       // 卸载实体
		VOID DetachAllEntity(VOID);                                                                // 卸载所有实体

		CEntity* QueryEntity(const CHAR *szName) const;                                            // 查询挂入实体
		CEntity* QueryEntity(DWORD dwName) const;                                                  // 查询挂入实体

		CEntity* GetEntityListHead(VOID) const;                                                    // 获得实体链表头

	public:
		BOOL AttachNode(CSceneNode *pNode);                                                        // 挂入节点
		VOID DetachNode(CSceneNode *pNode);                                                        // 卸载节点
		VOID DetachAllNode(VOID);                                                                  // 卸载所有节点

		CSceneNode* QueryChildNode(const CHAR *szName) const;                                      // 查询子节点
		CSceneNode* QueryChildNode(DWORD dwName) const;                                            // 查询子节点

		CSceneNode* GetNodeListHead(VOID) const;                                                   // 获得节点链表头

	public:
		VOID Identity(VOID);                                                                       // 初始化

		VOID SetLocalScale(FLOAT x, FLOAT y, FLOAT z);                                             // 设置本地缩放
		VOID SetLocalPosition(FLOAT x, FLOAT y, FLOAT z);                                          // 设置本地位置
		VOID SetLocalOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w);                              // 设置本地朝向
		VOID SetLocalDirection(FLOAT x, FLOAT y, FLOAT z, FLOAT upx = 0.0f, FLOAT upy = 1.0f, FLOAT upz = 0.0f); // 设置本地朝向

		VOID SetWorldScale(FLOAT x, FLOAT y, FLOAT z);                                             // 设置世界缩放
		VOID SetWorldPosition(FLOAT x, FLOAT y, FLOAT z);                                          // 设置世界位置
		VOID SetWorldOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w);                              // 设置世界朝向
		VOID SetWorldDirection(FLOAT x, FLOAT y, FLOAT z, FLOAT upx = 0.0f, FLOAT upy = 1.0f, FLOAT upz = 0.0f); // 设置本地朝向

		const VEC3* GetLocalScale(VOID);                                                           // 获得本地缩放
		const VEC3* GetLocalPosition(VOID);                                                        // 获得本地位置
		const QUAT* GetLocalOrientation(VOID);                                                     // 获得本地朝向
		const MATRIX4* GetLocalMatrix(VOID);                                                       // 获得本地变换矩阵

		const VEC3* GetWorldScale(VOID);                                                           // 获得世界缩放
		const VEC3* GetWorldPosition(VOID);                                                        // 获得世界位置
		const QUAT* GetWorldOrientation(VOID);                                                     // 获得世界朝向
		const MATRIX4* GetWorldMatrix(VOID);                                                       // 获得世界变换矩阵

	protected:
		VOID UpdateTransformImmediately(VOID);                                                     // 立即更新变换
		VOID UpdateTransform(BOOL bParentUpdate = FALSE);                                          // 更新变换
		BOOL UpdateAABB(BOOL bParentUpdate = FALSE);                                               // 更新包围盒
		VOID UpdateEntity(FLOAT deltaTime);                                                        // 更新实体
		VOID UpdateRenderQuene(const CEntityCamera *pCamera);                                      // 更新渲染队列


		// 属性
	public:
		VOID *userData;                                                                            // 用户数据

	protected:
		DWORD m_dwName;                                                                            // 名称

		BOOL m_bVisible;                                                                           // 可见性
		BOOL m_bNeedRender;                                                                        // 需要渲染否
		BOOL m_bNeedUpdateAABB;                                                                    // 需要更新包围盒
		BOOL m_bNeedUpdateTransform;                                                               // 需要更新变换
		DWORD m_dwUpdateTransformCount;                                                            // 当前节点更新变换次数
		DWORD m_dwParentUpdateTransformCount;                                                      // 父亲节点更新变换次数

		VEC3 m_localScale;                                                                         // 本地缩放
		VEC3 m_localPosition;                                                                      // 本地位置
		QUAT m_localOrientation;                                                                   // 本地朝向
		MATRIX4 m_mtxLocal;                                                                        // 本地变换矩阵

		VEC3 m_worldScale;                                                                         // 世界缩放
		VEC3 m_worldPosition;                                                                      // 世界位置
		QUAT m_worldOrientation;                                                                   // 世界朝向
		MATRIX4 m_mtxWorld;                                                                        // 世界变换矩阵

		AABB m_aabbCurrent;                                                                        // 当前包围盒
		AABB m_aabbCascade;                                                                        // 级联包围盒

	protected:
		EntityMap m_entities;                                                                      // 实体集合
		SceneNodeMap m_nodes;                                                                      // 节点集合

		CEntity *m_pEntityHead;                                                                    // 实体链表
		CSceneNode *m_pNodeHead;                                                                   // 节点链表

	protected:
		CSceneNode *m_pParentNode;                                                                 // 父节点指针
		CSceneManager *m_pSceneManager;                                                            // 场景管理器

	public:
		CSceneNode *pNext;                                                                         // 节点链表
	};

}

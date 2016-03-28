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
	class _CrossExport CSceneManager
	{
		friend class CSceneSystem;


		// 数据结构
	public:
		typedef std::map<DWORD, CEntity*> EntityMap;                                               // 实体集合
		typedef std::map<DWORD, CSceneNode*> SceneNodeMap;                                         // 节点集合


		// 构造/析构函数
	protected:
		CSceneManager(VOID);
		virtual ~CSceneManager(VOID);


		// 方法
	public:
		virtual VOID Init(VOID);                                                                   // 初始化场景管理器
		virtual VOID Free(VOID);                                                                   // 释放场景管理器

	private:
		virtual VOID FreeNodes(VOID);                                                              // 释放结点集合
		virtual VOID FreeEntities(EntityMap &entities);                                            // 释放实体集合

	public:
		virtual BOOL LoadFromFile(const CHAR *szFileName);                                         // 从文件加载资源
		virtual BOOL LoadFromZip(const CHAR *szZipName, const CHAR *szFileName);                   // 从压缩包加载资源
		virtual BOOL LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName);                      // 从压缩包文件加载数据流
		virtual BOOL LoadFromStream(CStream *pStream);                                             // 从数据流加载资源

	protected:
		virtual BOOL CreateRenderScene(VOID);                                                      // 创建渲染场景
		virtual BOOL CreatePhysicsScene(VOID);                                                     // 创建物理场景

	public:
		virtual VOID SetVisibleFlags(DWORD dwFlags);                                               // 设置可见性标识
		virtual DWORD GetVisibleFlags(VOID) const;                                                 // 获得可见性标识

	public:
		virtual CSceneNode* GetRootNode(VOID) const;                                               // 获得根节点

		virtual CSceneNode* GetMainCameraNode(VOID) const;                                         // 获得主相机节点
		virtual CEntityCamera* GetMainCameraEntity(VOID) const;                                    // 获得主相机实体

		virtual CSceneNode* GetMainShadowNode(VOID) const;                                         // 获得主阴影节点
		virtual CEntityShadow* GetMainShadowEntity(VOID) const;                                    // 获得主阴影实体

	public:
		virtual CPhysicsManager& GetPhysics(VOID);                                                 // 获得物理引擎

	public:
		virtual CSceneNode* CreateNode(const CHAR *szName);                                        // 创建节点
		virtual CSceneNode* CreateNode(DWORD dwName);                                              // 创建节点

		virtual VOID DestroyNode(CSceneNode *pNode);                                               // 销毁节点

		virtual CSceneNode* GetNode(const CHAR *szName) const;                                     // 获得节点
		virtual CSceneNode* GetNode(DWORD dwName) const;                                           // 获得节点

	public:
		virtual CEntity* CreateEntity(const CHAR *szName, CEntity::TYPE type);                     // 创建实体
		virtual CEntity* CreateEntity(DWORD dwName, CEntity::TYPE type);                           // 创建实体

		virtual VOID DestroyEntity(CEntity *pEntity);                                              // 销毁实体

		virtual CEntity* GetEntity(const CHAR *szName, CEntity::TYPE type) const;                  // 获得实体
		virtual CEntity* GetEntity(DWORD dwName, CEntity::TYPE type) const;                        // 获得实体

	public:
		virtual BOOL IsOccluded(INT index) const;                                                  // 遮挡判断
		virtual BOOL GetLightParams(DWORD dwChannel, const VEC3 *position, VEC4 *direction,        // 获得光照参数
			VEC3 *ambient = NULL, 
			VEC3 *diffuse = NULL, 
			VEC3 *specular = NULL, 
			VEC3 *rim = NULL, 
			VEC3 *skyLower = NULL, 
			VEC3 *skyUpper = NULL, 
			VEC3 *indirectUp = NULL, 
			VEC3 *indirectDown = NULL, 
			VEC3 *indirectLeft = NULL, 
			VEC3 *indirectRight = NULL, 
			VEC3 *indirectFront = NULL,
			VEC3 *indirectBack = NULL) const;

	public:
		virtual VOID AddSceneRenderQuene(const CEntityCamera *pCamera, CEntityRenderable *pEntity);// 添加场景渲染队列
		virtual VOID AddSceneRenderQuene(const CEntityCamera *pCamera, CEntityRenderable *pEntity, CMaterial *pMaterial); // 添加场景渲染队列
		virtual VOID AddLightRenderQuene(const CEntityCamera *pCamera, CEntityLight *pEntity);     // 添加光源渲染队列

	public:
		virtual VOID Update(FLOAT deltaTime);                                                      // 更新
		virtual VOID Render(CEntityCamera *pCamera = NULL);                                        // 渲染


		// 属性
	protected:
		DWORD m_dwVisibleFlags;                                                                    // 可见标识

	protected:
		EntityMap m_meshs;                                                                         // 网格集合
		EntityMap m_skins;                                                                         // 蒙皮集合
		EntityMap m_effects;                                                                       // 特效集合
		EntityMap m_halos;                                                                         // 光晕集合
		EntityMap m_lights;                                                                        // 光源集合

		SceneNodeMap m_nodes;                                                                      // 节点集合

	protected:
		CSceneNode *m_pRootNode;                                                                   // 根节点
		CSceneNode *m_pMainCameraNode;                                                             // 主相机节点
		CSceneNode *m_pMainShadowNode;                                                             // 主阴影节点
		CEntityCamera *m_pMainCameraEntity;                                                        // 主相机实体
		CEntityShadow *m_pMainShadowEntity;                                                        // 主阴影实体

	protected:
		CLightGrid *m_pLightGrid;                                                                  // 光照网格
		COctreeCulling *m_pOctreeCulling;                                                          // 八叉树剔除
		COcclusionCulling *m_pOcclusionCulling;                                                    // 遮挡剔除

	protected:
		CPhysicsManager m_physics;                                                                 // 物理引擎

	protected:
		CStream m_stream;                                                                          // 数据流
	};

}

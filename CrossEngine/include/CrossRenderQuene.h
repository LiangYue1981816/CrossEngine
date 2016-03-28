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

	class _CrossExport CMatPass;
	class _CrossExport CMaterial;
	class _CrossExport CEntityLight;
	class _CrossExport CEntityCamera;
	class _CrossExport CEntityShadow;
	class _CrossExport CEntityRenderable;
	class _CrossExport CRenderQuene
	{
		friend class CRenderSystem;


		// 数据结构
	public:
		typedef std::vector<const CEntityLight*> LightQuene;                                       // 光源渲染队列
		typedef std::vector<const CEntityRenderable*> RenderQuene;                                 // 物体渲染队列
		typedef std::map<UINT64, const CMatPass*> PassQuene;                                       // 通路渲染队列, 说明: 基于通路评估值排序
		typedef std::map<const CMaterial*, RenderQuene> MaterialQuene;                             // 材质渲染队列, 说明: 基于材质排序

	public:
		typedef struct {
			BOOL bRenderQuery;                                                                     // 渲染查询状态
			BOOL bRenderOcclude;                                                                   // 渲染遮挡状态
			LightQuene lights;                                                                     // 光源渲染队列
			RenderQuene occludes;                                                                  // 遮挡渲染队列
			MaterialQuene materials;                                                               // 材质渲染队列
		} SCENE_RENDER_QUENE;

		typedef struct {
			MATRIX4 mtxShadowProjection;                                                           // 阴影投影矩阵
			MaterialQuene casts;                                                                   // 产生阴影渲染队列
			MaterialQuene receives;                                                                // 接收阴影渲染队列
		} SHADOW_RENDER_QUENE;

		// 注意: 
		// 1. 多相机多阴影支持
		// 2. 场景渲染队列和阴影渲染队列彼此独立
		typedef std::map<const CEntityCamera*, SCENE_RENDER_QUENE> SceneRenderQueneMap;            // 场景渲染队列集合
		typedef std::map<const CEntityShadow*, SHADOW_RENDER_QUENE> ShadowRenderQueneMap;          // 阴影渲染队列集合


		// 构造/析构函数
	protected:
		CRenderQuene(VOID);
		virtual ~CRenderQuene(VOID);


		// 方法
	public:
		VOID AddSceneRenderQuene(const CEntityCamera *pCamera, const CMaterial *pMaterial, CEntityRenderable *pEntity); // 添加场景渲染队列
		VOID AddSceneLightRenderQuene(const CEntityCamera *pCamera, CEntityLight *pEntity);        // 添加场景光源渲染队列
		VOID AddSceneOccludeRenderQuene(const CEntityCamera *pCamera, CEntityRenderable *pEntity); // 添加场景遮挡渲染队列

		VOID AddCastShadowRenderQuene(const CEntityShadow *pShadow, const CMaterial *pMaterial, CEntityRenderable *pEntity); // 添加产生阴影渲染队列
		VOID AddReceiveShadowRenderQuene(const CEntityShadow *pShadow, const CMaterial *pMaterial, CEntityRenderable *pEntity); // 添加接收阴影渲染队列

		VOID ClearRenderQuene(VOID);                                                               // 清空渲染队列

	public:
		VOID RenderOcclude(const CEntityCamera *pCamera);                                          // 渲染遮挡
		VOID RenderQuery(const CEntityCamera *pCamera);                                            // 渲染查询

		VOID RenderOpaque(const CEntityCamera *pCamera, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID); // 渲染不透明物体
		VOID RenderTransparence(const CEntityCamera *pCamera, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID); // 渲染透明物体

		VOID RenderReceiveShadow(const CEntityCamera *pCamera, const CEntityShadow *pShadow, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID); // 渲染接收阴影
		VOID RenderCastShadow(const CEntityShadow *pShadow, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID); // 渲染产生阴影

		VOID RenderLighting(const CEntityCamera *pCamera);                                         // 渲染光照

	protected:
		VOID RenderOcclude(const RenderQuene &occludes);                                           // 渲染遮挡
		VOID RenderQuery(const MaterialQuene &materials, const LightQuene &lights);                // 渲染查询
		VOID RenderLight(const LightQuene &lights, BOOL bOccludeQuery);                            // 渲染灯光
		VOID Render(const MaterialQuene &materials, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID, // 渲染
			BOOL bOccludeQuery,                                                                    // 使用遮挡查询
			BOOL bCastShadow,                                                                      // 产生阴影
			BOOL bReceiveShadow,                                                                   // 接收阴影
			BOOL bTransparence,                                                                    // 渲染透明物体
			const MATRIX4 *mtxShadowProjection);                                                   // 阴影投影矩阵


		// 属性
	protected:
		SceneRenderQueneMap m_sceneRenderQuenes;                                                   // 场景渲染队列集合
		ShadowRenderQueneMap m_shadowRenderQuenes;                                                 // 阴影渲染队列集合
	};

}

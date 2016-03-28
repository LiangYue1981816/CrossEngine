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

	class _CrossExport CEntityRenderable : public CEntity
	{
		friend class CSceneManager;


		// 构造/析构函数
	protected:
		CEntityRenderable(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntityRenderable(VOID);


		// 方法
	public:
		virtual VOID SetVisibleFlags(DWORD dwFlags);                                               // 设置可见性标识
		virtual VOID SetVisibleDistance(FLOAT distance);                                           // 设置可见距离

		virtual DWORD GetVisibleFlags(VOID) const;                                                 // 获得可见性标识
		virtual FLOAT GetVisibleDistance(VOID) const;                                              // 获得可见距离

		virtual BOOL IsRenderable(const CEntityCamera *pCamera) const;                             // 获得可渲染

	public:
		virtual VOID SetLightChannel(DWORD dwChannel);                                             // 设置光照通道
		virtual DWORD GetLightChannel(VOID) const;                                                 // 后的光照通道

	public:
		virtual VOID SetIndexOcclude(INT index);                                                   // 设置遮挡索引
		virtual INT GetIndexOcclude(VOID) const;                                                   // 获得遮挡索引

		virtual VOID SetEnableOcclude(BOOL bEnable);                                               // 设置启用遮挡
		virtual BOOL IsEnableOcclude(VOID) const;                                                  // 获得启用遮挡

	public:
		virtual VOID SetEnableLightMapCastShadow(BOOL bEnable);                                    // 设置启用光照图产生阴影
		virtual BOOL IsEnableLightMapCastShadow(VOID) const;                                       // 获得启用光照图产生阴影

		virtual VOID SetEnableLightMapReceiveShadow(BOOL bEnable);                                 // 设置启用光照图接收阴影
		virtual BOOL IsEnableLightMapReceiveShadow(VOID) const;                                    // 获得启用光照图接收阴影

	public:
		virtual VOID SetShadowMapCastShadowDistance(FLOAT distance);                               // 设置阴影图产生阴影距离
		virtual FLOAT GetShadowMapCastShadowDistance(VOID) const;                                  // 获得阴影图产生阴影距离

		virtual VOID SetEnableShadowMapCastShadow(BOOL bEnable);                                   // 设置启用阴影图产生阴影
		virtual BOOL IsEnableShadowMapCastShadow(VOID) const;                                      // 获得启用阴影图产生阴影
		virtual BOOL IsShadowMapCastShadow(const CEntityCamera *pCamera) const;                    // 获得阴影图产生阴影

		virtual VOID SetEnableShadowMapReceiveShadow(BOOL bEnable);                                // 设置启用阴影图接收阴影
		virtual BOOL IsEnableShadowMapReceiveShadow(VOID) const;                                   // 获得启用阴影图接收阴影

	public:
		virtual const AABB* GetAABB(VOID);                                                         // 获得包围盒
		virtual const CMaterialPtr& GetMaterial(VOID) const;                                       // 获得材质

	public:
		virtual VOID UpdateRenderQuene(const CEntityCamera *pCamera);                              // 更新渲染队列
		virtual VOID Render(const CMaterial *pMaterial) const;                                     // 渲染
		virtual VOID RenderQuery(VOID) const;                                                      // 渲染查询
		virtual VOID RenderOcclude(VOID) const;                                                    // 渲染遮挡

	public:
		virtual UINT GetQuery(VOID) const;                                                         // 获得遮挡查询


		// 属性
	protected:
		INT m_indexOcclude;                                                                        // 遮挡索引
		                                                                                           // 说明: 遮挡剔除时用于查询是否被遮挡用
	protected:
		DWORD m_dwLightChannel;                                                                    // 光照通道

	protected:
		AABB m_aabbLocal;                                                                          // 包围盒
		AABB m_aabbWorld;                                                                          // 包围盒

	protected:
		DWORD m_dwVisibleFlags;                                                                    // 可见标识
		FLOAT m_visibleDistance;                                                                   // 可见距离
		FLOAT m_visibleDistance2;                                                                  // 可见距离平方

	protected:
		BOOL m_bEnableOcclude;                                                                     // 启用遮挡

	protected:
		BOOL m_bEnableLightMapCastShadow;                                                          // 启用光照图产生阴影
		BOOL m_bEnableLightMapReceiveShadow;                                                       // 启用光照图接收阴影

	protected:
		FLOAT m_shadowMapCastShadowDistance;                                                       // 阴影图产生阴影距离
		FLOAT m_shadowMapCastShadowDistance2;                                                      // 阴影图产生阴影距离平方

		BOOL m_bEnableShodowMapCastShadow;                                                         // 启用阴影图产生阴影
		BOOL m_bEnableShodowMapReceiveShadow;                                                      // 启用阴影图接收阴影

	protected:
		UINT m_query;                                                                              // 遮挡查询

		// 渲染链表
	public:
		CEntityRenderable *pRenderNext;                                                            // 实体渲染链表
	};

}

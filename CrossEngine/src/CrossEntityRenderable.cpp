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

	CEntityRenderable::CEntityRenderable(DWORD dwName, CSceneManager *pSceneManager)
		: CEntity(dwName, pSceneManager)
		, m_indexOcclude(-1)

		, m_dwLightChannel(0xffffffff)

		, m_dwVisibleFlags(0xffffffff)
		, m_visibleDistance(FLT_MAX)
		, m_visibleDistance2(FLT_MAX)

		, m_bEnableOcclude(FALSE)

		, m_bEnableLightMapCastShadow(FALSE)
		, m_bEnableLightMapReceiveShadow(FALSE)

		, m_shadowMapCastShadowDistance(FLT_MAX)
		, m_shadowMapCastShadowDistance2(FLT_MAX)
		, m_bEnableShodowMapCastShadow(FALSE)
		, m_bEnableShodowMapReceiveShadow(FALSE)

		, m_query(0)

		, pRenderNext(NULL)
	{
		AABBZero(&m_aabbLocal);
		AABBZero(&m_aabbWorld);

		m_query = Renderer()->CreateQuery();
	}

	CEntityRenderable::~CEntityRenderable(VOID)
	{
		Renderer()->DestroyQuery(&m_query);
	}

	//
	// 设置可见性标识
	//
	VOID CEntityRenderable::SetVisibleFlags(DWORD dwFlags)
	{
		m_dwVisibleFlags = dwFlags;
	}

	//
	// 设置可见距离
	//
	VOID CEntityRenderable::SetVisibleDistance(FLOAT distance)
	{
		m_visibleDistance = distance < FLT_MAX ? distance : FLT_MAX;
		m_visibleDistance2 = distance < FLT_MAX ? distance * distance : FLT_MAX;
	}

	//
	// 获得可见性标识
	//
	DWORD CEntityRenderable::GetVisibleFlags(VOID) const
	{
		return m_dwVisibleFlags;
	}

	//
	// 获得可见距离
	//
	FLOAT CEntityRenderable::GetVisibleDistance(VOID) const
	{
		return m_visibleDistance;
	}

	//
	// 获得可渲染
	//
	BOOL CEntityRenderable::IsRenderable(const CEntityCamera *pCamera) const
	{
		//
		// 1. 遮挡剔除检查
		//
		if (m_pSceneManager->IsOccluded(m_indexOcclude)) {
			return FALSE;
		}

		//
		// 2. 可见标识检查
		//
		if ((m_pSceneManager->GetVisibleFlags() & m_dwVisibleFlags) == 0) {
			return FALSE;
		}

		//
		// 3. 可见距离检查
		//
		if (Vec3Length2(pCamera->GetPosition(), &m_pSceneNode->m_worldPosition) > m_visibleDistance2) {
			return FALSE;
		}

		return TRUE;
	}

	//
	// 设置光照通道
	//
	VOID CEntityRenderable::SetLightChannel(DWORD dwChannel)
	{
		m_dwLightChannel = dwChannel;
	}

	//
	// 后的光照通道
	//
	DWORD CEntityRenderable::GetLightChannel(VOID) const
	{
		return m_dwLightChannel;
	}

	//
	// 设置遮挡索引
	//
	VOID CEntityRenderable::SetIndexOcclude(INT index)
	{
		m_indexOcclude = index;
	}

	//
	// 获得遮挡索引
	//
	INT CEntityRenderable::GetIndexOcclude(VOID) const
	{
		return m_indexOcclude;
	}

	//
	// 设置启用遮挡
	//
	VOID CEntityRenderable::SetEnableOcclude(BOOL bEnable)
	{
		m_bEnableOcclude = bEnable;
	}

	//
	// 获得启用遮挡
	//
	BOOL CEntityRenderable::IsEnableOcclude(VOID) const
	{
		return m_bEnableOcclude;
	}

	//
	// 设置启用光照图产生阴影
	//
	VOID CEntityRenderable::SetEnableLightMapCastShadow(BOOL bEnable)
	{
		m_bEnableLightMapCastShadow = bEnable;
	}

	//
	// 获得启用光照图产生阴影
	//
	BOOL CEntityRenderable::IsEnableLightMapCastShadow(VOID) const
	{
		return m_bEnableLightMapCastShadow;
	}

	//
	// 设置启用光照图接收阴影
	//
	VOID CEntityRenderable::SetEnableLightMapReceiveShadow(BOOL bEnable)
	{
		m_bEnableLightMapReceiveShadow = bEnable;
	}

	//
	// 获得启用光照图接收阴影
	//
	BOOL CEntityRenderable::IsEnableLightMapReceiveShadow(VOID) const
	{
		return m_bEnableLightMapReceiveShadow;
	}

	//
	// 设置阴影图产生阴影距离
	//
	VOID CEntityRenderable::SetShadowMapCastShadowDistance(FLOAT distance)
	{
		m_shadowMapCastShadowDistance = distance < FLT_MAX ? distance : FLT_MAX;
		m_shadowMapCastShadowDistance2 = distance < FLT_MAX ? distance * distance : FLT_MAX;
	}

	//
	// 获得阴影图产生阴影距离
	//
	FLOAT CEntityRenderable::GetShadowMapCastShadowDistance(VOID) const
	{
		return m_shadowMapCastShadowDistance;
	}

	//
	// 设置启用阴影图产生阴影
	//
	VOID CEntityRenderable::SetEnableShadowMapCastShadow(BOOL bEnable)
	{
		m_bEnableShodowMapCastShadow = bEnable;
	}

	//
	// 获得启用阴影图产生阴影
	//
	BOOL CEntityRenderable::IsEnableShadowMapCastShadow(VOID) const
	{
		return m_bEnableShodowMapCastShadow;
	}

	//
	// 获得阴影图产生阴影
	//
	BOOL CEntityRenderable::IsShadowMapCastShadow(const CEntityCamera *pCamera) const
	{
		//
		// 1. 产生阴影状态检查
		//
		if (m_bEnableShodowMapCastShadow == FALSE) {
			return FALSE;
		}

		//
		// 2. 产生阴影距离检查
		//
		if (Vec3Length2(pCamera->GetPosition(), &m_pSceneNode->m_worldPosition) > m_shadowMapCastShadowDistance2) {
			return FALSE;
		}

		return TRUE;
	}

	//
	// 设置启用阴影图接收阴影
	//
	VOID CEntityRenderable::SetEnableShadowMapReceiveShadow(BOOL bEnable)
	{
		m_bEnableShodowMapReceiveShadow = bEnable;
	}

	//
	// 获得启用阴影图接收阴影
	//
	BOOL CEntityRenderable::IsEnableShadowMapReceiveShadow(VOID) const
	{
		return m_bEnableShodowMapReceiveShadow;
	}

	//
	// 获得包围盒
	//
	const AABB* CEntityRenderable::GetAABB(VOID)
	{
		ASSERT(m_pSceneNode);
		return AABBTransform(&m_aabbWorld, &m_aabbLocal, &m_pSceneNode->m_mtxWorld);
	}

	//
	// 获得材质
	//
	const CMaterialPtr& CEntityRenderable::GetMaterial(VOID) const
	{
		static const CMaterialPtr ptrMaterial;
		return ptrMaterial;
	}

	//
	// 更新渲染队列
	//
	VOID CEntityRenderable::UpdateRenderQuene(const CEntityCamera *pCamera)
	{
		//
		// 1. 可渲染检查
		//
		if (IsRenderable(pCamera) == FALSE) {
			return;
		}

		//
		// 2. 材质检查
		//
		const CMaterialPtr& ptrMaterial = GetMaterial();

		if (ptrMaterial.IsNull() || ptrMaterial->IsValid() == FALSE) {
			return;
		}

		//
		// 3. 添加渲染队列
		//
		m_pSceneManager->AddSceneRenderQuene(pCamera, this);
	}

	//
	// 渲染
	//
	VOID CEntityRenderable::Render(const CMaterial *pMaterial) const
	{

	}

	//
	// 渲染查询
	//
	VOID CEntityRenderable::RenderQuery(VOID) const
	{
		Renderer()->RenderQuery(m_query, &m_aabbWorld);
	}

	//
	// 渲染遮挡
	//
	VOID CEntityRenderable::RenderOcclude(VOID) const
	{

	}

	//
	// 获得遮挡查询
	//
	UINT CEntityRenderable::GetQuery(VOID) const
	{
		return m_query;
	}

}

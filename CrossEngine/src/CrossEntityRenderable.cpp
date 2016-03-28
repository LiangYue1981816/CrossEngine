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
	// ���ÿɼ��Ա�ʶ
	//
	VOID CEntityRenderable::SetVisibleFlags(DWORD dwFlags)
	{
		m_dwVisibleFlags = dwFlags;
	}

	//
	// ���ÿɼ�����
	//
	VOID CEntityRenderable::SetVisibleDistance(FLOAT distance)
	{
		m_visibleDistance = distance < FLT_MAX ? distance : FLT_MAX;
		m_visibleDistance2 = distance < FLT_MAX ? distance * distance : FLT_MAX;
	}

	//
	// ��ÿɼ��Ա�ʶ
	//
	DWORD CEntityRenderable::GetVisibleFlags(VOID) const
	{
		return m_dwVisibleFlags;
	}

	//
	// ��ÿɼ�����
	//
	FLOAT CEntityRenderable::GetVisibleDistance(VOID) const
	{
		return m_visibleDistance;
	}

	//
	// ��ÿ���Ⱦ
	//
	BOOL CEntityRenderable::IsRenderable(const CEntityCamera *pCamera) const
	{
		//
		// 1. �ڵ��޳����
		//
		if (m_pSceneManager->IsOccluded(m_indexOcclude)) {
			return FALSE;
		}

		//
		// 2. �ɼ���ʶ���
		//
		if ((m_pSceneManager->GetVisibleFlags() & m_dwVisibleFlags) == 0) {
			return FALSE;
		}

		//
		// 3. �ɼ�������
		//
		if (Vec3Length2(pCamera->GetPosition(), &m_pSceneNode->m_worldPosition) > m_visibleDistance2) {
			return FALSE;
		}

		return TRUE;
	}

	//
	// ���ù���ͨ��
	//
	VOID CEntityRenderable::SetLightChannel(DWORD dwChannel)
	{
		m_dwLightChannel = dwChannel;
	}

	//
	// ��Ĺ���ͨ��
	//
	DWORD CEntityRenderable::GetLightChannel(VOID) const
	{
		return m_dwLightChannel;
	}

	//
	// �����ڵ�����
	//
	VOID CEntityRenderable::SetIndexOcclude(INT index)
	{
		m_indexOcclude = index;
	}

	//
	// ����ڵ�����
	//
	INT CEntityRenderable::GetIndexOcclude(VOID) const
	{
		return m_indexOcclude;
	}

	//
	// ���������ڵ�
	//
	VOID CEntityRenderable::SetEnableOcclude(BOOL bEnable)
	{
		m_bEnableOcclude = bEnable;
	}

	//
	// ��������ڵ�
	//
	BOOL CEntityRenderable::IsEnableOcclude(VOID) const
	{
		return m_bEnableOcclude;
	}

	//
	// �������ù���ͼ������Ӱ
	//
	VOID CEntityRenderable::SetEnableLightMapCastShadow(BOOL bEnable)
	{
		m_bEnableLightMapCastShadow = bEnable;
	}

	//
	// ������ù���ͼ������Ӱ
	//
	BOOL CEntityRenderable::IsEnableLightMapCastShadow(VOID) const
	{
		return m_bEnableLightMapCastShadow;
	}

	//
	// �������ù���ͼ������Ӱ
	//
	VOID CEntityRenderable::SetEnableLightMapReceiveShadow(BOOL bEnable)
	{
		m_bEnableLightMapReceiveShadow = bEnable;
	}

	//
	// ������ù���ͼ������Ӱ
	//
	BOOL CEntityRenderable::IsEnableLightMapReceiveShadow(VOID) const
	{
		return m_bEnableLightMapReceiveShadow;
	}

	//
	// ������Ӱͼ������Ӱ����
	//
	VOID CEntityRenderable::SetShadowMapCastShadowDistance(FLOAT distance)
	{
		m_shadowMapCastShadowDistance = distance < FLT_MAX ? distance : FLT_MAX;
		m_shadowMapCastShadowDistance2 = distance < FLT_MAX ? distance * distance : FLT_MAX;
	}

	//
	// �����Ӱͼ������Ӱ����
	//
	FLOAT CEntityRenderable::GetShadowMapCastShadowDistance(VOID) const
	{
		return m_shadowMapCastShadowDistance;
	}

	//
	// ����������Ӱͼ������Ӱ
	//
	VOID CEntityRenderable::SetEnableShadowMapCastShadow(BOOL bEnable)
	{
		m_bEnableShodowMapCastShadow = bEnable;
	}

	//
	// ���������Ӱͼ������Ӱ
	//
	BOOL CEntityRenderable::IsEnableShadowMapCastShadow(VOID) const
	{
		return m_bEnableShodowMapCastShadow;
	}

	//
	// �����Ӱͼ������Ӱ
	//
	BOOL CEntityRenderable::IsShadowMapCastShadow(const CEntityCamera *pCamera) const
	{
		//
		// 1. ������Ӱ״̬���
		//
		if (m_bEnableShodowMapCastShadow == FALSE) {
			return FALSE;
		}

		//
		// 2. ������Ӱ������
		//
		if (Vec3Length2(pCamera->GetPosition(), &m_pSceneNode->m_worldPosition) > m_shadowMapCastShadowDistance2) {
			return FALSE;
		}

		return TRUE;
	}

	//
	// ����������Ӱͼ������Ӱ
	//
	VOID CEntityRenderable::SetEnableShadowMapReceiveShadow(BOOL bEnable)
	{
		m_bEnableShodowMapReceiveShadow = bEnable;
	}

	//
	// ���������Ӱͼ������Ӱ
	//
	BOOL CEntityRenderable::IsEnableShadowMapReceiveShadow(VOID) const
	{
		return m_bEnableShodowMapReceiveShadow;
	}

	//
	// ��ð�Χ��
	//
	const AABB* CEntityRenderable::GetAABB(VOID)
	{
		ASSERT(m_pSceneNode);
		return AABBTransform(&m_aabbWorld, &m_aabbLocal, &m_pSceneNode->m_mtxWorld);
	}

	//
	// ��ò���
	//
	const CMaterialPtr& CEntityRenderable::GetMaterial(VOID) const
	{
		static const CMaterialPtr ptrMaterial;
		return ptrMaterial;
	}

	//
	// ������Ⱦ����
	//
	VOID CEntityRenderable::UpdateRenderQuene(const CEntityCamera *pCamera)
	{
		//
		// 1. ����Ⱦ���
		//
		if (IsRenderable(pCamera) == FALSE) {
			return;
		}

		//
		// 2. ���ʼ��
		//
		const CMaterialPtr& ptrMaterial = GetMaterial();

		if (ptrMaterial.IsNull() || ptrMaterial->IsValid() == FALSE) {
			return;
		}

		//
		// 3. �����Ⱦ����
		//
		m_pSceneManager->AddSceneRenderQuene(pCamera, this);
	}

	//
	// ��Ⱦ
	//
	VOID CEntityRenderable::Render(const CMaterial *pMaterial) const
	{

	}

	//
	// ��Ⱦ��ѯ
	//
	VOID CEntityRenderable::RenderQuery(VOID) const
	{
		Renderer()->RenderQuery(m_query, &m_aabbWorld);
	}

	//
	// ��Ⱦ�ڵ�
	//
	VOID CEntityRenderable::RenderOcclude(VOID) const
	{

	}

	//
	// ����ڵ���ѯ
	//
	UINT CEntityRenderable::GetQuery(VOID) const
	{
		return m_query;
	}

}

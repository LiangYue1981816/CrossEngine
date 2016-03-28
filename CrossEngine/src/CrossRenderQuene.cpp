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

	CRenderQuene::CRenderQuene(VOID)
	{

	}

	CRenderQuene::~CRenderQuene(VOID)
	{
		ClearRenderQuene();
	}

	//
	// ��ӳ�����Ⱦ����
	//
	VOID CRenderQuene::AddSceneRenderQuene(const CEntityCamera *pCamera, const CMaterial *pMaterial, CEntityRenderable *pEntity)
	{
		ASSERT(pCamera);
		ASSERT(pMaterial);
		ASSERT(pEntity);

		m_sceneRenderQuenes[pCamera].bRenderQuery = FALSE;
		m_sceneRenderQuenes[pCamera].materials[pMaterial].push_back(pEntity);
	}

	//
	// ��ӳ�����Դ��Ⱦ����
	//
	VOID CRenderQuene::AddSceneLightRenderQuene(const CEntityCamera *pCamera, CEntityLight *pEntity)
	{
		ASSERT(pCamera);
		ASSERT(pEntity);

		m_sceneRenderQuenes[pCamera].bRenderQuery = FALSE;
		m_sceneRenderQuenes[pCamera].lights.push_back(pEntity);
	}

	//
	// ��ӳ����ڵ���Ⱦ����
	//
	VOID CRenderQuene::AddSceneOccludeRenderQuene(const CEntityCamera *pCamera, CEntityRenderable *pEntity)
	{
		ASSERT(pCamera);
		ASSERT(pEntity);
		ASSERT(pEntity->IsEnableOcclude());

		m_sceneRenderQuenes[pCamera].bRenderOcclude = FALSE;
		m_sceneRenderQuenes[pCamera].occludes.push_back(pEntity);
	}

	//
	// ��Ӳ�����Ӱ��Ⱦ����
	//
	VOID CRenderQuene::AddCastShadowRenderQuene(const CEntityShadow *pShadow, const CMaterial *pMaterial, CEntityRenderable *pEntity)
	{
		ASSERT(pShadow);
		ASSERT(pMaterial);
		ASSERT(pEntity);
		ASSERT(pEntity->IsEnableShadowMapCastShadow());

		m_shadowRenderQuenes[pShadow].casts[pMaterial].push_back(pEntity);
	}

	//
	// ��ӽ�����Ӱ��Ⱦ����
	//
	VOID CRenderQuene::AddReceiveShadowRenderQuene(const CEntityShadow *pShadow, const CMaterial *pMaterial, CEntityRenderable *pEntity)
	{
		ASSERT(pShadow);
		ASSERT(pMaterial);
		ASSERT(pEntity);
		ASSERT(pEntity->IsEnableShadowMapReceiveShadow());

		m_shadowRenderQuenes[pShadow].receives[pMaterial].push_back(pEntity);
	}

	//
	// �����Ⱦ����
	//
	VOID CRenderQuene::ClearRenderQuene(VOID)
	{
		m_sceneRenderQuenes.clear();
		m_shadowRenderQuenes.clear();
	}

	//
	// ��Ⱦ�ڵ�
	//
	VOID CRenderQuene::RenderOcclude(const CEntityCamera *pCamera)
	{
		ASSERT(pCamera);

		//
		// 1. ���ҳ�����Ⱦ���в�����Ƿ��Ѿ���Ⱦ
		//
		SceneRenderQueneMap::iterator itSceneRenderQuene = m_sceneRenderQuenes.find(pCamera);
		if (itSceneRenderQuene == m_sceneRenderQuenes.end()) return;
		if (itSceneRenderQuene->second.bRenderOcclude) return;

		//
		// 2. �������
		//
		Renderer()->SetPerspective(pCamera->GetFovy(), pCamera->GetAspect(), pCamera->GetZNear(), pCamera->GetZFar());
		Renderer()->SetLookAt(*pCamera->GetPosition(), *pCamera->GetForwardDirection(), *pCamera->GetUpDirection());

		//
		// 3. ��Ⱦ
		//
		RenderOcclude(itSceneRenderQuene->second.occludes);

		//
		// 4. �����Ѿ���Ⱦ
		//
		itSceneRenderQuene->second.bRenderOcclude = TRUE;
	}

	//
	// ��Ⱦ��ѯ
	//
	VOID CRenderQuene::RenderQuery(const CEntityCamera *pCamera)
	{
		ASSERT(pCamera);

		//
		// 1. ���ҳ�����Ⱦ���в�����Ƿ��Ѿ���Ⱦ
		//
		SceneRenderQueneMap::iterator itSceneRenderQuene = m_sceneRenderQuenes.find(pCamera);
		if (itSceneRenderQuene == m_sceneRenderQuenes.end()) return;
		if (itSceneRenderQuene->second.bRenderQuery) return;

		//
		// 2. �������
		//
		Renderer()->SetPerspective(pCamera->GetFovy(), pCamera->GetAspect(), pCamera->GetZNear(), pCamera->GetZFar());
		Renderer()->SetLookAt(*pCamera->GetPosition(), *pCamera->GetForwardDirection(), *pCamera->GetUpDirection());

		//
		// 3. ��Ⱦ
		//
		RenderQuery(itSceneRenderQuene->second.materials, itSceneRenderQuene->second.lights);

		//
		// 4. �����Ѿ���Ⱦ
		//
		itSceneRenderQuene->second.bRenderQuery = TRUE;
	}

	//
	// ��Ⱦ��͸������
	//
	VOID CRenderQuene::RenderOpaque(const CEntityCamera *pCamera, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID)
	{
		ASSERT(pCamera);

		//
		// 1. ���ҳ�����Ⱦ����
		//
		SceneRenderQueneMap::iterator itSceneRenderQuene = m_sceneRenderQuenes.find(pCamera);
		if (itSceneRenderQuene == m_sceneRenderQuenes.end()) return;

		//
		// 2. �������
		//
		Renderer()->SetPerspective(pCamera->GetFovy(), pCamera->GetAspect(), pCamera->GetZNear(), pCamera->GetZFar());
		Renderer()->SetLookAt(*pCamera->GetPosition(), *pCamera->GetForwardDirection(), *pCamera->GetUpDirection());

		//
		// 3. ��Ⱦ
		//
		BOOL bOccludeQuery = itSceneRenderQuene->second.bRenderQuery;
		BOOL bCastShadow = FALSE;
		BOOL bReceiveShadow = FALSE;
		BOOL bTransparence = FALSE;
		const MATRIX4 *mtxShadowProjection = NULL;

		Render(itSceneRenderQuene->second.materials, dwSolution, dwPassGroup, dwBeginRenderID, dwEndRenderID, 
			bOccludeQuery, bCastShadow, bReceiveShadow, bTransparence, mtxShadowProjection);
	}

	//
	// ��Ⱦ͸������
	//
	VOID CRenderQuene::RenderTransparence(const CEntityCamera *pCamera, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID)
	{
		ASSERT(pCamera);

		//
		// 1. ���ҳ�����Ⱦ����
		//
		SceneRenderQueneMap::iterator itSceneRenderQuene = m_sceneRenderQuenes.find(pCamera);
		if (itSceneRenderQuene == m_sceneRenderQuenes.end()) return;

		//
		// 2. �������
		//
		Renderer()->SetPerspective(pCamera->GetFovy(), pCamera->GetAspect(), pCamera->GetZNear(), pCamera->GetZFar());
		Renderer()->SetLookAt(*pCamera->GetPosition(), *pCamera->GetForwardDirection(), *pCamera->GetUpDirection());

		//
		// 3. ��Ⱦ
		//
		BOOL bOccludeQuery = itSceneRenderQuene->second.bRenderQuery;
		BOOL bCastShadow = FALSE;
		BOOL bReceiveShadow = FALSE;
		BOOL bTransparence = TRUE;
		const MATRIX4 *mtxShadowProjection = NULL;

		Render(itSceneRenderQuene->second.materials, dwSolution, dwPassGroup, dwBeginRenderID, dwEndRenderID, 
			bOccludeQuery, bCastShadow, bReceiveShadow, bTransparence, mtxShadowProjection);
	}

	//
	// ��Ⱦ������Ӱ
	//
	VOID CRenderQuene::RenderReceiveShadow(const CEntityCamera *pCamera, const CEntityShadow *pShadow, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID)
	{
		ASSERT(pCamera);
		ASSERT(pShadow);

		//
		// 1. ���ҳ�����Ⱦ����
		//
		SceneRenderQueneMap::iterator itSceneRenderQuene = m_sceneRenderQuenes.find(pCamera);
		if (itSceneRenderQuene == m_sceneRenderQuenes.end()) return;

		//
		// 2. ������Ӱ��Ⱦ����
		//
		ShadowRenderQueneMap::iterator itShadowRenderQuene = m_shadowRenderQuenes.find(pShadow);
		if (itShadowRenderQuene == m_shadowRenderQuenes.end()) return;

		//
		// 3. �������
		//
		Renderer()->SetPerspective(pCamera->GetFovy(), pCamera->GetAspect(), pCamera->GetZNear(), pCamera->GetZFar());
		Renderer()->SetLookAt(*pCamera->GetPosition(), *pCamera->GetForwardDirection(), *pCamera->GetUpDirection());

		//
		// 4. ��Ⱦ
		//
		BOOL bOccludeQuery = itSceneRenderQuene->second.bRenderQuery;
		BOOL bCastShadow = FALSE;
		BOOL bReceiveShadow = TRUE;
		BOOL bTransparence = FALSE;
		const MATRIX4 *mtxShadowProjection = &itShadowRenderQuene->second.mtxShadowProjection;

		Render(itShadowRenderQuene->second.receives, dwSolution, dwPassGroup, dwBeginRenderID, dwEndRenderID, 
			bOccludeQuery, bCastShadow, bReceiveShadow, bTransparence, mtxShadowProjection);
	}

	//
	// ��Ⱦ������Ӱ
	//
	VOID CRenderQuene::RenderCastShadow(const CEntityShadow *pShadow, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID)
	{
		ASSERT(pShadow);

		//
		// 1. ������Ӱ��Ⱦ����
		//
		ShadowRenderQueneMap::iterator itShadowRenderQuene = m_shadowRenderQuenes.find(pShadow);
		if (itShadowRenderQuene == m_shadowRenderQuenes.end()) return;

		//
		// 2. �������
		//
		Renderer()->SetOrtho(pShadow->GetLeft(), pShadow->GetRight(), pShadow->GetBottom(), pShadow->GetTop(), pShadow->GetZNear(), pShadow->GetZFar());
		Renderer()->SetLookAt(*pShadow->GetPosition(), *pShadow->GetForwardDirection(), *pShadow->GetUpDirection());

		MATRIX4 mtxScaleAndBias;
		MtxSet(&mtxScaleAndBias, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f);
		MtxMul(&itShadowRenderQuene->second.mtxShadowProjection, &(MATRIX4)Renderer()->GetModelViewProjectionMatrix(), &mtxScaleAndBias);

		//
		// 3. ��Ⱦ
		//    1. ʹ��Scissor����ȷ����Ե������Ⱦ
		//    2. ����ʲô���͵Ĺ�Դ������ƽ�й⴦��, ��ʹ��ƽ��ͶӰ
		// 
		Renderer()->Enable(GL_SCISSOR_TEST);
		Renderer()->SetScissor(Renderer()->GetViewportX() + 1, Renderer()->GetViewportY() + 1, Renderer()->GetViewportWidth() - 2, Renderer()->GetViewportHeight() - 2);
		{
			BOOL bOccludeQuery = FALSE;
			BOOL bCastShadow = TRUE;
			BOOL bReceiveShadow = FALSE;
			BOOL bTransparence = FALSE;
			const MATRIX4 *mtxShadowProjection = NULL;

			Render(itShadowRenderQuene->second.receives, dwSolution, dwPassGroup, dwBeginRenderID, dwEndRenderID, 
				bOccludeQuery, bCastShadow, bReceiveShadow, bTransparence, mtxShadowProjection);
		}
		Renderer()->Disable(GL_SCISSOR_TEST);
	}

	//
	// ��Ⱦ����
	//
	VOID CRenderQuene::RenderLighting(const CEntityCamera *pCamera)
	{
		ASSERT(pCamera);

		//
		// 1. ���ҳ�����Ⱦ����
		//
		SceneRenderQueneMap::iterator itSceneRenderQuene = m_sceneRenderQuenes.find(pCamera);
		if (itSceneRenderQuene == m_sceneRenderQuenes.end()) return;

		//
		// 2. �������
		//
		Renderer()->SetPerspective(pCamera->GetFovy(), pCamera->GetAspect(), pCamera->GetZNear(), pCamera->GetZFar());
		Renderer()->SetLookAt(*pCamera->GetPosition(), *pCamera->GetForwardDirection(), *pCamera->GetUpDirection());

		//
		// 3. ��Ⱦ
		//
		RenderLight(itSceneRenderQuene->second.lights, itSceneRenderQuene->second.bRenderQuery);
	}

	//
	// ��Ⱦ�ڵ�
	//
	VOID CRenderQuene::RenderOcclude(const RenderQuene &occludes)
	{
		Renderer()->BeginOcclude();
		{
			for (RenderQuene::const_iterator itEntity = occludes.begin(); itEntity != occludes.end(); ++itEntity) {
				const CEntityRenderable *pEntity = *itEntity;
				ASSERT(pEntity);

				pEntity->RenderOcclude();
			}
		}
		Renderer()->EndOcclude();
	}

	//
	// ��Ⱦ��ѯ
	//
	VOID CRenderQuene::RenderQuery(const MaterialQuene &materials, const LightQuene &lights)
	{
		Renderer()->BeginQuery();
		{
			for (MaterialQuene::const_iterator itRenderQuene = materials.begin(); itRenderQuene != materials.end(); ++itRenderQuene) {
				for (RenderQuene::const_iterator itEntity = itRenderQuene->second.begin(); itEntity != itRenderQuene->second.end(); ++itEntity) {
					const CEntityRenderable *pEntity = *itEntity;
					ASSERT(pEntity);

					pEntity->RenderQuery();
				}
			}

			for (LightQuene::const_iterator itEntity = lights.begin(); itEntity != lights.end(); ++itEntity) {
				const CEntityLight *pEntity = *itEntity;
				ASSERT(pEntity);

				pEntity->RenderQuery();
			}
		}
		Renderer()->EndQuery();
	}

	//
	// ��Ⱦ�ƹ�
	//
	VOID CRenderQuene::RenderLight(const LightQuene &lights, BOOL bOccludeQuery)
	{
		Renderer()->BeginLighting();
		{
			for (LightQuene::const_iterator itLight = lights.begin(); itLight != lights.end(); ++itLight) {
				const CEntityLight *pLight = *itLight;
				ASSERT(pLight);

				if (bOccludeQuery == FALSE || Renderer()->QueryResult(pLight->GetQuery())) {
					pLight->RenderLighting();
				}
			}
		}
		Renderer()->EndLighting();
	}

	//
	// ��Ⱦ
	//
	VOID CRenderQuene::Render(const MaterialQuene &materials, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID,
		BOOL bOccludeQuery,                                                                    // ʹ���ڵ���ѯ
		BOOL bCastShadow,                                                                      // ������Ӱ
		BOOL bReceiveShadow,                                                                   // ������Ӱ
		BOOL bTransparence,                                                                    // ��Ⱦ͸������
		const MATRIX4 *mtxShadowProjection)                                                    // ��ӰͶӰ����
	{
		INT indexPass = 0;
		
		do {
			PassQuene passQuene;

			//
			// 1. ��֯��Ⱦͨ·����
			//
			for (MaterialQuene::const_iterator itRenderQuene = materials.begin(); itRenderQuene != materials.end(); ++itRenderQuene) {
				const CMaterial *pMaterial = itRenderQuene->first;
				ASSERT(pMaterial);

				//
				// 1.1. �����Ⱦͨ·
				//
				const CMatSolution *pSolution = pMaterial->GetSolution(dwSolution); if (pSolution == NULL) continue;
				const CMatPassGroup *pGroup = pSolution->GetPassGroup(dwPassGroup); if (pGroup == NULL) continue;
				const CMatPass *pPass = pGroup->GetPass(indexPass); if (pPass == NULL) continue;

				//
				// 1.2. �����Ⱦͨ·
				//
				if (pPass->IsTransparence() != bTransparence) continue;
				if (pPass->GetRenderID() < dwBeginRenderID || pPass->GetRenderID() > dwEndRenderID) continue;

				//
				// 1.3. ����ʵ������
				//
				CEntityRenderable *pEntityLast = NULL;
				CEntityRenderable *pEntityHead = NULL;

				for (RenderQuene::const_iterator itEntity = itRenderQuene->second.begin(); itEntity != itRenderQuene->second.end(); ++itEntity) {
					CEntityRenderable *pEntity = const_cast<CEntityRenderable*>(*itEntity);
					ASSERT(pEntity);

					if ((bCastShadow == FALSE && bReceiveShadow == FALSE) ||
						(bCastShadow && pEntity->IsEnableShadowMapCastShadow()) ||
						(bReceiveShadow && pEntity->IsEnableShadowMapReceiveShadow())) {
						if (bOccludeQuery == FALSE || Renderer()->QueryResult(pEntity->GetQuery())) {
							if (pEntityLast == NULL) {
								pEntityLast = pEntityHead = pEntity;
							}
							else {
								pEntityLast->pRenderNext = pEntity;
								pEntityLast = pEntity;
							}
						}
					}

					pEntity->pRenderNext = NULL;
				}

				//
				// 1.4. �����Ⱦͨ·����
				//
				if (pEntityHead) {
					const_cast<CMatPass*>(pPass)->pEntityHead = pEntityHead;
					const_cast<CMatPass*>(pPass)->pRenderNext = const_cast<CMatPass*>(passQuene[pPass->Evaluate()]);
					passQuene[pPass->Evaluate()] = pPass;
				}
			}

			//
			// 2. �����Ⱦͨ·����
			//
			if (passQuene.empty()) {
				break;
			}

			//
			// 3. ��Ⱦ
			//
			for (PassQuene::const_iterator itPassQuene = passQuene.begin(); itPassQuene != passQuene.end(); ++itPassQuene) {
				if (const CMatPass *pPass = itPassQuene->second) {
					do {
						if (CEntityRenderable *pEntity = pPass->pEntityHead) {
							//
							// 3.1. ������Ⱦͨ·
							//
							Renderer()->SetPass(pPass->GetPass());

							if (bReceiveShadow && mtxShadowProjection) {
								Renderer()->SetShadowProjectionMatrix(*mtxShadowProjection);
							}

							//
							// 3.2. ��Ⱦ
							//
							do {
								Renderer()->SetPassAnimator(pPass->GetPass(), pEntity->GetTime());
								pEntity->Render(pPass->GetParentMaterial());
							} while (pEntity = pEntity->pRenderNext);
						}
					} while (pPass = pPass->pRenderNext);
				}
			}

			//
			// 4. �л���һ����Ⱦͨ·
			//
			indexPass++;
		} while (TRUE);
	}

}

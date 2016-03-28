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
	// 添加场景渲染队列
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
	// 添加场景光源渲染队列
	//
	VOID CRenderQuene::AddSceneLightRenderQuene(const CEntityCamera *pCamera, CEntityLight *pEntity)
	{
		ASSERT(pCamera);
		ASSERT(pEntity);

		m_sceneRenderQuenes[pCamera].bRenderQuery = FALSE;
		m_sceneRenderQuenes[pCamera].lights.push_back(pEntity);
	}

	//
	// 添加场景遮挡渲染队列
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
	// 添加产生阴影渲染队列
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
	// 添加接收阴影渲染队列
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
	// 清空渲染队列
	//
	VOID CRenderQuene::ClearRenderQuene(VOID)
	{
		m_sceneRenderQuenes.clear();
		m_shadowRenderQuenes.clear();
	}

	//
	// 渲染遮挡
	//
	VOID CRenderQuene::RenderOcclude(const CEntityCamera *pCamera)
	{
		ASSERT(pCamera);

		//
		// 1. 查找场景渲染队列并检查是否已经渲染
		//
		SceneRenderQueneMap::iterator itSceneRenderQuene = m_sceneRenderQuenes.find(pCamera);
		if (itSceneRenderQuene == m_sceneRenderQuenes.end()) return;
		if (itSceneRenderQuene->second.bRenderOcclude) return;

		//
		// 2. 设置相机
		//
		Renderer()->SetPerspective(pCamera->GetFovy(), pCamera->GetAspect(), pCamera->GetZNear(), pCamera->GetZFar());
		Renderer()->SetLookAt(*pCamera->GetPosition(), *pCamera->GetForwardDirection(), *pCamera->GetUpDirection());

		//
		// 3. 渲染
		//
		RenderOcclude(itSceneRenderQuene->second.occludes);

		//
		// 4. 设置已经渲染
		//
		itSceneRenderQuene->second.bRenderOcclude = TRUE;
	}

	//
	// 渲染查询
	//
	VOID CRenderQuene::RenderQuery(const CEntityCamera *pCamera)
	{
		ASSERT(pCamera);

		//
		// 1. 查找场景渲染队列并检查是否已经渲染
		//
		SceneRenderQueneMap::iterator itSceneRenderQuene = m_sceneRenderQuenes.find(pCamera);
		if (itSceneRenderQuene == m_sceneRenderQuenes.end()) return;
		if (itSceneRenderQuene->second.bRenderQuery) return;

		//
		// 2. 设置相机
		//
		Renderer()->SetPerspective(pCamera->GetFovy(), pCamera->GetAspect(), pCamera->GetZNear(), pCamera->GetZFar());
		Renderer()->SetLookAt(*pCamera->GetPosition(), *pCamera->GetForwardDirection(), *pCamera->GetUpDirection());

		//
		// 3. 渲染
		//
		RenderQuery(itSceneRenderQuene->second.materials, itSceneRenderQuene->second.lights);

		//
		// 4. 设置已经渲染
		//
		itSceneRenderQuene->second.bRenderQuery = TRUE;
	}

	//
	// 渲染不透明物体
	//
	VOID CRenderQuene::RenderOpaque(const CEntityCamera *pCamera, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID)
	{
		ASSERT(pCamera);

		//
		// 1. 查找场景渲染队列
		//
		SceneRenderQueneMap::iterator itSceneRenderQuene = m_sceneRenderQuenes.find(pCamera);
		if (itSceneRenderQuene == m_sceneRenderQuenes.end()) return;

		//
		// 2. 设置相机
		//
		Renderer()->SetPerspective(pCamera->GetFovy(), pCamera->GetAspect(), pCamera->GetZNear(), pCamera->GetZFar());
		Renderer()->SetLookAt(*pCamera->GetPosition(), *pCamera->GetForwardDirection(), *pCamera->GetUpDirection());

		//
		// 3. 渲染
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
	// 渲染透明物体
	//
	VOID CRenderQuene::RenderTransparence(const CEntityCamera *pCamera, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID)
	{
		ASSERT(pCamera);

		//
		// 1. 查找场景渲染队列
		//
		SceneRenderQueneMap::iterator itSceneRenderQuene = m_sceneRenderQuenes.find(pCamera);
		if (itSceneRenderQuene == m_sceneRenderQuenes.end()) return;

		//
		// 2. 设置相机
		//
		Renderer()->SetPerspective(pCamera->GetFovy(), pCamera->GetAspect(), pCamera->GetZNear(), pCamera->GetZFar());
		Renderer()->SetLookAt(*pCamera->GetPosition(), *pCamera->GetForwardDirection(), *pCamera->GetUpDirection());

		//
		// 3. 渲染
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
	// 渲染接收阴影
	//
	VOID CRenderQuene::RenderReceiveShadow(const CEntityCamera *pCamera, const CEntityShadow *pShadow, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID)
	{
		ASSERT(pCamera);
		ASSERT(pShadow);

		//
		// 1. 查找场景渲染队列
		//
		SceneRenderQueneMap::iterator itSceneRenderQuene = m_sceneRenderQuenes.find(pCamera);
		if (itSceneRenderQuene == m_sceneRenderQuenes.end()) return;

		//
		// 2. 查找阴影渲染队列
		//
		ShadowRenderQueneMap::iterator itShadowRenderQuene = m_shadowRenderQuenes.find(pShadow);
		if (itShadowRenderQuene == m_shadowRenderQuenes.end()) return;

		//
		// 3. 设置相机
		//
		Renderer()->SetPerspective(pCamera->GetFovy(), pCamera->GetAspect(), pCamera->GetZNear(), pCamera->GetZFar());
		Renderer()->SetLookAt(*pCamera->GetPosition(), *pCamera->GetForwardDirection(), *pCamera->GetUpDirection());

		//
		// 4. 渲染
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
	// 渲染产生阴影
	//
	VOID CRenderQuene::RenderCastShadow(const CEntityShadow *pShadow, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID)
	{
		ASSERT(pShadow);

		//
		// 1. 查找阴影渲染队列
		//
		ShadowRenderQueneMap::iterator itShadowRenderQuene = m_shadowRenderQuenes.find(pShadow);
		if (itShadowRenderQuene == m_shadowRenderQuenes.end()) return;

		//
		// 2. 设置相机
		//
		Renderer()->SetOrtho(pShadow->GetLeft(), pShadow->GetRight(), pShadow->GetBottom(), pShadow->GetTop(), pShadow->GetZNear(), pShadow->GetZFar());
		Renderer()->SetLookAt(*pShadow->GetPosition(), *pShadow->GetForwardDirection(), *pShadow->GetUpDirection());

		MATRIX4 mtxScaleAndBias;
		MtxSet(&mtxScaleAndBias, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f);
		MtxMul(&itShadowRenderQuene->second.mtxShadowProjection, &(MATRIX4)Renderer()->GetModelViewProjectionMatrix(), &mtxScaleAndBias);

		//
		// 3. 渲染
		//    1. 使用Scissor测试确保边缘不被渲染
		//    2. 不论什么类型的光源都当作平行光处理, 即使用平行投影
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
	// 渲染光照
	//
	VOID CRenderQuene::RenderLighting(const CEntityCamera *pCamera)
	{
		ASSERT(pCamera);

		//
		// 1. 查找场景渲染队列
		//
		SceneRenderQueneMap::iterator itSceneRenderQuene = m_sceneRenderQuenes.find(pCamera);
		if (itSceneRenderQuene == m_sceneRenderQuenes.end()) return;

		//
		// 2. 设置相机
		//
		Renderer()->SetPerspective(pCamera->GetFovy(), pCamera->GetAspect(), pCamera->GetZNear(), pCamera->GetZFar());
		Renderer()->SetLookAt(*pCamera->GetPosition(), *pCamera->GetForwardDirection(), *pCamera->GetUpDirection());

		//
		// 3. 渲染
		//
		RenderLight(itSceneRenderQuene->second.lights, itSceneRenderQuene->second.bRenderQuery);
	}

	//
	// 渲染遮挡
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
	// 渲染查询
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
	// 渲染灯光
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
	// 渲染
	//
	VOID CRenderQuene::Render(const MaterialQuene &materials, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID,
		BOOL bOccludeQuery,                                                                    // 使用遮挡查询
		BOOL bCastShadow,                                                                      // 产生阴影
		BOOL bReceiveShadow,                                                                   // 接收阴影
		BOOL bTransparence,                                                                    // 渲染透明物体
		const MATRIX4 *mtxShadowProjection)                                                    // 阴影投影矩阵
	{
		INT indexPass = 0;
		
		do {
			PassQuene passQuene;

			//
			// 1. 组织渲染通路队列
			//
			for (MaterialQuene::const_iterator itRenderQuene = materials.begin(); itRenderQuene != materials.end(); ++itRenderQuene) {
				const CMaterial *pMaterial = itRenderQuene->first;
				ASSERT(pMaterial);

				//
				// 1.1. 获得渲染通路
				//
				const CMatSolution *pSolution = pMaterial->GetSolution(dwSolution); if (pSolution == NULL) continue;
				const CMatPassGroup *pGroup = pSolution->GetPassGroup(dwPassGroup); if (pGroup == NULL) continue;
				const CMatPass *pPass = pGroup->GetPass(indexPass); if (pPass == NULL) continue;

				//
				// 1.2. 检查渲染通路
				//
				if (pPass->IsTransparence() != bTransparence) continue;
				if (pPass->GetRenderID() < dwBeginRenderID || pPass->GetRenderID() > dwEndRenderID) continue;

				//
				// 1.3. 建立实体链表
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
				// 1.4. 添加渲染通路队列
				//
				if (pEntityHead) {
					const_cast<CMatPass*>(pPass)->pEntityHead = pEntityHead;
					const_cast<CMatPass*>(pPass)->pRenderNext = const_cast<CMatPass*>(passQuene[pPass->Evaluate()]);
					passQuene[pPass->Evaluate()] = pPass;
				}
			}

			//
			// 2. 检查渲染通路队列
			//
			if (passQuene.empty()) {
				break;
			}

			//
			// 3. 渲染
			//
			for (PassQuene::const_iterator itPassQuene = passQuene.begin(); itPassQuene != passQuene.end(); ++itPassQuene) {
				if (const CMatPass *pPass = itPassQuene->second) {
					do {
						if (CEntityRenderable *pEntity = pPass->pEntityHead) {
							//
							// 3.1. 设置渲染通路
							//
							Renderer()->SetPass(pPass->GetPass());

							if (bReceiveShadow && mtxShadowProjection) {
								Renderer()->SetShadowProjectionMatrix(*mtxShadowProjection);
							}

							//
							// 3.2. 渲染
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
			// 4. 切换下一个渲染通路
			//
			indexPass++;
		} while (TRUE);
	}

}

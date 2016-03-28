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


		// ���ݽṹ
	public:
		typedef std::vector<const CEntityLight*> LightQuene;                                       // ��Դ��Ⱦ����
		typedef std::vector<const CEntityRenderable*> RenderQuene;                                 // ������Ⱦ����
		typedef std::map<UINT64, const CMatPass*> PassQuene;                                       // ͨ·��Ⱦ����, ˵��: ����ͨ·����ֵ����
		typedef std::map<const CMaterial*, RenderQuene> MaterialQuene;                             // ������Ⱦ����, ˵��: ���ڲ�������

	public:
		typedef struct {
			BOOL bRenderQuery;                                                                     // ��Ⱦ��ѯ״̬
			BOOL bRenderOcclude;                                                                   // ��Ⱦ�ڵ�״̬
			LightQuene lights;                                                                     // ��Դ��Ⱦ����
			RenderQuene occludes;                                                                  // �ڵ���Ⱦ����
			MaterialQuene materials;                                                               // ������Ⱦ����
		} SCENE_RENDER_QUENE;

		typedef struct {
			MATRIX4 mtxShadowProjection;                                                           // ��ӰͶӰ����
			MaterialQuene casts;                                                                   // ������Ӱ��Ⱦ����
			MaterialQuene receives;                                                                // ������Ӱ��Ⱦ����
		} SHADOW_RENDER_QUENE;

		// ע��: 
		// 1. ���������Ӱ֧��
		// 2. ������Ⱦ���к���Ӱ��Ⱦ���б˴˶���
		typedef std::map<const CEntityCamera*, SCENE_RENDER_QUENE> SceneRenderQueneMap;            // ������Ⱦ���м���
		typedef std::map<const CEntityShadow*, SHADOW_RENDER_QUENE> ShadowRenderQueneMap;          // ��Ӱ��Ⱦ���м���


		// ����/��������
	protected:
		CRenderQuene(VOID);
		virtual ~CRenderQuene(VOID);


		// ����
	public:
		VOID AddSceneRenderQuene(const CEntityCamera *pCamera, const CMaterial *pMaterial, CEntityRenderable *pEntity); // ��ӳ�����Ⱦ����
		VOID AddSceneLightRenderQuene(const CEntityCamera *pCamera, CEntityLight *pEntity);        // ��ӳ�����Դ��Ⱦ����
		VOID AddSceneOccludeRenderQuene(const CEntityCamera *pCamera, CEntityRenderable *pEntity); // ��ӳ����ڵ���Ⱦ����

		VOID AddCastShadowRenderQuene(const CEntityShadow *pShadow, const CMaterial *pMaterial, CEntityRenderable *pEntity); // ��Ӳ�����Ӱ��Ⱦ����
		VOID AddReceiveShadowRenderQuene(const CEntityShadow *pShadow, const CMaterial *pMaterial, CEntityRenderable *pEntity); // ��ӽ�����Ӱ��Ⱦ����

		VOID ClearRenderQuene(VOID);                                                               // �����Ⱦ����

	public:
		VOID RenderOcclude(const CEntityCamera *pCamera);                                          // ��Ⱦ�ڵ�
		VOID RenderQuery(const CEntityCamera *pCamera);                                            // ��Ⱦ��ѯ

		VOID RenderOpaque(const CEntityCamera *pCamera, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID); // ��Ⱦ��͸������
		VOID RenderTransparence(const CEntityCamera *pCamera, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID); // ��Ⱦ͸������

		VOID RenderReceiveShadow(const CEntityCamera *pCamera, const CEntityShadow *pShadow, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID); // ��Ⱦ������Ӱ
		VOID RenderCastShadow(const CEntityShadow *pShadow, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID); // ��Ⱦ������Ӱ

		VOID RenderLighting(const CEntityCamera *pCamera);                                         // ��Ⱦ����

	protected:
		VOID RenderOcclude(const RenderQuene &occludes);                                           // ��Ⱦ�ڵ�
		VOID RenderQuery(const MaterialQuene &materials, const LightQuene &lights);                // ��Ⱦ��ѯ
		VOID RenderLight(const LightQuene &lights, BOOL bOccludeQuery);                            // ��Ⱦ�ƹ�
		VOID Render(const MaterialQuene &materials, DWORD dwSolution, DWORD dwPassGroup, DWORD dwBeginRenderID, DWORD dwEndRenderID, // ��Ⱦ
			BOOL bOccludeQuery,                                                                    // ʹ���ڵ���ѯ
			BOOL bCastShadow,                                                                      // ������Ӱ
			BOOL bReceiveShadow,                                                                   // ������Ӱ
			BOOL bTransparence,                                                                    // ��Ⱦ͸������
			const MATRIX4 *mtxShadowProjection);                                                   // ��ӰͶӰ����


		// ����
	protected:
		SceneRenderQueneMap m_sceneRenderQuenes;                                                   // ������Ⱦ���м���
		ShadowRenderQueneMap m_shadowRenderQuenes;                                                 // ��Ӱ��Ⱦ���м���
	};

}

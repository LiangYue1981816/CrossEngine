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


		// ����/��������
	protected:
		CEntityRenderable(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntityRenderable(VOID);


		// ����
	public:
		virtual VOID SetVisibleFlags(DWORD dwFlags);                                               // ���ÿɼ��Ա�ʶ
		virtual VOID SetVisibleDistance(FLOAT distance);                                           // ���ÿɼ�����

		virtual DWORD GetVisibleFlags(VOID) const;                                                 // ��ÿɼ��Ա�ʶ
		virtual FLOAT GetVisibleDistance(VOID) const;                                              // ��ÿɼ�����

		virtual BOOL IsRenderable(const CEntityCamera *pCamera) const;                             // ��ÿ���Ⱦ

	public:
		virtual VOID SetLightChannel(DWORD dwChannel);                                             // ���ù���ͨ��
		virtual DWORD GetLightChannel(VOID) const;                                                 // ��Ĺ���ͨ��

	public:
		virtual VOID SetIndexOcclude(INT index);                                                   // �����ڵ�����
		virtual INT GetIndexOcclude(VOID) const;                                                   // ����ڵ�����

		virtual VOID SetEnableOcclude(BOOL bEnable);                                               // ���������ڵ�
		virtual BOOL IsEnableOcclude(VOID) const;                                                  // ��������ڵ�

	public:
		virtual VOID SetEnableLightMapCastShadow(BOOL bEnable);                                    // �������ù���ͼ������Ӱ
		virtual BOOL IsEnableLightMapCastShadow(VOID) const;                                       // ������ù���ͼ������Ӱ

		virtual VOID SetEnableLightMapReceiveShadow(BOOL bEnable);                                 // �������ù���ͼ������Ӱ
		virtual BOOL IsEnableLightMapReceiveShadow(VOID) const;                                    // ������ù���ͼ������Ӱ

	public:
		virtual VOID SetShadowMapCastShadowDistance(FLOAT distance);                               // ������Ӱͼ������Ӱ����
		virtual FLOAT GetShadowMapCastShadowDistance(VOID) const;                                  // �����Ӱͼ������Ӱ����

		virtual VOID SetEnableShadowMapCastShadow(BOOL bEnable);                                   // ����������Ӱͼ������Ӱ
		virtual BOOL IsEnableShadowMapCastShadow(VOID) const;                                      // ���������Ӱͼ������Ӱ
		virtual BOOL IsShadowMapCastShadow(const CEntityCamera *pCamera) const;                    // �����Ӱͼ������Ӱ

		virtual VOID SetEnableShadowMapReceiveShadow(BOOL bEnable);                                // ����������Ӱͼ������Ӱ
		virtual BOOL IsEnableShadowMapReceiveShadow(VOID) const;                                   // ���������Ӱͼ������Ӱ

	public:
		virtual const AABB* GetAABB(VOID);                                                         // ��ð�Χ��
		virtual const CMaterialPtr& GetMaterial(VOID) const;                                       // ��ò���

	public:
		virtual VOID UpdateRenderQuene(const CEntityCamera *pCamera);                              // ������Ⱦ����
		virtual VOID Render(const CMaterial *pMaterial) const;                                     // ��Ⱦ
		virtual VOID RenderQuery(VOID) const;                                                      // ��Ⱦ��ѯ
		virtual VOID RenderOcclude(VOID) const;                                                    // ��Ⱦ�ڵ�

	public:
		virtual UINT GetQuery(VOID) const;                                                         // ����ڵ���ѯ


		// ����
	protected:
		INT m_indexOcclude;                                                                        // �ڵ�����
		                                                                                           // ˵��: �ڵ��޳�ʱ���ڲ�ѯ�Ƿ��ڵ���
	protected:
		DWORD m_dwLightChannel;                                                                    // ����ͨ��

	protected:
		AABB m_aabbLocal;                                                                          // ��Χ��
		AABB m_aabbWorld;                                                                          // ��Χ��

	protected:
		DWORD m_dwVisibleFlags;                                                                    // �ɼ���ʶ
		FLOAT m_visibleDistance;                                                                   // �ɼ�����
		FLOAT m_visibleDistance2;                                                                  // �ɼ�����ƽ��

	protected:
		BOOL m_bEnableOcclude;                                                                     // �����ڵ�

	protected:
		BOOL m_bEnableLightMapCastShadow;                                                          // ���ù���ͼ������Ӱ
		BOOL m_bEnableLightMapReceiveShadow;                                                       // ���ù���ͼ������Ӱ

	protected:
		FLOAT m_shadowMapCastShadowDistance;                                                       // ��Ӱͼ������Ӱ����
		FLOAT m_shadowMapCastShadowDistance2;                                                      // ��Ӱͼ������Ӱ����ƽ��

		BOOL m_bEnableShodowMapCastShadow;                                                         // ������Ӱͼ������Ӱ
		BOOL m_bEnableShodowMapReceiveShadow;                                                      // ������Ӱͼ������Ӱ

	protected:
		UINT m_query;                                                                              // �ڵ���ѯ

		// ��Ⱦ����
	public:
		CEntityRenderable *pRenderNext;                                                            // ʵ����Ⱦ����
	};

}

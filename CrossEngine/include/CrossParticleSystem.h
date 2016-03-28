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

	class _CrossExport CParticleSystem
	{
		// ���ݽṹ
	public:
		typedef std::vector<CEmitterBase*> EmitterSet;                                             // ����������
		typedef std::vector<CAffectorBase*> AffectorSet;                                           // Ӱ��������


		// ����/��������
	public:
		CParticleSystem(VOID);
		virtual ~CParticleSystem(VOID);


		// ����
	public:
		virtual VOID Init(VOID);                                                                   // ��ʼ��
		virtual VOID Free(VOID);                                                                   // �ͷ�

		virtual VOID LoadXML(TiXmlNode *pParticleSystemNode);                                      // ����XML
		virtual VOID SaveXML(TiXmlNode *pParticleSystemNode) const;                                // ����XML

	protected:
		VOID LoadGfx(TiXmlNode *pParticleSystemNode);                                              // ����ͼ��
		VOID LoadParticle(TiXmlNode *pParticleSystemNode);                                         // ��������
		VOID LoadMaterial(TiXmlNode *pParticleSystemNode);                                         // ���ز���
		VOID LoadEmitters(TiXmlNode *pParticleSystemNode);                                         // ���ط�����
		VOID LoadAffectors(TiXmlNode *pParticleSystemNode);                                        // ����Ӱ����

		VOID SaveGfx(TiXmlNode *pParticleSystemNode) const;                                        // ����ͼ��
		VOID SaveParticle(TiXmlNode *pParticleSystemNode) const;                                   // ��������
		VOID SaveMaterial(TiXmlNode *pParticleSystemNode) const;                                   // �������
		VOID SaveEmitters(TiXmlNode *pParticleSystemNode) const;                                   // ���淢����
		VOID SaveAffectors(TiXmlNode *pParticleSystemNode) const;                                  // ����Ӱ����

	public:
		VOID SetParticleQuota(INT quota);                                                          // ������������

		INT GetParticleQuota(VOID) const;                                                          // �����������
		INT GetActiveParticleCount(VOID) const;                                                    // ��û������
		INT GetFreeParticleCount(VOID) const;                                                      // ��ÿ���������

	public:
		CGfxBase* CreateGfx(CGfxBase::TYPE type);                                                  // ����ͼ��
		CGfxBase* GetGfx(VOID) const;                                                              // ���ͼ��

	public:
		CEmitterBase* CreateEmitter(CEmitterBase::TYPE type);                                      // ����������
		VOID DestroyEmitter(CEmitterBase *pEmitter);                                               // ���ٷ�����

		INT GetEmitterCount(VOID) const;                                                           // ��÷���������
		CEmitterBase* GetEmitter(INT indexEmitter) const;                                          // ��÷�����

	public:
		CAffectorBase* CreateAffector(CAffectorBase::TYPE type);                                   // ����Ӱ����
		VOID DestroyAffector(CAffectorBase *pAffector);                                            // ����Ӱ����

		INT GetAffectorCount(VOID) const;                                                          // ���Ӱ��������
		CAffectorBase* GetAffector(INT indexAffector) const;                                       // ���Ӱ����

	public:
		BOOL SetMaterial(const CHAR *szName, BOOL bClone);                                         // ���ò���
		BOOL SetTexture(const CHAR *szName, INT indexTexUnit);                                     // ��������Ԫ����

		const CMaterialPtr& GetMaterial(VOID) const;                                               // ��ò���
		const CTexturePtr& GetTexture(INT indexTexUnit) const;                                     // �������Ԫ����

		VOID SetRenderID(INT renderID);                                                            // ������ȾID
		VOID SetBlend(UINT srcFactor, UINT desFactor);                                             // ���û��ģʽ
		VOID SetTexWarpU(INT indexTexUnit, UINT warp);                                             // �����������Ѱַ��ʽ
		VOID SetTexWarpV(INT indexTexUnit, UINT warp);                                             // ������������Ѱַ��ʽ

		INT GetRenderID(VOID) const;                                                               // �����ȾID
		UINT GetBlendSrcFactor(VOID) const;                                                        // ��û��ģʽ
		UINT GetBlendDesFactor(VOID) const;                                                        // ��û��ģʽ
		UINT GetTexWarpU(INT indexTexUnit) const;                                                  // ����������Ѱַ��ʽ
		UINT GetTexWarpV(INT indexTexUnit) const;                                                  // �����������Ѱַ��ʽ

	public:
		VOID SetScale(FLOAT x, FLOAT y, FLOAT z);                                                  // ��������
		VOID SetScale(const VEC3 *scale);                                                          // ��������
		const VEC3* GetScale(VOID) const;                                                          // �������

		VOID SetPosition(FLOAT x, FLOAT y, FLOAT z);                                               // ����λ��
		VOID SetPosition(const VEC3 *position);                                                    // ����λ��
		const VEC3* GetPosition(VOID) const;                                                       // ���λ��

		VOID SetDirection(FLOAT x, FLOAT y, FLOAT z);                                              // ���ó���
		VOID SetDirection(const VEC3 *direction);                                                  // ���ó���
		VOID SetOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w);                                   // ���ó���
		VOID SetOrientation(const QUAT *orientation);                                              // ���ó���
		const QUAT* GetOrientation(VOID) const;                                                    // ��ó���

	public:
		VOID Play(BOOL bImmediately);                                                              // ����
		VOID Stop(BOOL bImmediately = FALSE);                                                      // ֹͣ

		VOID Pause(VOID);                                                                          // ��ͣ
		VOID Resume(VOID);                                                                         // ����

		BOOL IsPlaying(VOID);                                                                      // ����״̬
		BOOL IsPlayFinish(VOID);                                                                   // ���Ž���

	public:
		VOID Update(FLOAT deltaTime);                                                              // ����
		VOID UpdateGfx(const CEntityCamera *pCamera);                                              // ����ͼ��

	public:
		VOID Render(const CEntityCamera *pCamera);                                                 // ��Ⱦ

	protected:
		VOID UpdateMaterial(FLOAT deltaTime);                                                      // ���²���
		VOID UpdateParticles(FLOAT deltaTime);                                                     // ��������
		VOID UpdateEmitter(FLOAT deltaTime);                                                       // ���·�����
		VOID UpdateAffector(FLOAT deltaTime);                                                      // ����Ӱ����


		// ����
	protected:
		CParticle::TYPE m_particleType;                                                            // ��������

		INT m_numParticleQuota;                                                                    // ��������
		INT m_numActiveParticles;                                                                  // �������

		CGfxBase *m_pGfx;                                                                          // ͼ��
		CParticle *m_pParticles;                                                                   // ���ӳ�
		CParticle *m_pActiveParticleList;                                                          // ���������
		CParticle *m_pFreeParticleList;                                                            // ������������

		EmitterSet m_emitters;                                                                     // ����������
		AffectorSet m_affectors;                                                                   // Ӱ��������

		VEC3 m_scale;                                                                              // ����
		VEC3 m_position;                                                                           // λ��
		QUAT m_orientation;                                                                        // ����

	protected:
		CMaterialPtr m_ptrMaterial;                                                                // ����
		BOOL m_bCloneMaterial;                                                                     // ���Ӳ������¡
		BOOL m_bUpdateMaterial;                                                                    // ���Ӳ��������
		CHAR m_szMaterialName[_MAX_STRING];                                                        // ���Ӳ�����

		CTexturePtr m_ptrTextures[MAX_TEXUNITS];                                                   // ����Ԫ
		UINT m_texWrapU[MAX_TEXUNITS];                                                             // ������������Ʒ�ʽ
		UINT m_texWrapV[MAX_TEXUNITS];                                                             // �������������Ʒ�ʽ

		INT m_renderID;                                                                            // ��ȾID
		UINT m_blendSrcFactor;                                                                     // ���ģʽ
		UINT m_blendDesFactor;                                                                     // ���ģʽ

	protected:
		BOOL m_bPlaying;                                                                           // ����״̬
		BOOL m_bPause;                                                                             // ��ͣ����״̬
	};

}

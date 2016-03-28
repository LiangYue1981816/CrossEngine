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

	class _CrossExport CEffect : public CResource
	{
		friend class CEffectPtr;
		friend class CEffectManager;


		// ���ݽṹ
	public:
		typedef struct {
			CHAR szName[_MAX_STRING];                                                              // ����

			FLOAT beginTime;                                                                       // ����ϵͳ��ʼʱ��
			FLOAT durationTime;                                                                    // ����ϵͳ����ʱ��

			CParticleSystem *pParticleSystem;                                                      // ����ϵͳ
		} PARTICLESYSTEM;

		typedef struct {
			CHAR szName[_MAX_STRING];                                                              // ����

			FLOAT beginTime;                                                                       // ������ʼʱ��
			FLOAT durationTime;                                                                    // ��������ʱ��

			BOOL bLoop;                                                                            // ѭ������
			BOOL b3D;                                                                              // 3D��Ч
			FLOAT volume;                                                                          // ����

			CSoundPtr ptrSound;                                                                    // ����Դ
			FMOD_CHANNEL *pChannel;                                                                // ����ͨ��
		} SOUND;

		typedef struct {
			CHAR szName[_MAX_STRING];                                                              // ����

			FLOAT beginTime;                                                                       // �𶯿�ʼʱ��
			FLOAT durationTime;                                                                    // �𶯳���ʱ��

			VEC3 minShake;                                                                         // ��С�𶯷�Χ
			VEC3 maxShake;                                                                         // ����𶯷�Χ

			FLOAT constantAttenuation;                                                             // ����˥��ϵ��
			FLOAT linearAttenuation;                                                               // ����˥��ϵ��
			FLOAT quadraticAttenuation;                                                            // ƽ��˥��ϵ��
		} SHAKE;

		typedef std::map<DWORD, PARTICLESYSTEM*> ParticleSystemMap;                                // ����ϵͳ����
		typedef std::map<DWORD, SOUND*> SoundMap;                                                  // ��������
		typedef std::map<DWORD, SHAKE*> ShakeMap;                                                  // �𶯼���


		// ����/��������
	protected:
		CEffect(CResourceManager *pResourceManager);
		virtual ~CEffect(VOID);


		// ����
	public:
		virtual VOID Init(VOID);                                                                   // ��ʼ����Դ
		virtual VOID Free(VOID);                                                                   // �ͷ���Դ

	public:
		virtual TYPE GetType(VOID) const;                                                          // �������
		virtual BOOL IsValid(VOID) const;                                                          // ��Ч�Լ��

	public:
		virtual BOOL LoadFromStream(CStream *pStream);                                             // ��������������Դ
		virtual BOOL SaveToFileStream(FILE *pFile);                                                // ���浽�ļ�������

	protected:
		VOID LoadAABB(TiXmlNode *pEffectNode);                                                     // ���ذ�Χ��
		VOID LoadParticleSystems(TiXmlNode *pEffectNode);                                          // ��������ϵͳ
		VOID LoadSounds(TiXmlNode *pEffectNode);                                                   // ��������
		VOID LoadShakes(TiXmlNode *pEffectNode);                                                   // ������

		VOID SaveAABB(TiXmlNode *pEffectNode);                                                     // �����Χ��
		VOID SaveParticleSystems(TiXmlNode *pEffectNode);                                          // ��������ϵͳ
		VOID SaveSounds(TiXmlNode *pEffectNode);                                                   // ��������
		VOID SaveShakes(TiXmlNode *pEffectNode);                                                   // ������

	public:
		VOID SetAABB(FLOAT width, FLOAT height, FLOAT length);                                     // ���ð�Χ�гߴ�
		VOID SetAABBCenter(FLOAT x, FLOAT y, FLOAT z);                                             // ���ð�Χ������
		const AABB* GetAABB(VOID) const;                                                           // ��ð�Χ��

	public:
		PARTICLESYSTEM* CreateParticleSystem(const CHAR *szName);                                  // ��������ϵͳ
		VOID DestroyParticleSystem(const CHAR *szName);                                            // ��������ϵͳ
		PARTICLESYSTEM* GetParticleSystem(const CHAR *szName) const;                               // �������ϵͳ

		SOUND* CreateSound(const CHAR *szName);                                                    // ��������
		VOID DestroySound(const CHAR *szName);                                                     // ��������
		SOUND* GetSound(const CHAR *szName) const;                                                 // �������

		SHAKE* CreateShake(const CHAR *szName);                                                    // ������
		VOID DestroyShake(const CHAR *szName);                                                     // ������
		SHAKE* GetShake(const CHAR *szName) const;                                                 // �����

		const ParticleSystemMap& GetParticleSystems(VOID) const;                                   // �������ϵͳ����
		const SoundMap& GetSounds(VOID) const;                                                     // �����������
		const ShakeMap& GetShakes(VOID) const;                                                     // ����𶯼���


		// ����
	protected:
		AABB m_aabb;                                                                               // ��Χ��

		ParticleSystemMap m_particleSystems;                                                       // ����ϵͳ����
		SoundMap m_sounds;                                                                         // ��������
		ShakeMap m_shakes;                                                                         // �𶯼���
	};

	class _CrossExport CEffectPtr : public CSharedPtr<CEffect>
	{
		// ����/��������
	public:
		CEffectPtr(VOID) : CSharedPtr<CEffect>() {}
		CEffectPtr(const CEffect *pEffect) : CSharedPtr<CEffect>(pEffect) {}
		CEffectPtr(const CEffectPtr &ptrEffect) : CSharedPtr<CEffect>(ptrEffect) {}
		CEffectPtr(const CResourcePtr &ptrResource) { Set(static_cast<CEffect*>(ptrResource.GetPointer()), ptrResource.GetRefCount()); }
		virtual ~CEffectPtr(VOID) {}


		// ����
	protected:
		virtual VOID FreePointer(VOID)
		{
			if (m_pPointer) {
				m_pPointer->GetResourceManager()->DestroyResource(m_pPointer);
			}
		}

	public:
		CEffectPtr& operator = (const CResourcePtr &ptrResource)
		{
			Set(static_cast<CEffect*>(ptrResource.GetPointer()), ptrResource.GetRefCount());
			return *this;
		}

		operator CResource* ()
		{
			return (CResource *)m_pPointer;
		}

		operator const CResource* () const
		{
			return (const CResource *)m_pPointer;
		}
	};

}

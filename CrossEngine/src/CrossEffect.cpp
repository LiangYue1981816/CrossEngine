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

	CEffect::CEffect(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{
		Init();
	}

	CEffect::~CEffect(VOID)
	{
		Free();
	}

	//
	// ��ʼ����Դ
	//
	VOID CEffect::Init(VOID)
	{
		AABBZero(&m_aabb);

		m_particleSystems.clear();
		m_sounds.clear();
		m_shakes.clear();

		CResource::Init();
	}

	//
	// �ͷ���Դ
	//
	VOID CEffect::Free(VOID)
	{
		//
		// 1. �ͷ�����ϵͳ
		//
		for (ParticleSystemMap::const_iterator itParticleSystem = m_particleSystems.begin(); itParticleSystem != m_particleSystems.end(); ++itParticleSystem) {
			PARTICLESYSTEM *pParticleSystem = itParticleSystem->second;
			ASSERT(pParticleSystem);
			ASSERT(pParticleSystem->pParticleSystem);

			SAFE_DELETE(pParticleSystem->pParticleSystem);
			SAFE_DELETE(pParticleSystem);
		}

		//
		// 2. �ͷ�����
		//
		for (SoundMap::const_iterator itSound = m_sounds.begin(); itSound != m_sounds.end(); ++itSound) {
			SOUND *pSound = itSound->second;
			ASSERT(pSound);
			ASSERT(pSound->pChannel);

			SoundSystem()->Stop(pSound->pChannel);
			pSound->ptrSound.SetNull();

			SAFE_DELETE(pSound);
		}

		//
		// 3. �ͷ���
		//
		for (ShakeMap::const_iterator itShake = m_shakes.begin(); itShake != m_shakes.end(); ++itShake) {
			const SHAKE *pShake = itShake->second;
			ASSERT(pShake);

			SAFE_DELETE(pShake);
		}

		CResource::Free();
	}

	//
	// �������
	//
	CResource::TYPE CEffect::GetType(VOID) const
	{
		return CResource::EFFECT;
	}

	//
	// ��Ч�Լ��
	//
	BOOL CEffect::IsValid(VOID) const
	{
		return TRUE;
	}

	//
	// ��������������Դ
	//
	BOOL CEffect::LoadFromStream(CStream *pStream)
	{
		//
		// 1. ��������Ч�Լ��
		//
		if (pStream == NULL) {
			return FALSE;
		}

		if (pStream->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. �ͷ���Ч
		//
		Free();

		//
		// 3. ������Ч
		//
		try {
			//
			// 3.1. ��XML
			//
			TiXmlDocument doc;
			if (doc.LoadFile((CHAR *)pStream->GetCurrentAddress(), pStream->GetFreeSize()) == FALSE) {
				throw "Invalid xml format.";
			}

			//
			// 3.2. ��ýڵ�
			//
			TiXmlNode *pEffectNode = doc.FirstChild("Effect");
			if (pEffectNode == NULL) throw "Invalid effect format.";

			//
			// 3.3. ����
			//
			LoadAABB(pEffectNode);
			LoadParticleSystems(pEffectNode);
			LoadSounds(pEffectNode);
			LoadShakes(pEffectNode);

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CEffect::LoadFromStream: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ���浽�ļ�������
	//
	BOOL CEffect::SaveToFileStream(FILE *pFile)
	{
		//
		// 1. ��������Ч�Լ��
		//
		if (pFile == NULL) {
			return FALSE;
		}

		//
		// 2. ������Ч
		//
		TiXmlDocument doc;
		TiXmlElement *pEffectNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Effect"); ASSERT(pEffectNode);
		{
			SaveAABB(pEffectNode);
			SaveParticleSystems(pEffectNode);
			SaveSounds(pEffectNode);
			SaveShakes(pEffectNode);
		}
		doc.LinkEndChild(pEffectNode);
		return doc.SaveFile(pFile) ? TRUE : FALSE;
	}

	//
	// ���ذ�Χ��
	//
	VOID CEffect::LoadAABB(TiXmlNode *pEffectNode)
	{
		ASSERT(pEffectNode);

		FLOAT width = pEffectNode->ToElement()->AttributeFloat("width");
		FLOAT height = pEffectNode->ToElement()->AttributeFloat("height");
		FLOAT length = pEffectNode->ToElement()->AttributeFloat("length");
		FLOAT centerx = pEffectNode->ToElement()->AttributeFloat("center_x");
		FLOAT centery = pEffectNode->ToElement()->AttributeFloat("center_y");
		FLOAT centerz = pEffectNode->ToElement()->AttributeFloat("center_z");

		SetAABB(width, height, length);
		SetAABBCenter(centerx, centery, centerz);
	}

	//
	// ��������ϵͳ
	//
	VOID CEffect::LoadParticleSystems(TiXmlNode *pEffectNode)
	{
		ASSERT(pEffectNode);

		if (TiXmlNode *pParticleSystemNode = pEffectNode->FirstChild("ParticleSystem")) {
			do {
				const CHAR *szName = pParticleSystemNode->ToElement()->AttributeString("name");
				if (szName == NULL) throw "No particle system name.";

				PARTICLESYSTEM *pParticleSystem = CreateParticleSystem(szName);
				ASSERT(pParticleSystem);

				pParticleSystem->beginTime = pParticleSystemNode->ToElement()->AttributeFloat("begin_time");
				pParticleSystem->durationTime = pParticleSystemNode->ToElement()->AttributeFloat("duration_time");
				pParticleSystem->pParticleSystem->LoadXML(pParticleSystemNode);
			} while (pParticleSystemNode = pEffectNode->IterateChildren("ParticleSystem", pParticleSystemNode));
		}
	}

	//
	// ��������
	//
	VOID CEffect::LoadSounds(TiXmlNode *pEffectNode)
	{
		ASSERT(pEffectNode);

		if (TiXmlNode *pSoundNode = pEffectNode->FirstChild("Sound")) {
			do {
				const CHAR *szName = pSoundNode->ToElement()->AttributeString("name");
				if (szName == NULL) throw "No sound name.";

				SOUND *pSound = CreateSound(szName);
				ASSERT(pSound);

				pSound->beginTime = pSoundNode->ToElement()->AttributeFloat("begin_time");
				pSound->durationTime = pSoundNode->ToElement()->AttributeFloat("duration_time");
				pSound->volume = pSoundNode->ToElement()->AttributeFloat("volume");
				pSound->bLoop = cglStringToAttrib(pSoundNode->ToElement()->AttributeString("loop"));
				pSound->b3D = cglStringToAttrib(pSoundNode->ToElement()->AttributeString("space"));
			} while (pSoundNode = pEffectNode->IterateChildren("Sound", pSoundNode));
		}
	}

	//
	// ������
	//
	VOID CEffect::LoadShakes(TiXmlNode *pEffectNode)
	{
		ASSERT(pEffectNode);

		if (TiXmlNode *pShakeNode = pEffectNode->FirstChild("Shake")) {
			do {
				const CHAR *szName = pShakeNode->ToElement()->AttributeString("name");
				if (szName == NULL) throw "No shake name.";

				SHAKE *pShake = CreateShake(szName);
				ASSERT(pShake);

				pShake->beginTime = pShakeNode->ToElement()->AttributeFloat("begin_time");
				pShake->durationTime = pShakeNode->ToElement()->AttributeFloat("duration_time");
				pShake->constantAttenuation = pShakeNode->ToElement()->AttributeFloat("constant_attenuation");
				pShake->linearAttenuation = pShakeNode->ToElement()->AttributeFloat("linear_attenuation");
				pShake->quadraticAttenuation = pShakeNode->ToElement()->AttributeFloat("quadratic_attenuation");
				sscanf(pShakeNode->ToElement()->AttributeString("min_shake"), "%f %f %f", &pShake->minShake[0], &pShake->minShake[1], &pShake->minShake[2]);
				sscanf(pShakeNode->ToElement()->AttributeString("max_shake"), "%f %f %f", &pShake->maxShake[0], &pShake->maxShake[1], &pShake->maxShake[2]);
			} while (pShakeNode = pEffectNode->IterateChildren("Shake", pShakeNode));
		}
	}

	//
	// �����Χ��
	//
	VOID CEffect::SaveAABB(TiXmlNode *pEffectNode)
	{
		ASSERT(pEffectNode);

		FLOAT width = m_aabb.maxVertex[0] - m_aabb.minVertex[0];
		FLOAT height = m_aabb.maxVertex[1] - m_aabb.minVertex[1];
		FLOAT length = m_aabb.maxVertex[2] - m_aabb.minVertex[2];
		FLOAT centerx = m_aabb.center[0];
		FLOAT centery = m_aabb.center[1];
		FLOAT centerz = m_aabb.center[2];

		pEffectNode->ToElement()->SetAttributeFloat("width", width);
		pEffectNode->ToElement()->SetAttributeFloat("height", height);
		pEffectNode->ToElement()->SetAttributeFloat("length", length);
		pEffectNode->ToElement()->SetAttributeFloat("center_x", centerx);
		pEffectNode->ToElement()->SetAttributeFloat("center_y", centery);
		pEffectNode->ToElement()->SetAttributeFloat("center_z", centerz);
	}

	//
	// ��������ϵͳ
	//
	VOID CEffect::SaveParticleSystems(TiXmlNode *pEffectNode)
	{
		ASSERT(pEffectNode);

		for (ParticleSystemMap::const_iterator itParticleSystem = m_particleSystems.begin(); itParticleSystem != m_particleSystems.end(); ++itParticleSystem) {
			const PARTICLESYSTEM *pParticleSystem = itParticleSystem->second;
			ASSERT(pParticleSystem);
			ASSERT(pParticleSystem->pParticleSystem);

			TiXmlElement *pParticleSystemNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("ParticleSystem"); ASSERT(pParticleSystemNode);
			{
				pParticleSystemNode->ToElement()->SetAttributeString("name", pParticleSystem->szName);
				pParticleSystemNode->ToElement()->SetAttributeFloat("begin_time", pParticleSystem->beginTime);
				pParticleSystemNode->ToElement()->SetAttributeFloat("duration_time", pParticleSystem->durationTime);
				pParticleSystem->pParticleSystem->SaveXML(pParticleSystemNode);
			}
			pEffectNode->LinkEndChild(pParticleSystemNode);
		}
	}

	//
	// ��������
	//
	VOID CEffect::SaveSounds(TiXmlNode *pEffectNode)
	{
		ASSERT(pEffectNode);

		for (SoundMap::const_iterator itSound = m_sounds.begin(); itSound != m_sounds.end(); ++itSound) {
			const SOUND *pSound = itSound->second;
			ASSERT(pSound);

			TiXmlElement *pSoundNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Sound"); ASSERT(pSoundNode);
			{
				pSoundNode->ToElement()->SetAttributeString("name", pSound->szName);
				pSoundNode->ToElement()->SetAttributeFloat("begin_time", pSound->beginTime);
				pSoundNode->ToElement()->SetAttributeFloat("duration_time", pSound->durationTime);
				pSoundNode->ToElement()->SetAttributeFloat("volume", pSound->volume);
				pSoundNode->ToElement()->SetAttributeString("loop", cglAttribToString(pSound->bLoop));
				pSoundNode->ToElement()->SetAttributeString("space", cglAttribToString(pSound->b3D));
			}
			pEffectNode->LinkEndChild(pSoundNode);
		}
	}

	//
	// ������
	//
	VOID CEffect::SaveShakes(TiXmlNode *pEffectNode)
	{
		ASSERT(pEffectNode);

		for (ShakeMap::const_iterator itShake = m_shakes.begin(); itShake != m_shakes.end(); ++itShake) {
			const SHAKE *pShake = itShake->second;
			ASSERT(pShake);

			TiXmlElement *pShakeNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Shake"); ASSERT(pShakeNode);
			{
				pShakeNode->ToElement()->SetAttributeString("name", pShake->szName);
				pShakeNode->ToElement()->SetAttributeFloat("begin_time", pShake->beginTime);
				pShakeNode->ToElement()->SetAttributeFloat("duration_time", pShake->durationTime);
				pShakeNode->ToElement()->SetAttributeFloat("constant_attenuation", pShake->constantAttenuation);
				pShakeNode->ToElement()->SetAttributeFloat("linear_attenuation", pShake->linearAttenuation);
				pShakeNode->ToElement()->SetAttributeFloat("quadratic_attenuation", pShake->quadraticAttenuation);
				pShakeNode->ToElement()->SetAttributeString("min_shake", "%f %f %f", pShake->minShake[0], pShake->minShake[1], pShake->minShake[2]);
				pShakeNode->ToElement()->SetAttributeString("max_shake", "%f %f %f", pShake->maxShake[0], pShake->maxShake[1], pShake->maxShake[2]);
			}
			pEffectNode->LinkEndChild(pShakeNode);
		}
	}

	//
	// ���ð�Χ�гߴ�
	//
	VOID CEffect::SetAABB(FLOAT width, FLOAT height, FLOAT length)
	{
		Vec3Set(&m_aabb.minVertex, m_aabb.center[0] - width / 2.0f, m_aabb.center[1] - height / 2.0f, m_aabb.center[2] - length / 2.0f);
		Vec3Set(&m_aabb.maxVertex, m_aabb.center[0] + width / 2.0f, m_aabb.center[1] + height / 2.0f, m_aabb.center[2] + length / 2.0f);
		AABBNormalize(&m_aabb);
	}

	//
	// ���ð�Χ������
	//
	VOID CEffect::SetAABBCenter(FLOAT x, FLOAT y, FLOAT z)
	{
		FLOAT width = m_aabb.maxVertex[0] - m_aabb.minVertex[0];
		FLOAT height = m_aabb.maxVertex[1] - m_aabb.minVertex[1];
		FLOAT length = m_aabb.maxVertex[2] - m_aabb.minVertex[2];

		Vec3Set(&m_aabb.center, x, y, z);
		Vec3Set(&m_aabb.minVertex, m_aabb.center[0] - width / 2.0f, m_aabb.center[1] - height / 2.0f, m_aabb.center[2] - length / 2.0f);
		Vec3Set(&m_aabb.maxVertex, m_aabb.center[0] + width / 2.0f, m_aabb.center[1] + height / 2.0f, m_aabb.center[2] + length / 2.0f);
		AABBNormalize(&m_aabb);
	}

	//
	// ��ð�Χ��
	//
	const AABB* CEffect::GetAABB(VOID) const
	{
		return &m_aabb;
	}

	//
	// ��������ϵͳ
	//
	CEffect::PARTICLESYSTEM* CEffect::CreateParticleSystem(const CHAR *szName)
	{
		ASSERT(szName);
		DWORD dwName = HashValue(szName);

		//
		// 1. ��������ϵͳ
		//
		ParticleSystemMap::const_iterator itParticleSystem = m_particleSystems.find(dwName);
		if (itParticleSystem != m_particleSystems.end()) return itParticleSystem->second;

		//
		// 2. ��������ϵͳ
		//
		PARTICLESYSTEM *pParticleSystem = SAFE_NEW(MEMTYPE_HEAP) PARTICLESYSTEM;
		ASSERT(pParticleSystem);

		strcpy(pParticleSystem->szName, szName);
		pParticleSystem->beginTime = 0.0f;
		pParticleSystem->durationTime = 0.0f;
		pParticleSystem->pParticleSystem = SAFE_NEW(MEMTYPE_HEAP) CParticleSystem;
		m_particleSystems[dwName] = pParticleSystem;

		return pParticleSystem;
	}

	//
	// ��������ϵͳ
	//
	VOID CEffect::DestroyParticleSystem(const CHAR *szName)
	{
		ASSERT(szName);

		//
		// 1. ��������ϵͳ
		//
		ParticleSystemMap::const_iterator itParticleSystem = m_particleSystems.find(HashValue(szName));
		if (itParticleSystem == m_particleSystems.end()) return;

		//
		// 2. ��������ϵͳ
		//
		PARTICLESYSTEM *pParticleSystem = itParticleSystem->second;
		ASSERT(pParticleSystem);
		ASSERT(pParticleSystem->pParticleSystem);

		SAFE_DELETE(pParticleSystem->pParticleSystem);
		SAFE_DELETE(pParticleSystem);
		m_particleSystems.erase(itParticleSystem);
	}

	//
	// �������ϵͳ
	//
	CEffect::PARTICLESYSTEM* CEffect::GetParticleSystem(const CHAR *szName) const
	{
		ASSERT(szName);
		
		ParticleSystemMap::const_iterator itParticleSystem = m_particleSystems.find(HashValue(szName));
		return itParticleSystem != m_particleSystems.end() ? itParticleSystem->second : NULL;
	}

	//
	// ��������
	//
	CEffect::SOUND* CEffect::CreateSound(const CHAR *szName)
	{
		ASSERT(szName);
		DWORD dwName = HashValue(szName);

		//
		// 1. ��������
		//
		SoundMap::const_iterator itSound = m_sounds.find(dwName);
		if (itSound != m_sounds.end()) return itSound->second;

		//
		// 2. ��������
		//
		SOUND *pSound = SAFE_NEW(MEMTYPE_HEAP) SOUND;
		ASSERT(pSound);

		strcpy(pSound->szName, szName);
		pSound->beginTime = 0.0f;
		pSound->durationTime = 0.0f;
		pSound->bLoop = FALSE;
		pSound->b3D = FALSE;
		pSound->volume = 1.0f;
		pSound->ptrSound = SoundManager()->QueryResource(dwName);
		pSound->pChannel = NULL;
		m_sounds[dwName] = pSound;

		return pSound;
	}

	//
	// ��������
	//
	VOID CEffect::DestroySound(const CHAR *szName)
	{
		ASSERT(szName);

		//
		// 1. ��������
		//
		SoundMap::const_iterator itSound = m_sounds.find(HashValue(szName));
		if (itSound == m_sounds.end()) return;

		//
		// 2. ��������
		//
		SOUND *pSound = itSound->second;
		ASSERT(pSound);

		SoundSystem()->Stop(pSound->pChannel);
		pSound->ptrSound.SetNull();

		SAFE_DELETE(pSound);
		m_sounds.erase(itSound);
	}

	//
	// �������
	//
	CEffect::SOUND* CEffect::GetSound(const CHAR *szName) const
	{
		ASSERT(szName);

		SoundMap::const_iterator itSound = m_sounds.find(HashValue(szName));
		return itSound != m_sounds.end() ? itSound->second : NULL;
	}

	//
	// ������
	//
	CEffect::SHAKE* CEffect::CreateShake(const CHAR *szName)
	{
		ASSERT(szName);
		DWORD dwName = HashValue(szName);

		//
		// 1. ������
		//
		ShakeMap::const_iterator itShake = m_shakes.find(dwName);
		if (itShake != m_shakes.end()) return itShake->second;

		//
		// 2. ������
		//
		SHAKE *pShake = SAFE_NEW(MEMTYPE_HEAP) SHAKE;
		ASSERT(pShake);

		strcpy(pShake->szName, szName);
		pShake->beginTime = 0.0f;
		pShake->durationTime = 0.0f;
		pShake->constantAttenuation = 1.0f;
		pShake->linearAttenuation = 0.0f;
		pShake->quadraticAttenuation = 0.0f;
		Vec3Set(&pShake->minShake, -1.0f, -1.0f, -1.0f);
		Vec3Set(&pShake->maxShake,  1.0f,  1.0f,  1.0f);
		m_shakes[dwName] = pShake;

		return pShake;
	}

	//
	// ������
	//
	VOID CEffect::DestroyShake(const CHAR *szName)
	{
		ASSERT(szName);

		//
		// 1. ������
		//
		ShakeMap::const_iterator itShake = m_shakes.find(HashValue(szName));
		if (itShake == m_shakes.end()) return;

		//
		// 2. ������
		//
		SHAKE *pShake = itShake->second;
		ASSERT(pShake);

		SAFE_DELETE(pShake);
		m_shakes.erase(itShake);
	}

	//
	// �����
	//
	CEffect::SHAKE* CEffect::GetShake(const CHAR *szName) const
	{
		ASSERT(szName);

		ShakeMap::const_iterator itShake = m_shakes.find(HashValue(szName));
		return itShake != m_shakes.end() ? itShake->second : NULL;
	}

	//
	// �������ϵͳ����
	//
	const CEffect::ParticleSystemMap& CEffect::GetParticleSystems(VOID) const
	{
		return m_particleSystems;
	}

	//
	// �����������
	//
	const CEffect::SoundMap& CEffect::GetSounds(VOID) const
	{
		return m_sounds;
	}

	//
	// ����𶯼���
	//
	const CEffect::ShakeMap& CEffect::GetShakes(VOID) const
	{
		return m_shakes;
	}

}

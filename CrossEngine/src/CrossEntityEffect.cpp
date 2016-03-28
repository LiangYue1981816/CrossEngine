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

	CEntityEffect::CEntityEffect(DWORD dwName, CSceneManager *pSceneManager)
		: CEntityRenderable(dwName, pSceneManager)
		, m_bPlaying(FALSE)
		, m_bPause(FALSE)

		, m_currTime(0.0f)
		, m_beginTime(0.0f)
		, m_durationTime(0.0f)
	{

	}

	CEntityEffect::~CEntityEffect(VOID)
	{
		m_ptrEffect.SetNull();
	}

	//
	// �������
	//
	CEntity::TYPE CEntityEffect::GetType(VOID) const
	{
		return CEntity::EFFECT;
	}

	//
	// ��ÿ���Ⱦ
	//
	BOOL CEntityEffect::IsRenderable(VOID) const
	{
		return TRUE;
	}

	//
	// ������Ч
	//
	BOOL CEntityEffect::SetEffect(const CHAR *szName)
	{
		ASSERT(szName);
		return SetEffect(HashValue(szName));
	}

	//
	// ������Ч
	//
	BOOL CEntityEffect::SetEffect(DWORD dwName)
	{
		return SetEffect(EffectManager()->QueryResource(dwName));
	}

	//
	// ������Ч
	//
	BOOL CEntityEffect::SetEffect(const CEffectPtr &ptrEffect)
	{
		//
		// 1. ������ȫ���
		//
		if (ptrEffect.IsNull() || ptrEffect->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. ������Ч
		//
		m_ptrEffect = CEffectPtr(EffectManager()->CreateResource());
		m_ptrEffect->CopyFrom(ptrEffect.GetPointer());
		m_ptrEffect->SetName(Engine()->GetNextEffectName());

		//
		// 3. ���ð�Χ��
		//
		AABBCopy(&m_aabbLocal, m_ptrEffect->GetAABB());

		return TRUE;
	}

	//
	// �����Ч
	//
	const CEffectPtr& CEntityEffect::GetEffect(VOID) const
	{
		return m_ptrEffect;
	}

	//
	// ����
	//
	VOID CEntityEffect::Play(FLOAT beginTime, FLOAT durationTime, BOOL bImmediately)
	{
		//
		// 1. ������ȫ���
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return;
		}

		//
		// 2. ֹͣ
		//
		Stop(bImmediately);

		//
		// 3. ����
		//
		m_bPlaying = TRUE;
		m_bPause = FALSE;

		m_currTime = 0.0f;
		m_beginTime = beginTime;
		m_durationTime = durationTime;
	}

	//
	// ֹͣ
	//
	VOID CEntityEffect::Stop(BOOL bImmediately)
	{
		//
		// 1. ������ȫ���
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return;
		}

		//
		// 2. ֹͣ
		//
		m_bPlaying = FALSE;

		//
		// 3. ֹͣ����ϵͳ������
		//
		const CEffect::SoundMap &sounds = m_ptrEffect->GetSounds();
		const CEffect::ParticleSystemMap &particleSystems = m_ptrEffect->GetParticleSystems();

		for (CEffect::SoundMap::const_iterator itSound = sounds.begin(); itSound != sounds.end(); ++itSound) {
			CEffect::SOUND *pSound = itSound->second;
			ASSERT(pSound);

			SoundSystem()->Stop(pSound->pChannel);
			pSound->pChannel = NULL;
		}

		for (CEffect::ParticleSystemMap::const_iterator itParticleSystem = particleSystems.begin(); itParticleSystem != particleSystems.end(); ++itParticleSystem) {
			CEffect::PARTICLESYSTEM *pParticleSystem = itParticleSystem->second;
			ASSERT(pParticleSystem);
			ASSERT(pParticleSystem->pParticleSystem);

			pParticleSystem->pParticleSystem->Stop(bImmediately);
		}
	}

	//
	// ��ͣ
	//
	VOID CEntityEffect::Pause(VOID)
	{
		//
		// 1. ������ȫ���
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return;
		}

		//
		// 2. ��ͣ
		//
		m_bPause = TRUE;

		//
		// 3. ��ͣ����ϵͳ������
		//
		const CEffect::SoundMap &sounds = m_ptrEffect->GetSounds();
		const CEffect::ParticleSystemMap &particleSystems = m_ptrEffect->GetParticleSystems();

		for (CEffect::SoundMap::const_iterator itSound = sounds.begin(); itSound != sounds.end(); ++itSound) {
			CEffect::SOUND *pSound = itSound->second;
			ASSERT(pSound);

			SoundSystem()->Pause(pSound->pChannel);
		}

		for (CEffect::ParticleSystemMap::const_iterator itParticleSystem = particleSystems.begin(); itParticleSystem != particleSystems.end(); ++itParticleSystem) {
			CEffect::PARTICLESYSTEM *pParticleSystem = itParticleSystem->second;
			ASSERT(pParticleSystem);
			ASSERT(pParticleSystem->pParticleSystem);

			pParticleSystem->pParticleSystem->Pause();
		}
	}

	//
	// ����
	//
	VOID CEntityEffect::Resume(VOID)
	{
		//
		// 1. ������ȫ���
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return;
		}

		//
		// 2. ����
		//
		m_bPause = FALSE;

		//
		// 3. ��������ϵͳ������
		//
		const CEffect::SoundMap &sounds = m_ptrEffect->GetSounds();
		const CEffect::ParticleSystemMap &particleSystems = m_ptrEffect->GetParticleSystems();

		for (CEffect::SoundMap::const_iterator itSound = sounds.begin(); itSound != sounds.end(); ++itSound) {
			CEffect::SOUND *pSound = itSound->second;
			ASSERT(pSound);

			SoundSystem()->Resume(pSound->pChannel);
		}

		for (CEffect::ParticleSystemMap::const_iterator itParticleSystem = particleSystems.begin(); itParticleSystem != particleSystems.end(); ++itParticleSystem) {
			CEffect::PARTICLESYSTEM *pParticleSystem = itParticleSystem->second;
			ASSERT(pParticleSystem);
			ASSERT(pParticleSystem->pParticleSystem);

			pParticleSystem->pParticleSystem->Resume();
		}
	}

	//
	// ����״̬
	//
	BOOL CEntityEffect::IsPlaying(VOID)
	{
		return m_bPlaying && m_bPause == FALSE;
	}

	//
	// ���µ���һ֡
	//
	BOOL CEntityEffect::NextFrame(FLOAT deltaTime)
	{
		//
		// 1. ������ȫ���
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. ʱ��ݽ�
		//
		if (IsPlaying()) {
			m_currTime += deltaTime;
		}

		//
		// 3. ���µ���һ֡
		//
		BOOL bShakePlaying = FALSE;
		BOOL bSoundPlaying = FALSE;
		BOOL bParticleSystemPlaying = FALSE;
		BOOL bParticleSystemGfxPlaying = FALSE;

		if (m_currTime > m_beginTime && (m_currTime < m_beginTime + m_durationTime || m_durationTime < EPSILON_E3)) {
			if (IsPlaying()) {
				bShakePlaying = UpdateShakes(deltaTime);
				bSoundPlaying = UpdateSounds(deltaTime);
				bParticleSystemPlaying = UpdateParticleSystems(deltaTime);
			}

			// ˵��: ��Ⱦ���ӿɲ��ܲ��ſ���, �ڲ���ֹͣ�����ӿ��������������Զ�����!!!
			bParticleSystemGfxPlaying = UpdateParticleSystemsGfx(deltaTime);
		}

		return bShakePlaying || bSoundPlaying || bParticleSystemPlaying || bParticleSystemGfxPlaying ? TRUE : FALSE;
	}

	//
	// ��������ϵͳ
	//
	BOOL CEntityEffect::UpdateParticleSystems(FLOAT deltaTime)
	{
		BOOL bPlaying = FALSE;

		FLOAT localTime = m_currTime - m_beginTime;
		const CEffect::ParticleSystemMap &particleSystems = m_ptrEffect->GetParticleSystems();

		for (CEffect::ParticleSystemMap::const_iterator itParticleSystem = particleSystems.begin(); itParticleSystem != particleSystems.end(); ++itParticleSystem) {
			CEffect::PARTICLESYSTEM *pParticleSystem = itParticleSystem->second;
			ASSERT(pParticleSystem);
			ASSERT(pParticleSystem->pParticleSystem);

			if (localTime > pParticleSystem->beginTime && (localTime < pParticleSystem->beginTime + pParticleSystem->durationTime || pParticleSystem->durationTime < EPSILON_E3)) {
				if (pParticleSystem->pParticleSystem->IsPlaying() == FALSE) {
					pParticleSystem->pParticleSystem->Play(FALSE);
				}
			}
			else {
				if (pParticleSystem->pParticleSystem->IsPlaying()) {
					pParticleSystem->pParticleSystem->Stop();
				}
			}

			// ע��: ����ϵͳ��û��ʼ���Ż���ѭ�����Ż��߻�δ���Ž�������Ϊ�ǻ��ڲ�����!!!
			if (localTime < pParticleSystem->beginTime + pParticleSystem->durationTime || pParticleSystem->durationTime < EPSILON_E3 || pParticleSystem->pParticleSystem->IsPlayFinish() == FALSE) {
				bPlaying |= TRUE;
			}
		}

		return bPlaying;
	}

	//
	// ��������ϵͳͼ��
	//
	BOOL CEntityEffect::UpdateParticleSystemsGfx(FLOAT deltaTime)
	{
		BOOL bPlaying = FALSE;

		FLOAT localTime = m_currTime - m_beginTime;
		const CEffect::ParticleSystemMap &particleSystems = m_ptrEffect->GetParticleSystems();

		for (CEffect::ParticleSystemMap::const_iterator itParticleSystem = particleSystems.begin(); itParticleSystem != particleSystems.end(); ++itParticleSystem) {
			CEffect::PARTICLESYSTEM *pParticleSystem = itParticleSystem->second;
			ASSERT(pParticleSystem);
			ASSERT(pParticleSystem->pParticleSystem);

			if (pParticleSystem->pParticleSystem->IsPlayFinish() == FALSE) {
				pParticleSystem->pParticleSystem->SetScale(m_pSceneNode->GetWorldScale());
				pParticleSystem->pParticleSystem->SetPosition(m_pSceneNode->GetWorldPosition());
				pParticleSystem->pParticleSystem->SetOrientation(m_pSceneNode->GetWorldOrientation());

				pParticleSystem->pParticleSystem->Update(deltaTime);
				pParticleSystem->pParticleSystem->UpdateGfx(m_pSceneManager->GetMainCameraEntity());

				bPlaying |= TRUE;
			}
		}

		return bPlaying;
	}

	//
	// ��������
	//
	BOOL CEntityEffect::UpdateSounds(FLOAT deltaTime)
	{
		BOOL bPlaying = FALSE;

		FLOAT localTime = m_currTime - m_beginTime;
		const CEffect::SoundMap &sounds = m_ptrEffect->GetSounds();

		for (CEffect::SoundMap::const_iterator itSound = sounds.begin(); itSound != sounds.end(); ++itSound) {
			CEffect::SOUND *pSound = itSound->second;
			ASSERT(pSound);

			if (localTime > pSound->beginTime && (localTime < pSound->beginTime + pSound->durationTime || pSound->durationTime < EPSILON_E3)) {
				if (SoundSystem()->IsPlaying(pSound->pChannel) == FALSE && pSound->pChannel == NULL) {
					pSound->pChannel = SoundSystem()->Play(pSound->ptrSound->GetSound(), FALSE, pSound->bLoop, TRUE);
				}

				if (SoundSystem()->IsPlaying(pSound->pChannel)) {
					SoundSystem()->SetSoundPosition(pSound->pChannel, m_pSceneNode->GetWorldPosition());
					SoundSystem()->SetSoundVolume(pSound->pChannel, pSound->volume);
				}
			}
			else {
				if (pSound->pChannel) {
					SoundSystem()->Stop(pSound->pChannel);
					pSound->pChannel = NULL;
				}
			}

			// ע��: ������û��ʼ���Ż���ѭ�����ž���Ϊ�ǻ��ڲ�����!!!
			if (localTime < pSound->beginTime + pSound->durationTime || pSound->durationTime < EPSILON_E3) {
				bPlaying |= TRUE;
			}
		}

		return bPlaying;
	}

	//
	// ������
	//
	BOOL CEntityEffect::UpdateShakes(FLOAT deltaTime)
	{
		BOOL bPlaying = FALSE;

		FLOAT localTime = m_currTime - m_beginTime;
		const CEffect::ShakeMap shakes = m_ptrEffect->GetShakes();

		VEC3 distance;
		Vec3Sub(&distance, m_pSceneNode->GetWorldPosition(), m_pSceneManager->GetMainCameraEntity()->GetPosition());

		FLOAT len = Vec3Length(&distance);
		FLOAT len2 = Vec3Length2(&distance);

		for (CEffect::ShakeMap::const_iterator itShake = shakes.begin(); itShake != shakes.end(); ++itShake) {
			CEffect::SHAKE *pShake = itShake->second;
			ASSERT(pShake);

			FLOAT x = 0.0f;
			FLOAT y = 0.0f;
			FLOAT z = 0.0f;

			if (localTime > pShake->beginTime && (localTime < pShake->beginTime + pShake->durationTime || pShake->durationTime < EPSILON_E3)) {
				FLOAT attenuation = pShake->constantAttenuation + pShake->linearAttenuation*len + pShake->quadraticAttenuation*len2;
				FLOAT invAttenuation = attenuation < EPSILON_E3 ? 0.0f : 1.0f / attenuation;

				x = (pShake->minShake[0] + (FRandomValue() * (pShake->maxShake[0] - pShake->minShake[0]))) * invAttenuation;
				y = (pShake->minShake[1] + (FRandomValue() * (pShake->maxShake[1] - pShake->minShake[1]))) * invAttenuation;
				z = (pShake->minShake[2] + (FRandomValue() * (pShake->maxShake[2] - pShake->minShake[2]))) * invAttenuation;
			}

			// ע��: �𶯻�û��ʼ���Ż���ѭ�����ž���Ϊ�ǻ��ڲ�����!!!
			if (localTime < pShake->beginTime + pShake->durationTime || pShake->durationTime < EPSILON_E3) {
				bPlaying |= TRUE;
			}

			m_pSceneManager->GetMainCameraEntity()->SetShake(x, y, z);
		}

		return bPlaying;
	}

	//
	// ������Ⱦ����
	//
	VOID CEntityEffect::UpdateRenderQuene(const CEntityCamera *pCamera)
	{
		//
		// 1. ������ȫ���
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return;
		}

		//
		// 2. ������Ⱦ����
		//
		const CEffect::ParticleSystemMap &particleSystems = m_ptrEffect->GetParticleSystems();

		for (CEffect::ParticleSystemMap::const_iterator itParticleSystem = particleSystems.begin(); itParticleSystem != particleSystems.end(); ++itParticleSystem) {
			CEffect::PARTICLESYSTEM *pParticleSystem = itParticleSystem->second;
			ASSERT(pParticleSystem);
			ASSERT(pParticleSystem->pParticleSystem);

			if (pParticleSystem->pParticleSystem->IsPlayFinish() == FALSE) {
				m_pSceneManager->AddSceneRenderQuene(pCamera, this, pParticleSystem->pParticleSystem->GetMaterial().GetPointer());
			}
		}
	}

	//
	// ��Ⱦ
	//
	VOID CEntityEffect::Render(const CMaterial *pMaterial) const
	{
		//
		// 1. ������ȫ���
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return;
		}

		//
		// 2. ��Ⱦ
		//
		const CEffect::ParticleSystemMap &particleSystems = m_ptrEffect->GetParticleSystems();

		for (CEffect::ParticleSystemMap::const_iterator itParticleSystem = particleSystems.begin(); itParticleSystem != particleSystems.end(); ++itParticleSystem) {
			CEffect::PARTICLESYSTEM *pParticleSystem = itParticleSystem->second;
			ASSERT(pParticleSystem);
			ASSERT(pParticleSystem->pParticleSystem);

			if (pParticleSystem->pParticleSystem->IsPlayFinish() == FALSE) {
				if (pParticleSystem->pParticleSystem->GetMaterial().GetPointer() == pMaterial) {
					pParticleSystem->pParticleSystem->Render(m_pSceneManager->GetMainCameraEntity());
				}
			}
		}
	}

}

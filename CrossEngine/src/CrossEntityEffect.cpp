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
	// 获得类型
	//
	CEntity::TYPE CEntityEffect::GetType(VOID) const
	{
		return CEntity::EFFECT;
	}

	//
	// 获得可渲染
	//
	BOOL CEntityEffect::IsRenderable(VOID) const
	{
		return TRUE;
	}

	//
	// 设置特效
	//
	BOOL CEntityEffect::SetEffect(const CHAR *szName)
	{
		ASSERT(szName);
		return SetEffect(HashValue(szName));
	}

	//
	// 设置特效
	//
	BOOL CEntityEffect::SetEffect(DWORD dwName)
	{
		return SetEffect(EffectManager()->QueryResource(dwName));
	}

	//
	// 设置特效
	//
	BOOL CEntityEffect::SetEffect(const CEffectPtr &ptrEffect)
	{
		//
		// 1. 参数安全检查
		//
		if (ptrEffect.IsNull() || ptrEffect->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 设置特效
		//
		m_ptrEffect = CEffectPtr(EffectManager()->CreateResource());
		m_ptrEffect->CopyFrom(ptrEffect.GetPointer());
		m_ptrEffect->SetName(Engine()->GetNextEffectName());

		//
		// 3. 设置包围盒
		//
		AABBCopy(&m_aabbLocal, m_ptrEffect->GetAABB());

		return TRUE;
	}

	//
	// 获得特效
	//
	const CEffectPtr& CEntityEffect::GetEffect(VOID) const
	{
		return m_ptrEffect;
	}

	//
	// 播放
	//
	VOID CEntityEffect::Play(FLOAT beginTime, FLOAT durationTime, BOOL bImmediately)
	{
		//
		// 1. 参数安全检查
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return;
		}

		//
		// 2. 停止
		//
		Stop(bImmediately);

		//
		// 3. 播放
		//
		m_bPlaying = TRUE;
		m_bPause = FALSE;

		m_currTime = 0.0f;
		m_beginTime = beginTime;
		m_durationTime = durationTime;
	}

	//
	// 停止
	//
	VOID CEntityEffect::Stop(BOOL bImmediately)
	{
		//
		// 1. 参数安全检查
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return;
		}

		//
		// 2. 停止
		//
		m_bPlaying = FALSE;

		//
		// 3. 停止粒子系统和声音
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
	// 暂停
	//
	VOID CEntityEffect::Pause(VOID)
	{
		//
		// 1. 参数安全检查
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return;
		}

		//
		// 2. 暂停
		//
		m_bPause = TRUE;

		//
		// 3. 暂停粒子系统和声音
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
	// 继续
	//
	VOID CEntityEffect::Resume(VOID)
	{
		//
		// 1. 参数安全检查
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return;
		}

		//
		// 2. 继续
		//
		m_bPause = FALSE;

		//
		// 3. 继续粒子系统和声音
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
	// 播放状态
	//
	BOOL CEntityEffect::IsPlaying(VOID)
	{
		return m_bPlaying && m_bPause == FALSE;
	}

	//
	// 更新到下一帧
	//
	BOOL CEntityEffect::NextFrame(FLOAT deltaTime)
	{
		//
		// 1. 参数安全检查
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 时间递进
		//
		if (IsPlaying()) {
			m_currTime += deltaTime;
		}

		//
		// 3. 更新到下一帧
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

			// 说明: 渲染粒子可不受播放控制, 在播放停止后粒子可随其生命周期自动消亡!!!
			bParticleSystemGfxPlaying = UpdateParticleSystemsGfx(deltaTime);
		}

		return bShakePlaying || bSoundPlaying || bParticleSystemPlaying || bParticleSystemGfxPlaying ? TRUE : FALSE;
	}

	//
	// 更新粒子系统
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

			// 注意: 粒子系统还没开始播放或者循环播放或者还未播放结束均认为是还在播放中!!!
			if (localTime < pParticleSystem->beginTime + pParticleSystem->durationTime || pParticleSystem->durationTime < EPSILON_E3 || pParticleSystem->pParticleSystem->IsPlayFinish() == FALSE) {
				bPlaying |= TRUE;
			}
		}

		return bPlaying;
	}

	//
	// 更新粒子系统图形
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
	// 更新声音
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

			// 注意: 声音还没开始播放或者循环播放均认为是还在播放中!!!
			if (localTime < pSound->beginTime + pSound->durationTime || pSound->durationTime < EPSILON_E3) {
				bPlaying |= TRUE;
			}
		}

		return bPlaying;
	}

	//
	// 更新震动
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

			// 注意: 震动还没开始播放或者循环播放均认为是还在播放中!!!
			if (localTime < pShake->beginTime + pShake->durationTime || pShake->durationTime < EPSILON_E3) {
				bPlaying |= TRUE;
			}

			m_pSceneManager->GetMainCameraEntity()->SetShake(x, y, z);
		}

		return bPlaying;
	}

	//
	// 更新渲染队列
	//
	VOID CEntityEffect::UpdateRenderQuene(const CEntityCamera *pCamera)
	{
		//
		// 1. 参数安全检查
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return;
		}

		//
		// 2. 更新渲染队列
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
	// 渲染
	//
	VOID CEntityEffect::Render(const CMaterial *pMaterial) const
	{
		//
		// 1. 参数安全检查
		//
		if (m_ptrEffect.IsNull() || m_ptrEffect->IsValid() == FALSE) {
			return;
		}

		//
		// 2. 渲染
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

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

	inline BOOL ERRCHECK(FMOD_RESULT result)
	{
		if (result != FMOD_OK) {
			WriteLogE("FMod: (%d) %s\n", result, FMOD_ErrorString(result));
			return TRUE;
		}

		return FALSE;
	}

	inline BOOL ERRCHECK_FALSE(FMOD_RESULT result)
	{
		if (result != FMOD_OK) {
			WriteLogE("FMod: (%d) %s\n", result, FMOD_ErrorString(result));
			return TRUE;
		}

		return FALSE;
	}

	inline BOOL ERRCHECK_NULL(FMOD_RESULT result)
	{
		if (result != FMOD_OK) {
			WriteLogE("FMod: (%d) %s\n", result, FMOD_ErrorString(result));
			return TRUE;
		}

		return FALSE;
	}

	CSoundSystem::CSoundSystem(DWORD dwMemPoolSize, INT maxChannels)
		: m_pMemPool(NULL)
		, m_pSoundSystem(NULL)
	{
		Init(dwMemPoolSize, maxChannels);
	}

	CSoundSystem::~CSoundSystem(VOID)
	{
		Free();
	}

	//
	// ��ʼ������ϵͳ
	//
	BOOL CSoundSystem::Init(DWORD dwMemPoolSize, INT maxChannels)
	{
		FMOD_RESULT result;

		//
		// 1. ��������ϵͳ
		//
		if (dwMemPoolSize) {
			m_pMemPool = (VOID *)SAFE_MALLOC(dwMemPoolSize, MEMTYPE_HEAP);
			if (m_pMemPool == NULL) return FALSE;

			result = FMOD_Memory_Initialize(m_pMemPool, dwMemPoolSize, NULL, NULL, NULL, FMOD_MEMORY_ALL);
			if (ERRCHECK_FALSE(result)) return FALSE;
		}
		else {
			result = FMOD_Memory_Initialize(NULL, 0, NULL, NULL, NULL, FMOD_MEMORY_ALL);
			if (ERRCHECK_FALSE(result)) return FALSE;
		}

		result = FMOD_System_Create(&m_pSoundSystem);
		if (ERRCHECK_FALSE(result)) return FALSE;

		//
		// 2. ���汾
		//
		UINT version;

		result = FMOD_System_GetVersion(m_pSoundSystem, &version);
		if (ERRCHECK_FALSE(result)) return FALSE;

		WriteLogI("FMod Version: %08x\n", version);

		if (version < FMOD_VERSION) {
			WriteLogE("Error!  You are using an old version of FMOD %08x. This program requires %08x\n", version, FMOD_VERSION);
			return FALSE;
		}

		//
		// 3. �������
		//
		INT numDrivers;

		result = FMOD_System_GetNumDrivers(m_pSoundSystem, &numDrivers);
		if (ERRCHECK_FALSE(result)) return FALSE;

		if (numDrivers == 0) {
			result = FMOD_System_SetOutput(m_pSoundSystem, FMOD_OUTPUTTYPE_NOSOUND);
			if (ERRCHECK_FALSE(result)) return FALSE;
		}
		else {
			CHAR szName[256];

			FMOD_CAPS caps;
			FMOD_SPEAKERMODE speakerMode;

			result = FMOD_System_GetDriverCaps(m_pSoundSystem, 0, &caps, 0, &speakerMode);
			if (ERRCHECK_FALSE(result)) return FALSE;

			result = FMOD_System_SetSpeakerMode(m_pSoundSystem, speakerMode);
			if (ERRCHECK_FALSE(result)) return FALSE;

			if (caps & FMOD_CAPS_HARDWARE_EMULATED) {
				result = FMOD_System_SetDSPBufferSize(m_pSoundSystem, 1024, 10);
				if (ERRCHECK_FALSE(result)) return FALSE;
			}

			result = FMOD_System_GetDriverInfo(m_pSoundSystem, 0, szName, 256, 0);
			if (ERRCHECK_FALSE(result)) return FALSE;

			WriteLogI("Sound Device: %s\n", szName);
			WriteLogI("\n");
		}

		//
		// 4. ��ʼ������ϵͳ
		//
		result = FMOD_System_Init(m_pSoundSystem, maxChannels, FMOD_INIT_NORMAL, NULL);
		if (ERRCHECK_FALSE(result)) return FALSE;

		//
		// 5. ����3D����
		//
		Set3DSettings(1.0f, 1.0f, 1.0f);

		return TRUE;
	}

	//
	// �ͷ�����ϵͳ
	//
	VOID CSoundSystem::Free(VOID)
	{
		//
		// 1. ����ϵͳ���
		//
		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. �ͷ�����ϵͳ
		//
		FMOD_System_Close(m_pSoundSystem);
		FMOD_System_Release(m_pSoundSystem);
		SAFE_FREE(m_pMemPool);

		m_pMemPool = NULL;
		m_pSoundSystem = NULL;

		//
		// 3. �������ͨ������
		//
		m_channels.clear();
	}

	//
	// ��Ч��
	//
	BOOL CSoundSystem::IsValid(VOID) const
	{
		return m_pSoundSystem ? TRUE : FALSE;
	}

	//
	// ����������
	//
	BOOL CSoundSystem::SetSpeakerMode(FMOD_SPEAKERMODE speakerMode) const
	{
		//
		// 1. ����ϵͳ���
		//
		if (IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. ����������
		//
		FMOD_RESULT result;

		result = FMOD_System_SetSpeakerMode(m_pSoundSystem, speakerMode);
		if (ERRCHECK_FALSE(result)) return FALSE;

		return TRUE;
	}

	//
	// ��������
	//
	FMOD_SOUND* CSoundSystem::CreateSound(const CHAR *szName, BOOL b3D) const
	{
		//
		// 1. ������ȫ���
		//
		if (szName == NULL) {
			return NULL;
		}

		if (IsValid() == FALSE) {
			return NULL;
		}

		//
		// 2. ��������
		//
		FMOD_RESULT result;
		FMOD_SOUND *pSound = NULL;
		DWORD dwFlag = b3D ? FMOD_3D : FMOD_2D;

		result = FMOD_System_CreateSound(m_pSoundSystem, szName, FMOD_HARDWARE | dwFlag, NULL, &pSound);
		if (ERRCHECK_NULL(result)) return NULL;

		result = FMOD_Sound_Set3DMinMaxDistance(pSound, 0.01f, 1000.0f);
		if (ERRCHECK_NULL(result)) return NULL;

		return pSound;
	}

	//
	// ��������
	//
	FMOD_SOUND* CSoundSystem::CreateSound(const BYTE *pData, DWORD dwLength, BOOL b3D) const
	{
		//
		// 1. ������ȫ���
		//
		if (pData == NULL) {
			return NULL;
		}

		if (IsValid() == FALSE) {
			return NULL;
		}

		//
		// 2. ��������
		//
		FMOD_RESULT result;
		FMOD_SOUND *pSound = NULL;
		FMOD_CREATESOUNDEXINFO exInfo;
		DWORD dwFlag = b3D ? FMOD_3D : FMOD_2D;

		memset(&exInfo, 0, sizeof(exInfo));
		exInfo.cbsize = sizeof(exInfo);
		exInfo.length = dwLength;

		result = FMOD_System_CreateStream(m_pSoundSystem, (const CHAR *)pData, FMOD_HARDWARE | FMOD_OPENMEMORY | dwFlag, &exInfo, &pSound);
		if (ERRCHECK_NULL(result)) return NULL;

		result = FMOD_Sound_Set3DMinMaxDistance(pSound, 0.01f, 1000.0f);
		if (ERRCHECK_NULL(result)) return NULL;

		return pSound;
	}

	//
	// �ͷ�����
	//
	VOID CSoundSystem::ReleaseSound(FMOD_SOUND *pSound) const
	{
		//
		// 1. ������ȫ���
		//
		if (pSound == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. �ͷ�����
		//
		FMOD_Sound_Release(pSound);
	}

	//
	// �����������
	//
	FLOAT CSoundSystem::GetSoundLength(FMOD_SOUND *pSound) const
	{
		//
		// 1. ������ȫ���
		//
		if (pSound == NULL) {
			return 0.0f;
		}

		if (IsValid() == FALSE) {
			return 0.0f;
		}

		//
		// 2. �����������
		//
		UINT length = 0;
		FMOD_RESULT result;

		result = FMOD_Sound_GetLength(pSound, &length, FMOD_TIMEUNIT_MS);
		ERRCHECK(result);

		return length / 1000.0f;
	}

	//
	// ��������
	//
	FMOD_CHANNEL* CSoundSystem::Play(FMOD_SOUND *pSound, BOOL bPaused, BOOL bLoop, BOOL b3D, BOOL bReuseChannel)
	{
		//
		// 1. ������ȫ���
		//
		if (pSound == NULL) {
			return NULL;
		}

		if (IsValid() == FALSE) {
			return NULL;
		}

		//
		// 2. ��������
		//
		FMOD_RESULT result;
		FMOD_CHANNEL *pChannel = NULL;
		FMOD_CHANNELINDEX channelIndex = bReuseChannel ? FMOD_CHANNEL_REUSE : FMOD_CHANNEL_FREE;
		DWORD dwMode = (bLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF) | (b3D ? FMOD_3D : FMOD_2D);

		result = FMOD_Sound_SetMode(pSound, dwMode);
		ERRCHECK(result);

		result = FMOD_System_PlaySound(m_pSoundSystem, channelIndex, pSound, bPaused, &pChannel);
		if (ERRCHECK_NULL(result)) return NULL;

		result = FMOD_Channel_SetVolume(pChannel, 1.0f);
		if (ERRCHECK_NULL(result)) return NULL;

		//
		// 3. ��¼������ͨ��
		//
		m_channels[pChannel] = pChannel;

		return pChannel;
	}

	//
	// ֹͣ����
	//
	VOID CSoundSystem::Stop(FMOD_CHANNEL *pChannel)
	{
		//
		// 1. ������ȫ���
		//
		if (pChannel == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. ֹͣ����
		//
		FMOD_RESULT result;

		result = FMOD_Channel_Stop(pChannel);
		ERRCHECK(result);

		//
		// 3. �Ƴ�������ͨ��
		//
		ChannelMap::iterator itChannel = m_channels.find(pChannel);
		if (itChannel != m_channels.end()) m_channels.erase(pChannel);
	}

	//
	// ��ͣ����
	//
	VOID CSoundSystem::Pause(FMOD_CHANNEL *pChannel)
	{
		//
		// 1. ������ȫ���
		//
		if (pChannel == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. ��ͣ����
		//
		FMOD_RESULT result;

		result = FMOD_Channel_SetPaused(pChannel, TRUE);
		ERRCHECK(result);
	}

	//
	// ��������
	//
	VOID CSoundSystem::Resume(FMOD_CHANNEL *pChannel)
	{
		//
		// 1. ������ȫ���
		//
		if (pChannel == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. ��������
		//
		FMOD_RESULT result;

		result = FMOD_Channel_SetPaused(pChannel, FALSE);
		ERRCHECK(result);
	}

	//
	// ����״̬
	//
	BOOL CSoundSystem::IsPlaying(FMOD_CHANNEL *pChannel) const
	{
		//
		// 1. ������ȫ���
		//
		if (pChannel == NULL) {
			return FALSE;
		}

		if (IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. ����״̬
		//
		FMOD_BOOL bPlaying;
		FMOD_RESULT result;

		result = FMOD_Channel_IsPlaying(pChannel, &bPlaying);
		if (ERRCHECK_FALSE(result)) return FALSE;

		return bPlaying;
	}

	//
	// ������������
	//
	VOID CSoundSystem::SetSoundVolume(FMOD_CHANNEL *pChannel, FLOAT volume) const
	{
		//
		// 1. ������ȫ���
		//
		if (pChannel == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. ������������
		//
		FMOD_RESULT result;

		result = FMOD_Channel_SetVolume(pChannel, volume);
		ERRCHECK(result);
	}

	//
	// �����������
	//
	FLOAT CSoundSystem::GetSoundVolume(FMOD_CHANNEL *pChannel) const
	{
		//
		// 1. ������ȫ���
		//
		if (pChannel == NULL) {
			return 0.0f;
		}

		if (IsValid() == FALSE) {
			return 0.0f;
		}

		//
		// 2. �����������
		//
		FLOAT volume;
		FMOD_RESULT result;

		result = FMOD_Channel_GetVolume(pChannel, &volume);
		if (ERRCHECK_FALSE(result)) return 0.0f;

		return volume;
	}

	//
	// ��������Ƶ��
	//
	VOID CSoundSystem::SetSoundFrequency(FMOD_CHANNEL *pChannel, FLOAT frequency) const
	{
		//
		// 1. ������ȫ���
		//
		if (pChannel == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. ��������Ƶ��
		//
		FMOD_RESULT result;

		result = FMOD_Channel_SetFrequency(pChannel, frequency);
		ERRCHECK_FALSE(result);
	}

	//
	// �������Ƶ��
	//
	FLOAT CSoundSystem::GetSoundFrequency(FMOD_CHANNEL *pChannel) const
	{
		//
		// 1. ������ȫ���
		//
		if (pChannel == NULL) {
			return 0.0f;
		}

		if (IsValid() == FALSE) {
			return 0.0f;
		}

		//
		// 2. �������Ƶ��
		//
		FLOAT frequency;
		FMOD_RESULT result;

		result = FMOD_Channel_GetFrequency(pChannel, &frequency);
		if (ERRCHECK_FALSE(result)) return 0.0f;

		return frequency;
	}

	//
	// ��������λ��
	//
	VOID CSoundSystem::SetSoundPosition(FMOD_CHANNEL *pChannel, const VEC3 *position) const
	{
		//
		// 1. ������ȫ���
		//
		if (pChannel == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. ��������λ��
		//
		FMOD_RESULT result;

		FMOD_VECTOR fmodPos = { (*position)[0], (*position)[1], (*position)[2] };
		FMOD_VECTOR fmodVel = { 0.0f, 0.0f, 0.0f };

		result = FMOD_Channel_Set3DAttributes(pChannel, &fmodPos, &fmodVel);
		ERRCHECK(result);
	}

	//
	// ���ü�����λ��
	//
	VOID CSoundSystem::SetListenerPosition(const VEC3 *position, const VEC3 *forward, const VEC3 *up) const
	{
		//
		// 1. ������ȫ���
		//
		if (position == NULL || forward == NULL || up == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. ���ü�����λ��
		//
		FMOD_RESULT result;

		FMOD_VECTOR fmodPos = { (*position)[0], (*position)[1], (*position)[2] };
		FMOD_VECTOR fmodForward = { (*forward)[0], (*forward)[1], (*forward)[2] };
		FMOD_VECTOR fmodUp = { (*up)[0], (*up)[1], (*up)[2] };

		result = FMOD_System_Set3DListenerAttributes(m_pSoundSystem, 0, &fmodPos, NULL, &fmodForward, &fmodUp);
		ERRCHECK(result);
	}

	//
	// ����3D����
	//
	VOID CSoundSystem::Set3DSettings(FLOAT dopplerScale, FLOAT distanceFactor, FLOAT rolloffScale) const
	{
		//
		// 1. ������ȫ���
		//
		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. ����3D����
		//
		FMOD_RESULT result;

		result = FMOD_System_Set3DSettings(m_pSoundSystem, dopplerScale, distanceFactor, rolloffScale);
		ERRCHECK(result);
	}

	//
	// ����
	//
	VOID CSoundSystem::Update(FLOAT deltaTime)
	{
		if (IsValid()) {
			FMOD_System_Update(m_pSoundSystem);
		}
	}

	//
	// ��ͣ
	//
	VOID CSoundSystem::Pause(VOID)
	{
		for (ChannelMap::const_iterator itChannel = m_channels.begin(); itChannel != m_channels.end(); ++itChannel) {
			FMOD_RESULT result = FMOD_Channel_SetPaused(itChannel->second, TRUE);
			ERRCHECK(result);
		}
	}

	//
	// ����
	//
	VOID CSoundSystem::Resume(VOID)
	{
		for (ChannelMap::const_iterator itChannel = m_channels.begin(); itChannel != m_channels.end(); ++itChannel) {
			FMOD_RESULT result = FMOD_Channel_SetPaused(itChannel->second, FALSE);
			ERRCHECK(result);
		}
	}

	//
	// ����ڴ�ʹ�����
	//
	INT CSoundSystem::GetMemUsage(VOID) const
	{
		INT memAlloced = 0;

		FMOD_Memory_GetStats(&memAlloced, NULL, FALSE);
		return memAlloced;
	}

	//
	// ���CPUʹ�����
	//
	FLOAT CSoundSystem::GetCpuUsage(VOID) const
	{
		FLOAT cpuUsage = 0.0f;

		FMOD_System_GetCPUUsage(m_pSoundSystem, NULL, NULL, NULL, NULL, &cpuUsage);
		return cpuUsage;
	}

}

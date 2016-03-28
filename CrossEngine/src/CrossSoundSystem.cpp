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
	// 初始化声音系统
	//
	BOOL CSoundSystem::Init(DWORD dwMemPoolSize, INT maxChannels)
	{
		FMOD_RESULT result;

		//
		// 1. 创建声音系统
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
		// 2. 检查版本
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
		// 3. 驱动检测
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
		// 4. 初始化声音系统
		//
		result = FMOD_System_Init(m_pSoundSystem, maxChannels, FMOD_INIT_NORMAL, NULL);
		if (ERRCHECK_FALSE(result)) return FALSE;

		//
		// 5. 设置3D参数
		//
		Set3DSettings(1.0f, 1.0f, 1.0f);

		return TRUE;
	}

	//
	// 释放声音系统
	//
	VOID CSoundSystem::Free(VOID)
	{
		//
		// 1. 声音系统检查
		//
		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. 释放声音系统
		//
		FMOD_System_Close(m_pSoundSystem);
		FMOD_System_Release(m_pSoundSystem);
		SAFE_FREE(m_pMemPool);

		m_pMemPool = NULL;
		m_pSoundSystem = NULL;

		//
		// 3. 清空声音通道集合
		//
		m_channels.clear();
	}

	//
	// 有效性
	//
	BOOL CSoundSystem::IsValid(VOID) const
	{
		return m_pSoundSystem ? TRUE : FALSE;
	}

	//
	// 设置扬声器
	//
	BOOL CSoundSystem::SetSpeakerMode(FMOD_SPEAKERMODE speakerMode) const
	{
		//
		// 1. 声音系统检查
		//
		if (IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 设置扬声器
		//
		FMOD_RESULT result;

		result = FMOD_System_SetSpeakerMode(m_pSoundSystem, speakerMode);
		if (ERRCHECK_FALSE(result)) return FALSE;

		return TRUE;
	}

	//
	// 创建声音
	//
	FMOD_SOUND* CSoundSystem::CreateSound(const CHAR *szName, BOOL b3D) const
	{
		//
		// 1. 参数安全检查
		//
		if (szName == NULL) {
			return NULL;
		}

		if (IsValid() == FALSE) {
			return NULL;
		}

		//
		// 2. 创建声音
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
	// 创建声音
	//
	FMOD_SOUND* CSoundSystem::CreateSound(const BYTE *pData, DWORD dwLength, BOOL b3D) const
	{
		//
		// 1. 参数安全检查
		//
		if (pData == NULL) {
			return NULL;
		}

		if (IsValid() == FALSE) {
			return NULL;
		}

		//
		// 2. 创建声音
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
	// 释放声音
	//
	VOID CSoundSystem::ReleaseSound(FMOD_SOUND *pSound) const
	{
		//
		// 1. 参数安全检查
		//
		if (pSound == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. 释放声音
		//
		FMOD_Sound_Release(pSound);
	}

	//
	// 获得声音长度
	//
	FLOAT CSoundSystem::GetSoundLength(FMOD_SOUND *pSound) const
	{
		//
		// 1. 参数安全检查
		//
		if (pSound == NULL) {
			return 0.0f;
		}

		if (IsValid() == FALSE) {
			return 0.0f;
		}

		//
		// 2. 获得声音长度
		//
		UINT length = 0;
		FMOD_RESULT result;

		result = FMOD_Sound_GetLength(pSound, &length, FMOD_TIMEUNIT_MS);
		ERRCHECK(result);

		return length / 1000.0f;
	}

	//
	// 播放声音
	//
	FMOD_CHANNEL* CSoundSystem::Play(FMOD_SOUND *pSound, BOOL bPaused, BOOL bLoop, BOOL b3D, BOOL bReuseChannel)
	{
		//
		// 1. 参数安全检查
		//
		if (pSound == NULL) {
			return NULL;
		}

		if (IsValid() == FALSE) {
			return NULL;
		}

		//
		// 2. 播放声音
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
		// 3. 记录播声音通道
		//
		m_channels[pChannel] = pChannel;

		return pChannel;
	}

	//
	// 停止播放
	//
	VOID CSoundSystem::Stop(FMOD_CHANNEL *pChannel)
	{
		//
		// 1. 参数安全检查
		//
		if (pChannel == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. 停止播放
		//
		FMOD_RESULT result;

		result = FMOD_Channel_Stop(pChannel);
		ERRCHECK(result);

		//
		// 3. 移除播声音通道
		//
		ChannelMap::iterator itChannel = m_channels.find(pChannel);
		if (itChannel != m_channels.end()) m_channels.erase(pChannel);
	}

	//
	// 暂停播放
	//
	VOID CSoundSystem::Pause(FMOD_CHANNEL *pChannel)
	{
		//
		// 1. 参数安全检查
		//
		if (pChannel == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. 暂停播放
		//
		FMOD_RESULT result;

		result = FMOD_Channel_SetPaused(pChannel, TRUE);
		ERRCHECK(result);
	}

	//
	// 继续播放
	//
	VOID CSoundSystem::Resume(FMOD_CHANNEL *pChannel)
	{
		//
		// 1. 参数安全检查
		//
		if (pChannel == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. 继续播放
		//
		FMOD_RESULT result;

		result = FMOD_Channel_SetPaused(pChannel, FALSE);
		ERRCHECK(result);
	}

	//
	// 播放状态
	//
	BOOL CSoundSystem::IsPlaying(FMOD_CHANNEL *pChannel) const
	{
		//
		// 1. 参数安全检查
		//
		if (pChannel == NULL) {
			return FALSE;
		}

		if (IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 播放状态
		//
		FMOD_BOOL bPlaying;
		FMOD_RESULT result;

		result = FMOD_Channel_IsPlaying(pChannel, &bPlaying);
		if (ERRCHECK_FALSE(result)) return FALSE;

		return bPlaying;
	}

	//
	// 设置声音音量
	//
	VOID CSoundSystem::SetSoundVolume(FMOD_CHANNEL *pChannel, FLOAT volume) const
	{
		//
		// 1. 参数安全检查
		//
		if (pChannel == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. 设置声音音量
		//
		FMOD_RESULT result;

		result = FMOD_Channel_SetVolume(pChannel, volume);
		ERRCHECK(result);
	}

	//
	// 获得声音音量
	//
	FLOAT CSoundSystem::GetSoundVolume(FMOD_CHANNEL *pChannel) const
	{
		//
		// 1. 参数安全检查
		//
		if (pChannel == NULL) {
			return 0.0f;
		}

		if (IsValid() == FALSE) {
			return 0.0f;
		}

		//
		// 2. 获得声音音量
		//
		FLOAT volume;
		FMOD_RESULT result;

		result = FMOD_Channel_GetVolume(pChannel, &volume);
		if (ERRCHECK_FALSE(result)) return 0.0f;

		return volume;
	}

	//
	// 设置声音频率
	//
	VOID CSoundSystem::SetSoundFrequency(FMOD_CHANNEL *pChannel, FLOAT frequency) const
	{
		//
		// 1. 参数安全检查
		//
		if (pChannel == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. 设置声音频率
		//
		FMOD_RESULT result;

		result = FMOD_Channel_SetFrequency(pChannel, frequency);
		ERRCHECK_FALSE(result);
	}

	//
	// 获得声音频率
	//
	FLOAT CSoundSystem::GetSoundFrequency(FMOD_CHANNEL *pChannel) const
	{
		//
		// 1. 参数安全检查
		//
		if (pChannel == NULL) {
			return 0.0f;
		}

		if (IsValid() == FALSE) {
			return 0.0f;
		}

		//
		// 2. 获得声音频率
		//
		FLOAT frequency;
		FMOD_RESULT result;

		result = FMOD_Channel_GetFrequency(pChannel, &frequency);
		if (ERRCHECK_FALSE(result)) return 0.0f;

		return frequency;
	}

	//
	// 设置声音位置
	//
	VOID CSoundSystem::SetSoundPosition(FMOD_CHANNEL *pChannel, const VEC3 *position) const
	{
		//
		// 1. 参数安全检查
		//
		if (pChannel == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. 设置声音位置
		//
		FMOD_RESULT result;

		FMOD_VECTOR fmodPos = { (*position)[0], (*position)[1], (*position)[2] };
		FMOD_VECTOR fmodVel = { 0.0f, 0.0f, 0.0f };

		result = FMOD_Channel_Set3DAttributes(pChannel, &fmodPos, &fmodVel);
		ERRCHECK(result);
	}

	//
	// 设置监听者位置
	//
	VOID CSoundSystem::SetListenerPosition(const VEC3 *position, const VEC3 *forward, const VEC3 *up) const
	{
		//
		// 1. 参数安全检查
		//
		if (position == NULL || forward == NULL || up == NULL) {
			return;
		}

		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. 设置监听者位置
		//
		FMOD_RESULT result;

		FMOD_VECTOR fmodPos = { (*position)[0], (*position)[1], (*position)[2] };
		FMOD_VECTOR fmodForward = { (*forward)[0], (*forward)[1], (*forward)[2] };
		FMOD_VECTOR fmodUp = { (*up)[0], (*up)[1], (*up)[2] };

		result = FMOD_System_Set3DListenerAttributes(m_pSoundSystem, 0, &fmodPos, NULL, &fmodForward, &fmodUp);
		ERRCHECK(result);
	}

	//
	// 设置3D参数
	//
	VOID CSoundSystem::Set3DSettings(FLOAT dopplerScale, FLOAT distanceFactor, FLOAT rolloffScale) const
	{
		//
		// 1. 参数安全检查
		//
		if (IsValid() == FALSE) {
			return;
		}

		//
		// 2. 设置3D参数
		//
		FMOD_RESULT result;

		result = FMOD_System_Set3DSettings(m_pSoundSystem, dopplerScale, distanceFactor, rolloffScale);
		ERRCHECK(result);
	}

	//
	// 更新
	//
	VOID CSoundSystem::Update(FLOAT deltaTime)
	{
		if (IsValid()) {
			FMOD_System_Update(m_pSoundSystem);
		}
	}

	//
	// 暂停
	//
	VOID CSoundSystem::Pause(VOID)
	{
		for (ChannelMap::const_iterator itChannel = m_channels.begin(); itChannel != m_channels.end(); ++itChannel) {
			FMOD_RESULT result = FMOD_Channel_SetPaused(itChannel->second, TRUE);
			ERRCHECK(result);
		}
	}

	//
	// 继续
	//
	VOID CSoundSystem::Resume(VOID)
	{
		for (ChannelMap::const_iterator itChannel = m_channels.begin(); itChannel != m_channels.end(); ++itChannel) {
			FMOD_RESULT result = FMOD_Channel_SetPaused(itChannel->second, FALSE);
			ERRCHECK(result);
		}
	}

	//
	// 获得内存使用情况
	//
	INT CSoundSystem::GetMemUsage(VOID) const
	{
		INT memAlloced = 0;

		FMOD_Memory_GetStats(&memAlloced, NULL, FALSE);
		return memAlloced;
	}

	//
	// 获得CPU使用情况
	//
	FLOAT CSoundSystem::GetCpuUsage(VOID) const
	{
		FLOAT cpuUsage = 0.0f;

		FMOD_System_GetCPUUsage(m_pSoundSystem, NULL, NULL, NULL, NULL, &cpuUsage);
		return cpuUsage;
	}

}

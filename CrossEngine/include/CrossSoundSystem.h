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

	class _CrossExport CSoundSystem
	{
		friend class CEngine;


		// 数据结构
	public:
		typedef std::map<FMOD_CHANNEL*, FMOD_CHANNEL*> ChannelMap;                                 // 声音通道集合


		// 构造/析构函数
	protected:
		CSoundSystem(DWORD dwMemPoolSize, INT maxChannels);
		virtual ~CSoundSystem(VOID);


		// 方法
	public:
		BOOL Init(DWORD dwMemPoolSize, INT maxChannels);                                           // 初始化声音系统
		VOID Free(VOID);                                                                           // 释放声音系统

		BOOL IsValid(VOID) const;                                                                  // 有效性

	public:
		BOOL SetSpeakerMode(FMOD_SPEAKERMODE speakerMode) const;                                   // 设置扬声器

	public:
		FMOD_SOUND* CreateSound(const CHAR *szName, BOOL b3D = FALSE) const;                       // 创建声音
		FMOD_SOUND* CreateSound(const BYTE *pData, DWORD dwLength, BOOL b3D = FALSE) const;        // 创建声音
		VOID ReleaseSound(FMOD_SOUND *pSound) const;                                               // 释放声音

		FLOAT GetSoundLength(FMOD_SOUND *pSound) const;                                            // 获得声音长度

		FMOD_CHANNEL* Play(FMOD_SOUND *pSound, BOOL bPaused, BOOL bLoop, BOOL b3D = FALSE, BOOL bReuseChannel = FALSE); // 播放声音
		VOID Stop(FMOD_CHANNEL *pChannel);                                                         // 停止播放
		VOID Pause(FMOD_CHANNEL *pChannel);                                                        // 暂停播放
		VOID Resume(FMOD_CHANNEL *pChannel);                                                       // 继续播放
		BOOL IsPlaying(FMOD_CHANNEL *pChannel) const;                                              // 播放状态

		VOID SetSoundVolume(FMOD_CHANNEL *pChannel, FLOAT volume) const;                           // 设置声音音量
		FLOAT GetSoundVolume(FMOD_CHANNEL *pChannel) const;                                        // 获得声音音量

		VOID SetSoundFrequency(FMOD_CHANNEL *pChannel, FLOAT frequency) const;                     // 设置声音频率
		FLOAT GetSoundFrequency(FMOD_CHANNEL *pChannel) const;                                     // 获得声音频率

		VOID SetSoundPosition(FMOD_CHANNEL *pChannel, const VEC3 *position) const;                 // 设置声音位置
		VOID SetListenerPosition(const VEC3 *position, const VEC3 *forward, const VEC3 *up) const; // 设置监听者位置
		VOID Set3DSettings(FLOAT dopplerScale, FLOAT distanceFactor, FLOAT rolloffScale) const;    // 设置3D参数

	public:
		VOID Update(FLOAT deltaTime);                                                              // 更新

		VOID Pause(VOID);                                                                          // 暂停
		VOID Resume(VOID);                                                                         // 继续

	public:
		INT GetMemUsage(VOID) const;                                                               // 获得内存使用情况
		FLOAT GetCpuUsage(VOID) const;                                                             // 获得CPU使用情况

		// 属性
	protected:
		VOID *m_pMemPool;                                                                          // 内存池
		FMOD_SYSTEM *m_pSoundSystem;                                                               // 声音系统

	protected:
		ChannelMap m_channels;                                                                     // 播放中的
	};

}

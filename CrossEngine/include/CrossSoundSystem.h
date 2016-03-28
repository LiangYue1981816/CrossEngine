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


		// ���ݽṹ
	public:
		typedef std::map<FMOD_CHANNEL*, FMOD_CHANNEL*> ChannelMap;                                 // ����ͨ������


		// ����/��������
	protected:
		CSoundSystem(DWORD dwMemPoolSize, INT maxChannels);
		virtual ~CSoundSystem(VOID);


		// ����
	public:
		BOOL Init(DWORD dwMemPoolSize, INT maxChannels);                                           // ��ʼ������ϵͳ
		VOID Free(VOID);                                                                           // �ͷ�����ϵͳ

		BOOL IsValid(VOID) const;                                                                  // ��Ч��

	public:
		BOOL SetSpeakerMode(FMOD_SPEAKERMODE speakerMode) const;                                   // ����������

	public:
		FMOD_SOUND* CreateSound(const CHAR *szName, BOOL b3D = FALSE) const;                       // ��������
		FMOD_SOUND* CreateSound(const BYTE *pData, DWORD dwLength, BOOL b3D = FALSE) const;        // ��������
		VOID ReleaseSound(FMOD_SOUND *pSound) const;                                               // �ͷ�����

		FLOAT GetSoundLength(FMOD_SOUND *pSound) const;                                            // �����������

		FMOD_CHANNEL* Play(FMOD_SOUND *pSound, BOOL bPaused, BOOL bLoop, BOOL b3D = FALSE, BOOL bReuseChannel = FALSE); // ��������
		VOID Stop(FMOD_CHANNEL *pChannel);                                                         // ֹͣ����
		VOID Pause(FMOD_CHANNEL *pChannel);                                                        // ��ͣ����
		VOID Resume(FMOD_CHANNEL *pChannel);                                                       // ��������
		BOOL IsPlaying(FMOD_CHANNEL *pChannel) const;                                              // ����״̬

		VOID SetSoundVolume(FMOD_CHANNEL *pChannel, FLOAT volume) const;                           // ������������
		FLOAT GetSoundVolume(FMOD_CHANNEL *pChannel) const;                                        // �����������

		VOID SetSoundFrequency(FMOD_CHANNEL *pChannel, FLOAT frequency) const;                     // ��������Ƶ��
		FLOAT GetSoundFrequency(FMOD_CHANNEL *pChannel) const;                                     // �������Ƶ��

		VOID SetSoundPosition(FMOD_CHANNEL *pChannel, const VEC3 *position) const;                 // ��������λ��
		VOID SetListenerPosition(const VEC3 *position, const VEC3 *forward, const VEC3 *up) const; // ���ü�����λ��
		VOID Set3DSettings(FLOAT dopplerScale, FLOAT distanceFactor, FLOAT rolloffScale) const;    // ����3D����

	public:
		VOID Update(FLOAT deltaTime);                                                              // ����

		VOID Pause(VOID);                                                                          // ��ͣ
		VOID Resume(VOID);                                                                         // ����

	public:
		INT GetMemUsage(VOID) const;                                                               // ����ڴ�ʹ�����
		FLOAT GetCpuUsage(VOID) const;                                                             // ���CPUʹ�����

		// ����
	protected:
		VOID *m_pMemPool;                                                                          // �ڴ��
		FMOD_SYSTEM *m_pSoundSystem;                                                               // ����ϵͳ

	protected:
		ChannelMap m_channels;                                                                     // �����е�
	};

}

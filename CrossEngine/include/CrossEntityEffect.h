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

	class _CrossExport CEntityEffect : public CEntityRenderable
	{
		friend class CSceneManager;


		// 构造/析构函数
	protected:
		CEntityEffect(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntityEffect(VOID);


		// 方法
	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型
		virtual BOOL IsRenderable(VOID) const;                                                     // 获得可渲染

	public:
		BOOL SetEffect(const CHAR *szName);                                                        // 设置特效
		BOOL SetEffect(DWORD dwName);                                                              // 设置特效
		BOOL SetEffect(const CEffectPtr &ptrEffect);                                               // 设置特效

		const CEffectPtr& GetEffect(VOID) const;                                                   // 获得特效

	public:
		VOID Play(FLOAT beginTime, FLOAT durationTime, BOOL bImmediately);                         // 播放
		VOID Stop(BOOL bImmediately = FALSE);                                                      // 停止

		VOID Pause(VOID);                                                                          // 暂停
		VOID Resume(VOID);                                                                         // 继续

		BOOL IsPlaying(VOID);                                                                      // 播放状态

	public:
		BOOL NextFrame(FLOAT deltaTime);                                                           // 更新到下一帧

	protected:
		BOOL UpdateParticleSystems(FLOAT deltaTime);                                               // 更新粒子系统
		BOOL UpdateParticleSystemsGfx(FLOAT deltaTime);                                            // 更新粒子系统图形
		BOOL UpdateSounds(FLOAT deltaTime);                                                        // 更新声音
		BOOL UpdateShakes(FLOAT deltaTime);                                                        // 更新震动

	public:
		virtual VOID UpdateRenderQuene(const CEntityCamera *pCamera);                              // 更新渲染队列
		virtual VOID Render(const CMaterial *pMaterial) const;                                     // 渲染


		// 属性
	protected:
		BOOL m_bPlaying;                                                                           // 播放状态
		BOOL m_bPause;                                                                             // 暂停播放状态

		FLOAT m_currTime;                                                                          // 当前时间
		FLOAT m_beginTime;                                                                         // 开始时间
		FLOAT m_durationTime;                                                                      // 持续时间

	protected:
		CEffectPtr m_ptrEffect;                                                                    // 特效指针
	};

}

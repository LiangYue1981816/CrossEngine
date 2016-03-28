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

	class _CrossExport CAffectorBase
	{
		friend class CParticleSystem;


		// 数据结构
	public:
		typedef enum { BASE, FORCE, COLOR, SCALE, ROTATE, RANDOM, TEXTURE, VELOCITY, KEEPLOCAL } TYPE;   // 影响器类型


		// 构造/析构函数
	protected:
		CAffectorBase(CParticleSystem *pParent);
		virtual ~CAffectorBase(VOID);


		// 方法
	public:
		virtual TYPE GetType(VOID) const { return BASE;  }                                         // 获得类型
		virtual CParticleSystem* GetParticleSystem(VOID) const;                                    // 获得粒子系统

		virtual VOID Init(VOID);                                                                   // 初始化
		virtual VOID Free(VOID);                                                                   // 释放

		virtual VOID LoadXML(TiXmlNode *pAffectorNode);                                            // 加载XML
		virtual VOID SaveXML(TiXmlNode *pAffectorNode) const;                                      // 保存XML

	protected:
		VOID LoadBeginTime(TiXmlNode *pAffectorNode);                                              // 加载开始时间
		VOID LoadEndTime(TiXmlNode *pAffectorNode);                                                // 加载结束时间
		VOID LoadStepTime(TiXmlNode *pAffectorNode);                                               // 加载步进时间

		VOID SaveBeginTime(TiXmlNode *pAffectorNode, const CAffectorBase *pDefaultAffector) const; // 保存开始时间
		VOID SaveEndTime(TiXmlNode *pAffectorNode, const CAffectorBase *pDefaultAffector) const;   // 保存结束时间
		VOID SaveStepTime(TiXmlNode *pAffectorNode, const CAffectorBase *pDefaultAffector) const;  // 保存步进时间


	public:
		VOID SetEnable(BOOL bEnable);                                                              // 设置启用状态
		BOOL IsEnable(VOID) const;                                                                 // 获得启用状态

	public:
		VOID SetStep(FLOAT step);                                                                  // 设置响应间隔
		FLOAT GetStep(VOID) const;                                                                 // 获得响应间隔

		VOID SetBeginTime(FLOAT minTime, FLOAT maxTime);                                           // 设置开始时间
		FLOAT GetMinBeginTime(VOID) const;                                                         // 获得最小开始时间
		FLOAT GetMaxBeginTime(VOID) const;                                                         // 获得最大开始时间

		VOID SetEndTime(FLOAT minTime, FLOAT maxTime);                                             // 设置结束时间
		FLOAT GetMinEndTime(VOID) const;                                                           // 获得最小结束时间
		FLOAT GetMaxEndTime(VOID) const;                                                           // 获得最大结束时间

	public:
		const VEC3* GetWorldScale(VOID);                                                           // 获得世界缩放
		const VEC3* GetWorldPosition(VOID);                                                        // 获得世界位置
		const QUAT* GetWorldOrientation(VOID);                                                     // 获得世界朝向

	public:
		VOID Play(VOID);                                                                           // 播放
		VOID Stop(VOID);                                                                           // 停止

		VOID Pause(VOID);                                                                          // 暂停
		VOID Resume(VOID);                                                                         // 继续

		BOOL IsPlaying(VOID) const;                                                                // 播放状态

	public:
		VOID Update(FLOAT deltaTime);                                                              // 更新

	public:
		BOOL IsCanAffect(const CParticle *pParticle);                                              // 施加影响检查
		virtual VOID Affect(CParticle *pParticle, FLOAT deltaTime) {}                              // 影响粒子


		// 属性
	protected:
		BOOL m_bEnable;                                                                            // 起用状态
		BOOL m_bActive;                                                                            // 活动状态

		BOOL m_bPlaying;                                                                           // 播放状态
		BOOL m_bPause;                                                                             // 暂停播放状态

		FLOAT m_beginTime;                                                                         // 开始时间
		FLOAT m_endTime;                                                                           // 持续时间

	protected:
		FLOAT m_step;                                                                              // 响应间隔, 每时间间隔响应一次
		FLOAT m_delta;                                                                             // 间隔计时

		FLOAT m_minBeginTime;                                                                      // 最小开始时间
		FLOAT m_maxBeginTime;                                                                      // 最大开始时间

		FLOAT m_minEndTime;                                                                        // 最小持续时间
		FLOAT m_maxEndTime;                                                                        // 最大持续时间

	protected:
		CParticleSystem *m_pParent;                                                                // 粒子系统
	};

}

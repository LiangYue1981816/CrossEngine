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

	class _CrossExport CEmitterBase
	{
		friend class CParticleSystem;


		// 数据结构
	public:
		typedef enum { BASE, POINT, BOX, SPHERE } TYPE;                                            // 发射器类型


		// 构造/析构函数
	protected:
		CEmitterBase(CParticleSystem *pParent);
		virtual ~CEmitterBase(VOID);


		// 方法
	public:
		virtual TYPE GetType(VOID) const { return BASE;  };                                        // 获得类型
		virtual CParticleSystem* GetParticleSystem(VOID) const;                                    // 获得粒子系统

		virtual VOID Init(VOID);                                                                   // 初始化
		virtual VOID Free(VOID);                                                                   // 释放

		virtual VOID LoadXML(TiXmlNode *pEmitterNode);                                             // 加载XML
		virtual VOID SaveXML(TiXmlNode *pEmitterNode) const;                                       // 保存XML

	protected:
		VOID LoadEmitRate(TiXmlNode *pEmitterNode);                                                // 加载发射率
		VOID LoadBeginTime(TiXmlNode *pEmitterNode);                                               // 加载开始时间
		VOID LoadDurationTime(TiXmlNode *pEmitterNode);                                            // 加载持续时间
		VOID LoadRadianX(TiXmlNode *pEmitterNode);                                                 // 加载绕X轴发射偏角
		VOID LoadRadianY(TiXmlNode *pEmitterNode);                                                 // 加载绕Y轴发射偏角
		VOID LoadRadianZ(TiXmlNode *pEmitterNode);                                                 // 加载绕Z轴发射偏角
		VOID LoadKeepLocal(TiXmlNode *pEmitterNode);                                               // 加载保持本地位置状态
		VOID LoadLiveTime(TiXmlNode *pEmitterNode);                                                // 加载生存时间
		VOID LoadVelocity(TiXmlNode *pEmitterNode);                                                // 加载速度
		VOID LoadSelfRadian(TiXmlNode *pEmitterNode);                                              // 加载自转角
		VOID LoadScale(TiXmlNode *pEmitterNode);                                                   // 加载缩放
		VOID LoadColor(TiXmlNode *pEmitterNode);                                                   // 加载颜色
		VOID LoadUVOffset(TiXmlNode *pEmitterNode);                                                // 加载纹理偏移

		VOID SaveEmitRate(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;     // 保存发射率
		VOID SaveBeginTime(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;    // 保存开始时间
		VOID SaveDurationTime(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const; // 保存持续时间
		VOID SaveRadianX(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;      // 保存绕X轴发射偏角
		VOID SaveRadianY(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;      // 保存绕Y轴发射偏角
		VOID SaveRadianZ(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;      // 保存绕Z轴发射偏角
		VOID SaveKeepLocal(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;    // 保存保持本地位置状态
		VOID SaveLiveTime(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;     // 保存生存时间
		VOID SaveVelocity(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;     // 保存速度
		VOID SaveSelfRadian(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;   // 保存自转角
		VOID SaveScale(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;        // 保存缩放
		VOID SaveColor(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;        // 保存颜色
		VOID SaveUVOffset(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;     // 保存纹理偏移

	public:
		VOID SetEnable(BOOL bEnable);                                                              // 设置启用状态
		BOOL IsEnable(VOID) const;                                                                 // 获得启用状态
		BOOL IsActive(VOID) const;                                                                 // 获得活动状态

	public:
		VOID SetEmitRate(INT rate);                                                                // 设置发射率
		INT GetEmitRate(VOID) const;                                                               // 获得发射率

		VOID SetBeginTime(FLOAT minTime, FLOAT maxTime);                                           // 设置开始时间
		FLOAT GetMinBeginTime(VOID) const;                                                         // 获得最小开始时间
		FLOAT GetMaxBeginTime(VOID) const;                                                         // 获得最大开始时间

		VOID SetDurationTime(FLOAT minTime, FLOAT maxTime);                                        // 设置持续时间
		FLOAT GetMinDurationTime(VOID) const;                                                      // 获得最小持续时间
		FLOAT GetMaxDurationTime(VOID) const;                                                      // 获得最大持续时间

		VOID SetRadianX(FLOAT minRadian, FLOAT maxRadian);                                         // 设置绕X轴发射偏角
		FLOAT GetMinRadianX(VOID) const;                                                           // 获得最小绕X轴发射偏角
		FLOAT GetMaxRadianX(VOID) const;                                                           // 获得最大绕X轴发射偏角

		VOID SetRadianY(FLOAT minRadian, FLOAT maxRadian);                                         // 设置绕Y轴发射偏角
		FLOAT GetMinRadianY(VOID) const;                                                           // 获得最小绕Y轴发射偏角
		FLOAT GetMaxRadianY(VOID) const;                                                           // 获得最大绕Y轴发射偏角

		VOID SetRadianZ(FLOAT minRadian, FLOAT maxRadian);                                         // 设置绕Z轴发射偏角
		FLOAT GetMinRadianZ(VOID) const;                                                           // 获得最小绕Z轴发射偏角
		FLOAT GetMaxRadianZ(VOID) const;                                                           // 获得最大绕Z轴发射偏角

	public:
		VOID SetKeepLocal(BOOL bKeepLocal);                                                        // 设置保持本地位置状态
		BOOL IsKeepLocal(VOID) const;                                                              // 获得保持本地位置状态

		VOID SetLiveTime(FLOAT minTime, FLOAT maxTime);                                            // 设置生存时间
		FLOAT GetMinLiveTime(VOID) const;                                                          // 获得最小生存时间
		FLOAT GetMaxLiveTime(VOID) const;                                                          // 获得最大生存时间

		VOID SetVelocity(FLOAT minVelocity, FLOAT maxVelocity);                                    // 设置速度
		FLOAT GetMinVelocity(VOID) const;                                                          // 获得最小速度
		FLOAT GetMaxVelocity(VOID) const;                                                          // 设置最大速度

		VOID SetSelfRadian(FLOAT minRadian, FLOAT maxRadian);                                      // 设置自转偏角
		FLOAT GetMinSelfRadian(VOID) const;                                                        // 获得最小自转偏角
		FLOAT GetMaxSelfRadian(VOID) const;                                                        // 获得最大自转偏角

		VOID SetScale(FLOAT minScaleX, FLOAT minScaleY, FLOAT minScaleZ, FLOAT maxScaleX, FLOAT maxScaleY, FLOAT maxScaleZ); // 设置缩放
		VOID SetScale(const VEC3* minScale, const VEC3 *maxScale);                                 // 设置缩放
		const VEC3* GetMinScale(VOID) const;                                                       // 获得最小缩放
		const VEC3* GetMaxScale(VOID) const;                                                       // 获得最大缩放

		VOID SetColor(FLOAT minColorR, FLOAT minColorG, FLOAT minColorB, FLOAT minColorA, FLOAT maxColorR, FLOAT maxColorG, FLOAT maxColorB, FLOAT maxColorA); // 设置颜色
		VOID SetColor(const VEC4* minColor, const VEC4 *maxColor);                                 // 设置颜色
		const VEC4* GetMinColor(VOID) const;                                                       // 获得最小颜色
		const VEC4* GetMaxColor(VOID) const;                                                       // 获得最大颜色

		VOID SetUVOffset(FLOAT minUVOffsetX, FLOAT minUVOffsetY, FLOAT maxUVOffsetX, FLOAT maxUVOffsetY); // 设置纹理偏移
		VOID SetUVOffset(const VEC2* minUVOffset, const VEC2 *maxUVOffset);                        // 设置纹理偏移
		const VEC2* GetMinUVOffset(VOID) const;                                                    // 获得最小纹理偏移
		const VEC2* GetMaxUVOffset(VOID) const;                                                    // 获得最大纹理偏移

	public:
		VOID SetPosition(FLOAT x, FLOAT y, FLOAT z);                                               // 设置位置
		VOID SetPosition(const VEC3 *position);                                                    // 设置位置
		const VEC3* GetPosition(VOID) const;                                                       // 获得位置

		VOID SetDirection(FLOAT x, FLOAT y, FLOAT z);                                              // 设置朝向
		VOID SetDirection(const VEC3 *direction);                                                  // 设置朝向
		VOID SetOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w);                                   // 设置朝向
		VOID SetOrientation(const QUAT *orientation);                                              // 设置朝向
		const QUAT* GetOrientation(VOID) const;                                                    // 获得朝向

		const VEC3* GetWorldScale(VOID);                                                           // 获得世界缩放
		const VEC3* GetWorldPosition(VOID);                                                        // 获得世界位置
		const QUAT* GetWorldOrientation(VOID);                                                     // 获得世界朝向

		INT GetEmitMount(VOID) const;                                                              // 获得发射量

	public:
		VOID Play(VOID);                                                                           // 播放
		VOID Stop(VOID);                                                                           // 停止

		VOID Pause(VOID);                                                                          // 暂停
		VOID Resume(VOID);                                                                         // 继续

		BOOL IsPlaying(VOID) const;                                                                // 播放状态

	public:
		VOID Update(FLOAT deltaTime);                                                              // 更新

	public:
		virtual BOOL Emit(CParticle *pParticle, const VEC3 *localPosition);                        // 发射粒子
		virtual BOOL Emit(CParticle *pParticle) { return FALSE; }                                  // 发射粒子


		// 属性
	protected:
		BOOL m_bEnable;                                                                            // 启用状态
		BOOL m_bActive;                                                                            // 活动状态

		BOOL m_bPlaying;                                                                           // 播放状态
		BOOL m_bPause;                                                                             // 暂停播放状态

		FLOAT m_emitMount;                                                                         // 发射量
		FLOAT m_emitDeltaTime;                                                                     // 发射间隔时间

		FLOAT m_currTime;                                                                          // 当前时间
		FLOAT m_beginTime;                                                                         // 开始时间
		FLOAT m_durationTime;                                                                      // 持续时间

		VEC3 m_worldPosition;                                                                      // 世界坐标系位置
		QUAT m_worldOrientation;                                                                   // 世界坐标系朝向

	protected:
		INT m_emitRate;                                                                            // 发射器发射率

		FLOAT m_minBeginTime;                                                                      // 发射器最小开始时间
		FLOAT m_maxBeginTime;                                                                      // 发射器最大开始时间

		FLOAT m_minDurationTime;                                                                   // 发射器最小持续时间
		FLOAT m_maxDurationTime;                                                                   // 发射器最大持续时间

		FLOAT m_minRadianX;                                                                        // 发射器最小绕X轴发射角度
		FLOAT m_maxRadianX;                                                                        // 发射器最大绕X轴发射角度

		FLOAT m_minRadianY;                                                                        // 发射器最小绕Y轴发射角度
		FLOAT m_maxRadianY;                                                                        // 发射器最大绕Y轴发射角度

		FLOAT m_minRadianZ;                                                                        // 发射器最小绕Z轴发射角度
		FLOAT m_maxRadianZ;                                                                        // 发射器最大绕Z轴发射角度

		VEC3 m_position;                                                                           // 发射器位置
		QUAT m_orientation;                                                                        // 发射器朝向

	protected:
		BOOL m_bKeepLocal;                                                                         // 粒子保持本地位置状态

		FLOAT m_minLiveTime;                                                                       // 粒子最小粒子生存时间
		FLOAT m_maxLiveTime;                                                                       // 粒子最大粒子生存时间

		FLOAT m_minVelocity;                                                                       // 粒子最小速度
		FLOAT m_maxVelocity;                                                                       // 粒子最大速度

		FLOAT m_minSelfRadian;                                                                     // 粒子最小自转角度
		FLOAT m_maxSelfRadian;                                                                     // 粒子最大自转角度

		VEC3 m_minScale;                                                                           // 粒子最小缩放
		VEC3 m_maxScale;                                                                           // 粒子最大缩放

		VEC4 m_minColor;                                                                           // 粒子最小颜色
		VEC4 m_maxColor;                                                                           // 粒子最大颜色

		VEC2 m_minUVOffset;                                                                        // 粒子最小纹理偏移
		VEC2 m_maxUVOffset;                                                                        // 粒子最大纹理偏移

	protected:
		CParticleSystem *m_pParent;                                                                // 粒子系统
	};

}

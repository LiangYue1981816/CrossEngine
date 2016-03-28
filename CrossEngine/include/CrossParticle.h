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

	class _CrossExport CEmitterBase;
	class _CrossExport CAffectorBase;
	class _CrossExport CParticle
	{
		friend class CParticleSystem;


		// 数据结构
	public:
		typedef enum { VIRTUAL, PARTICLE_SYSTEM } TYPE;                                            // 粒子类型


		// 构造/析构函数
	protected:
		CParticle(VOID);
		virtual ~CParticle(VOID);

		VOID Reset(VOID);                                                                          // 重置


		// 属性
	public:
		FLOAT currTime;                                                                            // 当前时间
		FLOAT liveTime;                                                                            // 生存时间

		BOOL bKeepLocal;                                                                           // 保持本地坐标
		                                                                                           // 说明: 相对于发射他的发射器坐标系

		VEC3 localScale;                                                                           // 缩放
		VEC3 localPosition;                                                                        // 位置
		QUAT localOrientation;                                                                     // 朝向
		VEC3 localVelocity;                                                                        // 速度

		FLOAT radian;                                                                              // 旋转角度

		VEC4 color;                                                                                // 颜色
		VEC2 uvOffset;                                                                             // 纹理坐标偏移

		VEC2 texScrollOffset;                                                                      // 纹理滚动偏移
		VEC2 texSequenceScale;                                                                     // 序列帧动画缩放
		VEC2 texSequenceOffset;                                                                    // 序列帧动画偏移

	public:
		std::map<CAffectorBase*, VEC3> deltaScale;                                                 // 缩放变化量
		std::map<CAffectorBase*, VEC4> deltaColor;                                                 // 颜色变化量
		std::map<CAffectorBase*, VEC3> deltaForce;                                                 // 力变化量

		std::map<CAffectorBase*, FLOAT> deltaRotateSpeed;                                          // 旋转速度变化量
		std::map<CAffectorBase*, FLOAT> deltaVelocityRatios;                                       // 速度比率变化量

		std::map<CAffectorBase*, FLOAT> texSequenceTime;                                           // 序列帧动画时间

	public:
		CEmitterBase *pEmitter;                                                                    // 发射器指针
		CParticleSystem *pEmitParticleSystem;                                                      // 发射器粒子系统

	public:
		CParticle *pNext;                                                                          // 链接下一个粒子
	};

}

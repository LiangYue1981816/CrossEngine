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

	class _CrossExport CAffectorTexture : public CAffectorBase
	{
		friend class CParticleSystem;


		// 构造/析构函数
	protected:
		CAffectorTexture(CParticleSystem *pParent);
		virtual ~CAffectorTexture(VOID);


		// 方法
	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型

		virtual VOID Init(VOID);                                                                   // 初始化
		virtual VOID Free(VOID);                                                                   // 释放

		virtual VOID LoadXML(TiXmlNode *pAffectorNode);                                            // 加载XML
		virtual VOID SaveXML(TiXmlNode *pAffectorNode) const;                                      // 保存XML

		virtual VOID Affect(CParticle *pParticle, FLOAT deltaTime);                                // 影响粒子

	public:
		VOID SetTexScrollU(FLOAT u);                                                               // 设置纹理横向滚动
		VOID SetTexScrollV(FLOAT v);                                                               // 设置纹理纵向滚动
		VOID SetTexSequence(UINT cols, UINT rows, FLOAT duration);                                 // 设置纹理序列帧

		FLOAT GetTexScrollU(VOID) const;                                                           // 获得纹理横向滚动
		FLOAT GetTexScrollV(VOID) const;                                                           // 获得纹理纵向滚动
		UINT GetTexSequenceCols(VOID) const;                                                       // 获得纹理序列帧列数
		UINT GetTexSequenceRows(VOID) const;                                                       // 获得纹理序列帧行数
		FLOAT GetTexSequenceDurationTime(VOID) const;                                              // 获得纹理序列帧持续时间


		// 属性
	protected:
		FLOAT m_texScrollU;                                                                        // 横向滚动速度
		FLOAT m_texScrollV;                                                                        // 纵向滚动速度

		UINT m_texSequenceCols;                                                                    // 序列帧列数
		UINT m_texSequenceRows;                                                                    // 序列帧行数
		FLOAT m_texSequenceDurationTime;                                                           // 序列帧持续时间
	};

}

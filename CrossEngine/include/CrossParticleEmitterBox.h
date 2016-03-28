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

	class _CrossExport CEmitterBox : public CEmitterBase
	{
		friend class CParticleSystem;


		// 构造/析构函数
	protected:
		CEmitterBox(CParticleSystem *pParent);
		virtual ~CEmitterBox(VOID);


		// 方法
	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型

		virtual VOID Init(VOID);                                                                   // 初始化
		virtual VOID Free(VOID);                                                                   // 释放

		virtual VOID LoadXML(TiXmlNode *pEmitterNode);                                             // 加载XML
		virtual VOID SaveXML(TiXmlNode *pEmitterNode) const;                                       // 保存XML

		virtual BOOL Emit(CParticle *pParticle);                                                   // 发射粒子

	public:
		VOID SetWidth(FLOAT width);                                                                // 设置盒宽
		VOID SetHeight(FLOAT height);                                                              // 设置盒高
		VOID SetLength(FLOAT length);                                                              // 设置盒长

		FLOAT GetWidth(VOID) const;                                                                // 获得盒宽
		FLOAT GetHeight(VOID) const;                                                               // 获得盒高
		FLOAT GetLength(VOID) const;                                                               // 获得盒长


		// 属性
	protected:
		FLOAT m_width;                                                                             // 盒宽
		FLOAT m_height;                                                                            // 盒高
		FLOAT m_length;                                                                            // 盒长
	};

}

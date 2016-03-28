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

	class _CrossExport CGfxDebug : public CGfxBase
	{
		friend class CParticleSystem;


		// 构造/析构函数
	protected:
		CGfxDebug(VOID);
		virtual ~CGfxDebug(VOID);


		// 方法
	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型

		virtual VOID Init(VOID);                                                                   // 初始化
		virtual VOID Free(VOID);                                                                   // 释放

	public:
		virtual VOID LoadXML(TiXmlNode *pGfxNode);                                                 // 加载XML
		virtual VOID SaveXML(TiXmlNode *pGfxNode) const;                                           // 保存XML

	public:
		virtual VOID SetParticleQuota(INT quota) {};                                               // 设置粒子总量

	public:
		virtual VOID Update(const CEntityCamera *pCamera, CParticle *pParticleList, INT numParticles); // 更新
		virtual VOID Render(INT numParticles);                                                     // 渲染
	};

}

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

	CEmitterPoint::CEmitterPoint(CParticleSystem *pParent)
		: CEmitterBase(pParent)
	{
		Init();
	}

	CEmitterPoint::~CEmitterPoint(VOID)
	{
		Free();
	}

	//
	// 获得类型
	//
	CEmitterBase::TYPE CEmitterPoint::GetType(VOID) const
	{
		return CEmitterBase::POINT;
	}

	//
	// 初始化
	//
	VOID CEmitterPoint::Init(VOID)
	{
		CEmitterBase::Init();
	}

	//
	// 释放
	//
	VOID CEmitterPoint::Free(VOID)
	{
		CEmitterBase::Free();
	}

	//
	// 加载XML
	//
	VOID CEmitterPoint::LoadXML(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);
		CEmitterBase::LoadXML(pEmitterNode);
	}

	//
	// 保存XML
	//
	VOID CEmitterPoint::SaveXML(TiXmlNode *pEmitterNode) const
	{
		ASSERT(pEmitterNode);
		CEmitterBase::SaveXML(pEmitterNode);
	}

	//
	// 发射粒子
	//
	BOOL CEmitterPoint::Emit(CParticle *pParticle)
	{
		VEC3 localPosition;
		Vec3Set(&localPosition, 0.0f, 0.0f, 0.0f);
		return CEmitterBase::Emit(pParticle, &localPosition);
	}

}

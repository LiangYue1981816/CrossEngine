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

	CGfxDebug::CGfxDebug(VOID)
	{
		Init();
	}

	CGfxDebug::~CGfxDebug(VOID)
	{
		Free();
	}

	//
	// 获得类型
	//
	CGfxBase::TYPE CGfxDebug::GetType(VOID) const
	{
		return CGfxBase::GFX_DEBUG;
	}

	//
	// 初始化
	//
	VOID CGfxDebug::Init(VOID)
	{
		CGfxBase::Init();
	}

	//
	// 释放
	//
	VOID CGfxDebug::Free(VOID)
	{
		CGfxBase::Free();
	}

	//
	// 加载XML
	//
	VOID CGfxDebug::LoadXML(TiXmlNode *pGfxNode)
	{
		ASSERT(pGfxNode);
		CGfxBase::LoadXML(pGfxNode);
	}

	//
	// 保存XML
	//
	VOID CGfxDebug::SaveXML(TiXmlNode *pGfxNode) const
	{
		ASSERT(pGfxNode);
		CGfxBase::SaveXML(pGfxNode);
	}

	//
	// 更新
	//
	VOID CGfxDebug::Update(const CEntityCamera *pCamera, CParticle *pParticleList, INT numParticles)
	{

	}

	//
	// 渲染
	//
	VOID CGfxDebug::Render(INT numParticles)
	{

	}

}

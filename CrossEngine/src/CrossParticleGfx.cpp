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

	CGfxBase::CGfxBase(VOID)
		: m_vao(0)
		, m_vbo(0)
		, m_ibo(0)
	{
		Init();
	}

	CGfxBase::~CGfxBase(VOID)
	{
		Free();
	}

	//
	// 初始化
	//
	VOID CGfxBase::Init(VOID)
	{
		m_vao = 0;
		m_vbo = 0;
		m_ibo = 0;
	}

	//
	// 释放
	//
	VOID CGfxBase::Free(VOID)
	{
		Renderer()->DestroyVAO(&m_vao);
		Renderer()->DestroyVBO(&m_vbo);
		Renderer()->DestroyVBO(&m_ibo);

		Init();
	}

	//
	// 加载XML
	//
	VOID CGfxBase::LoadXML(TiXmlNode *pGfxNode)
	{
		ASSERT(pGfxNode);
	}

	//
	// 保存XML
	//
	VOID CGfxBase::SaveXML(TiXmlNode *pGfxNode) const
	{
		ASSERT(pGfxNode);
	}

}

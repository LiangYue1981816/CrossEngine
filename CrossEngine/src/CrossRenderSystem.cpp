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

	CRenderSystem::CRenderSystem(HWND hWnd)
		: m_pRenderer(NULL)
		, m_pRenderFlow(NULL)
		, m_pRenderQuene(NULL)
	{
		m_pRenderer = SAFE_NEW(MEMTYPE_HEAP) CRenderer(hWnd);
		m_pRenderFlow = SAFE_NEW(MEMTYPE_HEAP) CRenderFlow();
		m_pRenderQuene = SAFE_NEW(MEMTYPE_HEAP) CRenderQuene();
	}

	CRenderSystem::~CRenderSystem(VOID)
	{
		SAFE_DELETE(m_pRenderQuene);
		SAFE_DELETE(m_pRenderFlow);
		SAFE_DELETE(m_pRenderer);
	}

	//
	// 获得渲染器
	//
	CRenderer* CRenderSystem::GetRenderer(VOID) const
	{
		return m_pRenderer;
	}

	//
	// 获得渲染流程
	//
	CRenderFlow* CRenderSystem::GetRenderFlow(VOID) const
	{
		return m_pRenderFlow;
	}

	//
	// 获得渲染队列
	//
	CRenderQuene* CRenderSystem::GetRenderQuene(VOID) const
	{
		return m_pRenderQuene;
	}

}

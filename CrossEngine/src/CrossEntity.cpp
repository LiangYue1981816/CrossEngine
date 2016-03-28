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

	CEntity::CEntity(DWORD dwName, CSceneManager *pSceneManager)
		: userData(NULL)

		, m_time(0.0f)
		, m_dwName(dwName)
		, m_dwUpdateFrameCount(0)

		, m_pSceneNode(NULL)
		, m_pSceneManager(pSceneManager)

		, pNext(NULL)
	{
		ASSERT(m_pSceneManager);
	}

	CEntity::~CEntity(VOID)
	{
		if (m_pSceneNode) {
			m_pSceneNode->DetachEntity(this);
		}
	}

	//
	// 获得时间
	//
	FLOAT CEntity::GetTime(VOID) const
	{
		return m_time;
	}

	//
	// 获得名称
	//
	DWORD CEntity::GetName(VOID) const
	{
		return m_dwName;
	}

	//
	// 获得场景节点
	//
	CSceneNode* CEntity::GetSceneNode(VOID) const
	{
		return m_pSceneNode;
	}

	//
	// 获得场景管理器
	//
	CSceneManager* CEntity::GetSceneManager(VOID) const
	{
		return m_pSceneManager;
	}

	//
	// 更新
	//
	VOID CEntity::Update(FLOAT deltaTime)
	{
		//
		// 1. 检查帧计数
		//
		if (m_dwUpdateFrameCount == Engine()->GetFrameCount()) {
			return;
		}

		//
		// 2. 更新
		//
		m_dwUpdateFrameCount = Engine()->GetFrameCount();
		UpdateInstance(deltaTime);
	}

	//
	// 更新
	//
	VOID CEntity::UpdateInstance(FLOAT deltaTime)
	{
		m_time += deltaTime;
	}

}

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

	class _CrossExport CEntity
	{
		friend class CSceneNode;
		friend class CSceneManager;


		// 数据结构
	public:
		typedef enum { CAMERA, MESH, SKIN, EFFECT, HALO, LIGHT, SHADOW } TYPE;                     // 实体类型


		// 构造/析构函数
	protected:
		CEntity(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntity(VOID);


		// 方法
	public:
		FLOAT GetTime(VOID) const;                                                                 // 获得时间
		DWORD GetName(VOID) const;                                                                 // 获得名称

		CSceneNode* GetSceneNode(VOID) const;                                                      // 获得场景节点
		CSceneManager* GetSceneManager(VOID) const;                                                // 获得场景管理器

	public:
		virtual TYPE GetType(VOID) const = 0;                                                      // 获得类型
		virtual BOOL IsRenderable(VOID) const = 0;                                                 // 获得可渲染

	public:
		virtual VOID Update(FLOAT deltaTime);                                                      // 更新
		virtual VOID UpdateInstance(FLOAT deltaTime);                                              // 更新

	public:
		virtual VOID UpdateRenderQuene(const CEntityCamera *pCamera) {}                            // 更新渲染队列


		// 属性
	public:
		VOID *userData;                                                                            // 用户数据

	protected:
		FLOAT m_time;                                                                              // 时间
		DWORD m_dwName;                                                                            // 名称

	protected:
		CSceneNode *m_pSceneNode;                                                                  // 场景节点
		CSceneManager *m_pSceneManager;                                                            // 场景管理器

	protected:
		DWORD m_dwUpdateFrameCount;                                                                // 更新帧计数

	public:
		CEntity *pNext;                                                                            // 实体链表
	};

}

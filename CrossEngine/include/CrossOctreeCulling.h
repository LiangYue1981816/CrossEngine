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

	class _CrossExport COctreeCulling
	{
		friend class CSceneManager;


		// 数据结构
	public:
		typedef struct NODE {
			AABB aabb;                                                                             // AABB
			NODE *pNodes[8];                                                                       // 子节点
			std::vector<CSceneNode*> nodes;                                                        // 场景节点集合
		} NODE;


		// 构造/析构函数
	protected:
		COctreeCulling(CSceneManager *pSceneManager);
		virtual ~COctreeCulling(VOID);


		// 方法
	public:
		VOID Init(VOID);                                                                           // 初始化
		VOID Free(VOID);                                                                           // 释放

	protected:
		VOID Free(NODE *pNode);                                                                    // 释放

	public:
		BOOL LoadFromStream(CStream *pStream);                                                     // 从数据流加载资源

	public:
		VOID UpdateCulling(const CEntityCamera *pCamera) const;                                    // 更新剔除

	protected:
		VOID OctreeCulling(const CEntityCamera *pCamera, NODE *pNode) const;                       // 八叉树剔除
		VOID CameraCulling(const CEntityCamera *pCamera, CSceneNode *pSceneNode) const;            // 相机剔除


		// 属性
	protected:
		NODE *m_pRoot;                                                                             // 根节点

	protected:
		CSceneManager *m_pSceneManager;                                                            // 场景管理器
	};

}

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

	class _CrossExport CSceneSystem
	{
		friend class CEngine;


		// 数据结构
	public:
		typedef std::map<DWORD, CSceneManager*> SceneManagerMap;                                   // 场景管理器集合


		// 构造/析构函数
	protected:
		CSceneSystem(VOID);
		virtual ~CSceneSystem(VOID);


		// 方法
	public:
		BOOL PluginSceneManager(const CHAR *szName, CSceneManager *pSceneManager);                 // 挂入场景管理器
		BOOL PluginSceneManager(DWORD dwName, CSceneManager *pSceneManager);                       // 挂入场景管理器

		CSceneManager* CreateSceneManager(const CHAR *szName);                                     // 创建场景管理器
		CSceneManager* CreateSceneManager(DWORD dwName);                                           // 创建场景管理器

		VOID DestroySceneManager(const CHAR *szName);                                              // 销毁场景管理器
		VOID DestroySceneManager(DWORD dwName);                                                    // 销毁场景管理器

		CSceneManager* GetSceneManager(const CHAR *szName) const;                                  // 获得场景管理器
		CSceneManager* GetSceneManager(DWORD dwName) const;                                        // 获得场景管理器

	public:
		VOID Update(DWORD dwName, FLOAT deltaTime);                                                // 更新


		// 属性
	protected:
		SceneManagerMap m_sceneManagers;                                                           // 场景管理器集合
		SceneManagerMap m_sceneManagersPlugin;                                                     // 场景管理器插件集合
	};

}

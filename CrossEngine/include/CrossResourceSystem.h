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

	class _CrossExport CResourceSystem
	{
		friend class CEngine;


		// 数据结构
	public:
		typedef enum {
			RESOURCE_MANAGER_SOUND = 0,                                                            // 声音资源管理器
			RESOURCE_MANAGER_SHADER,                                                               // 着色器资源管理器
			RESOURCE_MANAGER_TEXTURE,                                                              // 纹理资源管理器
			RESOURCE_MANAGER_MATERIAL,                                                             // 材质资源管理器
			RESOURCE_MANAGER_SKELETON,                                                             // 骨骼资源管理器
			RESOURCE_MANAGER_MESH,                                                                 // 网格资源管理器
			RESOURCE_MANAGER_EFFECT,                                                               // 特效资源管理器
			RESOURCE_MANAGER_HALO,                                                                 // 光晕资源管理器
			RESOURCE_MANAGER_COUNT                                                                 // 资源管理器数
		} RESOURCE_MANAGER;


		// 构造/析构函数
	protected:
		CResourceSystem(VOID);
		virtual ~CResourceSystem(VOID);


		// 方法
	public:
		CResourceManager* GetResourceManager(RESOURCE_MANAGER manager) const;                      // 获得资源管理器

	public:
		CResource* CreateResource(RESOURCE_MANAGER manager);                                       // 创建资源
		VOID DestroyResource(CResource *pResource);                                                // 销毁资源

	public:
		BOOL LoadResourceZip(const CHAR *szZipName);                                               // 加载资源包资源
		BOOL LoadResourcePath(const CHAR *szPathName);                                             // 加载文件夹资源

		BOOL ReloadAll(VOID);                                                                      // 重新加载所有资源
		BOOL ReloadGfx(VOID);                                                                      // 重新加载图形资源

	public:
		VOID GarbageCollection(VOID);                                                              // 垃圾回收


		// 属性
	protected:
		CResourceManager *m_pResourceManager[RESOURCE_MANAGER_COUNT];                              // 资源管理器集合
	};

}

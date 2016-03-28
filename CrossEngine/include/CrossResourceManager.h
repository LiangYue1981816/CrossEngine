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

	class _CrossExport CResourceHandle
	{
		friend class CResourceManager;


		// 构造/析构函数
	protected:
		CResourceHandle(CResourceManager *pResourceManager, const CHAR *szName);
		CResourceHandle(CResourceManager *pResourceManager, const CHAR *szName, const CHAR *szFileName);
		CResourceHandle(CResourceManager *pResourceManager, const CHAR *szName, const CHAR *szFileName, ZZIP_DIR *pZipPack);
		virtual ~CResourceHandle(VOID);


		// 方法
	public:
		BOOL IsWaste(VOID) const;                                                                  // 垃圾资源

		const CHAR* GetName(VOID) const;                                                           // 获得资源名
		const CHAR* GetFileName(VOID) const;                                                       // 获得资源文件名
		CResourcePtr& GetResourcePtr(VOID);                                                        // 获得资源指针

	public:
		BOOL LoadResource(BOOL bReload);                                                           // 加载资源
		VOID FreeResource(VOID);                                                                   // 释放资源
		VOID DeleteResource(VOID);                                                                 // 删除资源


		// 属性
	protected:
		BOOL m_bIsWaste;                                                                           // 垃圾资源

	protected:
		CHAR *m_szName;                                                                            // 资源名
		CHAR *m_szFileName;                                                                        // 资源文件名
		ZZIP_DIR *m_pZipPack;                                                                      // 资源压缩包

	protected:
		CResourcePtr m_ptrResource;                                                                // 资源指针
		CResourceManager *m_pResourceManager;                                                      // 资源管理器
	};

	class _CrossExport CResourceManager
	{
		friend class CResourceSystem;


		// 数据结构
	public:
		typedef std::map<DWORD, ZZIP_DIR*> PackMap;                                                // 包集合
		typedef std::map<DWORD, CResourceHandle*> ResourceMap;                                     // 资源集合


		// 构造/析构函数
	protected:
		CResourceManager(VOID);
		virtual ~CResourceManager(VOID);


		// 方法
	protected:
		virtual VOID Init(VOID);                                                                   // 初始化资源管理器
		virtual VOID Free(VOID);                                                                   // 释放资源管理器

	public:
		virtual CResource::TYPE GetType(VOID) const = 0;                                           // 获得类型

	public:
		virtual CResource* CreateResource(VOID) = 0;                                               // 创建资源
		virtual VOID DestroyResource(CResource *pResource);                                        // 销毁资源

	public:
		const CResourcePtr& QueryResource(const CHAR *szName, BOOL bReload = FALSE);               // 查找资源
		const CResourcePtr& QueryResource(DWORD dwName, BOOL bReload = FALSE);                     // 查找资源

		BOOL FreeResource(const CHAR *szName);                                                     // 释放资源
		BOOL FreeResource(DWORD dwName);                                                           // 释放资源

		BOOL DeleteResource(const CHAR *szName);                                                   // 删除资源
		BOOL DeleteResource(DWORD dwName);                                                         // 删除资源

		const ResourceMap& GetResources(VOID) const;                                               // 获得资源集合

	public:
		virtual const CResourcePtr& CopyFrom(const CHAR *szName, const CResource *pCopyFrom);      // 复制资源

		virtual BOOL LoadFromFile(const CHAR *szFileName) = 0;                                     // 从文件加载
		virtual BOOL LoadFromPath(const CHAR *szPathName) = 0;                                     // 从文件夹加载
		virtual BOOL LoadFromZip(const CHAR *szZipName) = 0;                                       // 从压缩包加载

		virtual BOOL Reload(VOID);                                                                 // 重新加载资源

	protected:
		virtual BOOL LoadFromFile(const CHAR *szFileName, const CHAR *szExtName);                  // 从文件加载
		virtual BOOL LoadFromPath(const CHAR *szPathName, const CHAR *szExtName);                  // 从文件夹加载
		virtual BOOL LoadFromZip(const CHAR *szZipName, const CHAR *szExtName);                    // 从压缩包加载

		virtual BOOL PreLoad(CResourceHandle *pResource);                                          // 预加载资源

	public:
		virtual VOID GarbageCollection(VOID);                                                      // 垃圾回收


		// 属性
	protected:
		PackMap m_packs;                                                                           // 包集合
		ResourceMap m_resources;                                                                   // 资源集合

	protected:
		pthread_mutex_t m_mutex;                                                                   // 互斥量
	};

}

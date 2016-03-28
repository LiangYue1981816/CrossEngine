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

	class _CrossExport CShaderManager : public CResourceManager
	{
		friend class CResourceSystem;


		// 构造/析构函数
	protected:
		CShaderManager(VOID);
		virtual ~CShaderManager(VOID);


		// 方法
	public:
		virtual CResource::TYPE GetType(VOID) const;                                               // 获得类型

	public:
		virtual CResource* CreateResource(VOID);                                                   // 创建资源

		virtual BOOL LoadFromPath(const CHAR *szPathName);                                         // 从文件夹加载
		virtual BOOL LoadFromFile(const CHAR *szFileName);                                         // 从文件加载
		virtual BOOL LoadFromZip(const CHAR *szZipName);                                           // 从文件压缩包加载

		virtual const CResourcePtr& CopyFrom(const CHAR *szName, const CResource *pCopyFrom);      // 复制资源

	protected:
		virtual BOOL PreLoad(CResourceHandle *pResource);                                          // 预加载资源

	public:
		virtual VOID GarbageCollection(VOID);                                                      // 垃圾回收
	};

}

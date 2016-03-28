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

	class _CrossExport CResource
	{
		friend class CResourcePtr;
		friend class CResourceManager;
		friend class CSoundManager;
		friend class CShaderManager;
		friend class CTextureManager;
		friend class CMaterialManager;
		friend class CSkeletonManager;
		friend class CMeshManager;
		friend class CEffectManager;


		// 数据结构
	public:
		typedef enum { SOUND, SHADER, TEXTURE, MATERIAL, SKELETON, MESH, EFFECT, HALO } TYPE;      // 资源类型


		// 构造/析构函数
	protected:
		CResource(CResourceManager *pResourceManager);
		virtual ~CResource(VOID);


		// 方法
	public:
		virtual BOOL IsValid(VOID) const = 0;                                                      // 有效性检查

		virtual VOID Init(VOID);                                                                   // 初始化资源
		virtual VOID Free(VOID);                                                                   // 释放资源

	public:
		virtual TYPE GetType(VOID) const = 0;                                                      // 获得类型
		virtual CResourceManager* GetResourceManager(VOID) const;                                  // 获得资源管理器

	public:
		virtual const CStream* GetStream(VOID) const;                                              // 获得数据流

	public:
		virtual BOOL SetName(const CHAR *szName);                                                  // 设置资源名
		virtual const CHAR* GetName(VOID) const;                                                   // 获得资源名
		virtual DWORD GetHashName(VOID) const;                                                     // 获得资源名

		virtual BOOL SetFileName(const CHAR *szFileName);                                          // 设置文件名
		virtual const CHAR* GetFileName(VOID) const;                                               // 获得文件名

	public:
		virtual BOOL CopyFrom(const CResource *pCopyFrom);                                         // 复制资源

		virtual BOOL LoadFromFile(const CHAR *szFileName);                                         // 从文件加载资源
		virtual BOOL LoadFromZip(const CHAR *szZipName, const CHAR *szFileName);                   // 从压缩包加载资源
		virtual BOOL LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName);                      // 从压缩包加载资源
		virtual BOOL LoadFromStream(CStream *pStream) = 0;                                         // 从数据流加载资源

		virtual BOOL SaveToFile(const CHAR *szFileName);                                           // 保存到文件
		virtual BOOL SaveToFileStream(FILE *pFile);                                                // 保存到文件数据流


		// 属性
	protected:
		CStream m_stream;                                                                          // 资源数据流
		                                                                                           // 注意: 并不是所有类型资源都使用资源数据流

	protected:
		CResourceManager *m_pResourceManager;                                                      // 资源管理器
	};

	class _CrossExport CResourcePtr : public CSharedPtr<CResource>
	{
		// 构造/析构函数
	public:
		CResourcePtr(VOID) : CSharedPtr<CResource>() {}
		CResourcePtr(const CResource *pResource) : CSharedPtr<CResource>(pResource) {}
		CResourcePtr(const CResourcePtr &ptrResource) : CSharedPtr<CResource>(ptrResource) {}
		virtual ~CResourcePtr(VOID) {}


		// 方法
	protected:
		virtual VOID FreePointer(VOID)
		{
			if (m_pPointer) {
				SAFE_DELETE(m_pPointer);
			}
		}
	};

}

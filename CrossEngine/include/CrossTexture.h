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

	class _CrossExport CTexture : public CResource
	{
		friend class CTexturePtr;
		friend class CTextureManager;


		// 数据结构
	public:
		typedef enum { UNKNOW = 0, BITMAP, DDS, PVR, ETC } TEXTURE_TYPE;                           // 纹理类型


		// 构造/析构函数
	protected:
		CTexture(CResourceManager *pResourceManager);
		virtual ~CTexture(VOID);


		// 方法
	public:
		virtual VOID Init(VOID);                                                                   // 初始化资源
		virtual VOID Free(VOID);                                                                   // 释放资源

	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型
		virtual BOOL IsValid(VOID) const;                                                          // 有效性检查

	public:
		virtual BOOL CopyFrom(const CResource *pCopyFrom);                                         // 复制资源

		virtual BOOL LoadFromFile(const CHAR *szFileName);                                         // 从文件加载资源
		virtual BOOL LoadFromZip(const CHAR *szZipName, const CHAR *szFileName);                   // 从压缩包加载资源
		virtual BOOL LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName);                      // 从压缩包加载资源
		virtual BOOL LoadFromStream(CStream *pStream);                                             // 从数据流加载资源

	protected:
		VOID LoadBMP(CStream *pStream);                                                            // 加载BMP纹理
		VOID LoadDDS(CStream *pStream);                                                            // 加载DDS压缩纹理
		VOID LoadPVR(CStream *pStream);                                                            // 加载PVR压缩纹理
		VOID LoadTexture(CStream *pStream);                                                        // 加载纹理
		VOID CreateTexture(VOID);                                                                  // 创建纹理

	public:
		UINT GetTargetGL(VOID) const;                                                              // 获得目标
		UINT GetTextureGL(VOID) const;                                                             // 获得纹理

		INT GetInternalFormatGL(VOID) const;                                                       // 获得纹理内部格式
		INT GetFormatGL(VOID) const;                                                               // 获得纹理格式
		INT GetTypeGL(VOID) const;                                                                 // 获得纹理像素数据类型


		// 属性
	protected:
		TEXTURE_TYPE m_type;                                                                       // 纹理类型

		INT m_width;                                                                               // 纹理宽
		INT m_height;                                                                              // 纹理高
		INT m_bitcount;                                                                            // 纹理色深
		INT m_mipmaps;                                                                             // 纹理等级
		INT m_ddsFormat;                                                                           // DDS格式
		INT m_pvrFormat;                                                                           // PVR格式
		BYTE *m_pData;                                                                             // 纹理数据缓冲

	protected:
		UINT m_target;                                                                             // 目标
		UINT m_texture;                                                                            // 纹理
	};

	class _CrossExport CTexturePtr : public CSharedPtr<CTexture>
	{
		// 构造/析构函数
	public:
		CTexturePtr(VOID) : CSharedPtr<CTexture>() {}
		CTexturePtr(const CTexture *pTexture) : CSharedPtr<CTexture>(pTexture) {}
		CTexturePtr(const CTexturePtr &ptrTexture) : CSharedPtr<CTexture>(ptrTexture) {}
		CTexturePtr(const CResourcePtr &ptrResource) { Set(static_cast<CTexture*>(ptrResource.GetPointer()), ptrResource.GetRefCount()); }
		virtual ~CTexturePtr(VOID) {}


		// 方法
	protected:
		virtual VOID FreePointer(VOID)
		{
			if (m_pPointer) {
				m_pPointer->GetResourceManager()->DestroyResource(m_pPointer);
			}
		}

	public:
		CTexturePtr& operator = (const CResourcePtr &ptrResource)
		{
			Set(static_cast<CTexture*>(ptrResource.GetPointer()), ptrResource.GetRefCount());
			return *this;
		}

		operator CResource* ()
		{
			return (CResource *)m_pPointer;
		}

		operator const CResource* () const
		{
			return (const CResource *)m_pPointer;
		}
	};

}

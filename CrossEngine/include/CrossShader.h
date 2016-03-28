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

	class _CrossExport CShader : public CResource
	{
		friend class CShaderPtr;
		friend class CShaderManager;


		// 构造/析构函数
	protected:
		CShader(CResourceManager *pResourceManager);
		virtual ~CShader(VOID);


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
		virtual BOOL LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName);                      // 从压缩包文件加载数据流
		virtual BOOL LoadFromStream(CStream *pStream);                                             // 从数据流加载资源

	public:
		UINT GetShaderGL(VOID) const;                                                              // 获得着色器
		const CRenderer::UniformLocationMap& GetParams(VOID) const;                                // 获得参数集合


		// 属性
	protected:
		UINT m_shader;                                                                             // 着色器
		CRenderer::UniformLocationMap m_params;                                                    // 着色器参数集合
	};

	class _CrossExport CShaderPtr : public CSharedPtr<CShader>
	{
		// 构造/析构函数
	public:
		CShaderPtr(VOID) : CSharedPtr<CShader>() {}
		CShaderPtr(const CShader *pShader) : CSharedPtr<CShader>(pShader) {}
		CShaderPtr(const CShaderPtr &ptrShader) : CSharedPtr<CShader>(ptrShader) {}
		CShaderPtr(const CResourcePtr &ptrResource) { Set(static_cast<CShader*>(ptrResource.GetPointer()), ptrResource.GetRefCount()); }
		virtual ~CShaderPtr(VOID) {}


		// 方法
	protected:
		virtual VOID FreePointer(VOID)
		{
			if (m_pPointer) {
				m_pPointer->GetResourceManager()->DestroyResource(m_pPointer);
			}
		}

	public:
		CShaderPtr& operator = (const CResourcePtr &ptrResource)
		{
			Set(static_cast<CShader*>(ptrResource.GetPointer()), ptrResource.GetRefCount());
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

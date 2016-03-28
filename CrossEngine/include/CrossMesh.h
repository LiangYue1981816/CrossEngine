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

	class _CrossExport CSubMesh
	{
		friend class CMesh;


		// 构造/析构函数
	protected:
		CSubMesh(DWORD dwName);
		virtual ~CSubMesh(VOID);


		// 方法
	public:
		const CHAR* GetName(VOID) const;                                                           // 获得名称
		DWORD GetHashName(VOID) const;                                                             // 获得名称

		BOOL IsValid(VOID) const;                                                                  // 有效性检查
		BOOL IsSkinMesh(VOID) const;                                                               // 蒙皮网格

	public:
		INT GetFaceCount(VOID) const;                                                              // 获得面数
		INT GetVertexCount(VOID) const;                                                            // 获得顶点数

		const FACE* GetFace(INT index) const;                                                      // 获得面数据
		const VERTEX* GetVertex(INT index) const;                                                  // 获得顶点数据

		const FACE* GetFaceBuffer(VOID) const;                                                     // 获得面数据缓冲
		const VERTEX* GetVertexBuffer(VOID) const;                                                 // 获得顶点数据缓冲

	public:
		const CMaterialPtr& GetMaterial(VOID) const;                                               // 获得材质
		const CTexturePtr& GetLightMapTexture(VOID) const;                                         // 获得光照贴图纹理

	public:
		UINT GetVAO(VOID) const;                                                                   // 获得VAO
		UINT GetVBO(VOID) const;                                                                   // 获得VBO
		UINT GetIBO(VOID) const;                                                                   // 获得IBO


		// 属性
	protected:
		CHAR m_szName[_MAX_STRING];                                                                // 名称
		DWORD m_dwName;                                                                            // 名称

		BOOL m_bSkinMesh;                                                                          // 蒙皮网格

	protected:
		INT m_numFaces;                                                                            // 面数
		INT m_numVertices;                                                                         // 顶点数

		FACE *m_pFaces;                                                                            // 面集合
		VERTEX *m_pVertices;                                                                       // 顶点集合

	protected:
		CMaterialPtr m_ptrMaterial;                                                                // 材质
		CTexturePtr m_ptrLightMapTexture;                                                          // 光照贴图纹理

	protected:
		UINT m_vao;                                                                                // VAO
		UINT m_vbo;                                                                                // VBO
		UINT m_ibo;                                                                                // IBO
	};

	class _CrossExport CMesh : public CResource
	{
		friend class CMeshPtr;
		friend class CMeshManager;


		// 数据结构
	public:
		typedef std::map<DWORD, CSubMesh*> SubMeshMap;                                             // 子网格集合


		// 构造/析构函数
	protected:
		CMesh(CResourceManager *pResourceManager);
		virtual ~CMesh(VOID);


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
		VOID LoadAABB(CStream *pStream);                                                           // 加载包围盒
		VOID LoadSubMesh(CStream *pStream);                                                        // 加载子网格
		VOID LoadSkeleton(CStream *pStream);                                                       // 加载骨骼
		VOID CreateSubMesh(VOID);                                                                  // 创建子网格

	public:
		const AABB* GetAABB(VOID) const;                                                           // 获得包围盒

		const CSubMesh* GetSubMesh(const CHAR *szName) const;                                      // 获得子网格
		const CSubMesh* GetSubMesh(DWORD dwName) const;                                            // 获得子网格
		const SubMeshMap& GetSubMeshs(VOID) const;                                                 // 获得子网格集合

		const CSkeletonPtr& GetSkeleton(VOID) const;                                               // 获得骨骼


		// 属性
	protected:
		AABB m_aabb;                                                                               // 包围盒

		SubMeshMap m_subMeshs;                                                                     // 子网格
		CSkeletonPtr m_ptrSkeleton;                                                                // 骨骼
	};

	class _CrossExport CMeshPtr : public CSharedPtr<CMesh>
	{
		// 构造/析构函数
	public:
		CMeshPtr(VOID) : CSharedPtr<CMesh>() {}
		CMeshPtr(const CMesh *pMesh) : CSharedPtr<CMesh>(pMesh) {}
		CMeshPtr(const CMeshPtr &ptrMesh) : CSharedPtr<CMesh>(ptrMesh) {}
		CMeshPtr(const CResourcePtr &ptrResource) { Set(static_cast<CMesh*>(ptrResource.GetPointer()), ptrResource.GetRefCount()); }
		virtual ~CMeshPtr(VOID) {}


		// 方法
	protected:
		virtual VOID FreePointer(VOID)
		{
			if (m_pPointer) {
				m_pPointer->GetResourceManager()->DestroyResource(m_pPointer);
			}
		}

	public:
		CMeshPtr& operator = (const CResourcePtr &ptrResource)
		{
			Set(static_cast<CMesh*>(ptrResource.GetPointer()), ptrResource.GetRefCount());
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

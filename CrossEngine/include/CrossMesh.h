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


		// ����/��������
	protected:
		CSubMesh(DWORD dwName);
		virtual ~CSubMesh(VOID);


		// ����
	public:
		const CHAR* GetName(VOID) const;                                                           // �������
		DWORD GetHashName(VOID) const;                                                             // �������

		BOOL IsValid(VOID) const;                                                                  // ��Ч�Լ��
		BOOL IsSkinMesh(VOID) const;                                                               // ��Ƥ����

	public:
		INT GetFaceCount(VOID) const;                                                              // �������
		INT GetVertexCount(VOID) const;                                                            // ��ö�����

		const FACE* GetFace(INT index) const;                                                      // ���������
		const VERTEX* GetVertex(INT index) const;                                                  // ��ö�������

		const FACE* GetFaceBuffer(VOID) const;                                                     // ��������ݻ���
		const VERTEX* GetVertexBuffer(VOID) const;                                                 // ��ö������ݻ���

	public:
		const CMaterialPtr& GetMaterial(VOID) const;                                               // ��ò���
		const CTexturePtr& GetLightMapTexture(VOID) const;                                         // ��ù�����ͼ����

	public:
		UINT GetVAO(VOID) const;                                                                   // ���VAO
		UINT GetVBO(VOID) const;                                                                   // ���VBO
		UINT GetIBO(VOID) const;                                                                   // ���IBO


		// ����
	protected:
		CHAR m_szName[_MAX_STRING];                                                                // ����
		DWORD m_dwName;                                                                            // ����

		BOOL m_bSkinMesh;                                                                          // ��Ƥ����

	protected:
		INT m_numFaces;                                                                            // ����
		INT m_numVertices;                                                                         // ������

		FACE *m_pFaces;                                                                            // �漯��
		VERTEX *m_pVertices;                                                                       // ���㼯��

	protected:
		CMaterialPtr m_ptrMaterial;                                                                // ����
		CTexturePtr m_ptrLightMapTexture;                                                          // ������ͼ����

	protected:
		UINT m_vao;                                                                                // VAO
		UINT m_vbo;                                                                                // VBO
		UINT m_ibo;                                                                                // IBO
	};

	class _CrossExport CMesh : public CResource
	{
		friend class CMeshPtr;
		friend class CMeshManager;


		// ���ݽṹ
	public:
		typedef std::map<DWORD, CSubMesh*> SubMeshMap;                                             // �����񼯺�


		// ����/��������
	protected:
		CMesh(CResourceManager *pResourceManager);
		virtual ~CMesh(VOID);


		// ����
	public:
		virtual VOID Init(VOID);                                                                   // ��ʼ����Դ
		virtual VOID Free(VOID);                                                                   // �ͷ���Դ

	public:
		virtual TYPE GetType(VOID) const;                                                          // �������
		virtual BOOL IsValid(VOID) const;                                                          // ��Ч�Լ��

	public:
		virtual BOOL CopyFrom(const CResource *pCopyFrom);                                         // ������Դ

		virtual BOOL LoadFromFile(const CHAR *szFileName);                                         // ���ļ�������Դ
		virtual BOOL LoadFromZip(const CHAR *szZipName, const CHAR *szFileName);                   // ��ѹ����������Դ
		virtual BOOL LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName);                      // ��ѹ����������Դ
		virtual BOOL LoadFromStream(CStream *pStream);                                             // ��������������Դ

	protected:
		VOID LoadAABB(CStream *pStream);                                                           // ���ذ�Χ��
		VOID LoadSubMesh(CStream *pStream);                                                        // ����������
		VOID LoadSkeleton(CStream *pStream);                                                       // ���ع���
		VOID CreateSubMesh(VOID);                                                                  // ����������

	public:
		const AABB* GetAABB(VOID) const;                                                           // ��ð�Χ��

		const CSubMesh* GetSubMesh(const CHAR *szName) const;                                      // ���������
		const CSubMesh* GetSubMesh(DWORD dwName) const;                                            // ���������
		const SubMeshMap& GetSubMeshs(VOID) const;                                                 // ��������񼯺�

		const CSkeletonPtr& GetSkeleton(VOID) const;                                               // ��ù���


		// ����
	protected:
		AABB m_aabb;                                                                               // ��Χ��

		SubMeshMap m_subMeshs;                                                                     // ������
		CSkeletonPtr m_ptrSkeleton;                                                                // ����
	};

	class _CrossExport CMeshPtr : public CSharedPtr<CMesh>
	{
		// ����/��������
	public:
		CMeshPtr(VOID) : CSharedPtr<CMesh>() {}
		CMeshPtr(const CMesh *pMesh) : CSharedPtr<CMesh>(pMesh) {}
		CMeshPtr(const CMeshPtr &ptrMesh) : CSharedPtr<CMesh>(ptrMesh) {}
		CMeshPtr(const CResourcePtr &ptrResource) { Set(static_cast<CMesh*>(ptrResource.GetPointer()), ptrResource.GetRefCount()); }
		virtual ~CMeshPtr(VOID) {}


		// ����
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

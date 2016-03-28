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

#include "_CrossEngine.h"


namespace CrossEngine {

	CSubMesh::CSubMesh(DWORD dwName)
		: m_dwName(dwName)

		, m_bSkinMesh(FALSE)

		, m_numFaces(0)
		, m_numVertices(0)
		, m_pFaces(NULL)
		, m_pVertices(NULL)

		, m_vao(0)
		, m_vbo(0)
		, m_ibo(0)
	{
		memset(m_szName, 0, sizeof(m_szName));
	}

	CSubMesh::~CSubMesh(VOID)
	{
		m_ptrMaterial.SetNull();
		m_ptrLightMapTexture.SetNull();

		Renderer()->DestroyVAO(&m_vao);
		Renderer()->DestroyVBO(&m_vbo);
		Renderer()->DestroyVBO(&m_ibo);
	}

	//
	// �������
	//
	const CHAR* CSubMesh::GetName(VOID) const
	{
		return m_szName;
	}

	//
	// �������
	//
	DWORD CSubMesh::GetHashName(VOID) const
	{
		return m_dwName;
	}

	//
	// ��Ч�Լ��
	//
	BOOL CSubMesh::IsValid(VOID) const
	{
		return m_numFaces > 0 && m_numVertices > 0 ? TRUE : FALSE;
	}

	//
	// ��Ƥ����
	//
	BOOL CSubMesh::IsSkinMesh(VOID) const
	{
		return m_bSkinMesh;
	}

	//
	// �������
	//
	INT CSubMesh::GetFaceCount(VOID) const
	{
		return m_numFaces;
	}

	//
	// ��ö�����
	//
	INT CSubMesh::GetVertexCount(VOID) const
	{
		return m_numVertices;
	}

	//
	// ���������
	//
	const FACE* CSubMesh::GetFace(INT index) const
	{
		ASSERT(index >= 0 && index < m_numFaces);
		return &m_pFaces[index];
	}

	//
	// ��ö�������
	//
	const VERTEX* CSubMesh::GetVertex(INT index) const
	{
		ASSERT(index >= 0 && index < m_numVertices);
		return &m_pVertices[index];
	}

	//
	// ��������ݻ���
	//
	const FACE* CSubMesh::GetFaceBuffer(VOID) const
	{
		return m_pFaces;
	}

	//
	// ��ö������ݻ���
	//
	const VERTEX* CSubMesh::GetVertexBuffer(VOID) const
	{
		return m_pVertices;
	}

	//
	// ��ò���
	//
	const CMaterialPtr& CSubMesh::GetMaterial(VOID) const
	{
		return m_ptrMaterial;
	}

	//
	// ��ù�����ͼ����
	//
	const CTexturePtr& CSubMesh::GetLightMapTexture(VOID) const
	{
		return m_ptrLightMapTexture;
	}

	//
	// ���VAO
	//
	UINT CSubMesh::GetVAO(VOID) const
	{
		return m_vao;
	}

	//
	// ���VBO
	//
	UINT CSubMesh::GetVBO(VOID) const
	{
		return m_vbo;
	}

	//
	// ���IBO
	//
	UINT CSubMesh::GetIBO(VOID) const
	{
		return m_ibo;
	}

	CMesh::CMesh(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{
		Init();
	}

	CMesh::~CMesh(VOID)
	{
		Free();
	}

	//
	// ��ʼ����Դ
	//
	VOID CMesh::Init(VOID)
	{
		AABBZero(&m_aabb);
		m_subMeshs.clear();

		CResource::Init();
	}

	//
	// �ͷ���Դ
	//
	VOID CMesh::Free(VOID)
	{
		//
		// 1. �ͷŹ���
		//
		m_ptrSkeleton.SetNull();

		//
		// 2. �ͷ�������
		//
		for (SubMeshMap::const_iterator itSubMesh = m_subMeshs.begin(); itSubMesh != m_subMeshs.end(); ++itSubMesh) {
			const CSubMesh *pSubMesh = itSubMesh->second;
			ASSERT(pSubMesh);

			SAFE_DELETE(pSubMesh);
		}

		CResource::Free();
	}

	//
	// �������
	//
	CResource::TYPE CMesh::GetType(VOID) const
	{
		return CResource::MESH;
	}

	//
	// ��Ч�Լ��
	//
	BOOL CMesh::IsValid(VOID) const
	{
		return m_subMeshs.empty() ? FALSE : TRUE;
	}

	//
	// ������Դ
	// ע��: ��ֹ������Դ!!!
	//
	BOOL CMesh::CopyFrom(const CResource *pCopyFrom)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//
	// ���ļ�������Դ
	//
	BOOL CMesh::LoadFromFile(const CHAR *szFileName)
	{
		//
		// 1. ��������
		//
		BOOL rcode = CResource::LoadFromFile(szFileName);

		//
		// 2. �ͷ�������
		//
		m_stream.FreeData();

		return rcode;
	}

	//
	// ��ѹ����������Դ
	//
	BOOL CMesh::LoadFromZip(const CHAR *szZipName, const CHAR *szFileName)
	{
		//
		// 1. ��������
		//
		BOOL rcode = CResource::LoadFromZip(szZipName, szFileName);

		//
		// 2. �ͷ�������
		//
		m_stream.FreeData();

		return rcode;
	}

	//
	// ��ѹ����������Դ
	//
	BOOL CMesh::LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName)
	{
		//
		// 1. ��������
		//
		BOOL rcode = CResource::LoadFromZip(pZipPack, szFileName);

		//
		// 2. �ͷ�������
		//
		m_stream.FreeData();

		return rcode;
	}

	//
	// ��������������Դ
	//
	BOOL CMesh::LoadFromStream(CStream *pStream)
	{
		//
		// 1. ��������Ч�Լ��
		//
		if (pStream == NULL) {
			return FALSE;
		}

		if (pStream->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. �ͷ�����
		//
		Free();

		//
		// 3. ��������
		//
		try {
			//
			// 3.1. �����Χ��
			//
			LoadAABB(pStream);

			//
			// 3.2. ����������
			//
			LoadSubMesh(pStream);

			//
			// 3.3. �������
			//
			LoadSkeleton(pStream);

			//
			// 3.4. ����������
			//
			CreateSubMesh();

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CMesh::LoadFromStream: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ���ذ�Χ��
	//
	VOID CMesh::LoadAABB(CStream *pStream)
	{
		ASSERT(pStream);
		ASSERT(pStream->IsValid());

		m_aabb.center[0] = pStream->ReadFLOAT();
		m_aabb.center[1] = pStream->ReadFLOAT();
		m_aabb.center[2] = pStream->ReadFLOAT();
		m_aabb.minVertex[0] = pStream->ReadFLOAT();
		m_aabb.minVertex[1] = pStream->ReadFLOAT();
		m_aabb.minVertex[2] = pStream->ReadFLOAT();
		m_aabb.maxVertex[0] = pStream->ReadFLOAT();
		m_aabb.maxVertex[1] = pStream->ReadFLOAT();
		m_aabb.maxVertex[2] = pStream->ReadFLOAT();
	}

	//
	// ����������
	//
	VOID CMesh::LoadSubMesh(CStream *pStream)
	{
		ASSERT(pStream);
		ASSERT(pStream->IsValid());

		//
		// 1. ������������
		//
		INT numSubMeshs = (INT)pStream->ReadDWORD();
		if (numSubMeshs == 0) return;

		//
		// 2. ����������
		//
		for (INT indexSubMesh = 0; indexSubMesh < numSubMeshs; indexSubMesh++) {
			CSubMesh *pSubMesh = SAFE_NEW(MEMTYPE_HEAP) CSubMesh(pStream->ReadDWORD());
			ASSERT(pSubMesh);

			//
			// 2.1. ��ӵ������񼯺�
			//      ע��: ����ӵ������񼯺ϱ����ڴ�й©!!!
			//
			m_subMeshs[pSubMesh->GetHashName()] = pSubMesh;

			//
			// 2.2. ������������
			//
			DWORD dwNameLen = pStream->ReadDWORD();
			strcpy(pSubMesh->m_szName, (CHAR *)pStream->GetCurrentAddress());
			pStream->Seek(dwNameLen, CStream::SEEK_MODE_CUR);

			//
			// 2.3. ������ʼ�����
			//
			DWORD dwMaterialName = pStream->ReadDWORD();
			DWORD dwLightMapName = pStream->ReadDWORD();

			if (dwMaterialName != INVALID_HASHNAME) {
				pSubMesh->m_ptrMaterial = MaterialManager()->QueryResource(dwMaterialName);

				if (pSubMesh->m_ptrMaterial.IsNull() || pSubMesh->m_ptrMaterial->IsValid() == FALSE) {
					throw "Invalid material.";
				}
			}

			if (dwLightMapName != INVALID_HASHNAME) {
				pSubMesh->m_ptrLightMapTexture = TextureManager()->QueryResource(dwLightMapName);

				if (pSubMesh->m_ptrLightMapTexture.IsNull() || pSubMesh->m_ptrLightMapTexture->IsValid() == FALSE) {
					throw "Invalid lightmap.";
				}
			}

			//
			// 2.4. ������Ƥ����״̬
			//
			pSubMesh->m_bSkinMesh = (BOOL)pStream->ReadDWORD();

			//
			// 2.5. ����ͼ������
			//
			pSubMesh->m_numVertices = (INT)pStream->ReadDWORD();
			pSubMesh->m_pVertices = (VERTEX *)pStream->GetCurrentAddress();
			pStream->Seek(sizeof(*pSubMesh->m_pVertices) * pSubMesh->m_numVertices, CStream::SEEK_MODE_CUR);

			pSubMesh->m_numFaces = (INT)pStream->ReadDWORD();
			pSubMesh->m_pFaces = (FACE *)pStream->GetCurrentAddress();
			pStream->Seek(sizeof(*pSubMesh->m_pFaces) * pSubMesh->m_numFaces, CStream::SEEK_MODE_CUR);
		}
	}

	//
	// ���ع���
	//
	VOID CMesh::LoadSkeleton(CStream *pStream)
	{
		ASSERT(pStream);
		ASSERT(pStream->IsValid());

		DWORD dwSkeletonName = pStream->ReadDWORD();

		if (dwSkeletonName != INVALID_HASHNAME) {
			m_ptrSkeleton = SkeletonManager()->QueryResource(dwSkeletonName);

			if (m_ptrSkeleton.IsNull() || m_ptrSkeleton->IsValid() == FALSE) {
				throw "Invalid skeleton.";
			}
		}
	}

	//
	// ����������
	//
	VOID CMesh::CreateSubMesh(VOID)
	{
		for (SubMeshMap::const_iterator itSubMesh = m_subMeshs.begin(); itSubMesh != m_subMeshs.end(); ++itSubMesh) {
			CSubMesh *pSubMesh = itSubMesh->second;
			ASSERT(pSubMesh);

			pSubMesh->m_vbo = Renderer()->CreateVBO(GL_ARRAY_BUFFER, sizeof(*pSubMesh->m_pVertices) * pSubMesh->m_numVertices, pSubMesh->m_pVertices, GL_STATIC_DRAW);
			pSubMesh->m_ibo = Renderer()->CreateVBO(GL_ELEMENT_ARRAY_BUFFER, sizeof(*pSubMesh->m_pFaces) * pSubMesh->m_numFaces, pSubMesh->m_pFaces, GL_STATIC_DRAW);

			if (pSubMesh->m_vbo == 0) throw "CreateVBO failed.";
			if (pSubMesh->m_ibo == 0) throw "CreateIBO failed.";
		}
	}

	//
	// ��ð�Χ��
	//
	const AABB* CMesh::GetAABB(VOID) const
	{
		return &m_aabb;
	}

	//
	// ���������
	//
	const CSubMesh* CMesh::GetSubMesh(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetSubMesh(HashValue(szName));
	}

	//
	// ���������
	//
	const CSubMesh* CMesh::GetSubMesh(DWORD dwName) const
	{
		SubMeshMap::const_iterator itSubMesh = m_subMeshs.find(dwName);
		return itSubMesh != m_subMeshs.end() ? itSubMesh->second : NULL;
	}

	//
	// ��������񼯺�
	//
	const CMesh::SubMeshMap& CMesh::GetSubMeshs(VOID) const
	{
		return m_subMeshs;
	}

	//
	// ��ù���
	//
	const CSkeletonPtr& CMesh::GetSkeleton(VOID) const
	{
		return m_ptrSkeleton;
	}

}

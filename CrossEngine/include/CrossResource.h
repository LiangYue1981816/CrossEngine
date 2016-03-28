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


		// ���ݽṹ
	public:
		typedef enum { SOUND, SHADER, TEXTURE, MATERIAL, SKELETON, MESH, EFFECT, HALO } TYPE;      // ��Դ����


		// ����/��������
	protected:
		CResource(CResourceManager *pResourceManager);
		virtual ~CResource(VOID);


		// ����
	public:
		virtual BOOL IsValid(VOID) const = 0;                                                      // ��Ч�Լ��

		virtual VOID Init(VOID);                                                                   // ��ʼ����Դ
		virtual VOID Free(VOID);                                                                   // �ͷ���Դ

	public:
		virtual TYPE GetType(VOID) const = 0;                                                      // �������
		virtual CResourceManager* GetResourceManager(VOID) const;                                  // �����Դ������

	public:
		virtual const CStream* GetStream(VOID) const;                                              // ���������

	public:
		virtual BOOL SetName(const CHAR *szName);                                                  // ������Դ��
		virtual const CHAR* GetName(VOID) const;                                                   // �����Դ��
		virtual DWORD GetHashName(VOID) const;                                                     // �����Դ��

		virtual BOOL SetFileName(const CHAR *szFileName);                                          // �����ļ���
		virtual const CHAR* GetFileName(VOID) const;                                               // ����ļ���

	public:
		virtual BOOL CopyFrom(const CResource *pCopyFrom);                                         // ������Դ

		virtual BOOL LoadFromFile(const CHAR *szFileName);                                         // ���ļ�������Դ
		virtual BOOL LoadFromZip(const CHAR *szZipName, const CHAR *szFileName);                   // ��ѹ����������Դ
		virtual BOOL LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName);                      // ��ѹ����������Դ
		virtual BOOL LoadFromStream(CStream *pStream) = 0;                                         // ��������������Դ

		virtual BOOL SaveToFile(const CHAR *szFileName);                                           // ���浽�ļ�
		virtual BOOL SaveToFileStream(FILE *pFile);                                                // ���浽�ļ�������


		// ����
	protected:
		CStream m_stream;                                                                          // ��Դ������
		                                                                                           // ע��: ����������������Դ��ʹ����Դ������

	protected:
		CResourceManager *m_pResourceManager;                                                      // ��Դ������
	};

	class _CrossExport CResourcePtr : public CSharedPtr<CResource>
	{
		// ����/��������
	public:
		CResourcePtr(VOID) : CSharedPtr<CResource>() {}
		CResourcePtr(const CResource *pResource) : CSharedPtr<CResource>(pResource) {}
		CResourcePtr(const CResourcePtr &ptrResource) : CSharedPtr<CResource>(ptrResource) {}
		virtual ~CResourcePtr(VOID) {}


		// ����
	protected:
		virtual VOID FreePointer(VOID)
		{
			if (m_pPointer) {
				SAFE_DELETE(m_pPointer);
			}
		}
	};

}

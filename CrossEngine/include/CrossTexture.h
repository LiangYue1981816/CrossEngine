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


		// ���ݽṹ
	public:
		typedef enum { UNKNOW = 0, BITMAP, DDS, PVR, ETC } TEXTURE_TYPE;                           // ��������


		// ����/��������
	protected:
		CTexture(CResourceManager *pResourceManager);
		virtual ~CTexture(VOID);


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
		VOID LoadBMP(CStream *pStream);                                                            // ����BMP����
		VOID LoadDDS(CStream *pStream);                                                            // ����DDSѹ������
		VOID LoadPVR(CStream *pStream);                                                            // ����PVRѹ������
		VOID LoadTexture(CStream *pStream);                                                        // ��������
		VOID CreateTexture(VOID);                                                                  // ��������

	public:
		UINT GetTargetGL(VOID) const;                                                              // ���Ŀ��
		UINT GetTextureGL(VOID) const;                                                             // �������

		INT GetInternalFormatGL(VOID) const;                                                       // ��������ڲ���ʽ
		INT GetFormatGL(VOID) const;                                                               // ��������ʽ
		INT GetTypeGL(VOID) const;                                                                 // �������������������


		// ����
	protected:
		TEXTURE_TYPE m_type;                                                                       // ��������

		INT m_width;                                                                               // �����
		INT m_height;                                                                              // �����
		INT m_bitcount;                                                                            // ����ɫ��
		INT m_mipmaps;                                                                             // ����ȼ�
		INT m_ddsFormat;                                                                           // DDS��ʽ
		INT m_pvrFormat;                                                                           // PVR��ʽ
		BYTE *m_pData;                                                                             // �������ݻ���

	protected:
		UINT m_target;                                                                             // Ŀ��
		UINT m_texture;                                                                            // ����
	};

	class _CrossExport CTexturePtr : public CSharedPtr<CTexture>
	{
		// ����/��������
	public:
		CTexturePtr(VOID) : CSharedPtr<CTexture>() {}
		CTexturePtr(const CTexture *pTexture) : CSharedPtr<CTexture>(pTexture) {}
		CTexturePtr(const CTexturePtr &ptrTexture) : CSharedPtr<CTexture>(ptrTexture) {}
		CTexturePtr(const CResourcePtr &ptrResource) { Set(static_cast<CTexture*>(ptrResource.GetPointer()), ptrResource.GetRefCount()); }
		virtual ~CTexturePtr(VOID) {}


		// ����
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

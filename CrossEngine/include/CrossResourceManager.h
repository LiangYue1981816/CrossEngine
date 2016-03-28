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


		// ����/��������
	protected:
		CResourceHandle(CResourceManager *pResourceManager, const CHAR *szName);
		CResourceHandle(CResourceManager *pResourceManager, const CHAR *szName, const CHAR *szFileName);
		CResourceHandle(CResourceManager *pResourceManager, const CHAR *szName, const CHAR *szFileName, ZZIP_DIR *pZipPack);
		virtual ~CResourceHandle(VOID);


		// ����
	public:
		BOOL IsWaste(VOID) const;                                                                  // ������Դ

		const CHAR* GetName(VOID) const;                                                           // �����Դ��
		const CHAR* GetFileName(VOID) const;                                                       // �����Դ�ļ���
		CResourcePtr& GetResourcePtr(VOID);                                                        // �����Դָ��

	public:
		BOOL LoadResource(BOOL bReload);                                                           // ������Դ
		VOID FreeResource(VOID);                                                                   // �ͷ���Դ
		VOID DeleteResource(VOID);                                                                 // ɾ����Դ


		// ����
	protected:
		BOOL m_bIsWaste;                                                                           // ������Դ

	protected:
		CHAR *m_szName;                                                                            // ��Դ��
		CHAR *m_szFileName;                                                                        // ��Դ�ļ���
		ZZIP_DIR *m_pZipPack;                                                                      // ��Դѹ����

	protected:
		CResourcePtr m_ptrResource;                                                                // ��Դָ��
		CResourceManager *m_pResourceManager;                                                      // ��Դ������
	};

	class _CrossExport CResourceManager
	{
		friend class CResourceSystem;


		// ���ݽṹ
	public:
		typedef std::map<DWORD, ZZIP_DIR*> PackMap;                                                // ������
		typedef std::map<DWORD, CResourceHandle*> ResourceMap;                                     // ��Դ����


		// ����/��������
	protected:
		CResourceManager(VOID);
		virtual ~CResourceManager(VOID);


		// ����
	protected:
		virtual VOID Init(VOID);                                                                   // ��ʼ����Դ������
		virtual VOID Free(VOID);                                                                   // �ͷ���Դ������

	public:
		virtual CResource::TYPE GetType(VOID) const = 0;                                           // �������

	public:
		virtual CResource* CreateResource(VOID) = 0;                                               // ������Դ
		virtual VOID DestroyResource(CResource *pResource);                                        // ������Դ

	public:
		const CResourcePtr& QueryResource(const CHAR *szName, BOOL bReload = FALSE);               // ������Դ
		const CResourcePtr& QueryResource(DWORD dwName, BOOL bReload = FALSE);                     // ������Դ

		BOOL FreeResource(const CHAR *szName);                                                     // �ͷ���Դ
		BOOL FreeResource(DWORD dwName);                                                           // �ͷ���Դ

		BOOL DeleteResource(const CHAR *szName);                                                   // ɾ����Դ
		BOOL DeleteResource(DWORD dwName);                                                         // ɾ����Դ

		const ResourceMap& GetResources(VOID) const;                                               // �����Դ����

	public:
		virtual const CResourcePtr& CopyFrom(const CHAR *szName, const CResource *pCopyFrom);      // ������Դ

		virtual BOOL LoadFromFile(const CHAR *szFileName) = 0;                                     // ���ļ�����
		virtual BOOL LoadFromPath(const CHAR *szPathName) = 0;                                     // ���ļ��м���
		virtual BOOL LoadFromZip(const CHAR *szZipName) = 0;                                       // ��ѹ��������

		virtual BOOL Reload(VOID);                                                                 // ���¼�����Դ

	protected:
		virtual BOOL LoadFromFile(const CHAR *szFileName, const CHAR *szExtName);                  // ���ļ�����
		virtual BOOL LoadFromPath(const CHAR *szPathName, const CHAR *szExtName);                  // ���ļ��м���
		virtual BOOL LoadFromZip(const CHAR *szZipName, const CHAR *szExtName);                    // ��ѹ��������

		virtual BOOL PreLoad(CResourceHandle *pResource);                                          // Ԥ������Դ

	public:
		virtual VOID GarbageCollection(VOID);                                                      // ��������


		// ����
	protected:
		PackMap m_packs;                                                                           // ������
		ResourceMap m_resources;                                                                   // ��Դ����

	protected:
		pthread_mutex_t m_mutex;                                                                   // ������
	};

}

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

	class _CrossExport CResourceSystem
	{
		friend class CEngine;


		// ���ݽṹ
	public:
		typedef enum {
			RESOURCE_MANAGER_SOUND = 0,                                                            // ������Դ������
			RESOURCE_MANAGER_SHADER,                                                               // ��ɫ����Դ������
			RESOURCE_MANAGER_TEXTURE,                                                              // ������Դ������
			RESOURCE_MANAGER_MATERIAL,                                                             // ������Դ������
			RESOURCE_MANAGER_SKELETON,                                                             // ������Դ������
			RESOURCE_MANAGER_MESH,                                                                 // ������Դ������
			RESOURCE_MANAGER_EFFECT,                                                               // ��Ч��Դ������
			RESOURCE_MANAGER_HALO,                                                                 // ������Դ������
			RESOURCE_MANAGER_COUNT                                                                 // ��Դ��������
		} RESOURCE_MANAGER;


		// ����/��������
	protected:
		CResourceSystem(VOID);
		virtual ~CResourceSystem(VOID);


		// ����
	public:
		CResourceManager* GetResourceManager(RESOURCE_MANAGER manager) const;                      // �����Դ������

	public:
		CResource* CreateResource(RESOURCE_MANAGER manager);                                       // ������Դ
		VOID DestroyResource(CResource *pResource);                                                // ������Դ

	public:
		BOOL LoadResourceZip(const CHAR *szZipName);                                               // ������Դ����Դ
		BOOL LoadResourcePath(const CHAR *szPathName);                                             // �����ļ�����Դ

		BOOL ReloadAll(VOID);                                                                      // ���¼���������Դ
		BOOL ReloadGfx(VOID);                                                                      // ���¼���ͼ����Դ

	public:
		VOID GarbageCollection(VOID);                                                              // ��������


		// ����
	protected:
		CResourceManager *m_pResourceManager[RESOURCE_MANAGER_COUNT];                              // ��Դ����������
	};

}

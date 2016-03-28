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

	CMaterialManager::CMaterialManager(VOID)
	{

	}

	CMaterialManager::~CMaterialManager(VOID)
	{

	}

	//
	// �������
	//
	CResource::TYPE CMaterialManager::GetType(VOID) const
	{
		return CResource::MATERIAL;
	}

	//
	// ������Դ
	//
	CResource* CMaterialManager::CreateResource(VOID)
	{
		return SAFE_NEW(MEMTYPE_HEAP) CMaterial(this);
	}

	//
	// ���ļ��м���
	//
	BOOL CMaterialManager::LoadFromPath(const CHAR *szPathName)
	{
		ASSERT(szPathName);
		return CResourceManager::LoadFromPath(szPathName, MATERIAL_EXT_NAME);
	}

	//
	// ���ļ�����
	//
	BOOL CMaterialManager::LoadFromFile(const CHAR *szFileName)
	{
		ASSERT(szFileName);
		return CResourceManager::LoadFromFile(szFileName, MATERIAL_EXT_NAME);
	}

	//
	// ���ļ�ѹ��������
	//
	BOOL CMaterialManager::LoadFromZip(const CHAR *szZipName)
	{
		ASSERT(szZipName);
		return CResourceManager::LoadFromZip(szZipName, MATERIAL_EXT_NAME);
	}

}

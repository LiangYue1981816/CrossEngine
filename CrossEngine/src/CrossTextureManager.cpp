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

	CTextureManager::CTextureManager(VOID)
	{

	}

	CTextureManager::~CTextureManager(VOID)
	{

	}

	//
	// 获得类型
	//
	CResource::TYPE CTextureManager::GetType(VOID) const
	{
		return CResource::TEXTURE;
	}

	//
	// 创建资源
	//
	CResource* CTextureManager::CreateResource(VOID)
	{
		return SAFE_NEW(MEMTYPE_HEAP) CTexture(this);
	}

	//
	// 从文件夹加载
	//
	BOOL CTextureManager::LoadFromPath(const CHAR *szPathName)
	{
		ASSERT(szPathName);

		if (CResourceManager::LoadFromPath(szPathName, TEXTURE_EXT_NAME0) == FALSE) return FALSE;
		if (CResourceManager::LoadFromPath(szPathName, TEXTURE_EXT_NAME1) == FALSE) return FALSE;
		if (CResourceManager::LoadFromPath(szPathName, TEXTURE_EXT_NAME2) == FALSE) return FALSE;

		return TRUE;
	}

	//
	// 从文件加载
	//
	BOOL CTextureManager::LoadFromFile(const CHAR *szFileName)
	{
		ASSERT(szFileName);

		if (CResourceManager::LoadFromFile(szFileName, TEXTURE_EXT_NAME0) == FALSE) return FALSE;
		if (CResourceManager::LoadFromFile(szFileName, TEXTURE_EXT_NAME1) == FALSE) return FALSE;
		if (CResourceManager::LoadFromFile(szFileName, TEXTURE_EXT_NAME2) == FALSE) return FALSE;

		return TRUE;
	}

	//
	// 从文件压缩包加载	
	//
	BOOL CTextureManager::LoadFromZip(const CHAR *szZipName)
	{
		ASSERT(szZipName);

		if (CResourceManager::LoadFromZip(szZipName, TEXTURE_EXT_NAME0) == FALSE) return FALSE;
		if (CResourceManager::LoadFromZip(szZipName, TEXTURE_EXT_NAME1) == FALSE) return FALSE;
		if (CResourceManager::LoadFromZip(szZipName, TEXTURE_EXT_NAME2) == FALSE) return FALSE;

		return TRUE;
	}

	//
	// 复制资源
	// 注意: 禁止复制资源!!!
	//
	const CResourcePtr& CTextureManager::CopyFrom(const CHAR *szName, const CResource *pCopyFrom)
	{
		ASSERT(FALSE);
		static const CResourcePtr ptrResourceNull;
		return ptrResourceNull;
	}

}

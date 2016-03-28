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

	static const DWORD dwKey = 0x19810816;
	static const DWORD dwKernel = 0x19810130;

	CStream::CStream(VOID)
	{
		Init();
	}

	CStream::~CStream(VOID)
	{
		Free();
	}

	//
	// 初始化
	//
	VOID CStream::Init(VOID)
	{
		m_dwName = INVALID_HASHNAME;
		memset(m_szName, 0, sizeof(m_szName));

		m_pZipPack = NULL;
		memset(m_szZipName, 0, sizeof(m_szZipName));
		memset(m_szFileName, 0, sizeof(m_szFileName));

		m_bAlloced = FALSE;
		m_pAddress = NULL;

		m_dwSize = 0;
		m_dwPosition = 0;
	}

	//
	// 释放数据流
	//
	VOID CStream::Free(VOID)
	{
		FreeData();
		Init();
	}

	//
	// 释放数据流存储
	//
	VOID CStream::FreeData(VOID)
	{
		if (m_bAlloced) {
			SAFE_FREE(m_pAddress);
		}

		m_bAlloced = FALSE;
		m_pAddress = NULL;

		m_dwSize = 0;
		m_dwPosition = 0;
	}

	//
	// 有效性检查
	//
	BOOL CStream::IsValid(VOID) const
	{
		return (m_pAddress && m_dwSize) ? TRUE : FALSE;
	}

	//
	// 分配数据流存储
	//
	BOOL CStream::Alloc(DWORD size)
	{
		//
		// 1. 有效性检查
		//
		if (size == 0) {
			return FALSE;
		}

		if (IsValid()) {
			return FALSE;
		}

		//
		// 2. 分配数据流存储
		//
		m_pAddress = (BYTE *)SAFE_MALLOC(size, MEMTYPE_HEAP);
		if (m_pAddress == NULL) return FALSE;

		m_dwSize = size;
		m_dwPosition = 0;

		m_bAlloced = TRUE;

		return TRUE;
	}

	//
	// 复制数据流
	//
	BOOL CStream::CopyFrom(const CStream *pStream)
	{
		//
		// 1. 数据流有效性检查
		//
		if (pStream == NULL) {
			return FALSE;
		}

		if (pStream->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 释放当前数据流
		//
		Free();

		//
		// 3. 分配数据流存储
		//
		if (Alloc(pStream->GetFullSize()) == FALSE) {
			return FALSE;
		}

		//
		// 4. 复制数据流
		//
		m_dwName = pStream->GetHashName();
		strcpy(m_szName, pStream->GetName());

		m_pZipPack = pStream->GetZipPack();
		strcpy(m_szZipName, pStream->GetZipName());
		strcpy(m_szFileName, pStream->GetFileName());

		memcpy(m_pAddress, pStream->GetBeginAddress(), pStream->GetFullSize());

		return TRUE;
	}

	//
	// 从文件加载数据流
	//
	BOOL CStream::LoadFromFile(const CHAR *szFileName)
	{
		//
		// 1. 参数安全检查
		//
		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. 释放当前数据流
		//
		Free();

		//
		// 3. 从文件加载数据流
		//
		FILE *pFile = NULL;

		try {
			//
			// 3.1. 打开文件
			//
			pFile = fopen(szFileName, "rb");
			if (pFile == NULL) throw "Open file failed.";

			//
			// 3.2. 分配数据流存储
			//
			DWORD dwSize = (DWORD)fsize(pFile);

			if (Alloc(dwSize) == FALSE) {
				throw "Alloc memory failed.";
			}

			//
			// 3.3. 读入文件数据
			//
			if (m_dwSize != fread(m_pAddress, sizeof(*m_pAddress), m_dwSize, pFile)) {
				throw "Read data failed.";
			}

			//
			// 3.4. 解密文件数据
			//
#ifndef _DEV_

			/*
			DWORD dwRandom = dwKey;
			for (DWORD index = 0; index < m_dwSize; index++) {
				dwRandom = PseudoRandom(dwRandom, dwKernel);
				m_pAddress[index] -= (BYTE)dwRandom;
			}
			*/

			DWORD dwRandom = (DWORD)((UINT64)(dwKey)*(UINT64)(dwKernel));
			for (DWORD index = 0; index < m_dwSize; index++) {
				m_pAddress[index] -= (BYTE)dwRandom;
			}

#endif

			//
			// 3.5. 设置文件名
			//
			SetFileName(szFileName);

			//
			// 3.6. 关闭文件
			//
			fclose(pFile);

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CStream::LoadFromFile: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			if (pFile) {
				fclose(pFile);
			}

			return FALSE;
		}
	}

	//
	// 从压缩包文件加载数据流
	//
	BOOL CStream::LoadFromZip(const CHAR *szZipName, const CHAR *szFileName)
	{
		//
		// 1. 参数安全检查
		//
		if (szZipName == NULL) {
			return FALSE;
		}

		if (szFileName == NULL) {
			return NULL;
		}

		//
		// 2. 释放当前数据流
		//
		Free();

		//
		// 3. 从压缩包文件加载数据流
		//
		ZZIP_DIR *pZipPack = NULL;

		try {
			//
			// 3.1. 打开压缩包文件
			//
			pZipPack = zzip_opendir(szZipName);
			if (pZipPack == NULL) throw "Open zip failed.";

			//
			// 3.2. 加载压缩包文件
			//
			if (LoadFromZip(pZipPack, szFileName) == FALSE) {
				throw "Load from zip failed.";
			}

			//
			// 3.3. 设置文件名
			//
			SetZipName(szZipName);
			SetFileName(szFileName);

			//
			// 3.4. 关闭压缩包文件
			//
			zzip_closedir(pZipPack);

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CStream::LoadFromZip: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			if (pZipPack) {
				zzip_closedir(pZipPack);
			}

			return FALSE;
		}
	}

	//
	// 从压缩包文件加载数据流
	//
	BOOL CStream::LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName)
	{
		//
		// 1. 参数安全检查
		//
		if (pZipPack == NULL) {
			return FALSE;
		}

		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. 释放当前数据流
		//
		Free();

		//
		// 3. 从压缩包文件加载数据流
		//
		ZZIP_STAT zstat;
		ZZIP_FILE *pZipFile = NULL;

		try {
			//
			// 3.1. 获得压缩文件信息并分配数据流存储
			//
			if (zzip_dir_stat(pZipPack, szFileName, &zstat, ZZIP_CASEINSENSITIVE) != ZZIP_NO_ERROR) throw "Can not find file.";
			if (Alloc(zstat.st_size) == FALSE) throw "Alloc memory failed.";

			//
			// 3.2. 打开压缩文件
			//
			pZipFile = zzip_file_open(pZipPack, szFileName, ZZIP_ONLYZIP | ZZIP_CASELESS);
			if (pZipFile == NULL) throw "Open file failed.";

			//
			// 3.3. 加载压缩文件数据
			//
			if (m_dwSize != zzip_file_read(pZipFile, m_pAddress, m_dwSize)) {
				throw "Read data failed.";
			}

			//
			// 3.4. 解密文件数据
			//
#ifndef _DEV_

			/*
			DWORD dwRandom = dwKey;
			for (DWORD index = 0; index < m_dwSize; index++) {
				dwRandom = PseudoRandom(dwRandom, dwKernel);
				m_pAddress[index] -= (BYTE)dwRandom;
			}
			*/

			DWORD dwRandom = (DWORD)((UINT64)(dwKey)*(UINT64)(dwKernel));
			for (DWORD index = 0; index < m_dwSize; index++) {
				m_pAddress[index] -= (BYTE)dwRandom;
			}

#endif

			//
			// 3.5. 设置文件名
			//
			SetZipPack(pZipPack);
			SetFileName(szFileName);

			//
			// 3.6. 关闭压缩文件
			//
			zzip_file_close(pZipFile);

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CStream::LoadFromZip: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			if (pZipFile) {
				zzip_file_close(pZipFile);
			}

			return FALSE;
		}
	}

	//
	// 重新加载
	//
	BOOL CStream::Reload(VOID)
	{
		ZZIP_DIR *pZipPack = m_pZipPack;

		CHAR szZipName[_MAX_STRING];
		CHAR szFileName[_MAX_STRING];

		strcpy(szZipName, m_szZipName);
		strcpy(szFileName, m_szFileName);

		if (strlen(szFileName)) {
			//
			// 1. 从压缩包中加载
			//
			if (pZipPack) {
				return LoadFromZip(pZipPack, szFileName);
			}

			//
			// 2. 从压缩包中加载
			//
			if (strlen(szZipName)) {
				return LoadFromZip(szZipName, szFileName);
			}

			//
			// 3. 从文件加载
			//
			return LoadFromFile(szFileName);
		}

		return FALSE;
	}

	//
	// 设置数据流名
	//
	BOOL CStream::SetName(const CHAR *szName)
	{
		//
		// 1. 参数安全检查
		//
		if (szName == NULL) {
			return FALSE;
		}

		//
		// 2. 设置名称
		//
		strcpy(m_szName, szName);
		m_dwName = HashValue(szName);

		return TRUE;
	}

	//
	// 获得数据流名
	//
	const CHAR* CStream::GetName(VOID) const
	{
		return m_szName;
	}

	//
	// 获得数据流名
	//
	DWORD CStream::GetHashName(VOID) const
	{
		return m_dwName;
	}

	//
	// 设置数据流文件名
	//
	BOOL CStream::SetFileName(const CHAR *szFileName)
	{
		//
		// 1. 参数安全检查
		//
		if (szFileName == NULL) {
			return FALSE;
		}

		//
		// 2. 设置数据流文件名
		//
		CHAR szName[_MAX_STRING];
		CHAR szFName[_MAX_STRING];
		CHAR szEName[_MAX_STRING];

		strcpy(m_szFileName, szFileName);
		splitfilename(m_szFileName, szFName, szEName);
		sprintf(szName, "%s%s", szFName, szEName);

		SetName(szName);

		return TRUE;
	}

	//
	// 获得数据流文件名
	//
	const CHAR* CStream::GetFileName(VOID) const
	{
		return m_szFileName;
	}

	//
	// 设置压缩包指针
	//
	BOOL CStream::SetZipPack(ZZIP_DIR *pZipPack)
	{
		//
		// 1. 参数安全检查
		//
		if (pZipPack == NULL) {
			return FALSE;
		}

		//
		// 2. 设置压缩包指针
		//
		m_pZipPack = pZipPack;

		return TRUE;
	}

	//
	// 获得压缩包指针
	//
	ZZIP_DIR* CStream::GetZipPack(VOID) const
	{
		return m_pZipPack;
	}

	//
	// 设置压缩包名
	//
	BOOL CStream::SetZipName(const CHAR *szZipName)
	{
		//
		// 1. 参数安全检查
		//
		if (szZipName == NULL) {
			return FALSE;
		}

		//
		// 2. 设置压缩包名
		//
		strcpy(m_szZipName, szZipName);

		return TRUE;
	}

	//
	// 获得压缩包名
	//
	const CHAR* CStream::GetZipName(VOID) const
	{
		return m_szZipName;
	}

	//
	// 获得数据流空间
	//
	DWORD CStream::GetFullSize(VOID) const
	{
		return m_dwSize;
	}

	//
	// 获得数据流剩余空间
	//
	DWORD CStream::GetFreeSize(VOID) const
	{
		return m_dwSize - m_dwPosition;
	}

	//
	// 获得数据流当前位置
	//
	DWORD CStream::GetCurrentPosition(VOID) const
	{
		return m_dwPosition;
	}

	//
	// 设置数据流数据地址
	//
	BOOL CStream::SetStreamAddress(BYTE *pAddress, DWORD dwSize)
	{
		//
		// 1. 参数安全检查
		//
		if (pAddress == NULL) {
			return FALSE;
		}

		if (dwSize == 0) {
			return FALSE;
		}

		//
		// 2. 设置数据流
		//
		Free();

		m_bAlloced = FALSE;
		m_pAddress = pAddress;

		m_dwSize = dwSize;
		m_dwPosition = 0;

		return TRUE;
	}

	//
	// 获得数据流起始地址
	//
	VOID* CStream::GetBeginAddress(VOID) const
	{
		return m_pAddress;
	}

	//
	// 获得数据流当前地址
	//
	VOID* CStream::GetCurrentAddress(VOID) const
	{
		return m_pAddress + m_dwPosition;
	}

	//
	// 加载数据
	//
	DWORD CStream::Read(VOID *pBuffer, DWORD size, DWORD count)
	{
		//
		// 1. 参数安全检查
		//
		if (pBuffer == NULL) {
			return 0;
		}

		if (size == 0 || count == 0) {
			return 0;
		}

		if (IsValid() == FALSE) {
			return 0;
		}

		//
		// 2. 加载流内容
		//
		DWORD dwReadSize = min(GetFreeSize(), size*count);

		memcpy(pBuffer, m_pAddress + m_dwPosition, dwReadSize);
		m_dwPosition += dwReadSize;

		return dwReadSize / size;
	}

	//
	// 读入字符串
	//
	DWORD CStream::ReadString(CHAR *szString, DWORD size)
	{
		//
		// 1. 参数安全检查
		//
		if (szString == NULL) {
			return 0;
		}

		if (size == 0) {
			return 0;
		}

		if (IsValid() == FALSE) {
			return 0;
		}

		//
		// 2. 读入字符串
		//
		DWORD dwReadSize = 0;
		DWORD len = ReadDWORD();

		dwReadSize = Read(szString, sizeof(*szString), min(len, size));
		szString[dwReadSize] = 0;

		return dwReadSize;
	}

	//
	// 加载BYTE
	//
	BYTE CStream::ReadBYTE(VOID)
	{
		//
		// 1. 检查数据流
		//
		if (IsValid() == FALSE) {
			return 0;
		}

		//
		// 2. 读入字节
		//
		BYTE data;

		if (1 != Read(&data, sizeof(data), 1)) {
			return 0;
		}

		return data;
	}

	//
	// 加载WORD
	//
	WORD CStream::ReadWORD(VOID)
	{
		//
		// 1. 检查数据流
		//
		if (IsValid() == FALSE) {
			return 0;
		}

		//
		// 2. 读入缓冲
		//
		BYTE buffer[2];

		if (1 != Read(buffer, sizeof(buffer), 1)) {
			return 0;
		}

		//
		// 3. 转换为单字
		//
		WORD data;

#ifdef _BIG_ENDIAN_

		((BYTE *)&data)[0] = buffer[1];
		((BYTE *)&data)[1] = buffer[0];

#else

		((BYTE *)&data)[0] = buffer[0];
		((BYTE *)&data)[1] = buffer[1];

#endif

		return data;
	}

	//
	// 加载DWORD
	//
	DWORD CStream::ReadDWORD(VOID)
	{
		//
		// 1. 检查数据流
		//
		if (IsValid() == FALSE) {
			return 0;
		}

		//
		// 2. 读入缓冲
		//
		BYTE buffer[4];

		if (1 != Read(buffer, sizeof(buffer), 1)) {
			return 0;
		}

		//
		// 3. 转换为双字
		//
		DWORD data;

#ifdef _BIG_ENDIAN_

		((BYTE *)&data)[0] = buffer[3];
		((BYTE *)&data)[1] = buffer[2];
		((BYTE *)&data)[2] = buffer[1];
		((BYTE *)&data)[3] = buffer[0];	

#else

		((BYTE *)&data)[0] = buffer[0];
		((BYTE *)&data)[1] = buffer[1];
		((BYTE *)&data)[2] = buffer[2];
		((BYTE *)&data)[3] = buffer[3];

#endif

		return data;
	}

	//
	// 读入FLOAT
	//
	FLOAT CStream::ReadFLOAT(VOID)
	{
		//
		// 1. 检查数据流
		//
		if (IsValid() == FALSE) {
			return 0;
		}

		//
		// 2. 读入缓冲
		//
		BYTE buffer[4];

		if (1 != Read(buffer, sizeof(buffer), 1)) {
			return 0;
		}

		//
		// 3. 转换为浮点数
		//
		FLOAT data;

#ifdef _BIG_ENDIAN_

		((BYTE *)&data)[0] = buffer[3];
		((BYTE *)&data)[1] = buffer[2];
		((BYTE *)&data)[2] = buffer[1];
		((BYTE *)&data)[3] = buffer[0];

#else

		((BYTE *)&data)[0] = buffer[0];
		((BYTE *)&data)[1] = buffer[1];
		((BYTE *)&data)[2] = buffer[2];
		((BYTE *)&data)[3] = buffer[3];

#endif

		return data;
	}

	//
	// 定位
	//
	BOOL CStream::Seek(DWORD offset, SEEK_MODE mode)
	{
		//
		// 1. 检查数据流
		//
		if (IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 定位流位置
		//
		switch (mode) {
		case SEEK_MODE_CUR:
			if (m_dwPosition + offset > m_dwSize) {
				return FALSE;
			}

			m_dwPosition = m_dwPosition + offset;

			break;
		case SEEK_MODE_END:
			if (m_dwSize + offset > m_dwSize) {
				return FALSE;
			}

			m_dwPosition = m_dwSize + offset;

			break;
		case SEEK_MODE_SET:
			if (offset > m_dwSize) {
				return FALSE;
			}

			m_dwPosition = offset;

			break;
		}

		return TRUE;
	}

	//
	// 数据流结束判断
	//
	BOOL CStream::IsEOF(VOID) const
	{
		return (GetFreeSize() == 0) ? TRUE : FALSE;
	}

}

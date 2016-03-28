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

	class _CrossExport CStream
	{
		// 数据结构
	public:
		typedef enum {
			SEEK_MODE_CUR = SEEK_CUR,                                                              // 数据流当前位置
			SEEK_MODE_END = SEEK_END,                                                              // 数据流结尾
			SEEK_MODE_SET = SEEK_SET                                                               // 数据流开始
		} SEEK_MODE;


		// 构造/析构函数
	public:
		CStream(VOID);
		virtual ~CStream(VOID);


		// 方法
	public:
		BOOL IsValid(VOID) const;                                                                  // 有效性检查

		VOID Init(VOID);                                                                           // 初始化
		VOID Free(VOID);                                                                           // 释放数据流
		VOID FreeData(VOID);                                                                       // 释放数据流存储

		BOOL Alloc(DWORD size);                                                                    // 分配数据流存储
		BOOL CopyFrom(const CStream *pStream);                                                     // 复制数据流

	public:
		BOOL LoadFromFile(const CHAR *szFileName);                                                 // 从文件加载数据流
		BOOL LoadFromZip(const CHAR *szZipName, const CHAR *szFileName);                           // 从压缩包文件加载数据流
		BOOL LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName);                              // 从压缩包文件加载数据流

		BOOL Reload(VOID);                                                                         // 重新加载

	public:
		BOOL SetName(const CHAR *szName);                                                          // 设置数据流名
		const CHAR* GetName(VOID) const;                                                           // 获得数据流名
		DWORD GetHashName(VOID) const;                                                             // 获得数据流名

		BOOL SetFileName(const CHAR *szFileName);                                                  // 设置数据流文件名
		const CHAR* GetFileName(VOID) const;                                                       // 获得数据流文件名

		BOOL SetZipName(const CHAR *szZipName);                                                    // 设置压缩包名
		const CHAR* GetZipName(VOID) const;                                                        // 获得压缩包名

		BOOL SetZipPack(ZZIP_DIR *pZipPack);                                                       // 设置压缩包指针
		ZZIP_DIR* GetZipPack(VOID) const;                                                          // 获得压缩包指针

	public:
		DWORD GetFullSize(VOID) const;                                                             // 获得数据流空间
		DWORD GetFreeSize(VOID) const;                                                             // 获得数据流剩余空间

		BOOL SetStreamAddress(BYTE *pAddress, DWORD dwSize);                                       // 设置数据流数据地址

		VOID* GetBeginAddress(VOID) const;                                                         // 获得数据流起始地址
		VOID* GetCurrentAddress(VOID) const;                                                       // 获得数据流当前地址
		DWORD GetCurrentPosition(VOID) const;                                                      // 获得数据流当前位置

	public:
		DWORD Read(VOID *pBuffer, DWORD size, DWORD count);                                        // 加载数据
		DWORD ReadString(CHAR *szString, DWORD size = 0xffffffff);                                 // 读入字符串

		BYTE ReadBYTE(VOID);                                                                       // 加载BYTE
		WORD ReadWORD(VOID);                                                                       // 加载WORD
		DWORD ReadDWORD(VOID);                                                                     // 加载DWORD
		FLOAT ReadFLOAT(VOID);                                                                     // 读入FLOAT

		BOOL Seek(DWORD offset, SEEK_MODE mode);                                                   // 定位
		BOOL IsEOF(VOID) const;                                                                    // 数据流结束判断


		// 属性
	protected:
		DWORD m_dwName;                                                                            // 数据流名称
		CHAR m_szName[_MAX_STRING];                                                                // 数据流名称

		ZZIP_DIR *m_pZipPack;                                                                      // 压缩包指针
		CHAR m_szZipName[_MAX_STRING];                                                             // 压缩包名称
		CHAR m_szFileName[_MAX_STRING];                                                            // 文件名称

	protected:
		BOOL m_bAlloced;                                                                           // 数据流分配内存否
		BYTE *m_pAddress;                                                                          // 数据流地址

		DWORD m_dwSize;                                                                            // 数据流空间
		DWORD m_dwPosition;                                                                        // 当前位置
	};

}

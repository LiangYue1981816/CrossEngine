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
		// ���ݽṹ
	public:
		typedef enum {
			SEEK_MODE_CUR = SEEK_CUR,                                                              // ��������ǰλ��
			SEEK_MODE_END = SEEK_END,                                                              // ��������β
			SEEK_MODE_SET = SEEK_SET                                                               // ��������ʼ
		} SEEK_MODE;


		// ����/��������
	public:
		CStream(VOID);
		virtual ~CStream(VOID);


		// ����
	public:
		BOOL IsValid(VOID) const;                                                                  // ��Ч�Լ��

		VOID Init(VOID);                                                                           // ��ʼ��
		VOID Free(VOID);                                                                           // �ͷ�������
		VOID FreeData(VOID);                                                                       // �ͷ��������洢

		BOOL Alloc(DWORD size);                                                                    // �����������洢
		BOOL CopyFrom(const CStream *pStream);                                                     // ����������

	public:
		BOOL LoadFromFile(const CHAR *szFileName);                                                 // ���ļ�����������
		BOOL LoadFromZip(const CHAR *szZipName, const CHAR *szFileName);                           // ��ѹ�����ļ�����������
		BOOL LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName);                              // ��ѹ�����ļ�����������

		BOOL Reload(VOID);                                                                         // ���¼���

	public:
		BOOL SetName(const CHAR *szName);                                                          // ������������
		const CHAR* GetName(VOID) const;                                                           // �����������
		DWORD GetHashName(VOID) const;                                                             // �����������

		BOOL SetFileName(const CHAR *szFileName);                                                  // �����������ļ���
		const CHAR* GetFileName(VOID) const;                                                       // ����������ļ���

		BOOL SetZipName(const CHAR *szZipName);                                                    // ����ѹ������
		const CHAR* GetZipName(VOID) const;                                                        // ���ѹ������

		BOOL SetZipPack(ZZIP_DIR *pZipPack);                                                       // ����ѹ����ָ��
		ZZIP_DIR* GetZipPack(VOID) const;                                                          // ���ѹ����ָ��

	public:
		DWORD GetFullSize(VOID) const;                                                             // ����������ռ�
		DWORD GetFreeSize(VOID) const;                                                             // ���������ʣ��ռ�

		BOOL SetStreamAddress(BYTE *pAddress, DWORD dwSize);                                       // �������������ݵ�ַ

		VOID* GetBeginAddress(VOID) const;                                                         // �����������ʼ��ַ
		VOID* GetCurrentAddress(VOID) const;                                                       // �����������ǰ��ַ
		DWORD GetCurrentPosition(VOID) const;                                                      // �����������ǰλ��

	public:
		DWORD Read(VOID *pBuffer, DWORD size, DWORD count);                                        // ��������
		DWORD ReadString(CHAR *szString, DWORD size = 0xffffffff);                                 // �����ַ���

		BYTE ReadBYTE(VOID);                                                                       // ����BYTE
		WORD ReadWORD(VOID);                                                                       // ����WORD
		DWORD ReadDWORD(VOID);                                                                     // ����DWORD
		FLOAT ReadFLOAT(VOID);                                                                     // ����FLOAT

		BOOL Seek(DWORD offset, SEEK_MODE mode);                                                   // ��λ
		BOOL IsEOF(VOID) const;                                                                    // �����������ж�


		// ����
	protected:
		DWORD m_dwName;                                                                            // ����������
		CHAR m_szName[_MAX_STRING];                                                                // ����������

		ZZIP_DIR *m_pZipPack;                                                                      // ѹ����ָ��
		CHAR m_szZipName[_MAX_STRING];                                                             // ѹ��������
		CHAR m_szFileName[_MAX_STRING];                                                            // �ļ�����

	protected:
		BOOL m_bAlloced;                                                                           // �����������ڴ��
		BYTE *m_pAddress;                                                                          // ��������ַ

		DWORD m_dwSize;                                                                            // �������ռ�
		DWORD m_dwPosition;                                                                        // ��ǰλ��
	};

}

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

	class _CrossExport COcclusionCulling
	{
		friend class CSceneManager;


		// ���ݽṹ
	public:
		typedef struct {
			BYTE *pvs;                                                                             // Ǳ�ڿ��Ӽ�
		} CELL;

		typedef struct {
			AABB aabb;                                                                             // ��Χ������

			FLOAT stepx;                                                                           // X���򲽽�
			FLOAT stepy;                                                                           // Y���򲽽�
			FLOAT stepz;                                                                           // Z���򲽽�

			CELL ***pppCells;                                                                      // Ǳ�ڿ��Ӽ���Ԫ�񼯺�
		} VISIBLE_VOLUME;

		typedef std::vector<VISIBLE_VOLUME*> VisibleVolumeSet;                                     // �����弯��


		// ����/��������
	protected:
		COcclusionCulling(CSceneManager *pSceneManager);
		virtual ~COcclusionCulling(VOID);


		// ����
	public:
		VOID Init(VOID);                                                                           // ��ʼ��
		VOID Free(VOID);                                                                           // �ͷ�

	public:
		BOOL LoadFromStream(CStream *pStream);                                                     // ��������������Դ

	public:
		BOOL IsOccluded(INT index) const;                                                          // �ڵ��ж�
		VOID UpdateCulling(const CEntityCamera *pCamera) const;                                    // �����޳�


		// ����
	protected:
		BYTE *m_pvs;                                                                               // ��ǰǱ�ڿ��Ӽ�
		VisibleVolumeSet m_volumes;                                                                // �����弯��

	protected:
		CSceneManager *m_pSceneManager;                                                            // ����������
	};

}

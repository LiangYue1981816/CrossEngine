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

	class _CrossExport CEmitterBox : public CEmitterBase
	{
		friend class CParticleSystem;


		// ����/��������
	protected:
		CEmitterBox(CParticleSystem *pParent);
		virtual ~CEmitterBox(VOID);


		// ����
	public:
		virtual TYPE GetType(VOID) const;                                                          // �������

		virtual VOID Init(VOID);                                                                   // ��ʼ��
		virtual VOID Free(VOID);                                                                   // �ͷ�

		virtual VOID LoadXML(TiXmlNode *pEmitterNode);                                             // ����XML
		virtual VOID SaveXML(TiXmlNode *pEmitterNode) const;                                       // ����XML

		virtual BOOL Emit(CParticle *pParticle);                                                   // ��������

	public:
		VOID SetWidth(FLOAT width);                                                                // ���úп�
		VOID SetHeight(FLOAT height);                                                              // ���úи�
		VOID SetLength(FLOAT length);                                                              // ���úг�

		FLOAT GetWidth(VOID) const;                                                                // ��úп�
		FLOAT GetHeight(VOID) const;                                                               // ��úи�
		FLOAT GetLength(VOID) const;                                                               // ��úг�


		// ����
	protected:
		FLOAT m_width;                                                                             // �п�
		FLOAT m_height;                                                                            // �и�
		FLOAT m_length;                                                                            // �г�
	};

}

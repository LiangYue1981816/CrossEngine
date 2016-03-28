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

	class _CrossExport CEmitterSphere : public CEmitterBase
	{
		friend class CParticleSystem;


		// ����/��������
	protected:
		CEmitterSphere(CParticleSystem *pParent);
		virtual ~CEmitterSphere(VOID);


		// ����
	public:
		virtual TYPE GetType(VOID) const;                                                          // �������

		virtual VOID Init(VOID);                                                                   // ��ʼ��
		virtual VOID Free(VOID);                                                                   // �ͷ�

		virtual VOID LoadXML(TiXmlNode *pEmitterNode);                                             // ����XML
		virtual VOID SaveXML(TiXmlNode *pEmitterNode) const;                                       // ����XML

		virtual BOOL Emit(CParticle *pParticle);                                                   // ��������

	public:
		VOID SetRadius(FLOAT radius);                                                              // ������뾶
		FLOAT GetRadius(VOID) const;                                                               // �����뾶

		VOID SetSurface(BOOL bSurface);                                                            // ���ñ��淢��
		BOOL IsSurface(VOID) const;                                                                // ��ñ��淢��

		VOID SetEnableDirectionX(BOOL bEnable);                                                    // ��������X����״̬
		VOID SetEnableDirectionY(BOOL bEnable);                                                    // ��������Y����״̬
		VOID SetEnableDirectionZ(BOOL bEnable);                                                    // ��������Z����״̬

		BOOL IsEnableDirectionX(VOID) const;                                                       // �������X����״̬
		BOOL IsEnableDirectionY(VOID) const;                                                       // �������Y����״̬
		BOOL IsEnableDirectionZ(VOID) const;                                                       // �������Z����״̬


		// ����
	protected:
		FLOAT m_radius;                                                                            // �뾶

		BOOL m_bSurface;                                                                           // ����
		BOOL m_bEnableDirectionX;                                                                  // ����X����״̬
		BOOL m_bEnableDirectionY;                                                                  // ����Y����״̬
		BOOL m_bEnableDirectionZ;                                                                  // ����Z����״̬
	};

}

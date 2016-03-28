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

	class _CrossExport CGfxBillboard : public CGfxBase
	{
		friend class CParticleSystem;


		// ���ݽṹ
	public:
		typedef enum {
			DIRECTION_CAMERA,                                                                      // ���������
			DIRECTION_FIXED                                                                        // �̶�����
		} DIRECTION_TYPE;


		// ����/��������
	protected:
		CGfxBillboard(VOID);
		virtual ~CGfxBillboard(VOID);


		// ����
	public:
		virtual TYPE GetType(VOID) const;                                                          // �������

		virtual VOID Init(VOID);                                                                   // ��ʼ��
		virtual VOID Free(VOID);                                                                   // �ͷ�

	public:
		virtual VOID LoadXML(TiXmlNode *pGfxNode);                                                 // ����XML
		virtual VOID SaveXML(TiXmlNode *pGfxNode) const;                                           // ����XML

	public:
		virtual VOID SetParticleQuota(INT quota);                                                  // ������������

	public:
		virtual VOID Update(const CEntityCamera *pCamera, CParticle *pParticleList, INT numParticles); // ����
		virtual VOID Render(INT numParticles);                                                     // ��Ⱦ

	public:
		VOID SetOffset(FLOAT offset);                                                              // ����ƫ����
		FLOAT GetOffset(VOID) const;                                                               // ���ƫ����

		VOID SetDirectionType(DIRECTION_TYPE type);                                                // ���ó�������
		DIRECTION_TYPE GetDirectionType(VOID) const;                                               // ��ó�������


		// ����
	protected:
		FLOAT m_offset;                                                                            // ��Ƭƫ����
		MATRIX4 m_mtxFaceToCamera;                                                                 // ���������ת����
		DIRECTION_TYPE m_directionType;                                                            // ��������
	};

}

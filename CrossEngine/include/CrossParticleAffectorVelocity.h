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

	class _CrossExport CAffectorVelocity : public CAffectorBase
	{
		friend class CParticleSystem;


		// ����/��������
	protected:
		CAffectorVelocity(CParticleSystem *pParent);
		virtual ~CAffectorVelocity(VOID);


		// ����
	public:
		virtual TYPE GetType(VOID) const;                                                          // �������

		virtual VOID Init(VOID);                                                                   // ��ʼ��
		virtual VOID Free(VOID);                                                                   // �ͷ�

		virtual VOID LoadXML(TiXmlNode *pAffectorNode);                                            // ����XML
		virtual VOID SaveXML(TiXmlNode *pAffectorNode) const;                                      // ����XML

		virtual VOID Affect(CParticle *pParticle, FLOAT deltaTime);                                // Ӱ������

	public:
		VOID SetVelocityRatio(FLOAT minVelocityRatio, FLOAT maxVelocityRatio);                     // �����ٶȱ���
		FLOAT GetMinVelocityRatio(VOID) const;                                                     // ������С�ٶȱ���
		FLOAT GetMaxVelocityRatio(VOID) const;                                                     // ��������ٶȱ���


		// ����
	protected:
		FLOAT m_minVelocityRatio;                                                                  // ��С�ٶȱ���
		FLOAT m_maxVelocityRatio;                                                                  // ����ٶȱ���
	};

}

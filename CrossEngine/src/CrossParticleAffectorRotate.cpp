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

	CAffectorRotate::CAffectorRotate(CParticleSystem *pParent)
		: CAffectorBase(pParent)
	{
		Init();
	}

	CAffectorRotate::~CAffectorRotate(VOID)
	{
		Free();
	}

	//
	// �������
	//
	CAffectorBase::TYPE CAffectorRotate::GetType(VOID) const
	{
		return CAffectorBase::ROTATE;
	}

	//
	// ��ʼ��
	//
	VOID CAffectorRotate::Init(VOID)
	{
		m_minRotateSpeed = 0.0f;
		m_maxRotateSpeed = 0.0f;

		CAffectorBase::Init();
	}

	//
	// �ͷ�
	//
	VOID CAffectorRotate::Free(VOID)
	{
		CAffectorBase::Free();
	}

	//
	// ����XML
	//
	VOID CAffectorRotate::LoadXML(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);

		if (TiXmlNode *pRotateNode = pAffectorNode->FirstChild("Rotate")) {
			SetRotateSpeed(
				pRotateNode->ToElement()->AttributeFloat("min_value"), 
				pRotateNode->ToElement()->AttributeFloat("max_value"));
		}

		CAffectorBase::LoadXML(pAffectorNode);
	}

	//
	// ����XML
	//
	VOID CAffectorRotate::SaveXML(TiXmlNode *pAffectorNode) const
	{
		ASSERT(pAffectorNode);

		TiXmlElement *pRotateNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Rotate"); ASSERT(pRotateNode);
		{
			pRotateNode->SetAttributeFloat("min_value", m_minRotateSpeed);
			pRotateNode->SetAttributeFloat("max_value", m_maxRotateSpeed);
		}
		pAffectorNode->LinkEndChild(pRotateNode);

		CAffectorBase::SaveXML(pAffectorNode);
	}

	//
	// Ӱ������
	//
	VOID CAffectorRotate::Affect(CParticle *pParticle, FLOAT deltaTime)
	{
		if (IsCanAffect(pParticle)) {
			if (pParticle->deltaRotateSpeed.find(this) == pParticle->deltaRotateSpeed.end()) {
				pParticle->deltaRotateSpeed[this] = m_minRotateSpeed + FRandomValue() * (m_maxRotateSpeed - m_minRotateSpeed);
			}

			pParticle->radian += deltaTime * pParticle->deltaRotateSpeed[this];
		}
	}

	//
	// ������ת�ٶ�
	//
	VOID CAffectorRotate::SetRotateSpeed(FLOAT minSpeed, FLOAT maxSpeed)
	{
		m_minRotateSpeed = minSpeed;
		m_maxRotateSpeed = maxSpeed;
	}

	//
	// ������С��ת�ٶ�
	//
	FLOAT CAffectorRotate::GetMinRotateSpeed(VOID) const
	{
		return m_minRotateSpeed;
	}

	//
	// ���������ת�ٶ�
	//
	FLOAT CAffectorRotate::GetMaxRotateSpeed(VOID) const
	{
		return m_maxRotateSpeed;
	}

}

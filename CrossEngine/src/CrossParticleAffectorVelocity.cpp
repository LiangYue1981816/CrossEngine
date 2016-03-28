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

	CAffectorVelocity::CAffectorVelocity(CParticleSystem *pParent)
		: CAffectorBase(pParent)
	{
		Init();
	}

	CAffectorVelocity::~CAffectorVelocity(VOID)
	{
		Free();
	}

	//
	// �������
	//
	CAffectorBase::TYPE CAffectorVelocity::GetType(VOID) const
	{
		return CAffectorBase::VELOCITY;
	}

	//
	// ��ʼ��
	//
	VOID CAffectorVelocity::Init(VOID)
	{
		m_minVelocityRatio = 1.0f;
		m_maxVelocityRatio = 1.0f;

		CAffectorBase::Init();
	}

	//
	// �ͷ�
	//
	VOID CAffectorVelocity::Free(VOID)
	{
		CAffectorBase::Free();
	}

	//
	// ����XML
	//
	VOID CAffectorVelocity::LoadXML(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);

		if (TiXmlNode *pVelocityNode = pAffectorNode->FirstChild("Velocity")) {
			SetVelocityRatio(
				pVelocityNode->ToElement()->AttributeFloat("min_value"), 
				pVelocityNode->ToElement()->AttributeFloat("max_value"));
		}

		CAffectorBase::LoadXML(pAffectorNode);
	}

	//
	// ����XML
	//
	VOID CAffectorVelocity::SaveXML(TiXmlNode *pAffectorNode) const
	{
		ASSERT(pAffectorNode);

		TiXmlElement *pVelocityNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Velocity"); ASSERT(pVelocityNode);
		{
			pVelocityNode->SetAttributeFloat("min_value", m_minVelocityRatio);
			pVelocityNode->SetAttributeFloat("max_value", m_maxVelocityRatio);
		}
		pAffectorNode->LinkEndChild(pVelocityNode);

		CAffectorBase::SaveXML(pAffectorNode);
	}

	//
	// Ӱ������
	//
	VOID CAffectorVelocity::Affect(CParticle *pParticle, FLOAT deltaTime)
	{
		if (IsCanAffect(pParticle)) {
			if (pParticle->deltaVelocityRatios.find(this) == pParticle->deltaVelocityRatios.end()) {
				pParticle->deltaVelocityRatios[this] = m_minVelocityRatio + FRandomValue() * (m_maxVelocityRatio - m_minVelocityRatio);
			}

			pParticle->localVelocity[0] *= pParticle->deltaVelocityRatios[this];
			pParticle->localVelocity[1] *= pParticle->deltaVelocityRatios[this];
			pParticle->localVelocity[2] *= pParticle->deltaVelocityRatios[this];
		}
	}

	//
	// �����ٶȱ���
	//
	VOID CAffectorVelocity::SetVelocityRatio(FLOAT minVelocityRatio, FLOAT maxVelocityRatio)
	{
		m_minVelocityRatio = minVelocityRatio;
		m_maxVelocityRatio = maxVelocityRatio;
	}

	//
	// ������С�ٶȱ���
	//
	FLOAT CAffectorVelocity::GetMinVelocityRatio(VOID) const
	{
		return m_minVelocityRatio;
	}

	//
	// ��������ٶȱ���
	//
	FLOAT CAffectorVelocity::GetMaxVelocityRatio(VOID) const
	{
		return m_maxVelocityRatio;
	}

}

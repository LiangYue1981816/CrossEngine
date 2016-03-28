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

	CAffectorScale::CAffectorScale(CParticleSystem *pParent)
		: CAffectorBase(pParent)
	{
		Init();
	}

	CAffectorScale::~CAffectorScale(VOID)
	{
		Free();
	}

	//
	// �������
	//
	CAffectorBase::TYPE CAffectorScale::GetType(VOID) const
	{
		return CAffectorBase::SCALE;
	}

	//
	// ��ʼ��
	//
	VOID CAffectorScale::Init(VOID)
	{
		Vec3Set(&m_minScale, 1.0f, 1.0f, 1.0f);
		Vec3Set(&m_maxScale, 1.0f, 1.0f, 1.0f);

		CAffectorBase::Init();
	}

	//
	// �ͷ�
	//
	VOID CAffectorScale::Free(VOID)
	{
		CAffectorBase::Free();
	}

	//
	// ����XML
	//
	VOID CAffectorScale::LoadXML(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);

		if (TiXmlNode *pScaleNode = pAffectorNode->FirstChild("Scale")) {
			VEC3 minScale;
			VEC3 maxScale;
			sscanf(pScaleNode->ToElement()->AttributeString("min_value"), "%f %f %f", &minScale[0], &minScale[1], &minScale[2]);
			sscanf(pScaleNode->ToElement()->AttributeString("max_value"), "%f %f %f", &maxScale[0], &maxScale[1], &maxScale[2]);
			SetScale(&minScale, &maxScale);
		}

		CAffectorBase::LoadXML(pAffectorNode);
	}

	//
	// ����XML
	//
	VOID CAffectorScale::SaveXML(TiXmlNode *pAffectorNode) const
	{
		ASSERT(pAffectorNode);

		TiXmlElement *pScaleNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Scale"); ASSERT(pScaleNode);
		{
			pScaleNode->SetAttributeString("min_value", "%f %f %f", m_minScale[0], m_minScale[1], m_minScale[2]);
			pScaleNode->SetAttributeString("max_value", "%f %f %f", m_maxScale[0], m_maxScale[1], m_maxScale[2]);
		}
		pAffectorNode->LinkEndChild(pScaleNode);

		CAffectorBase::SaveXML(pAffectorNode);
	}

	//
	// Ӱ������
	//
	VOID CAffectorScale::Affect(CParticle *pParticle, FLOAT deltaTime)
	{
		if (IsCanAffect(pParticle)) {
			if (pParticle->deltaScale.find(this) == pParticle->deltaScale.end()) {
				FLOAT scale = FRandomValue();
				pParticle->deltaScale[this][0] = ((m_minScale[0] + scale * (m_maxScale[0] - m_minScale[0])) - pParticle->localScale[0]) / (m_endTime - m_beginTime);
				pParticle->deltaScale[this][1] = ((m_minScale[1] + scale * (m_maxScale[1] - m_minScale[1])) - pParticle->localScale[1]) / (m_endTime - m_beginTime);
				pParticle->deltaScale[this][2] = ((m_minScale[2] + scale * (m_maxScale[2] - m_minScale[2])) - pParticle->localScale[2]) / (m_endTime - m_beginTime);
			}

			pParticle->localScale[0] += deltaTime * pParticle->deltaScale[this][0];
			pParticle->localScale[1] += deltaTime * pParticle->deltaScale[this][1];
			pParticle->localScale[2] += deltaTime * pParticle->deltaScale[this][2];

			if (pParticle->localScale[0] < 0.0f) pParticle->localScale[0] = 0.0f;
			if (pParticle->localScale[1] < 0.0f) pParticle->localScale[1] = 0.0f;
			if (pParticle->localScale[2] < 0.0f) pParticle->localScale[2] = 0.0f;
		}
	}

	//
	// ��������
	//
	VOID CAffectorScale::SetScale(FLOAT minScaleX, FLOAT minScaleY, FLOAT minScaleZ, FLOAT maxScaleX, FLOAT maxScaleY, FLOAT maxScaleZ)
	{
		Vec3Set(&m_minScale, minScaleX, minScaleY, minScaleZ);
		Vec3Set(&m_maxScale, maxScaleX, maxScaleY, maxScaleZ);
	}

	//
	// ��������
	//
	VOID CAffectorScale::SetScale(const VEC3* minScale, const VEC3 *maxScale)
	{
		ASSERT(minScale);
		ASSERT(maxScale);
		Vec3Copy(&m_minScale, minScale);
		Vec3Copy(&m_maxScale, maxScale);
	}

	//
	// �����С����
	//
	const VEC3* CAffectorScale::GetMinScale(VOID) const
	{
		return &m_minScale;
	}

	//
	// ����������
	//
	const VEC3* CAffectorScale::GetMaxScale(VOID) const
	{
		return &m_maxScale;
	}

}

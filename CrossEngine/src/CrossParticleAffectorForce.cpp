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

	CAffectorForce::CAffectorForce(CParticleSystem *pParent)
		: CAffectorBase(pParent)
	{
		Init();
	}

	CAffectorForce::~CAffectorForce(VOID)
	{
		Free();
	}

	//
	// 获得类型
	//
	CAffectorBase::TYPE CAffectorForce::GetType(VOID) const
	{
		return CAffectorBase::FORCE;
	}

	//
	// 初始化
	//
	VOID CAffectorForce::Init(VOID)
	{
		Vec3Set(&m_minForce, 0.0f, 0.0f, 0.0f);
		Vec3Set(&m_maxForce, 0.0f, 0.0f, 0.0f);

		CAffectorBase::Init();
	}

	//
	// 释放
	//
	VOID CAffectorForce::Free(VOID)
	{
		CAffectorBase::Free();
	}

	//
	// 加载XML
	//
	VOID CAffectorForce::LoadXML(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);

		if (TiXmlNode *pForceNode = pAffectorNode->FirstChild("Force")) {
			VEC3 minForce;
			VEC3 maxForce;
			sscanf(pForceNode->ToElement()->AttributeString("min_value"), "%f %f %f", &minForce[0], &minForce[1], &minForce[2]);
			sscanf(pForceNode->ToElement()->AttributeString("max_value"), "%f %f %f", &maxForce[0], &maxForce[1], &maxForce[2]);
			SetForce(&minForce, &maxForce);
		}

		CAffectorBase::LoadXML(pAffectorNode);
	}

	//
	// 保存XML
	//
	VOID CAffectorForce::SaveXML(TiXmlNode *pAffectorNode) const
	{
		ASSERT(pAffectorNode);

		TiXmlElement *pForceNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Force"); ASSERT(pForceNode);
		{
			pForceNode->SetAttributeString("min_value", "%f %f %f", m_minForce[0], m_minForce[1], m_minForce[2]);
			pForceNode->SetAttributeString("max_value", "%f %f %f", m_maxForce[0], m_maxForce[1], m_maxForce[2]);
		}
		pAffectorNode->LinkEndChild(pForceNode);

		CAffectorBase::SaveXML(pAffectorNode);
	}

	//
	// 影响粒子
	//
	VOID CAffectorForce::Affect(CParticle *pParticle, FLOAT deltaTime)
	{
		if (IsCanAffect(pParticle)) {
			if (pParticle->deltaForce.find(this) == pParticle->deltaForce.end()) {
				FLOAT scale = FRandomValue();
				pParticle->deltaForce[this][0] = m_minForce[0] + scale * (m_maxForce[0] - m_minForce[0]);
				pParticle->deltaForce[this][1] = m_minForce[1] + scale * (m_maxForce[1] - m_minForce[1]);
				pParticle->deltaForce[this][2] = m_minForce[2] + scale * (m_maxForce[2] - m_minForce[2]);
			}

			pParticle->localVelocity[0] += deltaTime * pParticle->deltaForce[this][0];
			pParticle->localVelocity[1] += deltaTime * pParticle->deltaForce[this][1];
			pParticle->localVelocity[2] += deltaTime * pParticle->deltaForce[this][2];
		}
	}

	//
	// 设置力
	//
	VOID CAffectorForce::SetForce(FLOAT minForceX, FLOAT minForceY, FLOAT minForceZ, FLOAT maxForceX, FLOAT maxForceY, FLOAT maxForceZ)
	{
		Vec3Set(&m_minForce, minForceX, minForceY, minForceZ);
		Vec3Set(&m_maxForce, maxForceX, maxForceY, maxForceZ);
	}

	//
	// 设置力
	//
	VOID CAffectorForce::SetForce(const VEC3 *minForce, const VEC3 *maxForce)
	{
		ASSERT(minForce);
		ASSERT(maxForce);
		Vec3Copy(&m_minForce, minForce);
		Vec3Copy(&m_maxForce, maxForce);
	}

	//
	// 获得力
	//
	const VEC3* CAffectorForce::GetMinForce(VOID) const
	{
		return &m_minForce;
	}

	//
	// 获得力
	//
	const VEC3* CAffectorForce::GetMaxForce(VOID) const
	{
		return &m_maxForce;
	}

}

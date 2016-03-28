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

	CAffectorRandom::CAffectorRandom(CParticleSystem *pParent)
		: CAffectorBase(pParent)
	{
		Init();
	}

	CAffectorRandom::~CAffectorRandom(VOID)
	{
		Free();
	}

	//
	// 获得类型
	//
	CAffectorBase::TYPE CAffectorRandom::GetType(VOID) const
	{
		return CAffectorBase::RANDOM;
	}

	//
	// 初始化
	//
	VOID CAffectorRandom::Init(VOID)
	{
		Vec3Set(&m_minDirection, 0.0f, 0.0f, 0.0f);
		Vec3Set(&m_maxDirection, 0.0f, 0.0f, 0.0f);

		CAffectorBase::Init();
	}

	//
	// 释放
	//
	VOID CAffectorRandom::Free(VOID)
	{
		CAffectorBase::Free();
	}

	//
	// 加载XML
	//
	VOID CAffectorRandom::LoadXML(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);

		if (TiXmlNode *pDirectionNode = pAffectorNode->FirstChild("Direction")) {
			VEC3 minDirection;
			VEC3 maxDirection;
			sscanf(pDirectionNode->ToElement()->AttributeString("min_value"), "%f %f %f", &minDirection[0], &minDirection[1], &minDirection[2]);
			sscanf(pDirectionNode->ToElement()->AttributeString("max_value"), "%f %f %f", &maxDirection[0], &maxDirection[1], &maxDirection[2]);
			SetDirection(&minDirection, &maxDirection);
		}

		CAffectorBase::LoadXML(pAffectorNode);
	}

	//
	// 保存XML
	//
	VOID CAffectorRandom::SaveXML(TiXmlNode *pAffectorNode) const
	{
		ASSERT(pAffectorNode);

		TiXmlElement *pDirectionNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Direction"); ASSERT(pDirectionNode);
		{
			pDirectionNode->SetAttributeString("min_value", "%f %f %f", m_minDirection[0], m_minDirection[1], m_minDirection[2]);
			pDirectionNode->SetAttributeString("max_value", "%f %f %f", m_maxDirection[0], m_maxDirection[1], m_maxDirection[2]);
		}
		pAffectorNode->LinkEndChild(pDirectionNode);

		CAffectorBase::SaveXML(pAffectorNode);
	}

	//
	// 影响粒子
	//
	VOID CAffectorRandom::Affect(CParticle *pParticle, FLOAT deltaTime)
	{
		if (IsCanAffect(pParticle)) {
			pParticle->localVelocity[0] += deltaTime * (m_minDirection[0] + FRandomValue() * (m_maxDirection[0] - m_minDirection[0]));
			pParticle->localVelocity[1] += deltaTime * (m_minDirection[1] + FRandomValue() * (m_maxDirection[1] - m_minDirection[1]));
			pParticle->localVelocity[2] += deltaTime * (m_minDirection[2] + FRandomValue() * (m_maxDirection[2] - m_minDirection[2]));
		}
	}

	//
	// 设置方向
	//
	VOID CAffectorRandom::SetDirection(FLOAT minDirectionX, FLOAT minDirectionY, FLOAT minDirectionZ, FLOAT maxDirectionX, FLOAT maxDirectionY, FLOAT maxDirectionZ)
	{
		Vec3Set(&m_minDirection, minDirectionX, minDirectionY, minDirectionZ);
		Vec3Set(&m_maxDirection, maxDirectionX, maxDirectionY, maxDirectionZ);
	}

	//
	// 设置方向
	//
	VOID CAffectorRandom::SetDirection(const VEC3* minDirection, const VEC3 *maxDirection)
	{
		ASSERT(minDirection);
		ASSERT(maxDirection);
		Vec3Copy(&m_minDirection, minDirection);
		Vec3Copy(&m_maxDirection, maxDirection);
	}

	//
	// 获得最小方向
	//
	const VEC3* CAffectorRandom::GetMinDirection(VOID) const
	{
		return &m_minDirection;
	}

	//
	// 获得最大方向
	//
	const VEC3* CAffectorRandom::GetMaxDirection(VOID) const
	{
		return &m_maxDirection;
	}

}

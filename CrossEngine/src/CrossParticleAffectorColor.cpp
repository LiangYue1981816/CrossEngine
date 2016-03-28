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

	CAffectorColor::CAffectorColor(CParticleSystem *pParent)
		: CAffectorBase(pParent)
	{
		Init();
	}

	CAffectorColor::~CAffectorColor(VOID)
	{
		Free();
	}

	//
	// 获得类型
	//
	CAffectorBase::TYPE CAffectorColor::GetType(VOID) const
	{
		return CAffectorBase::COLOR;
	}

	//
	// 初始化
	//
	VOID CAffectorColor::Init(VOID)
	{
		Vec4Set(&m_minColor, 1.0f, 1.0f, 1.0f, 1.0f);
		Vec4Set(&m_maxColor, 1.0f, 1.0f, 1.0f, 1.0f);

		CAffectorBase::Init();
	}

	//
	// 释放
	//
	VOID CAffectorColor::Free(VOID)
	{
		CAffectorBase::Free();
	}

	//
	// 加载XML
	//
	VOID CAffectorColor::LoadXML(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);

		if (TiXmlNode *pColorNode = pAffectorNode->FirstChild("Color")) {
			VEC4 minColor;
			VEC4 maxColor;
			sscanf(pColorNode->ToElement()->AttributeString("min_value"), "%f %f %f %f", &minColor[0], &minColor[1], &minColor[2], &minColor[3]);
			sscanf(pColorNode->ToElement()->AttributeString("max_value"), "%f %f %f %f", &maxColor[0], &maxColor[1], &maxColor[2], &maxColor[3]);
			SetColor(&minColor, &maxColor);
		}

		CAffectorBase::LoadXML(pAffectorNode);
	}

	//
	// 保存XML
	//
	VOID CAffectorColor::SaveXML(TiXmlNode *pAffectorNode) const
	{
		ASSERT(pAffectorNode);

		TiXmlElement *pColorNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Color"); ASSERT(pColorNode);
		{
			pColorNode->SetAttributeString("min_value", "%f %f %f %f", m_minColor[0], m_minColor[1], m_minColor[2], m_minColor[3]);
			pColorNode->SetAttributeString("max_value", "%f %f %f %f", m_maxColor[0], m_maxColor[1], m_maxColor[2], m_maxColor[3]);
		}
		pAffectorNode->LinkEndChild(pColorNode);

		CAffectorBase::SaveXML(pAffectorNode);
	}

	//
	// 影响粒子
	//
	VOID CAffectorColor::Affect(CParticle *pParticle, FLOAT deltaTime)
	{
		if (IsCanAffect(pParticle)) {
			if (pParticle->deltaColor.find(this) == pParticle->deltaColor.end()) {
				FLOAT scale = FRandomValue();
				pParticle->deltaColor[this][0] = ((m_minColor[0] + scale * (m_maxColor[0] - m_minColor[0])) - pParticle->color[0]) / (m_endTime - m_beginTime);
				pParticle->deltaColor[this][1] = ((m_minColor[1] + scale * (m_maxColor[1] - m_minColor[1])) - pParticle->color[1]) / (m_endTime - m_beginTime);
				pParticle->deltaColor[this][2] = ((m_minColor[2] + scale * (m_maxColor[2] - m_minColor[2])) - pParticle->color[2]) / (m_endTime - m_beginTime);
				pParticle->deltaColor[this][3] = ((m_minColor[3] + scale * (m_maxColor[3] - m_minColor[3])) - pParticle->color[3]) / (m_endTime - m_beginTime);
			}

			pParticle->color[0] += deltaTime * pParticle->deltaColor[this][0];
			pParticle->color[1] += deltaTime * pParticle->deltaColor[this][1];
			pParticle->color[2] += deltaTime * pParticle->deltaColor[this][2];
			pParticle->color[3] += deltaTime * pParticle->deltaColor[this][3];

			Vec4Clamp(&pParticle->color);
		}
	}

	//
	// 设置颜色
	//
	VOID CAffectorColor::SetColor(FLOAT minR, FLOAT minG, FLOAT minB, FLOAT minA, FLOAT maxR, FLOAT maxG, FLOAT maxB, FLOAT maxA)
	{
		Vec4Set(&m_minColor, minR, minG, minB, minA);
		Vec4Set(&m_maxColor, maxR, maxG, maxB, maxA);

		Vec4Clamp(&m_minColor);
		Vec4Clamp(&m_maxColor);
	}

	//
	// 设置颜色
	//
	VOID CAffectorColor::SetColor(const VEC4 *minColor, const VEC4 *maxColor)
	{
		ASSERT(minColor);
		ASSERT(maxColor);

		Vec4Copy(&m_minColor, minColor);
		Vec4Copy(&m_maxColor, maxColor);

		Vec4Clamp(&m_minColor);
		Vec4Clamp(&m_maxColor);
	}

	//
	// 获得颜色
	//
	const VEC4* CAffectorColor::GetMinColor(VOID) const
	{
		return &m_minColor;
	}

	//
	// 获得颜色
	//
	const VEC4* CAffectorColor::GetMaxColor(VOID) const
	{
		return &m_maxColor;
	}

}

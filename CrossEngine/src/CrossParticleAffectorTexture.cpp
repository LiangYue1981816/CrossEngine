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

	CAffectorTexture::CAffectorTexture(CParticleSystem *pParent)
		: CAffectorBase(pParent)
	{
		Init();
	}

	CAffectorTexture::~CAffectorTexture(VOID)
	{
		Free();
	}

	//
	// 获得类型
	//
	CAffectorBase::TYPE CAffectorTexture::GetType(VOID) const
	{
		return CAffectorBase::TEXTURE;
	}

	//
	// 初始化
	//
	VOID CAffectorTexture::Init(VOID)
	{
		m_texScrollU = 0.0f;
		m_texScrollV = 0.0f;

		m_texSequenceCols = 1;
		m_texSequenceRows = 1;
		m_texSequenceDurationTime = 0.0f;

		CAffectorBase::Init();
	}

	//
	// 释放
	//
	VOID CAffectorTexture::Free(VOID)
	{
		CAffectorBase::Free();
	}

	//
	// 加载XML
	//
	VOID CAffectorTexture::LoadXML(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);

		if (TiXmlNode *pScrollNode = pAffectorNode->FirstChild("Scroll")) {
			SetTexScrollU(pScrollNode->ToElement()->AttributeFloat("u"));
			SetTexScrollV(pScrollNode->ToElement()->AttributeFloat("v"));
		}

		if (TiXmlNode *pSequenceNode = pAffectorNode->FirstChild("Sequence")) {
			SetTexSequence(
				pSequenceNode->ToElement()->AttributeInt("cols"),
				pSequenceNode->ToElement()->AttributeInt("rows"),
				pSequenceNode->ToElement()->AttributeFloat("duration"));
		}

		CAffectorBase::LoadXML(pAffectorNode);
	}

	//
	// 保存XML
	//
	VOID CAffectorTexture::SaveXML(TiXmlNode *pAffectorNode) const
	{
		ASSERT(pAffectorNode);

		TiXmlElement *pScrollNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Scroll"); ASSERT(pScrollNode);
		{
			pScrollNode->SetAttributeFloat("u", m_texScrollU);
			pScrollNode->SetAttributeFloat("v", m_texScrollV);
		}
		pAffectorNode->LinkEndChild(pScrollNode);

		TiXmlElement *pSequenceNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Sequence"); ASSERT(pSequenceNode);
		{
			pSequenceNode->SetAttributeInt("cols", m_texSequenceCols);
			pSequenceNode->SetAttributeInt("rows", m_texSequenceRows);
			pSequenceNode->SetAttributeFloat("duration", m_texSequenceDurationTime);
		}
		pAffectorNode->LinkEndChild(pSequenceNode);

		CAffectorBase::SaveXML(pAffectorNode);
	}

	//
	// 影响粒子
	//
	VOID CAffectorTexture::Affect(CParticle *pParticle, FLOAT deltaTime)
	{
		if (IsCanAffect(pParticle)) {
			if (pParticle->texSequenceTime.find(this) == pParticle->texSequenceTime.end()) {
				pParticle->texSequenceTime[this] = 0.0f;
			}

			pParticle->texSequenceTime[this] += deltaTime;

			if (m_texSequenceCols >= 1 && m_texSequenceRows >= 1 && m_texSequenceDurationTime > 0.0f) {
				INT index = (INT)(pParticle->texSequenceTime[this] / (m_texSequenceDurationTime / (m_texSequenceCols * m_texSequenceRows)) + 0.5f);
				index = index % (m_texSequenceCols * m_texSequenceRows);

				pParticle->texSequenceScale[0] = 1.0f / m_texSequenceCols;
				pParticle->texSequenceScale[1] = 1.0f / m_texSequenceRows;
				pParticle->texSequenceOffset[0] = pParticle->texSequenceScale[0] * (index % m_texSequenceCols);
				pParticle->texSequenceOffset[1] = pParticle->texSequenceScale[1] * (index / m_texSequenceCols);
			}

			if (m_texScrollU != 0.0f) {
				pParticle->texScrollOffset[0] = m_texScrollU * pParticle->texSequenceTime[this];
			}

			if (m_texScrollV != 0.0f) {
				pParticle->texScrollOffset[1] = m_texScrollV * pParticle->texSequenceTime[this];
			}
		}
	}

	//
	// 设置纹理横向滚动
	//
	VOID CAffectorTexture::SetTexScrollU(FLOAT u)
	{
		m_texScrollU = u;
	}

	//
	// 设置纹理纵向滚动
	//
	VOID CAffectorTexture::SetTexScrollV(FLOAT v)
	{
		m_texScrollV = v;
	}

	//
	// 设置纹理序列帧
	//
	VOID CAffectorTexture::SetTexSequence(UINT cols, UINT rows, FLOAT duration)
	{
		m_texSequenceCols = cols;
		m_texSequenceRows = rows;
		m_texSequenceDurationTime = duration;
	}

	//
	// 获得纹理横向滚动
	//
	FLOAT CAffectorTexture::GetTexScrollU(VOID) const
	{
		return m_texScrollU;
	}

	//
	// 获得纹理纵向滚动
	//
	FLOAT CAffectorTexture::GetTexScrollV(VOID) const
	{
		return m_texScrollV;
	}

	//
	// 获得纹理序列帧列数
	//
	UINT CAffectorTexture::GetTexSequenceCols(VOID) const
	{
		return m_texSequenceCols;
	}

	//
	// 获得纹理序列帧行数
	//
	UINT CAffectorTexture::GetTexSequenceRows(VOID) const
	{
		return m_texSequenceRows;
	}

	//
	// 获得纹理序列帧持续时间
	//
	FLOAT CAffectorTexture::GetTexSequenceDurationTime(VOID) const
	{
		return m_texSequenceDurationTime;
	}

}

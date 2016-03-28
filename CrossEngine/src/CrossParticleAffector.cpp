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

	CAffectorBase::CAffectorBase(CParticleSystem *pParent)
		: m_pParent(pParent)
	{
		Init();
	}

	CAffectorBase::~CAffectorBase(VOID)
	{
		Free();
	}

	//
	// 获得粒子系统
	//
	CParticleSystem* CAffectorBase::GetParticleSystem(VOID) const
	{
		return m_pParent;
	}

	//
	// 初始化
	//
	VOID CAffectorBase::Init(VOID)
	{
		//
		// 1. 影响器参数
		//
		m_step = 0.0f;
		m_delta = 0.0f;

		m_minBeginTime = 0.0f;
		m_maxBeginTime = 0.0f;

		m_minEndTime = 1.0f;
		m_maxEndTime = 1.0f;

		//
		// 2. 播放参数
		//
		m_bEnable = TRUE;
		m_bActive = FALSE;

		m_bPlaying = FALSE;
		m_bPause = FALSE;

		m_beginTime = 0.0f;
		m_endTime = 1.0f;
	}

	//
	// 释放
	//
	VOID CAffectorBase::Free(VOID)
	{

	}

	//
	// 加载XML
	//
	VOID CAffectorBase::LoadXML(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);

		LoadBeginTime(pAffectorNode);
		LoadEndTime(pAffectorNode);
		LoadStepTime(pAffectorNode);
	}

	//
	// 保存XML
	//
	VOID CAffectorBase::SaveXML(TiXmlNode *pAffectorNode) const
	{
		ASSERT(pAffectorNode);
		static const CAffectorBase defaultAffector(NULL);

		SaveBeginTime(pAffectorNode, &defaultAffector);
		SaveEndTime(pAffectorNode, &defaultAffector);
		SaveStepTime(pAffectorNode, &defaultAffector);
	}
	
	//
	// 加载开始时间
	//
	VOID CAffectorBase::LoadBeginTime(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);

		if (TiXmlNode *pBeginTimeNode = pAffectorNode->FirstChild("BeginTime")) {
			SetBeginTime(
				pBeginTimeNode->ToElement()->AttributeFloat("min_value"),
				pBeginTimeNode->ToElement()->AttributeFloat("max_value"));
		}
	}

	//
	// 加载结束时间
	//
	VOID CAffectorBase::LoadEndTime(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);

		if (TiXmlNode *pEndTimeNode = pAffectorNode->FirstChild("EndTime")) {
			SetEndTime(
				pEndTimeNode->ToElement()->AttributeFloat("min_value"),
				pEndTimeNode->ToElement()->AttributeFloat("max_value"));
		}
	}

	//
	// 加载步进时间
	//
	VOID CAffectorBase::LoadStepTime(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);


		if (TiXmlNode *pStepTimeNode = pAffectorNode->FirstChild("StepTime")) {
			SetStep(pStepTimeNode->ToElement()->AttributeFloat("value"));
		}
	}

	//
	// 保存开始时间
	//
	VOID CAffectorBase::SaveBeginTime(TiXmlNode *pAffectorNode, const CAffectorBase *pDefaultAffector) const
	{
		ASSERT(pAffectorNode);

		if (m_minBeginTime != pDefaultAffector->m_minBeginTime || m_maxBeginTime != pDefaultAffector->m_maxBeginTime) {
			TiXmlElement *pBeginTimeNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("BeginTime"); ASSERT(pBeginTimeNode);
			{
				pBeginTimeNode->SetAttributeFloat("min_value", m_minBeginTime);
				pBeginTimeNode->SetAttributeFloat("max_value", m_maxBeginTime);
			}
			pAffectorNode->LinkEndChild(pBeginTimeNode);
		}
	}

	//
	// 保存结束时间
	//
	VOID CAffectorBase::SaveEndTime(TiXmlNode *pAffectorNode, const CAffectorBase *pDefaultAffector) const
	{
		ASSERT(pAffectorNode);

		if (m_minEndTime != pDefaultAffector->m_minEndTime || m_maxEndTime != pDefaultAffector->m_maxEndTime) {
			TiXmlElement *pEndTimeNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("EndTime"); ASSERT(pEndTimeNode);
			{
				pEndTimeNode->SetAttributeFloat("min_value", m_minEndTime);
				pEndTimeNode->SetAttributeFloat("max_value", m_maxEndTime);
			}
			pAffectorNode->LinkEndChild(pEndTimeNode);
		}
	}

	//
	// 保存步进时间
	//
	VOID CAffectorBase::SaveStepTime(TiXmlNode *pAffectorNode, const CAffectorBase *pDefaultAffector) const
	{
		ASSERT(pAffectorNode);

		if (m_step != pDefaultAffector->m_step) {
			TiXmlElement *pStepTimeNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("StepTime"); ASSERT(pStepTimeNode);
			{
				pStepTimeNode->SetAttributeFloat("value", m_step);
			}
			pAffectorNode->LinkEndChild(pStepTimeNode);
		}
	}

	//
	// 设置启用状态
	//
	VOID CAffectorBase::SetEnable(BOOL bEnable)
	{
		m_bEnable = bEnable;
	}

	//
	// 获得启用状态
	//
	BOOL CAffectorBase::IsEnable(VOID) const
	{
		return m_bEnable;
	}

	//
	// 设置响应间隔
	//
	VOID CAffectorBase::SetStep(FLOAT step)
	{
		m_delta = m_step = step > 0.0f ? step : 0.0f;
	}

	//
	// 获得响应间隔
	//
	FLOAT CAffectorBase::GetStep(VOID) const
	{
		return m_step;
	}

	//
	// 设置开始时间
	//
	VOID CAffectorBase::SetBeginTime(FLOAT minTime, FLOAT maxTime)
	{
		m_minBeginTime = min(minTime, maxTime);
		m_maxBeginTime = max(minTime, maxTime);
	}

	//
	// 获得最小开始时间
	//
	FLOAT CAffectorBase::GetMinBeginTime(VOID) const
	{
		return m_minBeginTime;
	}

	//
	// 获得最大开始时间
	//
	FLOAT CAffectorBase::GetMaxBeginTime(VOID) const
	{
		return m_maxBeginTime;
	}

	//
	// 设置结束时间
	//
	VOID CAffectorBase::SetEndTime(FLOAT minTime, FLOAT maxTime)
	{
		m_minEndTime = min(minTime, maxTime);
		m_maxEndTime = max(minTime, maxTime);
	}

	//
	// 获得最小结束时间
	//
	FLOAT CAffectorBase::GetMinEndTime(VOID) const
	{
		return m_minEndTime;
	}

	//
	// 获得最大结束时间
	//
	FLOAT CAffectorBase::GetMaxEndTime(VOID) const
	{
		return m_maxEndTime;
	}

	//
	// 获得世界缩放
	//
	const VEC3* CAffectorBase::GetWorldScale(VOID)
	{
		return m_pParent->GetScale();
	}

	//
	// 获得世界位置
	//
	const VEC3* CAffectorBase::GetWorldPosition(VOID)
	{
		return m_pParent->GetPosition();
	}

	//
	// 获得世界朝向
	//
	const QUAT* CAffectorBase::GetWorldOrientation(VOID)
	{
		return m_pParent->GetOrientation();
	}

	//
	// 播放
	//
	VOID CAffectorBase::Play(VOID)
	{
		FLOAT time0 = m_minBeginTime + FRandomValue()*(m_maxBeginTime - m_minBeginTime);
		FLOAT time1 = m_minEndTime + FRandomValue()*(m_maxEndTime - m_minEndTime);

		m_beginTime = min(time0, time1);
		m_endTime = max(time0, time1);

		m_bPlaying = TRUE;
		m_bPause = FALSE;
	}

	//
	// 停止
	//
	VOID CAffectorBase::Stop(VOID)
	{
		m_bPlaying = FALSE;
	}

	//
	// 暂停
	//
	VOID CAffectorBase::Pause(VOID)
	{
		m_bPause = TRUE;
	}

	//
	// 继续
	//
	VOID CAffectorBase::Resume(VOID)
	{
		m_bPause = FALSE;
	}

	//
	// 播放状态
	//
	BOOL CAffectorBase::IsPlaying(VOID) const
	{
		return m_bPlaying && m_bPause == FALSE ? TRUE : FALSE;
	}

	//
	// 施加影响检查
	//
	BOOL CAffectorBase::IsCanAffect(const CParticle *pParticle)
	{
		return m_bActive && pParticle && pParticle->currTime > m_beginTime && pParticle->currTime < m_endTime ? TRUE : FALSE;
	}

	//
	// 更新
	//
	VOID CAffectorBase::Update(FLOAT deltaTime)
	{
		m_delta -= deltaTime;

		if (m_delta < EPSILON_E3) {
			m_delta = m_step;
			m_bActive = IsEnable() && IsPlaying() ? TRUE : FALSE;
		}
		else {
			m_bActive = FALSE;
		}
	}

}

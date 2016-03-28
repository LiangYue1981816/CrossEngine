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
	// �������ϵͳ
	//
	CParticleSystem* CAffectorBase::GetParticleSystem(VOID) const
	{
		return m_pParent;
	}

	//
	// ��ʼ��
	//
	VOID CAffectorBase::Init(VOID)
	{
		//
		// 1. Ӱ��������
		//
		m_step = 0.0f;
		m_delta = 0.0f;

		m_minBeginTime = 0.0f;
		m_maxBeginTime = 0.0f;

		m_minEndTime = 1.0f;
		m_maxEndTime = 1.0f;

		//
		// 2. ���Ų���
		//
		m_bEnable = TRUE;
		m_bActive = FALSE;

		m_bPlaying = FALSE;
		m_bPause = FALSE;

		m_beginTime = 0.0f;
		m_endTime = 1.0f;
	}

	//
	// �ͷ�
	//
	VOID CAffectorBase::Free(VOID)
	{

	}

	//
	// ����XML
	//
	VOID CAffectorBase::LoadXML(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);

		LoadBeginTime(pAffectorNode);
		LoadEndTime(pAffectorNode);
		LoadStepTime(pAffectorNode);
	}

	//
	// ����XML
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
	// ���ؿ�ʼʱ��
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
	// ���ؽ���ʱ��
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
	// ���ز���ʱ��
	//
	VOID CAffectorBase::LoadStepTime(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);


		if (TiXmlNode *pStepTimeNode = pAffectorNode->FirstChild("StepTime")) {
			SetStep(pStepTimeNode->ToElement()->AttributeFloat("value"));
		}
	}

	//
	// ���濪ʼʱ��
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
	// �������ʱ��
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
	// ���沽��ʱ��
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
	// ��������״̬
	//
	VOID CAffectorBase::SetEnable(BOOL bEnable)
	{
		m_bEnable = bEnable;
	}

	//
	// �������״̬
	//
	BOOL CAffectorBase::IsEnable(VOID) const
	{
		return m_bEnable;
	}

	//
	// ������Ӧ���
	//
	VOID CAffectorBase::SetStep(FLOAT step)
	{
		m_delta = m_step = step > 0.0f ? step : 0.0f;
	}

	//
	// �����Ӧ���
	//
	FLOAT CAffectorBase::GetStep(VOID) const
	{
		return m_step;
	}

	//
	// ���ÿ�ʼʱ��
	//
	VOID CAffectorBase::SetBeginTime(FLOAT minTime, FLOAT maxTime)
	{
		m_minBeginTime = min(minTime, maxTime);
		m_maxBeginTime = max(minTime, maxTime);
	}

	//
	// �����С��ʼʱ��
	//
	FLOAT CAffectorBase::GetMinBeginTime(VOID) const
	{
		return m_minBeginTime;
	}

	//
	// ������ʼʱ��
	//
	FLOAT CAffectorBase::GetMaxBeginTime(VOID) const
	{
		return m_maxBeginTime;
	}

	//
	// ���ý���ʱ��
	//
	VOID CAffectorBase::SetEndTime(FLOAT minTime, FLOAT maxTime)
	{
		m_minEndTime = min(minTime, maxTime);
		m_maxEndTime = max(minTime, maxTime);
	}

	//
	// �����С����ʱ��
	//
	FLOAT CAffectorBase::GetMinEndTime(VOID) const
	{
		return m_minEndTime;
	}

	//
	// ���������ʱ��
	//
	FLOAT CAffectorBase::GetMaxEndTime(VOID) const
	{
		return m_maxEndTime;
	}

	//
	// �����������
	//
	const VEC3* CAffectorBase::GetWorldScale(VOID)
	{
		return m_pParent->GetScale();
	}

	//
	// �������λ��
	//
	const VEC3* CAffectorBase::GetWorldPosition(VOID)
	{
		return m_pParent->GetPosition();
	}

	//
	// ������糯��
	//
	const QUAT* CAffectorBase::GetWorldOrientation(VOID)
	{
		return m_pParent->GetOrientation();
	}

	//
	// ����
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
	// ֹͣ
	//
	VOID CAffectorBase::Stop(VOID)
	{
		m_bPlaying = FALSE;
	}

	//
	// ��ͣ
	//
	VOID CAffectorBase::Pause(VOID)
	{
		m_bPause = TRUE;
	}

	//
	// ����
	//
	VOID CAffectorBase::Resume(VOID)
	{
		m_bPause = FALSE;
	}

	//
	// ����״̬
	//
	BOOL CAffectorBase::IsPlaying(VOID) const
	{
		return m_bPlaying && m_bPause == FALSE ? TRUE : FALSE;
	}

	//
	// ʩ��Ӱ����
	//
	BOOL CAffectorBase::IsCanAffect(const CParticle *pParticle)
	{
		return m_bActive && pParticle && pParticle->currTime > m_beginTime && pParticle->currTime < m_endTime ? TRUE : FALSE;
	}

	//
	// ����
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

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

	CEmitterBase::CEmitterBase(CParticleSystem *pParent)
		: m_pParent(pParent)
	{
		Init();
	}

	CEmitterBase::~CEmitterBase(VOID)
	{
		Free();
	}

	//
	// �������ϵͳ
	//
	CParticleSystem* CEmitterBase::GetParticleSystem(VOID) const
	{
		return m_pParent;
	}

	//
	// ��ʼ��
	//
	VOID CEmitterBase::Init(VOID)
	{
		//
		// 1. ����������
		//
		m_emitRate = 10;

		m_minBeginTime = 0.0f;
		m_maxBeginTime = 0.0f;

		m_minDurationTime = 0.0f;
		m_maxDurationTime = 0.0f;

		m_minRadianX = 0.0f;
		m_maxRadianX = 0.0f;

		m_minRadianY = 0.0f;
		m_maxRadianY = 0.0f;

		m_minRadianZ = 0.0f;
		m_maxRadianZ = 0.0f;

		Vec3Set(&m_position, 0.0f, 0.0f, 0.0f);
		QuatSet(&m_orientation, -0.70710677f, 0.0f, 0.0f, 0.70710677f);

		//
		// 2. ���Ӳ���
		//
		m_bKeepLocal = FALSE;

		m_minLiveTime = 1.0f;
		m_maxLiveTime = 1.0f;

		m_minVelocity = 1.0f;
		m_maxVelocity = 1.0f;

		m_minSelfRadian = 0.0f;
		m_maxSelfRadian = 0.0f;

		Vec3Set(&m_minScale, 1.0f, 1.0f, 1.0f);
		Vec3Set(&m_maxScale, 1.0f, 1.0f, 1.0f);

		Vec4Set(&m_minColor, 1.0f, 1.0f, 1.0f, 1.0f);
		Vec4Set(&m_maxColor, 1.0f, 1.0f, 1.0f, 1.0f);

		Vec2Set(&m_minUVOffset, 0.0f, 0.0f);
		Vec2Set(&m_maxUVOffset, 0.0f, 0.0f);

		//
		// 3. ���Ų���
		//
		m_bEnable = TRUE;
		m_bActive = FALSE;

		m_bPlaying = FALSE;
		m_bPause = FALSE;

		m_emitMount = 0.0f;
		m_emitDeltaTime = 0.0f;

		m_currTime = 0.0f;
		m_beginTime = 0.0f;
		m_durationTime = 0.0f;

		Vec3Set(&m_worldPosition, 0.0f, 0.0f, 0.0f);
		QuatSet(&m_worldOrientation, 0.0f, 0.0f, 0.0f, 1.0f);
	}

	//
	// �ͷ�
	//
	VOID CEmitterBase::Free(VOID)
	{

	}

	//
	// ����XML
	//
	VOID CEmitterBase::LoadXML(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		LoadEmitRate(pEmitterNode);
		LoadBeginTime(pEmitterNode);
		LoadDurationTime(pEmitterNode);
		LoadRadianX(pEmitterNode);
		LoadRadianY(pEmitterNode);
		LoadRadianZ(pEmitterNode);
		LoadKeepLocal(pEmitterNode);
		LoadLiveTime(pEmitterNode);
		LoadVelocity(pEmitterNode);
		LoadSelfRadian(pEmitterNode);
		LoadScale(pEmitterNode);
		LoadColor(pEmitterNode);
		LoadUVOffset(pEmitterNode);
	}

	//
	// ����XML
	//
	VOID CEmitterBase::SaveXML(TiXmlNode *pEmitterNode) const
	{
		ASSERT(pEmitterNode);
		static const CEmitterBase defaultEmitter(NULL);

		SaveEmitRate(pEmitterNode, &defaultEmitter);
		SaveBeginTime(pEmitterNode, &defaultEmitter);
		SaveDurationTime(pEmitterNode, &defaultEmitter);
		SaveRadianX(pEmitterNode, &defaultEmitter);
		SaveRadianY(pEmitterNode, &defaultEmitter);
		SaveRadianZ(pEmitterNode, &defaultEmitter);
		SaveKeepLocal(pEmitterNode, &defaultEmitter);
		SaveLiveTime(pEmitterNode, &defaultEmitter);
		SaveVelocity(pEmitterNode, &defaultEmitter);
		SaveSelfRadian(pEmitterNode, &defaultEmitter);
		SaveScale(pEmitterNode, &defaultEmitter);
		SaveColor(pEmitterNode, &defaultEmitter);
		SaveUVOffset(pEmitterNode, &defaultEmitter);
	}

	//
	// ���ط�����
	//
	VOID CEmitterBase::LoadEmitRate(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pEmitRateNode = pEmitterNode->FirstChild("EmitRate")) {
			SetEmitRate(pEmitterNode->ToElement()->AttributeInt("value"));
		}
	}

	//
	// ���ؿ�ʼʱ��
	//
	VOID CEmitterBase::LoadBeginTime(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pBeginTimeNode = pEmitterNode->FirstChild("BeginTime")) {
			SetBeginTime(
				pBeginTimeNode->ToElement()->AttributeFloat("min_value"),
				pBeginTimeNode->ToElement()->AttributeFloat("max_value"));
		}
	}

	//
	// ���س���ʱ��
	//
	VOID CEmitterBase::LoadDurationTime(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pDurationTimeNode = pEmitterNode->FirstChild("DurationTime")) {
			SetDurationTime(
				pDurationTimeNode->ToElement()->AttributeFloat("min_value"),
				pDurationTimeNode->ToElement()->AttributeFloat("max_value"));
		}
	}

	//
	// ������X�ᷢ��ƫ��
	//
	VOID CEmitterBase::LoadRadianX(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pRadianXNode = pEmitterNode->FirstChild("RadianX")) {
			SetRadianX(
				pRadianXNode->ToElement()->AttributeFloat("min_value"),
				pRadianXNode->ToElement()->AttributeFloat("max_value"));
		}
	}

	//
	// ������Y�ᷢ��ƫ��
	//
	VOID CEmitterBase::LoadRadianY(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pRadianYNode = pEmitterNode->FirstChild("RadianY")) {
			SetRadianY(
				pRadianYNode->ToElement()->AttributeFloat("min_value"),
				pRadianYNode->ToElement()->AttributeFloat("max_value"));
		}
	}

	//
	// ������Z�ᷢ��ƫ��
	//
	VOID CEmitterBase::LoadRadianZ(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pRadianZNode = pEmitterNode->FirstChild("RadianZ")) {
			SetRadianZ(
				pRadianZNode->ToElement()->AttributeFloat("min_value"),
				pRadianZNode->ToElement()->AttributeFloat("max_value"));
		}
	}

	//
	// ���ر��ֱ���λ��״̬
	//
	VOID CEmitterBase::LoadKeepLocal(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pKeepLocalNode = pEmitterNode->FirstChild("KeepLocal")) {
			SetKeepLocal(cglStringToAttrib(pKeepLocalNode->ToElement()->AttributeString("enable")));
		}
	}

	//
	// ��������ʱ��
	//
	VOID CEmitterBase::LoadLiveTime(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pLiveTimeNode = pEmitterNode->FirstChild("LiveTime")) {
			SetLiveTime(
				pLiveTimeNode->ToElement()->AttributeFloat("min_value"),
				pLiveTimeNode->ToElement()->AttributeFloat("max_value"));
		}
	}

	//
	// �����ٶ�
	//
	VOID CEmitterBase::LoadVelocity(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pVelocityNode = pEmitterNode->FirstChild("Velocity")) {
			SetVelocity(
				pVelocityNode->ToElement()->AttributeFloat("min_value"),
				pVelocityNode->ToElement()->AttributeFloat("max_value"));
		}
	}

	//
	// ������ת��
	//
	VOID CEmitterBase::LoadSelfRadian(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pSelfRadianNode = pEmitterNode->FirstChild("SelfRadian")) {
			SetSelfRadian(
				pSelfRadianNode->ToElement()->AttributeFloat("min_value"),
				pSelfRadianNode->ToElement()->AttributeFloat("max_value"));
		}
	}

	//
	// ��������
	//
	VOID CEmitterBase::LoadScale(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pScaleNode = pEmitterNode->FirstChild("Scale")) {
			VEC3 minScale;
			VEC3 maxScale;
			sscanf(pScaleNode->ToElement()->AttributeString("min_value"), "%f %f %f", &minScale[0], &minScale[1], &minScale[2]);
			sscanf(pScaleNode->ToElement()->AttributeString("max_value"), "%f %f %f", &maxScale[0], &maxScale[1], &maxScale[2]);
			SetScale(&minScale, &maxScale);
		}
	}

	//
	// ������ɫ
	//
	VOID CEmitterBase::LoadColor(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pColorNode = pEmitterNode->FirstChild("Color")) {
			VEC4 minColor;
			VEC4 maxColor;
			sscanf(pColorNode->ToElement()->AttributeString("min_value"), "%f %f %f %f", &minColor[0], &minColor[1], &minColor[2], &minColor[3]);
			sscanf(pColorNode->ToElement()->AttributeString("max_value"), "%f %f %f %f", &maxColor[0], &maxColor[1], &maxColor[2], &maxColor[3]);
			SetColor(&minColor, &maxColor);
		}
	}

	//
	// ��������ƫ��
	//
	VOID CEmitterBase::LoadUVOffset(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pUVOffsetNode = pEmitterNode->FirstChild("UVOffset")) {
			VEC2 minUVOffset;
			VEC2 maxUVOffset;
			sscanf(pUVOffsetNode->ToElement()->AttributeString("min_value"), "%f %f", &minUVOffset[0], &minUVOffset[1]);
			sscanf(pUVOffsetNode->ToElement()->AttributeString("max_value"), "%f %f", &maxUVOffset[0], &maxUVOffset[1]);
			SetUVOffset(&minUVOffset, &maxUVOffset);
		}
	}

	//
	// ���淢����
	//
	VOID CEmitterBase::SaveEmitRate(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_emitRate != pDefaultEmitter->m_emitRate) {
			TiXmlElement *pEmitRateNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("EmitRate"); ASSERT(pEmitRateNode);
			{
				pEmitRateNode->SetAttributeInt("value", m_emitRate);
			}
			pEmitterNode->LinkEndChild(pEmitRateNode);
		}
	}

	//
	// ���濪ʼʱ��
	//
	VOID CEmitterBase::SaveBeginTime(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_minBeginTime != pDefaultEmitter->m_minBeginTime || m_maxBeginTime != pDefaultEmitter->m_maxBeginTime) {
			TiXmlElement *pBeginTimeNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("BeginTime"); ASSERT(pBeginTimeNode);
			{
				pBeginTimeNode->SetAttributeFloat("min_value", m_minBeginTime);
				pBeginTimeNode->SetAttributeFloat("max_value", m_maxBeginTime);
			}
			pEmitterNode->LinkEndChild(pBeginTimeNode);
		}
	}

	//
	// �������ʱ��
	//
	VOID CEmitterBase::SaveDurationTime(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_minDurationTime != pDefaultEmitter->m_minDurationTime || m_maxDurationTime != pDefaultEmitter->m_maxDurationTime) {
			TiXmlElement *pDurationTimeNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("DurationTime"); ASSERT(pDurationTimeNode);
			{
				pDurationTimeNode->SetAttributeFloat("min_value", m_minDurationTime);
				pDurationTimeNode->SetAttributeFloat("max_value", m_maxDurationTime);
			}
			pEmitterNode->LinkEndChild(pDurationTimeNode);
		}
	}

	//
	// ������X�ᷢ��ƫ��
	//
	VOID CEmitterBase::SaveRadianX(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_minRadianX != pDefaultEmitter->m_minRadianX || m_maxRadianX != pDefaultEmitter->m_maxRadianX) {
			TiXmlElement *pRadianXNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("RadianX"); ASSERT(pRadianXNode);
			{
				pRadianXNode->SetAttributeFloat("min_value", m_minRadianX);
				pRadianXNode->SetAttributeFloat("max_value", m_maxRadianX);
			}
			pEmitterNode->LinkEndChild(pRadianXNode);
		}
	}

	//
	// ������Y�ᷢ��ƫ��
	//
	VOID CEmitterBase::SaveRadianY(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_minRadianY != pDefaultEmitter->m_minRadianY || m_maxRadianY != pDefaultEmitter->m_maxRadianY) {
			TiXmlElement *pRadianYNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("RadianY"); ASSERT(pRadianYNode);
			{
				pRadianYNode->SetAttributeFloat("min_value", m_minRadianY);
				pRadianYNode->SetAttributeFloat("max_value", m_maxRadianY);
			}
			pEmitterNode->LinkEndChild(pRadianYNode);
		}
	}

	//
	// ������Z�ᷢ��ƫ��
	//
	VOID CEmitterBase::SaveRadianZ(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_minRadianZ != pDefaultEmitter->m_minRadianZ || m_maxRadianZ != pDefaultEmitter->m_maxRadianZ) {
			TiXmlElement *pRadianZNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("RadianZ"); ASSERT(pRadianZNode);
			{
				pRadianZNode->SetAttributeFloat("min_value", m_minRadianZ);
				pRadianZNode->SetAttributeFloat("max_value", m_maxRadianZ);
			}
			pEmitterNode->LinkEndChild(pRadianZNode);
		}
	}

	//
	// ���汣�ֱ���λ��״̬
	//
	VOID CEmitterBase::SaveKeepLocal(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_bKeepLocal != pDefaultEmitter->m_bKeepLocal) {
			TiXmlElement *pKeepLocalNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("KeepLocal"); ASSERT(pKeepLocalNode);
			{
				pKeepLocalNode->SetAttributeString("enable", cglAttribToString(m_bKeepLocal));
			}
			pEmitterNode->LinkEndChild(pKeepLocalNode);
		}
	}

	//
	// ��������ʱ��
	//
	VOID CEmitterBase::SaveLiveTime(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_minLiveTime != pDefaultEmitter->m_minLiveTime || m_maxLiveTime != pDefaultEmitter->m_maxLiveTime) {
			TiXmlElement *pLiveTimeNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("LiveTime"); ASSERT(pLiveTimeNode);
			{
				pLiveTimeNode->SetAttributeFloat("min_value", m_minLiveTime);
				pLiveTimeNode->SetAttributeFloat("max_value", m_maxLiveTime);
			}
			pEmitterNode->LinkEndChild(pLiveTimeNode);
		}
	}

	//
	// �����ٶ�
	//
	VOID CEmitterBase::SaveVelocity(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_minVelocity != pDefaultEmitter->m_minVelocity || m_maxVelocity != pDefaultEmitter->m_maxVelocity) {
			TiXmlElement *pVelocityNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Velocity"); ASSERT(pVelocityNode);
			{
				pVelocityNode->SetAttributeFloat("min_value", m_minVelocity);
				pVelocityNode->SetAttributeFloat("max_value", m_maxVelocity);
			}
			pEmitterNode->LinkEndChild(pVelocityNode);
		}
	}

	//
	// ������ת��
	//
	VOID CEmitterBase::SaveSelfRadian(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_minSelfRadian != pDefaultEmitter->m_minSelfRadian || m_maxSelfRadian != pDefaultEmitter->m_maxSelfRadian) {
			TiXmlElement *pSelfRadianNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("SelfRadian"); ASSERT(pSelfRadianNode);
			{
				pSelfRadianNode->SetAttributeFloat("min_value", m_minSelfRadian);
				pSelfRadianNode->SetAttributeFloat("max_value", m_maxSelfRadian);
			}
			pEmitterNode->LinkEndChild(pSelfRadianNode);
		}
	}

	//
	// ��������
	//
	VOID CEmitterBase::SaveScale(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_minScale[0] != pDefaultEmitter->m_minScale[0] || m_minScale[1] != pDefaultEmitter->m_minScale[1] || m_minScale[2] != pDefaultEmitter->m_minScale[2] ||
			m_maxScale[0] != pDefaultEmitter->m_maxScale[0] || m_maxScale[1] != pDefaultEmitter->m_maxScale[1] || m_maxScale[2] != pDefaultEmitter->m_maxScale[2]) {
			TiXmlElement *pScaleNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Scale"); ASSERT(pScaleNode);
			{
				pScaleNode->SetAttributeString("min_value", "%f %f %f", m_minScale[0], m_minScale[1], m_minScale[2]);
				pScaleNode->SetAttributeString("max_value", "%f %f %f", m_maxScale[0], m_maxScale[1], m_maxScale[2]);
			}
			pEmitterNode->LinkEndChild(pScaleNode);
		}
	}

	//
	// ������ɫ
	//
	VOID CEmitterBase::SaveColor(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_minColor[0] != pDefaultEmitter->m_minColor[0] || m_minColor[1] != pDefaultEmitter->m_minColor[1] || m_minColor[2] != pDefaultEmitter->m_minColor[2] || m_minColor[3] != pDefaultEmitter->m_minColor[3] ||
			m_maxColor[0] != pDefaultEmitter->m_maxColor[0] || m_maxColor[1] != pDefaultEmitter->m_maxColor[1] || m_maxScale[2] != pDefaultEmitter->m_maxColor[2] || m_maxColor[3] != pDefaultEmitter->m_maxColor[3]) {
			TiXmlElement *pColorNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Color"); ASSERT(pColorNode);
			{
				pColorNode->SetAttributeString("min_value", "%f %f %f %f", m_minColor[0], m_minColor[1], m_minColor[2], m_minColor[3]);
				pColorNode->SetAttributeString("max_value", "%f %f %f %f", m_maxColor[0], m_maxColor[1], m_maxColor[2], m_maxColor[3]);
			}
			pEmitterNode->LinkEndChild(pColorNode);
		}
	}

	//
	// ��������ƫ��
	//
	VOID CEmitterBase::SaveUVOffset(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const
	{
		ASSERT(pEmitterNode);

		if (m_minUVOffset[0] != pDefaultEmitter->m_minUVOffset[0] || m_minUVOffset[1] != pDefaultEmitter->m_minUVOffset[1] ||
			m_maxUVOffset[0] != pDefaultEmitter->m_maxUVOffset[0] || m_maxUVOffset[1] != pDefaultEmitter->m_maxUVOffset[1]) {
			TiXmlElement *pUVOffsetNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("UVOffset"); ASSERT(pUVOffsetNode);
			{
				pUVOffsetNode->SetAttributeString("min_value", "%f %f", m_minUVOffset[0], m_minUVOffset[1]);
				pUVOffsetNode->SetAttributeString("max_value", "%f %f", m_maxUVOffset[0], m_maxUVOffset[1]);
			}
			pEmitterNode->LinkEndChild(pUVOffsetNode);
		}
	}

	//
	// ��������״̬
	//
	VOID CEmitterBase::SetEnable(BOOL bEnable)
	{
		m_bEnable = bEnable;
	}

	//
	// �������״̬
	//
	BOOL CEmitterBase::IsEnable(VOID) const
	{
		return m_bEnable;
	}

	//
	// ��û״̬
	//
	BOOL CEmitterBase::IsActive(VOID) const
	{
		return m_bActive;
	}

	//
	// ���÷�����
	//
	VOID CEmitterBase::SetEmitRate(INT rate)
	{
		m_emitRate = rate > 0 ? rate : 0;
	}

	//
	// ��÷�����
	//
	INT CEmitterBase::GetEmitRate(VOID) const
	{
		return m_emitRate;
	}

	//
	// ���ÿ�ʼʱ��
	//
	VOID CEmitterBase::SetBeginTime(FLOAT minTime, FLOAT maxTime)
	{
		m_minBeginTime = min(minTime, maxTime);
		m_maxBeginTime = max(minTime, maxTime);
	}

	//
	// �����С��ʼʱ��
	//
	FLOAT CEmitterBase::GetMinBeginTime(VOID) const
	{
		return m_minBeginTime;
	}

	//
	// ������ʼʱ��
	//
	FLOAT CEmitterBase::GetMaxBeginTime(VOID) const
	{
		return m_maxBeginTime;
	}

	//
	// ���ó���ʱ��
	//
	VOID CEmitterBase::SetDurationTime(FLOAT minTime, FLOAT maxTime)
	{
		m_minDurationTime = min(minTime, maxTime);
		m_maxDurationTime = max(minTime, maxTime);
	}

	//
	// �����С����ʱ��
	//
	FLOAT CEmitterBase::GetMinDurationTime(VOID) const
	{
		return m_minDurationTime;
	}

	//
	// ���������ʱ��
	//
	FLOAT CEmitterBase::GetMaxDurationTime(VOID) const
	{
		return m_maxDurationTime;
	}

	//
	// ������X�ᷢ��ƫ��
	//
	VOID CEmitterBase::SetRadianX(FLOAT minRadian, FLOAT maxRadian)
	{
		m_minRadianX = min(minRadian, maxRadian);
		m_maxRadianX = max(minRadian, maxRadian);
	}

	//
	// �����С��X�ᷢ��ƫ��
	//
	FLOAT CEmitterBase::GetMinRadianX(VOID) const
	{
		return m_minRadianX;
	}

	//
	// ��������X�ᷢ��ƫ��
	//
	FLOAT CEmitterBase::GetMaxRadianX(VOID) const
	{
		return m_maxRadianX;
	}

	//
	// ������Y�ᷢ��ƫ��
	//
	VOID CEmitterBase::SetRadianY(FLOAT minRadian, FLOAT maxRadian)
	{
		m_minRadianY = min(minRadian, maxRadian);
		m_maxRadianY = max(minRadian, maxRadian);
	}

	//
	// �����С��Y�ᷢ��ƫ��
	//
	FLOAT CEmitterBase::GetMinRadianY(VOID) const
	{
		return m_minRadianY;
	}

	//
	// ��������Y�ᷢ��ƫ��
	//
	FLOAT CEmitterBase::GetMaxRadianY(VOID) const
	{
		return m_maxRadianY;
	}

	//
	// ������Z�ᷢ��ƫ��
	//
	VOID CEmitterBase::SetRadianZ(FLOAT minRadian, FLOAT maxRadian)
	{
		m_minRadianZ = min(minRadian, maxRadian);
		m_maxRadianZ = max(minRadian, maxRadian);
	}

	//
	// �����С��Z�ᷢ��ƫ��
	//
	FLOAT CEmitterBase::GetMinRadianZ(VOID) const
	{
		return m_minRadianZ;
	}

	//
	// ��������Z�ᷢ��ƫ��
	//
	FLOAT CEmitterBase::GetMaxRadianZ(VOID) const
	{
		return m_maxRadianZ;
	}

	//
	// ���ñ��ֱ���λ��״̬
	//
	VOID CEmitterBase::SetKeepLocal(BOOL bKeepLocal)
	{
		m_bKeepLocal = bKeepLocal;
	}

	//
	// ��ñ��ֱ���λ��״̬
	//
	BOOL CEmitterBase::IsKeepLocal(VOID) const
	{
		return m_bKeepLocal;
	}

	//
	// ��������ʱ��
	//
	VOID CEmitterBase::SetLiveTime(FLOAT minTime, FLOAT maxTime)
	{
		m_minLiveTime = min(minTime, maxTime);
		m_maxLiveTime = max(minTime, maxTime);
	}

	//
	// �����С����ʱ��
	//
	FLOAT CEmitterBase::GetMinLiveTime(VOID) const
	{
		return m_minLiveTime;
	}

	//
	// ����������ʱ��
	//
	FLOAT CEmitterBase::GetMaxLiveTime(VOID) const
	{
		return m_maxLiveTime;
	}

	//
	// �����ٶ�
	//
	VOID CEmitterBase::SetVelocity(FLOAT minVelocity, FLOAT maxVelocity)
	{
		m_minVelocity = min(minVelocity, maxVelocity);
		m_maxVelocity = max(minVelocity, maxVelocity);
	}

	//
	// �����С�ٶ�
	//
	FLOAT CEmitterBase::GetMinVelocity(VOID) const
	{
		return m_minVelocity;
	}

	//
	// ��������ٶ�
	//
	FLOAT CEmitterBase::GetMaxVelocity(VOID) const
	{
		return m_maxVelocity;
	}

	//
	// ������תƫ��
	//
	VOID CEmitterBase::SetSelfRadian(FLOAT minRadian, FLOAT maxRadian)
	{
		m_minSelfRadian = min(minRadian, maxRadian);
		m_maxSelfRadian = max(minRadian, maxRadian);
	}

	//
	// �����С��תƫ��
	//
	FLOAT CEmitterBase::GetMinSelfRadian(VOID) const
	{
		return m_minSelfRadian;
	}

	//
	// ��������תƫ��
	//
	FLOAT CEmitterBase::GetMaxSelfRadian(VOID) const
	{
		return m_maxSelfRadian;
	}

	//
	// ��������
	//
	VOID CEmitterBase::SetScale(FLOAT minScaleX, FLOAT minScaleY, FLOAT minScaleZ, FLOAT maxScaleX, FLOAT maxScaleY, FLOAT maxScaleZ)
	{
		m_minScale[0] = min(minScaleX, maxScaleX);
		m_minScale[1] = min(minScaleY, maxScaleY);
		m_minScale[2] = min(minScaleZ, maxScaleZ);

		m_maxScale[0] = max(minScaleX, maxScaleX);
		m_maxScale[1] = max(minScaleY, maxScaleY);
		m_maxScale[2] = max(minScaleZ, maxScaleZ);
	}

	//
	// ��������
	//
	VOID CEmitterBase::SetScale(const VEC3* minScale, const VEC3 *maxScale)
	{
		ASSERT(minScale);
		ASSERT(maxScale);
		SetScale((*minScale)[0], (*minScale)[1], (*minScale)[2], (*maxScale)[0], (*maxScale)[1], (*maxScale)[2]);
	}

	//
	// �����С����
	//
	const VEC3* CEmitterBase::GetMinScale(VOID) const
	{
		return &m_minScale;
	}

	//
	// ����������
	//
	const VEC3* CEmitterBase::GetMaxScale(VOID) const
	{
		return &m_maxScale;
	}

	//
	// ������ɫ
	//
	VOID CEmitterBase::SetColor(FLOAT minColorR, FLOAT minColorG, FLOAT minColorB, FLOAT minColorA, FLOAT maxColorR, FLOAT maxColorG, FLOAT maxColorB, FLOAT maxColorA)
	{
		m_minColor[0] = min(minColorR, maxColorR);
		m_minColor[1] = min(minColorG, maxColorG);
		m_minColor[2] = min(minColorB, maxColorB);
		m_minColor[3] = min(minColorA, maxColorA);

		m_maxColor[0] = max(minColorR, maxColorR);
		m_maxColor[1] = max(minColorG, maxColorG);
		m_maxColor[2] = max(minColorB, maxColorB);
		m_maxColor[3] = max(minColorA, maxColorA);
	}

	//
	// ������ɫ
	//
	VOID CEmitterBase::SetColor(const VEC4* minColor, const VEC4 *maxColor)
	{
		ASSERT(minColor);
		ASSERT(maxColor);
		SetColor((*minColor)[0], (*minColor)[1], (*minColor)[2], (*minColor)[3], (*maxColor)[0], (*maxColor)[1], (*maxColor)[2], (*maxColor)[3]);
	}

	//
	// �����С��ɫ
	//
	const VEC4* CEmitterBase::GetMinColor(VOID) const
	{
		return &m_minColor;
	}

	//
	// ��������ɫ
	//
	const VEC4* CEmitterBase::GetMaxColor(VOID) const
	{
		return &m_maxColor;
	}

	//
	// ��������ƫ��
	//
	VOID CEmitterBase::SetUVOffset(FLOAT minUVOffsetX, FLOAT minUVOffsetY, FLOAT maxUVOffsetX, FLOAT maxUVOffsetY)
	{
		m_minUVOffset[0] = min(minUVOffsetX, maxUVOffsetX);
		m_minUVOffset[1] = min(minUVOffsetY, maxUVOffsetY);

		m_maxUVOffset[0] = max(minUVOffsetX, maxUVOffsetX);
		m_maxUVOffset[1] = max(minUVOffsetY, maxUVOffsetY);
	}

	//
	// ��������ƫ��
	//
	VOID CEmitterBase::SetUVOffset(const VEC2* minUVOffset, const VEC2 *maxUVOffset)
	{
		ASSERT(minUVOffset);
		ASSERT(maxUVOffset);
		SetUVOffset((*minUVOffset)[0], (*minUVOffset)[1], (*maxUVOffset)[0], (*maxUVOffset)[1]);
	}

	//
	// �����С����ƫ��
	//
	const VEC2* CEmitterBase::GetMinUVOffset(VOID) const
	{
		return &m_minUVOffset;
	}

	//
	// ����������ƫ��
	//
	const VEC2* CEmitterBase::GetMaxUVOffset(VOID) const
	{
		return &m_maxUVOffset;
	}

	//
	// ����λ��
	//
	VOID CEmitterBase::SetPosition(FLOAT x, FLOAT y, FLOAT z)
	{
		Vec3Set(&m_position, x, y, z);
	}

	//
	// ����λ��
	//
	VOID CEmitterBase::SetPosition(const VEC3 *position)
	{
		ASSERT(position);
		Vec3Copy(&m_position, position);
	}

	//
	// ���λ��
	//
	const VEC3* CEmitterBase::GetPosition(VOID) const
	{
		return &m_position;
	}

	//
	// ���ó���
	//
	VOID CEmitterBase::SetDirection(FLOAT x, FLOAT y, FLOAT z)
	{
		VEC3 direction;
		Vec3Set(&direction, x, y, z);
		SetDirection(&direction);
	}

	//
	// ���ó���
	//
	VOID CEmitterBase::SetDirection(const VEC3 *direction)
	{
		ASSERT(direction);

		FLOAT dot;
		FLOAT rad;

		VEC3 axis;
		VEC3 inDirection;
		VEC3 localDirection;

		Vec3Normalize(&inDirection, direction);
		Vec3Set(&localDirection, 0.0f, 0.0f, 1.0f);

		dot = Vec3Dot(&localDirection, &inDirection);
		rad = FastACos(dot);

		if (FastFAbs(1.0f - FastFAbs(dot)) < EPSILON_E3) {
			Vec3Set(&axis, 0.0f, 1.0f, 0.0f);
		}
		else {
			Vec3Cross(&axis, &localDirection, &inDirection);
		}

		QuatDefRotateAxisAngle(&m_orientation, &axis, rad);
	}

	//
	// ���ó���
	//
	VOID CEmitterBase::SetOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w)
	{
		QuatSet(&m_orientation, x, y, z, w);
		QuatNormalize(&m_orientation);
	}

	//
	// ���ó���
	//
	VOID CEmitterBase::SetOrientation(const QUAT *orientation)
	{
		ASSERT(orientation);
		QuatCopy(&m_orientation, orientation);
		QuatNormalize(&m_orientation);
	}

	//
	// ��ó���
	//
	const QUAT* CEmitterBase::GetOrientation(VOID) const
	{
		return &m_orientation;
	}

	//
	// �����������
	//
	const VEC3* CEmitterBase::GetWorldScale(VOID)
	{
		return m_pParent->GetScale();
	}

	//
	// �������λ��
	//
	const VEC3* CEmitterBase::GetWorldPosition(VOID)
	{
		VEC3 scalePosition;
		VEC3 scaleOrientationPosition;

		Vec3Mul(&scalePosition, &m_position, m_pParent->GetScale());
		Vec3MulQuat(&scaleOrientationPosition, &scalePosition, m_pParent->GetOrientation());
		Vec3Add(&m_worldPosition, &scaleOrientationPosition, m_pParent->GetPosition());

		return &m_worldPosition;
	}

	//
	// ������糯��
	//
	const QUAT* CEmitterBase::GetWorldOrientation(VOID)
	{
		QuatMul(&m_worldOrientation, &m_orientation, m_pParent->GetOrientation());
		return &m_worldOrientation;
	}

	//
	// ��÷�����
	//
	INT CEmitterBase::GetEmitMount(VOID) const
	{
		return (INT)m_emitMount;
	}

	//
	// ����
	//
	VOID CEmitterBase::Play(VOID)
	{
		m_bPlaying = TRUE;
		m_bPause = FALSE;

		m_emitMount = 0.0f;
		m_emitDeltaTime = 0.0f;

		m_currTime = 0.0f;
		m_beginTime = m_minBeginTime + FRandomValue()*(m_maxBeginTime - m_minBeginTime);
		m_durationTime = m_minDurationTime + FRandomValue()*(m_maxDurationTime - m_minDurationTime);
	}

	//
	// ֹͣ
	//
	VOID CEmitterBase::Stop(VOID)
	{
		m_bPlaying = FALSE;
	}

	//
	// ��ͣ
	//
	VOID CEmitterBase::Pause(VOID)
	{
		m_bPause = TRUE;
	}

	//
	// ����
	//
	VOID CEmitterBase::Resume(VOID)
	{
		m_bPause = FALSE;
	}

	//
	// ����״̬
	//
	BOOL CEmitterBase::IsPlaying(VOID) const
	{
		return m_bPlaying && m_bPause == FALSE ? TRUE : FALSE;
	}

	//
	// ����
	//
	VOID CEmitterBase::Update(FLOAT deltaTime)
	{
		m_bActive = FALSE;

		//
		// 1. ����״̬���
		//
		if (IsEnable() == FALSE && IsPlaying() == FALSE) {
			return;
		}

		//
		// 2. ����
		//
		m_currTime += deltaTime;

		if (m_currTime > m_beginTime && (m_currTime < m_beginTime + m_durationTime || m_durationTime < EPSILON_E3)) {
			m_bActive = TRUE;

			m_emitDeltaTime += deltaTime;
			m_emitMount = m_emitRate * m_emitDeltaTime;

			if (m_emitMount > 1.0f) {
				m_emitDeltaTime = (m_emitMount - (INT)m_emitMount) / m_emitRate;
			}
		}
	}

	//
	// ��������
	//
	BOOL CEmitterBase::Emit(CParticle *pParticle, const VEC3 *localPosition)
	{
		ASSERT(pParticle);
		ASSERT(localPosition);

		if (m_bActive) {
			pParticle->pEmitter = this;
			pParticle->bKeepLocal = m_bKeepLocal;

			const VEC3 *parentWorldScale = GetWorldScale();
			const VEC3 *parentWorldPosition = GetWorldPosition();
			const QUAT *parentWorldOrientation = GetWorldOrientation();

			//
			// 1. ������������
			//
			pParticle->currTime = 0.0f;
			pParticle->liveTime = m_minLiveTime + FRandomValue() * (m_maxLiveTime - m_minLiveTime);

			//
			// 2. ��������
			//
			VEC3 localScale;
			FLOAT scale = FRandomValue();

			Vec3Set(&localScale,
				m_minScale[0] + scale * (m_maxScale[0] - m_minScale[0]),
				m_minScale[1] + scale * (m_maxScale[1] - m_minScale[1]),
				m_minScale[2] + scale * (m_maxScale[2] - m_minScale[2]));

			if (m_bKeepLocal) {
				Vec3Copy(&pParticle->localScale, &localScale);
			}
			else {
				Vec3Mul(&pParticle->localScale, &localScale, parentWorldScale);
			}

			//
			// 3. ���㳯��
			//
			QUAT localOrientationX;
			QUAT localOrientationY;
			QUAT localOrientationZ;
			QUAT localOrientationXY;
			QUAT localOrientationXYZ;
			FLOAT radianX = m_minRadianX + FRandomValue() * (m_maxRadianX - m_minRadianX);
			FLOAT radianY = m_minRadianY + FRandomValue() * (m_maxRadianY - m_minRadianY);
			FLOAT radianZ = m_minRadianZ + FRandomValue() * (m_maxRadianZ - m_minRadianZ);

			QuatDefRotateAxisAngle(&localOrientationX, &axisx, radianX);
			QuatDefRotateAxisAngle(&localOrientationY, &axisy, radianY);
			QuatDefRotateAxisAngle(&localOrientationZ, &axisz, radianZ);
			QuatMul(&localOrientationXY, &localOrientationX, &localOrientationY);
			QuatMul(&localOrientationXYZ, &localOrientationXY, &localOrientationZ);

			if (m_bKeepLocal) {
				QuatCopy(&pParticle->localOrientation, &localOrientationXYZ);
			}
			else {
				QuatMul(&pParticle->localOrientation, &localOrientationXYZ, parentWorldOrientation);
			}

			//
			// 4. ����λ��
			//
			if (m_bKeepLocal) {
				Vec3Copy(&pParticle->localPosition, localPosition);
			}
			else {
				VEC3 scalePosition;
				VEC3 scaleOrientationPosition;
				Vec3Mul(&scalePosition, localPosition, parentWorldScale);
				Vec3MulQuat(&scaleOrientationPosition, &scalePosition, parentWorldOrientation);
				Vec3Add(&pParticle->localPosition, &scaleOrientationPosition, parentWorldPosition);
			}

			//
			// 5. �����ٶ�
			//
			VEC3 localVelocity;
			VEC3 localDirection;
			FLOAT velocity = m_minVelocity + FRandomValue() * (m_maxVelocity - m_minVelocity);

			Vec3Set(&localDirection, 0.0f, 0.0f, 1.0f);
			Vec3MulQuat(&localVelocity, &localDirection, &pParticle->localOrientation);
			Vec3Normalize(&localVelocity);
			Vec3Scale(&localVelocity, &localVelocity, velocity);

			if (m_bKeepLocal) {
				Vec3Copy(&pParticle->localVelocity, &localVelocity);
			}
			else {
				Vec3Mul(&pParticle->localVelocity, &localVelocity, parentWorldScale);
			}

			//
			// 6. ������ת��
			//
			pParticle->radian = m_minSelfRadian + FRandomValue() * (m_maxSelfRadian - m_minSelfRadian);

			//
			// 7. ������ɫ
			//
			FLOAT color = FRandomValue();
			pParticle->color[0] = m_minColor[0] + color * (m_maxColor[0] - m_minColor[0]);
			pParticle->color[1] = m_minColor[1] + color * (m_maxColor[1] - m_minColor[1]);
			pParticle->color[2] = m_minColor[2] + color * (m_maxColor[2] - m_minColor[2]);
			pParticle->color[3] = m_minColor[3] + color * (m_maxColor[3] - m_minColor[3]);

			//
			// 8. ��������ƫ��
			//
			FLOAT uOffset = FRandomValue();
			FLOAT vOffset = FRandomValue();
			pParticle->uvOffset[0] = m_minUVOffset[0] + uOffset * (m_maxUVOffset[0] - m_minUVOffset[0]);
			pParticle->uvOffset[1] = m_minUVOffset[1] + vOffset * (m_maxUVOffset[1] - m_minUVOffset[1]);
		}

		return m_bActive;
	}

}

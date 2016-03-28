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
	// 获得粒子系统
	//
	CParticleSystem* CEmitterBase::GetParticleSystem(VOID) const
	{
		return m_pParent;
	}

	//
	// 初始化
	//
	VOID CEmitterBase::Init(VOID)
	{
		//
		// 1. 发射器参数
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
		// 2. 粒子参数
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
		// 3. 播放参数
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
	// 释放
	//
	VOID CEmitterBase::Free(VOID)
	{

	}

	//
	// 加载XML
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
	// 保存XML
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
	// 加载发射率
	//
	VOID CEmitterBase::LoadEmitRate(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pEmitRateNode = pEmitterNode->FirstChild("EmitRate")) {
			SetEmitRate(pEmitterNode->ToElement()->AttributeInt("value"));
		}
	}

	//
	// 加载开始时间
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
	// 加载持续时间
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
	// 加载绕X轴发射偏角
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
	// 加载绕Y轴发射偏角
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
	// 加载绕Z轴发射偏角
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
	// 加载保持本地位置状态
	//
	VOID CEmitterBase::LoadKeepLocal(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pKeepLocalNode = pEmitterNode->FirstChild("KeepLocal")) {
			SetKeepLocal(cglStringToAttrib(pKeepLocalNode->ToElement()->AttributeString("enable")));
		}
	}

	//
	// 加载生存时间
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
	// 加载速度
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
	// 加载自转角
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
	// 加载缩放
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
	// 加载颜色
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
	// 加载纹理偏移
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
	// 保存发射率
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
	// 保存开始时间
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
	// 保存持续时间
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
	// 保存绕X轴发射偏角
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
	// 保存绕Y轴发射偏角
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
	// 保存绕Z轴发射偏角
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
	// 保存保持本地位置状态
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
	// 保存生存时间
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
	// 保存速度
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
	// 保存自转角
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
	// 保存缩放
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
	// 保存颜色
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
	// 保存纹理偏移
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
	// 设置启用状态
	//
	VOID CEmitterBase::SetEnable(BOOL bEnable)
	{
		m_bEnable = bEnable;
	}

	//
	// 获得启用状态
	//
	BOOL CEmitterBase::IsEnable(VOID) const
	{
		return m_bEnable;
	}

	//
	// 获得活动状态
	//
	BOOL CEmitterBase::IsActive(VOID) const
	{
		return m_bActive;
	}

	//
	// 设置发射率
	//
	VOID CEmitterBase::SetEmitRate(INT rate)
	{
		m_emitRate = rate > 0 ? rate : 0;
	}

	//
	// 获得发射率
	//
	INT CEmitterBase::GetEmitRate(VOID) const
	{
		return m_emitRate;
	}

	//
	// 设置开始时间
	//
	VOID CEmitterBase::SetBeginTime(FLOAT minTime, FLOAT maxTime)
	{
		m_minBeginTime = min(minTime, maxTime);
		m_maxBeginTime = max(minTime, maxTime);
	}

	//
	// 获得最小开始时间
	//
	FLOAT CEmitterBase::GetMinBeginTime(VOID) const
	{
		return m_minBeginTime;
	}

	//
	// 获得最大开始时间
	//
	FLOAT CEmitterBase::GetMaxBeginTime(VOID) const
	{
		return m_maxBeginTime;
	}

	//
	// 设置持续时间
	//
	VOID CEmitterBase::SetDurationTime(FLOAT minTime, FLOAT maxTime)
	{
		m_minDurationTime = min(minTime, maxTime);
		m_maxDurationTime = max(minTime, maxTime);
	}

	//
	// 获得最小持续时间
	//
	FLOAT CEmitterBase::GetMinDurationTime(VOID) const
	{
		return m_minDurationTime;
	}

	//
	// 获得最大持续时间
	//
	FLOAT CEmitterBase::GetMaxDurationTime(VOID) const
	{
		return m_maxDurationTime;
	}

	//
	// 设置绕X轴发射偏角
	//
	VOID CEmitterBase::SetRadianX(FLOAT minRadian, FLOAT maxRadian)
	{
		m_minRadianX = min(minRadian, maxRadian);
		m_maxRadianX = max(minRadian, maxRadian);
	}

	//
	// 获得最小绕X轴发射偏角
	//
	FLOAT CEmitterBase::GetMinRadianX(VOID) const
	{
		return m_minRadianX;
	}

	//
	// 获得最大绕X轴发射偏角
	//
	FLOAT CEmitterBase::GetMaxRadianX(VOID) const
	{
		return m_maxRadianX;
	}

	//
	// 设置绕Y轴发射偏角
	//
	VOID CEmitterBase::SetRadianY(FLOAT minRadian, FLOAT maxRadian)
	{
		m_minRadianY = min(minRadian, maxRadian);
		m_maxRadianY = max(minRadian, maxRadian);
	}

	//
	// 获得最小绕Y轴发射偏角
	//
	FLOAT CEmitterBase::GetMinRadianY(VOID) const
	{
		return m_minRadianY;
	}

	//
	// 获得最大绕Y轴发射偏角
	//
	FLOAT CEmitterBase::GetMaxRadianY(VOID) const
	{
		return m_maxRadianY;
	}

	//
	// 设置绕Z轴发射偏角
	//
	VOID CEmitterBase::SetRadianZ(FLOAT minRadian, FLOAT maxRadian)
	{
		m_minRadianZ = min(minRadian, maxRadian);
		m_maxRadianZ = max(minRadian, maxRadian);
	}

	//
	// 获得最小绕Z轴发射偏角
	//
	FLOAT CEmitterBase::GetMinRadianZ(VOID) const
	{
		return m_minRadianZ;
	}

	//
	// 获得最大绕Z轴发射偏角
	//
	FLOAT CEmitterBase::GetMaxRadianZ(VOID) const
	{
		return m_maxRadianZ;
	}

	//
	// 设置保持本地位置状态
	//
	VOID CEmitterBase::SetKeepLocal(BOOL bKeepLocal)
	{
		m_bKeepLocal = bKeepLocal;
	}

	//
	// 获得保持本地位置状态
	//
	BOOL CEmitterBase::IsKeepLocal(VOID) const
	{
		return m_bKeepLocal;
	}

	//
	// 设置生存时间
	//
	VOID CEmitterBase::SetLiveTime(FLOAT minTime, FLOAT maxTime)
	{
		m_minLiveTime = min(minTime, maxTime);
		m_maxLiveTime = max(minTime, maxTime);
	}

	//
	// 获得最小生存时间
	//
	FLOAT CEmitterBase::GetMinLiveTime(VOID) const
	{
		return m_minLiveTime;
	}

	//
	// 获得最大生存时间
	//
	FLOAT CEmitterBase::GetMaxLiveTime(VOID) const
	{
		return m_maxLiveTime;
	}

	//
	// 设置速度
	//
	VOID CEmitterBase::SetVelocity(FLOAT minVelocity, FLOAT maxVelocity)
	{
		m_minVelocity = min(minVelocity, maxVelocity);
		m_maxVelocity = max(minVelocity, maxVelocity);
	}

	//
	// 获得最小速度
	//
	FLOAT CEmitterBase::GetMinVelocity(VOID) const
	{
		return m_minVelocity;
	}

	//
	// 设置最大速度
	//
	FLOAT CEmitterBase::GetMaxVelocity(VOID) const
	{
		return m_maxVelocity;
	}

	//
	// 设置自转偏角
	//
	VOID CEmitterBase::SetSelfRadian(FLOAT minRadian, FLOAT maxRadian)
	{
		m_minSelfRadian = min(minRadian, maxRadian);
		m_maxSelfRadian = max(minRadian, maxRadian);
	}

	//
	// 获得最小自转偏角
	//
	FLOAT CEmitterBase::GetMinSelfRadian(VOID) const
	{
		return m_minSelfRadian;
	}

	//
	// 获得最大自转偏角
	//
	FLOAT CEmitterBase::GetMaxSelfRadian(VOID) const
	{
		return m_maxSelfRadian;
	}

	//
	// 设置缩放
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
	// 设置缩放
	//
	VOID CEmitterBase::SetScale(const VEC3* minScale, const VEC3 *maxScale)
	{
		ASSERT(minScale);
		ASSERT(maxScale);
		SetScale((*minScale)[0], (*minScale)[1], (*minScale)[2], (*maxScale)[0], (*maxScale)[1], (*maxScale)[2]);
	}

	//
	// 获得最小缩放
	//
	const VEC3* CEmitterBase::GetMinScale(VOID) const
	{
		return &m_minScale;
	}

	//
	// 获得最大缩放
	//
	const VEC3* CEmitterBase::GetMaxScale(VOID) const
	{
		return &m_maxScale;
	}

	//
	// 设置颜色
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
	// 设置颜色
	//
	VOID CEmitterBase::SetColor(const VEC4* minColor, const VEC4 *maxColor)
	{
		ASSERT(minColor);
		ASSERT(maxColor);
		SetColor((*minColor)[0], (*minColor)[1], (*minColor)[2], (*minColor)[3], (*maxColor)[0], (*maxColor)[1], (*maxColor)[2], (*maxColor)[3]);
	}

	//
	// 获得最小颜色
	//
	const VEC4* CEmitterBase::GetMinColor(VOID) const
	{
		return &m_minColor;
	}

	//
	// 获得最大颜色
	//
	const VEC4* CEmitterBase::GetMaxColor(VOID) const
	{
		return &m_maxColor;
	}

	//
	// 设置纹理偏移
	//
	VOID CEmitterBase::SetUVOffset(FLOAT minUVOffsetX, FLOAT minUVOffsetY, FLOAT maxUVOffsetX, FLOAT maxUVOffsetY)
	{
		m_minUVOffset[0] = min(minUVOffsetX, maxUVOffsetX);
		m_minUVOffset[1] = min(minUVOffsetY, maxUVOffsetY);

		m_maxUVOffset[0] = max(minUVOffsetX, maxUVOffsetX);
		m_maxUVOffset[1] = max(minUVOffsetY, maxUVOffsetY);
	}

	//
	// 设置纹理偏移
	//
	VOID CEmitterBase::SetUVOffset(const VEC2* minUVOffset, const VEC2 *maxUVOffset)
	{
		ASSERT(minUVOffset);
		ASSERT(maxUVOffset);
		SetUVOffset((*minUVOffset)[0], (*minUVOffset)[1], (*maxUVOffset)[0], (*maxUVOffset)[1]);
	}

	//
	// 获得最小纹理偏移
	//
	const VEC2* CEmitterBase::GetMinUVOffset(VOID) const
	{
		return &m_minUVOffset;
	}

	//
	// 获得最大纹理偏移
	//
	const VEC2* CEmitterBase::GetMaxUVOffset(VOID) const
	{
		return &m_maxUVOffset;
	}

	//
	// 设置位置
	//
	VOID CEmitterBase::SetPosition(FLOAT x, FLOAT y, FLOAT z)
	{
		Vec3Set(&m_position, x, y, z);
	}

	//
	// 设置位置
	//
	VOID CEmitterBase::SetPosition(const VEC3 *position)
	{
		ASSERT(position);
		Vec3Copy(&m_position, position);
	}

	//
	// 获得位置
	//
	const VEC3* CEmitterBase::GetPosition(VOID) const
	{
		return &m_position;
	}

	//
	// 设置朝向
	//
	VOID CEmitterBase::SetDirection(FLOAT x, FLOAT y, FLOAT z)
	{
		VEC3 direction;
		Vec3Set(&direction, x, y, z);
		SetDirection(&direction);
	}

	//
	// 设置朝向
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
	// 设置朝向
	//
	VOID CEmitterBase::SetOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w)
	{
		QuatSet(&m_orientation, x, y, z, w);
		QuatNormalize(&m_orientation);
	}

	//
	// 设置朝向
	//
	VOID CEmitterBase::SetOrientation(const QUAT *orientation)
	{
		ASSERT(orientation);
		QuatCopy(&m_orientation, orientation);
		QuatNormalize(&m_orientation);
	}

	//
	// 获得朝向
	//
	const QUAT* CEmitterBase::GetOrientation(VOID) const
	{
		return &m_orientation;
	}

	//
	// 获得世界缩放
	//
	const VEC3* CEmitterBase::GetWorldScale(VOID)
	{
		return m_pParent->GetScale();
	}

	//
	// 获得世界位置
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
	// 获得世界朝向
	//
	const QUAT* CEmitterBase::GetWorldOrientation(VOID)
	{
		QuatMul(&m_worldOrientation, &m_orientation, m_pParent->GetOrientation());
		return &m_worldOrientation;
	}

	//
	// 获得发射量
	//
	INT CEmitterBase::GetEmitMount(VOID) const
	{
		return (INT)m_emitMount;
	}

	//
	// 播放
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
	// 停止
	//
	VOID CEmitterBase::Stop(VOID)
	{
		m_bPlaying = FALSE;
	}

	//
	// 暂停
	//
	VOID CEmitterBase::Pause(VOID)
	{
		m_bPause = TRUE;
	}

	//
	// 继续
	//
	VOID CEmitterBase::Resume(VOID)
	{
		m_bPause = FALSE;
	}

	//
	// 播放状态
	//
	BOOL CEmitterBase::IsPlaying(VOID) const
	{
		return m_bPlaying && m_bPause == FALSE ? TRUE : FALSE;
	}

	//
	// 更新
	//
	VOID CEmitterBase::Update(FLOAT deltaTime)
	{
		m_bActive = FALSE;

		//
		// 1. 播放状态检查
		//
		if (IsEnable() == FALSE && IsPlaying() == FALSE) {
			return;
		}

		//
		// 2. 更新
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
	// 发射粒子
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
			// 1. 计算粒子生命
			//
			pParticle->currTime = 0.0f;
			pParticle->liveTime = m_minLiveTime + FRandomValue() * (m_maxLiveTime - m_minLiveTime);

			//
			// 2. 计算缩放
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
			// 3. 计算朝向
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
			// 4. 计算位置
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
			// 5. 计算速度
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
			// 6. 计算自转角
			//
			pParticle->radian = m_minSelfRadian + FRandomValue() * (m_maxSelfRadian - m_minSelfRadian);

			//
			// 7. 计算颜色
			//
			FLOAT color = FRandomValue();
			pParticle->color[0] = m_minColor[0] + color * (m_maxColor[0] - m_minColor[0]);
			pParticle->color[1] = m_minColor[1] + color * (m_maxColor[1] - m_minColor[1]);
			pParticle->color[2] = m_minColor[2] + color * (m_maxColor[2] - m_minColor[2]);
			pParticle->color[3] = m_minColor[3] + color * (m_maxColor[3] - m_minColor[3]);

			//
			// 8. 计算纹理偏移
			//
			FLOAT uOffset = FRandomValue();
			FLOAT vOffset = FRandomValue();
			pParticle->uvOffset[0] = m_minUVOffset[0] + uOffset * (m_maxUVOffset[0] - m_minUVOffset[0]);
			pParticle->uvOffset[1] = m_minUVOffset[1] + vOffset * (m_maxUVOffset[1] - m_minUVOffset[1]);
		}

		return m_bActive;
	}

}

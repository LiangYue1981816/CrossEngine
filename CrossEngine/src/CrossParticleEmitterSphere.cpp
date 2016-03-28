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

	CEmitterSphere::CEmitterSphere(CParticleSystem *pParent)
		: CEmitterBase(pParent)
	{
		Init();
	}

	CEmitterSphere::~CEmitterSphere(VOID)
	{
		Free();
	}

	//
	// 获得类型
	//
	CEmitterBase::TYPE CEmitterSphere::GetType(VOID) const
	{
		return CEmitterBase::SPHERE;
	}

	//
	// 初始化
	//
	VOID CEmitterSphere::Init(VOID)
	{
		m_radius = 1.0f;

		m_bSurface = FALSE;
		m_bEnableDirectionX = TRUE;
		m_bEnableDirectionY = TRUE;
		m_bEnableDirectionZ = TRUE;

		CEmitterBase::Init();
	}

	//
	// 释放
	//
	VOID CEmitterSphere::Free(VOID)
	{
		CEmitterBase::Free();
	}

	//
	// 加载XML
	//
	VOID CEmitterSphere::LoadXML(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pRadiusNode = pEmitterNode->FirstChild("Radius")) {
			SetRadius(pRadiusNode->ToElement()->AttributeFloat("value"));
		}

		if (TiXmlNode *pSurfaceNode = pEmitterNode->FirstChild("Surface")) {
			SetSurface(cglStringToAttrib(pSurfaceNode->ToElement()->AttributeString("enable")));
		}

		if (TiXmlNode *pDirectionXNode = pEmitterNode->FirstChild("DirectionX")) {
			SetEnableDirectionX(cglStringToAttrib(pDirectionXNode->ToElement()->AttributeString("enable")));
		}

		if (TiXmlNode *pDirectionYNode = pEmitterNode->FirstChild("DirectionY")) {
			SetEnableDirectionY(cglStringToAttrib(pDirectionYNode->ToElement()->AttributeString("enable")));
		}

		if (TiXmlNode *pDirectionZNode = pEmitterNode->FirstChild("DirectionZ")) {
			SetEnableDirectionZ(cglStringToAttrib(pDirectionZNode->ToElement()->AttributeString("enable")));
		}

		CEmitterBase::LoadXML(pEmitterNode);
	}

	//
	// 保存XML
	//
	VOID CEmitterSphere::SaveXML(TiXmlNode *pEmitterNode) const
	{
		ASSERT(pEmitterNode);

		TiXmlElement *pRadiusNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Radius"); ASSERT(pRadiusNode);
		{
			pRadiusNode->SetAttributeFloat("value", m_radius);
		}
		pEmitterNode->LinkEndChild(pRadiusNode);

		TiXmlElement *pSurfaceNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Surface"); ASSERT(pSurfaceNode);
		{
			pSurfaceNode->SetAttributeString("enable", cglAttribToString(m_bSurface));
		}
		pEmitterNode->LinkEndChild(pSurfaceNode);

		TiXmlElement *pDirectionXNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("DirectionX"); ASSERT(pDirectionXNode);
		{
			pDirectionXNode->SetAttributeString("enable", cglAttribToString(m_bEnableDirectionX));
		}
		pEmitterNode->LinkEndChild(pDirectionXNode);

		TiXmlElement *pDirectionYNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("DirectionY"); ASSERT(pDirectionYNode);
		{
			pDirectionYNode->SetAttributeString("enable", cglAttribToString(m_bEnableDirectionY));
		}
		pEmitterNode->LinkEndChild(pDirectionYNode);

		TiXmlElement *pDirectionZNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("DirectionZ"); ASSERT(pDirectionZNode);
		{
			pDirectionZNode->SetAttributeString("enable", cglAttribToString(m_bEnableDirectionZ));
		}
		pEmitterNode->LinkEndChild(pDirectionZNode);

		CEmitterBase::SaveXML(pEmitterNode);
	}

	//
	// 发射粒子
	//
	BOOL CEmitterSphere::Emit(CParticle *pParticle)
	{
		VEC3 direction;
		Vec3Set(&direction, 
			m_bEnableDirectionX ? -1.0f + FRandomValue() * 2.0f : 0.0f,
			m_bEnableDirectionY ? -1.0f + FRandomValue() * 2.0f : 0.0f,
			m_bEnableDirectionZ ? -1.0f + FRandomValue() * 2.0f : 0.0f);

		FLOAT length = Vec3Normalize(&direction);
		length = m_bSurface ? m_radius : m_radius * length;

		VEC3 localPosition;
		Vec3Set(&localPosition, 0.0f, 0.0f, 0.0f);
		Vec3Ma(&localPosition, &localPosition, &direction, length);

		return CEmitterBase::Emit(pParticle, &localPosition);
	}

	//
	// 设置球半径
	//
	VOID CEmitterSphere::SetRadius(FLOAT radius)
	{
		m_radius = radius;
	}

	//
	// 获得球半径
	//
	FLOAT CEmitterSphere::GetRadius(VOID) const
	{
		return m_radius;
	}

	//
	// 设置表面发射
	//
	VOID CEmitterSphere::SetSurface(BOOL bSurface)
	{
		m_bSurface = bSurface;
	}

	//
	// 获得表面发射
	//
	BOOL CEmitterSphere::IsSurface(VOID) const
	{
		return m_bSurface;
	}

	//
	// 设置球体X方向状态
	//
	VOID CEmitterSphere::SetEnableDirectionX(BOOL bEnable)
	{
		m_bEnableDirectionX = bEnable;
	}

	//
	// 设置球体Y方向状态
	//
	VOID CEmitterSphere::SetEnableDirectionY(BOOL bEnable)
	{
		m_bEnableDirectionY = bEnable;
	}

	//
	// 设置球体Z方向状态
	//
	VOID CEmitterSphere::SetEnableDirectionZ(BOOL bEnable)
	{
		m_bEnableDirectionZ = bEnable;
	}

	//
	// 获得球体X方向状态
	//
	BOOL CEmitterSphere::IsEnableDirectionX(VOID) const
	{
		return m_bEnableDirectionX;
	}

	//
	// 获得球体Y方向状态
	//
	BOOL CEmitterSphere::IsEnableDirectionY(VOID) const
	{
		return m_bEnableDirectionY;
	}

	//
	// 获得球体Z方向状态
	//
	BOOL CEmitterSphere::IsEnableDirectionZ(VOID) const
	{
		return m_bEnableDirectionZ;
	}

}

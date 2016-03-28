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

	CAffectorKeepLocal::CAffectorKeepLocal(CParticleSystem *pParent)
		: CAffectorBase(pParent)
	{
		Init();
	}

	CAffectorKeepLocal::~CAffectorKeepLocal(VOID)
	{
		Free();
	}

	//
	// 获得类型
	//
	CAffectorBase::TYPE CAffectorKeepLocal::GetType(VOID) const
	{
		return CAffectorBase::KEEPLOCAL;
	}

	//
	// 初始化
	//
	VOID CAffectorKeepLocal::Init(VOID)
	{
		m_bKeepLocal = FALSE;
		CAffectorBase::Init();
	}

	//
	// 释放
	//
	VOID CAffectorKeepLocal::Free(VOID)
	{
		CAffectorBase::Free();
	}

	//
	// 加载XML
	//
	VOID CAffectorKeepLocal::LoadXML(TiXmlNode *pAffectorNode)
	{
		ASSERT(pAffectorNode);

		if (TiXmlNode *pKeepLocalNode = pAffectorNode->FirstChild("KeepLocal")) {
			SetKeepLocal(cglStringToAttrib(pKeepLocalNode->ToElement()->AttributeString("enable")));
		}

		CAffectorBase::LoadXML(pAffectorNode);
	}

	//
	// 保存XML
	//
	VOID CAffectorKeepLocal::SaveXML(TiXmlNode *pAffectorNode) const
	{
		ASSERT(pAffectorNode);

		TiXmlElement *pKeepLocalNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("KeepLocal"); ASSERT(pKeepLocalNode);
		{
			pKeepLocalNode->SetAttributeString("enable", cglAttribToString(m_bKeepLocal));
		}
		pAffectorNode->LinkEndChild(pKeepLocalNode);

		CAffectorBase::SaveXML(pAffectorNode);
	}

	//
	// 影响粒子
	// 注意: 只影响从KeepLocal=TRUE 到 KeepLocal=FALSE
	//
	VOID CAffectorKeepLocal::Affect(CParticle *pParticle, FLOAT deltaTime)
	{
		if (IsCanAffect(pParticle)) {
			if (pParticle->bKeepLocal && m_bKeepLocal == FALSE) {
				const VEC3 *parentWorldScale = pParticle->pEmitter->GetWorldScale();
				const VEC3 *parentWorldPosition = pParticle->pEmitter->GetWorldPosition();
				const QUAT *parentWorldOrientation = pParticle->pEmitter->GetWorldOrientation();

				QuatMul(&pParticle->localOrientation, &pParticle->localOrientation, parentWorldOrientation);

				VEC3 scalePosition;
				VEC3 scaleOrientationPosition;
				Vec3Mul(&scalePosition, &pParticle->localPosition, parentWorldScale);
				Vec3MulQuat(&scaleOrientationPosition, &scalePosition, parentWorldOrientation);
				Vec3Add(&pParticle->localPosition, &scaleOrientationPosition, parentWorldPosition);

				FLOAT velocity = Vec3Normalize(&pParticle->localVelocity);
				VEC3 localDirection;
				Vec3Set(&localDirection, 0.0f, 0.0f, 1.0f);
				Vec3MulQuat(&pParticle->localVelocity, &localDirection, &pParticle->localOrientation);
				Vec3Normalize(&pParticle->localVelocity);
				Vec3Scale(&pParticle->localVelocity, &pParticle->localVelocity, velocity);
				Vec3Mul(&pParticle->localVelocity, &pParticle->localVelocity, parentWorldScale);

				pParticle->bKeepLocal = FALSE;
			}
		}
	}

	//
	// 设置保持本地状态
	//
	VOID CAffectorKeepLocal::SetKeepLocal(BOOL bKeepLocal)
	{
		m_bKeepLocal = bKeepLocal;
	}

	//
	// 获得保持本地状态
	//
	BOOL CAffectorKeepLocal::IsKeepLocal(VOID) const
	{
		return m_bKeepLocal;
	}

}

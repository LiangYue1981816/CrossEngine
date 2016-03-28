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

	CParticleSystem::CParticleSystem(VOID)
	{
		Init();
	}

	CParticleSystem::~CParticleSystem(VOID)
	{
		Free();
	}

	//
	// ��ʼ��
	//
	VOID CParticleSystem::Init(VOID)
	{
		//
		// 1. ����ϵͳ����
		//
		m_particleType = CParticle::VIRTUAL;

		m_numParticleQuota = 0;
		m_numActiveParticles = 0;

		m_pGfx = NULL;
		m_pParticles = NULL;
		m_pActiveParticleList = NULL;
		m_pFreeParticleList = NULL;

		m_emitters.clear();
		m_affectors.clear();

		Vec3Set(&m_scale, 1.0f, 1.0f, 1.0f);
		Vec3Set(&m_position, 0.0f, 0.0f, 0.0f);
		QuatSet(&m_orientation, 0.0f, 0.0f, 0.0f, 1.0f);

		//
		// 2. ���ʲ���
		//
		m_bCloneMaterial = FALSE;
		m_bUpdateMaterial = FALSE;
		memset(m_szMaterialName, 0, sizeof(m_szMaterialName));

		m_renderID = 500;

		m_blendSrcFactor = GL_SRC_ALPHA;
		m_blendDesFactor = GL_DST_ALPHA;

		for (INT indexTexUnit = 0; indexTexUnit < MAX_TEXUNITS; indexTexUnit++) {
			m_texWrapU[indexTexUnit] = GL_REPEAT;
			m_texWrapV[indexTexUnit] = GL_REPEAT;
		}

		//
		// 3. ���Ų���
		//
		m_bPlaying = FALSE;
		m_bPause = FALSE;
	}

	//
	// �ͷ�
	//
	VOID CParticleSystem::Free(VOID)
	{
		//
		// 1. �ͷ�����
		//
		SAFE_DELETE_ARRAY(m_pParticles);

		//
		// 2. �ͷŷ�����
		//
		for (EmitterSet::const_iterator itEmitter = m_emitters.begin(); itEmitter != m_emitters.end(); ++itEmitter) {
			const CEmitterBase *pEmitter = *itEmitter;
			ASSERT(pEmitter);

			SAFE_DELETE(pEmitter);
		}

		//
		// 3. �ͷ�Ӱ����
		//
		for (AffectorSet::const_iterator itAffector = m_affectors.begin(); itAffector != m_affectors.end(); ++itAffector) {
			const CAffectorBase *pAffector = *itAffector;
			ASSERT(pAffector);

			SAFE_DELETE(pAffector);
		}

		//
		// 4. �ͷ�ͼ��
		//
		SAFE_DELETE(m_pGfx);

		//
		// 5. �ͷŲ���
		//
		m_ptrMaterial.SetNull();

		for (INT indexTexUnit = 0; indexTexUnit < MAX_TEXUNITS; indexTexUnit++) {
			m_ptrTextures[indexTexUnit].SetNull();
		}

		//
		// 6. ��ʼ��
		//
		Init();
	}

	//
	// ����XML
	//
	VOID CParticleSystem::LoadXML(TiXmlNode *pParticleSystemNode)
	{
		ASSERT(pParticleSystemNode);

		LoadGfx(pParticleSystemNode);
		LoadParticle(pParticleSystemNode);
		LoadMaterial(pParticleSystemNode);
		LoadEmitters(pParticleSystemNode);
		LoadAffectors(pParticleSystemNode);
	}

	//
	// ����XML
	//
	VOID CParticleSystem::SaveXML(TiXmlNode *pParticleSystemNode) const
	{
		ASSERT(pParticleSystemNode);

		SaveGfx(pParticleSystemNode);
		SaveParticle(pParticleSystemNode);
		SaveMaterial(pParticleSystemNode);
		SaveEmitters(pParticleSystemNode);
		SaveAffectors(pParticleSystemNode);
	}

	//
	// ����ͼ��
	//
	VOID CParticleSystem::LoadGfx(TiXmlNode *pParticleSystemNode)
	{
		ASSERT(pParticleSystemNode);

		if (TiXmlNode *pGfxNode = pParticleSystemNode->FirstChild("Gfx")) {
			CGfxBase *pGfx = CreateGfx((CGfxBase::TYPE)pGfxNode->ToElement()->AttributeInt("type"));
			ASSERT(pGfx);

			pGfx->LoadXML(pGfxNode);
		}
	}

	//
	// ��������
	//
	VOID CParticleSystem::LoadParticle(TiXmlNode *pParticleSystemNode)
	{
		ASSERT(pParticleSystemNode);

		if (TiXmlNode *pParticleNode = pParticleSystemNode->FirstChild("Particle")) {
			SetParticleQuota(pParticleNode->ToElement()->AttributeInt("quota"));
		}
	}

	//
	// ���ز���
	//
	VOID CParticleSystem::LoadMaterial(TiXmlNode *pParticleSystemNode)
	{
		ASSERT(pParticleSystemNode);

		if (TiXmlNode *pMaterialNode = pParticleSystemNode->FirstChild("Material")) {
			const CHAR *szName = pMaterialNode->ToElement()->AttributeString("name");
			if (szName == NULL) throw "No material name.";
			if (SetMaterial(szName, cglStringToAttrib(pMaterialNode->ToElement()->AttributeString("clone"))) == FALSE) throw "Invalid material.";

			if (TiXmlNode *pRenderIDNode = pMaterialNode->FirstChild("RenderID")) {
				SetRenderID(pRenderIDNode->ToElement()->AttributeInt("id"));
			}

			if (TiXmlNode *pBlendNode = pMaterialNode->FirstChild("Blend")) {
				SetBlend(
					cglStringToAttrib(pBlendNode->ToElement()->AttributeString("src_factor")), 
					cglStringToAttrib(pBlendNode->ToElement()->AttributeString("des_factor")));
			}

			if (TiXmlNode *pTextureNode = pMaterialNode->FirstChild("Texture")) {
				do {
					INT indexChannel = pTextureNode->ToElement()->AttributeInt("channel");
					const CHAR *szName = pTextureNode->ToElement()->AttributeString("name");
					if (szName == NULL) throw "No texture name.";
					if (indexChannel < 0 || indexChannel >= MAX_TEXUNITS) throw "Invalid texture channel.";
					if (SetTexture(szName, indexChannel) == FALSE) throw "Invalid texture.";

					SetTexWarpU(indexChannel, cglStringToAttrib(pTextureNode->ToElement()->AttributeString("wrapu")));
					SetTexWarpV(indexChannel, cglStringToAttrib(pTextureNode->ToElement()->AttributeString("wrapv")));
				} while (pTextureNode = pMaterialNode->IterateChildren("Texture", pTextureNode));
			}
		}
	}

	//
	// ���ط�����
	//
	VOID CParticleSystem::LoadEmitters(TiXmlNode *pParticleSystemNode)
	{
		ASSERT(pParticleSystemNode);

		if (TiXmlNode *pEmitterNode = pParticleSystemNode->FirstChild("Emitter")) {
			do {
				CEmitterBase *pEmitter = CreateEmitter((CEmitterBase::TYPE)pEmitterNode->ToElement()->AttributeInt("type"));
				ASSERT(pEmitter);

				VEC3 position;
				QUAT orientation;
				sscanf(pEmitterNode->ToElement()->AttributeString("position"), "%f %f %f", &position[0], &position[1], &position[2]);
				sscanf(pEmitterNode->ToElement()->AttributeString("orientation"), "%f %f %f %f", &orientation[0], &orientation[1], &orientation[2], &orientation[3]);
				pEmitter->SetPosition(&position);
				pEmitter->SetOrientation(&orientation);
				pEmitter->LoadXML(pEmitterNode);
			} while (pEmitterNode = pParticleSystemNode->IterateChildren("Emitter", pEmitterNode));
		}
	}

	//
	// ����Ӱ����
	//
	VOID CParticleSystem::LoadAffectors(TiXmlNode *pParticleSystemNode)
	{
		ASSERT(pParticleSystemNode);

		if (TiXmlNode *pAffectorNode = pParticleSystemNode->FirstChild("Affector")) {
			do {
				CAffectorBase *pAffector = CreateAffector((CAffectorBase::TYPE)pAffectorNode->ToElement()->AttributeInt("type"));
				ASSERT(pAffector);

				pAffector->LoadXML(pAffectorNode);
			} while (pAffectorNode = pParticleSystemNode->IterateChildren("Affector", pAffectorNode));
		}
	}

	//
	// ����ͼ��
	//
	VOID CParticleSystem::SaveGfx(TiXmlNode *pParticleSystemNode) const
	{
		ASSERT(pParticleSystemNode);

		TiXmlElement *pGfxNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Gfx"); ASSERT(pGfxNode);
		{
			pGfxNode->SetAttributeInt("type", m_pGfx->GetType());
			m_pGfx->SaveXML(pGfxNode);
		}
		pParticleSystemNode->LinkEndChild(pGfxNode);
	}

	//
	// ��������
	//
	VOID CParticleSystem::SaveParticle(TiXmlNode *pParticleSystemNode) const
	{
		ASSERT(pParticleSystemNode);

		TiXmlElement *pParticleNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Particle"); ASSERT(pParticleNode);
		{
			pParticleNode->SetAttributeInt("quota", m_numParticleQuota);
		}
		pParticleSystemNode->LinkEndChild(pParticleNode);
	}

	//
	// �������
	//
	VOID CParticleSystem::SaveMaterial(TiXmlNode *pParticleSystemNode) const
	{
		ASSERT(pParticleSystemNode);

		TiXmlElement *pMaterialNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Material"); ASSERT(pMaterialNode);
		{
			pMaterialNode->SetAttributeString("name", m_szMaterialName);
			pMaterialNode->SetAttributeString("clone", cglAttribToString(m_bCloneMaterial));

			TiXmlElement *pRenderIDNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("RenderID"); ASSERT(pRenderIDNode);
			{
				pRenderIDNode->SetAttributeInt("id", m_renderID);
			}
			pMaterialNode->LinkEndChild(pRenderIDNode);

			TiXmlElement *pBlendNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Blend"); ASSERT(pBlendNode);
			{
				pBlendNode->SetAttributeString("src_factor", cglAttribToString(m_blendSrcFactor));
				pBlendNode->SetAttributeString("des_factor", cglAttribToString(m_blendDesFactor));
			}
			pMaterialNode->LinkEndChild(pBlendNode);

			for (INT indexTexUnit = 0; indexTexUnit < MAX_TEXUNITS; indexTexUnit++) {
				if (m_ptrTextures[indexTexUnit].IsNull() == FALSE && m_ptrTextures[indexTexUnit]->IsValid()) {
					TiXmlElement *pTextureNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Texture"); ASSERT(pTextureNode);
					{
						pTextureNode->SetAttributeString("name", m_ptrTextures[indexTexUnit]->GetName());
						pTextureNode->SetAttributeInt("channel", indexTexUnit);
						pTextureNode->SetAttributeString("wrapu", cglAttribToString(m_texWrapU[indexTexUnit]));
						pTextureNode->SetAttributeString("wrapv", cglAttribToString(m_texWrapV[indexTexUnit]));
					}
					pMaterialNode->LinkEndChild(pTextureNode);
				}
			}
		}
		pParticleSystemNode->LinkEndChild(pMaterialNode);
	}

	//
	// ���淢����
	//
	VOID CParticleSystem::SaveEmitters(TiXmlNode *pParticleSystemNode) const
	{
		ASSERT(pParticleSystemNode);

		for (EmitterSet::const_iterator itEmitter = m_emitters.begin(); itEmitter != m_emitters.end(); ++itEmitter) {
			const CEmitterBase *pEmitter = *itEmitter;
			ASSERT(pEmitter);

			TiXmlElement *pEmitterNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Emitter"); ASSERT(pEmitterNode);
			{
				const VEC3 *position = pEmitter->GetPosition();
				const QUAT *orientation = pEmitter->GetOrientation();
				pEmitterNode->SetAttributeInt("type", pEmitter->GetType());
				pEmitterNode->SetAttributeString("position", "%f %f %f", (*position)[0], (*position)[1], (*position)[2]);
				pEmitterNode->SetAttributeString("orientation", "%f %f %f %f", (*orientation)[0], (*orientation)[1], (*orientation)[2], (*orientation)[3]);
				pEmitter->SaveXML(pEmitterNode);
			}
			pParticleSystemNode->LinkEndChild(pEmitterNode);
		}
	}

	//
	// ����Ӱ����
	//
	VOID CParticleSystem::SaveAffectors(TiXmlNode *pParticleSystemNode) const
	{
		ASSERT(pParticleSystemNode);

		for (AffectorSet::const_iterator itAffector = m_affectors.begin(); itAffector != m_affectors.end(); ++itAffector) {
			const CAffectorBase *pAffector = *itAffector;
			ASSERT(pAffector);

			TiXmlElement *pAffectorNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Affector"); ASSERT(pAffectorNode);
			{
				pAffectorNode->SetAttributeInt("type", pAffector->GetType());
				pAffector->SaveXML(pAffectorNode);
			}
			pParticleSystemNode->LinkEndChild(pAffectorNode);
		}
	}

	//
	// ������������
	//
	VOID CParticleSystem::SetParticleQuota(INT quota)
	{
		//
		// 1. ������ȫ���
		//
		if (quota <= 0) {
			return;
		}

		//
		// 2. ������������
		//
		m_numParticleQuota = quota;

		if (m_pGfx) {
			m_pGfx->SetParticleQuota(quota);
		}

		//
		// 3. �ͷ����Ӵ洢
		//
		SAFE_DELETE_ARRAY(m_pParticles);

		//
		// 4. �������Ӵ洢���ؽ���������
		//
		m_pParticles = SAFE_NEW(MEMTYPE_HEAP) CParticle[m_numParticleQuota];
		ASSERT(m_pParticles);

		for (INT indexParticle = 0; indexParticle < m_numParticleQuota - 1; indexParticle++) {
			m_pParticles[indexParticle + 0].pNext = &m_pParticles[indexParticle + 1];
			m_pParticles[indexParticle + 1].pNext = NULL;
		}

		m_pFreeParticleList = m_pParticles;
		m_pActiveParticleList = NULL;
	}

	//
	// �����������
	//
	INT CParticleSystem::GetParticleQuota(VOID) const
	{
		return m_numParticleQuota;
	}

	//
	// ��û������
	//
	INT CParticleSystem::GetActiveParticleCount(VOID) const
	{
		return m_numActiveParticles;
	}

	//
	// ��ÿ���������
	//
	INT CParticleSystem::GetFreeParticleCount(VOID) const
	{
		return m_numParticleQuota - m_numActiveParticles;
	}

	//
	// ����ͼ��
	//
	CGfxBase* CParticleSystem::CreateGfx(CGfxBase::TYPE type)
	{
		SAFE_DELETE(m_pGfx);

		switch (type) {
		case CGfxBase::GFX_DEBUG: m_pGfx = SAFE_NEW(MEMTYPE_HEAP) CGfxDebug; break;
		case CGfxBase::GFX_BILLBOARD: m_pGfx = SAFE_NEW(MEMTYPE_HEAP) CGfxBillboard; break;
		default: ASSERT(FALSE); break;
		}

		return m_pGfx;
	}

	//
	// ���ͼ��
	//
	CGfxBase* CParticleSystem::GetGfx(VOID) const
	{
		return m_pGfx;
	}

	//
	// ����������
	//
	CEmitterBase* CParticleSystem::CreateEmitter(CEmitterBase::TYPE type)
	{
		CEmitterBase *pEmitter = NULL;

		switch (type) {
		case CEmitterBase::POINT: pEmitter = SAFE_NEW(MEMTYPE_HEAP) CEmitterPoint(this); break;
		case CEmitterBase::SPHERE: pEmitter = SAFE_NEW(MEMTYPE_HEAP) CEmitterSphere(this); break;
		case CEmitterBase::BOX: pEmitter = SAFE_NEW(MEMTYPE_HEAP) CEmitterBox(this); break;
		default: ASSERT(FALSE); break;
		}

		m_emitters.push_back(pEmitter);
		return pEmitter;
	}

	//
	// ���ٷ�����
	//
	VOID CParticleSystem::DestroyEmitter(CEmitterBase *pEmitter)
	{
		if (pEmitter) {
			for (EmitterSet::const_iterator itEmitter = m_emitters.begin(); itEmitter != m_emitters.end(); ++itEmitter) {
				if (*itEmitter == pEmitter) {
					m_emitters.erase(itEmitter);
					break;
				}
			}

			SAFE_DELETE(pEmitter);
		}
	}

	//
	// ��÷���������
	//
	INT CParticleSystem::GetEmitterCount(VOID) const
	{
		return (INT)m_emitters.size();
	}

	//
	// ��÷�����
	//
	CEmitterBase* CParticleSystem::GetEmitter(INT indexEmitter) const
	{
		ASSERT(indexEmitter >= 0 && indexEmitter < (INT)m_emitters.size());
		return m_emitters[indexEmitter];
	}

	//
	// ����Ӱ����
	//
	CAffectorBase* CParticleSystem::CreateAffector(CAffectorBase::TYPE type)
	{
		CAffectorBase *pAffector = NULL;

		switch (type) {
		case CAffectorBase::FORCE: pAffector = SAFE_NEW(MEMTYPE_HEAP) CAffectorForce(this); break;
		case CAffectorBase::COLOR: pAffector = SAFE_NEW(MEMTYPE_HEAP) CAffectorColor(this); break;
		case CAffectorBase::SCALE: pAffector = SAFE_NEW(MEMTYPE_HEAP) CAffectorScale(this); break;
		case CAffectorBase::ROTATE: pAffector = SAFE_NEW(MEMTYPE_HEAP) CAffectorRotate(this); break;
		case CAffectorBase::RANDOM: pAffector = SAFE_NEW(MEMTYPE_HEAP) CAffectorRandom(this); break;
		case CAffectorBase::TEXTURE: pAffector = SAFE_NEW(MEMTYPE_HEAP) CAffectorTexture(this); break;
		case CAffectorBase::VELOCITY: pAffector = SAFE_NEW(MEMTYPE_HEAP) CAffectorVelocity(this); break;
		case CAffectorBase::KEEPLOCAL: pAffector = SAFE_NEW(MEMTYPE_HEAP) CAffectorKeepLocal(this); break;
		default: ASSERT(FALSE); break;
		}

		m_affectors.push_back(pAffector);
		return pAffector;
	}

	//
	// ����Ӱ����
	//
	VOID CParticleSystem::DestroyAffector(CAffectorBase *pAffector)
	{
		if (pAffector) {
			for (AffectorSet::const_iterator itAffector = m_affectors.begin(); itAffector != m_affectors.end(); ++itAffector) {
				if (*itAffector == pAffector) {
					m_affectors.erase(itAffector);
					break;
				}
			}

			SAFE_DELETE(pAffector);
		}
	}

	//
	// ���Ӱ��������
	//
	INT CParticleSystem::GetAffectorCount(VOID) const
	{
		return (INT)m_affectors.size();
	}

	//
	// ���Ӱ����
	//
	CAffectorBase* CParticleSystem::GetAffector(INT indexAffector) const
	{
		ASSERT(indexAffector >= 0 && indexAffector < (INT)m_affectors.size());
		return m_affectors[indexAffector];
	}

	//
	// ���ò���
	//
	BOOL CParticleSystem::SetMaterial(const CHAR *szName, BOOL bClone)
	{
		ASSERT(szName);

		//
		// 1. ������ȫ���
		//
		CMaterialPtr ptrMaterial = MaterialManager()->QueryResource(szName);

		if (ptrMaterial.IsNull() || ptrMaterial->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. ���ò���
		//
		if (bClone) {
			m_ptrMaterial = CMaterialPtr(MaterialManager()->CreateResource());
			m_ptrMaterial->CopyFrom(ptrMaterial.GetPointer());
			m_ptrMaterial->SetName(Engine()->GetNextMaterialName());
		}
		else {
			m_ptrMaterial = ptrMaterial;
		}

		m_bUpdateMaterial = m_bCloneMaterial = bClone;

		//
		// 3. ���������
		//
		strcpy(m_szMaterialName, ptrMaterial->GetName());

		CHAR *pch0 = strchr(m_szMaterialName, '.');
		CHAR *pch1 = strchr(++pch0, '.');
		if (pch1) *pch1 = 0;

		return TRUE;
	}

	//
	// ��������Ԫ����
	//
	BOOL CParticleSystem::SetTexture(const CHAR *szName, INT indexTexUnit)
	{
		ASSERT(szName);

		//
		// 1. ������ȫ���
		//
		if (indexTexUnit < 0 || indexTexUnit >= MAX_TEXUNITS) {
			return FALSE;
		}

		CTexturePtr ptrTexture = TextureManager()->QueryResource(szName);

		if (ptrTexture.IsNull() || ptrTexture->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. ��������Ԫ����
		//
		m_ptrTextures[indexTexUnit] = ptrTexture;
		m_bUpdateMaterial = m_bCloneMaterial;

		return TRUE;
	}

	//
	// ��ò���
	//
	const CMaterialPtr& CParticleSystem::GetMaterial(VOID) const
	{
		return m_ptrMaterial;
	}

	//
	// �������Ԫ����
	//
	const CTexturePtr& CParticleSystem::GetTexture(INT indexTexUnit) const
	{
		ASSERT(indexTexUnit >= 0 && indexTexUnit < MAX_TEXUNITS);
		return m_ptrTextures[indexTexUnit];
	}

	//
	// ������ȾID
	//
	VOID CParticleSystem::SetRenderID(INT renderID)
	{
		m_renderID = renderID;
		m_bUpdateMaterial = m_bCloneMaterial;
	}

	//
	// ���û��ģʽ
	//
	VOID CParticleSystem::SetBlend(UINT srcFactor, UINT desFactor)
	{
		m_blendSrcFactor = srcFactor;
		m_blendDesFactor = desFactor;
		m_bUpdateMaterial = m_bCloneMaterial;
	}

	//
	// �����������Ѱַ��ʽ
	//
	VOID CParticleSystem::SetTexWarpU(INT indexTexUnit, UINT warp)
	{
		ASSERT(indexTexUnit >= 0 && indexTexUnit < MAX_TEXUNITS);

		m_texWrapU[indexTexUnit] = warp;
		m_bUpdateMaterial = m_bCloneMaterial;
	}

	//
	// ������������Ѱַ��ʽ
	//
	VOID CParticleSystem::SetTexWarpV(INT indexTexUnit, UINT warp)
	{
		ASSERT(indexTexUnit >= 0 && indexTexUnit < MAX_TEXUNITS);

		m_texWrapV[indexTexUnit] = warp;
		m_bUpdateMaterial = m_bCloneMaterial;
	}

	//
	// �����ȾID
	//
	INT CParticleSystem::GetRenderID(VOID) const
	{
		return m_renderID;
	}

	//
	// ��û��ģʽ
	//
	UINT CParticleSystem::GetBlendSrcFactor(VOID) const
	{
		return m_blendSrcFactor;
	}

	//
	// ��û��ģʽ
	//
	UINT CParticleSystem::GetBlendDesFactor(VOID) const
	{
		return m_blendDesFactor;
	}

	//
	// ����������Ѱַ��ʽ
	//
	UINT CParticleSystem::GetTexWarpU(INT indexTexUnit) const
	{
		ASSERT(indexTexUnit >= 0 && indexTexUnit < MAX_TEXUNITS);
		return m_texWrapU[indexTexUnit];
	}

	//
	// �����������Ѱַ��ʽ
	//
	UINT CParticleSystem::GetTexWarpV(INT indexTexUnit) const
	{
		ASSERT(indexTexUnit >= 0 && indexTexUnit < MAX_TEXUNITS);
		return m_texWrapV[indexTexUnit];
	}

	//
	// ��������
	//
	VOID CParticleSystem::SetScale(FLOAT x, FLOAT y, FLOAT z)
	{
		Vec3Set(&m_scale, x, y, z);
	}

	//
	// ��������
	//
	VOID CParticleSystem::SetScale(const VEC3 *scale)
	{
		ASSERT(scale);
		Vec3Copy(&m_scale, scale);
	}

	//
	// �������
	//
	const VEC3* CParticleSystem::GetScale(VOID) const
	{
		return &m_scale;
	}

	//
	// ����λ��
	//
	VOID CParticleSystem::SetPosition(FLOAT x, FLOAT y, FLOAT z)
	{
		Vec3Set(&m_position, x, y, z);
	}

	//
	// ����λ��
	//
	VOID CParticleSystem::SetPosition(const VEC3 *position)
	{
		ASSERT(position);
		Vec3Copy(&m_position, position);
	}

	//
	// ���λ��
	//
	const VEC3* CParticleSystem::GetPosition(VOID) const
	{
		return &m_position;
	}

	//
	// ���ó���
	//
	VOID CParticleSystem::SetDirection(FLOAT x, FLOAT y, FLOAT z)
	{
		VEC3 direction;
		Vec3Set(&direction, x, y, z);
		SetDirection(&direction);
	}

	//
	// ���ó���
	//
	VOID CParticleSystem::SetDirection(const VEC3 *direction)
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
	VOID CParticleSystem::SetOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w)
	{
		QuatSet(&m_orientation, x, y, z, w);
		QuatNormalize(&m_orientation);
	}

	//
	// ���ó���
	//
	VOID CParticleSystem::SetOrientation(const QUAT *orientation)
	{
		ASSERT(orientation);

		QuatCopy(&m_orientation, orientation);
		QuatNormalize(&m_orientation);
	}

	//
	// ��ó���
	//
	const QUAT* CParticleSystem::GetOrientation(VOID) const
	{
		return &m_orientation;
	}

	//
	// ����
	//
	VOID CParticleSystem::Play(BOOL bImmediately)
	{
		Stop(bImmediately);

		m_bPlaying = TRUE;
		m_bPause = FALSE;
	}

	//
	// ֹͣ
	//
	VOID CParticleSystem::Stop(BOOL bImmediately)
	{
		//
		// 1. ֹͣ
		//
		m_bPlaying = FALSE;

		//
		// 2. �ؽ���������
		//
		if (bImmediately) {
			for (INT indexParticle = 0; indexParticle < m_numParticleQuota; indexParticle++) {
				m_pParticles[indexParticle].Reset();
			}

			for (INT indexParticle = 0; indexParticle < m_numParticleQuota - 1; indexParticle++) {
				m_pParticles[indexParticle + 0].pNext = &m_pParticles[indexParticle + 1];
				m_pParticles[indexParticle + 1].pNext = NULL;
			}

			m_pFreeParticleList = m_pParticles;
			m_pActiveParticleList = NULL;
		}
	}

	//
	// ��ͣ
	//
	VOID CParticleSystem::Pause(VOID)
	{
		m_bPause = TRUE;
	}

	//
	// ����
	//
	VOID CParticleSystem::Resume(VOID)
	{
		m_bPause = FALSE;
	}

	//
	// ����״̬
	//
	BOOL CParticleSystem::IsPlaying(VOID)
	{
		return m_bPlaying && m_bPause == FALSE ? TRUE : FALSE;
	}

	//
	// ���Ž���
	//
	BOOL CParticleSystem::IsPlayFinish(VOID)
	{
		//
		// 1. ����ϵͳ����״̬
		//
		if (IsPlaying()) {
			return FALSE;
		}

		//
		// 2. ����������״̬
		//
		for (EmitterSet::const_iterator itEmitter = m_emitters.begin(); itEmitter != m_emitters.end(); ++itEmitter) {
			const CEmitterBase *pEmitter = *itEmitter;
			ASSERT(pEmitter);

			if (pEmitter->IsPlaying()) {
				return FALSE;
			}
		}

		//
		// 3. Ӱ��������״̬
		//
		for (AffectorSet::const_iterator itAffector = m_affectors.begin(); itAffector != m_affectors.end(); ++itAffector) {
			const CAffectorBase *pAffector = *itAffector;
			ASSERT(pAffector);

			if (pAffector->IsPlaying()) {
				return FALSE;
			}
		}

		return TRUE;
	}

	//
	// ����
	//
	VOID CParticleSystem::Update(FLOAT deltaTime)
	{
		UpdateMaterial(deltaTime);
		UpdateEmitter(deltaTime);
		UpdateAffector(deltaTime);
		UpdateParticles(deltaTime);
	}

	//
	// ����ͼ��
	//
	VOID CParticleSystem::UpdateGfx(const CEntityCamera *pCamera)
	{
		if (m_pGfx) {
			m_pGfx->Update(pCamera, m_pActiveParticleList, m_numActiveParticles);
		}
	}

	//
	// ��Ⱦ
	//
	VOID CParticleSystem::Render(const CEntityCamera *pCamera)
	{
		if (m_pGfx) {
			m_pGfx->Render(m_numActiveParticles);
		}
	}

	//
	// ���²���
	//
	VOID CParticleSystem::UpdateMaterial(FLOAT deltaTime)
	{
		//
		// 1. ������
		//
		if (m_bCloneMaterial == FALSE || m_bUpdateMaterial == FALSE) {
			return;
		}

		if (m_ptrMaterial.IsNull() || m_ptrMaterial->IsValid() == FALSE) {
			return;
		}

		//
		// 2. ���²���
		//
		const CMaterial::SolutionMap &solutions = m_ptrMaterial->GetSolutions();
		for (CMaterial::SolutionMap::const_iterator itSolution = solutions.begin(); itSolution != solutions.end(); ++itSolution) {
			const CMatSolution *pSolution = itSolution->second;
			ASSERT(pSolution);

			const CMatSolution::PassGroupMap groups = pSolution->GetPassGroups();
			for (CMatSolution::PassGroupMap::const_iterator itGroup = groups.begin(); itGroup != groups.end(); ++itGroup) {
				const CMatPassGroup *pGroup = itGroup->second;
				ASSERT(pGroup);

				const CMatPassGroup::PassSet passes = pGroup->GetPasses();
				for (CMatPassGroup::PassSet::const_iterator itPass = passes.begin(); itPass != passes.end(); ++itPass) {
					CMatPass *pPass = *itPass;
					ASSERT(pPass);

					CRenderer::PASS *pRenderPass = pPass->GetPass();
					ASSERT(pRenderPass);

					pPass->SetRenderID(m_renderID);
					pRenderPass->blend.blendSrcColor = m_blendSrcFactor;
					pRenderPass->blend.blendSrcAlpha = m_blendSrcFactor;
					pRenderPass->blend.blendDesColor = m_blendDesFactor;
					pRenderPass->blend.blendDesAlpha = m_blendDesFactor;

					for (INT indexTexUnit = 0; indexTexUnit < MAX_TEXUNITS;  indexTexUnit++) {
						if (m_ptrTextures[indexTexUnit].IsNull() == FALSE && m_ptrTextures[indexTexUnit]->IsValid()) {
							pRenderPass->textures[indexTexUnit].target = m_ptrTextures[indexTexUnit]->GetTargetGL();
							pRenderPass->textures[indexTexUnit].texture = m_ptrTextures[indexTexUnit]->GetTextureGL();
							pRenderPass->textures[indexTexUnit].texWrapU = m_texWrapU[indexTexUnit];
							pRenderPass->textures[indexTexUnit].texWrapV = m_texWrapV[indexTexUnit];
							pRenderPass->textures[indexTexUnit].texMinFliter = GL_LINEAR_MIPMAP_LINEAR;
							pRenderPass->textures[indexTexUnit].texMagFilter = GL_LINEAR;
						}
					}
				}
			}
		}
	}

	//
	// ��������
	//
	VOID CParticleSystem::UpdateParticles(FLOAT deltaTime)
	{
		CParticle *pParticle = m_pActiveParticleList;
		CParticle *pPrevParticle = NULL;

		m_numActiveParticles = 0;

		while (pParticle) {
			pParticle->currTime += deltaTime;

			//
			// 1. ���Ӵ��, ����λ��
			//
			if (pParticle->currTime < pParticle->liveTime) {
				Vec3Ma(&pParticle->localPosition, &pParticle->localPosition, &pParticle->localVelocity, deltaTime);

				pPrevParticle = pParticle;
				pParticle = pParticle->pNext;

				m_numActiveParticles++;
			}
			//
			// 2. ��������, ����ӻ����ժ����ӵ���������
			//
			else {
				pParticle->Reset();

				if (pPrevParticle) {
					pPrevParticle->pNext = pParticle->pNext;
				}
				else {
					m_pActiveParticleList = pParticle->pNext;
				}

				pParticle->pNext = m_pFreeParticleList;
				m_pFreeParticleList = pParticle;

				pParticle = pPrevParticle ? pPrevParticle->pNext : m_pActiveParticleList;
			}
		}
	}

	//
	// ���·�����
	// ע��: ������������Ӧ����ϵͳ����ֹͣ!!!
	//
	VOID CParticleSystem::UpdateEmitter(FLOAT deltaTime)
	{
		if (IsPlaying()) {
			for (EmitterSet::const_iterator itEmitter = m_emitters.begin(); itEmitter != m_emitters.end(); ++itEmitter) {
				CEmitterBase *pEmitter = *itEmitter;
				ASSERT(pEmitter);

				if (pEmitter->IsPlaying() == FALSE) {
					pEmitter->Play();
				}

				pEmitter->Update(deltaTime);

				if (pEmitter->IsActive() && m_pFreeParticleList) {
					for (INT index = 0; index < pEmitter->GetEmitMount(); index++) {
						if (CParticle *pParticle = m_pFreeParticleList) {
							if (pEmitter->Emit(pParticle)) {
								m_pFreeParticleList = pParticle->pNext;
								pParticle->pNext = m_pActiveParticleList;
								m_pActiveParticleList = pParticle;

								continue;
							}
						}

						break;
					}
				}
			}
		}
		else {
			for (EmitterSet::const_iterator itEmitter = m_emitters.begin(); itEmitter != m_emitters.end(); ++itEmitter) {
				CEmitterBase *pEmitter = *itEmitter;
				ASSERT(pEmitter);

				if (pEmitter->IsPlaying()) {
					pEmitter->Stop();
				}
			}
		}
	}

	//
	// ����Ӱ����
	// ע��: Ӱ������������Ӧ����ϵͳ����ֹͣ, ֱ��û�л����!!!
	//
	VOID CParticleSystem::UpdateAffector(FLOAT deltaTime)
	{
		if (IsPlaying() || m_numActiveParticles) {
			for (AffectorSet::const_iterator itAffector = m_affectors.begin(); itAffector != m_affectors.end(); ++itAffector) {
				CAffectorBase *pAffector = *itAffector;
				ASSERT(pAffector);

				// ע��: 1. ��������ǰһ�β��ŵ�����������δ���������β����Ѿ���ʼ���´����Ӱ�����������²��ŵ����.
				//       2. Ӱ��������ֻӰ������ʼ�ͽ���ʱ���������Ӱ��ʱ����.
				if (pAffector->IsPlaying() == FALSE) {
					pAffector->Play();
				}

				pAffector->Update(deltaTime);

				if (CParticle *pParticle = m_pActiveParticleList) {
					do {
						pAffector->Affect(pParticle, deltaTime);
					} while (pParticle = pParticle->pNext);
				}
			}
		}
		else {
			for (AffectorSet::const_iterator itAffector = m_affectors.begin(); itAffector != m_affectors.end(); ++itAffector) {
				CAffectorBase *pAffector = *itAffector;
				ASSERT(pAffector);

				if (pAffector->IsPlaying()) {
					pAffector->Stop();
				}
			}
		}
	}

}

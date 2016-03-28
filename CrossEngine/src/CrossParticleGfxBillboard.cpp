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

	CGfxBillboard::CGfxBillboard(VOID)
	{
		Init();
	}

	CGfxBillboard::~CGfxBillboard(VOID)
	{
		Free();
	}

	//
	// 获得类型
	//
	CGfxBase::TYPE CGfxBillboard::GetType(VOID) const
	{
		return CGfxBase::GFX_BILLBOARD;
	}

	//
	// 初始化
	//
	VOID CGfxBillboard::Init(VOID)
	{
		m_offset = 0.0f;
		m_directionType = DIRECTION_CAMERA;
		MtxIdentity(&m_mtxFaceToCamera);

		CGfxBase::Init();
	}

	//
	// 释放
	//
	VOID CGfxBillboard::Free(VOID)
	{
		CGfxBase::Free();
	}

	//
	// 加载XML
	//
	VOID CGfxBillboard::LoadXML(TiXmlNode *pGfxNode)
	{
		ASSERT(pGfxNode);

		if (TiXmlNode *pOffsetNode = pGfxNode->FirstChild("Offset")) {
			SetOffset(pOffsetNode->ToElement()->AttributeFloat("value"));
		}

		if (TiXmlNode *pDirectionNode = pGfxNode->FirstChild("Direction")) {
			SetDirectionType((DIRECTION_TYPE)pDirectionNode->ToElement()->AttributeInt("value"));
		}

		CGfxBase::LoadXML(pGfxNode);
	}

	//
	// 保存XML
	//
	VOID CGfxBillboard::SaveXML(TiXmlNode *pGfxNode) const
	{
		ASSERT(pGfxNode);

		TiXmlElement *pOffsetNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Offset"); ASSERT(pOffsetNode);
		{
			pOffsetNode->SetAttributeFloat("value", m_offset);
		}
		pGfxNode->LinkEndChild(pOffsetNode);

		TiXmlElement *pDirectionNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Direction"); ASSERT(pDirectionNode);
		{
			pDirectionNode->SetAttributeInt("value", m_directionType);
		}
		pGfxNode->LinkEndChild(pDirectionNode);

		CGfxBase::SaveXML(pGfxNode);
	}

	//
	// 设置粒子总量
	//
	VOID CGfxBillboard::SetParticleQuota(INT quota)
	{
		Renderer()->DestroyVAO(&m_vao);
		Renderer()->DestroyVBO(&m_vbo);
		Renderer()->DestroyVBO(&m_ibo);

		if (quota > 0) {
			INDEX *indices = (INDEX *)SAFE_MALLOC(6 * quota * sizeof(*indices), MEMTYPE_STACK);
			{
				for (INT index = 0; index < quota; index++) {
					indices[6 * index + 0] = 4 * index + 0;
					indices[6 * index + 1] = 4 * index + 1;
					indices[6 * index + 2] = 4 * index + 2;
					indices[6 * index + 3] = 4 * index + 2;
					indices[6 * index + 4] = 4 * index + 3;
					indices[6 * index + 5] = 4 * index + 0;
				}

				m_vao = Renderer()->CreateVAO();
				m_vbo = Renderer()->CreateVBO(GL_ARRAY_BUFFER, 4 * quota * sizeof(VERTEX), NULL, GL_DYNAMIC_DRAW);
				m_ibo = Renderer()->CreateVBO(GL_ELEMENT_ARRAY_BUFFER, 6 * quota * sizeof(INDEX), indices, GL_STATIC_DRAW);
			}
			SAFE_FREE(indices);
		}
	}

	//
	// 更新
	//
	VOID CGfxBillboard::Update(const CEntityCamera *pCamera, CParticle *pParticleList, INT numParticles)
	{
		//
		// 1. 参数安全检查
		//
		if (pCamera == NULL || pParticleList == NULL || numParticles <= 0) {
			return;
		}

		//
		// 2. 更新相机矩阵
		//
		if (m_directionType == DIRECTION_CAMERA) {
			VEC3 direction;
			VEC3 position, up, target;

			Vec3Scale(&direction, pCamera->GetForwardDirection(), -1.0f);
			Vec3Set(&up, 0.0f, 1.0f, 0.0f);
			Vec3Set(&position, 0.0f, 0.0f, 0.0f);
			Vec3Ma(&target, &position, &direction, 1.0f);

			MtxDefLookAt(&m_mtxFaceToCamera, &position, &up, &target);
		}

		//
		// 3. 更新粒子数据
		//
		VERTEX *vertices = (VERTEX *)SAFE_MALLOC(4 * numParticles * sizeof(*vertices), MEMTYPE_STACK);
		{
			//    Billboard
			//    0 ___ 3
			//     |   |
			//     |___|
			//    1     2

			INT indexVertex = 0;
			CParticle *pParticle = pParticleList;

			while (pParticle) {
				const VEC3 *parentWorldScale = pParticle->pEmitter->GetWorldScale();
				const VEC3 *parentWorldPosition = pParticle->pEmitter->GetWorldPosition();
				const QUAT *parentWorldOrientation = pParticle->pEmitter->GetWorldOrientation();

				//
				// 1. 计算粒子位置与朝向
				//
				VEC3 scale;
				VEC3 position;
				QUAT orientation;

				if (pParticle->bKeepLocal && pParticle->pEmitter) {
					Vec3Mul(&scale, &pParticle->localScale, parentWorldScale);

					if (m_directionType == DIRECTION_FIXED) {
						QuatMul(&orientation, &pParticle->localOrientation, parentWorldOrientation);
					}

					VEC3 scalePosition;
					VEC3 scaleOrientationPosition;
					Vec3Mul(&scalePosition, &pParticle->localPosition, parentWorldScale);
					Vec3MulQuat(&scaleOrientationPosition, &scalePosition, parentWorldOrientation);
					Vec3Add(&position, &scaleOrientationPosition, parentWorldPosition);
				}
				else {
					Vec3Copy(&scale, &pParticle->localScale);

					if (m_directionType == DIRECTION_FIXED) {
						QuatCopy(&orientation, &pParticle->localOrientation);
					}

					Vec3Copy(&position, &pParticle->localPosition);
				}

				//
				// 2. 粒子位置偏移量
				//
				MATRIX4 mtxOrientation;

				if (m_directionType == DIRECTION_CAMERA) {
					MtxCopy(&mtxOrientation, &m_mtxFaceToCamera);

					if (m_offset < -EPSILON_E3 || m_offset > EPSILON_E3) {
						VEC3 offsetDirection;
						Vec3Sub(&offsetDirection, pCamera->GetPosition(), &position);
						Vec3Normalize(&offsetDirection);
						Vec3Ma(&position, &position, &offsetDirection, m_offset);
					}
				}
				else {
					QuatToMtxRotation(&mtxOrientation, &orientation);

					if (m_offset < -EPSILON_E3 || m_offset > EPSILON_E3) {
						VEC3 localDirection;
						VEC3 offsetDirection;
						Vec3Set(&localDirection, 0.0f, 0.0f, 1.0f);
						Vec3MulQuat(&offsetDirection, &localDirection, &orientation);
						Vec3Normalize(&offsetDirection);
						Vec3Ma(&position, &position, &offsetDirection, m_offset);
					}
				}

				//
				// 3. 计算粒子变换矩阵
				//
				MATRIX4 mtxScale;
				MATRIX4 mtxRotate;
				MATRIX4 mtxRotateSelf;
				MATRIX4 mtxTranslate;
				MtxDefScale(&mtxScale, scale[0], scale[1], scale[2]);
				MtxDefTranslate(&mtxTranslate, position[0], position[1], position[2]);
				MtxDefRotateAxisAngle(&mtxRotateSelf, &axisz, pParticle->radian);
				MtxMul(&mtxRotate, &mtxRotateSelf, &mtxOrientation);

				MATRIX4 mtxSR;
				MATRIX4 mtxTransform;
				MtxMul(&mtxSR, &mtxScale, &mtxRotate);
				MtxMul(&mtxTransform, &mtxSR, &mtxTranslate);

				//
				// 4. 计算粒子纹理矩阵
				//
				MATRIX4 mtxTexScale;
				MATRIX4 mtxTexTranslate;
				MATRIX4 mtxTexTransform;
				MtxDefScale(&mtxTexScale, pParticle->texSequenceScale[0], pParticle->texSequenceScale[1], 1.0f);
				MtxDefTranslate(&mtxTexTranslate, pParticle->texSequenceOffset[0] + pParticle->texScrollOffset[0], pParticle->texSequenceOffset[1] + pParticle->texScrollOffset[1], 0.0f);
				MtxMul(&mtxTexTransform, &mtxTexScale, &mtxTexTranslate);

				//
				// 5. 计算粒子顶点
				//
				VEC3 desVertices[4];
				VEC3 srcVertices[4] = {
					VEC3(-1.0f,  1.0f, 0.0f),
					VEC3(-1.0f, -1.0f, 0.0f),
					VEC3( 1.0f, -1.0f, 0.0f),
					VEC3( 1.0f,  1.0f, 0.0f),
				};

				VEC2 texCoords[4] = {
					VEC2(pParticle->uvOffset[0] + 0.0f, pParticle->uvOffset[1] + 0.0f),
					VEC2(pParticle->uvOffset[0] + 0.0f, pParticle->uvOffset[1] + 1.0f),
					VEC2(pParticle->uvOffset[0] + 1.0f, pParticle->uvOffset[1] + 1.0f),
					VEC2(pParticle->uvOffset[0] + 1.0f, pParticle->uvOffset[1] + 0.0f),
				};

				VEC3 localNormal, localBinormal;
				VEC3 worldNormal, worldBinormal;

				Vec3Set(&localNormal, 0.0f, 0.0f, 1.0f);
				Vec3Set(&localBinormal, 1.0f, 0.0f, 0.0f);
				Vec3MulMtx3x3(&worldNormal, &localNormal, &mtxTransform);
				Vec3MulMtx3x3(&worldBinormal, &localBinormal, &mtxTransform);

				Vec3MulMtx4x4(&desVertices[0], &srcVertices[0], &mtxTransform);
				Vec3MulMtx4x4(&desVertices[1], &srcVertices[1], &mtxTransform);
				Vec3MulMtx4x4(&desVertices[2], &srcVertices[2], &mtxTransform);
				Vec3MulMtx4x4(&desVertices[3], &srcVertices[3], &mtxTransform);

				Vec3Copy(&vertices[indexVertex].position, &desVertices[0]);
				Vec3Copy(&vertices[indexVertex].normal, &worldNormal);
				Vec3Copy(&vertices[indexVertex].binormal, &worldBinormal);
				Vec4Copy(&vertices[indexVertex].color, &pParticle->color);
				Vec2MulMtx4x4(&vertices[indexVertex].texCoordDiffuse, &texCoords[0], &mtxTexTransform);
				indexVertex++;

				Vec3Copy(&vertices[indexVertex].position, &desVertices[1]);
				Vec3Copy(&vertices[indexVertex].normal, &worldNormal);
				Vec3Copy(&vertices[indexVertex].binormal, &worldBinormal);
				Vec4Copy(&vertices[indexVertex].color, &pParticle->color);
				Vec2MulMtx4x4(&vertices[indexVertex].texCoordDiffuse, &texCoords[1], &mtxTexTransform);
				indexVertex++;

				Vec3Copy(&vertices[indexVertex].position, &desVertices[2]);
				Vec3Copy(&vertices[indexVertex].normal, &worldNormal);
				Vec3Copy(&vertices[indexVertex].binormal, &worldBinormal);
				Vec4Copy(&vertices[indexVertex].color, &pParticle->color);
				Vec2MulMtx4x4(&vertices[indexVertex].texCoordDiffuse, &texCoords[2], &mtxTexTransform);
				indexVertex++;

				Vec3Copy(&vertices[indexVertex].position, &desVertices[3]);
				Vec3Copy(&vertices[indexVertex].normal, &worldNormal);
				Vec3Copy(&vertices[indexVertex].binormal, &worldBinormal);
				Vec4Copy(&vertices[indexVertex].color, &pParticle->color);
				Vec2MulMtx4x4(&vertices[indexVertex].texCoordDiffuse, &texCoords[3], &mtxTexTransform);
				indexVertex++;

				pParticle = pParticle->pNext;
			}

			Renderer()->BindVBO(GL_ARRAY_BUFFER, m_vbo);
			Renderer()->UpdateVBO(GL_ARRAY_BUFFER, 0, 4 * numParticles * sizeof(*vertices), vertices);
		}
		SAFE_FREE(vertices);
	}

	//
	// 渲染
	//
	VOID CGfxBillboard::Render(INT numParticles)
	{
		Renderer()->PushMatrix();
		{
			//
			// 1. 设置变换矩阵
			//7
			Renderer()->SetModelWorldMatrix(mtxIdentity);

			//
			// 2. 提交缓冲
			//
			Renderer()->BindVBO(GL_ARRAY_BUFFER, m_vbo);
			Renderer()->BindVBO(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
			Renderer()->SubmitVertexBuffer(CRenderer::VERTEX_FORMAT_PARTICLE, NULL);

			//
			// 3. 渲染
			//
			Renderer()->Render(NULL, 6 * numParticles);
		}
		Renderer()->PopMatrix();
	}

	//
	// 设置偏移量
	//
	VOID CGfxBillboard::SetOffset(FLOAT offset)
	{
		m_offset = offset;
	}

	//
	// 获得偏移量
	//
	FLOAT CGfxBillboard::GetOffset(VOID) const
	{
		return m_offset;
	}

	//
	// 设置朝向类型
	//
	VOID CGfxBillboard::SetDirectionType(CGfxBillboard::DIRECTION_TYPE type)
	{
		m_directionType = type;
	}

	//
	// 获得朝向类型
	//
	CGfxBillboard::DIRECTION_TYPE CGfxBillboard::GetDirectionType(VOID) const
	{
		return m_directionType;
	}

}

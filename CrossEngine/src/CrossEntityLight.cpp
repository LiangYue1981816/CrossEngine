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

	CEntityLight::CEntityLight(DWORD dwName, CSceneManager *pSceneManager)
		: CEntity(dwName, pSceneManager)
	{
		AABBZero(&m_aabbLocal);
		AABBZero(&m_aabbWorld);

		SetLightType(LIGHT_POINT);
		SetRadius(1.0f);
		SetAttenuation(1.0f, 0.0f, 0.0f);
		SetAmbient(0.2f, 0.2f, 0.2f);
		SetDiffuse(0.8f, 0.8f, 0.8f);
		SetSpecular(0.2f, 0.2f, 0.2f);
		SetRim(0.5f, 0.5f, 0.5f);
		SetSkyLower(0.1f, 0.1f, 0.1f);
		SetSkyUpper(0.3f, 0.3f, 0.3f);

		m_query = Renderer()->CreateQuery();
	}

	CEntityLight::~CEntityLight(VOID)
	{
		Renderer()->DestroyQuery(&m_query);
	}

	//
	// 获得类型
	//
	CEntity::TYPE CEntityLight::GetType(VOID) const
	{
		return CEntity::LIGHT;
	}

	//
	// 获得可渲染
	//
	BOOL CEntityLight::IsRenderable(VOID) const
	{
		return TRUE;
	}

	//
	// 设置类型
	//
	VOID CEntityLight::SetLightType(LIGHT_TYPE type)
	{
		m_type = type;
	}

	//
	// 设置半径
	//
	VOID CEntityLight::SetRadius(FLOAT radius)
	{
		m_radius = radius;

		Vec3Set(&m_aabbLocal.center, 0.0f, 0.0f, 0.0f);
		Vec3Set(&m_aabbLocal.minVertex, -radius, -radius, -radius);
		Vec3Set(&m_aabbLocal.maxVertex,  radius,  radius,  radius);
	}

	//
	// 设置衰减
	//
	VOID CEntityLight::SetAttenuation(FLOAT k0, FLOAT k1, FLOAT k2)
	{
		m_attenuation[0] = k0;
		m_attenuation[1] = k1;
		m_attenuation[2] = k2;
	}

	//
	// 设置环境光
	//
	VOID CEntityLight::SetAmbient(FLOAT r, FLOAT g, FLOAT b)
	{
		m_ambient[0] = r;
		m_ambient[1] = g;
		m_ambient[2] = b;
	}

	//
	// 设置漫反射光
	//
	VOID CEntityLight::SetDiffuse(FLOAT r, FLOAT g, FLOAT b)
	{
		m_diffuse[0] = r;
		m_diffuse[1] = g;
		m_diffuse[2] = b;
	}

	//
	// 设置镜面光
	//
	VOID CEntityLight::SetSpecular(FLOAT r, FLOAT g, FLOAT b)
	{
		m_specular[0] = r;
		m_specular[1] = g;
		m_specular[2] = b;
	}

	//
	// 设置轮廓光
	//
	VOID CEntityLight::SetRim(FLOAT r, FLOAT g, FLOAT b)
	{
		m_rim[0] = r;
		m_rim[1] = g;
		m_rim[2] = b;
	}

	//
	// 设置天光
	//
	VOID CEntityLight::SetSkyLower(FLOAT r, FLOAT g, FLOAT b)
	{
		m_skyLower[0] = r;
		m_skyLower[1] = g;
		m_skyLower[2] = b;
	}

	//
	// 设置天光
	//
	VOID CEntityLight::SetSkyUpper(FLOAT r, FLOAT g, FLOAT b)
	{
		m_skyUpper[0] = r;
		m_skyUpper[1] = g;
		m_skyUpper[2] = b;
	}

	//
	// 获得类型
	//
	CEntityLight::LIGHT_TYPE CEntityLight::GetLightType(VOID) const
	{
		return m_type;
	}

	//
	// 获得半径
	//
	FLOAT CEntityLight::GetRadius(VOID) const
	{
		return m_radius;
	}

	//
	// 获得衰减
	//
	const FLOAT* CEntityLight::GetAttenuation(VOID) const
	{
		return m_attenuation;
	}

	//
	// 获得环境光
	//
	const FLOAT* CEntityLight::GetAmbient(VOID) const
	{
		return m_ambient;
	}

	//
	// 获得漫反射光
	//
	const FLOAT* CEntityLight::GetDiffuse(VOID) const
	{
		return m_diffuse;
	}

	//
	// 获得镜面光
	//
	const FLOAT* CEntityLight::GetSpecular(VOID) const
	{
		return m_specular;
	}

	//
	// 获得轮廓光
	//
	const FLOAT* CEntityLight::GetRim(VOID) const
	{
		return m_rim;
	}

	//
	// 获得天光
	//
	const FLOAT* CEntityLight::GetSkyLower(VOID) const
	{
		return m_skyLower;
	}

	//
	// 获得天光
	//
	const FLOAT* CEntityLight::GetSkyUpper(VOID) const
	{
		return m_skyUpper;
	}

	//
	// 获得包围盒
	//
	const AABB* CEntityLight::GetAABB(VOID)
	{
		ASSERT(m_pSceneNode);
		return AABBTransform(&m_aabbWorld, &m_aabbLocal, &m_pSceneNode->m_mtxWorld);
	}

	//
	// 更新渲染队列
	//
	VOID CEntityLight::UpdateRenderQuene(const CEntityCamera *pCamera)
	{
		ASSERT(pCamera);
		m_pSceneManager->AddLightRenderQuene(pCamera, this);
	}

	//
	// 渲染查询
	//
	VOID CEntityLight::RenderQuery(VOID) const
	{
		ASSERT(m_pSceneNode);

		AABB aabbLight;

		Vec3Set(&aabbLight.center, m_aabbWorld.center[0], m_aabbWorld.center[1], m_aabbWorld.center[2]);
		Vec3Set(&aabbLight.minVertex, m_aabbWorld.center[0] - 0.1f, m_aabbWorld.center[1] - 0.1f, m_aabbWorld.center[2] - 0.1f);
		Vec3Set(&aabbLight.maxVertex, m_aabbWorld.center[0] + 0.1f, m_aabbWorld.center[1] + 0.1f, m_aabbWorld.center[2] + 0.1f);

		Renderer()->RenderQuery(m_query, &aabbLight);
	}

	//
	// 渲染光照
	//
	VOID CEntityLight::RenderLighting(VOID) const
	{
		ASSERT(m_pSceneNode);

		FLOAT position[4];

		if (m_type == LIGHT_POINT) {
			VEC3 localPosition;
			VEC3 worldPosition;
			Vec3Set(&localPosition, 0.0f, 0.0f, 0.0f);
			Vec3MulMtx4x4(&worldPosition, &localPosition, &m_pSceneNode->m_mtxWorld);

			position[0] = worldPosition[0];
			position[1] = worldPosition[1];
			position[2] = worldPosition[2];
			position[3] = 1.0f;
		}
		else {
			VEC3 localDirection;
			VEC3 worldDirection;
			Vec3Set(&localDirection, 0.0f, 0.0f, 1.0f);
			Vec3MulMtx3x3(&worldDirection, &localDirection, &m_pSceneNode->m_mtxWorld);

			position[0] = worldDirection[0];
			position[1] = worldDirection[1];
			position[2] = worldDirection[2];
			position[3] = 0.0f;
		}

		Renderer()->SetLight(position, m_ambient, m_diffuse, m_specular, m_rim, m_skyLower, m_skyUpper, m_attenuation);
		Renderer()->RenderLighting(m_aabbWorld.center, m_radius);
	}

	//
	// 获得遮挡查询
	//
	UINT CEntityLight::GetQuery(VOID) const
	{
		return m_query;
	}

}

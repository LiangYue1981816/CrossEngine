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

	const FLOAT CEntityCamera::DEFAULT_ZFAR = 500.0f;
	const FLOAT CEntityCamera::DEFAULT_ZNEAR = 0.01f;

	CEntityCamera::CEntityCamera(DWORD dwName, CSceneManager *pSceneManager)
		: CEntity(dwName, pSceneManager)
	{
		MtxIdentity(&m_mtxProjection);
		MtxIdentity(&m_mtxCameraToWorld);
		MtxIdentity(&m_mtxWorldToCamera);

		SetShake(0.0f, 0.0f, 0.0f);
		SetPosition(0.0f, 0.0f, 1.0f);
		SetUpDirection(0.0f, 1.0f, 0.0f);
		SetForwardDirection(0.0f, 0.0f, -1.0f);
		SetPerspective(60.0f, 1024.0f / 768.0f, DEFAULT_ZNEAR, DEFAULT_ZFAR);
	}

	CEntityCamera::~CEntityCamera(VOID)
	{

	}

	//
	// �������
	//
	CEntity::TYPE CEntityCamera::GetType(VOID) const
	{
		return CEntity::CAMERA;
	}

	//
	// ��ÿ���Ⱦ
	//
	BOOL CEntityCamera::IsRenderable(VOID) const
	{
		return FALSE;
	}

	//
	// ����͸��ͶӰ��׶
	//
	VOID CEntityCamera::SetPerspective(FLOAT fovy, FLOAT aspect, FLOAT zNear, FLOAT zFar)
	{
		//
		// 1. ������׶����
		//
		m_fovx = RAD2DEG(FastATan(FastTan(DEG2RAD(fovy) * 0.5f) * aspect) * 2.0f);
		m_fovy = fovy;
		m_aspect = aspect;
		m_zNear = zNear;
		m_zFar = zFar;

		MtxDefPerspective(&m_mtxProjection, m_fovy, m_aspect, m_zNear, m_zFar);

		//
		// 2. ������׶ƽ��
		//
		VEC3 vertexOrigin;
		VEC3 vertexNear;
		VEC3 vertexFar;

		VEC3 normalLeft;
		VEC3 normalRight;
		VEC3 normalTop;
		VEC3 normalBottom;
		VEC3 normalNear;
		VEC3 normalFar;

		FLOAT d = 1.0f / FastTan(DEG2RAD(m_fovy) * 0.5f);

		Vec3Set(&vertexOrigin, 0.0f, 0.0f, 0.0f);
		Vec3Set(&vertexNear, 0.0f, 0.0f, -m_zNear);
		Vec3Set(&vertexFar, 0.0f, 0.0f, -m_zFar);

		Vec3Set(&normalLeft, d, 0.0f, -m_aspect);
		Vec3Set(&normalRight, -d, 0.0f, -m_aspect);
		Vec3Set(&normalTop, 0.0f, -d, -1.0f);
		Vec3Set(&normalBottom, 0.0f, d, -1.0f);
		Vec3Set(&normalNear, 0.0f, 0.0f, -1.0f);
		Vec3Set(&normalFar, 0.0f, 0.0f, 1.0f);

		PlaneSet(&m_frustumPlanes[PLANE_TOP][LOCAL], &vertexOrigin, &normalTop);
		PlaneSet(&m_frustumPlanes[PLANE_BOTTOM][LOCAL], &vertexOrigin, &normalBottom);
		PlaneSet(&m_frustumPlanes[PLANE_LEFT][LOCAL], &vertexOrigin, &normalLeft);
		PlaneSet(&m_frustumPlanes[PLANE_RIGHT][LOCAL], &vertexOrigin, &normalRight);
		PlaneSet(&m_frustumPlanes[PLANE_NEAR][LOCAL], &vertexNear, &normalNear);
		PlaneSet(&m_frustumPlanes[PLANE_FAR][LOCAL], &vertexFar, &normalFar);
	}

	//
	// ��������ͶӰ��׶
	//
	VOID CEntityCamera::SetOrtho(FLOAT left, FLOAT right, FLOAT bottom, FLOAT top, FLOAT zNear, FLOAT zFar)
	{
		//
		// 1. ������׶����
		//
		m_left = left;
		m_right = right;
		m_top = top;
		m_bottom = bottom;
		m_zNear = zNear;
		m_zFar = zFar;

		MtxDefOrtho(&m_mtxProjection, m_top, m_bottom, m_left, m_right, m_zNear, m_zFar);

		//
		// 2. ������׶ƽ��
		//
		VEC3 vertexLeft;
		VEC3 vertexRight;
		VEC3 vertexTop;
		VEC3 vertexBottom;
		VEC3 vertexNear;
		VEC3 vertexFar;

		VEC3 normalLeft;
		VEC3 normalRight;
		VEC3 normalTop;
		VEC3 normalBottom;
		VEC3 normalNear;
		VEC3 normalFar;

		Vec3Set(&vertexLeft, m_left, 0.0f, 0.0f);
		Vec3Set(&vertexRight, m_right, 0.0f, 0.0f);
		Vec3Set(&vertexTop, 0.0f, m_top, 0.0f);
		Vec3Set(&vertexBottom, 0.0f, m_bottom, 0.0f);
		Vec3Set(&vertexNear, 0.0f, 0.0f, m_zNear);
		Vec3Set(&vertexFar, 0.0f, 0.0f, m_zFar);

		Vec3Set(&normalLeft, 1.0f, 0.0f, 0.0f);
		Vec3Set(&normalRight, -1.0f, 0.0f, 0.0f);
		Vec3Set(&normalTop, 0.0f, -1.0f, 0.0f);
		Vec3Set(&normalBottom, 0.0f, 1.0f, 0.0f);
		Vec3Set(&normalNear, 0.0f, 0.0f, -1.0f);
		Vec3Set(&normalFar, 0.0f, 0.0f, 1.0f);

		PlaneSet(&m_frustumPlanes[PLANE_TOP][LOCAL], &vertexTop, &normalTop);
		PlaneSet(&m_frustumPlanes[PLANE_BOTTOM][LOCAL], &vertexBottom, &normalBottom);
		PlaneSet(&m_frustumPlanes[PLANE_LEFT][LOCAL], &vertexLeft, &normalLeft);
		PlaneSet(&m_frustumPlanes[PLANE_RIGHT][LOCAL], &vertexRight, &normalRight);
		PlaneSet(&m_frustumPlanes[PLANE_NEAR][LOCAL], &vertexNear, &normalNear);
		PlaneSet(&m_frustumPlanes[PLANE_FAR][LOCAL], &vertexFar, &normalFar);
	}

	//
	// ���͸��ͶӰ��׶�н�
	//
	FLOAT CEntityCamera::GetFovx(VOID) const
	{
		return m_fovx;
	}

	//
	// ���͸��ͶӰ��׶�н�
	//
	FLOAT CEntityCamera::GetFovy(VOID) const
	{
		return m_fovy;
	}

	//
	// ���͸��ͶӰ��׶��߱�
	//
	FLOAT CEntityCamera::GetAspect(VOID) const
	{
		return m_aspect;
	}

	//
	// �������ͶӰ��׶��
	//
	FLOAT CEntityCamera::GetLeft(VOID) const
	{
		return m_left;
	}

	//
	// �������ͶӰ��׶�� 
	//
	FLOAT CEntityCamera::GetRight(VOID) const
	{
		return m_right;
	}

	//
	// �������ͶӰ��׶��
	//
	FLOAT CEntityCamera::GetTop(VOID) const
	{
		return m_top;
	}

	//
	// �������ͶӰ��׶��
	//
	FLOAT CEntityCamera::GetBottom(VOID) const
	{
		return m_bottom;
	}

	//
	// �����׶���������
	//
	FLOAT CEntityCamera::GetZNear(VOID) const
	{
		return m_zNear;
	}

	//
	// �����׶Զ�������
	//
	FLOAT CEntityCamera::GetZFar(VOID) const
	{
		return m_zFar;
	}

	//
	// ���������
	//
	VOID CEntityCamera::SetShake(FLOAT x, FLOAT y, FLOAT z)
	{
		Vec3Set(&m_shake, x, y, z);
	}

	//
	// �������λ��
	//
	VOID CEntityCamera::SetPosition(FLOAT x, FLOAT y, FLOAT z)
	{
		Vec3Set(&m_position, x, y, z);
	}

	//
	// �������ǰ����
	//
	VOID CEntityCamera::SetForwardDirection(FLOAT x, FLOAT y, FLOAT z)
	{
		Vec3Set(&m_forwardDirection, x, y, z);
		Vec3Normalize(&m_forwardDirection);
	}

	//
	// �������������
	//
	VOID CEntityCamera::SetUpDirection(FLOAT x, FLOAT y, FLOAT z)
	{
		Vec3Set(&m_upDirection, x, y, z);
		Vec3Normalize(&m_upDirection);
	}

	//
	// ��������
	//
	const VEC3* CEntityCamera::GetShake(VOID) const
	{
		return &m_shake;
	}

	//
	// ������λ��
	//
	const VEC3* CEntityCamera::GetPosition(VOID) const
	{
		return &m_position;
	}

	//
	// ������ǰ����
	//
	const VEC3* CEntityCamera::GetForwardDirection(VOID) const
	{
		return &m_forwardDirection;
	}

	//
	// ������������
	//
	const VEC3* CEntityCamera::GetUpDirection(VOID) const
	{
		return &m_upDirection;
	}

	//
	// �������
	//
	RAY* CEntityCamera::GetRay(RAY *ray, FLOAT scrx, FLOAT scry) const
	{
		ASSERT(ray);

		VEC2 screen;
		VEC3 target;
		VEC3 position;

		Vec2Set(&screen, scrx, scry);
		ScreenToWorld(&target, &screen);
		Vec3Add(&position, &m_position, &m_shake);

		Vec3Set(&ray->origin, position[0], position[1], position[2]);
		Vec3Sub(&ray->direction, &target, &position);
		Vec3Normalize(&ray->direction);

		return ray;
	}

	//
	// ��������ϵ�任����Ļ����ϵ
	//
	VEC3* CEntityCamera::WorldToScreen(VEC3 *screen, const VEC3 *world) const
	{
		ASSERT(world);
		ASSERT(screen);

		return Project(screen, world, &m_mtxWorldToCamera, &m_mtxProjection, Renderer()->GetViewportX(), Renderer()->GetViewportY(), Renderer()->GetViewportWidth(), Renderer()->GetViewportHeight());
	}

	//
	// ��Ļ����ϵ�任����������ϵ
	//
	VEC3* CEntityCamera::ScreenToWorld(VEC3 *world, const VEC2 *screen) const
	{
		ASSERT(world);
		ASSERT(screen);

		return UnProject(world, screen, &m_mtxCameraToWorld, &m_mtxProjection, Renderer()->GetViewportX(), Renderer()->GetViewportY(), Renderer()->GetViewportWidth(), Renderer()->GetViewportHeight());
	}

	//
	// ���ͶӰ�任����
	//
	const MATRIX4* CEntityCamera::GetProjectionMatrix(VOID) const
	{
		return &m_mtxProjection;
	}

	//
	// ����������ϵ����������ϵ�任����
	//
	const MATRIX4* CEntityCamera::GetCameraToWorldMatrix(VOID) const
	{
		return &m_mtxCameraToWorld;
	}

	//
	// �����������ϵ���������ϵ�任����
	//
	const MATRIX4* CEntityCamera::GetWorldToCameraMatrix(VOID) const
	{
		return &m_mtxWorldToCamera;
	}

	//
	// ��Χ�пɼ��Լ��
	//
	BOOL CEntityCamera::IsVisible(const AABB *pAABB) const
	{
		ASSERT(pAABB);

		//
		// 1. ��Χ�м��
		//
		if (IsAABBValid(pAABB) == FALSE) {
			return FALSE;
		}

		//
		// 2. ��Χ�пɼ��Լ��
		//    �㷨: ֻ��AABB��8�����㶼λ��ĳ�������汳��ʱ, ���AABB���ɼ�
		//
		VEC3 vertices[8];

		Vec3Set(&vertices[0], pAABB->minVertex[0], pAABB->minVertex[1], pAABB->minVertex[2]);
		Vec3Set(&vertices[1], pAABB->maxVertex[0], pAABB->minVertex[1], pAABB->minVertex[2]);
		Vec3Set(&vertices[2], pAABB->maxVertex[0], pAABB->minVertex[1], pAABB->maxVertex[2]);
		Vec3Set(&vertices[3], pAABB->minVertex[0], pAABB->minVertex[1], pAABB->maxVertex[2]);
		Vec3Set(&vertices[4], pAABB->minVertex[0], pAABB->maxVertex[1], pAABB->minVertex[2]);
		Vec3Set(&vertices[5], pAABB->maxVertex[0], pAABB->maxVertex[1], pAABB->minVertex[2]);
		Vec3Set(&vertices[6], pAABB->maxVertex[0], pAABB->maxVertex[1], pAABB->maxVertex[2]);
		Vec3Set(&vertices[7], pAABB->minVertex[0], pAABB->maxVertex[1], pAABB->maxVertex[2]);

		for (INT indexPlane = 0; indexPlane < 6; indexPlane++) {
			for (INT indexVertex = 0; indexVertex < 8; indexVertex++) {
				if (VertexPlaneSide(&vertices[indexVertex], &m_frustumPlanes[indexPlane][WORLD]) == PLANE_SIDE_FRONT) {
					goto NEXT;
				}
			}

			return FALSE;

		NEXT:
			continue;
		}

		return TRUE;
	}

	//
	// ����ɼ��Լ��
	//
	BOOL CEntityCamera::IsVisible(const VEC3 *pVertex) const
	{
		ASSERT(pVertex);

		if (VertexPlaneSide(pVertex, &m_frustumPlanes[PLANE_TOP][WORLD]) == PLANE_SIDE_BACK) return FALSE;
		if (VertexPlaneSide(pVertex, &m_frustumPlanes[PLANE_BOTTOM][WORLD]) == PLANE_SIDE_BACK) return FALSE;
		if (VertexPlaneSide(pVertex, &m_frustumPlanes[PLANE_LEFT][WORLD]) == PLANE_SIDE_BACK) return FALSE;
		if (VertexPlaneSide(pVertex, &m_frustumPlanes[PLANE_RIGHT][WORLD]) == PLANE_SIDE_BACK) return FALSE;
		if (VertexPlaneSide(pVertex, &m_frustumPlanes[PLANE_NEAR][WORLD]) == PLANE_SIDE_BACK) return FALSE;
		if (VertexPlaneSide(pVertex, &m_frustumPlanes[PLANE_FAR][WORLD]) == PLANE_SIDE_BACK) return FALSE;

		return TRUE;
	}

	//
	// ����
	//
	VOID CEntityCamera::UpdateInstance(VOID)
	{
		//
		// 1. ���ݳ����ڵ���·�λ
		//
		if (m_pSceneNode) {
			const MATRIX4 *mtxTransform = m_pSceneNode->GetWorldMatrix();

			const VEC3 localPosition(0.0f, 0.0f, 0.0f);
			const VEC3 localUpDirection(0.0f, 1.0f, 0.0f);
			const VEC3 localForwardDirection(0.0f, 0.0f, 1.0f);

			Vec3MulMtx4x4(&m_position, &localPosition, mtxTransform);
			Vec3MulMtx3x3(&m_upDirection, &localUpDirection, mtxTransform);
			Vec3MulMtx3x3(&m_forwardDirection, &localForwardDirection, mtxTransform);
		}

		//
		// 2. ��������任����
		//
		VEC3 u, v, n;
		VEC3 position;

		Vec3Add(&position, &m_position, &m_shake);

		Vec3Set(&v, m_upDirection[0], m_upDirection[1], m_upDirection[2]);
		Vec3Set(&n, -m_forwardDirection[0], -m_forwardDirection[1], -m_forwardDirection[2]);
		Vec3Cross(&u, &v, &n);
		Vec3Cross(&v, &n, &u);

		MtxSet(&m_mtxCameraToWorld,
			u[0], u[1], u[2], 0.0f,
			v[0], v[1], v[2], 0.0f,
			n[0], n[1], n[2], 0.0f,
			position[0], position[1], position[2], 1.0f);

		MtxSet(&m_mtxWorldToCamera,
			u[0], v[0], n[0], 0.0f,
			u[1], v[1], n[1], 0.0f,
			u[2], v[2], n[2], 0.0f,
			-Vec3Dot(&position, &u), -Vec3Dot(&position, &v), -Vec3Dot(&position, &n), 1.0f);

		//
		// 3. ��������������ϵ����׶ƽ��
		//
		Vec3MulMtx4x4(&m_frustumPlanes[PLANE_TOP][WORLD].vertex, &m_frustumPlanes[PLANE_TOP][LOCAL].vertex, &m_mtxCameraToWorld);
		Vec3MulMtx3x3(&m_frustumPlanes[PLANE_TOP][WORLD].normal, &m_frustumPlanes[PLANE_TOP][LOCAL].normal, &m_mtxCameraToWorld);
		Vec3Normalize(&m_frustumPlanes[PLANE_TOP][WORLD].normal);

		Vec3MulMtx4x4(&m_frustumPlanes[PLANE_BOTTOM][WORLD].vertex, &m_frustumPlanes[PLANE_BOTTOM][LOCAL].vertex, &m_mtxCameraToWorld);
		Vec3MulMtx3x3(&m_frustumPlanes[PLANE_BOTTOM][WORLD].normal, &m_frustumPlanes[PLANE_BOTTOM][LOCAL].normal, &m_mtxCameraToWorld);
		Vec3Normalize(&m_frustumPlanes[PLANE_BOTTOM][WORLD].normal);

		Vec3MulMtx4x4(&m_frustumPlanes[PLANE_LEFT][WORLD].vertex, &m_frustumPlanes[PLANE_LEFT][LOCAL].vertex, &m_mtxCameraToWorld);
		Vec3MulMtx3x3(&m_frustumPlanes[PLANE_LEFT][WORLD].normal, &m_frustumPlanes[PLANE_LEFT][LOCAL].normal, &m_mtxCameraToWorld);
		Vec3Normalize(&m_frustumPlanes[PLANE_LEFT][WORLD].normal);

		Vec3MulMtx4x4(&m_frustumPlanes[PLANE_RIGHT][WORLD].vertex, &m_frustumPlanes[PLANE_RIGHT][LOCAL].vertex, &m_mtxCameraToWorld);
		Vec3MulMtx3x3(&m_frustumPlanes[PLANE_RIGHT][WORLD].normal, &m_frustumPlanes[PLANE_RIGHT][LOCAL].normal, &m_mtxCameraToWorld);
		Vec3Normalize(&m_frustumPlanes[PLANE_RIGHT][WORLD].normal);

		Vec3MulMtx4x4(&m_frustumPlanes[PLANE_NEAR][WORLD].vertex, &m_frustumPlanes[PLANE_NEAR][LOCAL].vertex, &m_mtxCameraToWorld);
		Vec3MulMtx3x3(&m_frustumPlanes[PLANE_NEAR][WORLD].normal, &m_frustumPlanes[PLANE_NEAR][LOCAL].normal, &m_mtxCameraToWorld);
		Vec3Normalize(&m_frustumPlanes[PLANE_NEAR][WORLD].normal);

		Vec3MulMtx4x4(&m_frustumPlanes[PLANE_FAR][WORLD].vertex, &m_frustumPlanes[PLANE_FAR][LOCAL].vertex, &m_mtxCameraToWorld);
		Vec3MulMtx3x3(&m_frustumPlanes[PLANE_FAR][WORLD].normal, &m_frustumPlanes[PLANE_FAR][LOCAL].normal, &m_mtxCameraToWorld);
		Vec3Normalize(&m_frustumPlanes[PLANE_FAR][WORLD].normal);
	}

}

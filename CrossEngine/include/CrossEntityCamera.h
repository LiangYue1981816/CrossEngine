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

#pragma once


#include "CrossEngine.h"


namespace CrossEngine {

	class _CrossExport CEntityCamera : public CEntity
	{
		friend class CSceneManager;


		// ���ݽṹ
	public:
		static const FLOAT DEFAULT_ZFAR;                                                           // Ĭ��Զ����
		static const FLOAT DEFAULT_ZNEAR;                                                          // Ĭ�Ͻ�����


		// ����/��������
	protected:
		CEntityCamera(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntityCamera(VOID);


		// ����
	public:
		virtual TYPE GetType(VOID) const;                                                          // �������
		virtual BOOL IsRenderable(VOID) const;                                                     // ��ÿ���Ⱦ

	public:
		VOID SetPerspective(FLOAT fovy, FLOAT aspect, FLOAT zNear, FLOAT zFar);                    // ����͸��ͶӰ��׶
		VOID SetOrtho(FLOAT left, FLOAT right, FLOAT bottom, FLOAT top, FLOAT zNear, FLOAT zFar);  // ��������ͶӰ��׶

		FLOAT GetFovx(VOID) const;                                                                 // ���͸��ͶӰ��׶�н�
		FLOAT GetFovy(VOID) const;                                                                 // ���͸��ͶӰ��׶�н�
		FLOAT GetAspect(VOID) const;                                                               // ���͸��ͶӰ��׶��߱�

		FLOAT GetLeft(VOID) const;                                                                 // �������ͶӰ��׶��
		FLOAT GetRight(VOID) const;                                                                // �������ͶӰ��׶�� 
		FLOAT GetTop(VOID) const;                                                                  // �������ͶӰ��׶��
		FLOAT GetBottom(VOID) const;                                                               // �������ͶӰ��׶��

		FLOAT GetZNear(VOID) const;                                                                // �����׶���������
		FLOAT GetZFar(VOID) const;                                                                 // �����׶Զ�������

	public:
		VOID SetShake(FLOAT x, FLOAT y, FLOAT z);                                                  // ���������
		VOID SetPosition(FLOAT x, FLOAT y, FLOAT z);                                               // �������λ��
		VOID SetForwardDirection(FLOAT x, FLOAT y, FLOAT z);                                       // �������ǰ����
		VOID SetUpDirection(FLOAT x, FLOAT y, FLOAT z);                                            // �������������

		const VEC3* GetShake(VOID) const;                                                          // ��������
		const VEC3* GetPosition(VOID) const;                                                       // ������λ��
		const VEC3* GetForwardDirection(VOID) const;                                               // ������ǰ����
		const VEC3* GetUpDirection(VOID) const;                                                    // ������������

	public:
		RAY* GetRay(RAY *ray, FLOAT scrx, FLOAT scry) const;                                       // �������

		VEC3* WorldToScreen(VEC3 *screen, const VEC3 *world) const;                                // ��������ϵ�任����Ļ����ϵ
		VEC3* ScreenToWorld(VEC3 *world, const VEC2 *screen) const;                                // ��Ļ����ϵ�任����������ϵ

		const MATRIX4* GetProjectionMatrix(VOID) const;                                            // ���ͶӰ�任����
		const MATRIX4* GetCameraToWorldMatrix(VOID) const;                                         // ����������ϵ����������ϵ�任����
		const MATRIX4* GetWorldToCameraMatrix(VOID) const;                                         // �����������ϵ���������ϵ�任����

	public:
		BOOL IsVisible(const AABB *pAABB) const;                                                   // ��Χ�пɼ��Լ��
		BOOL IsVisible(const VEC3 *pVertex) const;                                                 // ����ɼ��Լ��

	public:
		VOID UpdateInstance(VOID);                                                                 // ����


		// ����
	protected:
		FLOAT m_fovx;                                                                              // ͸��ͶӰ��׶X����н�
		FLOAT m_fovy;                                                                              // ͸��ͶӰ��׶Y����н�
		FLOAT m_aspect;                                                                            // ͸��ͶӰ��׶��߱�

		FLOAT m_left;                                                                              // ����ͶӰ��׶��
		FLOAT m_right;                                                                             // ����ͶӰ��׶��
		FLOAT m_top;                                                                               // ����ͶӰ��׶��
		FLOAT m_bottom;                                                                            // ����ͶӰ��׶��

		FLOAT m_zNear;                                                                             // ��׶���������
		FLOAT m_zFar;                                                                              // ��׶Զ�������

	protected:
		VEC3 m_shake;                                                                              // �����
		VEC3 m_position;                                                                           // ���λ��
		VEC3 m_forwardDirection;                                                                   // ���ǰ����
		VEC3 m_upDirection;                                                                        // ���������

	protected:
		enum { LOCAL = 0, WORLD = 1 };
		enum { PLANE_TOP = 0, PLANE_BOTTOM = 1, PLANE_LEFT = 2, PLANE_RIGHT = 3, PLANE_NEAR = 4, PLANE_FAR = 5, PLANE_COUNT };

		PLANE m_frustumPlanes[PLANE_COUNT][2];                                                     // ��׶����

	protected:
		MATRIX4 m_mtxProjection;                                                                   // ͶӰ����
		MATRIX4 m_mtxCameraToWorld;                                                                // �������ϵ����������ϵ�任����
		MATRIX4 m_mtxWorldToCamera;                                                                // ��������ϵ���������ϵ�任����
	};

}

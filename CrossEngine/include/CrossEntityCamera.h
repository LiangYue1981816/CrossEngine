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


		// 数据结构
	public:
		static const FLOAT DEFAULT_ZFAR;                                                           // 默认远截面
		static const FLOAT DEFAULT_ZNEAR;                                                          // 默认近截面


		// 构造/析构函数
	protected:
		CEntityCamera(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntityCamera(VOID);


		// 方法
	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型
		virtual BOOL IsRenderable(VOID) const;                                                     // 获得可渲染

	public:
		VOID SetPerspective(FLOAT fovy, FLOAT aspect, FLOAT zNear, FLOAT zFar);                    // 设置透视投影视锥
		VOID SetOrtho(FLOAT left, FLOAT right, FLOAT bottom, FLOAT top, FLOAT zNear, FLOAT zFar);  // 设置正交投影视锥

		FLOAT GetFovx(VOID) const;                                                                 // 获得透视投影视锥夹角
		FLOAT GetFovy(VOID) const;                                                                 // 获得透视投影视锥夹角
		FLOAT GetAspect(VOID) const;                                                               // 获得透视投影视锥宽高比

		FLOAT GetLeft(VOID) const;                                                                 // 获得正交投影视锥左
		FLOAT GetRight(VOID) const;                                                                // 获得正交投影视锥右 
		FLOAT GetTop(VOID) const;                                                                  // 获得正交投影视锥上
		FLOAT GetBottom(VOID) const;                                                               // 获得正交投影视锥下

		FLOAT GetZNear(VOID) const;                                                                // 获得视锥近截面距离
		FLOAT GetZFar(VOID) const;                                                                 // 获得视锥远截面距离

	public:
		VOID SetShake(FLOAT x, FLOAT y, FLOAT z);                                                  // 设置相机震动
		VOID SetPosition(FLOAT x, FLOAT y, FLOAT z);                                               // 设置相机位置
		VOID SetForwardDirection(FLOAT x, FLOAT y, FLOAT z);                                       // 设置相机前向量
		VOID SetUpDirection(FLOAT x, FLOAT y, FLOAT z);                                            // 设置相机上向量

		const VEC3* GetShake(VOID) const;                                                          // 获得相机震动
		const VEC3* GetPosition(VOID) const;                                                       // 获得相机位置
		const VEC3* GetForwardDirection(VOID) const;                                               // 获得相机前向量
		const VEC3* GetUpDirection(VOID) const;                                                    // 获得相机上向量

	public:
		RAY* GetRay(RAY *ray, FLOAT scrx, FLOAT scry) const;                                       // 获得射线

		VEC3* WorldToScreen(VEC3 *screen, const VEC3 *world) const;                                // 世界坐标系变换到屏幕坐标系
		VEC3* ScreenToWorld(VEC3 *world, const VEC2 *screen) const;                                // 屏幕坐标系变换到世界坐标系

		const MATRIX4* GetProjectionMatrix(VOID) const;                                            // 获得投影变换矩阵
		const MATRIX4* GetCameraToWorldMatrix(VOID) const;                                         // 获得相机坐标系到世界坐标系变换矩阵
		const MATRIX4* GetWorldToCameraMatrix(VOID) const;                                         // 获得世界坐标系到相机坐标系变换矩阵

	public:
		BOOL IsVisible(const AABB *pAABB) const;                                                   // 包围盒可见性检查
		BOOL IsVisible(const VEC3 *pVertex) const;                                                 // 顶点可见性检查

	public:
		VOID UpdateInstance(VOID);                                                                 // 更新


		// 属性
	protected:
		FLOAT m_fovx;                                                                              // 透视投影视锥X方向夹角
		FLOAT m_fovy;                                                                              // 透视投影视锥Y方向夹角
		FLOAT m_aspect;                                                                            // 透视投影视锥宽高比

		FLOAT m_left;                                                                              // 正交投影视锥左
		FLOAT m_right;                                                                             // 正交投影视锥右
		FLOAT m_top;                                                                               // 正交投影视锥上
		FLOAT m_bottom;                                                                            // 正交投影视锥下

		FLOAT m_zNear;                                                                             // 视锥近截面距离
		FLOAT m_zFar;                                                                              // 视锥远截面距离

	protected:
		VEC3 m_shake;                                                                              // 相机震动
		VEC3 m_position;                                                                           // 相机位置
		VEC3 m_forwardDirection;                                                                   // 相机前向量
		VEC3 m_upDirection;                                                                        // 相机上向量

	protected:
		enum { LOCAL = 0, WORLD = 1 };
		enum { PLANE_TOP = 0, PLANE_BOTTOM = 1, PLANE_LEFT = 2, PLANE_RIGHT = 3, PLANE_NEAR = 4, PLANE_FAR = 5, PLANE_COUNT };

		PLANE m_frustumPlanes[PLANE_COUNT][2];                                                     // 视锥截面

	protected:
		MATRIX4 m_mtxProjection;                                                                   // 投影矩阵
		MATRIX4 m_mtxCameraToWorld;                                                                // 相机坐标系到世界坐标系变换矩阵
		MATRIX4 m_mtxWorldToCamera;                                                                // 世界坐标系到相机坐标系变换矩阵
	};

}

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

	class _CrossExport CEntityLight : public CEntity
	{
		friend class CSceneManager;


		// 数据结构
	public:
		typedef enum { LIGHT_POINT, LIGHT_DIRECTIONAL } LIGHT_TYPE;                                // 光源类型


		// 构造/析构函数
	protected:
		CEntityLight(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntityLight(VOID);


		// 方法
	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型
		virtual BOOL IsRenderable(VOID) const;                                                     // 获得可渲染

	public:
		VOID SetLightType(LIGHT_TYPE type);                                                        // 设置类型
		VOID SetRadius(FLOAT radius);                                                              // 设置半径
		VOID SetAttenuation(FLOAT k0, FLOAT k1, FLOAT k2);                                         // 设置衰减
		VOID SetAmbient(FLOAT r, FLOAT g, FLOAT b);                                                // 设置环境光
		VOID SetDiffuse(FLOAT r, FLOAT g, FLOAT b);                                                // 设置漫反射光
		VOID SetSpecular(FLOAT r, FLOAT g, FLOAT b);                                               // 设置镜面光
		VOID SetRim(FLOAT r, FLOAT g, FLOAT b);                                                    // 设置轮廓光
		VOID SetSkyLower(FLOAT r, FLOAT g, FLOAT b);                                               // 设置天光
		VOID SetSkyUpper(FLOAT r, FLOAT g, FLOAT b);                                               // 设置天光

		LIGHT_TYPE GetLightType(VOID) const;                                                       // 获得类型
		FLOAT GetRadius(VOID) const;                                                               // 获得半径
		const FLOAT* GetAttenuation(VOID) const;                                                   // 获得衰减
		const FLOAT* GetAmbient(VOID) const;                                                       // 获得环境光
		const FLOAT* GetDiffuse(VOID) const;                                                       // 获得漫反射光
		const FLOAT* GetSpecular(VOID) const;                                                      // 获得镜面光
		const FLOAT* GetRim(VOID) const;                                                           // 获得轮廓光
		const FLOAT* GetSkyLower(VOID) const;                                                      // 获得天光
		const FLOAT* GetSkyUpper(VOID) const;                                                      // 获得天光

	public:
		const AABB* GetAABB(VOID);                                                                 // 获得包围盒

	public:
		VOID UpdateRenderQuene(const CEntityCamera *pCamera);                                      // 更新渲染队列
		VOID RenderQuery(VOID) const;                                                              // 渲染查询
		VOID RenderLighting(VOID) const;                                                           // 渲染光照

	public:
		UINT GetQuery(VOID) const;                                                                 // 获得遮挡查询


		// 属性
	protected:
		AABB m_aabbLocal;                                                                          // 包围盒
		AABB m_aabbWorld;                                                                          // 包围盒

	protected:
		LIGHT_TYPE m_type;                                                                         // 光源类型
		FLOAT m_radius;                                                                            // 半径
		FLOAT m_attenuation[3];                                                                    // 衰减
		FLOAT m_ambient[3];                                                                        // 环境光
		FLOAT m_diffuse[3];                                                                        // 漫反射光
		FLOAT m_specular[3];                                                                       // 镜面光
		FLOAT m_rim[3];                                                                            // 轮廓光
		FLOAT m_skyLower[3];                                                                       // 天光
		FLOAT m_skyUpper[3];                                                                       // 天光

	protected:
		UINT m_query;                                                                              // 遮挡查询
	};

}

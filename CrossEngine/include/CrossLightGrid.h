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

	class _CrossExport CLightGrid
	{
		friend class CSceneManager;


		static const DWORD LIGHT_CHANNEL = 0x00000001;                                             // 光照通道


		// 数据结构
	public:
		typedef struct {
			BYTE direction[2];                                                                     // 朝向
			BYTE ambient[3];                                                                       // 环境光照
			BYTE diffuse[3];                                                                       // 漫反射光照
			BYTE indirectup[3];                                                                    // 间接光照上
			BYTE indirectdown[3];                                                                  // 间接光照下
			BYTE indirectleft[3];                                                                  // 间接光照左
			BYTE indirectright[3];                                                                 // 间接光照右
			BYTE indirectfront[3];                                                                 // 间接光照前
			BYTE indirectback[3];                                                                  // 间接光照后
		} LIGHT_POINT;

		typedef struct {
			AABB aabb;                                                                             // 包围体区域

			FLOAT stepx;                                                                           // X方向步进
			FLOAT stepy;                                                                           // Y方向步进
			FLOAT stepz;                                                                           // Z方向步进

			LIGHT_POINT ***pppLightPoints;                                                         // 光点集合
		} LIGHT_VOLUME;

		typedef std::vector<LIGHT_VOLUME*> LightVolumeSet;                                         // 光照体集合


		// 构造/析构函数
	protected:
		CLightGrid(CSceneManager *pSceneManager);
		virtual ~CLightGrid(VOID);


		// 方法
	public:
		VOID Init(VOID);                                                                           // 初始化
		VOID Free(VOID);                                                                           // 释放

	public:
		BOOL LoadFromStream(CStream *pStream);                                                     // 从数据流加载资源

	public:
		BOOL GetLightParams(DWORD dwChannel, const VEC3 *position, VEC4 *direction, VEC3 *ambient, VEC3 *diffuse, VEC3 *specular, VEC3 *rim, VEC3 *skyLower, VEC3 *skyUpper, VEC3 *indirectUp, VEC3 *indirectDown, VEC3 *indirectLeft, VEC3 *indirectRight, VEC3 *indirectFront, VEC3 *indirectBack) const; // 获得光照参数


		// 属性
	protected:
		LightVolumeSet m_volumes;                                                                  // 光照体集合

	protected:
		CSceneManager *m_pSceneManager;                                                            // 场景管理器
	};

}

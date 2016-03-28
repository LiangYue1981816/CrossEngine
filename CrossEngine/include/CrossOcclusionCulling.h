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

	class _CrossExport COcclusionCulling
	{
		friend class CSceneManager;


		// 数据结构
	public:
		typedef struct {
			BYTE *pvs;                                                                             // 潜在可视集
		} CELL;

		typedef struct {
			AABB aabb;                                                                             // 包围体区域

			FLOAT stepx;                                                                           // X方向步进
			FLOAT stepy;                                                                           // Y方向步进
			FLOAT stepz;                                                                           // Z方向步进

			CELL ***pppCells;                                                                      // 潜在可视集单元格集合
		} VISIBLE_VOLUME;

		typedef std::vector<VISIBLE_VOLUME*> VisibleVolumeSet;                                     // 可视体集合


		// 构造/析构函数
	protected:
		COcclusionCulling(CSceneManager *pSceneManager);
		virtual ~COcclusionCulling(VOID);


		// 方法
	public:
		VOID Init(VOID);                                                                           // 初始化
		VOID Free(VOID);                                                                           // 释放

	public:
		BOOL LoadFromStream(CStream *pStream);                                                     // 从数据流加载资源

	public:
		BOOL IsOccluded(INT index) const;                                                          // 遮挡判断
		VOID UpdateCulling(const CEntityCamera *pCamera) const;                                    // 更新剔除


		// 属性
	protected:
		BYTE *m_pvs;                                                                               // 当前潜在可视集
		VisibleVolumeSet m_volumes;                                                                // 可视体集合

	protected:
		CSceneManager *m_pSceneManager;                                                            // 场景管理器
	};

}

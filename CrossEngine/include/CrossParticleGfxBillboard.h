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

	class _CrossExport CGfxBillboard : public CGfxBase
	{
		friend class CParticleSystem;


		// 数据结构
	public:
		typedef enum {
			DIRECTION_CAMERA,                                                                      // 朝向摄像机
			DIRECTION_FIXED                                                                        // 固定朝向
		} DIRECTION_TYPE;


		// 构造/析构函数
	protected:
		CGfxBillboard(VOID);
		virtual ~CGfxBillboard(VOID);


		// 方法
	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型

		virtual VOID Init(VOID);                                                                   // 初始化
		virtual VOID Free(VOID);                                                                   // 释放

	public:
		virtual VOID LoadXML(TiXmlNode *pGfxNode);                                                 // 加载XML
		virtual VOID SaveXML(TiXmlNode *pGfxNode) const;                                           // 保存XML

	public:
		virtual VOID SetParticleQuota(INT quota);                                                  // 设置粒子总量

	public:
		virtual VOID Update(const CEntityCamera *pCamera, CParticle *pParticleList, INT numParticles); // 更新
		virtual VOID Render(INT numParticles);                                                     // 渲染

	public:
		VOID SetOffset(FLOAT offset);                                                              // 设置偏移量
		FLOAT GetOffset(VOID) const;                                                               // 获得偏移量

		VOID SetDirectionType(DIRECTION_TYPE type);                                                // 设置朝向类型
		DIRECTION_TYPE GetDirectionType(VOID) const;                                               // 获得朝向类型


		// 属性
	protected:
		FLOAT m_offset;                                                                            // 面片偏移量
		MATRIX4 m_mtxFaceToCamera;                                                                 // 面向相机旋转矩阵
		DIRECTION_TYPE m_directionType;                                                            // 朝向类型
	};

}

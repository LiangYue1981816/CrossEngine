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

	class _CrossExport CEntityMesh : public CEntityRenderable
	{
		friend class CSceneManager;


		// 构造/析构函数
	protected:
		CEntityMesh(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntityMesh(VOID);


		// 方法
	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型
		virtual BOOL IsRenderable(VOID) const;                                                     // 获得可渲染

	public:
		virtual BOOL SetMesh(const CHAR *szName);                                                  // 设置网格
		virtual BOOL SetMesh(DWORD dwName);                                                        // 设置网格
		virtual BOOL SetMesh(const CMeshPtr &ptrMesh);                                             // 设置网格

		BOOL SetRenderSubMesh(const CHAR *szName);                                                 // 设置渲染子网格
		BOOL SetRenderSubMesh(DWORD dwName);                                                       // 设置渲染子网格

		BOOL SetPhysicsSubMesh(const CHAR *szName);                                                // 设置物理子网格
		BOOL SetPhysicsSubMesh(DWORD dwName);                                                      // 设置物理子网格

		const CMeshPtr& GetMesh(VOID) const;                                                       // 获得网格
		const CSubMesh* GetRenderSubMesh(VOID) const;                                              // 获得渲染子网格
		const CSubMesh* GetPhysicsSubMesh(VOID) const;                                             // 获得物理子网格

	public:
		BOOL SetDiffuseMaterial(const CHAR *szName);                                               // 设置漫反射材质
		BOOL SetDiffuseMaterial(DWORD dwName);                                                     // 设置漫反射材质
		BOOL SetDiffuseMaterial(const CMaterialPtr &ptrMaterial);                                  // 设置漫反射材质

		BOOL SetPhysicsMaterial(const CHAR *szName);                                               // 设置物理材质
		BOOL SetPhysicsMaterial(DWORD dwName);                                                     // 设置物理材质
		BOOL SetPhysicsMaterial(const CMaterialPtr &ptrMaterial);                                  // 设置物理材质

		BOOL SetLightMapTexture(const CHAR *szName, FLOAT offsetu = 0.0f, FLOAT offsetv = 0.0f, FLOAT scaleu = 1.0f, FLOAT scalev = 1.0f); // 设置光照贴图纹理
		BOOL SetLightMapTexture(DWORD dwName, FLOAT offsetu = 0.0f, FLOAT offsetv = 0.0f, FLOAT scaleu = 1.0f, FLOAT scalev = 1.0f); // 设置光照贴图纹理
		BOOL SetLightMapTexture(const CTexturePtr &ptrTexture, FLOAT offsetu = 0.0f, FLOAT offsetv = 0.0f, FLOAT scaleu = 1.0f, FLOAT scalev = 1.0f); // 设置光照贴图纹理

		const CMaterialPtr& GetMaterial(VOID) const;                                               // 获得材质
		const CMaterialPtr& GetPhysicsMaterial(VOID) const;                                        // 获得物理材质
		const CTexturePtr& GetLightMapTexture(VOID) const;                                         // 获得光照贴图纹理
		
	public:
		BOOL SetTextureMatrix(INT indexTexUnit, const MATRIX4 *matrix);                            // 设置纹理矩阵
		const MATRIX4* GetTextureMatrix(INT indexTexUnit) const;                                   // 获得纹理矩阵
		const MATRIX4* GetLightMapTextureMatrix(VOID) const;                                       // 获得光照贴图变换矩阵

	public:
		VOID SetEnablePhysicsQuery(BOOL bEnable);                                                  // 设置物理查询状态
		VOID SetEnablePhysicsSimulation(BOOL bEnable);                                             // 设置物理模拟状态
		VOID SetPhysicsSimulationTypeIndex(INT index);                                             // 设置物理模拟类型索引

		BOOL IsEnablePhysicsQuery(VOID) const;                                                     // 获得物理查询状态
		BOOL IsEnablePhysicsSimulation(VOID) const;                                                // 获得物理模拟状态
		INT GetPhysicsSimulationTypeIndex(VOID) const;                                             // 获得物理模拟类型索引

	public:
		virtual VOID Render(const CMaterial *pMaterial) const;                                     // 渲染
		virtual VOID RenderOcclude(VOID) const;                                                    // 渲染遮挡


		// 属性
	protected:
		DWORD m_dwRenderMeshName;                                                                  // 渲染网格名
		DWORD m_dwPhysicsMeshName;                                                                 // 物理网格名

	protected:
		CMeshPtr m_ptrMesh;                                                                        // 网格
		CMaterialPtr m_ptrDiffuseMaterial;                                                         // 漫反射材质
		CMaterialPtr m_ptrPhysicsMaterial;                                                         // 物理材质
		CTexturePtr m_ptrLightMapTexture;                                                          // 光照贴图纹理

		MATRIX4 m_mtxTextures[MAX_TEXUNITS];                                                       // 纹理矩阵

	protected:
		BOOL m_bEnablePhysicsQuery;                                                                // 物理查询状态
		BOOL m_bEnablePhysicsSimulation;                                                           // 物理模拟状态
		INT m_nPhysicsSimulationTypeIndex;                                                         // 物理模拟类型索引
	};

}

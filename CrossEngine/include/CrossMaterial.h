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

	class _CrossExport CMatPhysics
	{
		friend class CMaterial;


		// 构造/析构函数
	protected:
		CMatPhysics(CMaterial *pMaterial);
		virtual ~CMatPhysics(VOID);


		// 方法
	public:
		VOID Init(VOID);                                                                           // 初始化资源
		VOID Free(VOID);                                                                           // 释放资源

		const CMaterial* GetParentMaterial(VOID) const;                                            // 获得父材质

	public:
		VOID SetType(MATERIAL_TYPE type);                                                          // 设置类型
		MATERIAL_TYPE GetType(VOID) const;                                                         // 获得类型

	protected:
		VOID LoadXML(TiXmlNode *pPhysicsNode);                                                     // 加载XML
		VOID SaveXML(TiXmlNode *pPhysicsNode) const;                                               // 保存XML


		// 属性
	protected:
		MATERIAL_TYPE m_type;                                                                      // 类型

	protected:
		CMaterial *m_pParentMaterial;                                                              // 父材质
	};

	class _CrossExport CEntityRenderable;
	class _CrossExport CMatPass
	{
		friend class CMatPassGroup;


		// 构造/析构函数
	protected:
		CMatPass(CMaterial *pMaterial);
		virtual ~CMatPass(VOID);


		// 方法
	public:
		VOID Init(VOID);                                                                           // 初始化资源
		VOID Free(VOID);                                                                           // 释放资源

		const CMaterial* GetParentMaterial(VOID) const;                                            // 获得父材质

	public:
		CRenderer::PASS* GetPass(VOID) const;                                                      // 获得渲染通路

		VOID SetRenderID(DWORD id);                                                                // 设置渲染ID
		DWORD GetRenderID(VOID) const;                                                             // 获得渲染ID

		VOID SetEnableCullFace(BOOL bEnable);                                                      // 设置面剔除状态
		VOID SetCullFaceMode(UINT mode);                                                           // 设置面剔除模式
		VOID SetFrontFaceMode(UINT mode);                                                          // 设置面顶点顺序模式
		BOOL IsEnableCullFace(VOID) const;                                                         // 获得面剔除状态
		UINT GetCullFaceMode(VOID) const;                                                          // 获得面剔除模式
		UINT GetFrontFaceMode(VOID) const;                                                         // 获得面顶点顺序模式

		VOID SetEnableDepthTest(BOOL bEnable);                                                     // 设置深度测试状态
		VOID SetEnableDepthWrite(BOOL bEnable);                                                    // 设置写深度状态
		VOID SetDepthFunc(UINT func);                                                              // 设置深度测试方法
		BOOL IsEnableDepthTest(VOID) const;                                                        // 获得深度测试状态
		BOOL IsEnableDepthWrite(VOID) const;                                                       // 获得写深度状态
		UINT GetDepthFunc(VOID) const;                                                             // 获得深度测试方法

		VOID SetEnableAlphaTest(BOOL bEnable);                                                     // 设置透明测试状态
		VOID SetAlphaFunc(UINT func);                                                              // 设置透明测试方法
		VOID SetAlphaRef(FLOAT ref);                                                               // 设置透明测试及参数
		BOOL IsEnableAlphaTest(VOID) const;                                                        // 获得透明测试状态
		UINT GetAlphaFunc(VOID) const;                                                             // 获得透明测试方法
		FLOAT GetAlphaRef(VOID) const;                                                             // 获得透明测试参数

		VOID SetEnableBlend(BOOL bEnable);                                                         // 设置混合状态
		VOID SetBlendMode(UINT color, UINT alpha);                                                 // 设置混合模式
		VOID SetBlendFunc(UINT srcColor, UINT srcAlpha, UINT desColor, UINT desAlpha);             // 设置混合参数
		BOOL IsEnableBlend(VOID) const;                                                            // 获得混合状态
		UINT GetBlendModeColor(VOID) const;                                                        // 获得颜色混合模式
		UINT GetBlendModeAlpha(VOID) const;                                                        // 获得透明混合模式
		UINT GetBlendFuncSrcColor(VOID) const;                                                     // 获得颜色混合参数
		UINT GetBlendFuncSrcAlpha(VOID) const;                                                     // 获得透明混合参数
		UINT GetBlendFuncDesColor(VOID) const;                                                     // 获得颜色混合参数
		UINT GetBlendFuncDesAlpha(VOID) const;                                                     // 获得目标混合参数

		VOID SetEnableLighting(BOOL bEnable);                                                      // 设置光照状态
		VOID SetAmbient(FLOAT r, FLOAT g, FLOAT b, FLOAT a);                                       // 设置环境光
		VOID SetDiffuse(FLOAT r, FLOAT g, FLOAT b, FLOAT a);                                       // 设置漫反射光
		VOID SetEmission(FLOAT r, FLOAT g, FLOAT b, FLOAT a);                                      // 设置自发光
		VOID SetSpecular(FLOAT r, FLOAT g, FLOAT b, FLOAT a);                                      // 设置高光
		VOID SetShininess(FLOAT shininess);                                                        // 设置高光级别
		BOOL IsEnableLighting(VOID) const;                                                         // 获得光照状态
		const FLOAT* GetAmbient(VOID) const;                                                       // 获得环境光
		const FLOAT* GetDiffuse(VOID) const;                                                       // 获得漫反射光
		const FLOAT* GetEmission(VOID) const;                                                      // 获得自发光
		const FLOAT* GetSpecular(VOID) const;                                                      // 获得高光
		FLOAT GetShininess(VOID) const;                                                            // 获得高光级别

		BOOL SetTexture(INT indexTexUnit, const CHAR *szName);                                     // 设置纹理单元
		VOID SetTextureParam(INT indexTexUnit, UINT minFilter, UINT magFilter, UINT warpu, UINT warpv); // 设置纹理参数
		VOID SetTextureScrollU(INT indexTexUnit, FLOAT scrollu);                                   // 设置纹理滚动
		VOID SetTextureScrollV(INT indexTexUnit, FLOAT scrollv);                                   // 设置纹理滚动
		VOID SetTextureSequence(INT indexTexUnit, INT cols, INT rows, FLOAT duration);             // 设置纹理动画
		const CTexturePtr& GetTexture(INT indexTexUnit) const;                                     // 获得纹理
		UINT GetTextureMinFilter(INT indexTexUnit) const;                                          // 获得纹理参数
		UINT GetTextureMagFilter(INT indexTexUnit) const;                                          // 获得纹理参数
		UINT GetTextureWarpU(INT indexTexUnit) const;                                              // 获得纹理参数
		UINT GetTextureWarpV(INT indexTexUnit) const;                                              // 获得纹理参数
		FLOAT GetTextureScrollU(INT indexTexUnit) const;                                           // 获得纹理滚动参数
		FLOAT GetTextureScrollV(INT indexTexUnit) const;                                           // 获得纹理滚动参数
		INT GetTextureSequenceCols(INT indexTexUnit) const;                                        // 获得纹理序列帧参数
		INT GetTextureSequenceRows(INT indexTexUnit) const;                                        // 获得纹理序列帧参数
		FLOAT GetTextureSequenceDuration(INT indexTexUnit) const;                                  // 获得纹理序列帧参数

		BOOL SetShaderVertex(const CHAR *szName);                                                  // 设置顶点着色器
		BOOL SetShaderFragment(const CHAR *szName);                                                // 设置片源着色器
		const CShaderPtr& GetShaderVertex(VOID) const;                                             // 获得顶点着色器
		const CShaderPtr& GetShaderFragment(VOID) const;                                           // 获得片源着色器

	public:
		UINT64 Evaluate(VOID) const;                                                               // 计算评价值
		BOOL IsTransparence(VOID) const;                                                           // 获得透明状态

	protected:
		VOID LoadXML(TiXmlNode *pPassNode);                                                        // 加载XML
		VOID LoadRenderID(TiXmlNode *pPassNode);                                                   // 加载渲染ID
		VOID LoadLighting(TiXmlNode *pPassNode);                                                   // 加载光照参数
		VOID LoadCull(TiXmlNode *pPassNode);                                                       // 加载剔除参数
		VOID LoadBlend(TiXmlNode *pPassNode);                                                      // 加载混合参数
		VOID LoadDepth(TiXmlNode *pPassNode);                                                      // 加载深度参数
		VOID LoadAlpha(TiXmlNode *pPassNode);                                                      // 加载透明参数
		VOID LoadTextureUnits(TiXmlNode *pPassNode);                                               // 加载纹理单元
		VOID LoadShaderVertex(TiXmlNode *pPassNode);                                               // 加载顶点着色器
		VOID LoadShaderFragment(TiXmlNode *pPassNode);                                             // 加载片段着色器
		VOID LoadShaderParams(TiXmlNode *pShaderNode);                                             // 加载着色器参数
		VOID SetShaderParams(const CRenderer::UniformLocationMap& params);                         // 设置着色器参数
		VOID CreateProgram(VOID);                                                                  // 创建着色器程序

	protected:
		VOID SaveXML(TiXmlNode *pPassNode) const;                                                  // 保存XML
		VOID SaveRenderID(TiXmlNode *pPassNode) const;                                             // 保存渲染ID
		VOID SaveLighting(TiXmlNode *pPassNode, const CRenderer::PASS_LIGHTING *pDefault) const;   // 保存光照参数
		VOID SaveCull(TiXmlNode *pPassNode, const CRenderer::PASS_CULL *pDefault) const;           // 保存剔除参数
		VOID SaveBlend(TiXmlNode *pPassNode, const CRenderer::PASS_BLEND *pDefault) const;         // 保存混合参数
		VOID SaveDepth(TiXmlNode *pPassNode, const CRenderer::PASS_DEPTH *pDefault) const;         // 保存深度参数
		VOID SaveAlpha(TiXmlNode *pPassNode, const CRenderer::PASS_ALPHA *pDefault) const;         // 保存透明参数
		VOID SaveTextureUnits(TiXmlNode *pPassNode) const;                                         // 保存纹理单元
		VOID SaveShaderVertex(TiXmlNode *pPassNode) const;                                         // 保存顶点着色器
		VOID SaveShaderFragment(TiXmlNode *pPassNode) const;                                       // 保存片段着色器
		VOID SaveShaderParams(TiXmlNode *pShaderNode, const CRenderer::UniformLocationMap& refParams) const; // 保存着色器参数


		// 属性
	protected:
		DWORD m_id;                                                                                // 渲染ID
		CRenderer::PASS m_pass;                                                                    // 渲染通路

	protected:
		CShaderPtr m_ptrShaderVertex;                                                              // 顶点着色器
		CShaderPtr m_ptrShaderFragment;                                                            // 像素着色器
		CTexturePtr m_ptrTextures[MAX_TEXUNITS];                                                   // 纹理

	protected:
		CMaterial *m_pParentMaterial;                                                              // 父材质

		// 渲染链表
	public:
		CMatPass *pRenderNext;                                                                     // 通路链表
		CEntityRenderable *pEntityHead;                                                            // 实体渲染链表头
	};

	class _CrossExport CMatPassGroup
	{
		friend class CMatSolution;


		// 数据结构
	public:
		typedef std::vector<CMatPass*> PassSet;                                                    // 渲染通路集合


		// 构造/析构函数
	protected:
		CMatPassGroup(const CHAR *szName, CMaterial *pMaterial);
		virtual ~CMatPassGroup(VOID);


		// 方法
	public:
		VOID Init(VOID);                                                                           // 初始化
		VOID Init(const CHAR *szName);                                                             // 初始化资源
		VOID Free(VOID);                                                                           // 释放资源

		const CHAR* GetName(VOID) const;                                                           // 获得名称
		const CMaterial* GetParentMaterial(VOID) const;                                            // 获得父材质

	public:
		INT GetPassCount(VOID) const;                                                              // 获得渲染通路数
		const CMatPass* GetPass(INT index) const;                                                  // 获得渲染通路
		const PassSet& GetPasses(VOID) const;                                                      // 获得渲染通路集合

	protected:
		VOID LoadXML(TiXmlNode *pPassGroupNode);                                                   // 加载XML
		VOID SaveXML(TiXmlNode *pPassGroupNode) const;                                             // 保存XML


		// 属性
	protected:
		CHAR m_szName[_MAX_STRING];                                                                // 渲染通路集合名
		PassSet m_passes;                                                                          // 渲染通路集合

	protected:
		CMaterial *m_pParentMaterial;                                                              // 父材质
	};

	class _CrossExport CMatSolution
	{
		friend class CMaterial;


		// 数据结构
	public:
		typedef std::map<DWORD, CMatPassGroup*> PassGroupMap;                                      // 渲染通路组集合


		// 构造/析构函数
	protected:
		CMatSolution(const CHAR *szName, CMaterial *pMaterial);
		virtual ~CMatSolution(VOID);


		// 方法
	public:
		VOID Init(VOID);                                                                           // 初始化
		VOID Init(const CHAR *szName);                                                             // 初始化资源
		VOID Free(VOID);                                                                           // 释放资源

		const CHAR* GetName(VOID) const;                                                           // 获得名称
		const CMaterial* GetParentMaterial(VOID) const;                                            // 获得父材质

	public:
		const CMatPassGroup* GetPassGroup(const CHAR *szName) const;                               // 获得渲染通路组
		const CMatPassGroup* GetPassGroup(DWORD dwName) const;                                     // 获得渲染通路组
		const PassGroupMap& GetPassGroups(VOID) const;                                             // 获得渲染通路组集合

	protected:
		VOID LoadXML(TiXmlNode *pSolutionNode);                                                    // 加载XML
		VOID SaveXML(TiXmlNode *pSolutionNode) const;                                              // 保存XML


		// 属性
	protected:
		CHAR m_szName[_MAX_STRING];                                                                // 材质方案名
		PassGroupMap m_groups;                                                                     // 渲染通路组集合

	protected:
		CMaterial *m_pParentMaterial;                                                              // 父材质
	};

	class _CrossExport CMaterial : public CResource
	{
		friend class CMaterialPtr;
		friend class CMaterialManager;


		// 数据结构
	public:
		typedef std::map<DWORD, CMatSolution*> SolutionMap;                                        // 解决方案集合


		// 构造/析构函数
	protected:
		CMaterial(CResourceManager *pResourceManager);
		virtual ~CMaterial(VOID);


		// 方法
	public:
		virtual VOID Init(VOID);                                                                   // 初始化资源
		virtual VOID Free(VOID);                                                                   // 释放资源

	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型
		virtual BOOL IsValid(VOID) const;                                                          // 有效性检查

	public:
		virtual BOOL LoadFromStream(CStream *pStream);                                             // 从数据流加载资源
		virtual BOOL SaveToFileStream(FILE *pFile);                                                // 保存到文件数据流

	protected:
		VOID LoadPhysics(TiXmlNode *pMaterialNode);                                                // 加载物理
		VOID LoadSolutions(TiXmlNode *pMaterialNode);                                              // 加载材质方案

		VOID SavePhysics(TiXmlNode *pMaterialNode);                                                // 保存物理
		VOID SaveSolutions(TiXmlNode *pMaterialNode);                                              // 保存渲染方案

	public:
		const CMatPhysics* GetPhysics(VOID) const;                                                 // 获得物理
		const CMatSolution* GetSolution(const CHAR *szName) const;                                 // 获得材质方案
		const CMatSolution* GetSolution(DWORD dwName) const;                                       // 获得材质方案
		const SolutionMap& GetSolutions(VOID) const;                                               // 获得材质方案集合


		// 属性
	protected:
		CMatPhysics m_physics;                                                                     // 物理属性
		SolutionMap m_solutions;                                                                   // 方案集合
	};

	class _CrossExport CMaterialPtr : public CSharedPtr<CMaterial>
	{
		// 构造/析构函数
	public:
		CMaterialPtr(VOID) : CSharedPtr<CMaterial>() {}
		CMaterialPtr(const CMaterial *pMaterial) : CSharedPtr<CMaterial>(pMaterial) {}
		CMaterialPtr(const CMaterialPtr &ptrMaterial) : CSharedPtr<CMaterial>(ptrMaterial) {}
		CMaterialPtr(const CResourcePtr &ptrResource) { Set(static_cast<CMaterial*>(ptrResource.GetPointer()), ptrResource.GetRefCount()); }
		virtual ~CMaterialPtr(VOID) {}


		// 方法
	protected:
		virtual VOID FreePointer(VOID)
		{
			if (m_pPointer) {
				m_pPointer->GetResourceManager()->DestroyResource(m_pPointer);
			}
		}

	public:
		CMaterialPtr& operator = (const CResourcePtr &ptrResource)
		{
			Set(static_cast<CMaterial*>(ptrResource.GetPointer()), ptrResource.GetRefCount());
			return *this;
		}

		operator CResource* ()
		{
			return (CResource *)m_pPointer;
		}

		operator const CResource* () const
		{
			return (const CResource *)m_pPointer;
		}
	};

}

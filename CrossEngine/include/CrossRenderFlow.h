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

	class _CrossExport CRenderStage
	{
		friend class CRenderFlow;
		friend class CRenderSolution;


		// 数据结构
	public:
		typedef enum { SCENE, LIGHT, SCREEN, SHADOW_CAST, SHADOW_RECEIVE } RENDERSTATE_TYPE;       // 渲染过程类型
		typedef enum { DEPTH, COLOR } ATTACHMENT_TYPE;                                             // 纹理挂接类型

		typedef struct {
			DWORD dwCategoryName;                                                                  // 类型名
			DWORD dwTextureName;                                                                   // 纹理名

			UINT channel;                                                                          // 通道
			UINT minFilter;                                                                        // 滤波方式
			UINT magFilter;                                                                        // 滤波方式
			UINT wrapu;                                                                            // 纹理坐标环绕方式
			UINT wrapv;                                                                            // 纹理坐标环绕方式

			ATTACHMENT_TYPE attachmentType;                                                        // 纹理挂接类型
		} TEXTURE;

		typedef std::vector<TEXTURE*> TextureSet;                                                  // 纹理集合


		// 构造/析构函数
	protected:
		CRenderStage(CRenderFlow *pRenderFlow);
		virtual ~CRenderStage(VOID);


		// 方法
	protected:
		VOID Init(VOID);                                                                           // 初始化
		VOID Free(VOID);                                                                           // 释放

	protected:
		VOID LoadFromStream(TiXmlNode *pRenderStageNode);                                          // 加载渲染过程
		VOID LoadName(TiXmlNode *pRenderStageNode);                                                // 加载名称
		VOID LoadType(TiXmlNode *pRenderStageNode);                                                // 加载类型
		VOID LoadEnable(TiXmlNode *pRenderStageNode);                                              // 加载状态
		VOID LoadCallback(TiXmlNode *pRenderStageNode);                                            // 加载回调
		VOID LoadClearBuffer(TiXmlNode *pRenderStageNode);                                         // 加载清理缓存
		VOID LoadMaterial(TiXmlNode *pRenderStageNode);                                            // 加载材质
		VOID LoadInputTextures(TiXmlNode *pRenderStageNode);                                       // 加载输入纹理
		VOID LoadOutputTextures(TiXmlNode *pRenderStageNode);                                      // 加载输出纹理
		VOID LoadShaderVertex(TiXmlNode *pRenderStageNode);                                        // 加载顶点着色器
		VOID LoadShaderFragment(TiXmlNode *pRenderStageNode);                                      // 加载片段着色器
		VOID LoadShaderParams(TiXmlNode *pShaderNode);                                             // 加载着色器参数
		VOID SetShaderParams(const CRenderer::UniformLocationMap& params);                         // 设置着色器参数

	protected:
		VOID Create(VOID);                                                                         // 创建
		VOID Destroy(VOID);                                                                        // 销毁

	public:
		VOID SetEnable(BOOL bEnable);                                                              // 设置启用状态
		BOOL IsEnable(VOID) const;                                                                 // 获得启用状态

		const CHAR* GetName(VOID) const;                                                           // 获得名称
		DWORD GetRenderTime(VOID) const;                                                           // 获得渲染时间

		VOID Render(DWORD dwSceneManager);                                                         // 渲染

	protected:
		VOID Begin(VOID);                                                                          // 开始渲染
		VOID End(VOID);                                                                            // 结束渲染
		VOID RenderScene(const CEntityCamera *pCamera);                                            // 渲染场景
		VOID RenderCastShadow(const CEntityShadow *pShadow);                                       // 渲染产生阴影
		VOID RenderReceiveShadow(const CEntityCamera *pCamera, const CEntityShadow *pShadow);      // 渲染接收阴影
		VOID RenderLighting(const CEntityCamera *pCamera);                                         // 渲染光照
		VOID RenderScreen(VOID);                                                                   // 渲染屏幕


		// 属性
	protected:
		BOOL m_bEnable;                                                                            // 状态
		CHAR m_szName[_MAX_STRING];                                                                // 名称
		RENDERSTATE_TYPE m_type;                                                                   // 类型

	protected:
		DWORD m_dwBeginRenderCallback;                                                             // 渲染前回调函数名
		DWORD m_dwEndRenderCallback;                                                               // 渲染后回调函数名

		FLOAT m_clearDepth;                                                                        // 清深度值
		FLOAT m_clearColor[3];                                                                     // 清颜色值

		BOOL m_bClearColorBuffer;                                                                  // 清空颜色缓冲
		BOOL m_bClearDepthBuffer;                                                                  // 清空深度缓冲

		DWORD m_dwMatBeginRenderID;                                                                // 材质渲染开始ID
		DWORD m_dwMatEndRenderID;                                                                  // 材质渲染结束ID
		DWORD m_dwMatSolution;                                                                     // 材质渲染方案名
		DWORD m_dwMatPassGroup;                                                                    // 材质渲染通路组名

		TextureSet m_inputTextures;                                                                // 输入纹理
		TextureSet m_outputTextures;                                                               // 输出纹理

		UINT m_vertex;                                                                             // 顶点着色器
		UINT m_fragment;                                                                           // 片断着色器
		CRenderer::UniformLocationMap m_params;                                                    // 着色器参数集合

		UINT m_frameBuffer;                                                                        // 帧缓冲
		UINT m_frameBufferWidth;                                                                   // 帧缓冲宽
		UINT m_frameBufferHeight;                                                                  // 帧缓冲高

		UINT m_numDraws;                                                                           // 多路输出数
		UINT m_drawBuffers[MAX_DRAW_BUFFERS];                                                      // 多路输出缓冲

	protected:
		INT m_lastViewportX;                                                                       // 视口位置X
		INT m_lastViewportY;                                                                       // 视口位置Y
		INT m_lastViewportWidth;                                                                   // 视口宽
		INT m_lastViewportHeight;                                                                  // 视口高
		INT m_lastFrameBuffer;                                                                     // FBO

	protected:
		DWORD m_dwRenderTime;                                                                      // 渲染时间

	protected:
		CRenderFlow *m_pRenderFlow;                                                                // 渲染流程
	};

	class _CrossExport CRenderTarget
	{
		friend class CRenderFlow;


		// 数据结构
	public:
		typedef struct {
			UINT texture;                                                                          // 纹理
			UINT type;                                                                             // 数据类型
			UINT format;                                                                           // 纹理格式
			UINT internalformat;                                                                   // 内部纹理格式
		} TEXTURE_TARGET;

		typedef std::map<DWORD, TEXTURE_TARGET*> TextureTargetMap;                                 // 纹理渲染对象集合


		// 构造/析构函数
	protected:
		CRenderTarget(CRenderFlow *pRenderFlow);
		virtual ~CRenderTarget(VOID);


		// 方法
	protected:
		VOID Init(VOID);                                                                           // 初始化
		VOID Free(VOID);                                                                           // 释放

	protected:
		VOID LoadFromStream(TiXmlNode *pRenderTargetNode);                                         // 加载渲染对象
		VOID LoadResolution(TiXmlNode *pRenderTargetNode);                                         // 加载分辨率
		VOID LoadTextureTargets(TiXmlNode *pRenderTargetNode);                                     // 加载纹理对象

	protected:
		VOID Create(VOID);                                                                         // 创建
		VOID Destroy(VOID);                                                                        // 销毁

	public:
		INT GetWidth(VOID) const;                                                                  // 获得宽
		INT GetHeight(VOID) const;                                                                 // 获得高

		UINT GetDepthRenderBuffer(VOID) const;                                                     // 获得深度渲染缓冲
		TEXTURE_TARGET* GetTextureTarget(const CHAR *szName) const;                                // 获得纹理渲染对象
		TEXTURE_TARGET* GetTextureTarget(DWORD dwName) const;                                      // 获得纹理渲染对象


		// 属性
	protected:
		INT m_width;                                                                               // 纹理渲染对象宽
		INT m_height;                                                                              // 纹理渲染对象高

		UINT m_depthRenderBuffer;                                                                  // 深度渲染缓冲
		TextureTargetMap m_textureTargets;                                                         // 纹理渲染对象

	protected:
		CRenderFlow *m_pRenderFlow;                                                                // 渲染流程
	};

	class _CrossExport CRenderSolution
	{
		friend class CRenderFlow;


		// 数据结构
	public:
		typedef std::vector<CRenderStage*> RenderStageSet;                                         // 渲染过程集合
		typedef std::map<DWORD, CRenderStage*> RenderStageMap;                                     // 渲染过程集合


		// 构造/析构函数
	protected:
		CRenderSolution(CRenderFlow *pRenderFlow);
		virtual ~CRenderSolution(VOID);


		// 方法
	protected:
		VOID Init(VOID);                                                                           // 初始化
		VOID Free(VOID);                                                                           // 释放

	protected:
		VOID LoadFromStream(TiXmlNode *pRenderSolutionNode);                                       // 加载渲染过程

	protected:
		VOID Create(VOID);                                                                         // 创建
		VOID Destroy(VOID);                                                                        // 销毁

	public:
		CRenderStage* GetRenderStage(const CHAR *szName) const;                                    // 获得渲染过程
		CRenderStage* GetRenderStage(DWORD dwName) const;                                          // 获得渲染过程

	protected:
		VOID Render(DWORD dwSceneManager);                                                         // 渲染


		// 属性
	protected:
		RenderStageSet m_renderStageSet;                                                           // 渲染过程集合
		RenderStageMap m_renderStageMap;                                                           // 渲染过程集合

	protected:
		CRenderFlow *m_pRenderFlow;                                                                // 渲染流程
	};

	typedef VOID(*RenderCallback)(CRenderStage *pRenderStage);

	class _CrossExport CRenderFlow
	{
		friend class CRenderSystem;


		// 数据结构
	public:
		typedef std::map<DWORD, CRenderTarget*> RenderTargetMap;                                   // 渲染对象集合
		typedef std::map<DWORD, CRenderSolution*> RenderSolutionMap;                               // 渲染方案集合
		typedef std::map<DWORD, RenderCallback> RenderCallbackMap;                                 // 渲染回调函数集合


		// 构造/析构函数
	protected:
		CRenderFlow(VOID);
		virtual ~CRenderFlow(VOID);


		// 方法
	public:
		VOID Init(VOID);                                                                           // 初始化资源
		VOID Free(VOID);                                                                           // 释放资源

		BOOL LoadFromFile(const CHAR *szFileName, FLOAT scalePPI = 1.0f);                          // 从文件加载资源
		BOOL LoadFromZip(const CHAR *szZipName, const CHAR *szFileName, FLOAT scalePPI = 1.0f);    // 从压缩包加载资源
		BOOL LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName, FLOAT scalePPI = 1.0f);       // 从压缩包文件加载数据流
		BOOL LoadFromStream(CStream *pStream, FLOAT scalePPI = 1.0f);                              // 从数据流加载资源

		BOOL Reload(FLOAT scalePPI = 1.0f);                                                        // 重新加载资源

	protected:
		VOID LoadRenderTargets(TiXmlNode *pRenderFlowNode);                                        // 加载渲染对象
		VOID LoadRenderSolutions(TiXmlNode *pRenderFlowNode);                                      // 加载渲染方案

	protected:
		VOID Create(VOID);                                                                         // 创建
		VOID Destroy(VOID);                                                                        // 销毁

	public:
		FLOAT GetScalePPI(VOID) const;                                                             // 获得PPI缩放比
		CRenderSolution* GetRenderSolution(DWORD dwName) const;                                    // 获得渲染方案
		CRenderTarget* GetRenderTarget(DWORD dwName) const;                                        // 获得渲染对象

	public:
		BOOL SetRenderCallback(DWORD dwName, RenderCallback fnCallback);                           // 设置渲染回调函数
		RenderCallback GetRenderCallback(DWORD dwName);                                            // 获得渲染回调函数

	public:
		VOID Render(DWORD dwSceneManager, DWORD dwSolution);                                       // 渲染


		// 属性
	protected:
		FLOAT m_scalePPI;                                                                          // PPI缩放比

		RenderTargetMap m_renderTargets;                                                           // 渲染对象集合
		RenderSolutionMap m_renderSolutions;                                                       // 渲染方案集合
		RenderCallbackMap m_renderCallbacks;                                                       // 渲染回调函数集合

	protected:
		CStream m_stream;                                                                          // 数据流
	};

}

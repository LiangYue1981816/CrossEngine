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

	typedef DWORD INDEX;                                                                           // 顶点索引

	typedef struct {
		DWORD index0;                                                                              // 三角面顶点0索引
		DWORD index1;                                                                              // 三角面顶点1索引
		DWORD index2;                                                                              // 三角面顶点2索引
	} FACE;

	typedef struct {
		VEC3 position;                                                                             // 位置
		VEC3 normal;                                                                               // 法线
		VEC3 binormal;                                                                             // 副法线
		VEC4 color;                                                                                // 颜色
		VEC2 texCoordDiffuse;                                                                      // 漫反射纹理坐标
		VEC2 texCoordLightMap;                                                                     // 光照图纹理坐标
		FLOAT indices[4];                                                                          // 蒙皮骨骼索引
		FLOAT weights[4];                                                                          // 蒙皮骨骼权重
	} VERTEX;

	class _CrossExport CRenderer
	{
		friend class CRenderSystem;


		// 顶点格式
	public:
		static const DWORD VERTEX_FORMAT_SKINMESH;                                                 // 蒙皮网格顶点格式
		static const DWORD VERTEX_FORMAT_STATICMESH;                                               // 静态网格顶点格式
		static const DWORD VERTEX_FORMAT_DYNAMICMESH;                                              // 动态网格顶点格式
		static const DWORD VERTEX_FORMAT_PARTICLE;                                                 // 粒子顶点格式
		static const DWORD VERTEX_FORMAT_HALO;                                                     // 光晕顶点格式
		static const DWORD VERTEX_FORMAT_SCREEN;                                                   // 屏幕顶点格式
		static const DWORD VERTEX_FORMAT_POSITION;                                                 // 位置顶点格式

		// 着色器数据结构
	public:
		enum {
			ATTRIB_LOCATION_POSITION = 0,                                                          // 位置属性
			ATTRIB_LOCATION_NORMAL,                                                                // 法线属性
			ATTRIB_LOCATION_BINORMAL,                                                              // 副法线属性
			ATTRIB_LOCATION_COLOR,                                                                 // 颜色属性
			ATTRIB_LOCATION_TEXCOORDDIFFUSE,                                                       // 漫反射纹理坐标属性
			ATTRIB_LOCATION_TEXCOORDLIGHTMAP,                                                      // 光照图纹理坐标属性
			ATTRIB_LOCATION_INDICES,                                                               // 蒙皮骨骼索引属性
			ATTRIB_LOCATION_WEIGHTS,                                                               // 蒙皮骨骼权重属性
			ATTRIB_LOCATION_COUNT
		};

		enum {
			UNIFORM_TYPE_UNKNOW = 0,                                                               // 未知
			UNIFORM_TYPE_INT,                                                                      // 整形
			UNIFORM_TYPE_INT2,                                                                     // 整形2
			UNIFORM_TYPE_INT3,                                                                     // 整形3
			UNIFORM_TYPE_INT4,                                                                     // 整形4
			UNIFORM_TYPE_FLOAT,                                                                    // 浮点
			UNIFORM_TYPE_FLOAT2,                                                                   // 浮点2
			UNIFORM_TYPE_FLOAT3,                                                                   // 浮点3
			UNIFORM_TYPE_FLOAT4,                                                                   // 浮点4
			UNIFORM_TYPE_MATRIX2,                                                                  // 矩阵2
			UNIFORM_TYPE_MATRIX3,                                                                  // 矩阵3
			UNIFORM_TYPE_MATRIX4                                                                   // 矩阵4
		};

		enum {
			UNIFORM_LOCATION_VIEWPORT = 0,                                                         // 视口
			UNIFORM_LOCATION_CAMERAPERSPECTIVE,                                                    // 相机视锥
			UNIFORM_LOCATION_CAMERAPOSITION,                                                       // 相机位置
			UNIFORM_LOCATION_CAMERAFORWARDDIRECTION,                                               // 相机朝前向量
			UNIFORM_LOCATION_CAMERAUPDIRECTION,                                                    // 相机朝上向量
			UNIFORM_LOCATION_MODELWORLDMATRIX,                                                     // 模型到世界变换矩阵
			UNIFORM_LOCATION_MODELVIEWMATRIX,                                                      // 模型到相机变换矩阵
			UNIFORM_LOCATION_MODELVIEWINVERSETRANSPOSEMATRIX,                                      // 模型到相机逆变换矩阵
			UNIFORM_LOCATION_MODELVIEWPROJECTIONMATRIX,                                            // 模型到投影变换矩阵
			UNIFORM_LOCATION_TEXTUREMATRIX0,                                                       // 纹理矩阵0
			UNIFORM_LOCATION_TEXTUREMATRIX1,                                                       // 纹理矩阵1
			UNIFORM_LOCATION_TEXTUREMATRIX2,                                                       // 纹理矩阵2
			UNIFORM_LOCATION_TEXTUREMATRIX3,                                                       // 纹理矩阵3
			UNIFORM_LOCATION_TEXTUREMATRIX4,                                                       // 纹理矩阵4
			UNIFORM_LOCATION_TEXTUREMATRIX5,                                                       // 纹理矩阵5
			UNIFORM_LOCATION_TEXTUREMATRIX6,                                                       // 纹理矩阵6
			UNIFORM_LOCATION_TEXTUREMATRIX7,                                                       // 纹理矩阵7
			UNIFORM_LOCATION_BONES,                                                                // 骨骼矩阵
			UNIFORM_LOCATION_ALPHAFUNC,                                                            // Alpha测试方法
			UNIFORM_LOCATION_ALPHAREF,                                                             // Alpha测试值
			UNIFORM_LOCATION_MATERIALAMBIENT,                                                      // 材质环境光
			UNIFORM_LOCATION_MATERIALDIFFUSE,                                                      // 材质漫反射光
			UNIFORM_LOCATION_MATERIALEMISSION,                                                     // 材质自发光
			UNIFORM_LOCATION_MATERIALSPECULAR,                                                     // 材质高光
			UNIFORM_LOCATION_MATERIALSHININESS,                                                    // 材质镜面指数
			UNIFORM_LOCATION_LIGHTPOSITION,                                                        // 光源位置
			UNIFORM_LOCATION_LIGHTAMBIENT,                                                         // 光源产生环境光
			UNIFORM_LOCATION_LIGHTDIFFUSE,                                                         // 光源产生漫反射光
			UNIFORM_LOCATION_LIGHTSPECULAR,                                                        // 光源产生高光
			UNIFORM_LOCATION_LIGHTRIM,                                                             // 光源泛光
			UNIFORM_LOCATION_LIGHTSKYLOWER,                                                        // 光源低天空光
			UNIFORM_LOCATION_LIGHTSKYUPPER,                                                        // 光源高天空光
			UNIFORM_LOCATION_LIGHTATTENUATION,                                                     // 光源衰减
			UNIFORM_LOCATION_SHADOWPROJECTIONMATRIX,                                               // 阴影投影矩阵
			UNIFORM_LOCATION_SUNSHAFTSSCREENSPACEPOSITION,                                         // 太阳射线在屏幕空间位置
			UNIFORM_LOCATION_COUNT
		};

		typedef struct {
			CHAR name[32];                                                                         // 名称
			UINT location;                                                                         // 定位
		} ATTRIB_LOCATION;

		typedef struct  {
			union {
				struct { INT   nvalue[16]; };                                                      // 整形
				struct { FLOAT fvalue[16]; };                                                      // 浮点
				struct { DWORD data[16];   };                                                      // 数据
			};
		} UNIFORM_LOCATION_VALUE;

		typedef struct {
			CHAR name[32];                                                                         // 名称
			UINT type;                                                                             // 类型
			UINT location;                                                                         // 定位

			UNIFORM_LOCATION_VALUE value;                                                          // 数值
			UNIFORM_LOCATION_VALUE cache;                                                          // 缓冲
		} UNIFORM_LOCATION;

		typedef std::map<DWORD, UNIFORM_LOCATION> UniformLocationMap;                              // 着色器参数集合

		typedef struct {
			UINT program;                                                                          // 着色器程序

			ATTRIB_LOCATION attribLocations[ATTRIB_LOCATION_COUNT];                                // 内部顶点属性集合
			UNIFORM_LOCATION uniformLocations[UNIFORM_LOCATION_COUNT];                             // 内部着色器参数集合

			UniformLocationMap uniformExternalLocations;                                           // 外部着色器参数集合
		} PROGRAM;

		typedef std::map<UINT64, PROGRAM*> ProgramMap;                                             // 程序集合

		// 渲染通路数据结构
	public:
		typedef struct {
			FLOAT texScrollU;                                                                      // 横向滚动速度
			FLOAT texScrollV;                                                                      // 纵向滚动速度

			UINT texSequenceCols;                                                                  // 序列帧列数
			UINT texSequenceRows;                                                                  // 序列帧行数
			FLOAT texSequenceDurationTime;                                                         // 序列帧持续时间
		} TEXTURE_ANIMATION;

		typedef struct {
			UINT target;                                                                           // 目标
			UINT texture;                                                                          // 纹理

			UINT texMinFliter;                                                                     // 近处纹理滤波方式
			UINT texMagFilter;                                                                     // 远处纹理滤波方式
			UINT texWrapU;                                                                         // 纹理坐标横向环绕方式
			UINT texWrapV;                                                                         // 纹理坐标纵向环绕方式

			TEXTURE_ANIMATION texAnimation;                                                        // 纹理动画
		} PASS_TEXTURE;

		typedef struct {
			UINT vertex;                                                                           // 顶点着色器
			UINT fragment;                                                                         // 片段着色器

			UniformLocationMap uniformExternalLocations;                                           // 外部着色器参数集合
		} PASS_PROGRAM;

		typedef struct {
			BOOL bEnableLighting;                                                                  // 光照状态
			FLOAT matAmbient[4];                                                                   // 环境光
			FLOAT matDiffuse[4];                                                                   // 漫反射光
			FLOAT matEmission[4];                                                                  // 自发光
			FLOAT matSpecular[4];                                                                  // 高光
			FLOAT matShininess;                                                                    // 高光级别
		} PASS_LIGHTING;

		typedef struct {
			BOOL bEnableCullFace;                                                                  // 面筛选状态
			UINT cullFaceMode;                                                                     // 面筛选模式
			UINT frontFaceMode;                                                                    // 面顶点顺序模式
		} PASS_CULL;

		typedef struct {
			BOOL bEnableDepthTest;                                                                 // 深度测试状态
			BOOL depthMask;                                                                        // 深度保存状态
			UINT depthFunc;                                                                        // 深度缓存使用方式
		} PASS_DEPTH;

		typedef struct {
			BOOL bEnableAlphaTest;                                                                 // Alpha测试状态
			UINT alphaFunc;                                                                        // Alpha测试方式
			FLOAT alphaRef;                                                                        // Alpha测试阈值
		} PASS_ALPHA;

		typedef struct {
			BOOL bEnableBlend;                                                                     // 混合状态
			UINT blendModeColor;                                                                   // 混合模式Color
			UINT blendModeAlpha;                                                                   // 混合模式Alpha
			UINT blendSrcColor;                                                                    // 源像素Color混合因子
			UINT blendSrcAlpha;                                                                    // 源像素Alpha混合因子
			UINT blendDesColor;                                                                    // 目标像素Color混合因子
			UINT blendDesAlpha;                                                                    // 目标像素Alpha混合因子
		} PASS_BLEND;

		typedef struct {
			PASS_PROGRAM program;                                                                  // 程序
			PASS_CULL cull;                                                                        // 筛选
			PASS_DEPTH depth;                                                                      // 深度
			PASS_ALPHA alpha;                                                                      // Alpha
			PASS_BLEND blend;                                                                      // 混合
			PASS_LIGHTING lighting;                                                                // 光照
			PASS_TEXTURE textures[MAX_TEXUNITS];                                                   // 纹理单元
		} PASS;

		
		// 构造/析构函数
	protected:
		CRenderer(HWND hWnd);
		virtual ~CRenderer(VOID);


		// 方法
	protected:
		BOOL Init(HWND hWnd);                                                                      // 初始化渲染系统
		VOID Free(VOID);                                                                           // 释放渲染系统

	protected:
		BOOL CreateRect(VOID);                                                                     // 创建矩形
		VOID DestroyRect(VOID);                                                                    // 销毁矩形

		BOOL CreateBox(VOID);                                                                      // 创建盒
		VOID DestroyBox(VOID);                                                                     // 销毁盒

		BOOL CreateSphere(VOID);                                                                   // 创建球
		VOID DestroySphere(VOID);                                                                  // 销毁球

	public:
		HWND GetHWND(VOID) const;                                                                  // 获得窗口句柄
		HDC GetHDC(VOID) const;                                                                    // 获得设备上下文
		HGLRC GetHRC(VOID) const;                                                                  // 获得资源上下文

	public:
		UINT GetMaxTexUnits(VOID) const;                                                           // 最大纹理单元数
		UINT GetMaxFBOAttachments(VOID) const;                                                     // 最大FBO挂接数
		UINT GetMaxFBODrawBuffers(VOID) const;                                                     // 最大FBO输出数

	public:
		BYTE GetBoolen(UINT name) const;                                                           // 获得参数
		FLOAT GetFloat(UINT name) const;                                                           // 获得参数
		INT GetInteger(UINT name) const;                                                           // 获得参数
		const CHAR* GetString(UINT name) const;                                                    // 获得参数

		VOID GetBooleanv(UINT name, BYTE *params) const;                                           // 获得参数
		VOID GetFloatv(UINT name, FLOAT *params) const;                                            // 获得参数
		VOID GetIntegerv(UINT name, INT *params) const;                                            // 获得参数

	public:
		VOID Enable(UINT cap) const;                                                               // 启用
		VOID Disable(UINT cap) const;                                                              // 禁用
		BOOL IsEnable(UINT cap) const;                                                             // 状态

	public:
		VOID SetCullFace(UINT mode) const;                                                         // 设置面剔除模式
		VOID SetFrontFace(UINT mode) const;                                                        // 设置面定义

		VOID SetDepthFunc(UINT func) const;                                                        // 设置深度测试方法
		VOID SetAlphaFunc(UINT func, FLOAT ref) const;                                             // 设置Alpha测试方法

		VOID SetBlendEquation(UINT modergb, UINT modealpha) const;                                 // 设置混合方程
		VOID SetBlendFunc(UINT srcrgb, UINT dstrgb, UINT srcalpha, UINT dstalpha) const;           // 设置混合方法

	public:
		VOID SetClearStencil(INT stencil);                                                         // 设置清空模板值
		VOID SetClearDepth(FLOAT depth);                                                           // 设置清空深度值
		VOID SetClearColor(FLOAT red, FLOAT green, FLOAT blue, FLOAT alpha);                       // 设置清空颜色值
		VOID ClearBuffer(BOOL bColorBuffer, BOOL bDepthBuffer, BOOL bStencelBuffer) const;         // 清空缓冲

	public:
		VOID SetStencilFunc(UINT func, UINT ref, UINT mask) const;                                 // 设置模板比较条件
		VOID SetStencilOp(UINT sfail, UINT zfail, UINT zpass) const;                               // 设置模板更新方法

	public:
		VOID SetScissor(INT x, INT y, INT width, INT height) const;                                // 设置裁减
		VOID SetViewport(INT x, INT y, INT width, INT height) const;                               // 设置视口

		INT GetViewportX(VOID) const;                                                              // 获得视口X
		INT GetViewportY(VOID) const;                                                              // 获得视口Y
		INT GetViewportWidth(VOID) const;                                                          // 获得视口宽
		INT GetViewportHeight(VOID) const;                                                         // 获得视口高

	public:
		VOID SetPerspective(FLOAT fovy, FLOAT aspect, FLOAT znear, FLOAT zfar) const;              // 设置透视投影视锥
		VOID SetOrtho(FLOAT left, FLOAT right, FLOAT bottom, FLOAT top, FLOAT znear, FLOAT zfar) const; // 设置正交投影视锥

	public:
		VOID PushMatrix(VOID) const;                                                               // 矩阵压栈
		VOID PopMatrix(VOID) const;                                                                // 矩阵弹栈

		VOID PushTextureMatrix(VOID) const;                                                        // 纹理矩阵压栈
		VOID PopTextureMatrix(VOID) const;                                                         // 纹理矩阵弹栈

		VOID SetLookAt(const FLOAT *position, const FLOAT *direction, const FLOAT *up) const;      // 设置相机观察方位
		VOID SetModelWorldMatrix(const FLOAT *matrix) const;                                       // 设置模型坐标系到世界坐标系矩阵
		VOID SetTextureMatrix(INT indexTexUnit, const FLOAT *matrix) const;                        // 设置纹理矩阵

		FLOAT* GetModelWorldMatrix(VOID) const;                                                    // 获得模型世界变换矩阵
		FLOAT* GetModelViewMatrix(VOID) const;                                                     // 获得模型视图变换矩阵
		FLOAT* GetModelViewInverseTransposeMatrix(VOID) const;                                     // 获得模型视图变换逆矩阵
		FLOAT* GetModelViewProjectionMatrix(VOID) const;                                           // 获得模型视图投影变换矩阵
		FLOAT* GetTextureMatrix(INT indexTexUnit) const;                                           // 获得纹理变换矩阵

	public:
		UINT CreateTexture2D(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT format, INT type, const BYTE *pixels) const; // 创建纹理
		UINT CreateTexture2DMipmapsBMP(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT format, INT type, INT bitcount, const BYTE *pixels) const; // 创建纹理
		UINT CreateTexture2DMipmapsDDS(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT bitcount, const BYTE *pixels) const; // 创建纹理
		UINT CreateTexture2DMipmapsPVR(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT bitcount, const BYTE *pixels) const; // 创建纹理
		UINT CreateTexture2DMipmapsETC(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT bitcount, const BYTE *pixels) const; // 创建纹理
		VOID DestroyTexture(UINT *texture) const;                                                  // 销毁纹理
		VOID BindTexture(UINT target, UINT texture) const;                                         // 绑定纹理
		VOID ActiveTexture(UINT texture) const;                                                    // 启用纹理
		VOID TextureParameter(UINT target, UINT name, INT param) const;                            // 设置纹理参数

		VOID SetTexture(INT indexTexUnit, UINT target, UINT texture, INT minFilter, INT maxFilter, INT warpU, INT warpV) const; // 设置纹理
		VOID SetLightMapTexture(UINT texture);                                                     // 设置光照纹理

	public:
		UINT CreateVAO(VOID) const;                                                                // 创建VAO
		VOID DestroyVAO(UINT *vao) const;                                                          // 销毁VAO
		VOID BindVAO(UINT vao) const;                                                              // 绑定VAO

		UINT CreateVBO(UINT target, INT size, const VOID* data, UINT usage) const;                 // 创建VBO
		VOID DestroyVBO(UINT *vbo) const;                                                          // 销毁VBO
		VOID BindVBO(UINT target, UINT vbo) const;                                                 // 绑定VBO
		VOID UpdateVBO(UINT target, INT offset, INT size, const VOID *data) const;                 // 更新VBO

	public:
		UINT CreateRenderBuffer(VOID) const;                                                       // 创建渲染缓冲
		VOID DestroyRenderBuffer(UINT *buffer) const;                                              // 销毁渲染缓冲
		VOID BindRenderBuffer(UINT buffer) const;                                                  // 绑定渲染缓冲
		VOID RenderBufferFormat(UINT internalformat, INT width, INT height) const;                 // 设置渲染缓冲存储格式

		UINT CreateFrameBuffer(VOID) const;                                                        // 创建FBO
		VOID DestroyFrameBuffer(UINT *fbo) const;                                                  // 销毁FBO
		VOID BindFrameBuffer(UINT fbo) const;                                                      // 绑定FBO
		VOID FrameBufferRenderBuffer(UINT attachment, UINT buffer) const;                          // 设置FBO渲染缓冲
		VOID FrameBufferTexture2D(UINT attachment, UINT texture, INT level) const;                 // 设置FBO渲染纹理
		BOOL CheckFrameBufferStatus(VOID) const;                                                   // 检查FBO状态

	public:
		UINT CreateQuery(VOID) const;                                                              // 创建遮挡查询
		VOID DestroyQuery(UINT *query) const;                                                      // 销毁遮挡查询
		BOOL QueryResult(UINT query) const;                                                        // 查询结果
		BOOL QueryResultAvailable(UINT query) const;                                               // 查询结果可用判断

	public:
		UINT CreateShader(UINT type, const CHAR *source) const;                                    // 创建着色器
		VOID DestroyShader(UINT *shader) const;                                                    // 销毁着色器

		UINT CreateProgram(UINT vertex, UINT fragment) const;                                      // 创建程序
		BOOL CreateProgram(UINT vertex, UINT fragment, const UniformLocationMap &params);          // 创建程序
		VOID DestroyProgram(UINT vertex, UINT fragment);                                           // 销毁程序
		VOID DestroyProgramAll(VOID);                                                              // 销毁所有程序

		BOOL UseProgram(UINT vertex, UINT fragment, const UniformLocationMap &params);             // 使用程序

		VOID SetProgramParam(const CHAR *szName, const UNIFORM_LOCATION_VALUE *value);             // 设置程序参数
		VOID SetProgramParam(DWORD dwName, const UNIFORM_LOCATION_VALUE *value);                   // 设置程序参数

	protected:
		VOID SetProgramParam(UNIFORM_LOCATION *pParam);                                            // 设置程序参数
		VOID SetProgramParams(VOID);                                                               // 设置程序参数

	public:
		VOID SetMaterialAmbient(const FLOAT *ambient);                                             // 设置材质环境光
		VOID SetMaterialDiffuse(const FLOAT *diffuse);                                             // 设置材质漫反射光
		VOID SetMaterialEmission(const FLOAT *emission);                                           // 设置材质自发光
		VOID SetMaterialSpecular(const FLOAT *specular);                                           // 设置材质高光
		VOID SetMaterialShininess(FLOAT shininess);                                                // 设置材质镜面指数

		VOID SetLight(const FLOAT *position, const FLOAT *ambient, const FLOAT *diffuse, const FLOAT *specular, const FLOAT *rim, const FLOAT *skyLower, const FLOAT *skyUpper, const FLOAT *attenuation); // 设置光源
		VOID SetShadowProjectionMatrix(const FLOAT *matrix);                                       // 设置阴影投影矩阵
		VOID SetSunShaftsScreenSpacePosition(const FLOAT *position);                               // 设置太阳射线在屏幕空间位置

	public:
		VERTEX* LockVertexBuffer(INT numVertices) const;                                           // 锁定顶点缓冲
		VOID UnlockVertexBuffer(VERTEX *pVertices) const;                                          // 解锁顶点缓冲

		INDEX* LockIndexBuffer(INT numIndices) const;                                              // 锁定索引缓冲
		VOID UnlockIndexBuffer(INDEX *pIndices) const;                                             // 解锁索引缓冲

	public:
		VOID SetRenderMode(UINT mode);                                                             // 设置渲染模式
		VOID SetDepthMask(BOOL depth) const;                                                       // 设置写深度
		VOID SetColorMask(BOOL red, BOOL green, BOOL blue, BOOL alpha) const;                      // 设置写颜色

		VOID SetReadBuffer(UINT mode) const;                                                       // 设置读取缓冲
		VOID SetDrawBuffer(UINT mode) const;                                                       // 设置输出缓冲
		VOID SetDrawBuffers(INT n, const UINT *buffers) const;                                     // 设置多路输出缓冲

		VOID SetPass(const PASS *pPass);                                                           // 设置材质通路
		VOID SetPassAnimator(const PASS *pPass, FLOAT time);                                       // 设置材质动画
		VOID SetPassProgram(const PASS_PROGRAM *pProgram);                                         // 设置材质程序
		VOID SetPassTexture(const PASS_TEXTURE *pTexture, INT indexTexUnit);                       // 设置材质纹理
		VOID SetPassTextureAnimator(const PASS_TEXTURE *pTexture, INT indexTexUnit, FLOAT time);   // 设置材质纹理动画
		VOID SetPassCull(const PASS_CULL *pCull);                                                  // 设置材质剔除
		VOID SetPassDepth(const PASS_DEPTH *pDepth);                                               // 设置材质深度
		VOID SetPassAlpha(const PASS_ALPHA *pAlpha);                                               // 设置材质Alpha
		VOID SetPassBlend(const PASS_BLEND *pBlend);                                               // 设置材质混合
		VOID SetPassLighting(const PASS_LIGHTING *pLighting);                                      // 设置材质光照

		VOID SubmitVertexBuffer(UINT vao) const;                                                   // 提交顶点缓冲
		VOID SubmitVertexBuffer(DWORD dwVertexFormat, const VERTEX *pVertices) const;              // 提交顶点缓冲
		VOID SubmitMatrixBuffer(const FLOAT *matrices, INT numMatrices);                           // 提交矩阵缓冲

		VOID BeginOcclude(VOID) const;                                                             // 开始渲染遮挡
		VOID EndOcclude(VOID) const;                                                               // 结束渲染遮挡
		VOID RenderOcclude(const INDEX *indices, INT count);                                       // 渲染遮挡

		VOID BeginQuery(VOID) const;                                                               // 开始渲染查询
		VOID EndQuery(VOID) const;                                                                 // 结束渲染查询
		VOID RenderQuery(UINT query, const AABB *aabb);                                            // 渲染查询

		VOID BeginLighting(VOID) const;                                                            // 开始渲染光照
		VOID EndLighting(VOID) const;                                                              // 结束渲染光照
		VOID RenderLighting(const FLOAT *position, FLOAT radius);                                  // 渲染光照

		VOID Render(const INDEX *indices, INT count);                                              // 渲染
		VOID RenderRect(FLOAT left, FLOAT right, FLOAT top, FLOAT bottom);                         // 渲染矩形
		VOID RenderViewport(VOID);                                                                 // 渲染视口

		VOID Flush(VOID) const;                                                                    // 刷新指令缓冲
		VOID Finish(VOID) const;                                                                   // 刷新指令缓冲并等待执行结束
		VOID SwapBuffers(VOID) const;                                                              // 交换缓冲

	public:
		INT GetFaces(VOID) const;                                                                  // 获得渲染面数
		INT GetDrawCalls(VOID) const;                                                              // 获得渲染调用次数


		// 属性
	protected:
		HDC m_hDC;                                                                                 // 设备上下文
		HGLRC m_hRC;                                                                               // 资源上下文
		HWND m_hWnd;                                                                               // 窗口句柄

	protected:
		UINT m_maxTexUnits;                                                                        // 最大纹理单元数
		UINT m_maxFBOAttachments;                                                                  // 最大FBO挂接数
		UINT m_maxFBODrawBuffers;                                                                  // 最大FBO输出数

	protected:
		INT m_clearStencil;                                                                        // 清模板值
		FLOAT m_clearDepth;                                                                        // 清深度值
		FLOAT m_clearRed, m_clearGreen, m_clearBlue, m_clearAlpha;                                 // 清颜色值

	protected:
		UINT m_rectIndexBuffer;                                                                    // 矩形索引缓冲
		UINT m_rectVertexBuffer;                                                                   // 矩形顶点缓冲

		UINT m_boxIndexBuffer;                                                                     // 盒索引缓冲
		UINT m_boxVertexBuffer;                                                                    // 盒顶点缓冲

		UINT m_sphereIndexBuffer;                                                                  // 球索引缓冲
		UINT m_sphereVertexBuffer;                                                                 // 球顶点缓冲

	protected:
		UINT m_renderMode;                                                                         // 渲染模式

	protected:
		PROGRAM *m_pProgram;                                                                       // 当前程序
		ProgramMap m_programs;                                                                     // 程序集合
		UniformLocationMap m_programParams;                                                        // 着色器参数集合

	protected:
		INT m_draws;                                                                               // 渲染次数
		INT m_faces;                                                                               // 渲染面数
	};

}

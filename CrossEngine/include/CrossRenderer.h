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

	typedef DWORD INDEX;                                                                           // ��������

	typedef struct {
		DWORD index0;                                                                              // �����涥��0����
		DWORD index1;                                                                              // �����涥��1����
		DWORD index2;                                                                              // �����涥��2����
	} FACE;

	typedef struct {
		VEC3 position;                                                                             // λ��
		VEC3 normal;                                                                               // ����
		VEC3 binormal;                                                                             // ������
		VEC4 color;                                                                                // ��ɫ
		VEC2 texCoordDiffuse;                                                                      // ��������������
		VEC2 texCoordLightMap;                                                                     // ����ͼ��������
		FLOAT indices[4];                                                                          // ��Ƥ��������
		FLOAT weights[4];                                                                          // ��Ƥ����Ȩ��
	} VERTEX;

	class _CrossExport CRenderer
	{
		friend class CRenderSystem;


		// �����ʽ
	public:
		static const DWORD VERTEX_FORMAT_SKINMESH;                                                 // ��Ƥ���񶥵��ʽ
		static const DWORD VERTEX_FORMAT_STATICMESH;                                               // ��̬���񶥵��ʽ
		static const DWORD VERTEX_FORMAT_DYNAMICMESH;                                              // ��̬���񶥵��ʽ
		static const DWORD VERTEX_FORMAT_PARTICLE;                                                 // ���Ӷ����ʽ
		static const DWORD VERTEX_FORMAT_HALO;                                                     // ���ζ����ʽ
		static const DWORD VERTEX_FORMAT_SCREEN;                                                   // ��Ļ�����ʽ
		static const DWORD VERTEX_FORMAT_POSITION;                                                 // λ�ö����ʽ

		// ��ɫ�����ݽṹ
	public:
		enum {
			ATTRIB_LOCATION_POSITION = 0,                                                          // λ������
			ATTRIB_LOCATION_NORMAL,                                                                // ��������
			ATTRIB_LOCATION_BINORMAL,                                                              // ����������
			ATTRIB_LOCATION_COLOR,                                                                 // ��ɫ����
			ATTRIB_LOCATION_TEXCOORDDIFFUSE,                                                       // ������������������
			ATTRIB_LOCATION_TEXCOORDLIGHTMAP,                                                      // ����ͼ������������
			ATTRIB_LOCATION_INDICES,                                                               // ��Ƥ������������
			ATTRIB_LOCATION_WEIGHTS,                                                               // ��Ƥ����Ȩ������
			ATTRIB_LOCATION_COUNT
		};

		enum {
			UNIFORM_TYPE_UNKNOW = 0,                                                               // δ֪
			UNIFORM_TYPE_INT,                                                                      // ����
			UNIFORM_TYPE_INT2,                                                                     // ����2
			UNIFORM_TYPE_INT3,                                                                     // ����3
			UNIFORM_TYPE_INT4,                                                                     // ����4
			UNIFORM_TYPE_FLOAT,                                                                    // ����
			UNIFORM_TYPE_FLOAT2,                                                                   // ����2
			UNIFORM_TYPE_FLOAT3,                                                                   // ����3
			UNIFORM_TYPE_FLOAT4,                                                                   // ����4
			UNIFORM_TYPE_MATRIX2,                                                                  // ����2
			UNIFORM_TYPE_MATRIX3,                                                                  // ����3
			UNIFORM_TYPE_MATRIX4                                                                   // ����4
		};

		enum {
			UNIFORM_LOCATION_VIEWPORT = 0,                                                         // �ӿ�
			UNIFORM_LOCATION_CAMERAPERSPECTIVE,                                                    // �����׶
			UNIFORM_LOCATION_CAMERAPOSITION,                                                       // ���λ��
			UNIFORM_LOCATION_CAMERAFORWARDDIRECTION,                                               // �����ǰ����
			UNIFORM_LOCATION_CAMERAUPDIRECTION,                                                    // �����������
			UNIFORM_LOCATION_MODELWORLDMATRIX,                                                     // ģ�͵�����任����
			UNIFORM_LOCATION_MODELVIEWMATRIX,                                                      // ģ�͵�����任����
			UNIFORM_LOCATION_MODELVIEWINVERSETRANSPOSEMATRIX,                                      // ģ�͵������任����
			UNIFORM_LOCATION_MODELVIEWPROJECTIONMATRIX,                                            // ģ�͵�ͶӰ�任����
			UNIFORM_LOCATION_TEXTUREMATRIX0,                                                       // �������0
			UNIFORM_LOCATION_TEXTUREMATRIX1,                                                       // �������1
			UNIFORM_LOCATION_TEXTUREMATRIX2,                                                       // �������2
			UNIFORM_LOCATION_TEXTUREMATRIX3,                                                       // �������3
			UNIFORM_LOCATION_TEXTUREMATRIX4,                                                       // �������4
			UNIFORM_LOCATION_TEXTUREMATRIX5,                                                       // �������5
			UNIFORM_LOCATION_TEXTUREMATRIX6,                                                       // �������6
			UNIFORM_LOCATION_TEXTUREMATRIX7,                                                       // �������7
			UNIFORM_LOCATION_BONES,                                                                // ��������
			UNIFORM_LOCATION_ALPHAFUNC,                                                            // Alpha���Է���
			UNIFORM_LOCATION_ALPHAREF,                                                             // Alpha����ֵ
			UNIFORM_LOCATION_MATERIALAMBIENT,                                                      // ���ʻ�����
			UNIFORM_LOCATION_MATERIALDIFFUSE,                                                      // �����������
			UNIFORM_LOCATION_MATERIALEMISSION,                                                     // �����Է���
			UNIFORM_LOCATION_MATERIALSPECULAR,                                                     // ���ʸ߹�
			UNIFORM_LOCATION_MATERIALSHININESS,                                                    // ���ʾ���ָ��
			UNIFORM_LOCATION_LIGHTPOSITION,                                                        // ��Դλ��
			UNIFORM_LOCATION_LIGHTAMBIENT,                                                         // ��Դ����������
			UNIFORM_LOCATION_LIGHTDIFFUSE,                                                         // ��Դ�����������
			UNIFORM_LOCATION_LIGHTSPECULAR,                                                        // ��Դ�����߹�
			UNIFORM_LOCATION_LIGHTRIM,                                                             // ��Դ����
			UNIFORM_LOCATION_LIGHTSKYLOWER,                                                        // ��Դ����չ�
			UNIFORM_LOCATION_LIGHTSKYUPPER,                                                        // ��Դ����չ�
			UNIFORM_LOCATION_LIGHTATTENUATION,                                                     // ��Դ˥��
			UNIFORM_LOCATION_SHADOWPROJECTIONMATRIX,                                               // ��ӰͶӰ����
			UNIFORM_LOCATION_SUNSHAFTSSCREENSPACEPOSITION,                                         // ̫����������Ļ�ռ�λ��
			UNIFORM_LOCATION_COUNT
		};

		typedef struct {
			CHAR name[32];                                                                         // ����
			UINT location;                                                                         // ��λ
		} ATTRIB_LOCATION;

		typedef struct  {
			union {
				struct { INT   nvalue[16]; };                                                      // ����
				struct { FLOAT fvalue[16]; };                                                      // ����
				struct { DWORD data[16];   };                                                      // ����
			};
		} UNIFORM_LOCATION_VALUE;

		typedef struct {
			CHAR name[32];                                                                         // ����
			UINT type;                                                                             // ����
			UINT location;                                                                         // ��λ

			UNIFORM_LOCATION_VALUE value;                                                          // ��ֵ
			UNIFORM_LOCATION_VALUE cache;                                                          // ����
		} UNIFORM_LOCATION;

		typedef std::map<DWORD, UNIFORM_LOCATION> UniformLocationMap;                              // ��ɫ����������

		typedef struct {
			UINT program;                                                                          // ��ɫ������

			ATTRIB_LOCATION attribLocations[ATTRIB_LOCATION_COUNT];                                // �ڲ��������Լ���
			UNIFORM_LOCATION uniformLocations[UNIFORM_LOCATION_COUNT];                             // �ڲ���ɫ����������

			UniformLocationMap uniformExternalLocations;                                           // �ⲿ��ɫ����������
		} PROGRAM;

		typedef std::map<UINT64, PROGRAM*> ProgramMap;                                             // ���򼯺�

		// ��Ⱦͨ·���ݽṹ
	public:
		typedef struct {
			FLOAT texScrollU;                                                                      // ��������ٶ�
			FLOAT texScrollV;                                                                      // ��������ٶ�

			UINT texSequenceCols;                                                                  // ����֡����
			UINT texSequenceRows;                                                                  // ����֡����
			FLOAT texSequenceDurationTime;                                                         // ����֡����ʱ��
		} TEXTURE_ANIMATION;

		typedef struct {
			UINT target;                                                                           // Ŀ��
			UINT texture;                                                                          // ����

			UINT texMinFliter;                                                                     // ���������˲���ʽ
			UINT texMagFilter;                                                                     // Զ�������˲���ʽ
			UINT texWrapU;                                                                         // ������������Ʒ�ʽ
			UINT texWrapV;                                                                         // �������������Ʒ�ʽ

			TEXTURE_ANIMATION texAnimation;                                                        // ������
		} PASS_TEXTURE;

		typedef struct {
			UINT vertex;                                                                           // ������ɫ��
			UINT fragment;                                                                         // Ƭ����ɫ��

			UniformLocationMap uniformExternalLocations;                                           // �ⲿ��ɫ����������
		} PASS_PROGRAM;

		typedef struct {
			BOOL bEnableLighting;                                                                  // ����״̬
			FLOAT matAmbient[4];                                                                   // ������
			FLOAT matDiffuse[4];                                                                   // �������
			FLOAT matEmission[4];                                                                  // �Է���
			FLOAT matSpecular[4];                                                                  // �߹�
			FLOAT matShininess;                                                                    // �߹⼶��
		} PASS_LIGHTING;

		typedef struct {
			BOOL bEnableCullFace;                                                                  // ��ɸѡ״̬
			UINT cullFaceMode;                                                                     // ��ɸѡģʽ
			UINT frontFaceMode;                                                                    // �涥��˳��ģʽ
		} PASS_CULL;

		typedef struct {
			BOOL bEnableDepthTest;                                                                 // ��Ȳ���״̬
			BOOL depthMask;                                                                        // ��ȱ���״̬
			UINT depthFunc;                                                                        // ��Ȼ���ʹ�÷�ʽ
		} PASS_DEPTH;

		typedef struct {
			BOOL bEnableAlphaTest;                                                                 // Alpha����״̬
			UINT alphaFunc;                                                                        // Alpha���Է�ʽ
			FLOAT alphaRef;                                                                        // Alpha������ֵ
		} PASS_ALPHA;

		typedef struct {
			BOOL bEnableBlend;                                                                     // ���״̬
			UINT blendModeColor;                                                                   // ���ģʽColor
			UINT blendModeAlpha;                                                                   // ���ģʽAlpha
			UINT blendSrcColor;                                                                    // Դ����Color�������
			UINT blendSrcAlpha;                                                                    // Դ����Alpha�������
			UINT blendDesColor;                                                                    // Ŀ������Color�������
			UINT blendDesAlpha;                                                                    // Ŀ������Alpha�������
		} PASS_BLEND;

		typedef struct {
			PASS_PROGRAM program;                                                                  // ����
			PASS_CULL cull;                                                                        // ɸѡ
			PASS_DEPTH depth;                                                                      // ���
			PASS_ALPHA alpha;                                                                      // Alpha
			PASS_BLEND blend;                                                                      // ���
			PASS_LIGHTING lighting;                                                                // ����
			PASS_TEXTURE textures[MAX_TEXUNITS];                                                   // ����Ԫ
		} PASS;

		
		// ����/��������
	protected:
		CRenderer(HWND hWnd);
		virtual ~CRenderer(VOID);


		// ����
	protected:
		BOOL Init(HWND hWnd);                                                                      // ��ʼ����Ⱦϵͳ
		VOID Free(VOID);                                                                           // �ͷ���Ⱦϵͳ

	protected:
		BOOL CreateRect(VOID);                                                                     // ��������
		VOID DestroyRect(VOID);                                                                    // ���پ���

		BOOL CreateBox(VOID);                                                                      // ������
		VOID DestroyBox(VOID);                                                                     // ���ٺ�

		BOOL CreateSphere(VOID);                                                                   // ������
		VOID DestroySphere(VOID);                                                                  // ������

	public:
		HWND GetHWND(VOID) const;                                                                  // ��ô��ھ��
		HDC GetHDC(VOID) const;                                                                    // ����豸������
		HGLRC GetHRC(VOID) const;                                                                  // �����Դ������

	public:
		UINT GetMaxTexUnits(VOID) const;                                                           // �������Ԫ��
		UINT GetMaxFBOAttachments(VOID) const;                                                     // ���FBO�ҽ���
		UINT GetMaxFBODrawBuffers(VOID) const;                                                     // ���FBO�����

	public:
		BYTE GetBoolen(UINT name) const;                                                           // ��ò���
		FLOAT GetFloat(UINT name) const;                                                           // ��ò���
		INT GetInteger(UINT name) const;                                                           // ��ò���
		const CHAR* GetString(UINT name) const;                                                    // ��ò���

		VOID GetBooleanv(UINT name, BYTE *params) const;                                           // ��ò���
		VOID GetFloatv(UINT name, FLOAT *params) const;                                            // ��ò���
		VOID GetIntegerv(UINT name, INT *params) const;                                            // ��ò���

	public:
		VOID Enable(UINT cap) const;                                                               // ����
		VOID Disable(UINT cap) const;                                                              // ����
		BOOL IsEnable(UINT cap) const;                                                             // ״̬

	public:
		VOID SetCullFace(UINT mode) const;                                                         // �������޳�ģʽ
		VOID SetFrontFace(UINT mode) const;                                                        // �����涨��

		VOID SetDepthFunc(UINT func) const;                                                        // ������Ȳ��Է���
		VOID SetAlphaFunc(UINT func, FLOAT ref) const;                                             // ����Alpha���Է���

		VOID SetBlendEquation(UINT modergb, UINT modealpha) const;                                 // ���û�Ϸ���
		VOID SetBlendFunc(UINT srcrgb, UINT dstrgb, UINT srcalpha, UINT dstalpha) const;           // ���û�Ϸ���

	public:
		VOID SetClearStencil(INT stencil);                                                         // �������ģ��ֵ
		VOID SetClearDepth(FLOAT depth);                                                           // ����������ֵ
		VOID SetClearColor(FLOAT red, FLOAT green, FLOAT blue, FLOAT alpha);                       // ���������ɫֵ
		VOID ClearBuffer(BOOL bColorBuffer, BOOL bDepthBuffer, BOOL bStencelBuffer) const;         // ��ջ���

	public:
		VOID SetStencilFunc(UINT func, UINT ref, UINT mask) const;                                 // ����ģ��Ƚ�����
		VOID SetStencilOp(UINT sfail, UINT zfail, UINT zpass) const;                               // ����ģ����·���

	public:
		VOID SetScissor(INT x, INT y, INT width, INT height) const;                                // ���òü�
		VOID SetViewport(INT x, INT y, INT width, INT height) const;                               // �����ӿ�

		INT GetViewportX(VOID) const;                                                              // ����ӿ�X
		INT GetViewportY(VOID) const;                                                              // ����ӿ�Y
		INT GetViewportWidth(VOID) const;                                                          // ����ӿڿ�
		INT GetViewportHeight(VOID) const;                                                         // ����ӿڸ�

	public:
		VOID SetPerspective(FLOAT fovy, FLOAT aspect, FLOAT znear, FLOAT zfar) const;              // ����͸��ͶӰ��׶
		VOID SetOrtho(FLOAT left, FLOAT right, FLOAT bottom, FLOAT top, FLOAT znear, FLOAT zfar) const; // ��������ͶӰ��׶

	public:
		VOID PushMatrix(VOID) const;                                                               // ����ѹջ
		VOID PopMatrix(VOID) const;                                                                // ����ջ

		VOID PushTextureMatrix(VOID) const;                                                        // �������ѹջ
		VOID PopTextureMatrix(VOID) const;                                                         // �������ջ

		VOID SetLookAt(const FLOAT *position, const FLOAT *direction, const FLOAT *up) const;      // ��������۲췽λ
		VOID SetModelWorldMatrix(const FLOAT *matrix) const;                                       // ����ģ������ϵ����������ϵ����
		VOID SetTextureMatrix(INT indexTexUnit, const FLOAT *matrix) const;                        // �����������

		FLOAT* GetModelWorldMatrix(VOID) const;                                                    // ���ģ������任����
		FLOAT* GetModelViewMatrix(VOID) const;                                                     // ���ģ����ͼ�任����
		FLOAT* GetModelViewInverseTransposeMatrix(VOID) const;                                     // ���ģ����ͼ�任�����
		FLOAT* GetModelViewProjectionMatrix(VOID) const;                                           // ���ģ����ͼͶӰ�任����
		FLOAT* GetTextureMatrix(INT indexTexUnit) const;                                           // �������任����

	public:
		UINT CreateTexture2D(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT format, INT type, const BYTE *pixels) const; // ��������
		UINT CreateTexture2DMipmapsBMP(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT format, INT type, INT bitcount, const BYTE *pixels) const; // ��������
		UINT CreateTexture2DMipmapsDDS(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT bitcount, const BYTE *pixels) const; // ��������
		UINT CreateTexture2DMipmapsPVR(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT bitcount, const BYTE *pixels) const; // ��������
		UINT CreateTexture2DMipmapsETC(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT bitcount, const BYTE *pixels) const; // ��������
		VOID DestroyTexture(UINT *texture) const;                                                  // ��������
		VOID BindTexture(UINT target, UINT texture) const;                                         // ������
		VOID ActiveTexture(UINT texture) const;                                                    // ��������
		VOID TextureParameter(UINT target, UINT name, INT param) const;                            // �����������

		VOID SetTexture(INT indexTexUnit, UINT target, UINT texture, INT minFilter, INT maxFilter, INT warpU, INT warpV) const; // ��������
		VOID SetLightMapTexture(UINT texture);                                                     // ���ù�������

	public:
		UINT CreateVAO(VOID) const;                                                                // ����VAO
		VOID DestroyVAO(UINT *vao) const;                                                          // ����VAO
		VOID BindVAO(UINT vao) const;                                                              // ��VAO

		UINT CreateVBO(UINT target, INT size, const VOID* data, UINT usage) const;                 // ����VBO
		VOID DestroyVBO(UINT *vbo) const;                                                          // ����VBO
		VOID BindVBO(UINT target, UINT vbo) const;                                                 // ��VBO
		VOID UpdateVBO(UINT target, INT offset, INT size, const VOID *data) const;                 // ����VBO

	public:
		UINT CreateRenderBuffer(VOID) const;                                                       // ������Ⱦ����
		VOID DestroyRenderBuffer(UINT *buffer) const;                                              // ������Ⱦ����
		VOID BindRenderBuffer(UINT buffer) const;                                                  // ����Ⱦ����
		VOID RenderBufferFormat(UINT internalformat, INT width, INT height) const;                 // ������Ⱦ����洢��ʽ

		UINT CreateFrameBuffer(VOID) const;                                                        // ����FBO
		VOID DestroyFrameBuffer(UINT *fbo) const;                                                  // ����FBO
		VOID BindFrameBuffer(UINT fbo) const;                                                      // ��FBO
		VOID FrameBufferRenderBuffer(UINT attachment, UINT buffer) const;                          // ����FBO��Ⱦ����
		VOID FrameBufferTexture2D(UINT attachment, UINT texture, INT level) const;                 // ����FBO��Ⱦ����
		BOOL CheckFrameBufferStatus(VOID) const;                                                   // ���FBO״̬

	public:
		UINT CreateQuery(VOID) const;                                                              // �����ڵ���ѯ
		VOID DestroyQuery(UINT *query) const;                                                      // �����ڵ���ѯ
		BOOL QueryResult(UINT query) const;                                                        // ��ѯ���
		BOOL QueryResultAvailable(UINT query) const;                                               // ��ѯ��������ж�

	public:
		UINT CreateShader(UINT type, const CHAR *source) const;                                    // ������ɫ��
		VOID DestroyShader(UINT *shader) const;                                                    // ������ɫ��

		UINT CreateProgram(UINT vertex, UINT fragment) const;                                      // ��������
		BOOL CreateProgram(UINT vertex, UINT fragment, const UniformLocationMap &params);          // ��������
		VOID DestroyProgram(UINT vertex, UINT fragment);                                           // ���ٳ���
		VOID DestroyProgramAll(VOID);                                                              // �������г���

		BOOL UseProgram(UINT vertex, UINT fragment, const UniformLocationMap &params);             // ʹ�ó���

		VOID SetProgramParam(const CHAR *szName, const UNIFORM_LOCATION_VALUE *value);             // ���ó������
		VOID SetProgramParam(DWORD dwName, const UNIFORM_LOCATION_VALUE *value);                   // ���ó������

	protected:
		VOID SetProgramParam(UNIFORM_LOCATION *pParam);                                            // ���ó������
		VOID SetProgramParams(VOID);                                                               // ���ó������

	public:
		VOID SetMaterialAmbient(const FLOAT *ambient);                                             // ���ò��ʻ�����
		VOID SetMaterialDiffuse(const FLOAT *diffuse);                                             // ���ò����������
		VOID SetMaterialEmission(const FLOAT *emission);                                           // ���ò����Է���
		VOID SetMaterialSpecular(const FLOAT *specular);                                           // ���ò��ʸ߹�
		VOID SetMaterialShininess(FLOAT shininess);                                                // ���ò��ʾ���ָ��

		VOID SetLight(const FLOAT *position, const FLOAT *ambient, const FLOAT *diffuse, const FLOAT *specular, const FLOAT *rim, const FLOAT *skyLower, const FLOAT *skyUpper, const FLOAT *attenuation); // ���ù�Դ
		VOID SetShadowProjectionMatrix(const FLOAT *matrix);                                       // ������ӰͶӰ����
		VOID SetSunShaftsScreenSpacePosition(const FLOAT *position);                               // ����̫����������Ļ�ռ�λ��

	public:
		VERTEX* LockVertexBuffer(INT numVertices) const;                                           // �������㻺��
		VOID UnlockVertexBuffer(VERTEX *pVertices) const;                                          // �������㻺��

		INDEX* LockIndexBuffer(INT numIndices) const;                                              // ������������
		VOID UnlockIndexBuffer(INDEX *pIndices) const;                                             // ������������

	public:
		VOID SetRenderMode(UINT mode);                                                             // ������Ⱦģʽ
		VOID SetDepthMask(BOOL depth) const;                                                       // ����д���
		VOID SetColorMask(BOOL red, BOOL green, BOOL blue, BOOL alpha) const;                      // ����д��ɫ

		VOID SetReadBuffer(UINT mode) const;                                                       // ���ö�ȡ����
		VOID SetDrawBuffer(UINT mode) const;                                                       // �����������
		VOID SetDrawBuffers(INT n, const UINT *buffers) const;                                     // ���ö�·�������

		VOID SetPass(const PASS *pPass);                                                           // ���ò���ͨ·
		VOID SetPassAnimator(const PASS *pPass, FLOAT time);                                       // ���ò��ʶ���
		VOID SetPassProgram(const PASS_PROGRAM *pProgram);                                         // ���ò��ʳ���
		VOID SetPassTexture(const PASS_TEXTURE *pTexture, INT indexTexUnit);                       // ���ò�������
		VOID SetPassTextureAnimator(const PASS_TEXTURE *pTexture, INT indexTexUnit, FLOAT time);   // ���ò���������
		VOID SetPassCull(const PASS_CULL *pCull);                                                  // ���ò����޳�
		VOID SetPassDepth(const PASS_DEPTH *pDepth);                                               // ���ò������
		VOID SetPassAlpha(const PASS_ALPHA *pAlpha);                                               // ���ò���Alpha
		VOID SetPassBlend(const PASS_BLEND *pBlend);                                               // ���ò��ʻ��
		VOID SetPassLighting(const PASS_LIGHTING *pLighting);                                      // ���ò��ʹ���

		VOID SubmitVertexBuffer(UINT vao) const;                                                   // �ύ���㻺��
		VOID SubmitVertexBuffer(DWORD dwVertexFormat, const VERTEX *pVertices) const;              // �ύ���㻺��
		VOID SubmitMatrixBuffer(const FLOAT *matrices, INT numMatrices);                           // �ύ���󻺳�

		VOID BeginOcclude(VOID) const;                                                             // ��ʼ��Ⱦ�ڵ�
		VOID EndOcclude(VOID) const;                                                               // ������Ⱦ�ڵ�
		VOID RenderOcclude(const INDEX *indices, INT count);                                       // ��Ⱦ�ڵ�

		VOID BeginQuery(VOID) const;                                                               // ��ʼ��Ⱦ��ѯ
		VOID EndQuery(VOID) const;                                                                 // ������Ⱦ��ѯ
		VOID RenderQuery(UINT query, const AABB *aabb);                                            // ��Ⱦ��ѯ

		VOID BeginLighting(VOID) const;                                                            // ��ʼ��Ⱦ����
		VOID EndLighting(VOID) const;                                                              // ������Ⱦ����
		VOID RenderLighting(const FLOAT *position, FLOAT radius);                                  // ��Ⱦ����

		VOID Render(const INDEX *indices, INT count);                                              // ��Ⱦ
		VOID RenderRect(FLOAT left, FLOAT right, FLOAT top, FLOAT bottom);                         // ��Ⱦ����
		VOID RenderViewport(VOID);                                                                 // ��Ⱦ�ӿ�

		VOID Flush(VOID) const;                                                                    // ˢ��ָ���
		VOID Finish(VOID) const;                                                                   // ˢ��ָ��岢�ȴ�ִ�н���
		VOID SwapBuffers(VOID) const;                                                              // ��������

	public:
		INT GetFaces(VOID) const;                                                                  // �����Ⱦ����
		INT GetDrawCalls(VOID) const;                                                              // �����Ⱦ���ô���


		// ����
	protected:
		HDC m_hDC;                                                                                 // �豸������
		HGLRC m_hRC;                                                                               // ��Դ������
		HWND m_hWnd;                                                                               // ���ھ��

	protected:
		UINT m_maxTexUnits;                                                                        // �������Ԫ��
		UINT m_maxFBOAttachments;                                                                  // ���FBO�ҽ���
		UINT m_maxFBODrawBuffers;                                                                  // ���FBO�����

	protected:
		INT m_clearStencil;                                                                        // ��ģ��ֵ
		FLOAT m_clearDepth;                                                                        // �����ֵ
		FLOAT m_clearRed, m_clearGreen, m_clearBlue, m_clearAlpha;                                 // ����ɫֵ

	protected:
		UINT m_rectIndexBuffer;                                                                    // ������������
		UINT m_rectVertexBuffer;                                                                   // ���ζ��㻺��

		UINT m_boxIndexBuffer;                                                                     // ����������
		UINT m_boxVertexBuffer;                                                                    // �ж��㻺��

		UINT m_sphereIndexBuffer;                                                                  // ����������
		UINT m_sphereVertexBuffer;                                                                 // �򶥵㻺��

	protected:
		UINT m_renderMode;                                                                         // ��Ⱦģʽ

	protected:
		PROGRAM *m_pProgram;                                                                       // ��ǰ����
		ProgramMap m_programs;                                                                     // ���򼯺�
		UniformLocationMap m_programParams;                                                        // ��ɫ����������

	protected:
		INT m_draws;                                                                               // ��Ⱦ����
		INT m_faces;                                                                               // ��Ⱦ����
	};

}

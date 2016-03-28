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

#include "_CrossEngine.h"


namespace CrossEngine {

	const DWORD VERTEX_SIZE = 100;
	const DWORD POSITION_OFFSET = 0;
	const DWORD NORMAL_OFFSET = 12;
	const DWORD BINORMAL_OFFSET = 24;
	const DWORD COLOR_OFFSET = 36;
	const DWORD TEXCOORD_DIFFUSE_OFFSET = 52;
	const DWORD TEXCOORD_LIGHTMAP_OFFSET = 60;
	const DWORD INDICES_OFFSET = 68;
	const DWORD WEIGHTS_OFFSET = 84;

	const DWORD VERTEX_FORMAT_BITMASK_POSITION = 0x00000001;
	const DWORD VERTEX_FORMAT_BITMASK_NORMAL = 0x00000002;
	const DWORD VERTEX_FORMAT_BITMASK_BINORMAL = 0x00000004;
	const DWORD VERTEX_FORMAT_BITMASK_COLOR = 0x00000008;
	const DWORD VERTEX_FORMAT_BITMASK_TEXCOORD_DIFFUSE = 0x00000010;
	const DWORD VERTEX_FORMAT_BITMASK_TEXCOORD_LIGHTMAP = 0x00000020;
	const DWORD VERTEX_FORMAT_BITMASK_INDICES = 0x00000040;
	const DWORD VERTEX_FORMAT_BITMASK_WEIGHTS = 0x00000080;

	const DWORD CRenderer::VERTEX_FORMAT_SKINMESH = VERTEX_FORMAT_BITMASK_POSITION | VERTEX_FORMAT_BITMASK_NORMAL | VERTEX_FORMAT_BITMASK_BINORMAL | VERTEX_FORMAT_BITMASK_TEXCOORD_DIFFUSE | VERTEX_FORMAT_BITMASK_INDICES | VERTEX_FORMAT_BITMASK_WEIGHTS;
	const DWORD CRenderer::VERTEX_FORMAT_STATICMESH = VERTEX_FORMAT_BITMASK_POSITION | VERTEX_FORMAT_BITMASK_NORMAL | VERTEX_FORMAT_BITMASK_BINORMAL | VERTEX_FORMAT_BITMASK_TEXCOORD_DIFFUSE | VERTEX_FORMAT_BITMASK_TEXCOORD_LIGHTMAP;
	const DWORD CRenderer::VERTEX_FORMAT_DYNAMICMESH = VERTEX_FORMAT_BITMASK_POSITION | VERTEX_FORMAT_BITMASK_NORMAL | VERTEX_FORMAT_BITMASK_BINORMAL | VERTEX_FORMAT_BITMASK_TEXCOORD_DIFFUSE;
	const DWORD CRenderer::VERTEX_FORMAT_PARTICLE = VERTEX_FORMAT_BITMASK_POSITION | VERTEX_FORMAT_BITMASK_TEXCOORD_DIFFUSE | VERTEX_FORMAT_BITMASK_COLOR;
	const DWORD CRenderer::VERTEX_FORMAT_HALO = VERTEX_FORMAT_BITMASK_POSITION | VERTEX_FORMAT_BITMASK_TEXCOORD_DIFFUSE | VERTEX_FORMAT_BITMASK_COLOR;
	const DWORD CRenderer::VERTEX_FORMAT_SCREEN = VERTEX_FORMAT_BITMASK_POSITION | VERTEX_FORMAT_BITMASK_TEXCOORD_DIFFUSE;
	const DWORD CRenderer::VERTEX_FORMAT_POSITION = VERTEX_FORMAT_BITMASK_POSITION;


	typedef struct { CHAR name[32];            } ATTRIB_INFO;
	typedef struct { CHAR name[32]; UINT type; } UNIFORM_INFO;

	const ATTRIB_INFO ATTRIBS[CRenderer::ATTRIB_LOCATION_COUNT] = {
		"_position",                                                           // ATTRIB_LOCATION_POSITION
		"_normal",                                                             // ATTRIB_LOCATION_NORMAL
		"_binormal",                                                           // ATTRIB_LOCATION_BINORMAL
		"_color",                                                              // ATTRIB_LOCATION_COLOR
		"_texcoord0",                                                          // ATTRIB_LOCATION_TEXCOORDDIFFUSE
		"_texcoord1",                                                          // ATTRIB_LOCATION_TEXCOORDLIGHTMAP
		"_indices",                                                            // ATTRIB_LOCATION_INDICES
		"_weights"                                                             // ATTRIB_LOCATION_WEIGHTS
	};

	const UNIFORM_INFO UNIFORMS[CRenderer::UNIFORM_LOCATION_COUNT] = {
		{ "_viewport", CRenderer::UNIFORM_TYPE_FLOAT4 },                       // UNIFORM_LOCATION_VIEWPORT
		{ "_cameraPerspective", CRenderer::UNIFORM_TYPE_FLOAT4 },              // UNIFORM_LOCATION_CAMERAPERSPECTIVE
		{ "_cameraPosition", CRenderer::UNIFORM_TYPE_FLOAT3 },                 // UNIFORM_LOCATION_CAMERAPOSITION
		{ "_cameraForwardDirection", CRenderer::UNIFORM_TYPE_FLOAT3 },         // UNIFORM_LOCATION_CAMERAFORWARDDIRECTION
		{ "_cameraUpDirection", CRenderer::UNIFORM_TYPE_FLOAT3 },              // UNIFORM_LOCATION_CAMERAUPDIRECTION
		{ "_modelWorldMatrix", CRenderer::UNIFORM_TYPE_MATRIX4 },              // UNIFORM_LOCATION_MODELWORLDMATRIX
		{ "_modelViewMatrix", CRenderer::UNIFORM_TYPE_MATRIX4 },               // UNIFORM_LOCATION_MODELVIEWMATRIX
		{ "_modelViewInvTransposeMatrix", CRenderer::UNIFORM_TYPE_MATRIX4 },   // UNIFORM_LOCATION_MODELVIEWINVERSETRANSPOSEMATRIX
		{ "_modelViewProjectionMatrix", CRenderer::UNIFORM_TYPE_MATRIX4 },     // UNIFORM_LOCATION_MODELVIEWPROJECTIONMATRIX
		{ "_textureMatrix0", CRenderer::UNIFORM_TYPE_MATRIX4 },                // UNIFORM_LOCATION_TEXTUREMATRIX0
		{ "_textureMatrix1", CRenderer::UNIFORM_TYPE_MATRIX4 },                // UNIFORM_LOCATION_TEXTUREMATRIX1
		{ "_textureMatrix2", CRenderer::UNIFORM_TYPE_MATRIX4 },                // UNIFORM_LOCATION_TEXTUREMATRIX2
		{ "_textureMatrix3", CRenderer::UNIFORM_TYPE_MATRIX4 },                // UNIFORM_LOCATION_TEXTUREMATRIX3
		{ "_textureMatrix4", CRenderer::UNIFORM_TYPE_MATRIX4 },                // UNIFORM_LOCATION_TEXTUREMATRIX4
		{ "_textureMatrix5", CRenderer::UNIFORM_TYPE_MATRIX4 },                // UNIFORM_LOCATION_TEXTUREMATRIX5
		{ "_textureMatrix6", CRenderer::UNIFORM_TYPE_MATRIX4 },                // UNIFORM_LOCATION_TEXTUREMATRIX6
		{ "_textureMatrix7", CRenderer::UNIFORM_TYPE_MATRIX4 },                // UNIFORM_LOCATION_TEXTUREMATRIX7
		{ "_bones", CRenderer::UNIFORM_TYPE_MATRIX4 },                         // UNIFORM_LOCATION_BONES
		{ "_alphaFunc", CRenderer::UNIFORM_TYPE_INT },                         // UNIFORM_LOCATION_ALPHAFUNC
		{ "_alphaRef", CRenderer::UNIFORM_TYPE_FLOAT },                        // UNIFORM_LOCATION_ALPHAREF
		{ "_matAmbient", CRenderer::UNIFORM_TYPE_FLOAT4 },                     // UNIFORM_LOCATION_MATERIALAMBIENT
		{ "_matDiffuse", CRenderer::UNIFORM_TYPE_FLOAT4 },                     // UNIFORM_LOCATION_MATERIALDIFFUSE
		{ "_matEmission", CRenderer::UNIFORM_TYPE_FLOAT4 },                    // UNIFORM_LOCATION_MATERIALEMISSION
		{ "_matSpecular", CRenderer::UNIFORM_TYPE_FLOAT4 },                    // UNIFORM_LOCATION_MATERIALSPECULAR
		{ "_matShininess", CRenderer::UNIFORM_TYPE_FLOAT },                    // UNIFORM_LOCATION_MATERIALSHININESS
		{ "_lightPosition", CRenderer::UNIFORM_TYPE_FLOAT4 },                  // UNIFORM_LOCATION_LIGHTPOSITION
		{ "_lightAmbient", CRenderer::UNIFORM_TYPE_FLOAT3 },                   // UNIFORM_LOCATION_LIGHTAMBIENT
		{ "_lightDiffuse", CRenderer::UNIFORM_TYPE_FLOAT3 },                   // UNIFORM_LOCATION_LIGHTDIFFUSE
		{ "_lightSpecular", CRenderer::UNIFORM_TYPE_FLOAT3 },                  // UNIFORM_LOCATION_LIGHTSPECULAR
		{ "_lightRim", CRenderer::UNIFORM_TYPE_FLOAT3 },                       // UNIFORM_LOCATION_LIGHTRIM
		{ "_lightSkyLower", CRenderer::UNIFORM_TYPE_FLOAT3 },                  // UNIFORM_LOCATION_LIGHTSKYLOWER
		{ "_lightSkyUpper", CRenderer::UNIFORM_TYPE_FLOAT3 },                  // UNIFORM_LOCATION_LIGHTSKYUPPER
		{ "_lightAttenuation", CRenderer::UNIFORM_TYPE_FLOAT3 },               // UNIFORM_LOCATION_LIGHTATTENUATION
		{ "_shadowProjectionMatrix", CRenderer::UNIFORM_TYPE_MATRIX4 },        // UNIFORM_LOCATION_SHADOWPROJECTIONMATRIX
		{ "_sunShaftsScreenSpacePosition", CRenderer::UNIFORM_TYPE_FLOAT2 }    // UNIFORM_LOCATION_SUNSHAFTSSCREENSPACEPOSITION
	};


	CRenderer::CRenderer(HWND hWnd)
		: m_hDC(NULL)
		, m_hRC(NULL)
		, m_hWnd(NULL)

		, m_maxTexUnits(0)
		, m_maxFBOAttachments(0)
		, m_maxFBODrawBuffers(0)

		, m_clearStencil(0)
		, m_clearDepth(1.0f)
		, m_clearRed(0.0f)
		, m_clearGreen(0.0f)
		, m_clearBlue(0.0f)
		, m_clearAlpha(1.0f)

		, m_rectIndexBuffer(0)
		, m_rectVertexBuffer(0)
		, m_boxIndexBuffer(0)
		, m_boxVertexBuffer(0)
		, m_sphereIndexBuffer(0)
		, m_sphereVertexBuffer(0)

		, m_renderMode(GL_TRIANGLES)
		, m_pProgram(NULL)

		, m_draws(0)
		, m_faces(0)
	{
		Init(hWnd);
	}

	CRenderer::~CRenderer(VOID)
	{
		Free();
	}

	//
	// ��ʼ����Ⱦϵͳ
	//
	BOOL CRenderer::Init(HWND hWnd)
	{
#if defined _PLATFORM_WINDOWS_

		ASSERT(hWnd);

		//
		// 1. ��ʼ��GL
		//
		INT pixelFormat;

		PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {
			sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd 
			1,                              // version number 
			PFD_DRAW_TO_WINDOW |            // support window 
			PFD_SUPPORT_OPENGL |            // support OpenGL 
			PFD_DOUBLEBUFFER,               // double buffered 
			PFD_TYPE_RGBA,                  // RGBA type 
			32,                             // 32-bit color depth 
			0, 0, 0, 0, 0, 0,               // color bits ignored 
			0,                              // no alpha buffer 
			0,                              // shift bit ignored 
			0,                              // no accumulation buffer 
			0, 0, 0, 0,                     // accum bits ignored 
			24,                             // 24-bit z-buffer 
			8,                              // 8-bit stencil buffer 
			0,                              // no auxiliary buffer 
			PFD_MAIN_PLANE,                 // main layer 
			0,                              // reserved 
			0, 0, 0                         // layer masks ignored 
		};

		m_hWnd = hWnd;
		m_hDC = GetDC(m_hWnd);

		pixelFormat = ChoosePixelFormat(m_hDC, &pixelFormatDescriptor);
		SetPixelFormat(m_hDC, pixelFormat, &pixelFormatDescriptor);

		m_hRC = wglCreateContext(m_hDC);
		wglMakeCurrent(m_hDC, m_hRC);

		glewInit();

#endif

		InitGL();
		CreateRect();
		CreateBox();
		CreateSphere();

		//
		// 2. ���GL����
		//
		const CHAR *szVendor = GetString(GL_VENDOR);
		const CHAR *szVersion = GetString(GL_VERSION);
		const CHAR *szRenderer = GetString(GL_RENDERER);
		const CHAR *szExtension = GetString(GL_EXTENSIONS);
		const CHAR *szShaderLanguageVersion = GetString(GL_SHADING_LANGUAGE_VERSION);

		INT maxTexUnits = GetInteger(GL_MAX_TEXTURE_IMAGE_UNITS);
		INT maxFBOAttachments = GetInteger(GL_MAX_COLOR_ATTACHMENTS);
		INT maxFBODrawBuffers = GetInteger(GL_MAX_DRAW_BUFFERS);
		INT maxVertexAttribs = GetInteger(GL_MAX_VERTEX_ATTRIBS);
		INT maxVertexUniformVectors = GetInteger(GL_MAX_VERTEX_UNIFORM_VECTORS);
		INT maxFragmentUniformVectors = GetInteger(GL_MAX_FRAGMENT_UNIFORM_VECTORS);
		INT maxVaryingVectors = GetInteger(GL_MAX_VARYING_VECTORS);

		WriteLogI("GL Vendor: %s\n", szVendor);
		WriteLogI("GL Version: %s\n", szVersion);
		WriteLogI("GL Renderer: %s\n", szRenderer);
		WriteLogI("GL Extension: %s\n", szExtension);
		WriteLogI("GL Max Texture Units: %d\n", maxTexUnits);
		WriteLogI("GL Max FBO Attachments: %d\n", maxFBOAttachments);
		WriteLogI("GL Max FBO Draw Buffers: %d\n", maxFBODrawBuffers);
		WriteLogI("GL Shader Version: %s\n", szShaderLanguageVersion);
		WriteLogI("GL Shader Max Vertex Attribs: %d\n", maxVertexAttribs);
		WriteLogI("GL Shader Max Vertex Uniform Vectors: %d\n", maxVertexUniformVectors);
		WriteLogI("GL Shader Max Fragment Uniform Vectors: %d\n", maxFragmentUniformVectors);
		WriteLogI("GL Shader Max Varying Vectors: %d\n", maxVaryingVectors);
		WriteLogI("\n");

		m_maxTexUnits = min(maxTexUnits, MAX_TEXUNITS);
		m_maxFBOAttachments = min(maxFBOAttachments, MAX_DRAW_BUFFERS);
		m_maxFBODrawBuffers = min(maxFBODrawBuffers, MAX_DRAW_BUFFERS);

		return TRUE;
	}

	//
	// �ͷ���Ⱦϵͳ
	//
	VOID CRenderer::Free(VOID)
	{
		DestroyRect();
		DestroyBox();
		DestroySphere();
		DestroyProgramAll();
	}

	//
	// ��������
	//
	BOOL CRenderer::CreateRect(VOID)
	{
		INDEX indices[6];
		VERTEX vertices[4];

		memset(indices, 0, sizeof(indices));
		memset(vertices, 0, sizeof(vertices));

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 3;
		indices[5] = 0;

		vertices[0].position[0] = -1.0f; vertices[0].position[1] = -1.0f; vertices[0].position[2] = 0.0f;
		vertices[1].position[0] =  1.0f; vertices[1].position[1] = -1.0f; vertices[1].position[2] = 0.0f;
		vertices[2].position[0] =  1.0f; vertices[2].position[1] =  1.0f; vertices[2].position[2] = 0.0f;
		vertices[3].position[0] = -1.0f; vertices[3].position[1] =  1.0f; vertices[3].position[2] = 0.0f;
		vertices[0].texCoordDiffuse[0] = 0.0f; vertices[0].texCoordDiffuse[1] = 0.0f;
		vertices[1].texCoordDiffuse[0] = 1.0f; vertices[1].texCoordDiffuse[1] = 0.0f;
		vertices[2].texCoordDiffuse[0] = 1.0f; vertices[2].texCoordDiffuse[1] = 1.0f;
		vertices[3].texCoordDiffuse[0] = 0.0f; vertices[3].texCoordDiffuse[1] = 1.0f;

		m_rectIndexBuffer = CreateVBO(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		m_rectVertexBuffer = CreateVBO(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		return TRUE;
	}

	//
	// ���پ���
	//
	VOID CRenderer::DestroyRect(VOID)
	{
		DestroyVBO(&m_rectIndexBuffer);
		DestroyVBO(&m_rectVertexBuffer);
	}

	//
	// ������
	//
	BOOL CRenderer::CreateBox(VOID)
	{
		INDEX indices[] = {
#include "CrossBoxIndices.h"
		};

		FLOAT positions[] = {
#include "CrossBoxPositions.h"
		};

		INT numVertices = sizeof(positions) / (sizeof(*positions) * 3);
		VERTEX *vertices = (VERTEX *)SAFE_MALLOC(sizeof(*vertices) * numVertices, MEMTYPE_STACK);
		{
			memset(vertices, 0, sizeof(*vertices) * numVertices);

			for (INT indexVertex = 0; indexVertex < numVertices; indexVertex++) {
				vertices[indexVertex].position[0] = positions[indexVertex * 3 + 0];
				vertices[indexVertex].position[1] = positions[indexVertex * 3 + 1];
				vertices[indexVertex].position[2] = positions[indexVertex * 3 + 2];
			}

			m_boxIndexBuffer = CreateVBO(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			m_boxVertexBuffer = CreateVBO(GL_ARRAY_BUFFER, sizeof(*vertices) * numVertices, vertices, GL_STATIC_DRAW);
		}
		SAFE_FREE(vertices);

		return TRUE;
	}

	//
	// ���ٺ�
	//
	VOID CRenderer::DestroyBox(VOID)
	{
		DestroyVBO(&m_boxIndexBuffer);
		DestroyVBO(&m_boxVertexBuffer);
	}

	//
	// ������
	//
	BOOL CRenderer::CreateSphere(VOID)
	{
		INDEX indices[] = {
#include "CrossSphereIndices.h"
		};

		FLOAT positions[] = {
#include "CrossSpherePositions.h"
		};

		INT numVertices = sizeof(positions) / (sizeof(*positions) * 3);
		VERTEX *vertices = (VERTEX *)SAFE_MALLOC(sizeof(*vertices) * numVertices, MEMTYPE_STACK);
		{
			memset(vertices, 0, sizeof(*vertices) * numVertices);

			for (INT indexVertex = 0; indexVertex < numVertices; indexVertex++) {
				vertices[indexVertex].position[0] = positions[indexVertex * 3 + 0];
				vertices[indexVertex].position[1] = positions[indexVertex * 3 + 1];
				vertices[indexVertex].position[2] = positions[indexVertex * 3 + 2];
			}

			m_sphereIndexBuffer = CreateVBO(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			m_sphereVertexBuffer = CreateVBO(GL_ARRAY_BUFFER, sizeof(*vertices) * numVertices, vertices, GL_STATIC_DRAW);
		}
		SAFE_FREE(vertices);

		return TRUE;
	}

	//
	// ������
	//
	VOID CRenderer::DestroySphere(VOID)
	{
		DestroyVBO(&m_sphereIndexBuffer);
		DestroyVBO(&m_sphereVertexBuffer);
	}

	//
	// ��ô��ھ��
	//
	HWND CRenderer::GetHWND(VOID) const
	{
		return m_hWnd;
	}

	//
	// ����豸������
	//
	HDC CRenderer::GetHDC(VOID) const
	{
		return m_hDC;
	}

	//
	// �����Դ������
	//
	HGLRC CRenderer::GetHRC(VOID) const
	{
		return m_hRC;
	}

	//
	// �������Ԫ��
	//
	UINT CRenderer::GetMaxTexUnits(VOID) const
	{
		return m_maxTexUnits;
	}

	//
	// ���FBO�ҽ���
	//
	UINT CRenderer::GetMaxFBOAttachments(VOID) const
	{
		return m_maxFBOAttachments;
	}

	//
	// ���FBO�����
	//
	UINT CRenderer::GetMaxFBODrawBuffers(VOID) const
	{
		return m_maxFBODrawBuffers;
	}

	//
	// ��ò���
	//
	BYTE CRenderer::GetBoolen(UINT name) const
	{
		BYTE param;
		cglGetBooleanv(name, &param);
		return param;
	}

	//
	// ��ò���
	//
	FLOAT CRenderer::GetFloat(UINT name) const
	{
		FLOAT param;
		cglGetFloatv(name, &param);
		return param;
	}

	//
	// ��ò���
	//
	INT CRenderer::GetInteger(UINT name) const
	{
		INT param;
		cglGetIntegerv(name, &param);
		return param;
	}

	//
	// ��ò���
	//
	const CHAR* CRenderer::GetString(UINT name) const
	{
		return cglGetString(name);
	}

	//
	// ��ò���
	//
	VOID CRenderer::GetBooleanv(UINT name, BYTE *params) const
	{
		ASSERT(params);
		cglGetBooleanv(name, params);
	}

	//
	// ��ò���
	//
	VOID CRenderer::GetFloatv(UINT name, FLOAT *params) const
	{
		ASSERT(params);
		cglGetFloatv(name, params);
	}

	//
	// ��ò���
	//
	VOID CRenderer::GetIntegerv(UINT name, INT *params) const
	{
		ASSERT(params);
		cglGetIntegerv(name, params);
	}

	//
	// ����
	//
	VOID CRenderer::Enable(UINT cap) const
	{
		cglEnable(cap);
	}

	//
	// ����
	//
	VOID CRenderer::Disable(UINT cap) const
	{
		cglDisable(cap);
	}

	//
	// ״̬
	//
	BOOL CRenderer::IsEnable(UINT cap) const
	{
		return cglIsEnabled(cap);
	}

	//
	// �������޳�ģʽ
	//
	VOID CRenderer::SetCullFace(UINT mode) const
	{
		cglCullFace(mode);
	}

	//
	// �����涨��
	//
	VOID CRenderer::SetFrontFace(UINT mode) const
	{
		cglFrontFace(mode);
	}

	//
	// ������Ȳ��Է���
	//
	VOID CRenderer::SetDepthFunc(UINT func) const
	{
		cglDepthFunc(func);
	}

	//
	// ����Alpha���Է���
	//
	VOID CRenderer::SetAlphaFunc(UINT func, FLOAT ref) const
	{
		cglAlphaFunc(func, ref);
	}

	//
	// ���û�Ϸ���
	//
	VOID CRenderer::SetBlendEquation(UINT modergb, UINT modealpha) const
	{
		cglBlendEquationSeparate(modergb, modealpha);
	}

	//
	// ���û�Ϸ���
	//
	VOID CRenderer::SetBlendFunc(UINT srcrgb, UINT dstrgb, UINT srcalpha, UINT dstalpha) const
	{
		cglBlendFuncSeparate(srcrgb, dstrgb, srcalpha, dstalpha);
	}

	//
	// �������ģ��ֵ
	//
	VOID CRenderer::SetClearStencil(INT stencil)
	{
		m_clearStencil = stencil;
	}

	//
	// ����������ֵ
	//
	VOID CRenderer::SetClearDepth(FLOAT depth)
	{
		m_clearDepth = depth;
	}

	//
	// ���������ɫֵ
	//
	VOID CRenderer::SetClearColor(FLOAT red, FLOAT green, FLOAT blue, FLOAT alpha)
	{
		m_clearRed = red;
		m_clearGreen = green;
		m_clearBlue = blue;
		m_clearAlpha = alpha;
	}

	//
	// ��ջ���
	//
	VOID CRenderer::ClearBuffer(BOOL bColorBuffer, BOOL bDepthBuffer, BOOL bStencelBuffer) const
	{
		DWORD dwFlags = 0;

		if (bColorBuffer) dwFlags |= GL_COLOR_BUFFER_BIT;
		if (bDepthBuffer) dwFlags |= GL_DEPTH_BUFFER_BIT;
		if (bStencelBuffer) dwFlags |= GL_STENCIL_BUFFER_BIT;

		if (dwFlags) {
			if (bColorBuffer) cglClearColor(m_clearRed, m_clearGreen, m_clearBlue, m_clearAlpha);
			if (bDepthBuffer) cglClearDepth(m_clearDepth);
			if (bStencelBuffer) cglClearStencil(m_clearStencil);
			cglClear(dwFlags);
		}
	}

	//
	// ����ģ��Ƚ�����
	//
	VOID CRenderer::SetStencilFunc(UINT func, UINT ref, UINT mask) const
	{
		cglStencilFunc(func, ref, mask);
	}

	//
	// ����ģ����·���
	//
	VOID CRenderer::SetStencilOp(UINT sfail, UINT zfail, UINT zpass) const
	{
		cglStencilOp(sfail, zfail, zpass);
	}

	//
	// ���òü�
	//
	VOID CRenderer::SetScissor(INT x, INT y, INT width, INT height) const
	{
		cglScissor(x, y, width, height);
	}

	//
	// �����ӿ�
	//
	VOID CRenderer::SetViewport(INT x, INT y, INT width, INT height) const
	{
		cglViewport(x, y, width, height);
	}

	//
	// ����ӿ�X
	//
	INT CRenderer::GetViewportX(VOID) const
	{
		return cglGetViewportX();
	}

	//
	// ����ӿ�Y
	//
	INT CRenderer::GetViewportY(VOID) const
	{
		return cglGetViewportY();
	}

	//
	// ����ӿڿ�
	//
	INT CRenderer::GetViewportWidth(VOID) const
	{
		return cglGetViewportWidth();
	}

	//
	// ����ӿڸ�
	//
	INT CRenderer::GetViewportHeight(VOID) const
	{
		return cglGetViewportHeight();
	}

	//
	// ����͸��ͶӰ��׶
	//
	VOID CRenderer::SetPerspective(FLOAT fovy, FLOAT aspect, FLOAT znear, FLOAT zfar) const
	{
		cglMatrixMode(GL_PROJECTION);
		cglLoadIdentity();
		cglPerspective(fovy, aspect, znear, zfar);
	}

	//
	// ��������ͶӰ��׶
	//
	VOID CRenderer::SetOrtho(FLOAT left, FLOAT right, FLOAT bottom, FLOAT top, FLOAT znear, FLOAT zfar) const
	{
		cglMatrixMode(GL_PROJECTION);
		cglLoadIdentity();
		cglOrtho(left, right, bottom, top, znear, zfar);
	}

	//
	// ����ѹջ
	//
	VOID CRenderer::PushMatrix(VOID) const
	{
		cglMatrixMode(GL_MODELWORLD);
		cglPushMatrix();

		cglMatrixMode(GL_MODELVIEW);
		cglPushMatrix();
	}

	//
	// ����ջ
	//
	VOID CRenderer::PopMatrix(VOID) const
	{
		cglMatrixMode(GL_MODELWORLD);
		cglPopMatrix();

		cglMatrixMode(GL_MODELVIEW);
		cglPopMatrix();
	}

	//
	// �������ѹջ
	//
	VOID CRenderer::PushTextureMatrix(VOID) const
	{
		for (INT indexTexUnit = 0; indexTexUnit < (INT)m_maxTexUnits; indexTexUnit++) {
			cglMatrixMode(GL_TEXTURE0 + indexTexUnit);
			cglPushMatrix();
		}
	}

	//
	// �������ջ
	//
	VOID CRenderer::PopTextureMatrix(VOID) const
	{
		for (INT indexTexUnit = 0; indexTexUnit < (INT)m_maxTexUnits; indexTexUnit++) {
			cglMatrixMode(GL_TEXTURE0 + indexTexUnit);
			cglPopMatrix();
		}
	}

	//
	// ��������۲췽λ
	//
	VOID CRenderer::SetLookAt(const FLOAT *position, const FLOAT *direction, const FLOAT *up) const
	{
		ASSERT(position);
		ASSERT(direction);
		ASSERT(up);

		VEC3 center;
		Vec3Ma(&center, &(VEC3)position, &(VEC3)direction, 1.0f);

		cglMatrixMode(GL_MODELWORLD);
		cglLoadIdentity();

		cglMatrixMode(GL_MODELVIEW);
		cglLoadIdentity();

		cglLookAt(position[0], position[1], position[2], center[0], center[1], center[2], up[0], up[1], up[2]);
	}

	//
	// ����ģ������ϵ����������ϵ����
	//
	VOID CRenderer::SetModelWorldMatrix(const FLOAT *matrix) const
	{
		ASSERT(matrix);

		cglMatrixMode(GL_MODELWORLD);
		cglMultMatrixf(matrix);

		cglMatrixMode(GL_MODELVIEW);
		cglMultMatrixf(matrix);
	}

	//
	// �����������
	//
	VOID CRenderer::SetTextureMatrix(INT indexTexUnit, const FLOAT *matrix) const
	{
		ASSERT(matrix);
		ASSERT(indexTexUnit >= 0 && indexTexUnit < (INT)m_maxTexUnits);

		cglMatrixMode(GL_TEXTURE0 + indexTexUnit);
		cglLoadIdentity();
		cglMultMatrixf(matrix);
	}

	//
	// ���ģ������任����
	//
	FLOAT* CRenderer::GetModelWorldMatrix(VOID) const
	{
		return cglGetModelWorldMatrix();
	}

	//
	// ���ģ����ͼ�任����
	//
	FLOAT* CRenderer::GetModelViewMatrix(VOID) const
	{
		return cglGetModelViewMatrix();
	}

	//
	// ���ģ����ͼ�任�����
	//
	FLOAT* CRenderer::GetModelViewInverseTransposeMatrix(VOID) const
	{
		return cglGetModelViewInverseTransposeMatrix();
	}

	//
	// ���ģ����ͼͶӰ�任����
	//
	FLOAT* CRenderer::GetModelViewProjectionMatrix(VOID) const
	{
		return cglGetModelViewProjectionMatrix();
	}

	//
	// �������任����
	//
	FLOAT* CRenderer::GetTextureMatrix(INT indexTexUnit) const
	{
		ASSERT(indexTexUnit >= 0 && indexTexUnit < (INT)m_maxTexUnits);
		return cglGetTextureMatrix(GL_TEXTURE0 + indexTexUnit);
	}

	//
	// ��������
	//
	UINT CRenderer::CreateTexture2D(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT format, INT type, const BYTE *pixels) const
	{
		ASSERT(width > 0);
		ASSERT(height > 0);

		UINT texture = 0;
		cglGenTextures(1, &texture);
		cglBindTexture(target, texture);
		cglTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
		cglBindTexture(target, 0);
		return texture;
	}

	//
	// ��������
	//
	UINT CRenderer::CreateTexture2DMipmapsBMP(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT format, INT type, INT bitcount, const BYTE *pixels) const
	{
		ASSERT(pixels);
		ASSERT(width > 0);
		ASSERT(height > 0);

		UINT texture = 0;
		cglGenTextures(1, &texture);
		cglBindTexture(target, texture);
		cglTexImage2DMipmaps(target, max(width, height), level, internalformat, width, height, border, format, type, bitcount, pixels);
		cglBindTexture(target, 0);
		return texture;
	}

	//
	// ��������
	//
	UINT CRenderer::CreateTexture2DMipmapsDDS(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT bitcount, const BYTE *pixels) const
	{
		ASSERT(pixels);
		ASSERT(width > 0);
		ASSERT(height > 0);

		UINT texture = 0;
		cglGenTextures(1, &texture);
		cglBindTexture(target, texture);
		cglCompressedTexImage2DMipmapsDDS(target, max(width, height), level, internalformat, width, height, border, bitcount, pixels);
		cglBindTexture(target, 0);
		return texture;
	}

	//
	// ��������
	//
	UINT CRenderer::CreateTexture2DMipmapsPVR(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT bitcount, const BYTE *pixels) const
	{
		ASSERT(pixels);
		ASSERT(width > 0);
		ASSERT(height > 0);

		UINT texture = 0;
		cglGenTextures(1, &texture);
		cglBindTexture(target, texture);
		cglCompressedTexImage2DMipmapsPVR(target, max(width, height), level, internalformat, width, height, border, bitcount, pixels);
		cglBindTexture(target, 0);
		return texture;
	}

	//
	// ��������
	//
	UINT CRenderer::CreateTexture2DMipmapsETC(UINT target, INT level, INT internalformat, INT width, INT height, INT border, INT bitcount, const BYTE *pixels) const
	{
		ASSERT(pixels);
		ASSERT(width > 0);
		ASSERT(height > 0);

		UINT texture = 0;
		cglGenTextures(1, &texture);
		cglBindTexture(target, texture);
		cglCompressedTexImage2DMipmapsETC(target, max(width, height), level, internalformat, width, height, border, bitcount, pixels);
		cglBindTexture(target, 0);
		return texture;
	}

	//
	// ��������
	//
	VOID CRenderer::DestroyTexture(UINT *texture) const
	{
		ASSERT(texture);
		cglDeleteTextures(1, texture);
		*texture = 0;
	}

	//
	// ������
	//
	VOID CRenderer::BindTexture(UINT target, UINT texture) const
	{
		cglBindTexture(target, texture);
	}

	//
	// ��������
	//
	VOID CRenderer::ActiveTexture(UINT texture) const
	{
		cglActiveTexture(texture);
	}

	//
	// �����������
	//
	VOID CRenderer::TextureParameter(UINT target, UINT name, INT param) const
	{
		cglTexParameteri(target, name, param);
	}

	//
	// ��������
	//
	VOID CRenderer::SetTexture(INT indexTexUnit, UINT target, UINT texture, INT minFilter, INT maxFilter, INT warpU, INT warpV) const
	{
		ASSERT(indexTexUnit >= 0 && indexTexUnit < (INT)m_maxTexUnits);

		ActiveTexture(GL_TEXTURE0 + indexTexUnit);
		{
			if (texture > 0) {
				Enable(GL_TEXTURE_2D);
				BindTexture(target, texture);

				TextureParameter(target, GL_TEXTURE_MIN_FILTER, minFilter);
				TextureParameter(target, GL_TEXTURE_MAG_FILTER, maxFilter);
				TextureParameter(target, GL_TEXTURE_WRAP_S, warpU);
				TextureParameter(target, GL_TEXTURE_WRAP_T, warpV);
			}
			else {
				Disable(GL_TEXTURE_2D);
			}
		}
		ActiveTexture(GL_TEXTURE0);
	}

	//
	// ���ù�������
	//
	VOID CRenderer::SetLightMapTexture(UINT texture)
	{
		SetTexture(1, GL_TEXTURE_2D, texture, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	}

	//
	// ����VAO
	//
	UINT CRenderer::CreateVAO(VOID) const
	{
		UINT vao = 0;
		cglGenVertexArrays(1, &vao);
		return vao;
	}

	//
	// ����VAO
	//
	VOID CRenderer::DestroyVAO(UINT *vao) const
	{
		ASSERT(vao);
		cglDeleteVertexArrays(1, vao);
		*vao = 0;
	}

	//
	// ��VAO
	//
	VOID CRenderer::BindVAO(UINT vao) const
	{
		cglBindVertexArray(vao);
	}

	//
	// ����VBO
	//
	UINT CRenderer::CreateVBO(UINT target, INT size, const VOID* data, UINT usage) const
	{
		ASSERT(data);
		ASSERT(size > 0);

		UINT vbo = 0;
		cglGenBuffers(1, &vbo);
		cglBindBuffer(target, vbo);
		cglBufferData(target, size, data, usage);
		cglBindBuffer(target, 0);
		return vbo;
	}

	//
	// ����VBO
	//
	VOID CRenderer::DestroyVBO(UINT *vbo) const
	{
		ASSERT(vbo);
		cglDeleteBuffers(1, vbo);
		*vbo = 0;
	}

	//
	// ��VBO
	//
	VOID CRenderer::BindVBO(UINT target, UINT vbo) const
	{
		cglBindBuffer(target, vbo);
	}

	//
	// ����VBO
	//
	VOID CRenderer::UpdateVBO(UINT target, INT offset, INT size, const VOID *data) const
	{
		ASSERT(data);
		ASSERT(size > 0);

		cglBufferSubData(target, offset, size, data);
	}

	//
	// ������Ⱦ����
	//
	UINT CRenderer::CreateRenderBuffer(VOID) const
	{
		UINT buffer = 0;
		cglGenRenderbuffers(1, &buffer);
		return buffer;
	}

	//
	// ������Ⱦ����
	//
	VOID CRenderer::DestroyRenderBuffer(UINT *buffer) const
	{
		ASSERT(buffer);
		cglDeleteRenderbuffers(1, buffer);
		*buffer = 0;
	}

	//
	// ����Ⱦ����
	//
	VOID CRenderer::BindRenderBuffer(UINT buffer) const
	{
		cglBindRenderbuffer(GL_RENDERBUFFER, buffer);
	}

	//
	// ������Ⱦ����洢��ʽ
	//
	VOID CRenderer::RenderBufferFormat(UINT internalformat, INT width, INT height) const
	{
		ASSERT(width > 0);
		ASSERT(height > 0);

		cglRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
	}

	//
	// ����FBO
	//
	UINT CRenderer::CreateFrameBuffer(VOID) const
	{
		UINT fbo = 0;
		cglGenFramebuffers(1, &fbo);
		return fbo;
	}

	//
	// ����FBO
	//
	VOID CRenderer::DestroyFrameBuffer(UINT *fbo) const
	{
		ASSERT(fbo);
		cglDeleteFramebuffers(1, fbo);
		*fbo = 0;
	}

	//
	// ��FBO
	//
	VOID CRenderer::BindFrameBuffer(UINT fbo) const
	{
		cglBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}

	//
	// ����FBO��Ⱦ����
	//
	VOID CRenderer::FrameBufferRenderBuffer(UINT attachment, UINT buffer) const
	{
		cglFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, buffer);
	}

	//
	// ����FBO��Ⱦ����
	//
	VOID CRenderer::FrameBufferTexture2D(UINT attachment, UINT texture, INT level) const
	{
		cglFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, level);
	}

	//
	// ���FBO״̬
	//
	BOOL CRenderer::CheckFrameBufferStatus(VOID) const
	{
		UINT status = cglCheckFramebufferStatus(GL_FRAMEBUFFER);
		return status == GL_FRAMEBUFFER_COMPLETE ? TRUE : FALSE;
	}

	//
	// �����ڵ���ѯ
	//
	UINT CRenderer::CreateQuery(VOID) const
	{
		UINT query = 0;
		cglGenQueries(1, &query);
		return query;
	}

	//
	// �����ڵ���ѯ
	//
	VOID CRenderer::DestroyQuery(UINT *query) const
	{
		ASSERT(query);
		cglDeleteQueries(1, query);
		*query = 0;
	}

	//
	// ��ѯ���
	//
	BOOL CRenderer::QueryResult(UINT query) const
	{
		UINT result = 0;
		cglGetQueryObjectuiv(query, GL_QUERY_RESULT, &result);
		return result;
	}

	//
	// ��ѯ��������ж�
	//
	BOOL CRenderer::QueryResultAvailable(UINT query) const
	{
		UINT result = 0;
		cglGetQueryObjectuiv(query, GL_QUERY_RESULT_AVAILABLE, &result);
		return result;
	}

	//
	// ������ɫ��
	//
	UINT CRenderer::CreateShader(UINT type, const CHAR *source) const
	{
		ASSERT(source);

		//
		// 1. ������ɫ��������
		//
		UINT shader = 0;

		static const CHAR *szVersion100 = "#version 100"; // GLES2
		static const CHAR *szVersion300 = "#version 300 es"; // GLES3
		const CHAR *szShaderCode[2] = { IsOpenGLES3() ? szVersion300 : szVersion100, source };

		shader = cglCreateShader(type);
		cglShaderSource(shader, 2, szShaderCode, NULL);
		cglCompileShader(shader);

		//
		// 2. ��������
		//
		INT len, compiled = GL_FALSE;
		static CHAR szLog[100 * 1024] = { 0 };

		cglGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (compiled == GL_FALSE) {
			cglGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
			cglGetShaderInfoLog(shader, sizeof(szLog), &len, szLog);
			cglDeleteShader(shader);
			shader = 0;

			WriteLogE("Compile shader fail: \n");
			WriteLogE(szLog);
		}

		return shader;
	}

	//
	// ������ɫ��
	//
	VOID CRenderer::DestroyShader(UINT *shader) const
	{
		ASSERT(shader);
		cglDeleteShader(*shader);
		*shader = 0;
	}

	//
	// ��������
	//
	UINT CRenderer::CreateProgram(UINT vertex, UINT fragment) const
	{
		//
		// 1. ������������
		//
		UINT program = 0;

		program = cglCreateProgram();
		cglAttachShader(program, vertex);
		cglAttachShader(program, fragment);
		cglLinkProgram(program);

		//
		// 2. ������ӽ��
		//
		INT len, linked = GL_FALSE;
		static CHAR szLog[100 * 1024] = { 0 };

		cglGetProgramiv(program, GL_LINK_STATUS, &linked);

		if (linked == GL_FALSE) {
			cglGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
			cglGetProgramInfoLog(program, sizeof(szLog), &len, szLog);
			cglDeleteProgram(program);
			program = 0;

			WriteLogE("Link program fail: \n");
			WriteLogE(szLog);
		}

		return program;
	}

	//
	// ��������
	//
	BOOL CRenderer::CreateProgram(UINT vertex, UINT fragment, const UniformLocationMap &params)
	{
		UINT64 key = ((UINT64)vertex << 32) | (UINT64)fragment;

		//
		// 1. ���ҳ���
		//
		ProgramMap::const_iterator itProgram = m_programs.find(key);
		if (itProgram != m_programs.end()) return TRUE;

		//
		// 2. ��������
		//
		UINT program = CreateProgram(vertex, fragment);
		if (program == 0) return FALSE;

		//
		// 3. �������
		//
		PROGRAM *pProgram = SAFE_NEW(MEMTYPE_HEAP) PROGRAM;
		if (pProgram == NULL) return FALSE;

		//
		// 4. ��ʼ������
		//
		pProgram->program = program;

		//
		// 5. ��ʼ���������Լ���
		//
		memset(pProgram->attribLocations, 0, sizeof(pProgram->attribLocations));
		{
			for (INT indexAttrib = 0; indexAttrib < ATTRIB_LOCATION_COUNT; indexAttrib++) {
				strcpy(m_pProgram->attribLocations[indexAttrib].name, ATTRIBS[indexAttrib].name);
				m_pProgram->attribLocations[indexAttrib].location = cglGetAttribLocation(m_pProgram->program, m_pProgram->attribLocations[indexAttrib].name);
			}
		}

		//
		// 6. ��ʼ����ɫ���ڲ���������
		//
		memset(pProgram->uniformLocations, 0, sizeof(pProgram->uniformLocations));
		{
			for (INT index = 0; index < UNIFORM_LOCATION_COUNT; index++) {
				strcpy(pProgram->uniformLocations[index].name, UNIFORMS[index].name);
				pProgram->uniformLocations[index].type = UNIFORMS[index].type;
				pProgram->uniformLocations[index].location = cglGetUniformLocation(program, pProgram->uniformLocations[index].name);
			}
		}

		//
		// 7. ��ʼ����ɫ���ⲿ��������
		//
		pProgram->uniformExternalLocations.clear();
		{
			for (UniformLocationMap::const_iterator itParam = params.begin(); itParam != params.end(); ++itParam) {
				pProgram->uniformExternalLocations[itParam->first] = itParam->second;
				pProgram->uniformExternalLocations[itParam->first].location = cglGetUniformLocation(program, itParam->second.name);
			}
		}

		//
		// 8. �������
		//
		m_programs[key] = pProgram;

		//
		// 9. ʹ�ó���
		//
		return UseProgram(vertex, fragment, params);
	}

	//
	// ���ٳ���
	//
	VOID CRenderer::DestroyProgram(UINT vertex, UINT fragment)
	{
		UINT64 key = ((UINT64)vertex << 32) | (UINT64)fragment;

		//
		// 1. ���ҳ���
		//
		ProgramMap::const_iterator itProgram = m_programs.find(key);
		if (itProgram == m_programs.end()) return;

		//
		// 2. �ͷų���
		//
		const PROGRAM *pProgram = itProgram->second;
		ASSERT(pProgram);

		if (m_pProgram == pProgram) {
			m_pProgram = NULL;
		}

		cglDeleteProgram(pProgram->program);
		SAFE_DELETE(pProgram);

		//
		// 3. �ӳ��򼯺���ɾ��
		//
		m_programs.erase(itProgram);
	}

	//
	// �������г���
	//
	VOID CRenderer::DestroyProgramAll(VOID)
	{
		//
		// 1. �ͷ����г���
		//
		for (ProgramMap::const_iterator itProgram = m_programs.begin(); itProgram != m_programs.end(); ++itProgram) {
			const PROGRAM *pProgram = itProgram->second;
			ASSERT(pProgram);

			cglDeleteProgram(pProgram->program);
			SAFE_DELETE(pProgram);
		}

		//
		// 2. ��ճ��򼯺�
		//
		m_pProgram = NULL;
		m_programs.clear();
	}

	//
	// ʹ�ó���
	//
	BOOL CRenderer::UseProgram(UINT vertex, UINT fragment, const UniformLocationMap &params)
	{
		UINT64 key = ((UINT64)vertex << 32) | (UINT64)fragment;

		//
		// 1. ���ҳ���
		//
		ProgramMap::const_iterator itProgram = m_programs.find(key);
		m_pProgram = itProgram != m_programs.end() ? itProgram->second : NULL;

		//
		// 2. ʹ�ó������ò���
		//
		if (m_pProgram && m_pProgram->program) {
			cglUseProgram(m_pProgram->program);

			for (UniformLocationMap::iterator itParamDefault = m_pProgram->uniformExternalLocations.begin(); itParamDefault != m_pProgram->uniformExternalLocations.end(); ++itParamDefault) {
				UniformLocationMap::const_iterator itParamCustom = params.find(itParamDefault->first);
				UniformLocationMap::const_iterator itParamGlobal = m_programParams.find(itParamDefault->first);

				if      (itParamGlobal != m_programParams.end()) itParamDefault->second.value = itParamGlobal->second.value;
				else if (itParamCustom != params.end())          itParamDefault->second.value = itParamCustom->second.value;

				SetProgramParam(&itParamDefault->second);
			}

			return TRUE;
		}

		return FALSE;
	}

	//
	// ���ó������
	//
	VOID CRenderer::SetProgramParam(const CHAR *szName, const UNIFORM_LOCATION_VALUE *value)
	{
		ASSERT(value);
		ASSERT(szName);
		SetProgramParam(HashValue(szName), value);
	}

	//
	// ���ó������
	//
	VOID CRenderer::SetProgramParam(DWORD dwName, const UNIFORM_LOCATION_VALUE *value)
	{
		ASSERT(value);
		m_programParams[dwName].value = *value;
	}

	//
	// ���ó������
	//
	VOID CRenderer::SetProgramParam(UNIFORM_LOCATION *pParam)
	{
		ASSERT(pParam);

		if (pParam && pParam->location >= 0) {
			switch (pParam->type) {
			case UNIFORM_TYPE_INT: cglUniform1iv(pParam->location, 1, pParam->value.nvalue, pParam->cache.nvalue); break;
			case UNIFORM_TYPE_INT2: cglUniform2iv(pParam->location, 1, pParam->value.nvalue, pParam->cache.nvalue); break;
			case UNIFORM_TYPE_INT3: cglUniform3iv(pParam->location, 1, pParam->value.nvalue, pParam->cache.nvalue); break;
			case UNIFORM_TYPE_INT4: cglUniform4iv(pParam->location, 1, pParam->value.nvalue, pParam->cache.nvalue); break;
			case UNIFORM_TYPE_FLOAT: cglUniform1fv(pParam->location, 1, pParam->value.fvalue, pParam->cache.fvalue); break;
			case UNIFORM_TYPE_FLOAT2: cglUniform2fv(pParam->location, 1, pParam->value.fvalue, pParam->cache.fvalue); break;
			case UNIFORM_TYPE_FLOAT3: cglUniform3fv(pParam->location, 1, pParam->value.fvalue, pParam->cache.fvalue); break;
			case UNIFORM_TYPE_FLOAT4: cglUniform4fv(pParam->location, 1, pParam->value.fvalue, pParam->cache.fvalue); break;
			case UNIFORM_TYPE_MATRIX2: cglUniformMatrix2fv(pParam->location, 1, GL_FALSE, pParam->value.fvalue, pParam->cache.fvalue); break;
			case UNIFORM_TYPE_MATRIX3: cglUniformMatrix3fv(pParam->location, 1, GL_FALSE, pParam->value.fvalue, pParam->cache.fvalue); break;
			case UNIFORM_TYPE_MATRIX4: cglUniformMatrix4fv(pParam->location, 1, GL_FALSE, pParam->value.fvalue, pParam->cache.fvalue); break;
			}
		}
	}

	//
	// ���ó������
	//
	VOID CRenderer::SetProgramParams(VOID)
	{
		if (m_pProgram && m_pProgram->program) {
			m_pProgram->uniformLocations[UNIFORM_LOCATION_VIEWPORT].value.fvalue[0] = 1.0f * cglGetViewportX();
			m_pProgram->uniformLocations[UNIFORM_LOCATION_VIEWPORT].value.fvalue[1] = 1.0f * cglGetViewportY();
			m_pProgram->uniformLocations[UNIFORM_LOCATION_VIEWPORT].value.fvalue[2] = 1.0f * cglGetViewportWidth();
			m_pProgram->uniformLocations[UNIFORM_LOCATION_VIEWPORT].value.fvalue[3] = 1.0f * cglGetViewportHeight();
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_VIEWPORT]);

			m_pProgram->uniformLocations[UNIFORM_LOCATION_CAMERAPERSPECTIVE].value.fvalue[0] = cglGetCameraFovy();
			m_pProgram->uniformLocations[UNIFORM_LOCATION_CAMERAPERSPECTIVE].value.fvalue[1] = cglGetCameraAspect();
			m_pProgram->uniformLocations[UNIFORM_LOCATION_CAMERAPERSPECTIVE].value.fvalue[2] = cglGetCameraZNear();
			m_pProgram->uniformLocations[UNIFORM_LOCATION_CAMERAPERSPECTIVE].value.fvalue[3] = cglGetCameraZFar();
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_CAMERAPOSITION].value.fvalue, cglGetCameraPosition(), sizeof(FLOAT) * 3);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_CAMERAFORWARDDIRECTION].value.fvalue, cglGetCameraForwardDirection(), sizeof(FLOAT) * 3);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_CAMERAUPDIRECTION].value.fvalue, cglGetCameraUpDirection(), sizeof(FLOAT) * 3);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_CAMERAPERSPECTIVE]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_CAMERAPOSITION]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_CAMERAFORWARDDIRECTION]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_CAMERAUPDIRECTION]);

			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_MODELWORLDMATRIX].value.fvalue, cglGetModelWorldMatrix(), sizeof(FLOAT) * 16);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_MODELVIEWMATRIX].value.fvalue, cglGetModelViewMatrix(), sizeof(FLOAT) * 16);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_MODELVIEWINVERSETRANSPOSEMATRIX].value.fvalue, cglGetModelViewInverseTransposeMatrix(), sizeof(FLOAT) * 16);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_MODELVIEWPROJECTIONMATRIX].value.fvalue, cglGetModelViewProjectionMatrix(), sizeof(FLOAT) * 16);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_MODELWORLDMATRIX]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_MODELVIEWMATRIX]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_MODELVIEWINVERSETRANSPOSEMATRIX]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_MODELVIEWPROJECTIONMATRIX]);

			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX0].value.fvalue, cglGetTextureMatrix(GL_TEXTURE0), sizeof(FLOAT) * 16);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX1].value.fvalue, cglGetTextureMatrix(GL_TEXTURE1), sizeof(FLOAT) * 16);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX2].value.fvalue, cglGetTextureMatrix(GL_TEXTURE2), sizeof(FLOAT) * 16);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX3].value.fvalue, cglGetTextureMatrix(GL_TEXTURE3), sizeof(FLOAT) * 16);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX4].value.fvalue, cglGetTextureMatrix(GL_TEXTURE4), sizeof(FLOAT) * 16);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX5].value.fvalue, cglGetTextureMatrix(GL_TEXTURE5), sizeof(FLOAT) * 16);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX6].value.fvalue, cglGetTextureMatrix(GL_TEXTURE6), sizeof(FLOAT) * 16);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX7].value.fvalue, cglGetTextureMatrix(GL_TEXTURE7), sizeof(FLOAT) * 16);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX0]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX1]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX2]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX3]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX4]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX5]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX6]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_TEXTUREMATRIX7]);

			m_pProgram->uniformExternalLocations[UNIFORM_LOCATION_ALPHAFUNC].value.nvalue[0] = cglGetAlphaFunc();
			m_pProgram->uniformExternalLocations[UNIFORM_LOCATION_ALPHAREF].value.fvalue[0] = cglGetAlphaRef();
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_ALPHAFUNC]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_ALPHAREF]);
		}
	}

	//
	// ���ò��ʻ�����
	//
	VOID CRenderer::SetMaterialAmbient(const FLOAT *ambient)
	{
		ASSERT(ambient);

		if (m_pProgram && m_pProgram->program) {
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_MATERIALAMBIENT].value.fvalue, ambient, sizeof(FLOAT) * 4);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_MATERIALAMBIENT]);
		}
	}

	//
	// ���ò����������
	//
	VOID CRenderer::SetMaterialDiffuse(const FLOAT *diffuse)
	{
		ASSERT(diffuse);

		if (m_pProgram && m_pProgram->program) {
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_MATERIALDIFFUSE].value.fvalue, diffuse, sizeof(FLOAT) * 4);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_MATERIALDIFFUSE]);
		}
	}

	//
	// ���ò����Է���
	//
	VOID CRenderer::SetMaterialEmission(const FLOAT *emission)
	{
		ASSERT(emission);

		if (m_pProgram && m_pProgram->program) {
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_MATERIALEMISSION].value.fvalue, emission, sizeof(FLOAT) * 4);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_MATERIALEMISSION]);
		}
	}

	//
	// ���ò��ʸ߹�
	//
	VOID CRenderer::SetMaterialSpecular(const FLOAT *specular)
	{
		ASSERT(specular);

		if (m_pProgram && m_pProgram->program) {
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_MATERIALSPECULAR].value.fvalue, specular, sizeof(FLOAT) * 4);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_MATERIALSPECULAR]);
		}
	}

	//
	// ���ò��ʾ���ָ��
	//
	VOID CRenderer::SetMaterialShininess(FLOAT shininess)
	{
		if (m_pProgram && m_pProgram->program) {
			m_pProgram->uniformLocations[UNIFORM_LOCATION_MATERIALSHININESS].value.fvalue[0] = shininess;
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_MATERIALSHININESS]);
		}
	}

	//
	// ���ù�Դ
	//
	VOID CRenderer::SetLight(const FLOAT *position, const FLOAT *ambient, const FLOAT *diffuse, const FLOAT *specular, const FLOAT *rim, const FLOAT *skyLower, const FLOAT *skyUpper, const FLOAT *attenuation)
	{
		ASSERT(position);
		ASSERT(ambient);
		ASSERT(diffuse);
		ASSERT(specular);
		ASSERT(rim);
		ASSERT(skyLower);
		ASSERT(skyUpper);
		ASSERT(attenuation);

		if (m_pProgram && m_pProgram->program) {
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTPOSITION].value.fvalue, position, sizeof(FLOAT) * 4);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTAMBIENT].value.fvalue, ambient, sizeof(FLOAT) * 3);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTDIFFUSE].value.fvalue, diffuse, sizeof(FLOAT) * 3);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTSPECULAR].value.fvalue, specular, sizeof(FLOAT) * 3);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTRIM].value.fvalue, rim, sizeof(FLOAT) * 3);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTSKYLOWER].value.fvalue, skyLower, sizeof(FLOAT) * 3);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTSKYUPPER].value.fvalue, skyUpper, sizeof(FLOAT) * 3);
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTATTENUATION].value.fvalue, attenuation, sizeof(FLOAT) * 3);

			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTPOSITION]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTAMBIENT]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTDIFFUSE]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTSPECULAR]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTRIM]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTSKYLOWER]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTSKYUPPER]);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_LIGHTATTENUATION]);
		}
	}

	//
	// ������ӰͶӰ����
	//
	VOID CRenderer::SetShadowProjectionMatrix(const FLOAT *matrix)
	{
		ASSERT(matrix);

		if (m_pProgram && m_pProgram->program) {
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_SHADOWPROJECTIONMATRIX].value.fvalue, matrix, sizeof(FLOAT) * 16);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_SHADOWPROJECTIONMATRIX]);
		}
	}

	//
	// ����̫����������Ļ�ռ�λ��
	//
	VOID CRenderer::SetSunShaftsScreenSpacePosition(const FLOAT *position)
	{
		ASSERT(position);

		if (m_pProgram && m_pProgram->program) {
			memcpy(m_pProgram->uniformLocations[UNIFORM_LOCATION_SUNSHAFTSSCREENSPACEPOSITION].value.fvalue, position, sizeof(FLOAT) * 2);
			SetProgramParam(&m_pProgram->uniformLocations[UNIFORM_LOCATION_SUNSHAFTSSCREENSPACEPOSITION]);
		}
	}

	//
	// �������㻺��
	//
	VERTEX* CRenderer::LockVertexBuffer(INT numVertices) const
	{
		ASSERT(numVertices > 0);
		return (VERTEX *)SAFE_MALLOC(sizeof(VERTEX) * numVertices, MEMTYPE_STACK);
	}

	//
	// �������㻺��
	//
	VOID CRenderer::UnlockVertexBuffer(VERTEX *pVertices) const
	{
		SAFE_FREE(pVertices);
	}

	//
	// ������������
	//
	INDEX* CRenderer::LockIndexBuffer(INT numIndices) const
	{
		ASSERT(numIndices > 0);
		return (INDEX *)SAFE_MALLOC(sizeof(INDEX) * numIndices, MEMTYPE_STACK);
	}

	//
	// ������������
	//
	VOID CRenderer::UnlockIndexBuffer(INDEX *pIndices) const
	{
		SAFE_FREE(pIndices);
	}

	//
	// ������Ⱦģʽ
	//
	VOID CRenderer::SetRenderMode(UINT mode)
	{
		m_renderMode = mode;
	}

	//
	// ����д���
	//
	VOID CRenderer::SetDepthMask(BOOL depth) const
	{
		cglDepthMask(depth);
	}

	//
	// ������ɫ����
	//
	VOID CRenderer::SetColorMask(BOOL red, BOOL green, BOOL blue, BOOL alpha) const
	{
		cglColorMask(red, green, blue, alpha);
	}

	//
	// ���ö�ȡ����
	//
	VOID CRenderer::SetReadBuffer(UINT mode) const
	{
		cglReadBuffer(mode);
	}

	//
	// �����������
	//
	VOID CRenderer::SetDrawBuffer(UINT mode) const
	{
		cglDrawBuffer(mode);
	}

	//
	// ���ö�·�������
	//
	VOID CRenderer::SetDrawBuffers(INT n, const UINT *buffers) const
	{
		ASSERT(buffers);
		ASSERT(n > 0 && n < (INT)m_maxFBODrawBuffers);
		cglDrawBuffers(n, buffers);
	}

	//
	// ���ò���ͨ·
	//
	VOID CRenderer::SetPass(const CRenderer::PASS *pPass)
	{
		ASSERT(pPass);

		SetPassProgram(&pPass->program);
		SetPassCull(&pPass->cull);
		SetPassDepth(&pPass->depth);
		SetPassAlpha(&pPass->alpha);
		SetPassBlend(&pPass->blend);
		SetPassLighting(&pPass->lighting);

		for (INT indexTexUnit = 0; indexTexUnit < (INT)m_maxTexUnits; indexTexUnit++) {
			SetPassTexture(&pPass->textures[indexTexUnit], indexTexUnit);
		}
	}

	//
	// ���ò��ʶ���
	//
	VOID CRenderer::SetPassAnimator(const CRenderer::PASS *pPass, FLOAT time)
	{
		ASSERT(pPass);

		for (INT indexTexUnit = 0; indexTexUnit < (INT)m_maxTexUnits; indexTexUnit++) {
			SetPassTextureAnimator(&pPass->textures[indexTexUnit], indexTexUnit, time);
		}
	}

	//
	// ���ò��ʳ���
	//
	VOID CRenderer::SetPassProgram(const PASS_PROGRAM *pProgram)
	{
		ASSERT(pProgram);
		ASSERT(pProgram->vertex);
		ASSERT(pProgram->fragment);

		UseProgram(pProgram->vertex, pProgram->fragment, pProgram->uniformExternalLocations);
	}

	//
	// ���ò�������
	//
	VOID CRenderer::SetPassTexture(const PASS_TEXTURE *pTexture, INT indexTexUnit)
	{
		ASSERT(pTexture);
		ASSERT(indexTexUnit >= 0 && indexTexUnit < (INT)m_maxTexUnits);

		MATRIX4 mtxTexture;
		MtxIdentity(&mtxTexture);
		SetTexture(indexTexUnit, pTexture->target, pTexture->texture, pTexture->texMinFliter, pTexture->texMagFilter, pTexture->texWrapU, pTexture->texWrapV);
		SetTextureMatrix(indexTexUnit, mtxTexture); // ����Ĭ���������
	}

	//
	// ���ò���������
	//
	VOID CRenderer::SetPassTextureAnimator(const PASS_TEXTURE *pTexture, INT indexTexUnit, FLOAT time)
	{
		ASSERT(pTexture);
		ASSERT(indexTexUnit >= 0 && indexTexUnit < (INT)m_maxTexUnits);

		if (pTexture->texture > 0) {
			INT index = 0;
			FLOAT scaleU = 1.0f;
			FLOAT scaleV = 1.0f;
			FLOAT offsetU = 0.0f;
			FLOAT offsetV = 0.0f;
			FLOAT totalOffsetU = 0.0f;
			FLOAT totalOffsetV = 0.0f;

			//
			// 1. ��������֡����
			//
			if ((pTexture->texAnimation.texSequenceCols > 0 && pTexture->texAnimation.texSequenceRows > 0 && pTexture->texAnimation.texSequenceDurationTime > 0.0f) &&
				(pTexture->texAnimation.texSequenceCols > 1 || pTexture->texAnimation.texSequenceRows > 1)) {
				index = (INT)(time / (pTexture->texAnimation.texSequenceDurationTime / (pTexture->texAnimation.texSequenceCols * pTexture->texAnimation.texSequenceRows)) + 0.5f);
				index = index % (pTexture->texAnimation.texSequenceCols * pTexture->texAnimation.texSequenceRows);

				scaleU = 1.0f / pTexture->texAnimation.texSequenceCols;
				scaleV = 1.0f / pTexture->texAnimation.texSequenceRows;
				offsetU = scaleU * (index % pTexture->texAnimation.texSequenceCols);
				offsetV = scaleV * (index / pTexture->texAnimation.texSequenceCols);
			}

			//
			// 2. ���������������
			//
			if (pTexture->texAnimation.texScrollU != 0.0f || pTexture->texAnimation.texScrollV != 0.0f) {
				totalOffsetU = pTexture->texAnimation.texScrollU * time;
				totalOffsetV = pTexture->texAnimation.texScrollV * time;
			}

			//
			// 3. �����������
			//
			MATRIX4 mtxScale;
			MATRIX4 mtxTranslate;
			MATRIX4 mtxTexture;

			MtxDefScale(&mtxScale, scaleU, scaleV, 1.0f);
			MtxDefTranslate(&mtxTranslate, offsetU + totalOffsetU, offsetV + totalOffsetV, 0.0f);
			MtxMul(&mtxTexture, &mtxScale, &mtxTranslate);

			SetTextureMatrix(indexTexUnit, mtxTexture);
		}
	}

	//
	// ���ò����޳�
	//
	VOID CRenderer::SetPassCull(const PASS_CULL *pCull)
	{
		ASSERT(pCull);

		if (pCull->bEnableCullFace) {
			Enable(GL_CULL_FACE);
		}
		else {
			Disable(GL_CULL_FACE);
		}

		SetCullFace(pCull->cullFaceMode);
		SetFrontFace(pCull->frontFaceMode);
	}

	//
	// ���ò������
	//
	VOID CRenderer::SetPassDepth(const PASS_DEPTH *pDepth)
	{
		ASSERT(pDepth);

		if (pDepth->bEnableDepthTest) {
			Enable(GL_DEPTH_TEST);
		}
		else {
			Disable(GL_DEPTH_TEST);
		}

		SetDepthFunc(pDepth->depthFunc);
		SetDepthMask(pDepth->depthMask);
	}

	//
	// ���ò���Alpha
	//
	VOID CRenderer::SetPassAlpha(const PASS_ALPHA *pAlpha)
	{
		ASSERT(pAlpha);

		if (pAlpha->bEnableAlphaTest) {
			Enable(GL_ALPHA_TEST);
			SetAlphaFunc(pAlpha->alphaFunc, pAlpha->alphaRef);
		}
		else {
			Disable(GL_ALPHA_TEST);
			SetAlphaFunc(GL_ALWAYS, 0.0f); // ע��: GLES2.0�Ժ���ֱ��֧��AlphaTest, �ú������ڵײ�ģ����Shaderʵ��, 
			                               //       ����ڽ��ú���������Ĭ��ֵ!!!
		}
	}

	//
	// ���ò��ʻ��
	//
	VOID CRenderer::SetPassBlend(const PASS_BLEND *pBlend)
	{
		ASSERT(pBlend);

		if (pBlend->bEnableBlend) {
			Enable(GL_BLEND);
		}
		else {
			Disable(GL_BLEND);
		}

		SetBlendEquation(pBlend->blendModeColor, pBlend->blendModeAlpha);
		SetBlendFunc(pBlend->blendSrcColor, pBlend->blendDesColor, pBlend->blendSrcAlpha, pBlend->blendDesAlpha);
	}

	//
	// ���ò��ʹ���
	//
	VOID CRenderer::SetPassLighting(const PASS_LIGHTING *pLighting)
	{
		ASSERT(pLighting);

		SetMaterialAmbient(pLighting->matAmbient);
		SetMaterialDiffuse(pLighting->matDiffuse);
		SetMaterialEmission(pLighting->matEmission);
		SetMaterialSpecular(pLighting->matSpecular);
		SetMaterialShininess(pLighting->matShininess);
	}

	//
	// �ύ���㻺��
	//
	VOID CRenderer::SubmitVertexBuffer(UINT vao) const
	{
		cglBindVertexArray(vao);
	}

	//
	// �ύ���㻺��
	//
	VOID CRenderer::SubmitVertexBuffer(DWORD dwVertexFormat, const VERTEX *pVertices) const
	{
		if (m_pProgram && m_pProgram->program) {
			//
			// 1. λ��
			//
			if (dwVertexFormat&VERTEX_FORMAT_BITMASK_POSITION && m_pProgram->attribLocations[ATTRIB_LOCATION_POSITION].location >= 0) {
				cglEnableVertexAttribArray(m_pProgram->attribLocations[ATTRIB_LOCATION_POSITION].location);

				if (pVertices) {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_POSITION].location, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)&pVertices->position);
				}
				else {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_POSITION].location, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)POSITION_OFFSET);
				}
			}

			//
			// 2. ����
			//
			if (dwVertexFormat&VERTEX_FORMAT_BITMASK_NORMAL && m_pProgram->attribLocations[ATTRIB_LOCATION_NORMAL].location >= 0) {
				cglEnableVertexAttribArray(m_pProgram->attribLocations[ATTRIB_LOCATION_NORMAL].location);

				if (pVertices) {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_NORMAL].location, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)&pVertices->normal);
				}
				else {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_NORMAL].location, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)NORMAL_OFFSET);
				}
			}

			//
			// 3. ������
			//
			if (dwVertexFormat&VERTEX_FORMAT_BITMASK_BINORMAL && m_pProgram->attribLocations[ATTRIB_LOCATION_BINORMAL].location >= 0) {
				cglEnableVertexAttribArray(m_pProgram->attribLocations[ATTRIB_LOCATION_BINORMAL].location);

				if (pVertices) {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_BINORMAL].location, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)&pVertices->binormal);
				}
				else {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_BINORMAL].location, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)BINORMAL_OFFSET);
				}
			}

			//
			// 4. ��ɫ
			//
			if (dwVertexFormat&VERTEX_FORMAT_BITMASK_COLOR && m_pProgram->attribLocations[ATTRIB_LOCATION_COLOR].location >= 0) {
				cglEnableVertexAttribArray(m_pProgram->attribLocations[ATTRIB_LOCATION_COLOR].location);

				if (pVertices) {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_COLOR].location, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)&pVertices->color);
				}
				else {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_COLOR].location, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)COLOR_OFFSET);
				}
			}

			//
			// 5. ��������������
			//
			if (dwVertexFormat&VERTEX_FORMAT_BITMASK_TEXCOORD_DIFFUSE && m_pProgram->attribLocations[ATTRIB_LOCATION_TEXCOORDDIFFUSE].location >= 0) {
				cglEnableVertexAttribArray(m_pProgram->attribLocations[ATTRIB_LOCATION_TEXCOORDDIFFUSE].location);

				if (pVertices) {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_TEXCOORDDIFFUSE].location, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)&pVertices->texCoordDiffuse);
				}
				else {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_TEXCOORDDIFFUSE].location, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)TEXCOORD_DIFFUSE_OFFSET);
				}
			}

			//
			// 6. ����ͼ��������
			//
			if (dwVertexFormat&VERTEX_FORMAT_BITMASK_TEXCOORD_LIGHTMAP && m_pProgram->attribLocations[ATTRIB_LOCATION_TEXCOORDLIGHTMAP].location >= 0) {
				cglEnableVertexAttribArray(m_pProgram->attribLocations[ATTRIB_LOCATION_TEXCOORDLIGHTMAP].location);

				if (pVertices) {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_TEXCOORDLIGHTMAP].location, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)&pVertices->texCoordLightMap);
				}
				else {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_TEXCOORDLIGHTMAP].location, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)TEXCOORD_LIGHTMAP_OFFSET);
				}
			}

			//
			// 7. ��������
			//
			if (dwVertexFormat&VERTEX_FORMAT_BITMASK_INDICES && m_pProgram->attribLocations[ATTRIB_LOCATION_INDICES].location >= 0) {
				cglEnableVertexAttribArray(m_pProgram->attribLocations[ATTRIB_LOCATION_INDICES].location);

				if (pVertices) {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_INDICES].location, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)&pVertices->indices);
				}
				else {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_INDICES].location, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)INDICES_OFFSET);
				}
			}

			//
			// 8. ����Ȩ��
			//
			if (dwVertexFormat&VERTEX_FORMAT_BITMASK_WEIGHTS && m_pProgram->attribLocations[ATTRIB_LOCATION_WEIGHTS].location >= 0) {
				cglEnableVertexAttribArray(m_pProgram->attribLocations[ATTRIB_LOCATION_WEIGHTS].location);

				if (pVertices) {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_WEIGHTS].location, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)&pVertices->weights);
				}
				else {
					cglVertexAttribPointer(m_pProgram->attribLocations[ATTRIB_LOCATION_WEIGHTS].location, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid *)WEIGHTS_OFFSET);
				}
			}
		}
	}

	//
	// �ύ���󻺳�
	//
	VOID CRenderer::SubmitMatrixBuffer(const FLOAT *matrices, INT numMatrices)
	{
		ASSERT(matrices);
		ASSERT(numMatrices > 0);

		if (m_pProgram && m_pProgram->program) {
			if (m_pProgram->uniformLocations[UNIFORM_LOCATION_BONES].location >= 0) {
				cglUniformMatrix4fv(m_pProgram->uniformLocations[UNIFORM_LOCATION_BONES].location, numMatrices, GL_FALSE, matrices);
			}
		}
	}

	//
	// ��ʼ��Ⱦ�ڵ�
	//
	VOID CRenderer::BeginOcclude(VOID) const
	{
		if (IsOpenGLES3()) {
			Disable(GL_BLEND);
			Disable(GL_NORMALIZE);
			Disable(GL_TEXTURE_2D);
			Disable(GL_ALPHA_TEST);
			Enable(GL_CULL_FACE);
			Enable(GL_DEPTH_TEST);
			SetDepthFunc(GL_LESS);
			SetDepthMask(GL_TRUE);
			SetColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			cglEnableClientState(GL_VERTEX_ARRAY);
			cglDisableClientState(GL_NORMAL_ARRAY);
			cglDisableClientState(GL_COLOR_ARRAY);
			cglDisableClientState(GL_TEXTURE_COORD_ARRAY);
			cglVertexPointer(3, GL_FLOAT, VERTEX_SIZE, (GLvoid *)POSITION_OFFSET);
		}
	}

	//
	// ������Ⱦ�ڵ�
	//
	VOID CRenderer::EndOcclude(VOID) const
	{
		if (IsOpenGLES3()) {
			cglDisableClientState(GL_VERTEX_ARRAY);
			cglDisableClientState(GL_NORMAL_ARRAY);
			cglDisableClientState(GL_COLOR_ARRAY);
			cglDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}

	//
	// ��Ⱦ�ڵ�
	//
	VOID CRenderer::RenderOcclude(const INDEX *indices, INT count)
	{
		ASSERT(indices);
		ASSERT(count > 0);

		if (IsOpenGLES3()) {
			SetProgramParams();
			cglDrawElementsCompatible(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, indices);

			m_draws += 1;
			m_faces += count / 3;
		}
	}

	//
	// ��ʼ��Ⱦ��ѯ
	//
	VOID CRenderer::BeginQuery(VOID) const
	{
		if (IsOpenGLES3()) {
			Disable(GL_BLEND);
			Disable(GL_NORMALIZE);
			Disable(GL_TEXTURE_2D);
			Disable(GL_ALPHA_TEST);
			Enable(GL_CULL_FACE);
			Enable(GL_DEPTH_TEST);
			SetDepthFunc(GL_LESS);
			SetDepthMask(GL_FALSE);
			SetColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			cglEnableClientState(GL_VERTEX_ARRAY);
			cglDisableClientState(GL_NORMAL_ARRAY);
			cglDisableClientState(GL_COLOR_ARRAY);
			cglDisableClientState(GL_TEXTURE_COORD_ARRAY);
			cglVertexPointer(3, GL_FLOAT, VERTEX_SIZE, (GLvoid *)POSITION_OFFSET);
		}
	}

	//
	// ������Ⱦ��ѯ
	//
	VOID CRenderer::EndQuery(VOID) const
	{
		if (IsOpenGLES3()) {
			cglDisableClientState(GL_VERTEX_ARRAY);
			cglDisableClientState(GL_NORMAL_ARRAY);
			cglDisableClientState(GL_COLOR_ARRAY);
			cglDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}

	//
	// ��Ⱦ��ѯ
	//
	VOID CRenderer::RenderQuery(UINT query, const AABB *aabb)
	{
		ASSERT(aabb);
		ASSERT(IsAABBValid(aabb));

		if (IsOpenGLES3()) {
			PushMatrix();
			{
				MATRIX4 mtxScale;
				MATRIX4 mtxTranslate;
				MATRIX4 mtxTransform;

				MtxDefScale(&mtxScale, (aabb->maxVertex[0] - aabb->minVertex[0]) / 2.0f, (aabb->maxVertex[1] - aabb->minVertex[1]) / 2.0f, (aabb->maxVertex[2] - aabb->minVertex[2]) / 2.0f);
				MtxDefTranslate(&mtxTranslate, aabb->center[0], aabb->center[1], aabb->center[2]);
				MtxMul(&mtxTransform, &mtxScale, &mtxTranslate);

				SetModelWorldMatrix(mtxTransform);

				cglBeginQuery(GL_SAMPLES_PASSED, query);
				{
					BindVBO(GL_ARRAY_BUFFER, m_boxVertexBuffer);
					BindVBO(GL_ELEMENT_ARRAY_BUFFER, m_boxIndexBuffer);
					SubmitVertexBuffer(VERTEX_FORMAT_POSITION, NULL);

					SetProgramParams();
					cglDrawElementsCompatible(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_SHORT, NULL);

					m_draws += 1;
					m_faces += 12;
				}
				cglEndQuery(GL_SAMPLES_PASSED);
			}
			PopMatrix();
		}
	}

	//
	// ��ʼ��Ⱦ����
	//
	VOID CRenderer::BeginLighting(VOID) const
	{
		Disable(GL_BLEND);
		Disable(GL_NORMALIZE);
		Disable(GL_ALPHA_TEST);
		Enable(GL_CULL_FACE);
		Enable(GL_DEPTH_TEST);
		SetDepthFunc(GL_LESS);
		SetDepthMask(GL_FALSE);
		SetColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}

	//
	// ������Ⱦ����
	//
	VOID CRenderer::EndLighting(VOID) const
	{

	}

	//
	// ��Ⱦ����
	//
	VOID CRenderer::RenderLighting(const FLOAT *position, FLOAT radius)
	{
		ASSERT(position);
		ASSERT(radius > 0.0f);

		PushMatrix();
		{
			MATRIX4 mtxScale;
			MATRIX4 mtxTranslate;
			MATRIX4 mtxTransform;

			MtxDefScale(&mtxScale, radius, radius, radius);
			MtxDefTranslate(&mtxTranslate, position[0], position[1], position[2]);
			MtxMul(&mtxTransform, &mtxScale, &mtxTranslate);

			SetModelWorldMatrix(mtxTransform);

			BindVBO(GL_ARRAY_BUFFER, m_sphereVertexBuffer);
			BindVBO(GL_ELEMENT_ARRAY_BUFFER, m_sphereIndexBuffer);
			SubmitVertexBuffer(VERTEX_FORMAT_POSITION, NULL);

			Render(NULL, 224 * 3);
		}
		PopMatrix();
	}

	//
	// ��Ⱦ
	//
	VOID CRenderer::Render(const INDEX *indices, INT count)
	{
		ASSERT(indices);
		ASSERT(count > 0);

		SetProgramParams();
		cglDrawElements(m_renderMode, count, GL_UNSIGNED_SHORT, indices);

		m_draws += 1;
		m_faces += count / 3;
	}

	//
	// ��Ⱦ����
	//
	VOID CRenderer::RenderRect(FLOAT left, FLOAT right, FLOAT top, FLOAT bottom)
	{
		//
		// 1. �������
		//
		VEC3 position;
		VEC3 direction, up;
		Vec3Set(&position, 0.0f, 0.0f, 1.0f);
		Vec3Set(&direction, 0.0f, 0.0f, -1.0f);
		Vec3Set(&up, 0.0f, 1.0f, 0.0f);

		MATRIX4 mtxTransform;
		MtxTranslate(&mtxTransform, (right + left) / 2.0f, (bottom + top) / 2.0f, 0.0f);
		MtxScale(&mtxTransform, (right - left) / 2.0f, (bottom - top) / 2.0f, 1.0f);

		SetOrtho(left, right, bottom, top, 0.0f, 100.0f);
		SetLookAt(position, direction, up);
		SetModelWorldMatrix(mtxTransform);

		//
		// 2. ��Ⱦ�ӿ�
		//
		Disable(GL_BLEND);
		Disable(GL_NORMALIZE);
		Disable(GL_CULL_FACE);
		Disable(GL_ALPHA_TEST);
		Disable(GL_DEPTH_TEST);
		SetDepthMask(GL_FALSE);
		SetColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		{
			BindVBO(GL_ARRAY_BUFFER, m_rectVertexBuffer);
			BindVBO(GL_ELEMENT_ARRAY_BUFFER, m_rectIndexBuffer);
			SubmitVertexBuffer(VERTEX_FORMAT_SCREEN, NULL);

			Render(NULL, 6);
		}
	}

	//
	// ��Ⱦ�ӿ�
	//
	VOID CRenderer::RenderViewport(VOID)
	{
		RenderRect(1.0f * GetViewportX(), 1.0f * GetViewportWidth(), 1.0f * GetViewportY(), 1.0f * GetViewportHeight());
	}

	//
	// ˢ��ָ���
	//
	VOID CRenderer::Flush(VOID) const
	{
		cglFlush();
	}

	//
	// ˢ��ָ��岢�ȴ�ִ�н���
	//
	VOID CRenderer::Finish(VOID) const
	{
		cglFinish();
	}

	//
	// ��������
	//
	VOID CRenderer::SwapBuffers(VOID) const
	{
#if defined _PLATFORM_WINDOWS_

		::SwapBuffers(m_hDC);

#endif
	}

	//
	// �����Ⱦ����
	//
	INT CRenderer::GetFaces(VOID) const
	{
		return m_faces;
	}

	//
	// �����Ⱦ���ô���
	//
	INT CRenderer::GetDrawCalls(VOID) const
	{
		return m_draws;
	}

}

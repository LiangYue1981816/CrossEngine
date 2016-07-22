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

	//////////////////////////////////////////////////////////////////////////
	// 数据结构
	//========================================================================
	#define MAX_MATRIX_STACK_DEPTH 32
	#define MAX_TEXTURES 65536
	#define MAX_VERTEX_ATTRIBS 1024


	typedef enum {
		GLES_VERSION_20 = 2,
		GLES_VERSION_30,
	} GLES_VERSION;

	typedef enum {
		RENDER_GENERAL = 0,
		RENDER_INTEL_HD_GRAPHICS_3000
	} RENDER_TYPE;

	typedef struct {
		GLint bEnableVertex;
		GLint bEnableNormal;
		GLint bEnableColor;
		GLint bEnableTexture;
	} CLIENTSTATE_PARAM;

	typedef enum {
		MATRIX_MODE_INVALID = -1,
		MATRIX_MODE_MODELWORLD,
		MATRIX_MODE_MODELVIEW,
		MATRIX_MODE_PROJECTION,
		MATRIX_MODE_TEXTURE0,
		MATRIX_MODE_TEXTURE1,
		MATRIX_MODE_TEXTURE2,
		MATRIX_MODE_TEXTURE3,
		MATRIX_MODE_TEXTURE4,
		MATRIX_MODE_TEXTURE5,
		MATRIX_MODE_TEXTURE6,
		MATRIX_MODE_TEXTURE7,
		MATRIX_MODE_COUNT
	} MATRIX_MODE;

	typedef struct {
		GLint indexStack;
		GLfloat matrices[MAX_MATRIX_STACK_DEPTH][16];
	} MATRIX_STACK;

	typedef struct {
		MATRIX_MODE matrixMode;
		MATRIX_STACK matrixStack[MATRIX_MODE_COUNT];

		GLfloat modelWorldMatrix[16];
		GLfloat modelViewMatrix[16];
		GLfloat modelViewInverseMatrix[16];
		GLfloat modelViewInverseTransposeMatrix[16];
		GLfloat modelViewProjectionMatrix[16];
		GLfloat textureMatrices[MAX_TEXUNITS][16];
	} MATRIX_PARAM;

	typedef struct {
		GLint x;
		GLint y;
		GLsizei width;
		GLsizei height;
	} SCISSOR_PARAM;

	typedef struct {
		GLint x;
		GLint y;
		GLsizei width;
		GLsizei height;
	} VIEWPORT_PARAM;

	typedef struct {
		GLfloat fovy;
		GLfloat aspect;
		GLfloat zfar;
		GLfloat znear;
		GLfloat position[3];
		GLfloat forward[3];
		GLfloat up[3];
	} CAMERA_PARAM;

	typedef struct {
		GLint cull;
		GLint front;
	} CULLFACE_PARAM;

	typedef struct {
		GLint func;
		GLfloat ref;
	} ALPHA_PARAM;

	typedef struct {
		GLint sfactor;
		GLint dfactor;

		GLint srcrgb;
		GLint dstrgb;
		GLint srcalpha;
		GLint dstalpha;

		GLfloat red;
		GLfloat green;
		GLfloat blue;
		GLfloat alpha;

		GLint mode;
		GLint modergb;
		GLint modealpha;
	} BLEND_PARAM;

	typedef struct {
		GLint func;
		GLint mask;
	} DEPTH_PARAM;

	typedef struct {
		GLint func;
		GLint ref;
		GLuint mask;

		GLint sfail;
		GLint zfail;
		GLint zpass;
	} STENCIL_PARAM;

	typedef struct {
		GLfloat factor;
		GLfloat units;
	} POLYGONOFFSET_PARAM;

	typedef struct {
		GLfloat width;
	} LINEWIDTH_PARAM;

	typedef struct {
		GLint pname;
		GLint param;
	} PIXELSTORE_PARAM;

	typedef struct {
		GLfloat value;
		GLboolean invert;
	} SAMPLECOVERAGE_PARAM;

	typedef struct {
		GLint stencil;
		GLfloat depth;
		GLfloat red;
		GLfloat green;
		GLfloat blue;
		GLfloat alpha;
	} CLEAR_PARAM;

	typedef struct {
		GLboolean red;
		GLboolean green;
		GLboolean blue;
		GLboolean alpha;
	} COLORMASK_PARAM;

	typedef struct {
		GLint vao;
		GLint vbo;
		GLint ibo;
	} BUFFER_PARAM;

	typedef struct {
		GLint numTextureUnits;
		GLint activeTextureUnit;

		GLint texEanble[MAX_TEXUNITS];
		GLint texture[MAX_TEXUNITS];

		GLint texMinFliter[MAX_TEXTURES];
		GLint texMagFilter[MAX_TEXTURES];
		GLint texWrapU[MAX_TEXTURES];
		GLint texWrapV[MAX_TEXTURES];
	} TEXUNIT_PARAM;

	typedef struct {
		GLint numVertexAttribs;
		GLint bEnableVertexAttrib[MAX_VERTEX_ATTRIBS];
	} VERTEXATTRIBSTATE_PARAM;

	typedef struct {
		GLint size;
		GLint type;
		GLsizei stride;
		void *ptr;
	} ATTRIB_POINTER;

	typedef struct {
		GLint program;
		GLint shaderVertex;
		GLint shaderFragment;

		GLint indexModelViewProjectionMatrix;
		GLint indexTextureMatrix;
		GLint indexTexture;

		GLint indexVertex;
		GLint indexColor;
		GLint indexTexCoord;

		GLfloat modelViewProjectionMatrix[16];
		GLfloat textureMatrix[16];
		GLint texture;
	} PROGRAM;

	typedef struct {
		GLint program;

		PROGRAM programDepth;
		PROGRAM programColor;
		PROGRAM programTexture;
		PROGRAM programTextureColor;

		ATTRIB_POINTER ptrVertex;
		ATTRIB_POINTER ptrColor;
		ATTRIB_POINTER ptrTexCoord;
	} PROGRAM_PARAM;

	typedef struct {
		GLint bEnableStateCache;
		GLint capabilities[128];
		CLIENTSTATE_PARAM clientStateParam;
		MATRIX_PARAM matrixParam;
		SCISSOR_PARAM scissorParam;
		VIEWPORT_PARAM viewportParam;
		CAMERA_PARAM cameraParam;
		CULLFACE_PARAM cullFaceParam;
		ALPHA_PARAM alphaParam;
		BLEND_PARAM blendParam;
		DEPTH_PARAM depthParam;
		STENCIL_PARAM stencilParam;
		POLYGONOFFSET_PARAM polygonOffsetParam;
		LINEWIDTH_PARAM lineWidthParam;
		PIXELSTORE_PARAM pixelStoreParam;
		SAMPLECOVERAGE_PARAM sampleCoverageParam;
		CLEAR_PARAM clearParam;
		COLORMASK_PARAM colorMaskParam;
		BUFFER_PARAM bufferParam;
		TEXUNIT_PARAM texUnitParam;
		VERTEXATTRIBSTATE_PARAM vertexAttribStateParam;
		PROGRAM_PARAM programParam;
	} PIPLINE_STATE;

	typedef struct {
		GLuint attrib;
		GLchar szString[64];
	} ATTRIB_STRING;


	static GLES_VERSION esVersion = GLES_VERSION_20;
	static RENDER_TYPE renderType = RENDER_GENERAL;
	static PIPLINE_STATE piplineState;

	static const ATTRIB_STRING attribString[] = {
		#include "CrossStringGLES1.h"
		#include "CrossStringGLES2.h"
		#include "CrossStringGLES3.h"
	};


	//////////////////////////////////////////////////////////////////////////
	// Shaders
	//========================================================================
	static const GLchar *UNIFORM_MODELVIEWPROJECTIONMATRIX = "_modelViewProjectionMatrix";
	static const GLchar *UNIFORM_TEXTUREMATRIX = "_textureMatrix";
	static const GLchar *UNIFORM_TEXTURE = "_texture";

	static const GLchar *VERTEX_ATTRIB_VERTEX = "_position";
	static const GLchar *VERTEX_ATTRIB_COLOR = "_color";
	static const GLchar *VERTEX_ATTRIB_TEXCOORD = "_texcoord";

	// Depth
	static const GLchar *szShaderVertexCode_Depth =
		"                                                            \n\
		uniform mat4 _modelViewProjectionMatrix;                     \n\
		                                                             \n\
		attribute vec4 _position;                                    \n\
		                                                             \n\
		void main()                                                  \n\
		{                                                            \n\
			gl_Position = _modelViewProjectionMatrix*_position;      \n\
		}                                                            \n\
																																																																																																																																					";

	static const GLchar *szShaderFragmentCode_Depth =
		"                                                            \n\
		#ifdef GL_ES                                                 \n\
		precision mediump float;                                     \n\
		#endif                                                       \n\
		                                                             \n\
		void main()                                                  \n\
		{                                                            \n\
		}                                                            \n\
		";

	// Color
	static const GLchar *szShaderVertexCode_Color =
		"                                                            \n\
		uniform mat4 _modelViewProjectionMatrix;                     \n\
		                                                             \n\
		attribute vec4 _position;                                    \n\
		attribute vec4 _color;                                       \n\
		                                                             \n\
		#ifdef GL_ES                                                 \n\
		varying lowp vec4 color;                                     \n\
		#else                                                        \n\
		varying vec4 color;                                          \n\
		#endif                                                       \n\
		                                                             \n\
		void main()                                                  \n\
		{                                                            \n\
			gl_Position = _modelViewProjectionMatrix*_position;      \n\
			color = _color/255.0;                                    \n\
		}                                                            \n\
		";

	static const GLchar *szShaderFragmentCode_Color =
		"                                                            \n\
		#ifdef GL_ES                                                 \n\
		precision mediump float;                                     \n\
		#endif                                                       \n\
		                                                             \n\
		#ifdef GL_ES                                                 \n\
		varying lowp vec4 color;                                     \n\
		#else                                                        \n\
		varying vec4 color;                                          \n\
		#endif                                                       \n\
		                                                             \n\
		void main()                                                  \n\
		{                                                            \n\
			gl_FragColor = color;                                    \n\
		}                                                            \n\
		";

	// Texture
	static const GLchar *szShaderVertexCode_Texture =
		"                                                            \n\
		uniform mat4 _modelViewProjectionMatrix;                     \n\
		uniform mat4 _textureMatrix;                                 \n\
		                                                             \n\
		attribute vec4 _position;                                    \n\
		attribute vec4 _texcoord;                                    \n\
		                                                             \n\
		#ifdef GL_ES                                                 \n\
		varying mediump vec4 texcoord;                               \n\
		#else                                                        \n\
		varying vec4 texcoord;                                       \n\
		#endif                                                       \n\
		                                                             \n\
		void main()                                                  \n\
		{                                                            \n\
			gl_Position = _modelViewProjectionMatrix*_position;      \n\
			texcoord = _textureMatrix*_texcoord;                     \n\
		}                                                            \n\
		";

	static const GLchar *szShaderFragmentCode_Texture =
		"                                                            \n\
		#ifdef GL_ES                                                 \n\
		precision mediump float;                                     \n\
		#endif                                                       \n\
		                                                             \n\
		uniform sampler2D _texture;                                  \n\
		                                                             \n\
		#ifdef GL_ES                                                 \n\
		varying mediump vec4 texcoord;                               \n\
		#else                                                        \n\
		varying vec4 texcoord;                                       \n\
		#endif                                                       \n\
		                                                             \n\
		void main()                                                  \n\
		{                                                            \n\
			gl_FragColor = texture2D(_texture, texcoord.xy);         \n\
		}                                                            \n\
		";

	// Texture&Color
	static const GLchar *szShaderVertexCode_TextureColor =
		"                                                            \n\
		uniform mat4 _modelViewProjectionMatrix;                     \n\
		uniform mat4 _textureMatrix;                                 \n\
		                                                             \n\
		attribute vec4 _position;                                    \n\
		attribute vec4 _texcoord;                                    \n\
		attribute vec4 _color;                                       \n\
		                                                             \n\
		#ifdef GL_ES                                                 \n\
		varying mediump vec4 texcoord;                               \n\
		varying lowp vec4 color;                                     \n\
		#else                                                        \n\
		varying vec4 texcoord;                                       \n\
		varying vec4 color;                                          \n\
		#endif                                                       \n\
		                                                             \n\
		void main()                                                  \n\
		{                                                            \n\
			gl_Position = _modelViewProjectionMatrix*_position;      \n\
			texcoord = _textureMatrix*_texcoord;                     \n\
			color = _color/255.0;                                    \n\
		}                                                            \n\
		";

	static const GLchar *szShaderFragmentCode_TextureColor =
		"                                                            \n\
		#ifdef GL_ES                                                 \n\
		precision mediump float;                                     \n\
		#endif                                                       \n\
		                                                             \n\
		uniform sampler2D _texture;                                  \n\
		                                                             \n\
		#ifdef GL_ES                                                 \n\
		varying mediump vec4 texcoord;                               \n\
		varying lowp vec4 color;                                     \n\
		#else                                                        \n\
		varying vec4 texcoord;                                       \n\
		varying vec4 color;                                          \n\
		#endif                                                       \n\
		                                                             \n\
		void main()                                                  \n\
		{                                                            \n\
			gl_FragColor = texture2D(_texture, texcoord.xy)*color;   \n\
		}                                                            \n\
		";


	//////////////////////////////////////////////////////////////////////////
	// 初始化/退出
	//========================================================================
	RENDER_TYPE GetRenderType(void)
	{
		static const GLchar *szRenderer = cglGetString(GL_RENDERER);

		if (stricmp(szRenderer, "Intel(R) HD Graphics 3000") == 0) {
			return RENDER_INTEL_HD_GRAPHICS_3000;
		}

		return RENDER_GENERAL;
	}

	GLboolean IsCapParamValid(GLenum state)
	{
		switch (state) {
		case GL_ALPHA_TEST:                 return GL_TRUE;
		case GL_BLEND:                      return GL_TRUE;
		case GL_CULL_FACE:                  return GL_TRUE;
		case GL_DEPTH_TEST:                 return GL_TRUE;
		case GL_LINE_SMOOTH:                return GL_TRUE;
		case GL_MULTISAMPLE:                return GL_TRUE;
		case GL_NORMALIZE:                  return GL_TRUE;
		case GL_POINT_SMOOTH:               return GL_TRUE;
		case GL_POLYGON_OFFSET_FILL:        return GL_TRUE;
		case GL_SAMPLE_ALPHA_TO_COVERAGE:   return GL_TRUE;
		case GL_SAMPLE_ALPHA_TO_ONE:        return GL_TRUE;
		case GL_SAMPLE_COVERAGE:            return GL_TRUE;
		case GL_SCISSOR_TEST:               return GL_TRUE;
		case GL_STENCIL_TEST:               return GL_TRUE;
		case GL_TEXTURE_2D:                 return GL_TRUE;
		}

		return GL_FALSE;
	}

	GLint CapIndex(GLenum state)
	{
		switch (state) {
		case GL_ALPHA_TEST:                 return 0;
		case GL_BLEND:                      return 1;
		case GL_CULL_FACE:                  return 2;
		case GL_DEPTH_TEST:                 return 3;
		case GL_LINE_SMOOTH:                return 4;
		case GL_MULTISAMPLE:                return 5;
		case GL_NORMALIZE:                  return 6;
		case GL_POINT_SMOOTH:               return 7;
		case GL_POLYGON_OFFSET_FILL:        return 8;
		case GL_SAMPLE_ALPHA_TO_COVERAGE:   return 9;
		case GL_SAMPLE_ALPHA_TO_ONE:        return 10;
		case GL_SAMPLE_COVERAGE:            return 11;
		case GL_SCISSOR_TEST:               return 12;
		case GL_STENCIL_TEST:               return 13;
		}

		return -1;
	}

	void InitPiplineStates(void)
	{
		for (GLint index = 0; index < sizeof(piplineState.capabilities) / sizeof(piplineState.capabilities[0]); index++) {
			piplineState.capabilities[index] = GL_INVALID_VALUE;
		}
	}

	void InitClientState(void)
	{
		piplineState.clientStateParam.bEnableVertex = GL_INVALID_VALUE;
		piplineState.clientStateParam.bEnableNormal = GL_INVALID_VALUE;
		piplineState.clientStateParam.bEnableColor = GL_INVALID_VALUE;
		piplineState.clientStateParam.bEnableTexture = GL_INVALID_VALUE;
	}

	void InitMatrixStack(void)
	{
		piplineState.matrixParam.matrixMode = MATRIX_MODE_INVALID;

		piplineState.matrixParam.matrixStack[MATRIX_MODE_MODELWORLD].indexStack = 0;
		piplineState.matrixParam.matrixStack[MATRIX_MODE_MODELVIEW].indexStack = 0;
		piplineState.matrixParam.matrixStack[MATRIX_MODE_PROJECTION].indexStack = 0;
		piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE0].indexStack = 0;
		piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE1].indexStack = 0;
		piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE2].indexStack = 0;
		piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE3].indexStack = 0;
		piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE4].indexStack = 0;
		piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE5].indexStack = 0;
		piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE6].indexStack = 0;
		piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE7].indexStack = 0;

		for (GLint indexDepth = 0; indexDepth < MAX_MATRIX_STACK_DEPTH; indexDepth++) {
			MtxIdentity((MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_MODELWORLD].matrices[indexDepth]);
			MtxIdentity((MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_MODELVIEW].matrices[indexDepth]);
			MtxIdentity((MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_PROJECTION].matrices[indexDepth]);
			MtxIdentity((MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE0].matrices[indexDepth]);
			MtxIdentity((MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE1].matrices[indexDepth]);
			MtxIdentity((MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE2].matrices[indexDepth]);
			MtxIdentity((MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE3].matrices[indexDepth]);
			MtxIdentity((MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE4].matrices[indexDepth]);
			MtxIdentity((MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE5].matrices[indexDepth]);
			MtxIdentity((MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE6].matrices[indexDepth]);
			MtxIdentity((MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_TEXTURE7].matrices[indexDepth]);
		}

		MtxIdentity((MATRIX4*)piplineState.matrixParam.modelWorldMatrix);
		MtxIdentity((MATRIX4*)piplineState.matrixParam.modelViewMatrix);
		MtxIdentity((MATRIX4*)piplineState.matrixParam.modelViewInverseMatrix);
		MtxIdentity((MATRIX4*)piplineState.matrixParam.modelViewInverseTransposeMatrix);
		MtxIdentity((MATRIX4*)piplineState.matrixParam.modelViewProjectionMatrix);
		MtxIdentity((MATRIX4*)piplineState.matrixParam.textureMatrices[0]);
		MtxIdentity((MATRIX4*)piplineState.matrixParam.textureMatrices[1]);
		MtxIdentity((MATRIX4*)piplineState.matrixParam.textureMatrices[2]);
		MtxIdentity((MATRIX4*)piplineState.matrixParam.textureMatrices[3]);
		MtxIdentity((MATRIX4*)piplineState.matrixParam.textureMatrices[4]);
		MtxIdentity((MATRIX4*)piplineState.matrixParam.textureMatrices[5]);
		MtxIdentity((MATRIX4*)piplineState.matrixParam.textureMatrices[6]);
		MtxIdentity((MATRIX4*)piplineState.matrixParam.textureMatrices[7]);
	}

	void InitScissor(void)
	{
		piplineState.scissorParam.x = INT_MAX;
		piplineState.scissorParam.y = INT_MAX;
		piplineState.scissorParam.width = INT_MAX;
		piplineState.scissorParam.height = INT_MAX;
	}

	void InitViewport(void)
	{
		piplineState.viewportParam.x = INT_MAX;
		piplineState.viewportParam.y = INT_MAX;
		piplineState.viewportParam.width = INT_MAX;
		piplineState.viewportParam.height = INT_MAX;
	}

	void InitCamera(void)
	{
		piplineState.cameraParam.fovy = FLT_MAX;
		piplineState.cameraParam.aspect = FLT_MAX;
		piplineState.cameraParam.znear = FLT_MAX;
		piplineState.cameraParam.zfar = FLT_MAX;

		piplineState.cameraParam.position[0] = FLT_MAX;
		piplineState.cameraParam.position[1] = FLT_MAX;
		piplineState.cameraParam.position[2] = FLT_MAX;

		piplineState.cameraParam.forward[0] = FLT_MAX;
		piplineState.cameraParam.forward[1] = FLT_MAX;
		piplineState.cameraParam.forward[2] = FLT_MAX;

		piplineState.cameraParam.up[0] = FLT_MAX;
		piplineState.cameraParam.up[1] = FLT_MAX;
		piplineState.cameraParam.up[2] = FLT_MAX;
	}

	void InitCullFace(void)
	{
		piplineState.cullFaceParam.cull = GL_INVALID_VALUE;
		piplineState.cullFaceParam.front = GL_INVALID_VALUE;
	}

	void InitAlpha(void)
	{
		piplineState.alphaParam.func = GL_INVALID_VALUE;
		piplineState.alphaParam.ref = FLT_MAX;
	}

	void InitBlend(void)
	{
		piplineState.blendParam.sfactor = GL_INVALID_VALUE;
		piplineState.blendParam.dfactor = GL_INVALID_VALUE;

		piplineState.blendParam.srcrgb = GL_INVALID_VALUE;
		piplineState.blendParam.dstrgb = GL_INVALID_VALUE;
		piplineState.blendParam.srcalpha = GL_INVALID_VALUE;
		piplineState.blendParam.dstalpha = GL_INVALID_VALUE;

		piplineState.blendParam.red = FLT_MAX;
		piplineState.blendParam.green = FLT_MAX;
		piplineState.blendParam.blue = FLT_MAX;
		piplineState.blendParam.alpha = FLT_MAX;

		piplineState.blendParam.mode = GL_INVALID_VALUE;
		piplineState.blendParam.modergb = GL_INVALID_VALUE;
		piplineState.blendParam.modealpha = GL_INVALID_VALUE;
	}

	void InitDepth(void)
	{
		piplineState.depthParam.func = GL_INVALID_VALUE;
		piplineState.depthParam.mask = GL_INVALID_VALUE;
	}

	void InitStencil(void)
	{
		piplineState.stencilParam.func = GL_INVALID_VALUE;
		piplineState.stencilParam.ref = GL_INVALID_VALUE;
		piplineState.stencilParam.mask = GL_INVALID_VALUE;

		piplineState.stencilParam.sfail = GL_INVALID_VALUE;
		piplineState.stencilParam.zfail = GL_INVALID_VALUE;
		piplineState.stencilParam.zpass = GL_INVALID_VALUE;
	}

	void InitPolygonOffset(void)
	{
		piplineState.polygonOffsetParam.factor = FLT_MAX;
		piplineState.polygonOffsetParam.units = FLT_MAX;
	}

	void InitLine(void)
	{
		piplineState.lineWidthParam.width = FLT_MAX;
	}

	void InitPixelStore(void)
	{
		piplineState.pixelStoreParam.pname = GL_INVALID_VALUE;
		piplineState.pixelStoreParam.param = GL_INVALID_VALUE;
	}

	void InitSampleCoverage(void)
	{
		piplineState.sampleCoverageParam.value = FLT_MAX;
		piplineState.sampleCoverageParam.invert = (GLboolean)GL_INVALID_VALUE;
	}

	void InitClear(void)
	{
		piplineState.clearParam.stencil = INT_MAX;
		piplineState.clearParam.depth = FLT_MAX;
		piplineState.clearParam.red = FLT_MAX;
		piplineState.clearParam.green = FLT_MAX;
		piplineState.clearParam.blue = FLT_MAX;
		piplineState.clearParam.alpha = FLT_MAX;
	}

	void InitColorMask(void)
	{
		piplineState.colorMaskParam.red = (GLboolean)GL_INVALID_VALUE;
		piplineState.colorMaskParam.green = (GLboolean)GL_INVALID_VALUE;
		piplineState.colorMaskParam.blue = (GLboolean)GL_INVALID_VALUE;
		piplineState.colorMaskParam.alpha = (GLboolean)GL_INVALID_VALUE;
	}

	void InitBuffer(void)
	{
		piplineState.bufferParam.vao = GL_INVALID_VALUE;
		piplineState.bufferParam.vbo = GL_INVALID_VALUE;
		piplineState.bufferParam.ibo = GL_INVALID_VALUE;
	}

	void InitTextureUnit(void)
	{
		cglGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &piplineState.texUnitParam.numTextureUnits);
		piplineState.texUnitParam.numTextureUnits = min(piplineState.texUnitParam.numTextureUnits, MAX_TEXUNITS);
		piplineState.texUnitParam.activeTextureUnit = 0;

		for (GLint indexTexUnit = 0; indexTexUnit < MAX_TEXUNITS; indexTexUnit++) {
			piplineState.texUnitParam.texEanble[indexTexUnit] = GL_INVALID_VALUE;
			piplineState.texUnitParam.texture[indexTexUnit] = GL_INVALID_VALUE;
		}

		for (GLint indexTex = 0; indexTex < MAX_TEXTURES; indexTex++) {
			piplineState.texUnitParam.texMinFliter[indexTex] = GL_INVALID_VALUE;
			piplineState.texUnitParam.texMagFilter[indexTex] = GL_INVALID_VALUE;
			piplineState.texUnitParam.texWrapU[indexTex] = GL_INVALID_VALUE;
			piplineState.texUnitParam.texWrapV[indexTex] = GL_INVALID_VALUE;
		}
	}

	void InitVertexAttribState(void)
	{
		cglGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &piplineState.vertexAttribStateParam.numVertexAttribs);
		piplineState.vertexAttribStateParam.numVertexAttribs = min(piplineState.vertexAttribStateParam.numVertexAttribs, MAX_VERTEX_ATTRIBS);

		for (GLint indexVertexAttrib = 0; indexVertexAttrib < MAX_VERTEX_ATTRIBS; indexVertexAttrib++) {
			piplineState.vertexAttribStateParam.bEnableVertexAttrib[indexVertexAttrib] = GL_INVALID_VALUE;
		}
	}

	void InitProgram(void)
	{
		piplineState.programParam.program = GL_INVALID_VALUE;

		memset(&piplineState.programParam.ptrVertex, 0, sizeof(piplineState.programParam.ptrVertex));
		memset(&piplineState.programParam.ptrColor, 0, sizeof(piplineState.programParam.ptrColor));
		memset(&piplineState.programParam.ptrTexCoord, 0, sizeof(piplineState.programParam.ptrTexCoord));
	}

	void InitProgram(PROGRAM *pProgram, const GLchar *szShaderVertexCode, const GLchar *szShaderFragmentCode)
	{
		GLint linked = 0;
		GLint compiled = 0;

		memset(pProgram, 0, sizeof(*pProgram));
		pProgram->program = cglCreateProgram();
		pProgram->shaderVertex = cglCreateShader(GL_VERTEX_SHADER);
		pProgram->shaderFragment = cglCreateShader(GL_FRAGMENT_SHADER);

		cglShaderSource(pProgram->shaderVertex, 1, &szShaderVertexCode, NULL);
		cglCompileShader(pProgram->shaderVertex);
		cglGetShaderiv(pProgram->shaderVertex, GL_COMPILE_STATUS, &compiled);
		ASSERT(compiled == GL_TRUE);

		cglShaderSource(pProgram->shaderFragment, 1, &szShaderFragmentCode, NULL);
		cglCompileShader(pProgram->shaderFragment);
		cglGetShaderiv(pProgram->shaderFragment, GL_COMPILE_STATUS, &compiled);
		ASSERT(compiled == GL_TRUE);

		cglAttachShader(pProgram->program, pProgram->shaderVertex);
		cglAttachShader(pProgram->program, pProgram->shaderFragment);
		cglLinkProgram(pProgram->program);
		cglGetProgramiv(pProgram->program, GL_LINK_STATUS, &linked);
		ASSERT(linked == GL_TRUE);

		pProgram->indexModelViewProjectionMatrix = cglGetUniformLocation(pProgram->program, UNIFORM_MODELVIEWPROJECTIONMATRIX);
		pProgram->indexTextureMatrix = cglGetUniformLocation(pProgram->program, UNIFORM_TEXTUREMATRIX);
		pProgram->indexTexture = cglGetUniformLocation(pProgram->program, UNIFORM_TEXTURE);

		pProgram->indexVertex = cglGetAttribLocation(pProgram->program, VERTEX_ATTRIB_VERTEX);
		pProgram->indexColor = cglGetAttribLocation(pProgram->program, VERTEX_ATTRIB_COLOR);
		pProgram->indexTexCoord = cglGetAttribLocation(pProgram->program, VERTEX_ATTRIB_TEXCOORD);
	}

	void DestoryProgram(PROGRAM *pProgram)
	{
		cglDetachShader(pProgram->program, pProgram->shaderVertex);
		cglDetachShader(pProgram->program, pProgram->shaderFragment);

		cglDeleteShader(pProgram->shaderVertex);
		cglDeleteShader(pProgram->shaderFragment);

		cglDeleteProgram(pProgram->program);
	}

	void UseProgram(void)
	{
		PROGRAM *pProgram = NULL;

		if (piplineState.clientStateParam.bEnableVertex == GL_TRUE && piplineState.clientStateParam.bEnableColor == GL_FALSE && piplineState.clientStateParam.bEnableTexture == GL_FALSE) {
			pProgram = &piplineState.programParam.programDepth;
		}

		if (piplineState.clientStateParam.bEnableVertex == GL_TRUE && piplineState.clientStateParam.bEnableColor == GL_TRUE && piplineState.clientStateParam.bEnableTexture == GL_FALSE) {
			pProgram = &piplineState.programParam.programColor;
		}

		if (piplineState.clientStateParam.bEnableVertex == GL_TRUE && piplineState.clientStateParam.bEnableColor == GL_FALSE && piplineState.clientStateParam.bEnableTexture == GL_TRUE) {
			pProgram = &piplineState.programParam.programTexture;
		}

		if (piplineState.clientStateParam.bEnableVertex == GL_TRUE && piplineState.clientStateParam.bEnableColor == GL_TRUE && piplineState.clientStateParam.bEnableTexture == GL_TRUE) {
			pProgram = &piplineState.programParam.programTextureColor;
		}

		if (pProgram) {
			cglUseProgram(pProgram->program);

			if (pProgram->indexModelViewProjectionMatrix >= 0) {
				cglUniformMatrix4fv(pProgram->indexModelViewProjectionMatrix, 1, GL_FALSE, cglGetModelViewProjectionMatrix(), pProgram->modelViewProjectionMatrix);
			}

			if (pProgram->indexTextureMatrix >= 0) {
				cglUniformMatrix4fv(pProgram->indexTextureMatrix, 1, GL_FALSE, cglGetTextureMatrix(GL_TEXTURE0), pProgram->textureMatrix);
			}

			if (pProgram->indexTexture >= 0) {
				cglUniform1i(pProgram->indexTexture, 0, &pProgram->texture);
			}

			if (pProgram->indexVertex >= 0) {
				cglEnableVertexAttribArray(pProgram->indexVertex);
				cglVertexAttribPointer(pProgram->indexVertex, piplineState.programParam.ptrVertex.size, piplineState.programParam.ptrVertex.type, GL_FALSE, piplineState.programParam.ptrVertex.stride, piplineState.programParam.ptrVertex.ptr);
			}

			if (pProgram->indexColor >= 0) {
				cglEnableVertexAttribArray(pProgram->indexColor);
				cglVertexAttribPointer(pProgram->indexColor, piplineState.programParam.ptrColor.size, piplineState.programParam.ptrColor.type, GL_FALSE, piplineState.programParam.ptrColor.stride, piplineState.programParam.ptrColor.ptr);
			}

			if (pProgram->indexTexCoord >= 0) {
				cglEnableVertexAttribArray(pProgram->indexTexCoord);
				cglVertexAttribPointer(pProgram->indexTexCoord, piplineState.programParam.ptrTexCoord.size, piplineState.programParam.ptrTexCoord.type, GL_FALSE, piplineState.programParam.ptrTexCoord.stride, piplineState.programParam.ptrTexCoord.ptr);
			}
		}
		else {
			cglUseProgram(0);
		}
	}

	//
	// 初始化渲染引擎
	//
	_CrossExport void InitGL(void)
	{
#if defined _PLATFORM_WINDOWS_

		esVersion = GLES_VERSION_30;
		renderType = GetRenderType();

#else

		const CHAR *szVersion = cglGetString(GL_VERSION);
		esVersion = strstr(szVersion, "OpenGL ES 3.") != NULL ? GLES_VERSION_30 : GLES_VERSION_20;
		renderType = GetRenderType();

#endif

		ResetGL();

		InitProgram(&piplineState.programParam.programDepth, szShaderVertexCode_Depth, szShaderFragmentCode_Depth);
		InitProgram(&piplineState.programParam.programColor, szShaderVertexCode_Color, szShaderFragmentCode_Color);
		InitProgram(&piplineState.programParam.programTexture, szShaderVertexCode_Texture, szShaderFragmentCode_Texture);
		InitProgram(&piplineState.programParam.programTextureColor, szShaderVertexCode_TextureColor, szShaderFragmentCode_TextureColor);
	}

	//
	// 退出渲染引擎
	//
	_CrossExport void ExitGL(void)
	{
		DestoryProgram(&piplineState.programParam.programDepth);
		DestoryProgram(&piplineState.programParam.programColor);
		DestoryProgram(&piplineState.programParam.programTexture);
		DestoryProgram(&piplineState.programParam.programTextureColor);
	}

	_CrossExport void ResetGL(void)
	{
		piplineState.bEnableStateCache = GL_TRUE;

		//
		// 1. 初始化数据
		//
		InitPiplineStates();
		InitClientState();
		InitMatrixStack();
		InitScissor();
		InitViewport();
		InitCamera();
		InitCullFace();
		InitAlpha();
		InitBlend();
		InitDepth();
		InitStencil();
		InitPolygonOffset();
		InitLine();
		InitPixelStore();
		InitSampleCoverage();
		InitClear();
		InitColorMask();
		InitBuffer();
		InitTextureUnit();
		InitVertexAttribState();
		InitProgram();

		//
		// 2. 初始化管线状态
		//
		cglDisable(GL_ALPHA_TEST);
		cglDisable(GL_BLEND);
		cglDisable(GL_CULL_FACE);
		cglDisable(GL_DEPTH_TEST);
		cglDisable(GL_LINE_SMOOTH);
		cglDisable(GL_MULTISAMPLE);
		cglDisable(GL_NORMALIZE);
		cglDisable(GL_POINT_SMOOTH);
		cglDisable(GL_POLYGON_OFFSET_FILL);
		cglDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		cglDisable(GL_SAMPLE_ALPHA_TO_ONE);
		cglDisable(GL_SAMPLE_COVERAGE);
		cglDisable(GL_SCISSOR_TEST);
		cglDisable(GL_STENCIL_TEST);
		cglDisable(GL_TEXTURE_2D);

		cglUseProgram(0);

		cglBindVertexArray(0);
		cglBindBuffer(GL_ARRAY_BUFFER, 0);
		cglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		cglDisableClientState(GL_VERTEX_ARRAY);
		cglDisableClientState(GL_NORMAL_ARRAY);
		cglDisableClientState(GL_COLOR_ARRAY);

		for (GLint indexTexUnit = 0; indexTexUnit < piplineState.texUnitParam.numTextureUnits; indexTexUnit++) {
			cglActiveTexture(GL_TEXTURE0 + indexTexUnit);
			cglDisable(GL_TEXTURE_2D);
		}
		cglActiveTexture(GL_TEXTURE0);
	}

	_CrossExport GLboolean IsOpenGLES3(void)
	{
		return esVersion == GLES_VERSION_30 ? TRUE : FALSE;
	}

	_CrossExport GLboolean IsSupportExtensionGL(const GLchar *feature)
	{
		static const GLchar *szExtension = (GLchar *)cglGetString(GL_EXTENSIONS);
		return strstr(szExtension, feature) ? GL_TRUE : GL_FALSE;
	}

	_CrossExport const GLchar* cglAttribToString(GLenum attrib)
	{
		for (GLint indexAttrib = 0; indexAttrib < sizeof(attribString) / sizeof(attribString[0]); indexAttrib++) {
			if (attribString[indexAttrib].attrib == attrib) {
				return attribString[indexAttrib].szString;
			}
		}

		return NULL;
	}

	_CrossExport GLenum cglStringToAttrib(const GLchar *str)
	{
		if (str) {
			for (GLint indexAttrib = 0; indexAttrib < sizeof(attribString) / sizeof(attribString[0]); indexAttrib++) {
				if (stricmp(attribString[indexAttrib].szString, str) == 0) {
					return attribString[indexAttrib].attrib;
				}
			}
		}

		return GL_FALSE;
	}


	_CrossExport void cglDisableClientState(GLenum array)
	{
		switch (array) {
		case GL_VERTEX_ARRAY:
			if (piplineState.clientStateParam.bEnableVertex != GL_FALSE) {
				piplineState.clientStateParam.bEnableVertex = GL_FALSE;
			}
			break;
		case GL_NORMAL_ARRAY:
			if (piplineState.clientStateParam.bEnableNormal != GL_FALSE) {
				piplineState.clientStateParam.bEnableNormal = GL_FALSE;
			}
			break;
		case GL_COLOR_ARRAY:
			if (piplineState.clientStateParam.bEnableColor != GL_FALSE) {
				piplineState.clientStateParam.bEnableColor = GL_FALSE;
			}
			break;
		case GL_TEXTURE_COORD_ARRAY:
			if (piplineState.clientStateParam.bEnableTexture != GL_FALSE) {
				piplineState.clientStateParam.bEnableTexture = GL_FALSE;
			}
			break;
		}
	}

	_CrossExport void cglEnableClientState(GLenum array)
	{
		switch (array) {
		case GL_VERTEX_ARRAY:
			if (piplineState.clientStateParam.bEnableVertex != GL_TRUE) {
				piplineState.clientStateParam.bEnableVertex = GL_TRUE;
			}
			break;
		case GL_NORMAL_ARRAY:
			if (piplineState.clientStateParam.bEnableNormal != GL_TRUE) {
				piplineState.clientStateParam.bEnableNormal = GL_TRUE;
			}
			break;
		case GL_COLOR_ARRAY:
			if (piplineState.clientStateParam.bEnableColor != GL_TRUE) {
				piplineState.clientStateParam.bEnableColor = GL_TRUE;
			}
			break;
		case GL_TEXTURE_COORD_ARRAY:
			if (piplineState.clientStateParam.bEnableTexture != GL_TRUE) {
				piplineState.clientStateParam.bEnableTexture = GL_TRUE;
			}
			break;
		}
	}

	_CrossExport void cglColorPointer(GLint size, GLenum type, GLsizei stride, const void *pointer)
	{
		piplineState.programParam.ptrColor.size = size;
		piplineState.programParam.ptrColor.type = type;
		piplineState.programParam.ptrColor.stride = stride;
		piplineState.programParam.ptrColor.ptr = const_cast<void *>(pointer);
	}

	_CrossExport void cglTexCoordPointer(GLint size, GLenum type, GLsizei stride, const void *pointer)
	{
		piplineState.programParam.ptrTexCoord.size = size;
		piplineState.programParam.ptrTexCoord.type = type;
		piplineState.programParam.ptrTexCoord.stride = stride;
		piplineState.programParam.ptrTexCoord.ptr = const_cast<void *>(pointer);
	}

	_CrossExport void cglVertexPointer(GLint size, GLenum type, GLsizei stride, const void *pointer)
	{
		piplineState.programParam.ptrVertex.size = size;
		piplineState.programParam.ptrVertex.type = type;
		piplineState.programParam.ptrVertex.stride = stride;
		piplineState.programParam.ptrVertex.ptr = const_cast<void *>(pointer);
	}

	_CrossExport void cglActiveTexture(GLenum texture)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.texUnitParam.activeTextureUnit != texture - GL_TEXTURE0) {
				glActiveTexture(texture);
			}
		}
		else {
			glActiveTexture(texture);
		}

		switch (texture) {
		case GL_TEXTURE0: piplineState.texUnitParam.activeTextureUnit = 0; break;
		case GL_TEXTURE1: piplineState.texUnitParam.activeTextureUnit = 1; break;
		case GL_TEXTURE2: piplineState.texUnitParam.activeTextureUnit = 2; break;
		case GL_TEXTURE3: piplineState.texUnitParam.activeTextureUnit = 3; break;
		case GL_TEXTURE4: piplineState.texUnitParam.activeTextureUnit = 4; break;
		case GL_TEXTURE5: piplineState.texUnitParam.activeTextureUnit = 5; break;
		case GL_TEXTURE6: piplineState.texUnitParam.activeTextureUnit = 6; break;
		case GL_TEXTURE7: piplineState.texUnitParam.activeTextureUnit = 7; break;
		default: ASSERT(GL_FALSE); break;
		}
	}

	_CrossExport void cglBindBuffer(GLenum target, GLuint buffer)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			switch (target) {
			case GL_ARRAY_BUFFER:
				if (piplineState.bufferParam.vbo != buffer) {
					piplineState.bufferParam.vbo = buffer;
					glBindBuffer(target, buffer);
				}
				break;
			case GL_ELEMENT_ARRAY_BUFFER:
				if (piplineState.bufferParam.ibo != buffer) {
					piplineState.bufferParam.ibo = buffer;
					glBindBuffer(target, buffer);
				}
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		else {
			glBindBuffer(target, buffer);
		}
	}

	_CrossExport void cglBindFramebuffer(GLenum target, GLuint framebuffer)
	{
		glBindFramebuffer(target, framebuffer);
	}

	_CrossExport void cglBindRenderbuffer(GLenum target, GLuint renderbuffer)
	{
		glBindRenderbuffer(target, renderbuffer);
	}

	_CrossExport void cglBindTexture(GLenum target, GLuint texture)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.texUnitParam.texture[piplineState.texUnitParam.activeTextureUnit] != texture) {
				piplineState.texUnitParam.texture[piplineState.texUnitParam.activeTextureUnit] = texture;
				glBindTexture(target, texture);
			}
		}
		else {
			glBindTexture(target, texture);
		}
	}

	_CrossExport void cglBindVertexArray(GLuint array)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.bufferParam.vao != array) {
				piplineState.bufferParam.vao = array;
				glBindVertexArray(array);
			}
		}
		else {
			glBindVertexArray(array);
		}
	}

	_CrossExport void cglBlendFunc(GLenum sfactor, GLenum dfactor)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.blendParam.sfactor != sfactor || piplineState.blendParam.dfactor != dfactor) {
				piplineState.blendParam.sfactor = sfactor;
				piplineState.blendParam.dfactor = dfactor;
				glBlendFunc(sfactor, dfactor);
			}
		}
		else {
			glBlendFunc(sfactor, dfactor);
		}
	}

	_CrossExport void cglBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
	{
		glBufferData(target, size, data, usage);
	}

	_CrossExport void cglBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data)
	{
		glBufferSubData(target, offset, size, data);
	}

	_CrossExport GLenum cglCheckFramebufferStatus(GLenum target)
	{
		return glCheckFramebufferStatus(target);
	}

	_CrossExport void cglClear(GLbitfield mask)
	{
		glClear(mask);
	}

	_CrossExport void cglClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.clearParam.red != red || piplineState.clearParam.green != green || piplineState.clearParam.blue != blue || piplineState.clearParam.alpha != alpha) {
				piplineState.clearParam.red = red;
				piplineState.clearParam.green = green;
				piplineState.clearParam.blue = blue;
				piplineState.clearParam.alpha = alpha;
				glClearColor(red, green, blue, alpha);
			}
		}
		else {
			glClearColor(red, green, blue, alpha);
		}
	}

	_CrossExport void cglClearDepth(GLclampf depth)
	{
		// 注意: 在Intel集成显卡上会导致当机!!!
		if (renderType != RENDER_INTEL_HD_GRAPHICS_3000) {
			if (piplineState.bEnableStateCache == GL_TRUE) {
				if (piplineState.clearParam.depth != depth) {
					piplineState.clearParam.depth = depth;
					glClearDepthf(depth);
				}
			}
			else {
				glClearDepthf(depth);
			}
		}
	}

	_CrossExport void cglClearStencil(GLint stencil)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.clearParam.stencil != stencil) {
				piplineState.clearParam.stencil = stencil;
				glClearStencil(stencil);
			}
		}
		else {
			glClearStencil(stencil);
		}
	}

	_CrossExport void cglColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.colorMaskParam.red != red || piplineState.colorMaskParam.green != green || piplineState.colorMaskParam.blue != blue || piplineState.colorMaskParam.alpha != alpha) {
				piplineState.colorMaskParam.red = red;
				piplineState.colorMaskParam.green = green;
				piplineState.colorMaskParam.blue = blue;
				piplineState.colorMaskParam.alpha = alpha;
				glColorMask(red, green, blue, alpha);
			}
		}
		else {
			glColorMask(red, green, blue, alpha);
		}
	}

	_CrossExport void cglCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data)
	{
		glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
	}

	_CrossExport void cglCompressedTexImage2DMipmapsDDS(GLenum target, GLint maxresolution, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLuint bitcount, const void *pixels)
	{
		GLint mipSize = 0;
		GLint mipOffset = 0;

		GLint numSurface = target == GL_TEXTURE_CUBE_MAP ? 6 : 1;
		GLenum texTarget = target == GL_TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP_POSITIVE_X : target;

		for (GLint indexSurface = 0; indexSurface < numSurface; indexSurface++) {
			GLint levelCount = 0;
			GLint levelWidth = width;
			GLint levelHeight = height;

			for (GLint indexLevel = 0; indexLevel <= level; indexLevel++) {
				mipSize = ((levelWidth + 3) / 4)*((levelHeight + 3) / 4)*bitcount;
				if (levelWidth <= maxresolution && levelHeight <= maxresolution) {
					cglCompressedTexImage2D(texTarget + indexSurface, levelCount, internalformat, levelWidth, levelHeight, border, mipSize, (GLubyte *)pixels + mipOffset);
					levelCount++;
				}
				mipOffset += mipSize;

				levelWidth >>= 1;
				levelHeight >>= 1;

				if (0 == levelWidth && 0 == levelHeight) {
					break;
				}

				if (0 == levelWidth) levelWidth = 1;
				if (0 == levelHeight) levelHeight = 1;
			}
		}
	}

	_CrossExport void cglCompressedTexImage2DMipmapsETC(GLenum target, GLint maxresolution, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLuint bitcount, const void *pixels)
	{
		GLint mipSize = 0;
		GLint mipOffset = 0;

		GLint numSurface = target == GL_TEXTURE_CUBE_MAP ? 6 : 1;
		GLenum texTarget = target == GL_TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP_POSITIVE_X : target;

		for (GLint indexSurface = 0; indexSurface < numSurface; indexSurface++) {
			GLint levelCount = 0;
			GLint levelWidth = width;
			GLint levelHeight = height;

			for (GLint indexLevel = 0; indexLevel <= level; indexLevel++) {
				mipSize = (max(levelWidth, ETC_MIN_TEXWIDTH) * max(levelHeight, ETC_MIN_TEXHEIGHT) * bitcount) / 8;

				if (levelWidth <= maxresolution && levelHeight <= maxresolution) {
					cglCompressedTexImage2D(texTarget + indexSurface, levelCount, internalformat, levelWidth, levelHeight, border, mipSize, (GLubyte *)pixels + mipOffset);
					levelCount++;
				}
				mipOffset += mipSize;

				levelWidth >>= 1;
				levelHeight >>= 1;

				if (0 == levelWidth && 0 == levelHeight) {
					break;
				}

				if (0 == levelWidth) levelWidth = 1;
				if (0 == levelHeight) levelHeight = 1;
			}
		}
	}

	_CrossExport void cglCompressedTexImage2DMipmapsPVR(GLenum target, GLint maxresolution, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLuint bitcount, const void *pixels)
	{
		GLint mipSize = 0;
		GLint mipOffset = 0;

		GLint numSurface = target == GL_TEXTURE_CUBE_MAP ? 6 : 1;
		GLenum texTarget = target == GL_TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP_POSITIVE_X : target;

		for (GLint indexSurface = 0; indexSurface < numSurface; indexSurface++) {
			GLint levelCount = 0;
			GLint levelWidth = width;
			GLint levelHeight = height;

			for (GLint indexLevel = 0; indexLevel <= level; indexLevel++) {
				switch (bitcount) {
				case 2: mipSize = (max(levelWidth, PVRTC2_MIN_TEXWIDTH) * max(levelHeight, PVRTC2_MIN_TEXHEIGHT) * bitcount) / 8; break;
				case 4: mipSize = (max(levelWidth, PVRTC4_MIN_TEXWIDTH) * max(levelHeight, PVRTC4_MIN_TEXHEIGHT) * bitcount) / 8; break;
				}

				if (levelWidth <= maxresolution && levelHeight <= maxresolution) {
					cglCompressedTexImage2D(texTarget + indexSurface, levelCount, internalformat, levelWidth, levelHeight, border, mipSize, (GLubyte *)pixels + mipOffset);
					levelCount++;
				}
				mipOffset += mipSize;

				levelWidth >>= 1;
				levelHeight >>= 1;

				if (0 == levelWidth && 0 == levelHeight) {
					break;
				}

				if (0 == levelWidth) levelWidth = 1;
				if (0 == levelHeight) levelHeight = 1;
			}
		}
	}

	_CrossExport void cglCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data)
	{
		glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
	}

	_CrossExport void cglCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
	{
		glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
	}

	_CrossExport void cglCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
	{
		glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
	}

	_CrossExport void cglCullFace(GLenum mode)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.cullFaceParam.cull != mode) {
				piplineState.cullFaceParam.cull = mode;
				glCullFace(mode);
			}
		}
		else {
			glCullFace(mode);
		}
	}

	_CrossExport void cglDeleteBuffers(GLsizei n, const GLuint *buffers)
	{
		for (GLint indexBuffer = 0; indexBuffer < n; indexBuffer++) {
			if (piplineState.bufferParam.vbo == buffers[indexBuffer]) {
				piplineState.bufferParam.vbo = GL_INVALID_VALUE;
			}

			if (piplineState.bufferParam.ibo == buffers[indexBuffer]) {
				piplineState.bufferParam.ibo = GL_INVALID_VALUE;
			}
		}

		glDeleteBuffers(n, buffers);
	}

	_CrossExport void cglDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
	{
		glDeleteFramebuffers(n, framebuffers);
	}

	_CrossExport void cglDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
	{
		glDeleteRenderbuffers(n, renderbuffers);
	}

	_CrossExport void cglDeleteTextures(GLsizei n, const GLuint *textures)
	{
		for (GLint indexTex = 0; indexTex < n; indexTex++) {
			for (GLint indexTexUnit = 0; indexTexUnit < piplineState.texUnitParam.numTextureUnits; indexTexUnit++) {
				if (piplineState.texUnitParam.texture[indexTexUnit] == textures[indexTex]) {
					piplineState.texUnitParam.texture[indexTexUnit] = GL_INVALID_VALUE;
				}
			}

			if (textures[indexTex] < MAX_TEXTURES) {
				piplineState.texUnitParam.texMinFliter[textures[indexTex]] = GL_INVALID_VALUE;
				piplineState.texUnitParam.texMagFilter[textures[indexTex]] = GL_INVALID_VALUE;
				piplineState.texUnitParam.texWrapU[textures[indexTex]] = GL_INVALID_VALUE;
				piplineState.texUnitParam.texWrapV[textures[indexTex]] = GL_INVALID_VALUE;
			}
		}

		glDeleteTextures(n, textures);
	}

	_CrossExport void cglDeleteVertexArrays(GLsizei n, const GLuint *arrays)
	{
		for (GLint indexVAO = 0; indexVAO < n; indexVAO++) {
			if (piplineState.bufferParam.vao == arrays[indexVAO]) {
				piplineState.bufferParam.vao = GL_INVALID_VALUE;
			}
		}

		glDeleteVertexArrays(n, arrays);
	}

	_CrossExport void cglDepthFunc(GLenum func)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.depthParam.func != func) {
				piplineState.depthParam.func = func;
				glDepthFunc(func);
			}
		}
		else {
			glDepthFunc(func);
		}
	}

	_CrossExport void cglDepthMask(GLboolean mask)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.depthParam.mask != mask) {
				piplineState.depthParam.mask = mask;
				glDepthMask(mask);
			}
		}
		else {
			glDepthMask(mask);
		}
	}

	_CrossExport void cglDepthRange(GLclampf znear, GLclampf zfar)
	{
		glDepthRangef(znear, zfar);
	}

	_CrossExport void cglDisable(GLenum cap)
	{
		if (IsCapParamValid(cap)) {
			if (piplineState.bEnableStateCache == GL_TRUE) {
				GLint indexState = CapIndex(cap);

				if (indexState >= 0) {
					if (piplineState.capabilities[indexState] != GL_FALSE) {
						piplineState.capabilities[indexState] = GL_FALSE;
						glDisable(cap);
					}
				}

				if (cap == GL_TEXTURE_2D) {
					if (piplineState.texUnitParam.texEanble[piplineState.texUnitParam.activeTextureUnit] != GL_FALSE) {
						piplineState.texUnitParam.texEanble[piplineState.texUnitParam.activeTextureUnit] = GL_FALSE;
						glDisable(cap);
					}
				}
			}
			else {
				glDisable(cap);
			}
		}
	}

	_CrossExport void cglDrawArrays(GLenum mode, GLint first, GLsizei count)
	{
		glDrawArrays(mode, first, count);
	}

	_CrossExport void cglDrawArraysCompatible(GLenum mode, GLint first, GLsizei count)
	{
		UseProgram();
		glDrawArrays(mode, first, count);
	}

	_CrossExport void cglDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices)
	{
		glDrawElements(mode, count, type, indices);
	}

	_CrossExport void cglDrawElementsCompatible(GLenum mode, GLsizei count, GLenum type, const void *indices)
	{
		UseProgram();
		glDrawElements(mode, count, type, indices);
	}

	_CrossExport void cglEnable(GLenum cap)
	{
		if (IsCapParamValid(cap)) {
			if (piplineState.bEnableStateCache == GL_TRUE) {
				GLint indexState = CapIndex(cap);

				if (indexState >= 0) {
					if (piplineState.capabilities[indexState] != GL_TRUE) {
						piplineState.capabilities[indexState] = GL_TRUE;
						glEnable(cap);
					}
				}

				if (cap == GL_TEXTURE_2D) {
					if (piplineState.texUnitParam.texEanble[piplineState.texUnitParam.activeTextureUnit] != GL_TRUE) {
						piplineState.texUnitParam.texEanble[piplineState.texUnitParam.activeTextureUnit] = GL_TRUE;
						glEnable(cap);
					}
				}
			}
			else {
				glEnable(cap);
			}
		}
	}

	_CrossExport void cglFinish(void)
	{
		glFinish();
	}

	_CrossExport void cglFlush(void)
	{
		glFlush();
	}

	_CrossExport void cglFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
	{
		glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
	}

	_CrossExport void cglFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
	{
		glFramebufferTexture2D(target, attachment, textarget, texture, level);
	}

	_CrossExport void cglFrontFace(GLenum mode)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.cullFaceParam.front != mode) {
				piplineState.cullFaceParam.front = mode;
				glFrontFace(mode);
			}
		}
		else {
			glFrontFace(mode);
		}
	}

	_CrossExport void cglGenBuffers(GLsizei n, GLuint *buffers)
	{
		glGenBuffers(n, buffers);
	}

	_CrossExport void cglGenFramebuffers(GLsizei n, GLuint *framebuffers)
	{
		glGenFramebuffers(n, framebuffers);
	}

	_CrossExport void cglGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
	{
		glGenRenderbuffers(n, renderbuffers);
	}

	_CrossExport void cglGenTextures(GLsizei n, GLuint *textures)
	{
		glGenTextures(n, textures);
	}

	_CrossExport void cglGenVertexArrays(GLsizei n, GLuint *arrays)
	{
		glGenVertexArrays(n, arrays);
	}

	_CrossExport void cglGetBooleanv(GLenum pname, GLboolean *params)
	{
		glGetBooleanv(pname, params);
	}

	_CrossExport void cglGetBufferParameteriv(GLenum target, GLenum pname, GLint *params)
	{
		glGetBufferParameteriv(target, pname, params);
	}

	_CrossExport GLenum cglGetError(void)
	{
		return glGetError();
	}

	_CrossExport void cglGetFloatv(GLenum pname, GLfloat *params)
	{
		glGetFloatv(pname, params);
	}

	_CrossExport void cglGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
	{
		glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
	}

	_CrossExport void cglGetIntegerv(GLenum pname, GLint *params)
	{
		glGetIntegerv(pname, params);
	}

	_CrossExport void cglGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
	{
		glGetRenderbufferParameteriv(target, pname, params);
	}

	#define MAX_DATA_LEN 4096
	#define MAX_SEPARATORS  6

	#define TEGRA_SOC_ERROR  -2 // detection method failed, error.
	#define TEGRA_SOC_NOTNV  -1 // detect succeeded, but not a tegra
	#define TEGRA_SOC_UNKNOWN 0 // detect succeeded, seems like a tegra, not sure what though.
	#define TEGRA_SOC_T2      1 // likely a tegra 2
	#define TEGRA_SOC_T3      2 // likely a tegra 3
	#define TEGRA_SOC_T4      3 // likely a tegra 4
	#define TEGRA_SOC_T4i     4 // likely a tegra 4i
	#define TEGRA_SOC_FUTURE  5 // likely a future tegra

	static GLboolean CharIsSeparator(GLchar c)
	{
		GLchar separators[MAX_SEPARATORS] = { ' ', '\t', '\n', '\r', '\0', ':' };

		for (GLint i = 0; i < MAX_SEPARATORS; i++) {
			if (c == separators[i]) {
				return GL_TRUE;
			}
		}

		return GL_FALSE;
	}

	static GLint ReadDataFile(const GLchar *fname, GLchar *outbuf, size_t buflen)
	{
		if (buflen == 0) {
			return -1;
		}

		if (outbuf == NULL) {
			return -1;
		}

		if (fname == NULL) {
			return -1;
		}

		FILE *f = fopen(fname, "r");
		if (f == NULL) {
			return -1;
		}

		GLint count = (GLint)fread(outbuf, 1, buflen, f);
		outbuf[buflen - 1] = 0;
		fclose(f);

		return count;
	}

	static GLint GetTegraSocFamily(void)
	{
		GLchar databuf[MAX_DATA_LEN];

		GLint dataLen;
		GLint cpuFamily = -1;

		dataLen = ReadDataFile("/sys/bus/soc/devices/soc0/family", databuf, MAX_DATA_LEN);
		if (dataLen <= 1) {
			return TEGRA_SOC_ERROR;
		}

		databuf[dataLen] = 0;
		if (CharIsSeparator(databuf[dataLen - 1])) {
			databuf[--dataLen] = 0;
		}

		if (strncmp(databuf, "Tegra", 5)) {
			return TEGRA_SOC_NOTNV;
		}

		cpuFamily = atoi(databuf + 5);

		if (cpuFamily == 2) return TEGRA_SOC_T2;
		if (cpuFamily == 3) return TEGRA_SOC_T3;
		if (cpuFamily == 14) return TEGRA_SOC_T4i;
		if (cpuFamily == 11) return TEGRA_SOC_T4;
		if (cpuFamily >= 12) return TEGRA_SOC_FUTURE; // if we get here, we will assume no future chip less than family value 12.

		return TEGRA_SOC_UNKNOWN;
	}

	_CrossExport const GLchar* cglGetString(GLenum name)
	{
		if (name == GL_RENDERER) {
			const GLchar *szRender = (const GLchar *)glGetString(name);
			ASSERT(szRender);

			if (strncmp(szRender, "NVIDIA Tegra", strlen("NVIDIA Tegra")) == 0) {
				switch (GetTegraSocFamily()) {
				case TEGRA_SOC_T2: return "NVIDIA Tegra2";
				case TEGRA_SOC_T3: return "NVIDIA Tegra3";
				case TEGRA_SOC_T4: return "NVIDIA Tegra4";
				case TEGRA_SOC_T4i: return "NVIDIA Tegra4i";
				case TEGRA_SOC_FUTURE: return "NVIDIA Tegra Future";
				}
			}

			return szRender;
		}

		return (const GLchar *)glGetString(name);
	}

	_CrossExport void cglGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
	{
		glGetTexParameterfv(target, pname, params);
	}

	_CrossExport void cglGetTexParameteriv(GLenum target, GLenum pname, GLint *params)
	{
		glGetTexParameteriv(target, pname, params);
	}

	_CrossExport void cglHint(GLenum target, GLenum mode)
	{
		glHint(target, mode);
	}

	_CrossExport GLboolean cglIsBuffer(GLuint buffer)
	{
		return glIsBuffer(buffer);
	}

	_CrossExport GLboolean cglIsEnabled(GLenum cap)
	{
		if (IsCapParamValid(cap)) {
			if (piplineState.bEnableStateCache == GL_TRUE) {
				GLint indexState = CapIndex(cap);

				if (indexState >= 0) {
					return piplineState.capabilities[indexState] == GL_TRUE ? GL_TRUE : GL_FALSE;
				}
			}
			else {
				return glIsEnabled(cap);
			}
		}

		return GL_FALSE;
	}

	_CrossExport GLboolean cglIsFramebuffer(GLuint framebuffer)
	{
		return glIsFramebuffer(framebuffer);
	}

	_CrossExport GLboolean cglIsRenderbuffer(GLuint renderbuffer)
	{
		return glIsRenderbuffer(renderbuffer);
	}

	_CrossExport GLboolean cglIsTexture(GLuint texture)
	{
		return glIsTexture(texture);
	}

	_CrossExport GLboolean cglIsVertexArray(GLuint array)
	{
		return glIsVertexArray(array);
	}

	_CrossExport void cglLineWidth(GLfloat width)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.lineWidthParam.width != width) {
				piplineState.lineWidthParam.width = width;
				glLineWidth(width);
			}
		}
		else {
			glLineWidth(width);
		}
	}

	_CrossExport void cglPixelStorei(GLenum pname, GLint param)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.pixelStoreParam.pname != pname || piplineState.pixelStoreParam.param != param) {
				piplineState.pixelStoreParam.pname = pname;
				piplineState.pixelStoreParam.param = param;
				glPixelStorei(pname, param);
			}
		}
		else {
			glPixelStorei(pname, param);
		}
	}

	_CrossExport void cglPolygonOffset(GLfloat factor, GLfloat units)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.polygonOffsetParam.factor != factor || piplineState.polygonOffsetParam.units != units) {
				piplineState.polygonOffsetParam.factor = factor;
				piplineState.polygonOffsetParam.units = units;
				glPolygonOffset(factor, units);
			}
		}
		else {
			glPolygonOffset(factor, units);
		}
	}

	_CrossExport void cglReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
	{
		glReadPixels(x, y, width, height, format, type, pixels);
	}

	_CrossExport void cglRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
	{
		glRenderbufferStorage(target, internalformat, width, height);
	}

	_CrossExport void cglSampleCoverage(GLclampf value, GLboolean invert)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.sampleCoverageParam.value != value || piplineState.sampleCoverageParam.invert != invert) {
				piplineState.sampleCoverageParam.value = value;
				piplineState.sampleCoverageParam.invert = invert;
				glSampleCoverage(value, invert);
			}
		}
		else {
			glSampleCoverage(value, invert);
		}
	}

	_CrossExport void cglScissor(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.scissorParam.x != x || piplineState.scissorParam.y != y || piplineState.scissorParam.width != width || piplineState.scissorParam.height != height) {
				piplineState.scissorParam.x = x;
				piplineState.scissorParam.y = y;
				piplineState.scissorParam.width = width;
				piplineState.scissorParam.height = height;
				glScissor(x, y, width, height);
			}
		}
		else {
			glScissor(x, y, width, height);
		}

	}

	_CrossExport void cglStencilFunc(GLenum func, GLint ref, GLuint mask)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.stencilParam.func != func || piplineState.stencilParam.ref != ref || piplineState.stencilParam.mask != mask) {
				piplineState.stencilParam.func = func;
				piplineState.stencilParam.ref = ref;
				piplineState.stencilParam.mask = mask;
				glStencilFunc(func, ref, mask);
			}
		}
		else {
			glStencilFunc(func, ref, mask);
		}
	}

	_CrossExport void cglStencilMask(GLuint mask)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.stencilParam.mask != mask) {
				piplineState.stencilParam.mask = mask;
				glStencilMask(mask);
			}
		}
		else {
			glStencilMask(mask);
		}
	}

	_CrossExport void cglStencilOp(GLenum sfail, GLenum zfail, GLenum zpass)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.stencilParam.sfail != sfail || piplineState.stencilParam.zfail != zfail || piplineState.stencilParam.zpass != zpass) {
				piplineState.stencilParam.sfail = sfail;
				piplineState.stencilParam.zfail = zfail;
				piplineState.stencilParam.zpass = zpass;
				glStencilOp(sfail, zfail, zpass);
			}
		}
		else {
			glStencilOp(sfail, zfail, zpass);
		}
	}

	_CrossExport void cglTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
	{
		glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
	}

	_CrossExport void cglTexImage2DMipmaps(GLenum target, GLint maxresolution, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, GLuint bitcount, const void *pixels)
	{
		GLint mipSize = 0;
		GLint mipOffset = 0;

		GLint numSurface = target == GL_TEXTURE_CUBE_MAP ? 6 : 1;
		GLenum texTarget = target == GL_TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP_POSITIVE_X : target;

		for (GLint indexSurface = 0; indexSurface < numSurface; indexSurface++) {
			GLint levelCount = 0;
			GLint levelWidth = width;
			GLint levelHeight = height;

			for (GLint indexLevel = 0; indexLevel <= level; indexLevel++) {
				mipSize = levelWidth*levelHeight*bitcount / 8;
				if (levelWidth <= maxresolution && levelHeight <= maxresolution) {
					cglTexImage2D(texTarget + indexSurface, levelCount, internalformat, levelWidth, levelHeight, border, format, type, (GLubyte *)pixels + mipOffset);
					levelCount++;
				}
				mipOffset += mipSize;

				levelWidth >>= 1;
				levelHeight >>= 1;

				if (0 == levelWidth && 0 == levelHeight) {
					break;
				}

				if (0 == levelWidth) levelWidth = 1;
				if (0 == levelHeight) levelHeight = 1;
			}
		}
	}

	_CrossExport void cglTexParameterf(GLenum target, GLenum pname, GLfloat param)
	{
		glTexParameterf(target, pname, param);
	}

	_CrossExport void cglTexParameterfv(GLenum target, GLenum pname, const GLfloat *params)
	{
		glTexParameterfv(target, pname, params);
	}

	_CrossExport void cglTexParameteri(GLenum target, GLenum pname, GLint param)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			GLint indexTexUnit = piplineState.texUnitParam.activeTextureUnit;
			GLint indexTex = piplineState.texUnitParam.texture[indexTexUnit];

			if (indexTex < MAX_TEXTURES) {
				switch (pname) {
				case GL_TEXTURE_MIN_FILTER:
					if (piplineState.texUnitParam.texMinFliter[indexTex] != param) {
						piplineState.texUnitParam.texMinFliter[indexTex] = param;
						glTexParameteri(target, pname, param);
					}
					break;
				case GL_TEXTURE_MAG_FILTER:
					if (piplineState.texUnitParam.texMagFilter[indexTex] != param) {
						piplineState.texUnitParam.texMagFilter[indexTex] = param;
						glTexParameteri(target, pname, param);
					}
					break;
				case GL_TEXTURE_WRAP_S:
					if (piplineState.texUnitParam.texWrapU[indexTex] != param) {
						piplineState.texUnitParam.texWrapU[indexTex] = param;
						glTexParameteri(target, pname, param);
					}
					break;
				case GL_TEXTURE_WRAP_T:
					if (piplineState.texUnitParam.texWrapV[indexTex] != param) {
						piplineState.texUnitParam.texWrapV[indexTex] = param;
						glTexParameteri(target, pname, param);
					}
					break;
				default:
					glTexParameteri(target, pname, param);
					break;
				}
			}
			else {
				glTexParameteri(target, pname, param);
			}
		}
		else {
			glTexParameteri(target, pname, param);
		}
	}

	_CrossExport void cglTexParameteriv(GLenum target, GLenum pname, const GLint *params)
	{
		glTexParameteriv(target, pname, params);
	}

	_CrossExport void cglTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
	{
		glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
	}

	_CrossExport void cglViewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.viewportParam.x != x || piplineState.viewportParam.y != y || piplineState.viewportParam.width != width || piplineState.viewportParam.height != height) {
				piplineState.viewportParam.x = x;
				piplineState.viewportParam.y = y;
				piplineState.viewportParam.width = width;
				piplineState.viewportParam.height = height;
				glViewport(x, y, width, height);
			}
		}
		else {
			glViewport(x, y, width, height);
		}
	}

	_CrossExport void cglAlphaFunc(GLenum func, GLclampf ref)
	{
		piplineState.alphaParam.func = func;
		piplineState.alphaParam.ref = ref;
	}

	_CrossExport void cglMatrixMode(GLenum mode)
	{
		switch (mode) {
		case GL_MODELWORLD:
			piplineState.matrixParam.matrixMode = MATRIX_MODE_MODELWORLD;
			return;

		case GL_MODELVIEW:
			piplineState.matrixParam.matrixMode = MATRIX_MODE_MODELVIEW;
			break;

		case GL_PROJECTION:
			piplineState.matrixParam.matrixMode = MATRIX_MODE_PROJECTION;
			break;
		/*
		case GL_TEXTURE:
			switch (piplineState.texUnitParam.activeTextureUnit) {
			case 0: piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE0; break;
			case 1: piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE1; break;
			case 2: piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE2; break;
			case 3: piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE3; break;
			case 4: piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE4; break;
			case 5: piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE5; break;
			case 6: piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE6; break;
			case 7: piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE7; break;
			default: ASSERT(GL_FALSE); break;
			}
			break;
		*/
		case GL_TEXTURE0:
			piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE0;
			break;

		case GL_TEXTURE1:
			piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE1;
			break;

		case GL_TEXTURE2:
			piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE2;
			break;

		case GL_TEXTURE3:
			piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE3;
			break;

		case GL_TEXTURE4:
			piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE4;
			break;

		case GL_TEXTURE5:
			piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE5;
			break;

		case GL_TEXTURE6:
			piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE6;
			break;

		case GL_TEXTURE7:
			piplineState.matrixParam.matrixMode = MATRIX_MODE_TEXTURE7;
			break;

		default:
			ASSERT(FALSE);
			break;
		}
	}

	_CrossExport void cglPopMatrix(void)
	{
		if (piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].indexStack > 0) {
			piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].indexStack--;
			cglLoadMatrixf(piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].matrices[piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].indexStack]);
		}
	}

	_CrossExport void cglPushMatrix(void)
	{
		if (piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].indexStack < MAX_MATRIX_STACK_DEPTH - 1) {
			MtxCopy(
				(MATRIX4*)piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].matrices[piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].indexStack + 1],
				(MATRIX4*)piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].matrices[piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].indexStack + 0]);

			piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].indexStack++;
			cglLoadMatrixf(piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].matrices[piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].indexStack]);
		}
	}

	_CrossExport void cglMultMatrixf(const GLfloat *m)
	{
		GLfloat matrixParam[16];
		MtxMul((MATRIX4*)matrixParam, (MATRIX4*)m, (MATRIX4*)piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].matrices[piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].indexStack]);
		cglLoadMatrixf(matrixParam);
	}

	_CrossExport void cglLoadMatrixf(const GLfloat *m)
	{
		MtxCopy(
			(MATRIX4*)piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].matrices[piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].indexStack],
			(MATRIX4*)m);

		switch (piplineState.matrixParam.matrixMode) {
		case MATRIX_MODE_MODELWORLD:
			MtxCopy(
				(MATRIX4*)piplineState.matrixParam.modelWorldMatrix,
				(MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_MODELWORLD].matrices[piplineState.matrixParam.matrixStack[MATRIX_MODE_MODELWORLD].indexStack]);
			break;

		case MATRIX_MODE_MODELVIEW:
			MtxCopy(
				(MATRIX4*)piplineState.matrixParam.modelViewMatrix,
				(MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_MODELVIEW].matrices[piplineState.matrixParam.matrixStack[MATRIX_MODE_MODELVIEW].indexStack]);

			MtxInverse(
				(MATRIX4*)piplineState.matrixParam.modelViewInverseMatrix,
				(MATRIX4*)piplineState.matrixParam.modelViewMatrix);

			MtxTranspose(
				(MATRIX4*)piplineState.matrixParam.modelViewInverseTransposeMatrix,
				(MATRIX4*)piplineState.matrixParam.modelViewInverseMatrix);

		case MATRIX_MODE_PROJECTION:
			MtxMul(
				(MATRIX4*)piplineState.matrixParam.modelViewProjectionMatrix,
				(MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_MODELVIEW].matrices[piplineState.matrixParam.matrixStack[MATRIX_MODE_MODELVIEW].indexStack],
				(MATRIX4*)piplineState.matrixParam.matrixStack[MATRIX_MODE_PROJECTION].matrices[piplineState.matrixParam.matrixStack[MATRIX_MODE_PROJECTION].indexStack]);
			break;

		case MATRIX_MODE_TEXTURE0:
		case MATRIX_MODE_TEXTURE1:
		case MATRIX_MODE_TEXTURE2:
		case MATRIX_MODE_TEXTURE3:
		case MATRIX_MODE_TEXTURE4:
		case MATRIX_MODE_TEXTURE5:
		case MATRIX_MODE_TEXTURE6:
		case MATRIX_MODE_TEXTURE7:
			MtxCopy(
				(MATRIX4*)piplineState.matrixParam.textureMatrices[piplineState.matrixParam.matrixMode - MATRIX_MODE_TEXTURE0],
				(MATRIX4*)piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].matrices[piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].indexStack]);
			break;

		default:
			ASSERT(GL_FALSE);
			break;
		}
	}

	_CrossExport void cglLoadIdentity(void)
	{
		MtxSet((MATRIX4*)piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].matrices[piplineState.matrixParam.matrixStack[piplineState.matrixParam.matrixMode].indexStack],
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	_CrossExport void cglScalef(GLfloat x, GLfloat y, GLfloat z)
	{
		GLfloat matrixParam[16];

		MtxSet((MATRIX4*)matrixParam,
			x,    0.0f, 0.0f, 0.0f,
			0.0f, y,    0.0f, 0.0f,
			0.0f, 0.0f, z,    0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		cglMultMatrixf(matrixParam);
	}

	_CrossExport void cglRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
	{
		VEC3 axis;

		GLfloat matrixParam[16];
		GLfloat sine = FastSin(DEG2RAD(angle));
		GLfloat cosine = FastCos(DEG2RAD(angle));
		GLfloat one_minus_cosine = 1.0f - cosine;

		Vec3Set(&axis, x, y, z);
		Vec3Normalize(&axis);

		MtxSet((MATRIX4*)matrixParam,
			one_minus_cosine*axis[0] * axis[0] + cosine, one_minus_cosine*axis[0] * axis[1] + axis[2] * sine, one_minus_cosine*axis[0] * axis[2] - axis[1] * sine, 0.0f,
			one_minus_cosine*axis[0] * axis[1] - axis[2] * sine, one_minus_cosine*axis[1] * axis[1] + cosine, one_minus_cosine*axis[1] * axis[2] + axis[0] * sine, 0.0f,
			one_minus_cosine*axis[0] * axis[2] + axis[1] * sine, one_minus_cosine*axis[1] * axis[2] - axis[0] * sine, one_minus_cosine*axis[2] * axis[2] + cosine, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		cglMultMatrixf(matrixParam);
	}

	_CrossExport void cglTranslatef(GLfloat x, GLfloat y, GLfloat z)
	{
		GLfloat matrixParam[16];

		MtxSet((MATRIX4*)matrixParam,
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x,    y,    z,    1.0f);

		cglMultMatrixf(matrixParam);
	}

	_CrossExport void cglOrtho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar)
	{
		GLfloat matrixParam[16];
		GLfloat invWidth = 1.0f / (right - left);
		GLfloat invHeight = 1.0f / (top - bottom);
		GLfloat invDepth = 1.0f / (zfar - znear);

		MtxSet((MATRIX4*)matrixParam,
			2.0f*invWidth,             0.0f,                      0.0f,                    0.0f,
			0.0f,                      2.0f*invHeight,            0.0f,                    0.0f,
			0.0f,                      0.0f,                     -2.0f*invDepth,           0.0f,
			-(right + left)*invWidth, -(top + bottom)*invHeight, -(zfar + znear)*invDepth, 1.0f);

		cglMultMatrixf(matrixParam);
	}

	_CrossExport void cglFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar)
	{
		GLfloat matrixParam[16];
		GLfloat invWidth = 1.0f / (right - left);
		GLfloat invHeight = 1.0f / (top - bottom);
		GLfloat invDepth = 1.0f / (zfar - znear);

		MtxSet((MATRIX4*)matrixParam,
			2.0f*znear*invWidth,     0.0f,                      0.0f,                     0.0f,
			0.0f,                    2.0f*znear*invHeight,      0.0f,                     0.0f,
			(right + left)*invWidth, (top + bottom)*invHeight, -(zfar + znear)*invDepth, -1.0f,
			0.0f,                    0.0f,                     -2.0f*zfar*znear*invDepth, 0.0f);

		cglMultMatrixf(matrixParam);
	}

	_CrossExport void cglPerspective(GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar)
	{
		GLfloat matrixParam[16];
		GLfloat d = 1.0f / FastTan(DEG2RAD(fovy)*0.5f);
		GLfloat daspect = d / aspect;
		GLfloat invDepth = 1.0f / (zfar - znear);

		MtxSet((MATRIX4*)matrixParam,
			daspect, 0.0f,  0.0f,                     0.0f,
			0.0f,    d,     0.0f,                     0.0f,
			0.0f,    0.0f, -(zfar + znear)*invDepth, -1.0f,
			0.0f,    0.0f, -2.0f*zfar*znear*invDepth, 0.0f);

		cglMultMatrixf(matrixParam);

		piplineState.cameraParam.fovy = fovy;
		piplineState.cameraParam.aspect = aspect;
		piplineState.cameraParam.znear = znear;
		piplineState.cameraParam.zfar = zfar;
	}

	_CrossExport void cglLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx, GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy, GLfloat upz)
	{
		VEC3 u, v, n, eye;

		GLfloat matrixParam[16];
		GLfloat tx, ty, tz;

		Vec3Set(&eye, eyex, eyey, eyez);
		Vec3Set(&v, upx, upy, upz);
		Vec3Set(&n, eyex - centerx, eyey - centery, eyez - centerz);
		Vec3Normalize(&n);

		Vec3Cross(&u, &v, &n);
		Vec3Normalize(&u);

		Vec3Cross(&v, &n, &u);
		Vec3Normalize(&v);

		tx = eyex*u[0] + eyey*u[1] + eyez*u[2]; // Vec3Dot(&eye, &u);
		ty = eyex*v[0] + eyey*v[1] + eyez*v[2]; // Vec3Dot(&eye, &v);
		tz = eyex*n[0] + eyey*n[1] + eyez*n[2]; // Vec3Dot(&eye, &n);

		MtxSet((MATRIX4*)matrixParam,
			u[0], v[0], n[0], 0.0f,
			u[1], v[1], n[1], 0.0f,
			u[2], v[2], n[2], 0.0f,
			-tx,  -ty,  -tz,  1.0f);

		cglMultMatrixf(matrixParam);

		piplineState.cameraParam.position[0] = eye[0];
		piplineState.cameraParam.position[1] = eye[1];
		piplineState.cameraParam.position[2] = eye[2];

		piplineState.cameraParam.forward[0] = -n[0];
		piplineState.cameraParam.forward[1] = -n[1];
		piplineState.cameraParam.forward[2] = -n[2];

		piplineState.cameraParam.up[0] = v[0];
		piplineState.cameraParam.up[1] = v[1];
		piplineState.cameraParam.up[2] = v[2];
	}

	_CrossExport GLint cglGetAlphaFunc(void)
	{
		return piplineState.alphaParam.func;
	}

	_CrossExport GLfloat cglGetAlphaRef(void)
	{
		return piplineState.alphaParam.ref;
	}

	_CrossExport GLint cglGetViewportX(void)
	{
		return piplineState.viewportParam.x;
	}

	_CrossExport GLint cglGetViewportY(void)
	{
		return piplineState.viewportParam.y;
	}

	_CrossExport GLint cglGetViewportWidth(void)
	{
		return piplineState.viewportParam.width;
	}

	_CrossExport GLint cglGetViewportHeight(void)
	{
		return piplineState.viewportParam.height;
	}

	_CrossExport GLfloat cglGetCameraFovy(void)
	{
		return piplineState.cameraParam.fovy;
	}

	_CrossExport GLfloat cglGetCameraAspect(void)
	{
		return piplineState.cameraParam.aspect;
	}

	_CrossExport GLfloat cglGetCameraZNear(void)
	{
		return piplineState.cameraParam.znear;
	}

	_CrossExport GLfloat cglGetCameraZFar(void)
	{
		return piplineState.cameraParam.zfar;
	}

	_CrossExport GLfloat* cglGetCameraPosition(void)
	{
		return piplineState.cameraParam.position;
	}

	_CrossExport GLfloat* cglGetCameraForwardDirection(void)
	{
		return piplineState.cameraParam.forward;
	}

	_CrossExport GLfloat* cglGetCameraUpDirection(void)
	{
		return piplineState.cameraParam.up;
	}

	_CrossExport GLfloat* cglGetModelWorldMatrix(void)
	{
		return piplineState.matrixParam.modelWorldMatrix;
	}

	_CrossExport GLfloat* cglGetModelViewMatrix(void)
	{
		return piplineState.matrixParam.modelViewMatrix;
	}

	_CrossExport GLfloat* cglGetModelViewInverseTransposeMatrix(void)
	{
		return piplineState.matrixParam.modelViewInverseTransposeMatrix;
	}

	_CrossExport GLfloat* cglGetModelViewProjectionMatrix(void)
	{
		return piplineState.matrixParam.modelViewProjectionMatrix;
	}

	_CrossExport GLfloat* cglGetTextureMatrix(GLenum texture)
	{
		return piplineState.matrixParam.textureMatrices[texture - GL_TEXTURE0];
	}

	_CrossExport void cglAttachShader(GLuint program, GLuint shader)
	{
		glAttachShader(program, shader);
	}

	_CrossExport void cglBindAttribLocation(GLuint program, GLuint index, const GLchar *name)
	{
		glBindAttribLocation(program, index, name);
	}

	_CrossExport void cglBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.blendParam.red != red || piplineState.blendParam.green != green || piplineState.blendParam.blue != blue || piplineState.blendParam.alpha != alpha) {
				piplineState.blendParam.red = red;
				piplineState.blendParam.green = green;
				piplineState.blendParam.blue = blue;
				piplineState.blendParam.alpha = alpha;
				glBlendColor(red, green, blue, alpha);
			}
		}
		else {
			glBlendColor(red, green, blue, alpha);
		}
	}

	_CrossExport void cglBlendEquation(GLenum mode)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.blendParam.mode != mode) {
				piplineState.blendParam.mode = mode;
				glBlendEquation(mode);
			}
		}
		else {
			glBlendEquation(mode);
		}
	}

	_CrossExport void cglBlendEquationSeparate(GLenum modergb, GLenum modealpha)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.blendParam.modergb != modergb || piplineState.blendParam.modealpha != modealpha) {
				piplineState.blendParam.modergb = modergb;
				piplineState.blendParam.modealpha = modealpha;
				glBlendEquationSeparate(modergb, modealpha);
			}
		}
		else {
			glBlendEquationSeparate(modergb, modealpha);
		}
	}

	_CrossExport void cglBlendFuncSeparate(GLenum srcrgb, GLenum dstrgb, GLenum srcalpha, GLenum dstalpha)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.blendParam.srcrgb != srcrgb || piplineState.blendParam.dstrgb != dstrgb || piplineState.blendParam.srcalpha != srcalpha || piplineState.blendParam.dstalpha != dstalpha) {
				piplineState.blendParam.srcrgb = srcrgb;
				piplineState.blendParam.dstrgb = dstrgb;
				piplineState.blendParam.srcalpha = srcalpha;
				piplineState.blendParam.dstalpha = dstalpha;
				glBlendFuncSeparate(srcrgb, dstrgb, srcalpha, dstalpha);
			}
		}
		else {
			glBlendFuncSeparate(srcrgb, dstrgb, srcalpha, dstalpha);
		}
	}

	_CrossExport void cglCompileShader(GLuint shader)
	{
		glCompileShader(shader);
	}

	_CrossExport GLuint cglCreateProgram(void)
	{
		return glCreateProgram();
	}

	_CrossExport GLuint cglCreateShader(GLenum type)
	{
		return glCreateShader(type);
	}

	_CrossExport void cglDeleteProgram(GLuint program)
	{
		if (piplineState.programParam.program == program) {
			piplineState.programParam.program = GL_INVALID_VALUE;
		}

		glDeleteProgram(program);
	}

	_CrossExport void cglDeleteShader(GLuint shader)
	{
		glDeleteShader(shader);
	}

	_CrossExport void cglDetachShader(GLuint program, GLuint shader)
	{
		glDetachShader(program, shader);
	}

	_CrossExport GLint cglGetVertexAttribs(void)
	{
		return piplineState.vertexAttribStateParam.numVertexAttribs;
	}

	_CrossExport void cglDisableVertexAttribArray(GLuint index)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.vertexAttribStateParam.bEnableVertexAttrib[index] != GL_FALSE) {
				piplineState.vertexAttribStateParam.bEnableVertexAttrib[index] = GL_FALSE;
				glDisableVertexAttribArray(index);
			}
		}
		else {
			glDisableVertexAttribArray(index);
		}
	}

	_CrossExport void cglEnableVertexAttribArray(GLuint index)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.vertexAttribStateParam.bEnableVertexAttrib[index] != GL_TRUE) {
				piplineState.vertexAttribStateParam.bEnableVertexAttrib[index] = GL_TRUE;
				glEnableVertexAttribArray(index);
			}
		}
		else {
			glEnableVertexAttribArray(index);
		}
	}

	_CrossExport void cglGenerateMipmap(GLenum target)
	{
		glGenerateMipmap(target);
	}

	_CrossExport void cglGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
	{
		glGetActiveAttrib(program, index, bufsize, length, size, type, name);
	}

	_CrossExport void cglGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
	{
		glGetActiveUniform(program, index, bufsize, length, size, type, name);
	}

	_CrossExport void cglGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei *count, GLuint *shaders)
	{
		glGetAttachedShaders(program, maxcount, count, shaders);
	}

	_CrossExport GLint cglGetAttribLocation(GLuint program, const GLchar *name)
	{
		return glGetAttribLocation(program, name);
	}

	_CrossExport void cglGetProgramiv(GLuint program, GLenum pname, GLint *params)
	{
		glGetProgramiv(program, pname, params);
	}

	_CrossExport void cglGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei *length, GLchar *infolog)
	{
		glGetProgramInfoLog(program, bufsize, length, infolog);
	}

	_CrossExport void cglGetShaderiv(GLuint shader, GLenum pname, GLint *params)
	{
		glGetShaderiv(shader, pname, params);
	}

	_CrossExport void cglGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei *length, GLchar *infolog)
	{
		glGetShaderInfoLog(shader, bufsize, length, infolog);
	}

	_CrossExport void cglGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision)
	{
		glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
	}

	_CrossExport void cglGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei *length, GLchar *source)
	{
		glGetShaderSource(shader, bufsize, length, source);
	}

	_CrossExport void cglGetUniformfv(GLuint program, GLint location, GLfloat *params)
	{
		glGetUniformfv(program, location, params);
	}

	_CrossExport void cglGetUniformiv(GLuint program, GLint location, GLint *params)
	{
		glGetUniformiv(program, location, params);
	}

	_CrossExport GLint cglGetUniformLocation(GLuint program, const GLchar *name)
	{
		return glGetUniformLocation(program, name);
	}

	_CrossExport void cglGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params)
	{
		glGetVertexAttribfv(index, pname, params);
	}

	_CrossExport void cglGetVertexAttribiv(GLuint index, GLenum pname, GLint *params)
	{
		glGetVertexAttribiv(index, pname, params);
	}

	_CrossExport void cglGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer)
	{
		glGetVertexAttribPointerv(index, pname, pointer);
	}

	_CrossExport GLboolean cglIsProgram(GLuint program)
	{
		return glIsProgram(program);
	}

	_CrossExport GLboolean cglIsShader(GLuint shader)
	{
		return glIsShader(shader);
	}

	_CrossExport void cglLinkProgram(GLuint program)
	{
		glLinkProgram(program);
	}

	_CrossExport void cglReleaseShaderCompiler(void)
	{
		glReleaseShaderCompiler();
	}

	_CrossExport void cglShaderBinary(GLsizei n, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length)
	{
		glShaderBinary(n, shaders, binaryformat, binary, length);
	}

	_CrossExport void cglShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
	{
		glShaderSource(shader, count, string, length);
	}

	_CrossExport void cglStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
	{
		glStencilFuncSeparate(face, func, ref, mask);
	}

	_CrossExport void cglStencilMaskSeparate(GLenum face, GLuint mask)
	{
		glStencilMaskSeparate(face, mask);
	}

	_CrossExport void cglStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
	{
		glStencilOpSeparate(face, fail, zfail, zpass);
	}

	_CrossExport void cglUniform1f(GLint location, GLfloat x, GLfloat *cx)
	{
		if (cx) {
			if (*cx != x) {
				*cx = x;
				glUniform1f(location, x);
			}
		}
		else {
			glUniform1f(location, x);
		}
	}

	_CrossExport void cglUniform1fv(GLint location, GLsizei count, const GLfloat *v, GLfloat *cv)
	{
		if (cv) {
			if (memcmp(cv, v, sizeof(*v) * 1 * count)) {
				memcpy(cv, v, sizeof(*v) * 1 * count);
				glUniform1fv(location, count, v);
			}
		}
		else {
			glUniform1fv(location, count, v);
		}
	}

	_CrossExport void cglUniform1i(GLint location, GLint x, GLint *cx)
	{
		if (cx) {
			if (*cx != x) {
				*cx = x;
				glUniform1i(location, x);
			}
		}
		else {
			glUniform1i(location, x);
		}
	}

	_CrossExport void cglUniform1iv(GLint location, GLsizei count, const GLint *v, GLint *cv)
	{
		if (cv) {
			if (memcmp(cv, v, sizeof(*v) * 1 * count)) {
				memcpy(cv, v, sizeof(*v) * 1 * count);
				glUniform1iv(location, count, v);
			}
		}
		else {
			glUniform1iv(location, count, v);
		}
	}

	_CrossExport void cglUniform2f(GLint location, GLfloat x, GLfloat y, GLfloat *cx, GLfloat *cy)
	{
		if (cx && cy) {
			if (*cx != x || *cy != y) {
				*cx = x;
				*cy = y;
				glUniform2f(location, x, y);
			}
		}
		else {
			glUniform2f(location, x, y);
		}
	}

	_CrossExport void cglUniform2fv(GLint location, GLsizei count, const GLfloat *v, GLfloat *cv)
	{
		if (cv) {
			if (memcmp(cv, v, sizeof(*v) * 2 * count)) {
				memcpy(cv, v, sizeof(*v) * 2 * count);
				glUniform2fv(location, count, v);
			}
		}
		else {
			glUniform2fv(location, count, v);
		}
	}

	_CrossExport void cglUniform2i(GLint location, GLint x, GLint y, GLint *cx, GLint *cy)
	{
		if (cx && cy) {
			if (*cx != x || *cy != y) {
				*cx = x;
				*cy = y;
				glUniform2i(location, x, y);
			}
		}
		else {
			glUniform2i(location, x, y);
		}
	}

	_CrossExport void cglUniform2iv(GLint location, GLsizei count, const GLint *v, GLint *cv)
	{
		if (cv) {
			if (memcmp(cv, v, sizeof(*v) * 2 * count)) {
				memcpy(cv, v, sizeof(*v) * 2 * count);
				glUniform2iv(location, count, v);
			}
		}
		else {
			glUniform2iv(location, count, v);
		}
	}

	_CrossExport void cglUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat *cx, GLfloat *cy, GLfloat *cz)
	{
		if (cx && cy && cz) {
			if (*cx != x || *cy != y || *cz != z) {
				*cx = x;
				*cy = y;
				*cz = z;
				glUniform3f(location, x, y, z);
			}
		}
		else {
			glUniform3f(location, x, y, z);
		}
	}

	_CrossExport void cglUniform3fv(GLint location, GLsizei count, const GLfloat *v, GLfloat *cv)
	{
		if (cv) {
			if (memcmp(cv, v, sizeof(*v) * 3 * count)) {
				memcpy(cv, v, sizeof(*v) * 3 * count);
				glUniform3fv(location, count, v);
			}
		}
		else {
			glUniform3fv(location, count, v);
		}
	}

	_CrossExport void cglUniform3i(GLint location, GLint x, GLint y, GLint z, GLint *cx, GLint *cy, GLint *cz)
	{
		if (cx && cy && cz) {
			if (*cx != x || *cy != y || *cz != z) {
				*cx = x;
				*cy = y;
				*cz = z;
				glUniform3i(location, x, y, z);
			}
		}
		else {
			glUniform3i(location, x, y, z);
		}
	}

	_CrossExport void cglUniform3iv(GLint location, GLsizei count, const GLint *v, GLint *cv)
	{
		if (cv) {
			if (memcmp(cv, v, sizeof(*v) * 3 * count)) {
				memcpy(cv, v, sizeof(*v) * 3 * count);
				glUniform3iv(location, count, v);
			}
		}
		else {
			glUniform3iv(location, count, v);
		}
	}

	_CrossExport void cglUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat *cx, GLfloat *cy, GLfloat *cz, GLfloat *cw)
	{
		if (cx && cy && cz && cw) {
			if (*cx != x || *cy != y || *cz != z || *cw != w) {
				*cx = x;
				*cy = y;
				*cz = z;
				*cw = w;
				glUniform4f(location, x, y, z, w);
			}
		}
		else {
			glUniform4f(location, x, y, z, w);
		}
	}

	_CrossExport void cglUniform4fv(GLint location, GLsizei count, const GLfloat *v, GLfloat *cv)
	{
		if (cv) {
			if (memcmp(cv, v, sizeof(*v) * 4 * count)) {
				memcpy(cv, v, sizeof(*v) * 4 * count);
				glUniform4fv(location, count, v);
			}
		}
		else {
			glUniform4fv(location, count, v);
		}
	}

	_CrossExport void cglUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w, GLint *cx, GLint *cy, GLint *cz, GLint *cw)
	{
		if (cx && cy && cz && cw) {
			if (*cx != x || *cy != y || *cz != z || *cw != w) {
				*cx = x;
				*cy = y;
				*cz = z;
				*cw = w;
				glUniform4i(location, x, y, z, w);
			}
		}
		else {
			glUniform4i(location, x, y, z, w);
		}
	}

	_CrossExport void cglUniform4iv(GLint location, GLsizei count, const GLint *v, GLint *cv)
	{
		if (cv) {
			if (memcmp(cv, v, sizeof(*v) * 4 * count)) {
				memcpy(cv, v, sizeof(*v) * 4 * count);
				glUniform4iv(location, count, v);
			}
		}
		else {
			glUniform4iv(location, count, v);
		}
	}

	_CrossExport void cglUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *v, GLfloat *cv)
	{
		if (cv) {
			if (memcmp(cv, v, sizeof(*v) * 2 * 2 * count)) {
				memcpy(cv, v, sizeof(*v) * 2 * 2 * count);
				glUniformMatrix2fv(location, count, transpose, v);
			}
		}
		else {
			glUniformMatrix2fv(location, count, transpose, v);
		}
	}

	_CrossExport void cglUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *v, GLfloat *cv)
	{
		if (cv) {
			if (memcmp(cv, v, sizeof(*v) * 3 * 3 * count)) {
				memcpy(cv, v, sizeof(*v) * 3 * 3 * count);
				glUniformMatrix3fv(location, count, transpose, v);
			}
		}
		else {
			glUniformMatrix3fv(location, count, transpose, v);
		}
	}

	_CrossExport void cglUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *v, GLfloat *cv)
	{
		if (cv) {
			if (memcmp(cv, v, sizeof(*v) * 4 * 4 * count)) {
				memcpy(cv, v, sizeof(*v) * 4 * 4 * count);
				glUniformMatrix4fv(location, count, transpose, v);
			}
		}
		else {
			glUniformMatrix4fv(location, count, transpose, v);
		}
	}

	_CrossExport void cglUseProgram(GLuint program)
	{
		if (piplineState.bEnableStateCache == GL_TRUE) {
			if (piplineState.programParam.program != program) {
				piplineState.programParam.program = program;
				glUseProgram(program);
			}
		}
		else {
			glUseProgram(program);
		}
	}

	_CrossExport void cglValidateProgram(GLuint program)
	{
		glValidateProgram(program);
	}

	_CrossExport void cglVertexAttrib1f(GLuint index, GLfloat x)
	{
		glVertexAttrib1f(index, x);
	}

	_CrossExport void cglVertexAttrib1fv(GLuint index, const GLfloat *values)
	{
		glVertexAttrib1fv(index, values);
	}

	_CrossExport void cglVertexAttrib2f(GLuint index, GLfloat x, GLfloat y)
	{
		glVertexAttrib2f(index, x, y);
	}

	_CrossExport void cglVertexAttrib2fv(GLuint index, const GLfloat *values)
	{
		glVertexAttrib2fv(index, values);
	}

	_CrossExport void cglVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z)
	{
		glVertexAttrib3f(index, x, y, z);
	}

	_CrossExport void cglVertexAttrib3fv(GLuint index, const GLfloat *values)
	{
		glVertexAttrib3fv(index, values);
	}

	_CrossExport void cglVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		glVertexAttrib4f(index, x, y, z, w);
	}

	_CrossExport void cglVertexAttrib4fv(GLuint index, const GLfloat *values)
	{
		glVertexAttrib4fv(index, values);
	}

	_CrossExport void cglVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *ptr)
	{
		glVertexAttribPointer(index, size, type, normalized, stride, ptr);
	}

	_CrossExport void cglDrawBuffers(GLsizei n, const GLenum *bufs)
	{
		if (esVersion == GLES_VERSION_30) {
			glDrawBuffers(n, bufs);
		}
	}

	_CrossExport void cglReadBuffer(GLenum mode)
	{
		if (esVersion == GLES_VERSION_30) {
			glReadBuffer(mode);
		}
	}

	_CrossExport void cglGenQueries(GLsizei n, GLuint* ids)
	{
		if (esVersion == GLES_VERSION_30) {
			glGenQueries(n, ids);
		}
	}

	_CrossExport void cglDeleteQueries(GLsizei n, const GLuint* ids)
	{
		if (esVersion == GLES_VERSION_30) {
			glDeleteQueries(n, ids);
		}
	}

	_CrossExport void cglBeginQuery(GLenum target, GLuint id)
	{
		if (esVersion == GLES_VERSION_30) {
			glBeginQuery(target, id);
		}
	}

	_CrossExport void cglEndQuery(GLenum target)
	{
		if (esVersion == GLES_VERSION_30) {
			glEndQuery(target);
		}
	}

	_CrossExport void cglGetQueryiv(GLenum target, GLenum pname, GLint* params)
	{
		if (esVersion == GLES_VERSION_30) {
			glGetQueryiv(target, pname, params);
		}
	}

	_CrossExport void cglGetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params)
	{
		if (esVersion == GLES_VERSION_30) {
			glGetQueryObjectuiv(id, pname, params);
		}
		else {
			*params = INT_MAX;
		}
	}

	_CrossExport GLboolean cglIsQuery(GLuint id)
	{
		if (esVersion == GLES_VERSION_30) {
			return glIsQuery(id);
		}

		return GL_FALSE;
	}

	_CrossExport void cglBegin(GLenum mode)
	{
#ifdef _PLATFORM_WINDOWS_

		glBegin(mode);

#endif
	}

	_CrossExport void cglEnd(void)
	{
#ifdef _PLATFORM_WINDOWS_

		glEnd();

#endif
	}

	_CrossExport void cglColor3b(GLbyte red, GLbyte green, GLbyte blue)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3b(red, green, blue);

#endif
	}

	_CrossExport void cglColor3bv(const GLbyte *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3bv(v);

#endif
	}

	_CrossExport void cglColor3d(GLdouble red, GLdouble green, GLdouble blue)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3d(red, green, blue);

#endif
	}

	_CrossExport void cglColor3dv(const GLdouble *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3dv(v);

#endif
	}

	_CrossExport void cglColor3f(GLfloat red, GLfloat green, GLfloat blue)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3f(red, green, blue);

#endif
	}

	_CrossExport void cglColor3fv(const GLfloat *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3fv(v);

#endif
	}

	_CrossExport void cglColor3i(GLint red, GLint green, GLint blue)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3i(red, green, blue);

#endif
	}

	_CrossExport void cglColor3iv(const GLint *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3iv(v);

#endif
	}

	_CrossExport void cglColor3s(GLshort red, GLshort green, GLshort blue)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3s(red, green, blue);

#endif
	}

	_CrossExport void cglColor3sv(const GLshort *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3sv(v);

#endif
	}

	_CrossExport void cglColor3ub(GLubyte red, GLubyte green, GLubyte blue)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3ub(red, green, blue);

#endif
	}

	_CrossExport void cglColor3ubv(const GLubyte *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3ubv(v);

#endif
	}

	_CrossExport void cglColor3ui(GLuint red, GLuint green, GLuint blue)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3ui(red, green, blue);

#endif
	}

	_CrossExport void cglColor3uiv(const GLuint *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3uiv(v);

#endif
	}

	_CrossExport void cglColor3us(GLushort red, GLushort green, GLushort blue)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3us(red, green, blue);

#endif
	}

	_CrossExport void cglColor3usv(const GLushort *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor3usv(v);

#endif
	}

	_CrossExport void cglColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4b(red, green, blue, alpha);

#endif
	}

	_CrossExport void cglColor4bv(const GLbyte *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4bv(v);

#endif
	}

	_CrossExport void cglColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4d(red, green, blue, alpha);

#endif
	}

	_CrossExport void cglColor4dv(const GLdouble *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4dv(v);

#endif
	}

	_CrossExport void cglColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4f(red, green, blue, alpha);

#endif
	}

	_CrossExport void cglColor4fv(const GLfloat *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4fv(v);

#endif
	}

	_CrossExport void cglColor4i(GLint red, GLint green, GLint blue, GLint alpha)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4i(red, green, blue, alpha);

#endif
	}

	_CrossExport void cglColor4iv(const GLint *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4iv(v);

#endif
	}

	_CrossExport void cglColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4s(red, green, blue, alpha);

#endif
	}

	_CrossExport void cglColor4sv(const GLshort *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4sv(v);

#endif
	}

	_CrossExport void cglColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4ub(red, green, blue, alpha);

#endif
	}

	_CrossExport void cglColor4ubv(const GLubyte *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4ubv(v);

#endif
	}

	_CrossExport void cglColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4ui(red, green, blue, alpha);

#endif
	}

	_CrossExport void cglColor4uiv(const GLuint *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4uiv(v);

#endif
	}

	_CrossExport void cglColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4us(red, green, blue, alpha);

#endif
	}

	_CrossExport void cglColor4usv(const GLushort *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glColor4usv(v);

#endif
	}

	_CrossExport void cglNormal3b(GLbyte nx, GLbyte ny, GLbyte nz)
	{
#ifdef _PLATFORM_WINDOWS_

		glNormal3b(nx, ny, nz);

#endif
	}

	_CrossExport void cglNormal3bv(const GLbyte *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glNormal3bv(v);

#endif
	}

	_CrossExport void cglNormal3d(GLdouble nx, GLdouble ny, GLdouble nz)
	{
#ifdef _PLATFORM_WINDOWS_

		glNormal3d(nx, ny, nz);

#endif
	}

	_CrossExport void cglNormal3dv(const GLdouble *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glNormal3dv(v);

#endif
	}

	_CrossExport void cglNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
	{
#ifdef _PLATFORM_WINDOWS_

		glNormal3f(nx, ny, nz);

#endif
	}

	_CrossExport void cglNormal3fv(const GLfloat *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glNormal3fv(v);

#endif
	}

	_CrossExport void cglNormal3i(GLint nx, GLint ny, GLint nz)
	{
#ifdef _PLATFORM_WINDOWS_

		glNormal3i(nx, ny, nz);

#endif
	}

	_CrossExport void cglNormal3iv(const GLint *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glNormal3iv(v);

#endif
	}

	_CrossExport void cglNormal3s(GLshort nx, GLshort ny, GLshort nz)
	{
#ifdef _PLATFORM_WINDOWS_

		glNormal3s(nx, ny, nz);

#endif
	}

	_CrossExport void cglNormal3sv(const GLshort *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glNormal3sv(v);

#endif
	}

	_CrossExport void cglVertex2d(GLdouble x, GLdouble y)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex2d(x, y);

#endif
	}

	_CrossExport void cglVertex2dv(const GLdouble *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex2dv(v);

#endif
	}

	_CrossExport void cglVertex2f(GLfloat x, GLfloat y)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex2f(x, y);

#endif
	}

	_CrossExport void cglVertex2fv(const GLfloat *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex2fv(v);

#endif
	}

	_CrossExport void cglVertex2i(GLint x, GLint y)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex2i(x, y);

#endif
	}

	_CrossExport void cglVertex2iv(const GLint *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex2iv(v);

#endif
	}

	_CrossExport void cglVertex2s(GLshort x, GLshort y)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex2s(x, y);

#endif
	}

	_CrossExport void cglVertex2sv(const GLshort *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex2sv(v);

#endif
	}

	_CrossExport void cglVertex3d(GLdouble x, GLdouble y, GLdouble z)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex3d(x, y, z);

#endif
	}

	_CrossExport void cglVertex3dv(const GLdouble *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex3dv(v);

#endif
	}

	_CrossExport void cglVertex3f(GLfloat x, GLfloat y, GLfloat z)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex3f(x, y, z);

#endif
	}

	_CrossExport void cglVertex3fv(const GLfloat *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex3fv(v);

#endif
	}

	_CrossExport void cglVertex3i(GLint x, GLint y, GLint z)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex3i(x, y, z);

#endif
	}

	_CrossExport void cglVertex3iv(const GLint *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex3iv(v);

#endif
	}

	_CrossExport void cglVertex3s(GLshort x, GLshort y, GLshort z)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex3s(x, y, z);

#endif
	}

	_CrossExport void cglVertex3sv(const GLshort *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex3sv(v);

#endif
	}

	_CrossExport void cglVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex4d(x, y, z, w);

#endif
	}

	_CrossExport void cglVertex4dv(const GLdouble *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex4dv(v);

#endif
	}

	_CrossExport void cglVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex4f(x, y, z, w);

#endif
	}

	_CrossExport void cglVertex4fv(const GLfloat *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex4fv(v);

#endif
	}

	_CrossExport void cglVertex4i(GLint x, GLint y, GLint z, GLint w)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex4i(x, y, z, w);

#endif
	}

	_CrossExport void cglVertex4iv(const GLint *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex4iv(v);

#endif
	}

	_CrossExport void cglVertex4s(GLshort x, GLshort y, GLshort z, GLshort w)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex4s(x, y, z, w);

#endif
	}

	_CrossExport void cglVertex4sv(const GLshort *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glVertex4sv(v);

#endif
	}

	_CrossExport void cglTexCoord1d(GLdouble s)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord1d(s);

#endif
	}

	_CrossExport void cglTexCoord1dv(const GLdouble *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord1dv(v);

#endif
	}

	_CrossExport void cglTexCoord1f(GLfloat s)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord1f(s);

#endif
	}

	_CrossExport void cglTexCoord1fv(const GLfloat *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord1fv(v);

#endif
	}

	_CrossExport void cglTexCoord1i(GLint s)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord1i(s);

#endif
	}

	_CrossExport void cglTexCoord1iv(const GLint *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord1iv(v);

#endif
	}

	_CrossExport void cglTexCoord1s(GLshort s)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord1s(s);

#endif
	}

	_CrossExport void cglTexCoord1sv(const GLshort *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord1sv(v);

#endif
	}

	_CrossExport void cglTexCoord2d(GLdouble s, GLdouble t)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord2d(s, t);

#endif
	}

	_CrossExport void cglTexCoord2dv(const GLdouble *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord2dv(v);

#endif
	}

	_CrossExport void cglTexCoord2f(GLfloat s, GLfloat t)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord2f(s, t);

#endif
	}

	_CrossExport void cglTexCoord2fv(const GLfloat *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord2fv(v);

#endif
	}

	_CrossExport void cglTexCoord2i(GLint s, GLint t)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord2i(s, t);

#endif
	}

	_CrossExport void cglTexCoord2iv(const GLint *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord2iv(v);

#endif
	}

	_CrossExport void cglTexCoord2s(GLshort s, GLshort t)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord2s(s, t);

#endif
	}

	_CrossExport void cglTexCoord2sv(const GLshort *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord2sv(v);

#endif
	}

	_CrossExport void cglTexCoord3d(GLdouble s, GLdouble t, GLdouble r)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord3d(s, t, r);

#endif
	}

	_CrossExport void cglTexCoord3dv(const GLdouble *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord3dv(v);

#endif
	}

	_CrossExport void cglTexCoord3f(GLfloat s, GLfloat t, GLfloat r)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord3f(s, t, r);

#endif
	}

	_CrossExport void cglTexCoord3fv(const GLfloat *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord3fv(v);

#endif
	}

	_CrossExport void cglTexCoord3i(GLint s, GLint t, GLint r)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord3i(s, t, r);

#endif
	}

	_CrossExport void cglTexCoord3iv(const GLint *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord3iv(v);

#endif
	}

	_CrossExport void cglTexCoord3s(GLshort s, GLshort t, GLshort r)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord3s(s, t, r);

#endif
	}

	_CrossExport void cglTexCoord3sv(const GLshort *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord3sv(v);

#endif
	}

	_CrossExport void cglTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord4d(s, t, r, q);

#endif
	}

	_CrossExport void cglTexCoord4dv(const GLdouble *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord4dv(v);

#endif
	}

	_CrossExport void cglTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord4f(s, t, r, q);

#endif
	}

	_CrossExport void cglTexCoord4fv(const GLfloat *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord4fv(v);

#endif
	}

	_CrossExport void cglTexCoord4i(GLint s, GLint t, GLint r, GLint q)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord4i(s, t, r, q);

#endif
	}

	_CrossExport void cglTexCoord4iv(const GLint *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord4iv(v);

#endif
	}

	_CrossExport void cglTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord4s(s, t, r, q);

#endif
	}

	_CrossExport void cglTexCoord4sv(const GLshort *v)
	{
#ifdef _PLATFORM_WINDOWS_

		glTexCoord4sv(v);

#endif
	}

	_CrossExport void cglDrawBuffer(GLenum mode)
	{
#ifdef _PLATFORM_WINDOWS_

		glDrawBuffer(mode);

#endif
	}

	_CrossExport void cglGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, void *pixels)
	{
#ifdef _PLATFORM_WINDOWS_

		glGetTexImage(target, level, format, type, pixels);

#endif
	}

	_CrossExport void cglLineStipple(GLint factor, GLushort pattern)
	{
#ifdef _PLATFORM_WINDOWS_

		glLineStipple(factor, pattern);

#endif
	}

}

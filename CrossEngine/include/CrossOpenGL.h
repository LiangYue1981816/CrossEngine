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

	#ifndef GL_MIN
	#define GL_MIN                               0x8007
	#endif

	#ifndef GL_MAX
	#define GL_MAX                               0x8008
	#endif

	#ifndef GL_BGRA
	#define GL_BGRA                              0x80E1
	#endif

	#ifndef GL_RGB565
	#define GL_RGB565                            0x8D62
	#endif

	#ifndef GL_STENCIL_INDEX
	#define GL_STENCIL_INDEX                     0x1901
	#endif

	#ifndef GL_DEPTH24_STENCIL8
	#define GL_DEPTH24_STENCIL8                  0x88F0
	#endif

	#ifndef GL_DEPTH_COMPONENT16
	#define GL_DEPTH_COMPONENT16                 0x81A5
	#endif

	#ifndef GL_DEPTH_COMPONENT24
	#define GL_DEPTH_COMPONENT24                 0x81A6
	#endif

	#ifndef GL_POINT_SPRITE
	#define GL_POINT_SPRITE                      0x8861
	#endif

	#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
	#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT      0x83F0
	#endif

	#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
	#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT     0x83F1
	#endif

	#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
	#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT     0x83F2
	#endif

	#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
	#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT     0x83F3
	#endif

	#ifndef GL_TEXTURE_COMPARE_MODE
	#define GL_TEXTURE_COMPARE_MODE              0x884C
	#endif

	#ifndef GL_TEXTURE_COMPARE_FUNC
	#define GL_TEXTURE_COMPARE_FUNC              0x884D
	#endif

	#ifndef GL_COMPARE_REF_TO_TEXTURE
	#define GL_COMPARE_REF_TO_TEXTURE            0x884E
	#endif

	#define GL_CROSS_BASE                        0xffff
	#define GL_MODELWORLD                        GL_CROSS_BASE + 0x0001


	#ifndef _PLATFORM_WINDOWS_

		typedef double GLdouble;

		#define glBindVertexArray                glBindVertexArrayOES
		#define glDeleteVertexArrays             glDeleteVertexArraysOES
		#define glGenVertexArrays                glGenVertexArraysOES
		#define glIsVertexArray                  glIsVertexArrayOES

	#endif


	#define MAX_BONES 55
	#define MAX_TEXUNITS 8
	#define MAX_DRAW_BUFFERS 4


	_CrossExport void InitGL(void);
	_CrossExport void ExitGL(void);
	_CrossExport void ResetGL(void);

	_CrossExport GLboolean IsOpenGLES3(void);
	_CrossExport GLboolean IsSupportExtensionGL(const GLchar *feature);

	_CrossExport const GLchar* cglAttribToString(GLenum attrib);
	_CrossExport GLenum cglStringToAttrib(const GLchar *str);

	_CrossExport void cglDisableClientState(GLenum array);
	_CrossExport void cglEnableClientState(GLenum array);
	_CrossExport void cglColorPointer(GLint size, GLenum type, GLsizei stride, const void *pointer);
	_CrossExport void cglTexCoordPointer(GLint size, GLenum type, GLsizei stride, const void *pointer);
	_CrossExport void cglVertexPointer(GLint size, GLenum type, GLsizei stride, const void *pointer);

	_CrossExport void cglActiveTexture(GLenum texture);
	_CrossExport void cglBindBuffer(GLenum target, GLuint buffer);
	_CrossExport void cglBindFramebuffer(GLenum target, GLuint framebuffer);
	_CrossExport void cglBindRenderbuffer(GLenum target, GLuint renderbuffer);
	_CrossExport void cglBindTexture(GLenum target, GLuint texture);
	_CrossExport void cglBindVertexArray(GLuint array);
	_CrossExport void cglBlendFunc(GLenum sfactor, GLenum dfactor);
	_CrossExport void cglBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
	_CrossExport void cglBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
	_CrossExport GLenum cglCheckFramebufferStatus(GLenum target);
	_CrossExport void cglClear(GLbitfield mask);
	_CrossExport void cglClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	_CrossExport void cglClearDepth(GLclampf depth);
	_CrossExport void cglClearStencil(GLint stencil);
	_CrossExport void cglColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
	_CrossExport void cglCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
	_CrossExport void cglCompressedTexImage2DMipmapsDDS(GLenum target, GLint maxresolution, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLuint bitcount, const void *pixels);
	_CrossExport void cglCompressedTexImage2DMipmapsETC(GLenum target, GLint maxresolution, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLuint bitcount, const void *pixels);
	_CrossExport void cglCompressedTexImage2DMipmapsPVR(GLenum target, GLint maxresolution, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLuint bitcount, const void *pixels);
	_CrossExport void cglCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
	_CrossExport void cglCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
	_CrossExport void cglCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	_CrossExport void cglCullFace(GLenum mode);
	_CrossExport void cglDeleteBuffers(GLsizei n, const GLuint *buffers);
	_CrossExport void cglDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
	_CrossExport void cglDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
	_CrossExport void cglDeleteTextures(GLsizei n, const GLuint *textures);
	_CrossExport void cglDeleteVertexArrays(GLsizei n, const GLuint *arrays);
	_CrossExport void cglDepthFunc(GLenum func);
	_CrossExport void cglDepthMask(GLboolean mask);
	_CrossExport void cglDepthRange(GLclampf znear, GLclampf zfar);
	_CrossExport void cglDisable(GLenum cap);
	_CrossExport void cglDrawArrays(GLenum mode, GLint first, GLsizei count);
	_CrossExport void cglDrawArraysCompatible(GLenum mode, GLint first, GLsizei count);
	_CrossExport void cglDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices);
	_CrossExport void cglDrawElementsCompatible(GLenum mode, GLsizei count, GLenum type, const void *indices);
	_CrossExport void cglEnable(GLenum cap);
	_CrossExport void cglFinish(void);
	_CrossExport void cglFlush(void);
	_CrossExport void cglFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	_CrossExport void cglFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	_CrossExport void cglFrontFace(GLenum mode);
	_CrossExport void cglGenBuffers(GLsizei n, GLuint *buffers);
	_CrossExport void cglGenFramebuffers(GLsizei n, GLuint *framebuffers);
	_CrossExport void cglGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
	_CrossExport void cglGenTextures(GLsizei n, GLuint *textures);
	_CrossExport void cglGenVertexArrays(GLsizei n, GLuint *arrays);
	_CrossExport void cglGetBooleanv(GLenum pname, GLboolean *params);
	_CrossExport void cglGetBufferParameteriv(GLenum target, GLenum pname, GLint *params);
	_CrossExport GLenum cglGetError(void);
	_CrossExport void cglGetFloatv(GLenum pname, GLfloat *params);
	_CrossExport void cglGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params);
	_CrossExport void cglGetIntegerv(GLenum pname, GLint *params);
	_CrossExport void cglGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params);
	_CrossExport const GLchar* cglGetString(GLenum name);
	_CrossExport void cglGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params);
	_CrossExport void cglGetTexParameteriv(GLenum target, GLenum pname, GLint *params);
	_CrossExport void cglHint(GLenum target, GLenum mode);
	_CrossExport GLboolean cglIsBuffer(GLuint buffer);
	_CrossExport GLboolean cglIsEnabled(GLenum cap);
	_CrossExport GLboolean cglIsFramebuffer(GLuint framebuffer);
	_CrossExport GLboolean cglIsRenderbuffer(GLuint renderbuffer);
	_CrossExport GLboolean cglIsTexture(GLuint texture);
	_CrossExport GLboolean cglIsVertexArray(GLuint array);
	_CrossExport void cglLineWidth(GLfloat width);
	_CrossExport void cglPixelStorei(GLenum pname, GLint param);
	_CrossExport void cglPolygonOffset(GLfloat factor, GLfloat units);
	_CrossExport void cglReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
	_CrossExport void cglRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	_CrossExport void cglSampleCoverage(GLclampf value, GLboolean invert);
	_CrossExport void cglScissor(GLint x, GLint y, GLsizei width, GLsizei height);
	_CrossExport void cglStencilFunc(GLenum func, GLint ref, GLuint mask);
	_CrossExport void cglStencilMask(GLuint mask);
	_CrossExport void cglStencilOp(GLenum sfail, GLenum zfail, GLenum zpass);
	_CrossExport void cglTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
	_CrossExport void cglTexImage2DMipmaps(GLenum target, GLint maxresolution, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, GLuint bitcount, const void *pixels);
	_CrossExport void cglTexParameterf(GLenum target, GLenum pname, GLfloat param);
	_CrossExport void cglTexParameterfv(GLenum target, GLenum pname, const GLfloat *params);
	_CrossExport void cglTexParameteri(GLenum target, GLenum pname, GLint param);
	_CrossExport void cglTexParameteriv(GLenum target, GLenum pname, const GLint *params);
	_CrossExport void cglTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
	_CrossExport void cglViewport(GLint x, GLint y, GLsizei width, GLsizei height);

	_CrossExport void cglAlphaFunc(GLenum func, GLclampf ref);
	_CrossExport void cglMatrixMode(GLenum mode);
	_CrossExport void cglPopMatrix(void);
	_CrossExport void cglPushMatrix(void);
	_CrossExport void cglMultMatrixf(const GLfloat *m);
	_CrossExport void cglLoadMatrixf(const GLfloat *m);
	_CrossExport void cglLoadIdentity(void);
	_CrossExport void cglScalef(GLfloat x, GLfloat y, GLfloat z);
	_CrossExport void cglRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	_CrossExport void cglTranslatef(GLfloat x, GLfloat y, GLfloat z);
	_CrossExport void cglOrtho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar);
	_CrossExport void cglFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar);
	_CrossExport void cglPerspective(GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar);
	_CrossExport void cglLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx, GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy, GLfloat upz);
	_CrossExport GLint cglGetAlphaFunc(void);
	_CrossExport GLfloat cglGetAlphaRef(void);
	_CrossExport GLint cglGetViewportX(void);
	_CrossExport GLint cglGetViewportY(void);
	_CrossExport GLint cglGetViewportWidth(void);
	_CrossExport GLint cglGetViewportHeight(void);
	_CrossExport GLfloat cglGetCameraFovy(void);
	_CrossExport GLfloat cglGetCameraAspect(void);
	_CrossExport GLfloat cglGetCameraZNear(void);
	_CrossExport GLfloat cglGetCameraZFar(void);
	_CrossExport GLfloat* cglGetCameraPosition(void);
	_CrossExport GLfloat* cglGetCameraForwardDirection(void);
	_CrossExport GLfloat* cglGetCameraUpDirection(void);
	_CrossExport GLfloat* cglGetModelWorldMatrix(void);
	_CrossExport GLfloat* cglGetModelViewMatrix(void);
	_CrossExport GLfloat* cglGetModelViewInverseTransposeMatrix(void);
	_CrossExport GLfloat* cglGetModelViewProjectionMatrix(void);
	_CrossExport GLfloat* cglGetTextureMatrix(GLenum texture);

	_CrossExport void cglAttachShader(GLuint program, GLuint shader);
	_CrossExport void cglBindAttribLocation(GLuint program, GLuint index, const GLchar *name);
	_CrossExport void cglBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	_CrossExport void cglBlendEquation(GLenum mode);
	_CrossExport void cglBlendEquationSeparate(GLenum modergb, GLenum modealpha);
	_CrossExport void cglBlendFuncSeparate(GLenum srcrgb, GLenum dstrgb, GLenum srcalpha, GLenum dstalpha);
	_CrossExport void cglCompileShader(GLuint shader);
	_CrossExport GLuint cglCreateProgram(void);
	_CrossExport GLuint cglCreateShader(GLenum type);
	_CrossExport void cglDeleteProgram(GLuint program);
	_CrossExport void cglDeleteShader(GLuint shader);
	_CrossExport void cglDetachShader(GLuint program, GLuint shader);
	_CrossExport void cglDisableVertexAttribArray(GLuint index);
	_CrossExport void cglEnableVertexAttribArray(GLuint index);
	_CrossExport void cglGenerateMipmap(GLenum target);
	_CrossExport void cglGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
	_CrossExport void cglGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
	_CrossExport void cglGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei *count, GLuint *shaders);
	_CrossExport GLint cglGetAttribLocation(GLuint program, const GLchar *name);
	_CrossExport void cglGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei *length, GLchar *infolog);
	_CrossExport void cglGetProgramiv(GLuint program, GLenum pname, GLint *params);
	_CrossExport void cglGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei *length, GLchar *infolog);
	_CrossExport void cglGetShaderiv(GLuint shader, GLenum pname, GLint *params);
	_CrossExport void cglGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
	_CrossExport void cglGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei *length, GLchar *source);
	_CrossExport void cglGetUniformfv(GLuint program, GLint location, GLfloat *params);
	_CrossExport void cglGetUniformiv(GLuint program, GLint location, GLint *params);
	_CrossExport GLint cglGetUniformLocation(GLuint program, const GLchar *name);
	_CrossExport void cglGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params);
	_CrossExport void cglGetVertexAttribiv(GLuint index, GLenum pname, GLint *params);
	_CrossExport void cglGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer);
	_CrossExport GLint cglGetVertexAttribs(void);
	_CrossExport GLboolean cglIsProgram(GLuint program);
	_CrossExport GLboolean cglIsShader(GLuint shader);
	_CrossExport void cglLinkProgram(GLuint program);
	_CrossExport void cglReleaseShaderCompiler(void);
	_CrossExport void cglShaderBinary(GLsizei n, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
	_CrossExport void cglShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
	_CrossExport void cglStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
	_CrossExport void cglStencilMaskSeparate(GLenum face, GLuint mask);
	_CrossExport void cglStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
	_CrossExport void cglUniform1f(GLint location, GLfloat x, GLfloat *cx = NULL);
	_CrossExport void cglUniform1fv(GLint location, GLsizei count, const GLfloat *v, GLfloat *cv = NULL);
	_CrossExport void cglUniform1i(GLint location, GLint x, GLint *cx = NULL);
	_CrossExport void cglUniform1iv(GLint location, GLsizei count, const GLint *v, GLint *cv = NULL);
	_CrossExport void cglUniform2f(GLint location, GLfloat x, GLfloat y, GLfloat *cx = NULL, GLfloat *cy = NULL);
	_CrossExport void cglUniform2fv(GLint location, GLsizei count, const GLfloat *v, GLfloat *cv = NULL);
	_CrossExport void cglUniform2i(GLint location, GLint x, GLint y, GLint *cx = NULL, GLint *cy = NULL);
	_CrossExport void cglUniform2iv(GLint location, GLsizei count, const GLint *v, GLint *cv = NULL);
	_CrossExport void cglUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat *cx = NULL, GLfloat *cy = NULL, GLfloat *cz = NULL);
	_CrossExport void cglUniform3fv(GLint location, GLsizei count, const GLfloat *v, GLfloat *cv = NULL);
	_CrossExport void cglUniform3i(GLint location, GLint x, GLint y, GLint z, GLint *cx = NULL, GLint *cy = NULL, GLint *cz = NULL);
	_CrossExport void cglUniform3iv(GLint location, GLsizei count, const GLint *v, GLint *cv = NULL);
	_CrossExport void cglUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat *cx = NULL, GLfloat *cy = NULL, GLfloat *cz = NULL, GLfloat *cw = NULL);
	_CrossExport void cglUniform4fv(GLint location, GLsizei count, const GLfloat *v, GLfloat *cv = NULL);
	_CrossExport void cglUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w, GLint *cx = NULL, GLint *cy = NULL, GLint *cz = NULL, GLint *cw = NULL);
	_CrossExport void cglUniform4iv(GLint location, GLsizei count, const GLint *v, GLint *cv = NULL);
	_CrossExport void cglUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *v, GLfloat *cv = NULL);
	_CrossExport void cglUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *v, GLfloat *cv = NULL);
	_CrossExport void cglUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *v, GLfloat *cv = NULL);
	_CrossExport void cglUseProgram(GLuint program);
	_CrossExport void cglValidateProgram(GLuint program);
	_CrossExport void cglVertexAttrib1f(GLuint index, GLfloat x);
	_CrossExport void cglVertexAttrib1fv(GLuint index, const GLfloat *values);
	_CrossExport void cglVertexAttrib2f(GLuint index, GLfloat x, GLfloat y);
	_CrossExport void cglVertexAttrib2fv(GLuint index, const GLfloat *values);
	_CrossExport void cglVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z);
	_CrossExport void cglVertexAttrib3fv(GLuint index, const GLfloat *values);
	_CrossExport void cglVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	_CrossExport void cglVertexAttrib4fv(GLuint index, const GLfloat *values);
	_CrossExport void cglVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *ptr);

	_CrossExport void cglDrawBuffers(GLsizei n, const GLenum *bufs);
	_CrossExport void cglReadBuffer(GLenum mode);
	_CrossExport void cglGenQueries(GLsizei n, GLuint* ids);
	_CrossExport void cglDeleteQueries(GLsizei n, const GLuint* ids);
	_CrossExport void cglBeginQuery(GLenum target, GLuint id);
	_CrossExport void cglEndQuery(GLenum target);
	_CrossExport void cglGetQueryiv(GLenum target, GLenum pname, GLint* params);
	_CrossExport void cglGetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params);
	_CrossExport GLboolean cglIsQuery(GLuint id);
	/*
	_CrossExport void cglBeginTransformFeedback(GLenum primitiveMode);
	_CrossExport void cglBindBufferBase(GLenum target, GLuint index, GLuint buffer);
	_CrossExport void cglBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
	_CrossExport void cglBindSampler(GLuint unit, GLuint sampler);
	_CrossExport void cglBindTransformFeedback(GLenum target, GLuint id);
	_CrossExport void cglBindVertexArray(GLuint array);
	_CrossExport void cglBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
	_CrossExport void cglClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
	_CrossExport void cglClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value);
	_CrossExport void cglClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint* value);
	_CrossExport void cglClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint* value);
	_CrossExport GLenum cglClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);
	_CrossExport void cglCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data);
	_CrossExport void cglCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data);
	_CrossExport void cglCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
	_CrossExport void cglCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	_CrossExport void cglDeleteSamplers(GLsizei count, const GLuint* samplers);
	_CrossExport void cglDeleteSync(GLsync sync);
	_CrossExport void cglDeleteTransformFeedbacks(GLsizei n, const GLuint* ids);
	_CrossExport void cglDeleteVertexArrays(GLsizei n, const GLuint* arrays);
	_CrossExport void cglDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount);
	_CrossExport void cglDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei instanceCount);
	_CrossExport void cglDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices);
	_CrossExport void cglEndTransformFeedback(void);
	_CrossExport GLsync cglFenceSync(GLenum condition, GLbitfield flags);
	_CrossExport void cglFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length);
	_CrossExport void cglFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
	_CrossExport void cglGenSamplers(GLsizei count, GLuint* samplers);
	_CrossExport void cglGenTransformFeedbacks(GLsizei n, GLuint* ids);
	_CrossExport void cglGenVertexArrays(GLsizei n, GLuint* arrays);
	_CrossExport void cglGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
	_CrossExport void cglGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
	_CrossExport void cglGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
	_CrossExport void cglGetBufferParameteri64v(GLenum target, GLenum pname, GLint64* params);
	_CrossExport void cglGetBufferPointerv(GLenum target, GLenum pname, GLvoid** params);
	_CrossExport GLint cglGetFragDataLocation(GLuint program, const GLchar *name);
	_CrossExport void cglGetInteger64i_v(GLenum target, GLuint index, GLint64* data);
	_CrossExport void cglGetInteger64v(GLenum pname, GLint64* params);
	_CrossExport void cglGetIntegeri_v(GLenum target, GLuint index, GLint* data);
	_CrossExport void cglGetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params);
	_CrossExport void cglGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, GLvoid* binary);
	_CrossExport void cglGetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat* params);
	_CrossExport void cglGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint* params);
	_CrossExport const GLubyte* cglGetStringi(GLenum name, GLuint index);
	_CrossExport void cglGetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values);
	_CrossExport void cglGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name);
	_CrossExport GLuint cglGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName);
	_CrossExport void cglGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices);
	_CrossExport void cglGetUniformuiv(GLuint program, GLint location, GLuint* params);
	_CrossExport void cglGetVertexAttribIiv(GLuint index, GLenum pname, GLint* params);
	_CrossExport void cglGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint* params);
	_CrossExport void cglInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum* attachments);
	_CrossExport void cglInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
	_CrossExport GLboolean cglIsSampler(GLuint sampler);
	_CrossExport GLboolean cglIsSync(GLsync sync);
	_CrossExport GLboolean cglIsTransformFeedback(GLuint id);
	_CrossExport GLboolean cglIsVertexArray(GLuint array);
	_CrossExport GLvoid* cglMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
	_CrossExport void cglPauseTransformFeedback(void);
	_CrossExport void cglProgramBinary(GLuint program, GLenum binaryFormat, const GLvoid* binary, GLsizei length);
	_CrossExport void cglProgramParameteri(GLuint program, GLenum pname, GLint value);
	_CrossExport void cglRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
	_CrossExport void cglResumeTransformFeedback(void);
	_CrossExport void cglSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param);
	_CrossExport void cglSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat* param);
	_CrossExport void cglSamplerParameteri(GLuint sampler, GLenum pname, GLint param);
	_CrossExport void cglSamplerParameteriv(GLuint sampler, GLenum pname, const GLint* param);
	_CrossExport void cglTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
	_CrossExport void cglTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
	_CrossExport void cglTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
	_CrossExport void cglTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels);
	_CrossExport void cglTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode);
	_CrossExport void cglUniform1ui(GLint location, GLuint v0);
	_CrossExport void cglUniform1uiv(GLint location, GLsizei count, const GLuint* value);
	_CrossExport void cglUniform2ui(GLint location, GLuint v0, GLuint v1);
	_CrossExport void cglUniform2uiv(GLint location, GLsizei count, const GLuint* value);
	_CrossExport void cglUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2);
	_CrossExport void cglUniform3uiv(GLint location, GLsizei count, const GLuint* value);
	_CrossExport void cglUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
	_CrossExport void cglUniform4uiv(GLint location, GLsizei count, const GLuint* value);
	_CrossExport void cglUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
	_CrossExport void cglUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	_CrossExport void cglUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	_CrossExport void cglUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	_CrossExport void cglUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	_CrossExport void cglUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	_CrossExport void cglUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	_CrossExport GLboolean cglUnmapBuffer(GLenum target);
	_CrossExport void cglVertexAttribDivisor(GLuint index, GLuint divisor);
	_CrossExport void cglVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w);
	_CrossExport void cglVertexAttribI4iv(GLuint index, const GLint* v);
	_CrossExport void cglVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
	_CrossExport void cglVertexAttribI4uiv(GLuint index, const GLuint* v);
	_CrossExport void cglVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
	_CrossExport void cglWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);
	*/

	_CrossExport void cglBegin(GLenum mode);
	_CrossExport void cglEnd(void);
	_CrossExport void cglColor3b(GLbyte red, GLbyte green, GLbyte blue);
	_CrossExport void cglColor3bv(const GLbyte *v);
	_CrossExport void cglColor3d(GLdouble red, GLdouble green, GLdouble blue);
	_CrossExport void cglColor3dv(const GLdouble *v);
	_CrossExport void cglColor3f(GLfloat red, GLfloat green, GLfloat blue);
	_CrossExport void cglColor3fv(const GLfloat *v);
	_CrossExport void cglColor3i(GLint red, GLint green, GLint blue);
	_CrossExport void cglColor3iv(const GLint *v);
	_CrossExport void cglColor3s(GLshort red, GLshort green, GLshort blue);
	_CrossExport void cglColor3sv(const GLshort *v);
	_CrossExport void cglColor3ub(GLubyte red, GLubyte green, GLubyte blue);
	_CrossExport void cglColor3ubv(const GLubyte *v);
	_CrossExport void cglColor3ui(GLuint red, GLuint green, GLuint blue);
	_CrossExport void cglColor3uiv(const GLuint *v);
	_CrossExport void cglColor3us(GLushort red, GLushort green, GLushort blue);
	_CrossExport void cglColor3usv(const GLushort *v);
	_CrossExport void cglColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
	_CrossExport void cglColor4bv(const GLbyte *v);
	_CrossExport void cglColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
	_CrossExport void cglColor4dv(const GLdouble *v);
	_CrossExport void cglColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	_CrossExport void cglColor4fv(const GLfloat *v);
	_CrossExport void cglColor4i(GLint red, GLint green, GLint blue, GLint alpha);
	_CrossExport void cglColor4iv(const GLint *v);
	_CrossExport void cglColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha);
	_CrossExport void cglColor4sv(const GLshort *v);
	_CrossExport void cglColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
	_CrossExport void cglColor4ubv(const GLubyte *v);
	_CrossExport void cglColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha);
	_CrossExport void cglColor4uiv(const GLuint *v);
	_CrossExport void cglColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha);
	_CrossExport void cglColor4usv(const GLushort *v);
	_CrossExport void cglNormal3b(GLbyte nx, GLbyte ny, GLbyte nz);
	_CrossExport void cglNormal3bv(const GLbyte *v);
	_CrossExport void cglNormal3d(GLdouble nx, GLdouble ny, GLdouble nz);
	_CrossExport void cglNormal3dv(const GLdouble *v);
	_CrossExport void cglNormal3f(GLfloat nx, GLfloat ny, GLfloat nz);
	_CrossExport void cglNormal3fv(const GLfloat *v);
	_CrossExport void cglNormal3i(GLint nx, GLint ny, GLint nz);
	_CrossExport void cglNormal3iv(const GLint *v);
	_CrossExport void cglNormal3s(GLshort nx, GLshort ny, GLshort nz);
	_CrossExport void cglNormal3sv(const GLshort *v);
	_CrossExport void cglVertex2d(GLdouble x, GLdouble y);
	_CrossExport void cglVertex2dv(const GLdouble *v);
	_CrossExport void cglVertex2f(GLfloat x, GLfloat y);
	_CrossExport void cglVertex2fv(const GLfloat *v);
	_CrossExport void cglVertex2i(GLint x, GLint y);
	_CrossExport void cglVertex2iv(const GLint *v);
	_CrossExport void cglVertex2s(GLshort x, GLshort y);
	_CrossExport void cglVertex2sv(const GLshort *v);
	_CrossExport void cglVertex3d(GLdouble x, GLdouble y, GLdouble z);
	_CrossExport void cglVertex3dv(const GLdouble *v);
	_CrossExport void cglVertex3f(GLfloat x, GLfloat y, GLfloat z);
	_CrossExport void cglVertex3fv(const GLfloat *v);
	_CrossExport void cglVertex3i(GLint x, GLint y, GLint z);
	_CrossExport void cglVertex3iv(const GLint *v);
	_CrossExport void cglVertex3s(GLshort x, GLshort y, GLshort z);
	_CrossExport void cglVertex3sv(const GLshort *v);
	_CrossExport void cglVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	_CrossExport void cglVertex4dv(const GLdouble *v);
	_CrossExport void cglVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	_CrossExport void cglVertex4fv(const GLfloat *v);
	_CrossExport void cglVertex4i(GLint x, GLint y, GLint z, GLint w);
	_CrossExport void cglVertex4iv(const GLint *v);
	_CrossExport void cglVertex4s(GLshort x, GLshort y, GLshort z, GLshort w);
	_CrossExport void cglVertex4sv(const GLshort *v);
	_CrossExport void cglTexCoord1d(GLdouble s);
	_CrossExport void cglTexCoord1dv(const GLdouble *v);
	_CrossExport void cglTexCoord1f(GLfloat s);
	_CrossExport void cglTexCoord1fv(const GLfloat *v);
	_CrossExport void cglTexCoord1i(GLint s);
	_CrossExport void cglTexCoord1iv(const GLint *v);
	_CrossExport void cglTexCoord1s(GLshort s);
	_CrossExport void cglTexCoord1sv(const GLshort *v);
	_CrossExport void cglTexCoord2d(GLdouble s, GLdouble t);
	_CrossExport void cglTexCoord2dv(const GLdouble *v);
	_CrossExport void cglTexCoord2f(GLfloat s, GLfloat t);
	_CrossExport void cglTexCoord2fv(const GLfloat *v);
	_CrossExport void cglTexCoord2i(GLint s, GLint t);
	_CrossExport void cglTexCoord2iv(const GLint *v);
	_CrossExport void cglTexCoord2s(GLshort s, GLshort t);
	_CrossExport void cglTexCoord2sv(const GLshort *v);
	_CrossExport void cglTexCoord3d(GLdouble s, GLdouble t, GLdouble r);
	_CrossExport void cglTexCoord3dv(const GLdouble *v);
	_CrossExport void cglTexCoord3f(GLfloat s, GLfloat t, GLfloat r);
	_CrossExport void cglTexCoord3fv(const GLfloat *v);
	_CrossExport void cglTexCoord3i(GLint s, GLint t, GLint r);
	_CrossExport void cglTexCoord3iv(const GLint *v);
	_CrossExport void cglTexCoord3s(GLshort s, GLshort t, GLshort r);
	_CrossExport void cglTexCoord3sv(const GLshort *v);
	_CrossExport void cglTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
	_CrossExport void cglTexCoord4dv(const GLdouble *v);
	_CrossExport void cglTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
	_CrossExport void cglTexCoord4fv(const GLfloat *v);
	_CrossExport void cglTexCoord4i(GLint s, GLint t, GLint r, GLint q);
	_CrossExport void cglTexCoord4iv(const GLint *v);
	_CrossExport void cglTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q);
	_CrossExport void cglTexCoord4sv(const GLshort *v);
	_CrossExport void cglDrawBuffer(GLenum mode);
	_CrossExport void cglGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
	_CrossExport void cglLineStipple(GLint factor, GLushort pattern);

}

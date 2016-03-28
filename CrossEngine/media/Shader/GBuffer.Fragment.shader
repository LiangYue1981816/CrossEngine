<Shader type="fragment">
<Parameters>
</Parameters>

<Source>
<![CDATA[
#if defined GL_ES
	#define LOWP lowp
	#define MEDIUMP mediump
	#define HIGHP highp

	#define ATTRIBUTE_LOWP attribute lowp
	#define ATTRIBUTE_MEDIUMP attribute mediump
	#define ATTRIBUTE_HIGHP attribute highp

	#define UNIFORM_LOWP uniform lowp
	#define UNIFORM_MEDIUMP uniform mediump
	#define UNIFORM_HIGHP uniform highp

	#define VARYING_LOWP varying lowp
	#define VARYING_MEDIUMP varying mediump
	#define VARYING_HIGHP varying mediump // highp (Compatible Mali GPU)

	precision mediump int;
	precision mediump float;
#else
	#define LOWP
	#define MEDIUMP
	#define HIGHP

	#define ATTRIBUTE_LOWP attribute
	#define ATTRIBUTE_MEDIUMP attribute
	#define ATTRIBUTE_HIGHP attribute

	#define UNIFORM_LOWP uniform
	#define UNIFORM_MEDIUMP uniform
	#define UNIFORM_HIGHP uniform

	#define VARYING_LOWP varying
	#define VARYING_MEDIUMP varying
	#define VARYING_HIGHP varying
#endif


UNIFORM_HIGHP vec4 _cameraPerspectiveParam; // fovy, aspect, znear, zfar

VARYING_HIGHP vec3 vPosition;


HIGHP vec4 EncodeFloatRGBA(HIGHP float value)
{
	const HIGHP vec4 magica = vec4(1.0, 255.0, 65025.0, 16581375.0);
	const HIGHP vec4 magicb = vec4(1.0/255.0, 1.0/255.0, 1.0/255.0, 0.0);

	HIGHP vec4 rgba = magica * value;
	rgba  = fract(rgba);
	rgba -= magicb * rgba.yzww;

	return rgba;
}

void main()
{
	HIGHP float depth = (length(vPosition.xyz) - _cameraPerspectiveParam.z) / (_cameraPerspectiveParam.w - _cameraPerspectiveParam.z);
	gl_FragColor = EncodeFloatRGBA(depth);
}
]]>
</Source>
</Shader>
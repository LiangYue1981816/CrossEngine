<Shader type="fragment">
<Parameters>
	<Uniform1i name="diffusemap" scope="Local" value="0"/>
	<Uniform1i name="blurmap" scope="Local" value="1"/>
	<Uniform1i name="depthmap" scope="Local" value="2"/>
	<Uniform1f name="focalDistance" scope="Global" value="128.0"/>
	<Uniform1f name="focalRange" scope="Global" value="512.0"/>
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

UNIFORM_MEDIUMP sampler2D diffusemap;
UNIFORM_MEDIUMP sampler2D blurmap;
UNIFORM_MEDIUMP sampler2D depthmap;

UNIFORM_MEDIUMP float focalDistance;
UNIFORM_MEDIUMP float focalRange;

VARYING_MEDIUMP vec2 vTexcoord;


HIGHP float DncodeFloatRGBA(HIGHP vec4 rgba)
{
	const HIGHP vec4 magic = vec4(1.0, 1.0/255.0, 1.0/65025.0, 1.0/16581375.0);
	return dot(rgba, magic);
}

void main()
{
	MEDIUMP vec4 diffuseColor = texture2D(diffusemap, vTexcoord);
	MEDIUMP vec4 blurColor = texture2D(blurmap, vTexcoord);

	HIGHP float clipRange = _cameraPerspectiveParam.w - _cameraPerspectiveParam.z;
	HIGHP float currDistance = DncodeFloatRGBA(texture2D(depthmap, vTexcoord)) * clipRange;
	HIGHP float lerp = min(1.0, abs(focalDistance - currDistance) / focalRange);

	gl_FragColor = mix(diffuseColor, blurColor, lerp);
}
]]>
</Source>
</Shader>
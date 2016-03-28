<Shader type="fragment">
<Parameters>
	<Uniform1i name="diffusemap" scope="Local" value="0"/>
	<Uniform1i name="depthmap" scope="Local" value="1"/>
	<Uniform1f name="ssrsMaxParallax" scope="Global" value="0.01"/>
	<Uniform1f name="ssrsFocusDistance" scope="Global" value="0.001"/>
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


UNIFORM_MEDIUMP sampler2D diffusemap;
UNIFORM_MEDIUMP sampler2D depthmap;

UNIFORM_MEDIUMP float ssrsMaxParallax;
UNIFORM_MEDIUMP float ssrsFocusDistance;

VARYING_MEDIUMP vec2 vTexcoord;


HIGHP float DncodeFloatRGBA(HIGHP vec4 rgba)
{
	const HIGHP vec4 magic = vec4(1.0, 1.0/255.0, 1.0/65025.0, 1.0/16581375.0);
	return dot(rgba, magic);
}

const MEDIUMP vec3 samples = vec3(0.5, 0.66, 1.0);
const MEDIUMP mat4 matLeft = mat4(0.4561, -0.0400822, -0.0152161, 0.0, 0.500484, -0.0378246, -0.0205971, 0.0, 0.176381, -0.0157589, -0.00546856, 0.0, 0.0, 0.0, 0.0, 1.0);
const MEDIUMP mat4 matRight = mat4(-0.0434706, 0.378476, -0.0721527, 0.0, -0.0879388, 0.73364, -0.112961, 0.0, -0.00155529, -0.0184503, 1.2264, 0.0, 0.0, 0.0, 0.0, 1.0);

void main()
{
	HIGHP float minDepthL = 1.0;
	HIGHP float minDepthR = 1.0;

	for (LOWP int i = 0; i < 3; i++) {
		MEDIUMP vec2 uv = vec2(samples[i] * ssrsMaxParallax, 0.0);
		minDepthL = min(minDepthL, DncodeFloatRGBA(texture2D(depthmap, vTexcoord + uv)));
		minDepthR = min(minDepthR, DncodeFloatRGBA(texture2D(depthmap, vTexcoord - uv)));
	}

	MEDIUMP float parallaxL = ssrsMaxParallax * (1.0 - ssrsFocusDistance / minDepthL);
	MEDIUMP float parallaxR = ssrsMaxParallax * (1.0 - ssrsFocusDistance / minDepthR);

	MEDIUMP vec4 colorL = matLeft * texture2D(diffusemap, vTexcoord + vec2(parallaxL, 0.0));
	MEDIUMP vec4 colorR = matRight * texture2D(diffusemap, vTexcoord - vec2(parallaxR, 0.0));

	gl_FragColor = clamp(colorL + colorR, 0.0, 1.0);
}
]]>
</Source>
</Shader>
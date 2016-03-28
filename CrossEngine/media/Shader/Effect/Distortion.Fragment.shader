<Shader type="fragment">
<Parameters>
	<Uniform1i name="diffusemap" scope="Local" value="0"/>
	<Uniform1i name="distortionmap" scope="Local" value="1"/>
	<Uniform1f name="distortionRange" scope="Global" value="0.05"/>
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
UNIFORM_MEDIUMP sampler2D distortionmap;
UNIFORM_MEDIUMP float distortionRange;

VARYING_MEDIUMP vec2 vTexcoord;


void main()
{
	MEDIUMP vec3 distortion = 2.0 * texture2D(distortionmap, vTexcoord).rgb - 1.0;
	MEDIUMP vec3 viewDirection = vec3(0.0, 0.0, 1.0);
	MEDIUMP vec3 reflectDirection = reflect(viewDirection, distortion);

	gl_FragColor = texture2D(diffusemap, vTexcoord + reflectDirection.xy * distortionRange);
}
]]>
</Source>
</Shader>
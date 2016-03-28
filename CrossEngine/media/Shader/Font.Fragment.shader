<Shader type="fragment">
<Parameters>
	<Uniform1i name="diffusemap" scope="Local" value="0"/>
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

VARYING_LOWP vec4 vColor;
VARYING_MEDIUMP vec2 vTexcoord;


void main()
{
	LOWP vec4 diffuseColor = texture2D(diffusemap, vTexcoord);
	gl_FragColor = vec4(vColor.rgb, vColor.a * diffuseColor.a);
}
]]>
</Source>
</Shader>
<Shader type="fragment">
<Parameters>
	<Uniform1i name="diffusemap" scope="Local" value="0"/>
	<Uniform1f name="bloomBrightLuminance" scope="Global" value="0.08"/>
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
UNIFORM_MEDIUMP float bloomBrightLuminance;

VARYING_MEDIUMP vec2 vTexcoord;


void main()
{
	/*
	//-------------------------------
	// High-pass filter for obtaining bloomBrightLuminance
	// We use an approximation formula that is pretty fast:
	//   f(x) = ( -3 * ( x - 1 )^2 + 1 ) * 2
	//   Color += Grayscale( f(Color) ) + 0.6
	//
	// Special thanks to ATI for their great HLSL2GLSL utility
	//     http://sourceforge.net/projects/hlsl2glsl
	//-------------------------------

	LOWP vec4 diffuse = texture2D(diffusemap, vTexcoord) - 1.0;
	MEDIUMP vec4 bright = -6.0 * diffuse * diffuse + 2.0;
	MEDIUMP float gray = dot(bright, vec4(0.33, 0.59, 0.11, 0.0));
//	gl_FragColor = diffuse + gray + 0.6;
	gl_FragColor = diffuse + gray;
	*/

	MEDIUMP float fMiddleGray = 0.18;
	MEDIUMP float fWhiteCutoff = 0.8;
	MEDIUMP vec3 diffuse = texture2D(diffusemap, vTexcoord).rgb;
	diffuse *= fMiddleGray / bloomBrightLuminance;
	diffuse *= (1.0 + (diffuse / (fWhiteCutoff * fWhiteCutoff)));
	diffuse -= 5.0;
	diffuse  = max(diffuse, 0.0);
	diffuse /= (10.0 + diffuse);
	gl_FragColor = vec4(diffuse, 1.0);
}
]]>
</Source>
</Shader>
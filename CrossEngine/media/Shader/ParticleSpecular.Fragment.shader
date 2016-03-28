<Shader type="fragment">
<Parameters>
	<Uniform1i name="diffusemap" scope="Local" value="0"/>
	<Uniform1i name="normalmap" scope="Local" value="1"/>
	<Uniform1i name="specularmap" scope="Local" value="2"/>
	<Uniform1f name="specularmapFactor" scope="Global" value="1.0"/>
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
UNIFORM_MEDIUMP sampler2D normalmap;
UNIFORM_MEDIUMP sampler2D specularmap;
UNIFORM_LOWP float specularmapFactor;

VARYING_LOWP vec4 vColor;
VARYING_MEDIUMP vec2 vTexcoord;
VARYING_MEDIUMP vec3 vHalfVector;


void main()
{
	MEDIUMP vec3 pixelNormal;
	pixelNormal = texture2D(normalmap, vTexcoord).xyz * 2.0 - 1.0;
	pixelNormal = normalize(pixelNormal);

	MEDIUMP vec3 halfVector;
	halfVector = normalize(vHalfVector);

	// Fast specular approximation (http://www.gamasutra.com/view/feature/2972/a_noninteger_power_function_on_.php)
	// Basically pow( N, P ) can be approximated by pow( max( A * N + B ), M )
	//      - A and B are constants that must be tweaked to get artifact-free results
	//      - M can be really small in practice (2 looks good, can be replaced by single multiply)
	// This should result in a mad_sat instruction plus one multiply (2 instructions total!)

	#define SPEC_APPROX_A  6.645
	#define SPEC_APPROX_B -5.645

	MEDIUMP float specularBase = max(dot(pixelNormal, halfVector), 0.0);
	MEDIUMP float specularAmount = clamp(SPEC_APPROX_A * specularBase + SPEC_APPROX_B, 0.0, 1.0);
	MEDIUMP vec3 specularLighting = texture2D(specularmap, vTexcoord).rgb * specularmapFactor * specularAmount * specularAmount;

//	gl_FragColor = (texture2D(diffusemap, vTexcoord) + vec4(specularLighting, 0.0)) * vColor;
	gl_FragColor = texture2D(diffusemap, vTexcoord) * vColor + vec4(specularLighting, 0.0);
}
]]>
</Source>
</Shader>
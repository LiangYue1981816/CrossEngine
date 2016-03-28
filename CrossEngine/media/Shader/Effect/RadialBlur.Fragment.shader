<Shader type="fragment">
<Parameters>
	<Uniform1i name="diffusemap" scope="Local" value="0"/>
	<Uniform1f name="radialBlurSampleRadius" scope="Global" value="0.25"/>
	<Uniform1f name="radialBlurSampleStrength" scope="Global" value="3.0"/>
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

UNIFORM_MEDIUMP float radialBlurSampleRadius;
UNIFORM_MEDIUMP float radialBlurSampleStrength;

VARYING_MEDIUMP vec2 vTexcoord;


void main()
{
	MEDIUMP vec2 dir = 0.5 - vTexcoord;
	MEDIUMP float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
	MEDIUMP float t = dist * radialBlurSampleStrength;

	MEDIUMP vec4 radiusColor;
	MEDIUMP vec4 diffuse = texture2D(diffusemap, vTexcoord);

	MEDIUMP float samples[11];

	samples[0] = -0.08;
	samples[1] = -0.05;
	samples[2] = -0.03;
	samples[3] = -0.02;
	samples[4] = -0.01;
	samples[5] =  0.00;
	samples[6] =  0.01;
	samples[7] =  0.02;
	samples[8] =  0.03;
	samples[9] =  0.05;
	samples[10]=  0.08;

	dir = normalize(dir);

	radiusColor += texture2D(diffusemap, vTexcoord + dir * samples[0] * radialBlurSampleRadius);
	radiusColor += texture2D(diffusemap, vTexcoord + dir * samples[1] * radialBlurSampleRadius);
	radiusColor += texture2D(diffusemap, vTexcoord + dir * samples[2] * radialBlurSampleRadius);
	radiusColor += texture2D(diffusemap, vTexcoord + dir * samples[3] * radialBlurSampleRadius);
	radiusColor += texture2D(diffusemap, vTexcoord + dir * samples[4] * radialBlurSampleRadius);
	radiusColor += texture2D(diffusemap, vTexcoord + dir * samples[5] * radialBlurSampleRadius);
	radiusColor += texture2D(diffusemap, vTexcoord + dir * samples[6] * radialBlurSampleRadius);
	radiusColor += texture2D(diffusemap, vTexcoord + dir * samples[7] * radialBlurSampleRadius);
	radiusColor += texture2D(diffusemap, vTexcoord + dir * samples[8] * radialBlurSampleRadius);
	radiusColor += texture2D(diffusemap, vTexcoord + dir * samples[9] * radialBlurSampleRadius);
	radiusColor += texture2D(diffusemap, vTexcoord + dir * samples[10]* radialBlurSampleRadius);
	radiusColor /= 11.0;

	t = clamp(t, 0.0, 1.0);
	gl_FragColor = mix(diffuse, radiusColor, t);
}
]]>
</Source>
</Shader>
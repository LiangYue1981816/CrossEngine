<Shader type="fragment">
<Parameters>
	<Uniform1i name="blurmap" scope="Local" value="0"/>
	<Uniform1i name="blendmap" scope="Local" value="1"/>
	<Uniform1f name="blurRange" scope="Local" value="0.01"/>
	<Uniform1f name="blurFactor" scope="Local" value="0.15"/>
	<Uniform1f name="blendFactor" scope="Local" value="0.95"/>
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


UNIFORM_MEDIUMP sampler2D blurmap;
UNIFORM_MEDIUMP sampler2D blendmap;
UNIFORM_MEDIUMP float blurRange;
UNIFORM_MEDIUMP float blurFactor;
UNIFORM_MEDIUMP float blendFactor;

VARYING_MEDIUMP vec2 vTexcoord;


void main()
{
	MEDIUMP float offsets[13];
	MEDIUMP float samples[13];
	MEDIUMP vec4 blurColor = vec4(0.0, 0.0, 0.0, 0.0);

	offsets[0] = -6.0;
	offsets[1] = -5.0;
	offsets[2] = -4.0;
	offsets[3] = -3.0;
	offsets[4] = -2.0;
	offsets[5] = -1.0;
	offsets[6] =  0.0;
	offsets[7] =  1.0;
	offsets[8] =  2.0;
	offsets[9] =  3.0;
	offsets[10] = 4.0;
	offsets[11] = 5.0;
	offsets[12] = 6.0;

	samples[0] = 0.002216;
	samples[1] = 0.008764;
	samples[2] = 0.026995;
	samples[3] = 0.064759;
	samples[4] = 0.120985;
	samples[5] = 0.176033;
	samples[6] = 0.199471;
	samples[7] = 0.176033;
	samples[8] = 0.120985;
	samples[9] = 0.064759;
	samples[10] = 0.026995;
	samples[11] = 0.008764;
	samples[12] = 0.002216;

	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[0] * blurRange, 0.0)) * samples[0];
	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[1] * blurRange, 0.0)) * samples[1];
	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[2] * blurRange, 0.0)) * samples[2];
	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[3] * blurRange, 0.0)) * samples[3];
	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[4] * blurRange, 0.0)) * samples[4];
	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[5] * blurRange, 0.0)) * samples[5];
	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[6] * blurRange, 0.0)) * samples[6];
	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[7] * blurRange, 0.0)) * samples[7];
	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[8] * blurRange, 0.0)) * samples[8];
	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[9] * blurRange, 0.0)) * samples[9];
	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[10] * blurRange, 0.0)) * samples[10];
	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[11] * blurRange, 0.0)) * samples[11];
	blurColor += texture2D(blurmap, vTexcoord + vec2(offsets[12] * blurRange, 0.0)) * samples[12];

	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[0] * blurRange)) * samples[0];
	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[1] * blurRange)) * samples[1];
	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[2] * blurRange)) * samples[2];
	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[3] * blurRange)) * samples[3];
	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[4] * blurRange)) * samples[4];
	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[5] * blurRange)) * samples[5];
	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[6] * blurRange)) * samples[6];
	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[7] * blurRange)) * samples[7];
	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[8] * blurRange)) * samples[8];
	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[9] * blurRange)) * samples[9];
	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[10] * blurRange)) * samples[10];
	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[11] * blurRange)) * samples[11];
	blurColor += texture2D(blurmap, vTexcoord + vec2(0.0, offsets[12] * blurRange)) * samples[12];

	MEDIUMP vec4 blendColor = texture2D(blendmap, vTexcoord);
	gl_FragColor = blurColor * blurFactor + blendColor * blendFactor;
}
]]>
</Source>
</Shader>
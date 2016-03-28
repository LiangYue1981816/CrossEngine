<Shader type="fragment">
<Parameters>
	<Uniform1i name="diffusemap" scope="Local" value="0"/>
	<Uniform1i name="raymap" scope="Local" value="1"/>
	<Uniform1f name="godRayLength" scope="Local" value="0.5"/>
	<Uniform1f name="godRayFactor" scope="Global" value="0.45"/>
	<Uniform1f name="godRayBlendFactor" scope="Global" value="1.0"/>
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


UNIFORM_MEDIUMP vec2 _godRayScreenPosition;

UNIFORM_MEDIUMP sampler2D diffusemap;
UNIFORM_MEDIUMP sampler2D raymap;
UNIFORM_MEDIUMP float godRayLength;
UNIFORM_MEDIUMP float godRayFactor;
UNIFORM_MEDIUMP float godRayBlendFactor;

VARYING_MEDIUMP vec2 vTexcoord;


void main()
{
	MEDIUMP vec2 ray = (_godRayScreenPosition - vTexcoord) * godRayLength;

	LOWP vec4 rayBaseColor = texture2D(raymap, vTexcoord);
	LOWP vec4 rayColor = rayBaseColor;

	rayColor += max(rayBaseColor, texture2D(raymap, vTexcoord + ray * 0.125));
	rayColor += max(rayBaseColor, texture2D(raymap, vTexcoord + ray * 0.250));
	rayColor += max(rayBaseColor, texture2D(raymap, vTexcoord + ray * 0.375));
	rayColor += max(rayBaseColor, texture2D(raymap, vTexcoord + ray * 0.500));
	rayColor += max(rayBaseColor, texture2D(raymap, vTexcoord + ray * 0.625));
	rayColor += max(rayBaseColor, texture2D(raymap, vTexcoord + ray * 0.750));
	rayColor += max(rayBaseColor, texture2D(raymap, vTexcoord + ray * 0.875));
	rayColor += max(rayBaseColor, texture2D(raymap, vTexcoord + ray * 1.000));
	rayColor *= 0.125;

	MEDIUMP vec4 diffuseColor = texture2D(diffusemap, vTexcoord);
	gl_FragColor = rayColor * godRayFactor + diffuseColor * godRayBlendFactor;
}
]]>
</Source>
</Shader>
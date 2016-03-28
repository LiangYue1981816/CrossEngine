<Shader type="fragment">
<Parameters>
	<Uniform1i name="bumpmap" scope="Local" value="0"/>
	<Uniform1i name="lightmap" scope="Local" value="1"/>
	<Uniform1i name="envmap" scope="Local" value="2"/>
	<Uniform3f name="color" scope="Local" value="0.19, 0.34, 0.55"/>
	<Uniform1f name="fade" scope="Local" value="0.5"/>
	<Uniform1f name="lightmapFactor" scope="Global" value="1.2"/>
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


UNIFORM_MEDIUMP sampler2D bumpmap;
UNIFORM_MEDIUMP sampler2D lightmap;
UNIFORM_MEDIUMP samplerCube envmap;

UNIFORM_LOWP vec3 color;
UNIFORM_LOWP float fade;
UNIFORM_LOWP float lightmapFactor;

VARYING_HIGHP vec3 vNormal;
VARYING_HIGHP vec3 vViewDirection;
VARYING_MEDIUMP vec2 vTexcoord0;
VARYING_MEDIUMP vec2 vTexcoord1;


void main()
{
	MEDIUMP vec3 bump = texture2D(bumpmap, vTexcoord0).xyz;
	bump     = 2.0 * bump - 1.0;
	bump.xz *= 0.15;
	bump.y   = 0.8 * abs(bump.y) + 0.2;
	bump     = normalize(bump + vNormal);

	MEDIUMP vec3 reflectDirection = reflect(vViewDirection, bump);
	LOWP vec3 envColor = textureCube(envmap, reflectDirection.xyz).rgb;

	LOWP float lrp = 1.0 - dot(-normalize(vViewDirection), bump);
	lrp = clamp(6.645 * lrp + -5.645, 0.0, 1.0);
	lrp *= lrp;

	LOWP vec3 lightmapColor = vec3(1.0, 1.0, 1.0);
//	LOWP vec3 lightmapColor = texture2D(lightmap, vTexcoord1).rgb * lightmapFactor;

	gl_FragColor.rgb = lightmapColor * mix(color, envColor, clamp(fade + lrp, 0.0, 1.0));
}
]]>
</Source>
</Shader>
<Shader type="fragment">
<Parameters>
	<Uniform1i name="diffusemap" scope="Local" value="0"/>
	<!--<Uniform1f name="colorThreshold" scope="Global" value="0.0"/>-->
	<!--<Uniform1f name="colorContrast" scope="Global" value="1.0"/>-->
	<!--<Uniform3f name="colorOverlay" scope="Global" value="0.5 0.5 0.5"/>-->
	<Uniform1f name="colorGrayFactor" scope="Global" value="0.0"/>
	<Uniform3f name="colorAdd" scope="Global" value="0.0 0.0 0.0"/>
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
/*
UNIFORM_MEDIUMP float colorThreshold;
UNIFORM_MEDIUMP float colorContrast;
UNIFORM_MEDIUMP vec3 colorOverlay;
*/
UNIFORM_MEDIUMP float colorGrayFactor;
UNIFORM_MEDIUMP vec3 colorAdd;

VARYING_MEDIUMP vec2 vTexcoord;


void main()
{
	MEDIUMP vec3 diffuseColor = texture2D(diffusemap, vTexcoord).rgb;
	MEDIUMP float gray = dot(diffuseColor, vec3(0.333333, 0.333333, 0.333333));

	/*
	// Bright/Expand
	diffuseColor = (diffuseColor - colorThreshold) * colorContrast;

	// Overlay
	if (gray < 0.5) {
		diffuseColor = 2.0 * diffuseColor * colorOverlay;
	}
	else {
		diffuseColor = 1.0 - 2.0 * (1.0 - diffuseColor) * (1.0 - colorOverlay);
	}
	*/

	// Add
	diffuseColor += colorAdd;

	// Gray
	diffuseColor = mix(diffuseColor, vec3(gray), colorGrayFactor);

	gl_FragColor = vec4(diffuseColor, 1.0);
}
]]>
</Source>
</Shader>
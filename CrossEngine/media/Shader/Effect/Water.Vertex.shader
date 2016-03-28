<Shader type="vertex">
<Parameters>
	<Uniform1f name="scale" scope="Local" value="0.015"/>
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


ATTRIBUTE_HIGHP vec3 _position;
ATTRIBUTE_MEDIUMP vec3 _normal;
ATTRIBUTE_MEDIUMP vec2 _texcoord1;

UNIFORM_HIGHP vec3 _cameraPosition;
UNIFORM_HIGHP mat4 _modelWorldMatrix;
UNIFORM_HIGHP mat4 _modelViewProjectionMatrix;
UNIFORM_HIGHP mat4 _textureMatrix0;
UNIFORM_MEDIUMP float scale;

VARYING_HIGHP vec3 vNormal;
VARYING_HIGHP vec3 vViewDirection;
VARYING_MEDIUMP vec2 vTexcoord0;
VARYING_MEDIUMP vec2 vTexcoord1;


void main()
{
	vNormal = (_modelWorldMatrix * vec4(_normal, 0.0)).xyz;
	vViewDirection = (_modelWorldMatrix * vec4(_position, 1.0)).xyz - _cameraPosition;

	vTexcoord0 = (_textureMatrix0 * vec4(_position.xz * scale, 0.0, 1.0)).xy;
	vTexcoord1 = _texcoord1;

	gl_Position = _modelViewProjectionMatrix * vec4(_position, 1.0);
}
]]>
</Source>
</Shader>
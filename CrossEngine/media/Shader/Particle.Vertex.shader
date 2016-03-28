<Shader type="vertex">
<Parameters>
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
ATTRIBUTE_LOWP vec4 _color;
ATTRIBUTE_MEDIUMP vec2 _texcoord0;

UNIFORM_HIGHP mat4 _modelViewProjectionMatrix;
UNIFORM_HIGHP mat4 _textureMatrix0;

VARYING_LOWP vec4 vColor;
VARYING_MEDIUMP vec2 vTexcoord;


void main()
{
	vColor = _color;
	vTexcoord = (_textureMatrix0 * vec4(_texcoord0, 0.0, 1.0)).xy;
	gl_Position = _modelViewProjectionMatrix * vec4(_position, 1.0);
}
]]>
</Source>
</Shader>
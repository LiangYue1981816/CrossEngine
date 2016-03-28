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
ATTRIBUTE_MEDIUMP vec3 _normal;
ATTRIBUTE_MEDIUMP vec3 _binormal;
ATTRIBUTE_MEDIUMP vec2 _texcoord0;
ATTRIBUTE_LOWP vec4 _color;

UNIFORM_HIGHP mat4 _modelViewMatrix;
UNIFORM_HIGHP mat4 _modelViewInverseTransposeMatrix;
UNIFORM_HIGHP mat4 _modelViewProjectionMatrix;
UNIFORM_HIGHP mat4 _textureMatrix0;

VARYING_LOWP vec4 vColor;
VARYING_MEDIUMP vec2 vTexcoord;
VARYING_MEDIUMP vec3 vHalfVector;


void main()
{
	vColor = _color;
	vTexcoord = (_textureMatrix0 * vec4(_texcoord0, 0.0, 1.0)).xy;

	MEDIUMP vec3 viewNormal = (_modelViewInverseTransposeMatrix * vec4(_normal, 0.0)).xyz;
	viewNormal = normalize(viewNormal);

	MEDIUMP vec3 viewBinormal = (_modelViewInverseTransposeMatrix * vec4(_binormal, 0.0)).xyz;
	viewBinormal = normalize(viewBinormal);

	MEDIUMP vec3 t = cross(viewBinormal, viewNormal);
	MEDIUMP vec3 b = cross(viewNormal, t);
	MEDIUMP mat3 tbn = mat3(t, b, viewNormal);

	HIGHP vec3 viewPosition = (_modelViewMatrix * vec4(_position, 1.0)).xyz;
	MEDIUMP vec3 positionVector = normalize(viewPosition);

	MEDIUMP vec3 lightVector = vec3(0.0, 0.0, 1.0);
	MEDIUMP vec3 halfVector = normalize(lightVector - positionVector);
	vHalfVector = halfVector * tbn;

	gl_Position = _modelViewProjectionMatrix * vec4(_position, 1.0);
}
]]>
</Source>
</Shader>
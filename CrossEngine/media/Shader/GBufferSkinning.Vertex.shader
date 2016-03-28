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

#define MAX_BONES 55


ATTRIBUTE_HIGHP vec3 _position;
ATTRIBUTE_MEDIUMP vec4 _indices;
ATTRIBUTE_MEDIUMP vec4 _weights;

UNIFORM_HIGHP mat4 _modelViewMatrix;
UNIFORM_HIGHP mat4 _modelViewProjectionMatrix;
UNIFORM_HIGHP mat4 _boneMatrices[MAX_BONES];

VARYING_HIGHP vec3 vPosition;


void main()
{
	MEDIUMP ivec4 indices = ivec4(_indices);
	HIGHP vec3 skinPosition = vec3(0.0, 0.0, 0.0);

	skinPosition += (_boneMatrices[indices.x] * vec4(_position, 1.0)).xyz * _weights.x;
	skinPosition += (_boneMatrices[indices.y] * vec4(_position, 1.0)).xyz * _weights.y;
	vPosition = (_modelViewMatrix * vec4(skinPosition, 1.0)).xyz;

	gl_Position = _modelViewProjectionMatrix * vec4(skinPosition, 1.0);
}
]]>
</Source>
</Shader>
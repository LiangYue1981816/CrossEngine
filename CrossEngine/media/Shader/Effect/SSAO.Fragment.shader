<Shader type="fragment">
<Parameters>
	<Uniform1i name="depthmap" scope="Local" value="0"/>
	<Uniform1i name="noisemap" scope="Local" value="1"/>
	<Uniform1i name="ssaoSampleInScreenSpace" scope="Global" value="1"/>
	<Uniform1f name="ssaoSampleLengthScale" scope="Global" value="0.01"/>
	<Uniform1f name="ssaoSampleLengthWorldSpace" scope="Global" value="100.0"/>
	<Uniform1f name="ssaoSampleLengthScreenSpace" scope="Global" value="0.20"/>
	<Uniform1f name="ssaoHighLight" scope="Global" value="1.85"/>
	<Uniform1f name="ssaoAccessibility" scope="Global" value="0.5"/>
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


UNIFORM_HIGHP vec4 _viewportParam; // x, y, width, height
UNIFORM_HIGHP vec4 _cameraPerspectiveParam; // fovy, aspect, znear, zfar

UNIFORM_MEDIUMP sampler2D depthmap;
UNIFORM_MEDIUMP sampler2D noisemap;

UNIFORM_LOWP int ssaoSampleInScreenSpace; // whether to sample in screen or world space
UNIFORM_MEDIUMP float ssaoSampleLengthScale; // [0, 1] The distance of the first sample.
UNIFORM_MEDIUMP float ssaoSampleLengthWorldSpace; // the sample length in world space in units
UNIFORM_MEDIUMP float ssaoSampleLengthScreenSpace; // The sample length in screen space [0, 1]
UNIFORM_MEDIUMP float ssaoHighLight; // multiplier for edge highlighting in [1, 2] 1 is full highlighting 2 is off
UNIFORM_MEDIUMP float ssaoAccessibility; // the default value used in the lerp() expression for invalid samples [0, 1]

VARYING_MEDIUMP vec2 vTexcoord;

const LOWP float sampleCount = 32.0;


HIGHP float DncodeFloatRGBA(HIGHP vec4 rgba)
{
	const HIGHP vec4 magic = vec4(1.0, 1.0/255.0, 1.0/65025.0, 1.0/16581375.0);
	return dot(rgba, magic);
}

void main()
{
	// compute the distance between the clipping planes to convert [0, 1] depth to world space units
	HIGHP float clipRange = _cameraPerspectiveParam.w - _cameraPerspectiveParam.z;

	// get the depth of the current pixel and convert into world space unit [0, inf]
	HIGHP float currDepth = DncodeFloatRGBA(texture2D(depthmap, vTexcoord)) * clipRange;

	// get rotation vector, rotation is tiled every 4 screen pixels [-1, 1]x[-1, 1]x[-1, 1]
	MEDIUMP vec2 rotationTC = vTexcoord * _viewportParam.zw / 4.0;
	MEDIUMP vec3 rotationVector = 2.0 * texture2D(noisemap, rotationTC).xyz - 1.0;

	// radius of influence in world space
	MEDIUMP float r = 0.0; // radius of influence in world space
	MEDIUMP float rUV = 0.0; // radius of influence in screen space

	if (ssaoSampleInScreenSpace == 1)
	{
		r = tan(ssaoSampleLengthScreenSpace * _cameraPerspectiveParam.x) * currDepth;
		rUV = ssaoSampleLengthScreenSpace;
	}
	else
	{
		r = ssaoSampleLengthWorldSpace;
		rUV = atan(ssaoSampleLengthWorldSpace / currDepth) / _cameraPerspectiveParam.x;
	}

	// sample the sphere and accumulate ao
	MEDIUMP float ao = 0.0;
	MEDIUMP float sampleLength = ssaoSampleLengthScale;
	MEDIUMP float sampleLengthStep = pow((rUV / ssaoSampleLengthScale), 1.0 / sampleCount);

	for (LOWP int i = 0; i < int(sampleCount / 8.0); i++)
	{
		for (MEDIUMP int x = -1; x <= 1; x += 2)
		for (MEDIUMP int y = -1; y <= 1; y += 2)
		for (MEDIUMP int z = -1; z <= 1; z += 2)
		{
			// generate offset vector
			MEDIUMP vec3 offset = normalize(vec3(x, y, z)) * sampleLength;

			// reflect offset vector by random rotation sample (i.e. rotating it) 
			MEDIUMP vec3 rotatedOffset = reflect(offset, rotationVector);
			MEDIUMP vec2 sampleTC = vTexcoord + rotatedOffset.xy * rUV;

			// read scene depth at sampling point and convert into world space units (m or whatever)
			HIGHP float sampleDepth = DncodeFloatRGBA(texture2D(depthmap, sampleTC)) * clipRange;

			// check if depths of both pixels are close enough and sampling point should affect our center pixel
			HIGHP float isRangeInvalid = clamp((currDepth - sampleDepth) / r, 0.0, 1.0);

			// accumulate ao, use default value of 0.5 if right computations are not possible
			ao += mix(float(sampleDepth > (currDepth + rotatedOffset.z * r)), ssaoAccessibility, isRangeInvalid);

			// update sample length
			sampleLength *= sampleLengthStep;
		}
	}

	// get average value
	ao /= sampleCount;

	// normalize, remove edge highlighting
	ao *= ssaoHighLight;

	// amplify and saturate if necessary
	gl_FragColor = vec4(ao, ao, ao, 1.0);
}
]]>
</Source>
</Shader>
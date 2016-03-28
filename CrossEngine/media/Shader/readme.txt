内置变量
attribute vec3 _position;
attribute vec3 _normal;
attribute vec3 _binormal;
attribute vec4 _color;
attribute vec2 _texcoord0;
attribute vec2 _texcoord1;
attribute vec2 _texcoord2;
attribute vec2 _texcoord3;
attribute vec4 _indices;
attribute vec4 _weights;

uniform vec4 _viewportParam; // x, y, width, height

uniform vec4 _cameraPerspectiveParam; // fovy, aspect, znear, zfar
uniform vec3 _cameraPosition;
uniform vec3 _cameraForwardDirection;
uniform vec3 _cameraUpDirection;

uniform mat4 _modelWorldMatrix;
uniform mat4 _modelViewMatrix;
uniform mat4 _modelViewInverseTransposeMatrix;
uniform mat4 _modelViewProjectionMatrix;
uniform mat4 _textureMatrix0;
uniform mat4 _textureMatrix1;
uniform mat4 _textureMatrix2;
uniform mat4 _textureMatrix3;
uniform mat4 _boneMatrices;

uniform vec4 _fogParam; // mode, start, end, density
uniform vec3 _fogColor;

uniform int _lightCount; // <= MAX_LIGHTS
uniform vec4 _lightPosition;
uniform vec3 _lightAmbient;
uniform vec3 _lightDiffuse;
uniform vec3 _lightSpecular;
uniform vec3 _lightRimColor;
uniform vec3 _lightLowerSkyColor;
uniform vec3 _lightUpperSkyColor;
uniform vec3 _lightAttenuation; // k0, k1, k2

uniform int _shadowReceiver;
uniform vec3 _shadowLightDirection;
uniform mat4 _shadowLightProjectionMatrix;

uniform vec2 _godRayScreenPosition;


全局参数
Base
<Uniform1f name="envmapFactor" scope="Global" value="0.1"/>
<Uniform1f name="lightmapFactor" scope="Global" value="1.2"/>
<Uniform1f name="specularmapFactor" scope="Global" value="1.0"/>
<Uniform1f name="shadowBias" scope="Global" value="0.00025"/>
<Uniform1f name="shadowEpsilon" scope="Global" value="0.00025"/>
<Uniform1f name="shadowValue" scope="Global" value="0.5"/>

Bloom
<Uniform1f name="bloomBrightLuminance" scope="Global" value="0.08"/>

GodRay
<Uniform1f name="godRayLength" scope="Global" value="0.5"/>
<Uniform1f name="godRayFactor" scope="Global" value="0.45"/>
<Uniform1f name="godRayBlendFactor" scope="Global" value="1.0"/>

Distortion
<Uniform1f name="distortionRange" scope="Global" value="0.05"/>

RadialBlur
<Uniform1f name="radialBlurSampleRadius" scope="Global" value="0.25"/>
<Uniform1f name="radialBlurSampleStrength" scope="Global" value="3.0"/>

MotionBlur
<Uniform1f name="motionBlurFactor" scope="Global" value="0.9"/>

SSAO
<Uniform1i name="ssaoSampleInScreenSpace" scope="Global" value="1"/>
<Uniform1f name="ssaoSampleLengthScale" scope="Global" value="0.01"/>
<Uniform1f name="ssaoSampleLengthWorldSpace" scope="Global" value="100.0"/>
<Uniform1f name="ssaoSampleLengthScreenSpace" scope="Global" value="0.20"/>
<Uniform1f name="ssaoHighLight" scope="Global" value="1.85"/>
<Uniform1f name="ssaoAccessibility" scope="Global" value="0.5"/>

Color
<Uniform1f name="colorThreshold" scope="Global" value="0.0"/>
<Uniform1f name="colorContrast" scope="Global" value="1.0"/>
<Uniform1f name="colorGrayFactor" scope="Global" value="0.0"/>
<Uniform3f name="colorAdd" scope="Global" value="0.0 0.0 0.0"/>
<Uniform3f name="colorOverlay" scope="Global" value="0.5 0.5 0.5"/>
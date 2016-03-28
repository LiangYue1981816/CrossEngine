@rem GL_ES
@rem ENABLE_SKINNING
@rem ENABLE_FOG
@rem ENABLE_LIGHTING_UNIT0
@rem ENABLE_LIGHTING_UNIT1
@rem ENABLE_DIFFUSELIGHTING
@rem ENABLE_SPECULARING
@rem ENABLE_RIMLIGHTING
@rem ENABLE_SKYLIGHTING
@rem ENABLE_AMBIENT_OCCLUSION
@rem ENABLE_ENVMAP
@rem ENABLE_LIGHTMAP
@rem ENABLE_NORMALMAP
@rem ENABLE_SPECULARMAP
@rem ENABLE_SHADOWMAP
@rem ENABLE_SHADOWMAP_PCF
@rem ENABLE_SHADOWMAP_PCF2X2
@rem ENABLE_MRT_COLOR_DEPTH
@rem ENABLE_OPTIONAL_NORMALIZE



set DEFINES=-DENABLE_SKINNING -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oSkinning.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oSkinning.Fragment.shader Default.Fragment.source

set DEFINES=-DENABLE_OPTIONAL_NORMALIZE -DENABLE_SKINNING -DENABLE_LIGHTING_UNIT0 -DENABLE_DIFFUSELIGHTING -DENABLE_SKYLIGHTING -DENABLE_RIMLIGHTING -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oSkinningLighting.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oSkinningLighting.Fragment.shader Default.Fragment.source

set DEFINES=-DENABLE_SKINNING -DENABLE_LIGHTING_UNIT0 -DENABLE_LIGHTING_UNIT1 -DENABLE_DIFFUSELIGHTING -DENABLE_SKYLIGHTING -DENABLE_RIMLIGHTING -DENABLE_SPECULARING -DENABLE_NORMALMAP -DENABLE_SPECULARMAP -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oSkinningLightingNormalMapSpecularMap.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oSkinningLightingNormalMapSpecularMap.Fragment.shader Default.Fragment.source

set DEFINES=-DENABLE_SKINNING -DENABLE_LIGHTING_UNIT0 -DENABLE_LIGHTING_UNIT1 -DENABLE_DIFFUSELIGHTING -DENABLE_SKYLIGHTING -DENABLE_RIMLIGHTING -DENABLE_SPECULARING -DENABLE_NORMALMAP -DENABLE_SPECULARMAP -DENABLE_SHADOWMAP -DENABLE_SHADOWMAP_PCF2X2 -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oSkinningLightingNormalMapSpecularMapShadowMap.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oSkinningLightingNormalMapSpecularMapShadowMap.Fragment.shader Default.Fragment.source


set DEFINES=-DENABLE_OPTIONAL_NORMALIZE -DENABLE_LIGHTING_UNIT0 -DENABLE_DIFFUSELIGHTING -DENABLE_SKYLIGHTING -DENABLE_RIMLIGHTING -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oLighting.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oLighting.Fragment.shader Default.Fragment.source

set DEFINES=-DENABLE_LIGHTING_UNIT0 -DENABLE_LIGHTING_UNIT1 -DENABLE_DIFFUSELIGHTING -DENABLE_SKYLIGHTING -DENABLE_RIMLIGHTING -DENABLE_SPECULARING -DENABLE_NORMALMAP -DENABLE_SPECULARMAP -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oLightingNormalMapSpecularMap.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oLightingNormalMapSpecularMap.Fragment.shader Default.Fragment.source

set DEFINES=-DENABLE_LIGHTING_UNIT0 -DENABLE_LIGHTING_UNIT1 -DENABLE_DIFFUSELIGHTING -DENABLE_SKYLIGHTING -DENABLE_RIMLIGHTING -DENABLE_SPECULARING -DENABLE_NORMALMAP -DENABLE_SPECULARMAP -DENABLE_SHADOWMAP -DENABLE_SHADOWMAP_PCF2X2 -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oLightingNormalMapSpecularMapShadowMap.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oLightingNormalMapSpecularMapShadowMap.Fragment.shader Default.Fragment.source


set DEFINES=-DENABLE_LIGHTMAP -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oLightMap.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oLightMap.Fragment.shader Default.Fragment.source

set DEFINES=-DENABLE_LIGHTMAP -DENABLE_SHADOWMAP -DENABLE_SHADOWMAP_PCF2X2 -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oLightMapShadowMap.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oLightMapShadowMap.Fragment.shader Default.Fragment.source

set DEFINES=-DENABLE_OPTIONAL_NORMALIZE -DENABLE_LIGHTMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_DIFFUSELIGHTING -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oLightingLightMap.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oLightingLightMap.Fragment.shader Default.Fragment.source

set DEFINES=-DENABLE_OPTIONAL_NORMALIZE -DENABLE_LIGHTMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_DIFFUSELIGHTING -DENABLE_SHADOWMAP -DENABLE_SHADOWMAP_PCF2X2 -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oLightingLightMapShadowMap.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oLightingLightMapShadowMap.Fragment.shader Default.Fragment.source

@rem set DEFINES=-DENABLE_LIGHTMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_LIGHTING_UNIT1 -DENABLE_DIFFUSELIGHTING -DENABLE_MRT_COLOR_DEPTH
@rem cpp32.exe -C -Sr %DEFINES% -oLightingLightMapNormalMapSpecularMap.Vertex.shader Default.Vertex.source
@rem cpp32.exe -C -Sr %DEFINES% -oLightingLightMapNormalMapSpecularMap.Fragment.shader Default.Fragment.source

@rem set DEFINES=-DENABLE_LIGHTMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_LIGHTING_UNIT1 -DENABLE_DIFFUSELIGHTING -DENABLE_SHADOWMAP -DENABLE_SHADOWMAP_PCF2X2 -DENABLE_MRT_COLOR_DEPTH
@rem cpp32.exe -C -Sr %DEFINES% -oLightingLightMapNormalMapSpecularMapShadowMap.Vertex.shader Default.Vertex.source
@rem cpp32.exe -C -Sr %DEFINES% -oLightingLightMapNormalMapSpecularMapShadowMap.Fragment.shader Default.Fragment.source



@rem set DEFINES=-DENABLE_SKINNING -DENABLE_ENVMAP -DENABLE_MRT_COLOR_DEPTH
@rem cpp32.exe -C -Sr %DEFINES% -oSkinningEnvMap.Vertex.shader Default.Vertex.source
@rem cpp32.exe -C -Sr %DEFINES% -oSkinningEnvMap.Fragment.shader Default.Fragment.source

@rem set DEFINES=-DENABLE_OPTIONAL_NORMALIZE -DENABLE_SKINNING -DENABLE_ENVMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_DIFFUSELIGHTING -DENABLE_SKYLIGHTING -DENABLE_RIMLIGHTING -DENABLE_MRT_COLOR_DEPTH
@rem cpp32.exe -C -Sr %DEFINES% -oSkinningLightingEnvMap.Vertex.shader Default.Vertex.source
@rem cpp32.exe -C -Sr %DEFINES% -oSkinningLightingEnvMap.Fragment.shader Default.Fragment.source

@rem set DEFINES=-DENABLE_SKINNING -DENABLE_ENVMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_LIGHTING_UNIT1 -DENABLE_DIFFUSELIGHTING -DENABLE_SKYLIGHTING -DENABLE_RIMLIGHTING -DENABLE_SPECULARING -DENABLE_NORMALMAP -DENABLE_SPECULARMAP -DENABLE_MRT_COLOR_DEPTH
@rem cpp32.exe -C -Sr %DEFINES% -oSkinningLightingEnvMapNormalMapSpecularMap.Vertex.shader Default.Vertex.source
@rem cpp32.exe -C -Sr %DEFINES% -oSkinningLightingEnvMapNormalMapSpecularMap.Fragment.shader Default.Fragment.source

@rem set DEFINES=-DENABLE_SKINNING -DENABLE_ENVMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_LIGHTING_UNIT1 -DENABLE_DIFFUSELIGHTING -DENABLE_SKYLIGHTING -DENABLE_RIMLIGHTING -DENABLE_SPECULARING -DENABLE_NORMALMAP -DENABLE_SPECULARMAP -DENABLE_SHADOWMAP -DENABLE_SHADOWMAP_PCF2X2 -DENABLE_MRT_COLOR_DEPTH
@rem cpp32.exe -C -Sr %DEFINES% -oSkinningLightingEnvMapNormalMapSpecularMapShadowMap.Vertex.shader Default.Vertex.source
@rem cpp32.exe -C -Sr %DEFINES% -oSkinningLightingEnvMapNormalMapSpecularMapShadowMap.Fragment.shader Default.Fragment.source


@rem set DEFINES=-DENABLE_OPTIONAL_NORMALIZE -DENABLE_ENVMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_DIFFUSELIGHTING -DENABLE_SKYLIGHTING -DENABLE_RIMLIGHTING -DENABLE_MRT_COLOR_DEPTH
@rem cpp32.exe -C -Sr %DEFINES% -oLightingEnvMap.Vertex.shader Default.Vertex.source
@rem cpp32.exe -C -Sr %DEFINES% -oLightingEnvMap.Fragment.shader Default.Fragment.source

@rem set DEFINES=-DENABLE_ENVMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_LIGHTING_UNIT1 -DENABLE_DIFFUSELIGHTING -DENABLE_SKYLIGHTING -DENABLE_RIMLIGHTING -DENABLE_SPECULARING -DENABLE_NORMALMAP -DENABLE_SPECULARMAP -DENABLE_MRT_COLOR_DEPTH
@rem cpp32.exe -C -Sr %DEFINES% -oLightingEnvMapNormalMapSpecularMap.Vertex.shader Default.Vertex.source
@rem cpp32.exe -C -Sr %DEFINES% -oLightingEnvMapNormalMapSpecularMap.Fragment.shader Default.Fragment.source

@rem set DEFINES=-DENABLE_ENVMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_LIGHTING_UNIT1 -DENABLE_DIFFUSELIGHTING -DENABLE_SKYLIGHTING -DENABLE_RIMLIGHTING -DENABLE_SPECULARING -DENABLE_NORMALMAP -DENABLE_SPECULARMAP -DENABLE_SHADOWMAP -DENABLE_SHADOWMAP_PCF2X2 -DENABLE_MRT_COLOR_DEPTH
@rem cpp32.exe -C -Sr %DEFINES% -oLightingEnvMapNormalMapSpecularMapShadowMap.Vertex.shader Default.Vertex.source
@rem cpp32.exe -C -Sr %DEFINES% -oLightingEnvMapNormalMapSpecularMapShadowMap.Fragment.shader Default.Fragment.source


set DEFINES=-DENABLE_ENVMAP -DENABLE_LIGHTMAP -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oEnvMapLightMap.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oEnvMapLightMap.Fragment.shader Default.Fragment.source

set DEFINES=-DENABLE_ENVMAP -DENABLE_LIGHTMAP -DENABLE_SHADOWMAP -DENABLE_SHADOWMAP_PCF2X2 -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oEnvMapLightMapShadowMap.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oEnvMapLightMapShadowMap.Fragment.shader Default.Fragment.source

set DEFINES=-DENABLE_OPTIONAL_NORMALIZE -DENABLE_ENVMAP -DENABLE_LIGHTMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_DIFFUSELIGHTING -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oLightingEnvMapLightMap.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oLightingEnvMapLightMap.Fragment.shader Default.Fragment.source

set DEFINES=-DENABLE_OPTIONAL_NORMALIZE -DENABLE_ENVMAP -DENABLE_LIGHTMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_DIFFUSELIGHTING -DENABLE_SHADOWMAP -DENABLE_SHADOWMAP_PCF2X2 -DENABLE_MRT_COLOR_DEPTH
cpp32.exe -C -Sr %DEFINES% -oLightingEnvMapLightMapShadowMap.Vertex.shader Default.Vertex.source
cpp32.exe -C -Sr %DEFINES% -oLightingEnvMapLightMapShadowMap.Fragment.shader Default.Fragment.source

@rem set DEFINES=-DENABLE_ENVMAP -DENABLE_LIGHTMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_LIGHTING_UNIT1 -DENABLE_DIFFUSELIGHTING -DENABLE_MRT_COLOR_DEPTH
@rem cpp32.exe -C -Sr %DEFINES% -oLightingEnvMapLightMapNormalMapSpecularMap.Vertex.shader Default.Vertex.source
@rem cpp32.exe -C -Sr %DEFINES% -oLightingEnvMapLightMapNormalMapSpecularMap.Fragment.shader Default.Fragment.source

@rem set DEFINES=-DENABLE_ENVMAP -DENABLE_LIGHTMAP -DENABLE_LIGHTING_UNIT0 -DENABLE_LIGHTING_UNIT1 -DENABLE_DIFFUSELIGHTING -DENABLE_SHADOWMAP -DENABLE_SHADOWMAP_PCF2X2 -DENABLE_MRT_COLOR_DEPTH
@rem cpp32.exe -C -Sr %DEFINES% -oLightingEnvMapLightMapNormalMapSpecularMapShadowMap.Vertex.shader Default.Vertex.source
@rem cpp32.exe -C -Sr %DEFINES% -oLightingEnvMapLightMapNormalMapSpecularMapShadowMap.Fragment.shader Default.Fragment.source



del ..\..\..\Bin\Media\Shader\*.* /q
copy *.shader ..\..\..\Bin\Media\Shader /y
copy .\Effect\*.shader ..\..\..\Bin\Media\Shader /y
@rem pause
/****************************************************************************
Copyright (c) 2015 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#pragma once


#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "time.h"
#include "math.h"
#include "ctype.h"
#include "memory.h"
#include "assert.h"
#include "pthread.h"
#include "semaphore.h"
#include "tinyxml.h"
#include "map"
#include "vector"
#include "string"
#include "algorithm"
#include "lua.h"
#include "picoc.h"
#include "fmod.h"
#include "fmod_errors.h"
#include "zzip/zzip.h"


#if defined _PLATFORM_WINDOWS_

	#include "winsock2.h"
	#include "windows.h"

	#include "glew/glew.h"
	#include "glew/wglew.h"

#elif defined _PLATFORM_ANDROID_

	#include "dirent.h"
	#include "unistd.h"
	#include "fnmatch.h"
	#include "sys/types.h"
	#include "sys/stat.h"
	#include "sys/time.h"

	#define GL_GLEXT_PROTOTYPES

	#include "GLESHeader/GLES2/gl2.h"
	#include "GLESHeader/GLES2/gl2ext.h"
	#include "GLESHeader/GLES3/gl3.h"
	#include "GLESHeader/GLES3/gl3ext.h"

	#include <jni.h>
	#include <string.h>
	#include <android/log.h>

	#define LOG_TAG    "Engine"
	#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
	#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
	#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#elif defined _PLATFORM_IOS_

	#include "dirent.h"
	#include "unistd.h"
	#include "fnmatch.h"
	#include "sys/types.h"
	#include "sys/stat.h"
	#include "sys/time.h"

	#include "GLESHeader/GLES2/gl2.h"
	#include "GLESHeader/GLES2/gl2ext.h"
	#include "GLESHeader/GLES3/gl3.h"
	#include "GLESHeader/GLES3/gl3ext.h"

#else

	#pragma message("Not support platform!")

#endif


#include "CrossType.h"
#include "CrossC.h"
#include "CrossLog.h"
#include "CrossMemory.h"
#include "CrossUtility.h"
#include "CrossProfiler.h"
#include "CrossMath.h"
#include "CrossMathD3D.h"
#include "CrossList.h"
#include "CrossStream.h"
#include "CrossSharedPtr.h"

#include "CrossOpenGL.h"
#include "CrossRenderer.h"
#include "CrossRenderQuene.h"
#include "CrossRenderFlow.h"
#include "CrossRenderSystem.h"
#include "CrossSoundSystem.h"
#include "CrossPhysics.h"
#include "CrossPhysicsManager.h"

#include "CrossResource.h"
#include "CrossResourceManager.h"
#include "CrossResourceSystem.h"
#include "CrossShader.h"
#include "CrossTexture.h"
#include "CrossMaterial.h"
#include "CrossSkeleton.h"
#include "CrossMesh.h"
#include "CrossSound.h"
#include "CrossParticle.h"
#include "CrossParticleGfx.h"
#include "CrossParticleGfxDebug.h"
#include "CrossParticleGfxBillboard.h"
#include "CrossParticleEmitter.h"
#include "CrossParticleEmitterBox.h"
#include "CrossParticleEmitterPoint.h"
#include "CrossParticleEmitterSphere.h"
#include "CrossParticleAffector.h"
#include "CrossParticleAffectorColor.h"
#include "CrossParticleAffectorForce.h"
#include "CrossParticleAffectorKeepLocal.h"
#include "CrossParticleAffectorRandom.h"
#include "CrossParticleAffectorRotate.h"
#include "CrossParticleAffectorScale.h"
#include "CrossParticleAffectorTexture.h"
#include "CrossParticleAffectorVelocity.h"
#include "CrossParticleSystem.h"
#include "CrossEffect.h"
#include "CrossShaderManager.h"
#include "CrossTextureManager.h"
#include "CrossMaterialManager.h"
#include "CrossSkeletonManager.h"
#include "CrossMeshManager.h"
#include "CrossSoundManager.h"
#include "CrossEffectManager.h"

#include "CrossEntity.h"
#include "CrossEntityRenderable.h"
#include "CrossEntityLight.h"
#include "CrossEntityCamera.h"
#include "CrossEntityMesh.h"
#include "CrossEntitySkin.h"
#include "CrossEntityEffect.h"
#include "CrossEntityHalo.h"
#include "CrossEntityShadow.h"
#include "CrossLightGrid.h"
#include "CrossOctreeCulling.h"
#include "CrossOcclusionCulling.h"
#include "CrossSceneNode.h"
#include "CrossSceneManager.h"
#include "CrossSceneSystem.h"


#define SHADER_EXT_NAME      ".shader"
#define TEXTURE_EXT_NAME0    ".bmp"
#define TEXTURE_EXT_NAME1    ".dds"
#define TEXTURE_EXT_NAME2    ".pvr"
#define MATERIAL_EXT_NAME    ".material"
#define SKELETON_EXT_NAME    ".skeleton"
#define MESH_EXT_NAME        ".mesh"
#define EFFECT_EXT_NAME      ".effect"
#define HALO_EXT_NAME        ".halo"
#define SOUND_EXT_NAME0      ".wav"
#define SOUND_EXT_NAME1      ".ogg"


#define Engine()             CrossEngine::CEngine::GetEngine()

#define ResourceSystem()     CrossEngine::CEngine::GetEngine()->GetResourceSystem()
#define SoundManager()       CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::CResourceSystem::RESOURCE_MANAGER_SOUND)
#define ShaderManager()      CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::CResourceSystem::RESOURCE_MANAGER_SHADER)
#define TextureManager()     CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::CResourceSystem::RESOURCE_MANAGER_TEXTURE)
#define MaterialManager()    CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::CResourceSystem::RESOURCE_MANAGER_MATERIAL)
#define SkeletonManager()    CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::CResourceSystem::RESOURCE_MANAGER_SKELETON)
#define MeshManager()        CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::CResourceSystem::RESOURCE_MANAGER_MESH)
#define EffectManager()      CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::CResourceSystem::RESOURCE_MANAGER_EFFECT)
#define HaloManager()        CrossEngine::CEngine::GetEngine()->GetResourceSystem()->GetResourceManager(CrossEngine::CResourceSystem::RESOURCE_MANAGER_HALO)

#define SceneSystem()        CrossEngine::CEngine::GetEngine()->GetSceneSystem()
#define SceneManager(name)   CrossEngine::CEngine::GetEngine()->GetSceneSystem()->GetSceneManager(name)
#define SceneCamera(name)    CrossEngine::CEngine::GetEngine()->GetSceneSystem()->GetSceneManager(name)->GetMainCameraEntity()

#define RenderSystem()       CrossEngine::CEngine::GetEngine()->GetRenderSystem()
#define Renderer()           CrossEngine::CEngine::GetEngine()->GetRenderSystem()->GetRenderer()
#define RenderFlow()         CrossEngine::CEngine::GetEngine()->GetRenderSystem()->GetRenderFlow()
#define RenderQuene()        CrossEngine::CEngine::GetEngine()->GetRenderSystem()->GetRenderQuene()

#define SoundSystem()        CrossEngine::CEngine::GetEngine()->GetSoundSystem()


namespace CrossEngine {

	class _CrossExport CEngine
	{
		// 构造/析构函数
	protected:
		CEngine(VOID);
		virtual ~CEngine(VOID);


		// 方法
	public:
		static CEngine* GetEngine(VOID);                                                           // 获得引擎

	public:
		CSoundSystem* GetSoundSystem(VOID) const;                                                  // 获得声音系统
		CRenderSystem* GetRenderSystem(VOID) const;                                                // 获得渲染系统
		CResourceSystem* GetResourceSystem(VOID) const;                                            // 获得资源系统
		CSceneSystem* GetSceneSystem(VOID) const;                                                  // 获得场景系统

	public:
		VOID Init(HWND hWnd, RECT rcView, DWORD dwSoundMemPoolSize, INT maxChannels, const CHAR *szLogFileName, BOOL bEditorMode); // 初始化引擎
		VOID Exit(VOID);                                                                           // 退出引擎

	protected:
		BOOL CreateSoundSystem(DWORD dwMemPoolSize, INT maxChannels);                              // 创建声音系统
		BOOL CreateRenderSystem(HWND hWnd, RECT rcView);                                           // 创建渲染系统
		BOOL CreateResourceSystem(VOID);                                                           // 创建资源系统
		BOOL CreateSceneSystem(VOID);                                                              // 创建场景系统

		VOID DestroySoundSystem(VOID);                                                             // 设置声音系统
		VOID DestroyRenderSystem(VOID);                                                            // 设置渲染系统
		VOID DestroyResourceSystem(VOID);                                                          // 创建资源系统
		VOID DestroySceneSystem(VOID);                                                             // 创建场景系统

	public:
		BOOL IsEditorMode(VOID) const;                                                             // 编辑器模式

	public:
		FLOAT GetTime(VOID) const;                                                                 // 获得时间
		DWORD GetFrameCount(VOID) const;                                                           // 获得帧计数

	public:
		BOOL LoadResourceZip(const CHAR *szZipName);                                               // 加载资源包资源
		BOOL LoadResourcePath(const CHAR *szPathName);                                             // 加载文件夹资源

		BOOL ReloadAll(VOID);                                                                      // 重新加载所有资源
		BOOL ReloadGfx(VOID);                                                                      // 重新加载图形资源

		VOID GarbageCollection(VOID);                                                              // 垃圾回收

	public:
		const CHAR* GetNextSoundName(VOID) const;                                                  // 获得新声音名
		const CHAR* GetNextShaderName(VOID) const;                                                 // 获得新着色器名
		const CHAR* GetNextTextureName(VOID) const;                                                // 获得新纹理名
		const CHAR* GetNextMaterialName(VOID) const;                                               // 获得新材质名
		const CHAR* GetNextSkeletonName(VOID) const;                                               // 获得新骨骼名
		const CHAR* GetNextMeshName(VOID) const;                                                   // 获得新网格名
		const CHAR* GetNextEffectName(VOID) const;                                                 // 获得新特效名
		const CHAR* GetNextHaloName(VOID) const;                                                   // 获得新光晕名

	public:
		VOID Pause(VOID);                                                                          // 暂停
		VOID Resume(VOID);                                                                         // 继续
		VOID Update(const CHAR *szSceneManager, FLOAT deltaTime);                                  // 更新
		VOID Update(DWORD dwSceneManager, FLOAT deltaTime);                                        // 更新

	public:
		VOID Render(const CHAR *szSceneManager, const CHAR *szSolution);                           // 渲染
		VOID Render(DWORD dwSceneManager, DWORD dwSolution);                                       // 渲染
		VOID SwapBuffers(VOID);                                                                    // 交换缓冲


		// 属性
	protected:
		BOOL m_bEditorMode;                                                                        // 编辑器模式

	protected:
		FLOAT m_time;                                                                              // 时间
		DWORD m_dwFrameCount;                                                                      // 帧计数

	protected:
		CSoundSystem *m_pSoundSystem;                                                              // 声音系统
		CRenderSystem *m_pRenderSystem;                                                            // 渲染系统
		CResourceSystem *m_pResourceSystem;                                                        // 资源系统
		CSceneSystem *m_pSceneSystem;                                                              // 场景系统

	protected:
		static CEngine sInstance;                                                                  // 引擎实例
	};

}

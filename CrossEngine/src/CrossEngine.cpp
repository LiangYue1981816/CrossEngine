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

#include "_CrossEngine.h"


namespace CrossEngine {

	CEngine CEngine::sInstance;

	CEngine::CEngine(VOID)
		: m_bEditorMode(FALSE)

		, m_time(0.0f)
		, m_dwFrameCount(0)

		, m_pSoundSystem(NULL)
		, m_pRenderSystem(NULL)
		, m_pResourceSystem(NULL)
		, m_pSceneSystem(NULL)
	{

	}

	CEngine::~CEngine(VOID)
	{

	}

	//
	// 获得引擎
	//
	CEngine* CEngine::GetEngine(VOID)
	{
		return &sInstance;
	}

	//
	// 获得声音系统
	//
	CSoundSystem* CEngine::GetSoundSystem(VOID) const
	{
		return m_pSoundSystem;
	}

	//
	// 获得渲染系统
	//
	CRenderSystem* CEngine::GetRenderSystem(VOID) const
	{
		return m_pRenderSystem;
	}

	//
	// 获得资源系统
	//
	CResourceSystem* CEngine::GetResourceSystem(VOID) const
	{
		return m_pResourceSystem;
	}

	//
	// 获得场景系统
	//
	CSceneSystem* CEngine::GetSceneSystem(VOID) const
	{
		return m_pSceneSystem;
	}

	//
	// 初始化引擎
	//
	VOID CEngine::Init(HWND hWnd, RECT rcView, DWORD dwSoundMemPoolSize, INT maxChannels, const CHAR *szLogFileName, BOOL bEditorMode)
	{
		//
		// 1. 参数安全检查
		//
		if (hWnd == NULL) {
			return;
		}

		//
		// 2. 设置编辑器模式
		//
		m_bEditorMode = bEditorMode;

		//
		// 3. 设置日至文件
		//
		EnableLog(TRUE, szLogFileName);

		//
		// 4. 分配引擎各个系统
		//
		CreateSoundSystem(dwSoundMemPoolSize, maxChannels);
		CreateRenderSystem(hWnd, rcView);
		CreateResourceSystem();
		CreateSceneSystem();

		InitPhysics();
	}

	//
	// 退出引擎
	//
	VOID CEngine::Exit(VOID)
	{
		DestroySceneSystem();
		DestroyResourceSystem();
		DestroyRenderSystem();
		DestroySoundSystem();

		ExitPhysics();
	}

	//
	// 创建声音系统
	//
	BOOL CEngine::CreateSoundSystem(DWORD dwMemPoolSize, INT maxChannels)
	{
		if (m_pSoundSystem == NULL) {
			m_pSoundSystem = (CSoundSystem *)SAFE_NEW(MEMTYPE_HEAP) CSoundSystem(dwMemPoolSize, maxChannels);
			ASSERT(m_pSoundSystem);
		}

		return TRUE;
	}

	//
	// 创建渲染系统
	//
	BOOL CEngine::CreateRenderSystem(HWND hWnd, RECT rcView)
	{
		if (m_pRenderSystem == NULL) {
			m_pRenderSystem = (CRenderSystem *)SAFE_NEW(MEMTYPE_HEAP) CRenderSystem(hWnd);
			ASSERT(m_pRenderSystem);
			ASSERT(m_pRenderSystem->GetRenderer());

			m_pRenderSystem->GetRenderer()->SetViewport(rcView.left, rcView.top, rcView.right - rcView.left + 1, rcView.bottom - rcView.top + 1);
		}

		return TRUE;
	}

	//
	// 创建资源系统
	//
	BOOL CEngine::CreateResourceSystem(VOID)
	{
		if (m_pResourceSystem == NULL) {
			m_pResourceSystem = SAFE_NEW(MEMTYPE_HEAP) CResourceSystem;
			ASSERT(m_pResourceSystem);
		}

		return TRUE;
	}

	//
	// 创建场景系统
	//
	BOOL CEngine::CreateSceneSystem(VOID)
	{
		if (m_pSceneSystem == NULL) {
			m_pSceneSystem = SAFE_NEW(MEMTYPE_HEAP) CSceneSystem;
			ASSERT(m_pSceneSystem);
		}

		return TRUE;
	}

	//
	// 设置声音系统
	//
	VOID CEngine::DestroySoundSystem(VOID)
	{
		SAFE_DELETE(m_pSoundSystem);
	}

	//
	// 设置渲染系统
	//
	VOID CEngine::DestroyRenderSystem(VOID)
	{
		SAFE_DELETE(m_pRenderSystem);
	}

	//
	// 创建资源系统
	//
	VOID CEngine::DestroyResourceSystem(VOID)
	{
		SAFE_DELETE(m_pResourceSystem);
	}

	//
	// 创建场景系统
	//
	VOID CEngine::DestroySceneSystem(VOID)
	{
		SAFE_DELETE(m_pSceneSystem);
	}

	//
	// 编辑器模式
	//
	BOOL CEngine::IsEditorMode(VOID) const
	{
		return m_bEditorMode;
	}

	//
	// 获得时间
	//
	FLOAT CEngine::GetTime(VOID) const
	{
		return m_time;
	}

	//
	// 获得帧计数
	//
	DWORD CEngine::GetFrameCount(VOID) const
	{
		return m_dwFrameCount;
	}

	//
	// 加载资源包资源
	//
	BOOL CEngine::LoadResourceZip(const CHAR *szZipName)
	{
		ASSERT(szZipName);
		ASSERT(m_pResourceSystem);
		return m_pResourceSystem->LoadResourceZip(szZipName);
	}

	//
	// 加载文件夹资源
	//
	BOOL CEngine::LoadResourcePath(const CHAR *szPathName)
	{
		ASSERT(szPathName);
		ASSERT(m_pResourceSystem);
		return m_pResourceSystem->LoadResourcePath(szPathName);
	}

	//
	// 重新加载所有资源
	//
	BOOL CEngine::ReloadAll(VOID)
	{
		ASSERT(m_pResourceSystem);
		return m_pResourceSystem->ReloadAll();
	}

	//
	// 重新加载图形资源
	//
	BOOL CEngine::ReloadGfx(VOID)
	{
		ASSERT(m_pResourceSystem);
		return m_pResourceSystem->ReloadGfx();
	}

	//
	// 垃圾回收
	//
	VOID CEngine::GarbageCollection(VOID)
	{
		ASSERT(m_pResourceSystem);
		m_pResourceSystem->GarbageCollection();
	}

	//
	// 获得新声音名
	//
	const CHAR* CEngine::GetNextSoundName(VOID) const
	{
		static DWORD dwCount = 0;
		static CHAR szName[_MAX_STRING];

		sprintf(szName, "SOUND_%d", dwCount++);
		return szName;
	}

	//
	// 获得新着色器名
	//
	const CHAR* CEngine::GetNextShaderName(VOID) const
	{
		static DWORD dwCount = 0;
		static CHAR szName[_MAX_STRING];

		sprintf(szName, "SHADER_%d", dwCount++);
		return szName;
	}

	//
	// 获得新纹理名
	//
	const CHAR* CEngine::GetNextTextureName(VOID) const
	{
		static DWORD dwCount = 0;
		static CHAR szName[_MAX_STRING];

		sprintf(szName, "TEXTURE_%d", dwCount++);
		return szName;
	}

	//
	// 获得新材质名
	//
	const CHAR* CEngine::GetNextMaterialName(VOID) const
	{
		static DWORD dwCount = 0;
		static CHAR szName[_MAX_STRING];

		sprintf(szName, "MATERIAL_%d", dwCount++);
		return szName;
	}

	//
	// 获得新骨骼名
	//
	const CHAR* CEngine::GetNextSkeletonName(VOID) const
	{
		static DWORD dwCount = 0;
		static CHAR szName[_MAX_STRING];

		sprintf(szName, "SKELETON_%d", dwCount++);
		return szName;
	}

	//
	// 获得新网格名
	//
	const CHAR* CEngine::GetNextMeshName(VOID) const
	{
		static DWORD dwCount = 0;
		static CHAR szName[_MAX_STRING];

		sprintf(szName, "MESH_%d", dwCount++);
		return szName;
	}

	//
	// 获得新特效名
	//
	const CHAR* CEngine::GetNextEffectName(VOID) const
	{
		static DWORD dwCount = 0;
		static CHAR szName[_MAX_STRING];

		sprintf(szName, "EFFECT_%d", dwCount++);
		return szName;
	}

	//
	// 获得新光晕名
	//
	const CHAR* CEngine::GetNextHaloName(VOID) const
	{
		static DWORD dwCount = 0;
		static CHAR szName[_MAX_STRING];

		sprintf(szName, "HALO_%d", dwCount++);
		return szName;
	}

	//
	// 暂停
	//
	VOID CEngine::Pause(VOID)
	{
		ASSERT(m_pSoundSystem);
		m_pSoundSystem->Pause();
	}

	//
	// 继续
	//
	VOID CEngine::Resume(VOID)
	{
		ASSERT(m_pSoundSystem);
		m_pSoundSystem->Resume();
	}

	//
	// 更新
	//
	VOID CEngine::Update(const CHAR *szSceneManager, FLOAT deltaTime)
	{
		ASSERT(szSceneManager);
		Update(HashValue(szSceneManager), deltaTime);
	}

	VOID CEngine::Update(DWORD dwSceneManager, FLOAT deltaTime)
	{
		ASSERT(m_pSoundSystem);
		ASSERT(m_pRenderSystem);
		ASSERT(m_pResourceSystem);
		ASSERT(m_pSceneSystem);

		//
		// 1. 更新帧计数
		//
		m_time += deltaTime;
		m_dwFrameCount++;

		//
		// 2. 更新场景系统
		//
		m_pSceneSystem->Update(dwSceneManager, deltaTime);

		//
		// 3. 更新声音系统
		//
		m_pSoundSystem->Update(deltaTime);
	}

	//
	// 渲染
	//
	VOID CEngine::Render(const CHAR *szSceneManager, const CHAR *szSolution)
	{
		ASSERT(szSolution);
		ASSERT(szSceneManager);
		Render(HashValue(szSceneManager), HashValue(szSolution));
	}

	VOID CEngine::Render(DWORD dwSceneManager, DWORD dwSolution)
	{
		ASSERT(m_pRenderSystem);
		ASSERT(m_pRenderSystem->GetRenderFlow());
		m_pRenderSystem->GetRenderFlow()->Render(dwSceneManager, dwSolution);
	}

	//
	// 交换缓冲
	//
	VOID CEngine::SwapBuffers(VOID)
	{
		ASSERT(m_pRenderSystem);
		ASSERT(m_pRenderSystem->GetRenderer());
		m_pRenderSystem->GetRenderer()->SwapBuffers();
	}

}

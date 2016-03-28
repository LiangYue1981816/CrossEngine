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

	CSceneSystem::CSceneSystem(VOID)
	{
		m_sceneManagers.clear();
		m_sceneManagersPlugin.clear();
	}

	CSceneSystem::~CSceneSystem(VOID)
	{
		for (SceneManagerMap::const_iterator itSceneManager = m_sceneManagers.begin(); itSceneManager != m_sceneManagers.end(); ++itSceneManager) {
			const CSceneManager *pSceneManager = itSceneManager->second;
			ASSERT(pSceneManager);

			SAFE_DELETE(pSceneManager);
		}

		m_sceneManagers.clear();
		m_sceneManagersPlugin.clear();
	}

	//
	// ���볡��������
	//
	BOOL CSceneSystem::PluginSceneManager(const CHAR *szName, CSceneManager *pSceneManager)
	{
		ASSERT(szName);
		ASSERT(pSceneManager);

		return PluginSceneManager(HashValue(szName), pSceneManager);
	}

	//
	// ���볡��������
	//
	BOOL CSceneSystem::PluginSceneManager(DWORD dwName, CSceneManager *pSceneManager)
	{
		ASSERT(pSceneManager);

		//
		// 1. ���ҳ���������
		//
		SceneManagerMap::const_iterator itSceneManager = m_sceneManagers.find(dwName);
		if (itSceneManager != m_sceneManagers.end()) return FALSE;

		SceneManagerMap::const_iterator itSceneManagerPlugin = m_sceneManagersPlugin.find(dwName);
		if (itSceneManagerPlugin != m_sceneManagersPlugin.end()) return FALSE;

		//
		// 2. ���볡��������
		//
		m_sceneManagersPlugin[dwName] = pSceneManager;

		return TRUE;
	}

	//
	// ��������������
	//
	CSceneManager* CSceneSystem::CreateSceneManager(const CHAR *szName)
	{
		ASSERT(szName);
		return CreateSceneManager(HashValue(szName));
	}

	//
	// ��������������
	//
	CSceneManager* CSceneSystem::CreateSceneManager(DWORD dwName)
	{
		//
		// 1. ���ҳ���������
		//
		SceneManagerMap::const_iterator itSceneManager = m_sceneManagers.find(dwName);
		if (itSceneManager != m_sceneManagers.end()) return itSceneManager->second;

		SceneManagerMap::const_iterator itSceneManagerPlugin = m_sceneManagersPlugin.find(dwName);
		if (itSceneManagerPlugin != m_sceneManagersPlugin.end()) return itSceneManagerPlugin->second;

		//
		// 2. ��������������
		//
		CSceneManager *pSceneManager = SAFE_NEW(MEMTYPE_HEAP) CSceneManager;
		ASSERT(pSceneManager);

		m_sceneManagers[dwName] = pSceneManager;

		return pSceneManager;
	}

	//
	// ���ٳ���������
	//
	VOID CSceneSystem::DestroySceneManager(const CHAR *szName)
	{
		ASSERT(szName);
		DestroySceneManager(HashValue(szName));
	}

	//
	// ���ٳ���������
	//
	VOID CSceneSystem::DestroySceneManager(DWORD dwName)
	{
		//
		// 1. ���ҳ���������
		//
		SceneManagerMap::iterator itSceneManager = m_sceneManagers.find(dwName);
		if (itSceneManager == m_sceneManagers.end()) return;

		//
		// 2. ɾ������������
		//
		const CSceneManager *pSceneManager = itSceneManager->second;
		ASSERT(pSceneManager);

		SAFE_DELETE(pSceneManager);
		m_sceneManagers.erase(itSceneManager);
	}

	//
	// ��ó���������
	//
	CSceneManager* CSceneSystem::GetSceneManager(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetSceneManager(HashValue(szName));
	}

	//
	// ��ó���������
	//
	CSceneManager* CSceneSystem::GetSceneManager(DWORD dwName) const
	{
		SceneManagerMap::const_iterator itSceneManager = m_sceneManagers.find(dwName);
		if (itSceneManager != m_sceneManagers.end()) return itSceneManager->second;

		SceneManagerMap::const_iterator itSceneManagerPlugin = m_sceneManagersPlugin.find(dwName);
		if (itSceneManagerPlugin != m_sceneManagersPlugin.end()) return itSceneManagerPlugin->second;

		return NULL;
	}

	//
	// ����
	//
	VOID CSceneSystem::Update(DWORD dwName, FLOAT deltaTime)
	{
		if (CSceneManager *pSceneManager = GetSceneManager(dwName)) {
			pSceneManager->Update(deltaTime);
		}
	}

}

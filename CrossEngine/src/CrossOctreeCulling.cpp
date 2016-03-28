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

	COctreeCulling::COctreeCulling(CSceneManager *pSceneManager)
		: m_pRoot(NULL)
		, m_pSceneManager(pSceneManager)
	{

	}

	COctreeCulling::~COctreeCulling(VOID)
	{
		Free();
	}

	//
	// ��ʼ��
	//
	VOID COctreeCulling::Init(VOID)
	{
		m_pRoot = NULL;
	}

	//
	// �ͷ�
	//
	VOID COctreeCulling::Free(VOID)
	{
		//
		// 1. �ͷŰ˲���
		//
		if (m_pRoot) {
			Free(m_pRoot);
		}

		//
		// 2. ��ʼ��
		//
		Init();
	}

	//
	// �ͷ�
	//
	VOID COctreeCulling::Free(NODE *pNode)
	{
		ASSERT(pNode);

		//
		// 1. �ͷ��ӽڵ�
		//
		if (pNode->pNodes[0]) Free(pNode->pNodes[0]);
		if (pNode->pNodes[1]) Free(pNode->pNodes[1]);
		if (pNode->pNodes[2]) Free(pNode->pNodes[2]);
		if (pNode->pNodes[3]) Free(pNode->pNodes[3]);
		if (pNode->pNodes[4]) Free(pNode->pNodes[4]);
		if (pNode->pNodes[5]) Free(pNode->pNodes[5]);
		if (pNode->pNodes[6]) Free(pNode->pNodes[6]);
		if (pNode->pNodes[7]) Free(pNode->pNodes[7]);

		//
		// 2. �ͷŵ�ǰ�ڵ�
		//
		SAFE_DELETE(pNode);
	}

	//
	// ��������������Դ
	//
	BOOL COctreeCulling::LoadFromStream(CStream *pStream)
	{
		return TRUE;
	}

	//
	// �����޳�
	//
	VOID COctreeCulling::UpdateCulling(const CEntityCamera *pCamera) const
	{
		ASSERT(pCamera);

		//
		// 1. �˲����޳�
		//
		if (m_pRoot) {
			OctreeCulling(pCamera, m_pRoot);
		}
		//
		// 2. ����޳�
		//
		else {
			CameraCulling(pCamera, m_pSceneManager->GetRootNode());
		}
	}

	//
	// �˲����޳�
	//
	VOID COctreeCulling::OctreeCulling(const CEntityCamera *pCamera, NODE *pNode) const
	{
		ASSERT(pNode);
		ASSERT(pCamera);

		//
		// 1. ��ǰ�ڵ�����ɼ��Լ��
		//
		if (pCamera->IsVisible(&pNode->aabb) == FALSE) {
			return;
		}

		//
		// 2. ���õ�ǰ�ڵ�ɼ���
		//
		for (std::vector<CSceneNode*>::const_iterator itNode = pNode->nodes.begin(); itNode != pNode->nodes.end(); ++itNode) {
			CSceneNode *pSceneNode = *itNode;
			ASSERT(pSceneNode);

			if (pSceneNode->IsRenderable()) continue;
			if (pSceneNode->IsVisible() == FALSE) continue;
			if (pCamera->IsVisible(pSceneNode->GetAABBCurrent()) == FALSE) continue;

			pSceneNode->SetRenderable(TRUE, FALSE);
		}

		//
		// 3. �ӽڵ��޳�
		//
		if (pNode->pNodes[0]) OctreeCulling(pCamera, pNode->pNodes[0]);
		if (pNode->pNodes[1]) OctreeCulling(pCamera, pNode->pNodes[1]);
		if (pNode->pNodes[2]) OctreeCulling(pCamera, pNode->pNodes[2]);
		if (pNode->pNodes[3]) OctreeCulling(pCamera, pNode->pNodes[3]);
		if (pNode->pNodes[4]) OctreeCulling(pCamera, pNode->pNodes[4]);
		if (pNode->pNodes[5]) OctreeCulling(pCamera, pNode->pNodes[5]);
		if (pNode->pNodes[6]) OctreeCulling(pCamera, pNode->pNodes[6]);
		if (pNode->pNodes[7]) OctreeCulling(pCamera, pNode->pNodes[7]);
	}

	//
	// ����޳�
	//
	VOID COctreeCulling::CameraCulling(const CEntityCamera *pCamera, CSceneNode *pSceneNode) const
	{
		ASSERT(pCamera);
		ASSERT(pSceneNode);

		//
		// 1. ��ǰ�ڵ�ɼ��Լ��
		//
		if (pSceneNode->IsVisible() == FALSE) {
			return;
		}

		//
		// 2. ��ǰ�ڵ�����ɼ��Լ��
		//
		if (pCamera->IsVisible(pSceneNode->GetAABBCascade()) == FALSE) {
			return;
		}

		//
		// 3. ���õ�ǰ�ڵ�ɼ���
		//
		if (pCamera->IsVisible(pSceneNode->GetAABBCurrent()) == TRUE) {
			pSceneNode->SetRenderable(TRUE, FALSE);
		}

		//
		// 4. �ӽڵ��޳�
		//
		if (CSceneNode *pChildSceneNode = pSceneNode->GetNodeListHead()) {
			do {
				CameraCulling(pCamera, pChildSceneNode);
			} while (pChildSceneNode = pChildSceneNode->pNext);
		}
	}

}

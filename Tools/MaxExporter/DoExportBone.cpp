#include "MaxExporter.h"


//
// ���������ṹ
//
void CModel::DoExportBoneHierarchy(void)
{
	//
	// 1. ��������ṹ�洢
	//
	m_pBoneHierarchy = new CBoneFrame;
	_ASSERT(m_pBoneHierarchy);

	//
	// 2. ���������ṹ
	//    ע��: �Ե�0���״̬��Ϊ�����ṹ
	//
	m_pGameScene->SetStaticFrame(0);

	for (int indexNode = 0; indexNode < m_pGameScene->GetTopLevelNodeCount(); indexNode++) {
		IGameNode *pNode = m_pGameScene->GetTopLevelNode(indexNode);
		_ASSERT(pNode);

		DoExportBone(pNode, NULL, NULL, m_pBoneHierarchy);
	}

	//
	// 3. �����ؼ�֡
	//
	DoExportKeyFrames();
}

//
// ������������
//
void CModel::DoExportAnimSkeleton(BOOL bExportKeyFrame)
{
	//
	// 1. ����ÿ������
	//
	for (vector<CAnimSkeleton*>::const_iterator itAnim = m_animSkeletons.begin(); itAnim != m_animSkeletons.end(); ++itAnim) {
		CAnimSkeleton *pAnim = *itAnim;
		_ASSERT(pAnim);

		//
		// 2. ��������ÿһ��
		//
		for (int indexFrame = pAnim->pAnimInfo->startFrame; indexFrame <= pAnim->pAnimInfo->endFrame; indexFrame++) {
			if (bExportKeyFrame) {
				map<int, int>::const_iterator itKeyFrame = m_keyFrames.find(indexFrame);
				if (itKeyFrame == m_keyFrames.end()) continue;
			}

			CBoneFrame *pBoneFrame = new CBoneFrame(1.0f*(indexFrame - pAnim->pAnimInfo->startFrame)/pAnim->pAnimInfo->fps);
			_ASSERT(pBoneFrame);

			m_pGameScene->SetStaticFrame(indexFrame);

			for (int indexNode = 0; indexNode < m_pGameScene->GetTopLevelNodeCount(); indexNode++) {
				IGameNode *pNode = m_pGameScene->GetTopLevelNode(indexNode);
				_ASSERT(pNode);

				DoExportBone(pNode, NULL, NULL, pBoneFrame);
			}

			pAnim->frames.push_back(pBoneFrame);
		}
	}
}

//
// ���������
// ע��: �ú���������ǰ��, ��ǰ����SetStaticFrame()��������
//
void CModel::DoExportBone(IGameNode *pNode, IGameNode *pParentNode, CBone *pParentBone, CBoneFrame *pBoneFrame)
{
	_ASSERT(pNode);
	_ASSERT(pBoneFrame);

	//
	// 1. ���ڵ�����
	//
	if (pNode->GetIGameObject()->GetIGameType() != IGameObject::IGAME_BONE && 
		pNode->GetIGameObject()->GetIGameType() != IGameObject::IGAME_HELPER) {
		return;
	}

	m_boneNodes.push_back(pNode);

	//
	// 2. ��������
	//
	CBone *pBone = new CBone(pNode);
	_ASSERT(pBone);

	sprintf(pBone->szName, "%s", pNode->GetName());

	if (pParentNode) {
		sprintf(pBone->szParentName, "%s", pParentNode->GetName());
	}
	else {
		sprintf(pBone->szParentName, "Scene");
	}

	ReplaceInvalidChar(pBone->szName);
	ReplaceInvalidChar(pBone->szParentName);

	pBone->pParentBone = pParentBone;

	pBone->isRoot = pParentNode ? FALSE : TRUE;
	pBone->isDummy = pNode->GetIGameObject()->GetIGameType() == IGameObject::IGAME_HELPER ? TRUE : FALSE;

	pBone->matLocal = pNode->GetLocalTM();
	pBone->matWorld = pNode->GetWorldTM();

	pBone->id = pBoneFrame->bones.size();
	pBoneFrame->bones.push_back(pBone);

	if (pParentBone) {
		pParentBone->childBones.push_back(pBone);
	}

	//
	// 3. ���ùҽڵ��������Ч
	//
	if (pBone->isDummy) {
		pBone->SetUsed();
	}

	//
	// 4. �����ӹ���
	//
	for (int indexChildNode = 0; indexChildNode < pNode->GetChildCount(); indexChildNode++) {
		IGameNode *pChildNode = pNode->GetNodeChild(indexChildNode);
		_ASSERT(pChildNode);

		DoExportBone(pChildNode, pNode, pBone, pBoneFrame);
	}
}

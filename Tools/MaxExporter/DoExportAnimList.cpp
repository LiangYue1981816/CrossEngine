#include "MaxExporter.h"


//
// �����ؼ�֡
//
void CModel::DoExportKeyFrames(void)
{
	for (vector<IGameNode*>::const_iterator itBoneNode = m_boneNodes.begin(); itBoneNode != m_boneNodes.end(); ++itBoneNode) {
		IGameControl *pGameControl = (*itBoneNode)->GetIGameControl();

		// IGAME_POS
		// IGAME_ROT
		// IGAME_SCALE
		// IGAME_FLOAT
		// IGAME_POINT3
		// IGAME_TM

		// IGAME_EULER_X
		// IGAME_EULER_Y
		// IGAME_EULER_Z

		// IGAME_POINT4

		// IGAME_POS_X
		// IGAME_POS_Y
		// IGAME_POS_Z

		IGameKeyTab keys;
		bool b = pGameControl->GetQuickSampledKeys(keys, IGameControlType(IGAME_POS));

		for (int index = 0; index < keys.Count(); index++) {
			int t = keys[index].t/m_pGameScene->GetSceneTicks();
			m_keyFrames[t] = t;
		}
	}
}

//
// ���������б�
//
void CModel::DoExportAnimList(void)
{
	//
	// 1. ����Max�ļ�������, �ռ�������Ϣ
	//
	for (int index = 0; index < m_pInterface->GetNumProperties(PROPSET_USERDEFINED); index++) {
		//
		// 1.1. ������Թؼ��ֺ�ֵ
		//
		const PROPSPEC *pPropSpec = m_pInterface->GetPropertySpec(PROPSET_USERDEFINED, index);
		const PROPVARIANT *pPropVar = m_pInterface->GetPropertyVariant(PROPSET_USERDEFINED, index);

		if (pPropVar->vt != VT_LPWSTR && pPropVar->vt != VT_LPSTR) {
			continue;
		}

		char szKey[_MAX_PATH];
		char szValue[_MAX_PATH];

		sprintf(szKey, "%s", TSTR(pPropSpec->lpwstr));
		sprintf(szValue, "%s", pPropVar->pszVal);

		//
		// 1.2. ���㶯�����
		//
		if (CAnimationInfo *pAnimInfo = DoExportAnimInfo(szKey, szValue, VERTEX_ANIM_KEYWORD)) {
			CAnimVertex *pAnim = new CAnimVertex;
			_ASSERT(pAnim);

			pAnim->pAnimInfo = pAnimInfo;
			m_animVertexs.push_back(pAnim);
		}

		//
		// 1.3. �����������
		//
		if (CAnimationInfo *pAnimInfo = DoExportAnimInfo(szKey, szValue, SKELETON_ANIM_KEYWORD)) {
			CAnimSkeleton *pAnim = new CAnimSkeleton;
			_ASSERT(pAnim);

			m_keyFrames[pAnimInfo->startFrame] = pAnimInfo->startFrame;
			m_keyFrames[pAnimInfo->endFrame] = pAnimInfo->endFrame;

			pAnim->pAnimInfo = pAnimInfo;
			m_animSkeletons.push_back(pAnim);
		}
	}
}

//
// ����������Ϣ
//
CAnimationInfo* CModel::DoExportAnimInfo(const char *szKey, const char *szValue, const char *szKeyWorld)
{
	//
	// 1. ���ؼ���
	//
	if (strnicmp(szKey, szKeyWorld, strlen(szKeyWorld))) {
		return NULL;
	}

	//
	// 2. ������������
	//
	char szAnimName[_MAX_PATH];
	int fps = 30;
	int startFrame = 0;
	int endFrame = 0;

	if (!ParseAnimationParams(szValue, szAnimName, &startFrame, &endFrame, &fps)) {
		return NULL;
	}

	int sceneStartFrame = m_pGameScene->GetSceneStartTime()/m_pGameScene->GetSceneTicks();
	int sceneEndFrame = m_pGameScene->GetSceneEndTime()/m_pGameScene->GetSceneTicks();

	if (startFrame < sceneStartFrame) startFrame = sceneStartFrame;
	if (endFrame > sceneEndFrame) endFrame = sceneEndFrame;
	if (endFrame < startFrame) endFrame = startFrame;

	//
	// 3. ���춯����Ϣ
	//
	CAnimationInfo *pAnimInfo = new CAnimationInfo;
	_ASSERT(pAnimInfo);

	sprintf(pAnimInfo->szName, "%s", szAnimName);
	pAnimInfo->fps = fps;
	pAnimInfo->startFrame = startFrame;
	pAnimInfo->endFrame = endFrame;

	return pAnimInfo;
}

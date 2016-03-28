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

	CAnimationState::CAnimationState(const CSkeletonPtr &ptrSkeleton, INT indexAnim)
		: m_pAnimation(NULL)
		, m_weights(NULL)
	{
		ASSERT(ptrSkeleton.IsNull() == FALSE && ptrSkeleton->IsValid());

		//
		// 1. ���ù���
		//
		m_ptrSkeleton = ptrSkeleton;
		m_pAnimation = const_cast<CAnimSkeleton *>(m_ptrSkeleton->GetAnimation(indexAnim)); ASSERT(m_pAnimation);
		m_weights = (FLOAT *)SAFE_MALLOC(m_ptrSkeleton->GetBoneCount() * sizeof(*m_weights), MEMTYPE_HEAP); ASSERT(m_weights);

		//
		// 2. ���ö���״̬
		//
		SetWeight(1.0f);
		SetBeginTime(0.0f);
		SetEndTime(m_pAnimation->GetLength());

		Stop();
		SetEnable(FALSE);
	}

	CAnimationState::~CAnimationState(VOID)
	{
		m_ptrSkeleton.SetNull();
		SAFE_FREE(m_weights);
	}

	//
	// ��ö�����
	//
	const CHAR* CAnimationState::GetName(VOID) const
	{
		return m_pAnimation->GetName();
	}

	//
	// ��ö�����
	//
	DWORD CAnimationState::GetHashName(VOID) const
	{
		return m_pAnimation->GetHashName();
	}

	//
	// ��ù�����
	//
	INT CAnimationState::GetBoneCount(VOID) const
	{
		return m_ptrSkeleton->GetBoneCount();
	}

	//
	// ���ʱ�䳤��
	//
	FLOAT CAnimationState::GetLength(VOID) const
	{
		return m_pAnimation->GetLength();
	}

	//
	// ��ùǼܽṹ
	//
	const BONE_HIERARCHY* CAnimationState::GetSkeletonHierarchy(VOID) const
	{
		return m_ptrSkeleton->GetBoneHierarchy();
	}

	//
	// ���ö���Ȩ��
	//
	VOID CAnimationState::SetWeight(FLOAT weight)
	{
		//
		// 1. ������ȫ���
		//
		if (weight < EPSILON_E3) {
			weight = 0.0f;
		}

		//
		// 2. ���ö���Ȩ��
		//
		for (INT indexBone = 0; indexBone < m_ptrSkeleton->GetBoneCount(); indexBone++) {
			m_weights[indexBone] = weight;
		}
	}

	//
	// ���ƶ���Ȩ��
	//
	VOID CAnimationState::ModulateWeight(FLOAT weight)
	{
		//
		// 1. ������ȫ���
		//
		if (weight < EPSILON_E3) {
			weight = 0.0f;
		}

		//
		// 2. ���ƶ���Ȩ��
		//
		for (INT indexBone = 0; indexBone < m_ptrSkeleton->GetBoneCount(); indexBone++) {
			m_weights[indexBone] *= weight;
		}
	}

	//
	// ���ù���Ȩ��
	//
	VOID CAnimationState::SetBoneWeightByName(const CHAR *szName, FLOAT weight)
	{
		ASSERT(szName);
		return SetBoneWeightByName(HashValue(szName), weight);
	}

	//
	// ���ù���Ȩ��
	//
	VOID CAnimationState::SetBoneWeightByName(DWORD dwName, FLOAT weight)
	{
		//
		// 1. ������ȫ���
		//
		if (weight < EPSILON_E3) {
			weight = 0.0f;
		}

		//
		// 2. ���ù���Ȩ��
		//
		for (INT indexBone = 0; indexBone < m_ptrSkeleton->GetBoneCount(); indexBone++) {
			if (m_ptrSkeleton->GetBoneHierarchy()->pBoneNames[indexBone].dwName == dwName) {
				m_weights[indexBone] = weight;
				return;
			}
		}
	}

	//
	// ���ù���Ȩ��
	//
	VOID CAnimationState::SetBoneWeightByIndex(INT indexBone, FLOAT weight)
	{
		//
		// 1. ������ȫ���
		//
		if (indexBone < 0 || indexBone >= m_ptrSkeleton->GetBoneCount()) {
			return;
		}

		if (weight < EPSILON_E3) {
			weight = 0.0f;
		}

		//
		// 2. ���ù���Ȩ��
		//
		m_weights[indexBone] = weight;
	}

	//
	// ��ù���Ȩ��
	//
	FLOAT CAnimationState::GetBoneWeightByName(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetBoneWeightByName(HashValue(szName));
	}

	//
	// ��ù���Ȩ��
	//
	FLOAT CAnimationState::GetBoneWeightByName(DWORD dwName) const
	{
		for (INT indexBone = 0; indexBone < m_ptrSkeleton->GetBoneCount(); indexBone++) {
			if (m_ptrSkeleton->GetBoneHierarchy()->pBoneNames[indexBone].dwName == dwName) {
				return m_weights[indexBone];
			}
		}

		return 0.0f;
	}

	//
	// ��ù���Ȩ��
	//
	FLOAT CAnimationState::GetBoneWeightByIndex(INT indexBone) const
	{
		//
		// 1. ������ȫ���
		//
		if (indexBone < 0 || indexBone >= m_ptrSkeleton->GetBoneCount()) {
			return 0.0f;
		}

		//
		// 2. ��ù���Ȩ��
		//
		return m_weights[indexBone];
	}

	//
	// ���ò����ٶ�
	//
	VOID CAnimationState::SetSpeed(FLOAT speed)
	{
		m_speed = speed;
	}

	//
	// ��õ�ǰʱ��λ��
	//
	VOID CAnimationState::SetTimePosition(FLOAT t)
	{
		m_currTime = t;
		m_forwardTime = t;
		m_backwardTime = t;
	}

	//
	// ���ÿ�ʼʱ��
	//
	VOID CAnimationState::SetBeginTime(FLOAT beginTime)
	{
		//
		// 1. ������ȫ���
		//
		if (beginTime < 0.0f || beginTime > m_pAnimation->GetLength()) {
			return;
		}

		//
		// 2. ���ÿ�ʼʱ��
		//
		m_beginTime = beginTime < m_endTime ? beginTime : m_endTime;
	}

	//
	// ������ֹʱ��
	//
	VOID CAnimationState::SetEndTime(FLOAT endTime)
	{
		//
		// 1. ������ȫ���
		//
		if (endTime < 0.0f || endTime > m_pAnimation->GetLength()) {
			return;
		}

		//
		// 2. ������ֹʱ��
		//
		m_endTime = endTime > m_beginTime ? endTime : m_beginTime;
	}

	//
	// ��ò����ٶ�
	//
	FLOAT CAnimationState::GetSpeed(VOID) const
	{
		return m_speed;
	}

	//
	// ���õ�ǰʱ��λ��
	//
	FLOAT CAnimationState::GetTimePosition(VOID) const
	{
		return m_currTime;
	}

	//
	// ��ÿ�ʼʱ��
	//
	FLOAT CAnimationState::GetBeginTime(VOID) const
	{
		return m_beginTime;
	}

	//
	// �����ֹʱ��
	//
	FLOAT CAnimationState::GetEndTime(VOID) const
	{
		return m_endTime;
	}

	//
	// ���ö���״̬
	//
	VOID CAnimationState::SetEnable(BOOL bEnable)
	{
		m_bEnable = bEnable;
	}

	//
	// ��ö���״̬
	//
	BOOL CAnimationState::IsEnable(VOID) const
	{
		return m_bEnable;
	}

	//
	// ���Ŷ���
	//
	VOID CAnimationState::Play(BOOL bLoop, FLOAT speed)
	{
		m_bPlaying = TRUE;
		m_bPause = FALSE;
		m_bLoop = bLoop;

		m_speed = speed;

		m_currTime = 0.0f;
		m_forwardTime = m_beginTime;
		m_backwardTime = m_endTime;
	}

	//
	// ֹͣ����
	//
	VOID CAnimationState::Stop(VOID)
	{
		m_bPlaying = FALSE;
		m_bPause = FALSE;
		m_bLoop = FALSE;

		m_speed = 1.0f;

		m_currTime = 0.0f;
		m_forwardTime = m_beginTime;
		m_backwardTime = m_endTime;
	}

	//
	// ��ͣ����
	//
	VOID CAnimationState::Pause(VOID)
	{
		m_bPause = TRUE;
	}

	//
	// ��������
	//
	VOID CAnimationState::Resume(VOID)
	{
		m_bPause = FALSE;
	}

	//
	// ����״̬
	//
	BOOL CAnimationState::IsPlaying(VOID) const
	{
		return m_bEnable && m_bPlaying && m_bPause == FALSE ? TRUE : FALSE;
	}

	//
	// ���µ���һ֡
	//
	VOID CAnimationState::NextFrame(FLOAT deltaTime)
	{
		//
		// 1. ����״̬���
		//
		if (IsPlaying() == FALSE) {
			return;
		}

		//
		// 2. �����ٶȵ���
		//
		deltaTime *= m_speed;

		//
		// 3. ���򲥷�
		//
		if (deltaTime > 0.0f) {
			m_forwardTime += deltaTime;

			if (m_forwardTime > m_endTime) {
				m_forwardTime = m_bLoop ? m_beginTime + m_forwardTime - m_endTime : m_endTime;
			}

			m_currTime = m_backwardTime = m_forwardTime;
		}

		//
		// 4. ���򲥷�
		//
		if (deltaTime < 0.0f) {
			m_backwardTime += deltaTime;

			if (m_backwardTime < m_beginTime) {
				m_backwardTime = m_bLoop ? m_endTime - m_beginTime + m_backwardTime : m_beginTime;
			}

			m_currTime = m_forwardTime = m_backwardTime;
		}
	}

	//
	// ���֡����
	//
	VOID CAnimationState::GetFrameData(BONE *pBones) const
	{
		//
		// 1. ����ؼ�֡����
		//
		INT indexKeyFrame0 = 0;
		INT indexKeyFrame1 = m_pAnimation->GetKeyFrameCount() - 1;

		for (INT indexKeyFrame = 0; indexKeyFrame < m_pAnimation->GetKeyFrameCount(); indexKeyFrame++) {
			if (m_currTime > m_pAnimation->GetKeyFrameTime(indexKeyFrame) - EPSILON_E3) {
				indexKeyFrame0 = indexKeyFrame;
			}
		}

		for (INT indexKeyFrame = m_pAnimation->GetKeyFrameCount() - 1; indexKeyFrame >= 0; indexKeyFrame--) {
			if (m_currTime < m_pAnimation->GetKeyFrameTime(indexKeyFrame) + EPSILON_E3) {
				indexKeyFrame1 = indexKeyFrame;
			}
		}

		//
		// 2. ֱ�ӻ�ùؼ�֡����
		//
		if (indexKeyFrame0 == indexKeyFrame1) {
			memcpy(pBones, m_pAnimation->GetKeyFrameData(indexKeyFrame0), m_ptrSkeleton->GetBoneCount() * sizeof(*pBones));
		}
		//
		// 3. ���ݹؼ�֡���㵱ǰ֡����
		//
		else {
			const BONE *pFrameBones0 = m_pAnimation->GetKeyFrameData(indexKeyFrame0);
			const BONE *pFrameBones1 = m_pAnimation->GetKeyFrameData(indexKeyFrame1);

			FLOAT weight = (m_currTime - m_pAnimation->GetKeyFrameTime(indexKeyFrame0)) / (m_pAnimation->GetKeyFrameTime(indexKeyFrame1) - m_pAnimation->GetKeyFrameTime(indexKeyFrame0));

			for (INT indexBone = 0; indexBone < m_ptrSkeleton->GetBoneCount(); indexBone++) {
				Vec3Lerp(&pBones[indexBone].scale, &pFrameBones0[indexBone].scale, &pFrameBones1[indexBone].scale, weight);
				Vec3Lerp(&pBones[indexBone].position, &pFrameBones0[indexBone].position, &pFrameBones1[indexBone].position, weight);
				QuatSlerp(&pBones[indexBone].orientation, &pFrameBones0[indexBone].orientation, &pFrameBones1[indexBone].orientation, weight);
			}
		}
	}

	CEntitySkin::CEntitySkin(DWORD dwName, CSceneManager *pSceneManager)
		: CEntityMesh(dwName, pSceneManager)

		, m_boneTransforms(NULL)
		, m_boneCustomBlends(NULL)
		, m_boneCustomOrientations(NULL)
	{

	}

	CEntitySkin::~CEntitySkin(VOID)
	{
		SAFE_FREE(m_boneTransforms);
		SAFE_FREE(m_boneCustomBlends);
		SAFE_FREE(m_boneCustomOrientations);

		FreeAnimationStates();
	}

	//
	// �������
	//
	CEntity::TYPE CEntitySkin::GetType(VOID) const
	{
		return CEntity::SKIN;
	}

	//
	// ��ÿ���Ⱦ
	//
	BOOL CEntitySkin::IsRenderable(VOID) const
	{
		return TRUE;
	}

	//
	// ��������
	//
	BOOL CEntitySkin::SetMesh(const CMeshPtr &ptrMesh)
	{
		SAFE_FREE(m_boneTransforms);
		SAFE_FREE(m_boneCustomBlends);
		SAFE_FREE(m_boneCustomOrientations);

		FreeAnimationStates();

		//
		// 1. ��������
		//
		if (CEntityMesh::SetMesh(ptrMesh) == FALSE) {
			return FALSE;
		}

		//
		// 2. ���ù�������
		//
		const CSkeletonPtr &ptrSkeleton = m_ptrMesh->GetSkeleton();

		if (ptrSkeleton.IsNull() == FALSE && ptrSkeleton->IsValid()) {
			m_boneTransforms = (MATRIX4 *)SAFE_MALLOC(ptrSkeleton->GetBoneCount() * sizeof(*m_boneTransforms), MEMTYPE_HEAP);
			m_boneCustomBlends = (BONE_BLEND *)SAFE_MALLOC(ptrSkeleton->GetBoneCount() * sizeof(*m_boneCustomBlends), MEMTYPE_HEAP);
			m_boneCustomOrientations = (QUAT *)SAFE_MALLOC(ptrSkeleton->GetBoneCount() * sizeof(*m_boneCustomOrientations), MEMTYPE_HEAP);
			ASSERT(m_boneTransforms);
			ASSERT(m_boneCustomBlends);
			ASSERT(m_boneCustomOrientations);

			const BONE_HIERARCHY *pBoneHierarchy = ptrSkeleton->GetBoneHierarchy();
			ASSERT(pBoneHierarchy);

			for (INT indexBone = 0; indexBone < ptrSkeleton->GetBoneCount(); indexBone++) {
				MtxIdentity(&m_boneTransforms[indexBone]);

				m_boneCustomBlends[indexBone].weightScale = 0.0f;
				m_boneCustomBlends[indexBone].weightPosition = 0.0f;
				m_boneCustomBlends[indexBone].weightOrientation = 0.0f;
				Vec3Set(&m_boneCustomBlends[indexBone].scale, 1.0f, 1.0f, 1.0f);
				Vec3Zero(&m_boneCustomBlends[indexBone].position);
				QuatZero(&m_boneCustomBlends[indexBone].orientation);
				m_boneCustomBlendMap[pBoneHierarchy->pBoneNames[indexBone].dwName] = &m_boneCustomBlends[indexBone];

				QuatZero(&m_boneCustomOrientations[indexBone]);
				m_boneCustomOrientationsMap[pBoneHierarchy->pBoneNames[indexBone].dwName] = &m_boneCustomOrientations[indexBone];
			}

			AddAnimations(ptrSkeleton);
		}

		return TRUE;
	}

	//
	// ��Ӷ���
	//
	BOOL CEntitySkin::AddAnimations(const CHAR *szName)
	{
		ASSERT(szName);
		return AddAnimations(HashValue(szName));
	}

	//
	// ��Ӷ���
	//
	BOOL CEntitySkin::AddAnimations(DWORD dwName)
	{
		return AddAnimations(SkeletonManager()->QueryResource(dwName));
	}

	//
	// ��Ӷ���
	//
	BOOL CEntitySkin::AddAnimations(const CSkeletonPtr &ptrAnimSkeleton)
	{
		//
		// 1. ������ȫ���
		//
		const CSkeletonPtr &ptrMeshSkeleton = m_ptrMesh->GetSkeleton();

		if (ptrMeshSkeleton.IsNull() || ptrMeshSkeleton->IsValid() == FALSE ||
			ptrAnimSkeleton.IsNull() || ptrAnimSkeleton->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. ��֤�Ǽ�һ����(������, ���ӹ�ϵ, ����������һ��)
		//
		const BONE_HIERARCHY *pMeshBoneHierarchy = ptrMeshSkeleton->GetBoneHierarchy();
		const BONE_HIERARCHY *pAnimBoneHierarchy = ptrAnimSkeleton->GetBoneHierarchy();
		ASSERT(pMeshBoneHierarchy);
		ASSERT(pAnimBoneHierarchy);

		if (ptrMeshSkeleton->GetBoneCount() != ptrAnimSkeleton->GetBoneCount()) {
			return FALSE;
		}

		for (INT indexBone = 0; indexBone < ptrMeshSkeleton->GetBoneCount(); indexBone++) {
			if (pMeshBoneHierarchy->pParentIDs[indexBone] != pAnimBoneHierarchy->pParentIDs[indexBone]) {
				return FALSE;
			}

			if (pMeshBoneHierarchy->pBoneNames[indexBone].dwName != pAnimBoneHierarchy->pBoneNames[indexBone].dwName) {
				return FALSE;
			}
		}

		//
		// 3. ��Ӷ���
		//
		for (INT indexAnim = 0; indexAnim < ptrAnimSkeleton->GetAnimationCount(); indexAnim++) {
			if (m_animationStates.find(ptrAnimSkeleton->GetAnimation(indexAnim)->GetHashName()) == m_animationStates.end()) {
				CAnimationState *pAnimationState = SAFE_NEW(MEMTYPE_HEAP) CAnimationState(ptrAnimSkeleton, indexAnim);
				ASSERT(pAnimationState);

				m_animationStates[pAnimationState->GetHashName()] = pAnimationState;
			}
		}

		return TRUE;
	}

	//
	// ��ö���
	//
	CAnimationState* CEntitySkin::GetAnimationState(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetAnimationState(HashValue(szName));
	}

	//
	// ��ö���
	//
	CAnimationState* CEntitySkin::GetAnimationState(DWORD dwName) const
	{
		AnimationStateMap::const_iterator itAnimState = m_animationStates.find(dwName);
		return itAnimState != m_animationStates.end() ? itAnimState->second : NULL;
	}

	//
	// ������ж���
	//
	const CEntitySkin::AnimationStateMap& CEntitySkin::GetAnimationStates(VOID) const
	{
		return m_animationStates;
	}

	//
	// �ͷŶ�������
	//
	VOID CEntitySkin::FreeAnimationStates(VOID)
	{
		for (AnimationStateMap::const_iterator itAnimState = m_animationStates.begin(); itAnimState != m_animationStates.end(); ++itAnimState) {
			const CAnimationState *pAnimState = itAnimState->second;
			ASSERT(pAnimState);

			SAFE_DELETE(pAnimState);
		}

		m_animationStates.clear();
	}

	//
	// �����Զ���������(ģ������ϵ)
	//
	VOID CEntitySkin::SetBoneCustomBlend(const CHAR *szBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT weightScale, FLOAT weightPosition, FLOAT weightOrientation)
	{
		ASSERT(szBoneName);
		SetBoneCustomBlend(HashValue(szBoneName), scale, position, orientation, weightScale, weightPosition, weightOrientation);
	}

	//
	// �����Զ���������(ģ������ϵ)
	//
	VOID CEntitySkin::SetBoneCustomBlend(DWORD dwBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT weightScale, FLOAT weightPosition, FLOAT weightOrientation)
	{
		//
		// 1. ���ҹ���
		//
		CustomBlendMap::const_iterator itBoneBlend = m_boneCustomBlendMap.find(dwBoneName);
		if (itBoneBlend == m_boneCustomBlendMap.end()) return;

		//
		// 2. �����Զ���������
		//
		itBoneBlend->second->weightScale = weightScale;
		itBoneBlend->second->weightPosition = weightPosition;
		itBoneBlend->second->weightOrientation = weightOrientation;

		if (scale) Vec3Copy(&itBoneBlend->second->scale, scale);
		if (position) Vec3Copy(&itBoneBlend->second->position, position);
		if (orientation) QuatCopy(&itBoneBlend->second->orientation, orientation);
	}

	//
	// ����Զ���������(ģ������ϵ)
	//
	VOID CEntitySkin::GetBoneCustomBlend(const CHAR *szBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT *weightScale, FLOAT *weightPosition, FLOAT *weightOrientation) const
	{
		ASSERT(szBoneName);
		GetBoneCustomBlend(HashValue(szBoneName), scale, position, orientation, weightScale, weightPosition, weightOrientation);
	}

	//
	// ����Զ���������(ģ������ϵ)
	//
	VOID CEntitySkin::GetBoneCustomBlend(DWORD dwBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT *weightScale, FLOAT *weightPosition, FLOAT *weightOrientation) const
	{
		//
		// 1. ���ҹ���
		//
		CustomBlendMap::const_iterator itBoneBlend = m_boneCustomBlendMap.find(dwBoneName);
		if (itBoneBlend == m_boneCustomBlendMap.end()) return;

		//
		// 2. ����Զ���������(ģ������ϵ)
		//
		if (weightScale) *weightScale = itBoneBlend->second->weightScale;
		if (weightPosition) *weightPosition = itBoneBlend->second->weightPosition;
		if (weightOrientation) *weightOrientation = itBoneBlend->second->weightOrientation;

		if (scale) Vec3Copy(scale, &itBoneBlend->second->scale);
		if (position) Vec3Copy(position, &itBoneBlend->second->position);
		if (orientation) QuatCopy(orientation, &itBoneBlend->second->orientation);
	}

	//
	// �����Զ����������(ģ������ϵ)
	//
	VOID CEntitySkin::SetBoneCustomOrientation(const CHAR *szBoneName, QUAT *orientation)
	{
		ASSERT(szBoneName);
		ASSERT(orientation);
		SetBoneCustomOrientation(HashValue(szBoneName), orientation);
	}

	//
	// �����Զ����������(ģ������ϵ)
	//
	VOID CEntitySkin::SetBoneCustomOrientation(DWORD dwBoneName, QUAT *orientation)
	{
		ASSERT(orientation);

		//
		// 1. ���ҹ���
		//
		CustomOrientationMap::const_iterator itBoneOrientation = m_boneCustomOrientationsMap.find(dwBoneName);
		if (itBoneOrientation == m_boneCustomOrientationsMap.end()) return;

		//
		// 2. �����Զ����������
		//
		QuatCopy(itBoneOrientation->second, orientation);
	}

	//
	// ����Զ����������(ģ������ϵ)
	//
	VOID CEntitySkin::GetBoneCustomOrientation(const CHAR *szBoneName, QUAT *orientation) const
	{
		ASSERT(szBoneName);
		GetBoneCustomOrientation(HashValue(szBoneName), orientation);
	}

	//
	// ����Զ����������(ģ������ϵ)
	//
	VOID CEntitySkin::GetBoneCustomOrientation(DWORD dwBoneName, QUAT *orientation) const
	{
		//
		// 1. ���ҹ���
		//
		CustomOrientationMap::const_iterator itBoneOrientation = m_boneCustomOrientationsMap.find(dwBoneName);
		if (itBoneOrientation == m_boneCustomOrientationsMap.end()) return;

		//
		// 2. �����Զ����������
		//
		if (orientation) QuatCopy(orientation, itBoneOrientation->second);
	}

	//
	// ��ñ任����
	//
	MATRIX4* CEntitySkin::GetBoneTransform(INT indexBone, MATRIX4 *mtxTransform) const
	{
		ASSERT(mtxTransform);
		MtxIdentity(mtxTransform);

		//
		// 1. ������ȫ���
		//
		const CSkeletonPtr &ptrSkeleton = m_ptrMesh->GetSkeleton();

		if (ptrSkeleton.IsNull() || ptrSkeleton->IsValid() == FALSE) {
			return mtxTransform;
		}

		if (indexBone < 0 || indexBone >= ptrSkeleton->GetBoneCount()) {
			return mtxTransform;
		}

		//
		// 2. ��ñ任����
		//    ˵��: NextFrame����õ��ı任�����������������ϵ�任��ģ������ϵ, ������Ҫ����ȥ��
		//
		const BONE_HIERARCHY *pBoneHierarchy = ptrSkeleton->GetBoneHierarchy();
		ASSERT(pBoneHierarchy);

		MATRIX4 mtxScale;
		MATRIX4 mtxRotate;
		MATRIX4 mtxTranslate;

		MtxDefScale(&mtxScale, pBoneHierarchy->pBoneWorld2Locals[indexBone].scale[0], pBoneHierarchy->pBoneWorld2Locals[indexBone].scale[1], pBoneHierarchy->pBoneWorld2Locals[indexBone].scale[2]);
		MtxDefTranslate(&mtxTranslate, pBoneHierarchy->pBoneWorld2Locals[indexBone].position[0], pBoneHierarchy->pBoneWorld2Locals[indexBone].position[1], pBoneHierarchy->pBoneWorld2Locals[indexBone].position[2]);
		QuatToMtxRotation(&mtxRotate, &pBoneHierarchy->pBoneWorld2Locals[indexBone].orientation);

		MATRIX4 mtxSR;
		MATRIX4 mtxSRT;
		MATRIX4 mtxSRTInv;

		MtxMul(&mtxSR, &mtxScale, &mtxRotate);
		MtxMul(&mtxSRT, &mtxSR, &mtxTranslate);
		MtxInverse(&mtxSRTInv, &mtxSRT);

		MtxMul(mtxTransform, &mtxSRTInv, &m_boneTransforms[indexBone]);

		return mtxTransform;
	}

	//
	// ���µ���һ֡
	//
	VOID CEntitySkin::NextFrame(FLOAT deltaTime)
	{
		//
		// 1. �����������
		//
		const CSkeletonPtr &ptrSkeleton = m_ptrMesh->GetSkeleton();

		if (ptrSkeleton.IsNull() || ptrSkeleton->IsValid() == FALSE) {
			return;
		}

		if (m_animationStates.empty()) {
			return;
		}

		//
		// 2. ��ù�����Ϣ����洢
		//
		const INT numBones = ptrSkeleton->GetBoneCount();

		BONE *pBoneBuffer = (BONE *)SAFE_MALLOC(numBones * sizeof(*pBoneBuffer), MEMTYPE_STACK); ASSERT(pBoneBuffer);
		FLOAT *pBoneWeights = (FLOAT *)SAFE_MALLOC(numBones * sizeof(*pBoneWeights), MEMTYPE_STACK); ASSERT(pBoneWeights);

		//
		// 3. ����Ȩ������
		// 
		for (INT indexBone = 0; indexBone < numBones; indexBone++) {
			pBoneWeights[indexBone] = 0.0f;
		}

		for (AnimationStateMap::const_iterator itAnimState = m_animationStates.begin(); itAnimState != m_animationStates.end(); ++itAnimState) {
			const CAnimationState *pAnimationState = itAnimState->second;
			ASSERT(pAnimationState);

			if (pAnimationState->IsPlaying() == FALSE) {
				continue;
			}

			for (INT indexBone = 0; indexBone < numBones; indexBone++) {
				pBoneWeights[indexBone] += pAnimationState->GetBoneWeightByIndex(indexBone);
			}
		}

		for (INT indexBone = 0; indexBone < numBones; indexBone++) {
			pBoneWeights[indexBone] = pBoneWeights[indexBone] < EPSILON_E3 ? 0.0f : 1.0f / pBoneWeights[indexBone];
		}

		//
		// 4. ����ÿ�������ľ���
		//
		// 4.1. ��ʼ����������
		for (INT indexBone = 0; indexBone < numBones; indexBone++) {
			pBoneBuffer[indexBone].bUpdate = FALSE;
			Vec3Set(&pBoneBuffer[indexBone].scale, 1.0f, 1.0f, 1.0f);
			Vec3Zero(&pBoneBuffer[indexBone].position);
			QuatZero(&pBoneBuffer[indexBone].orientation);
		}

		// 4.2. ��������ڵ�
		for (AnimationStateMap::const_iterator itAnimState = m_animationStates.begin(); itAnimState != m_animationStates.end(); ++itAnimState) {
			CAnimationState *pAnimationState = itAnimState->second;
			ASSERT(pAnimationState);

			if (pAnimationState->IsPlaying() == FALSE) {
				continue;
			}

			BONE *pBones = (BONE *)SAFE_MALLOC(numBones * sizeof(*pBones), MEMTYPE_STACK);
			ASSERT(pBones);

			pAnimationState->NextFrame(deltaTime);
			pAnimationState->GetFrameData(pBones);

			for (INT indexBone = 0; indexBone < numBones; indexBone++) {
				FLOAT weight = pAnimationState->GetBoneWeightByIndex(indexBone) * pBoneWeights[indexBone];

				if (FastFAbs(weight - 1.0f) < EPSILON_E3) {
					VEC3 curScale;
					VEC3 curPosition;
					QUAT curOrientation;

					Vec3Copy(&curScale, &pBoneBuffer[indexBone].scale);
					Vec3Copy(&curPosition, &pBoneBuffer[indexBone].position);
					QuatCopy(&curOrientation, &pBoneBuffer[indexBone].orientation);

					Vec3Mul(&pBoneBuffer[indexBone].scale, &curScale, &pBones[indexBone].scale);
					Vec3Add(&pBoneBuffer[indexBone].position, &curPosition, &pBones[indexBone].position);
					QuatMul(&pBoneBuffer[indexBone].orientation, &curOrientation, &pBones[indexBone].orientation);
				}
				else if (FastFAbs(weight) > EPSILON_E3) {
					VEC3 positionZero;
					QUAT orientationZero;

					Vec3Zero(&positionZero);
					QuatZero(&orientationZero);

					VEC3 newScale;
					VEC3 newPosition;
					QUAT newOrientation;

					Vec3Copy(&newScale, &pBones[indexBone].scale); // ??? �����������в�ֵ
					Vec3Lerp(&newPosition, &positionZero, &pBones[indexBone].position, weight);
					QuatSlerp(&newOrientation, &orientationZero, &pBones[indexBone].orientation, weight);

					VEC3 curScale;
					VEC3 curPosition;
					QUAT curOrientation;

					Vec3Copy(&curScale, &pBoneBuffer[indexBone].scale);
					Vec3Copy(&curPosition, &pBoneBuffer[indexBone].position);
					QuatCopy(&curOrientation, &pBoneBuffer[indexBone].orientation);

					Vec3Mul(&pBoneBuffer[indexBone].scale, &curScale, &newScale);
					Vec3Add(&pBoneBuffer[indexBone].position, &curPosition, &newPosition);
					QuatMul(&pBoneBuffer[indexBone].orientation, &curOrientation, &newOrientation);
				}
			}

			SAFE_FREE(pBones);
		}

		// 4.3. �����������
		const BONE_HIERARCHY *pBoneHierarchy = ptrSkeleton->GetBoneHierarchy();
		ASSERT(pBoneHierarchy);
		
		// LocalCurrent = DeltaCurrent * LocalInit;
		for (INT indexBone = 0; indexBone < numBones; indexBone++) {
			ConnectBone(&pBoneBuffer[indexBone], &pBoneBuffer[indexBone], &pBoneHierarchy->pBoneLocal2Parents[indexBone]);
		}

		// Local2WorldCurrent = LocalCurrent * Parent ...
		for (INT indexBone = 0; indexBone < numBones; indexBone++) {
			UpdateBone(pBoneBuffer, pBoneHierarchy->pParentIDs, indexBone);
		}

		// WorldCurrent = World2Local * Local2WorldCurrent
		for (INT indexBone = 0; indexBone < numBones; indexBone++) {
			ConnectBone(&pBoneBuffer[indexBone], &pBoneHierarchy->pBoneWorld2Locals[indexBone], &pBoneBuffer[indexBone]);
		}

		// Scale, Position, Rotate => Matrix
		for (INT indexBone = 0; indexBone < numBones; indexBone++) {
			MATRIX4 mtxSR;
			MATRIX4 mtxScale;
			MATRIX4 mtxRotate;
			MATRIX4 mtxTranslate;

			MtxDefScale(&mtxScale, pBoneBuffer[indexBone].scale[0], pBoneBuffer[indexBone].scale[1], pBoneBuffer[indexBone].scale[2]);
			MtxDefTranslate(&mtxTranslate, pBoneBuffer[indexBone].position[0], pBoneBuffer[indexBone].position[1], pBoneBuffer[indexBone].position[2]);
			QuatToMtxRotation(&mtxRotate, &pBoneBuffer[indexBone].orientation);

			MtxMul(&mtxSR, &mtxScale, &mtxRotate);
			MtxMul(&m_boneTransforms[indexBone], &mtxSR, &mtxTranslate);
		}

		//
		// 5. �ͷŴ洢
		//
		SAFE_FREE(pBoneWeights);
		SAFE_FREE(pBoneBuffer);
	}

	//
	// ���¹���
	//
	VOID CEntitySkin::UpdateBone(BONE *pBoneBuffer, const INT *pBoneParents, INT indexBone) const
	{
		BONE *pBone = &pBoneBuffer[indexBone];
		BONE *pBoneParent = pBoneParents[indexBone] >= 0 ? &pBoneBuffer[pBoneParents[indexBone]] : NULL;

		//
		// 1. ��������
		//
		if (pBoneParent) {
			if (FALSE == pBoneParent->bUpdate) {
				UpdateBone(pBoneBuffer, pBoneParents, pBoneParents[indexBone]);
			}

			ConnectBone(pBone, pBone, pBoneParent);
		}

		//
		// 2. �Զ��峯���滻
		//
		{
			QUAT orientation;
			QuatMul(&orientation, &pBone->orientation, &m_boneCustomOrientations[indexBone]);
			QuatCopy(&pBone->orientation, &orientation);
		}

		//
		// 3. �Զ�����
		//
		if (m_boneCustomBlends[indexBone].weightScale > EPSILON_E3) {
			Vec3Lerp(&pBone->scale, &pBone->scale, &m_boneCustomBlends[indexBone].scale, m_boneCustomBlends[indexBone].weightScale);
		}

		if (m_boneCustomBlends[indexBone].weightPosition > EPSILON_E3) {
			Vec3Lerp(&pBone->position, &pBone->position, &m_boneCustomBlends[indexBone].position, m_boneCustomBlends[indexBone].weightPosition);
		}

		if (m_boneCustomBlends[indexBone].weightOrientation > EPSILON_E3) {
			QuatSlerp(&pBone->orientation, &pBone->orientation, &m_boneCustomBlends[indexBone].orientation, m_boneCustomBlends[indexBone].weightOrientation);
		}

		//
		// 4. ���±��
		//
		pBone->bUpdate = TRUE;
	}

	//
	// ���ӹ�������
	//
	VOID CEntitySkin::ConnectBone(BONE *pBone, const BONE *pBoneChild, const BONE *pBoneParent) const
	{
		VEC3 tmp0;
		VEC3 tmp1;

		VEC3 scale;
		VEC3 position;
		QUAT orientation;

		Vec3Mul(&scale, &pBoneChild->scale, &pBoneParent->scale);

		Vec3Mul(&tmp0, &pBoneChild->position, &pBoneParent->scale);
		Vec3MulQuat(&tmp1, &tmp0, &pBoneParent->orientation);
		Vec3Add(&position, &tmp1, &pBoneParent->position);

		QuatMul(&orientation, &pBoneChild->orientation, &pBoneParent->orientation);

		Vec3Copy(&pBone->scale, &scale);
		Vec3Copy(&pBone->position, &position);
		QuatCopy(&pBone->orientation, &orientation);
	}

	//
	// ��Ⱦ
	//
	VOID CEntitySkin::Render(const CMaterial *pMaterial) const
	{
		//
		// 1. ������ȫ���
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return;
		}

		//
		// 2. ��Ⱦ
		//
		if (const CSubMesh *pRenderMesh = m_ptrMesh->GetSubMesh(m_dwRenderMeshName)) {
			Renderer()->PushMatrix();
			Renderer()->PushTextureMatrix();
			{
				//
				// 2.1. ���ù�����ͼ����
				//
				if (m_ptrLightMapTexture.IsNull() || m_ptrLightMapTexture->IsValid() == FALSE) {
					Renderer()->SetLightMapTexture(0);
				}
				else {
					Renderer()->SetLightMapTexture(m_ptrLightMapTexture->GetTextureGL());
				}

				//
				// 2.2. ���ö�̬��Դ
				//
				VEC4 direction;
				VEC3 ambient, diffuse, specular;
				VEC3 rim, skyLower, skyUpper;
				VEC3 indirectUp, indirectDown, indirectLeft, indirectRight, indirectFront, indirectBack;
				FLOAT attenuation[] = { 1.0f, 0.0f, 0.0f };

				if (m_pSceneManager->GetLightParams(m_dwLightChannel, &m_aabbWorld.center, &direction, &ambient, &diffuse, &specular, &rim, &skyLower, &skyUpper, &indirectUp, &indirectDown, &indirectLeft, &indirectRight, &indirectFront, &indirectBack)) {
					Renderer()->SetLight(direction, ambient, diffuse, specular, rim, skyLower, skyUpper, attenuation);
				}

				//
				// 2.3. ���ñ任����
				//
				MATRIX4 mtxTransform;

				if (m_pSceneNode) {
					MtxCopy(&mtxTransform, m_pSceneNode->GetWorldMatrix());
				}
				else {
					MtxIdentity(&mtxTransform);
				}

				Renderer()->SetModelWorldMatrix(mtxTransform);

				//
				// 2.4. �����������
				//
				for (INT indexTexUnit = 0; indexTexUnit < MAX_TEXUNITS; indexTexUnit++) {
					Renderer()->SetTextureMatrix(indexTexUnit, m_mtxTextures[indexTexUnit]);
				}

				//
				// 2.5. ��Ⱦ
				//
				Renderer()->BindVBO(GL_ARRAY_BUFFER, pRenderMesh->GetVBO());
				Renderer()->BindVBO(GL_ELEMENT_ARRAY_BUFFER, pRenderMesh->GetIBO());
				Renderer()->SubmitMatrixBuffer(*m_boneTransforms, m_ptrMesh->GetSkeleton()->GetBoneCount());
				Renderer()->SubmitVertexBuffer(CRenderer::VERTEX_FORMAT_STATICMESH, NULL);
				Renderer()->Render(NULL, pRenderMesh->GetFaceCount() * 3);
			}
			Renderer()->PopTextureMatrix();
			Renderer()->PopMatrix();
		}
	}

	//
	// ��Ⱦ�ڵ�
	//
	VOID CEntitySkin::RenderOcclude(VOID) const
	{

	}

}

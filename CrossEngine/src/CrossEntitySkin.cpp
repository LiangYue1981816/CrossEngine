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
		// 1. 设置骨骼
		//
		m_ptrSkeleton = ptrSkeleton;
		m_pAnimation = const_cast<CAnimSkeleton *>(m_ptrSkeleton->GetAnimation(indexAnim)); ASSERT(m_pAnimation);
		m_weights = (FLOAT *)SAFE_MALLOC(m_ptrSkeleton->GetBoneCount() * sizeof(*m_weights), MEMTYPE_HEAP); ASSERT(m_weights);

		//
		// 2. 设置动画状态
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
	// 获得动画名
	//
	const CHAR* CAnimationState::GetName(VOID) const
	{
		return m_pAnimation->GetName();
	}

	//
	// 获得动画名
	//
	DWORD CAnimationState::GetHashName(VOID) const
	{
		return m_pAnimation->GetHashName();
	}

	//
	// 获得骨骼数
	//
	INT CAnimationState::GetBoneCount(VOID) const
	{
		return m_ptrSkeleton->GetBoneCount();
	}

	//
	// 获得时间长度
	//
	FLOAT CAnimationState::GetLength(VOID) const
	{
		return m_pAnimation->GetLength();
	}

	//
	// 获得骨架结构
	//
	const BONE_HIERARCHY* CAnimationState::GetSkeletonHierarchy(VOID) const
	{
		return m_ptrSkeleton->GetBoneHierarchy();
	}

	//
	// 设置动画权重
	//
	VOID CAnimationState::SetWeight(FLOAT weight)
	{
		//
		// 1. 参数安全检查
		//
		if (weight < EPSILON_E3) {
			weight = 0.0f;
		}

		//
		// 2. 设置动画权重
		//
		for (INT indexBone = 0; indexBone < m_ptrSkeleton->GetBoneCount(); indexBone++) {
			m_weights[indexBone] = weight;
		}
	}

	//
	// 调制动画权重
	//
	VOID CAnimationState::ModulateWeight(FLOAT weight)
	{
		//
		// 1. 参数安全检查
		//
		if (weight < EPSILON_E3) {
			weight = 0.0f;
		}

		//
		// 2. 调制动画权重
		//
		for (INT indexBone = 0; indexBone < m_ptrSkeleton->GetBoneCount(); indexBone++) {
			m_weights[indexBone] *= weight;
		}
	}

	//
	// 设置骨骼权重
	//
	VOID CAnimationState::SetBoneWeightByName(const CHAR *szName, FLOAT weight)
	{
		ASSERT(szName);
		return SetBoneWeightByName(HashValue(szName), weight);
	}

	//
	// 设置骨骼权重
	//
	VOID CAnimationState::SetBoneWeightByName(DWORD dwName, FLOAT weight)
	{
		//
		// 1. 参数安全检查
		//
		if (weight < EPSILON_E3) {
			weight = 0.0f;
		}

		//
		// 2. 设置骨骼权重
		//
		for (INT indexBone = 0; indexBone < m_ptrSkeleton->GetBoneCount(); indexBone++) {
			if (m_ptrSkeleton->GetBoneHierarchy()->pBoneNames[indexBone].dwName == dwName) {
				m_weights[indexBone] = weight;
				return;
			}
		}
	}

	//
	// 设置骨骼权重
	//
	VOID CAnimationState::SetBoneWeightByIndex(INT indexBone, FLOAT weight)
	{
		//
		// 1. 参数安全检查
		//
		if (indexBone < 0 || indexBone >= m_ptrSkeleton->GetBoneCount()) {
			return;
		}

		if (weight < EPSILON_E3) {
			weight = 0.0f;
		}

		//
		// 2. 设置骨骼权重
		//
		m_weights[indexBone] = weight;
	}

	//
	// 获得骨骼权重
	//
	FLOAT CAnimationState::GetBoneWeightByName(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetBoneWeightByName(HashValue(szName));
	}

	//
	// 获得骨骼权重
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
	// 获得骨骼权重
	//
	FLOAT CAnimationState::GetBoneWeightByIndex(INT indexBone) const
	{
		//
		// 1. 参数安全检查
		//
		if (indexBone < 0 || indexBone >= m_ptrSkeleton->GetBoneCount()) {
			return 0.0f;
		}

		//
		// 2. 获得骨骼权重
		//
		return m_weights[indexBone];
	}

	//
	// 设置播放速度
	//
	VOID CAnimationState::SetSpeed(FLOAT speed)
	{
		m_speed = speed;
	}

	//
	// 获得当前时间位置
	//
	VOID CAnimationState::SetTimePosition(FLOAT t)
	{
		m_currTime = t;
		m_forwardTime = t;
		m_backwardTime = t;
	}

	//
	// 设置开始时间
	//
	VOID CAnimationState::SetBeginTime(FLOAT beginTime)
	{
		//
		// 1. 参数安全检查
		//
		if (beginTime < 0.0f || beginTime > m_pAnimation->GetLength()) {
			return;
		}

		//
		// 2. 设置开始时间
		//
		m_beginTime = beginTime < m_endTime ? beginTime : m_endTime;
	}

	//
	// 设置终止时间
	//
	VOID CAnimationState::SetEndTime(FLOAT endTime)
	{
		//
		// 1. 参数安全检查
		//
		if (endTime < 0.0f || endTime > m_pAnimation->GetLength()) {
			return;
		}

		//
		// 2. 设置终止时间
		//
		m_endTime = endTime > m_beginTime ? endTime : m_beginTime;
	}

	//
	// 获得播放速度
	//
	FLOAT CAnimationState::GetSpeed(VOID) const
	{
		return m_speed;
	}

	//
	// 设置当前时间位置
	//
	FLOAT CAnimationState::GetTimePosition(VOID) const
	{
		return m_currTime;
	}

	//
	// 获得开始时间
	//
	FLOAT CAnimationState::GetBeginTime(VOID) const
	{
		return m_beginTime;
	}

	//
	// 获得终止时间
	//
	FLOAT CAnimationState::GetEndTime(VOID) const
	{
		return m_endTime;
	}

	//
	// 设置动画状态
	//
	VOID CAnimationState::SetEnable(BOOL bEnable)
	{
		m_bEnable = bEnable;
	}

	//
	// 获得动画状态
	//
	BOOL CAnimationState::IsEnable(VOID) const
	{
		return m_bEnable;
	}

	//
	// 播放动画
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
	// 停止播放
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
	// 暂停播放
	//
	VOID CAnimationState::Pause(VOID)
	{
		m_bPause = TRUE;
	}

	//
	// 继续播放
	//
	VOID CAnimationState::Resume(VOID)
	{
		m_bPause = FALSE;
	}

	//
	// 播放状态
	//
	BOOL CAnimationState::IsPlaying(VOID) const
	{
		return m_bEnable && m_bPlaying && m_bPause == FALSE ? TRUE : FALSE;
	}

	//
	// 更新到下一帧
	//
	VOID CAnimationState::NextFrame(FLOAT deltaTime)
	{
		//
		// 1. 动画状态检查
		//
		if (IsPlaying() == FALSE) {
			return;
		}

		//
		// 2. 播放速度调节
		//
		deltaTime *= m_speed;

		//
		// 3. 正向播放
		//
		if (deltaTime > 0.0f) {
			m_forwardTime += deltaTime;

			if (m_forwardTime > m_endTime) {
				m_forwardTime = m_bLoop ? m_beginTime + m_forwardTime - m_endTime : m_endTime;
			}

			m_currTime = m_backwardTime = m_forwardTime;
		}

		//
		// 4. 反向播放
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
	// 获得帧数据
	//
	VOID CAnimationState::GetFrameData(BONE *pBones) const
	{
		//
		// 1. 计算关键帧索引
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
		// 2. 直接获得关键帧数据
		//
		if (indexKeyFrame0 == indexKeyFrame1) {
			memcpy(pBones, m_pAnimation->GetKeyFrameData(indexKeyFrame0), m_ptrSkeleton->GetBoneCount() * sizeof(*pBones));
		}
		//
		// 3. 根据关键帧计算当前帧数据
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
	// 获得类型
	//
	CEntity::TYPE CEntitySkin::GetType(VOID) const
	{
		return CEntity::SKIN;
	}

	//
	// 获得可渲染
	//
	BOOL CEntitySkin::IsRenderable(VOID) const
	{
		return TRUE;
	}

	//
	// 设置网格
	//
	BOOL CEntitySkin::SetMesh(const CMeshPtr &ptrMesh)
	{
		SAFE_FREE(m_boneTransforms);
		SAFE_FREE(m_boneCustomBlends);
		SAFE_FREE(m_boneCustomOrientations);

		FreeAnimationStates();

		//
		// 1. 设置网格
		//
		if (CEntityMesh::SetMesh(ptrMesh) == FALSE) {
			return FALSE;
		}

		//
		// 2. 设置骨骼动画
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
	// 添加动画
	//
	BOOL CEntitySkin::AddAnimations(const CHAR *szName)
	{
		ASSERT(szName);
		return AddAnimations(HashValue(szName));
	}

	//
	// 添加动画
	//
	BOOL CEntitySkin::AddAnimations(DWORD dwName)
	{
		return AddAnimations(SkeletonManager()->QueryResource(dwName));
	}

	//
	// 添加动画
	//
	BOOL CEntitySkin::AddAnimations(const CSkeletonPtr &ptrAnimSkeleton)
	{
		//
		// 1. 参数安全检查
		//
		const CSkeletonPtr &ptrMeshSkeleton = m_ptrMesh->GetSkeleton();

		if (ptrMeshSkeleton.IsNull() || ptrMeshSkeleton->IsValid() == FALSE ||
			ptrAnimSkeleton.IsNull() || ptrAnimSkeleton->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. 验证骨架一致性(骨骼数, 父子关系, 骨骼名必须一致)
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
		// 3. 添加动画
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
	// 获得动画
	//
	CAnimationState* CEntitySkin::GetAnimationState(const CHAR *szName) const
	{
		ASSERT(szName);
		return GetAnimationState(HashValue(szName));
	}

	//
	// 获得动画
	//
	CAnimationState* CEntitySkin::GetAnimationState(DWORD dwName) const
	{
		AnimationStateMap::const_iterator itAnimState = m_animationStates.find(dwName);
		return itAnimState != m_animationStates.end() ? itAnimState->second : NULL;
	}

	//
	// 获得所有动画
	//
	const CEntitySkin::AnimationStateMap& CEntitySkin::GetAnimationStates(VOID) const
	{
		return m_animationStates;
	}

	//
	// 释放动画集合
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
	// 设置自定义骨骼混合(模型坐标系)
	//
	VOID CEntitySkin::SetBoneCustomBlend(const CHAR *szBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT weightScale, FLOAT weightPosition, FLOAT weightOrientation)
	{
		ASSERT(szBoneName);
		SetBoneCustomBlend(HashValue(szBoneName), scale, position, orientation, weightScale, weightPosition, weightOrientation);
	}

	//
	// 设置自定义骨骼混合(模型坐标系)
	//
	VOID CEntitySkin::SetBoneCustomBlend(DWORD dwBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT weightScale, FLOAT weightPosition, FLOAT weightOrientation)
	{
		//
		// 1. 查找骨骼
		//
		CustomBlendMap::const_iterator itBoneBlend = m_boneCustomBlendMap.find(dwBoneName);
		if (itBoneBlend == m_boneCustomBlendMap.end()) return;

		//
		// 2. 设置自定义骨骼混合
		//
		itBoneBlend->second->weightScale = weightScale;
		itBoneBlend->second->weightPosition = weightPosition;
		itBoneBlend->second->weightOrientation = weightOrientation;

		if (scale) Vec3Copy(&itBoneBlend->second->scale, scale);
		if (position) Vec3Copy(&itBoneBlend->second->position, position);
		if (orientation) QuatCopy(&itBoneBlend->second->orientation, orientation);
	}

	//
	// 获得自定义骨骼混合(模型坐标系)
	//
	VOID CEntitySkin::GetBoneCustomBlend(const CHAR *szBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT *weightScale, FLOAT *weightPosition, FLOAT *weightOrientation) const
	{
		ASSERT(szBoneName);
		GetBoneCustomBlend(HashValue(szBoneName), scale, position, orientation, weightScale, weightPosition, weightOrientation);
	}

	//
	// 获得自定义骨骼混合(模型坐标系)
	//
	VOID CEntitySkin::GetBoneCustomBlend(DWORD dwBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT *weightScale, FLOAT *weightPosition, FLOAT *weightOrientation) const
	{
		//
		// 1. 查找骨骼
		//
		CustomBlendMap::const_iterator itBoneBlend = m_boneCustomBlendMap.find(dwBoneName);
		if (itBoneBlend == m_boneCustomBlendMap.end()) return;

		//
		// 2. 获得自定义骨骼混合(模型坐标系)
		//
		if (weightScale) *weightScale = itBoneBlend->second->weightScale;
		if (weightPosition) *weightPosition = itBoneBlend->second->weightPosition;
		if (weightOrientation) *weightOrientation = itBoneBlend->second->weightOrientation;

		if (scale) Vec3Copy(scale, &itBoneBlend->second->scale);
		if (position) Vec3Copy(position, &itBoneBlend->second->position);
		if (orientation) QuatCopy(orientation, &itBoneBlend->second->orientation);
	}

	//
	// 设置自定义骨骼朝向(模型坐标系)
	//
	VOID CEntitySkin::SetBoneCustomOrientation(const CHAR *szBoneName, QUAT *orientation)
	{
		ASSERT(szBoneName);
		ASSERT(orientation);
		SetBoneCustomOrientation(HashValue(szBoneName), orientation);
	}

	//
	// 设置自定义骨骼朝向(模型坐标系)
	//
	VOID CEntitySkin::SetBoneCustomOrientation(DWORD dwBoneName, QUAT *orientation)
	{
		ASSERT(orientation);

		//
		// 1. 查找骨骼
		//
		CustomOrientationMap::const_iterator itBoneOrientation = m_boneCustomOrientationsMap.find(dwBoneName);
		if (itBoneOrientation == m_boneCustomOrientationsMap.end()) return;

		//
		// 2. 设置自定义骨骼朝向
		//
		QuatCopy(itBoneOrientation->second, orientation);
	}

	//
	// 获得自定义骨骼朝向(模型坐标系)
	//
	VOID CEntitySkin::GetBoneCustomOrientation(const CHAR *szBoneName, QUAT *orientation) const
	{
		ASSERT(szBoneName);
		GetBoneCustomOrientation(HashValue(szBoneName), orientation);
	}

	//
	// 获得自定义骨骼朝向(模型坐标系)
	//
	VOID CEntitySkin::GetBoneCustomOrientation(DWORD dwBoneName, QUAT *orientation) const
	{
		//
		// 1. 查找骨骼
		//
		CustomOrientationMap::const_iterator itBoneOrientation = m_boneCustomOrientationsMap.find(dwBoneName);
		if (itBoneOrientation == m_boneCustomOrientationsMap.end()) return;

		//
		// 2. 设置自定义骨骼朝向
		//
		if (orientation) QuatCopy(orientation, itBoneOrientation->second);
	}

	//
	// 获得变换矩阵
	//
	MATRIX4* CEntitySkin::GetBoneTransform(INT indexBone, MATRIX4 *mtxTransform) const
	{
		ASSERT(mtxTransform);
		MtxIdentity(mtxTransform);

		//
		// 1. 参数安全检查
		//
		const CSkeletonPtr &ptrSkeleton = m_ptrMesh->GetSkeleton();

		if (ptrSkeleton.IsNull() || ptrSkeleton->IsValid() == FALSE) {
			return mtxTransform;
		}

		if (indexBone < 0 || indexBone >= ptrSkeleton->GetBoneCount()) {
			return mtxTransform;
		}

		//
		// 2. 获得变换矩阵
		//    说明: NextFrame计算得到的变换矩阵包括从世界坐标系变换到模型坐标系, 这里需要将其去除
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
	// 更新到下一帧
	//
	VOID CEntitySkin::NextFrame(FLOAT deltaTime)
	{
		//
		// 1. 骨骼动画检查
		//
		const CSkeletonPtr &ptrSkeleton = m_ptrMesh->GetSkeleton();

		if (ptrSkeleton.IsNull() || ptrSkeleton->IsValid() == FALSE) {
			return;
		}

		if (m_animationStates.empty()) {
			return;
		}

		//
		// 2. 获得骨骼信息分配存储
		//
		const INT numBones = ptrSkeleton->GetBoneCount();

		BONE *pBoneBuffer = (BONE *)SAFE_MALLOC(numBones * sizeof(*pBoneBuffer), MEMTYPE_STACK); ASSERT(pBoneBuffer);
		FLOAT *pBoneWeights = (FLOAT *)SAFE_MALLOC(numBones * sizeof(*pBoneWeights), MEMTYPE_STACK); ASSERT(pBoneWeights);

		//
		// 3. 计算权重因子
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
		// 4. 计算每个动画的矩阵
		//
		// 4.1. 初始化骨骼缓冲
		for (INT indexBone = 0; indexBone < numBones; indexBone++) {
			pBoneBuffer[indexBone].bUpdate = FALSE;
			Vec3Set(&pBoneBuffer[indexBone].scale, 1.0f, 1.0f, 1.0f);
			Vec3Zero(&pBoneBuffer[indexBone].position);
			QuatZero(&pBoneBuffer[indexBone].orientation);
		}

		// 4.2. 计算骨骼节点
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

					Vec3Copy(&newScale, &pBones[indexBone].scale); // ??? 缩放量不进行插值
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

		// 4.3. 计算骨骼矩阵
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
		// 5. 释放存储
		//
		SAFE_FREE(pBoneWeights);
		SAFE_FREE(pBoneBuffer);
	}

	//
	// 更新骨骼
	//
	VOID CEntitySkin::UpdateBone(BONE *pBoneBuffer, const INT *pBoneParents, INT indexBone) const
	{
		BONE *pBone = &pBoneBuffer[indexBone];
		BONE *pBoneParent = pBoneParents[indexBone] >= 0 ? &pBoneBuffer[pBoneParents[indexBone]] : NULL;

		//
		// 1. 骨骼级联
		//
		if (pBoneParent) {
			if (FALSE == pBoneParent->bUpdate) {
				UpdateBone(pBoneBuffer, pBoneParents, pBoneParents[indexBone]);
			}

			ConnectBone(pBone, pBone, pBoneParent);
		}

		//
		// 2. 自定义朝向替换
		//
		{
			QUAT orientation;
			QuatMul(&orientation, &pBone->orientation, &m_boneCustomOrientations[indexBone]);
			QuatCopy(&pBone->orientation, &orientation);
		}

		//
		// 3. 自定义混合
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
		// 4. 更新标记
		//
		pBone->bUpdate = TRUE;
	}

	//
	// 父子骨骼及联
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
	// 渲染
	//
	VOID CEntitySkin::Render(const CMaterial *pMaterial) const
	{
		//
		// 1. 参数安全检查
		//
		if (m_ptrMesh.IsNull() || m_ptrMesh->IsValid() == FALSE) {
			return;
		}

		//
		// 2. 渲染
		//
		if (const CSubMesh *pRenderMesh = m_ptrMesh->GetSubMesh(m_dwRenderMeshName)) {
			Renderer()->PushMatrix();
			Renderer()->PushTextureMatrix();
			{
				//
				// 2.1. 设置光照贴图纹理
				//
				if (m_ptrLightMapTexture.IsNull() || m_ptrLightMapTexture->IsValid() == FALSE) {
					Renderer()->SetLightMapTexture(0);
				}
				else {
					Renderer()->SetLightMapTexture(m_ptrLightMapTexture->GetTextureGL());
				}

				//
				// 2.2. 设置动态光源
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
				// 2.3. 设置变换矩阵
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
				// 2.4. 设置纹理矩阵
				//
				for (INT indexTexUnit = 0; indexTexUnit < MAX_TEXUNITS; indexTexUnit++) {
					Renderer()->SetTextureMatrix(indexTexUnit, m_mtxTextures[indexTexUnit]);
				}

				//
				// 2.5. 渲染
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
	// 渲染遮挡
	//
	VOID CEntitySkin::RenderOcclude(VOID) const
	{

	}

}

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


#include "CrossEngine.h"


namespace CrossEngine {

	class _CrossExport CAnimationState
	{
		friend class CEntitySkin;


		// ����/��������
	protected:
		CAnimationState(const CSkeletonPtr &ptrSkeleton, INT indexAnim);
		virtual ~CAnimationState(VOID);


		// ����
	public:
		const CHAR* GetName(VOID) const;                                                           // ��ö�����
		DWORD GetHashName(VOID) const;                                                             // ��ö�����

		INT GetBoneCount(VOID) const;                                                              // ��ù�����
		FLOAT GetLength(VOID) const;                                                               // ���ʱ�䳤��

		const BONE_HIERARCHY* GetSkeletonHierarchy(VOID) const;                                    // ��ùǼܽṹ

	public:
		VOID SetWeight(FLOAT weight);                                                              // ���ö���Ȩ��
		VOID ModulateWeight(FLOAT weight);                                                         // ���ƶ���Ȩ��

		VOID SetBoneWeightByName(const CHAR *szName, FLOAT weight);                                // ���ù���Ȩ��
		VOID SetBoneWeightByName(DWORD dwName, FLOAT weight);                                      // ���ù���Ȩ��
		VOID SetBoneWeightByIndex(INT indexBone, FLOAT weight);                                    // ���ù���Ȩ��

		FLOAT GetBoneWeightByName(const CHAR *szName) const;                                       // ��ù���Ȩ��
		FLOAT GetBoneWeightByName(DWORD dwName) const;                                             // ��ù���Ȩ��
		FLOAT GetBoneWeightByIndex(INT indexBone) const;                                           // ��ù���Ȩ��

	public:
		VOID SetSpeed(FLOAT speed);                                                                // ���ò����ٶ�
		VOID SetTimePosition(FLOAT t);                                                             // ��õ�ǰʱ��λ��

		VOID SetBeginTime(FLOAT beginTime);                                                        // ���ÿ�ʼʱ��
		VOID SetEndTime(FLOAT endTime);                                                            // ������ֹʱ��

		FLOAT GetSpeed(VOID) const;                                                                // ��ò����ٶ�
		FLOAT GetTimePosition(VOID) const;                                                         // ���õ�ǰʱ��λ��

		FLOAT GetBeginTime(VOID) const;                                                            // ��ÿ�ʼʱ��
		FLOAT GetEndTime(VOID) const;                                                              // �����ֹʱ��

	public:
		VOID SetEnable(BOOL bEnable);                                                              // ���ö���״̬
		BOOL IsEnable(VOID) const;                                                                 // ��ö���״̬

		VOID Play(BOOL bLoop, FLOAT speed = 1.0f);                                                 // ���Ŷ���
		VOID Stop(VOID);                                                                           // ֹͣ����

		VOID Pause(VOID);                                                                          // ��ͣ����
		VOID Resume(VOID);                                                                         // ��������

		BOOL IsPlaying(VOID) const;                                                                // ����״̬

	public:
		VOID NextFrame(FLOAT deltaTime);                                                           // ���µ���һ֡
		VOID GetFrameData(BONE *pBones) const;                                                     // ���֡����


		// ����
	protected:
		BOOL m_bEnable;                                                                            // ����״̬

		BOOL m_bPlaying;                                                                           // ���Ŷ���
		BOOL m_bPause;                                                                             // ��ͣ���Ŷ���
		BOOL m_bLoop;                                                                              // ѭ�����Ŷ���

		FLOAT m_speed;                                                                             // �����ٶ�

		FLOAT m_beginTime;                                                                         // ��ʼʱ��
		FLOAT m_endTime;                                                                           // ����ʱ��

		FLOAT m_currTime;                                                                          // ��ǰʱ��
		FLOAT m_forwardTime;                                                                       // ���򲥷�ʱ��
		FLOAT m_backwardTime;                                                                      // ���򲥷�ʱ��

		FLOAT *m_weights;                                                                          // ����Ȩ��
		CAnimSkeleton *m_pAnimation;                                                               // ����ָ��

	protected:
		CSkeletonPtr m_ptrSkeleton;                                                                // ����
	};

	class _CrossExport CEntitySkin : public CEntityMesh
	{
		friend class CSceneManager;


		// ���ݽṹ
	public:
		typedef std::map<DWORD, CAnimationState*> AnimationStateMap;                               // ����״̬����
		typedef std::map<DWORD, BONE_BLEND*> CustomBlendMap;                                       // �Զ�����(ģ������ϵ)����
		typedef std::map<DWORD, QUAT*> CustomOrientationMap;                                       // �Զ��峯��(ģ������ϵ)����


		// ����/��������
	protected:
		CEntitySkin(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntitySkin(VOID);


		// ����
	public:
		virtual TYPE GetType(VOID) const;                                                          // �������
		virtual BOOL IsRenderable(VOID) const;                                                     // ��ÿ���Ⱦ

	public:
		virtual BOOL SetMesh(const CMeshPtr &ptrMesh);                                             // ��������

	public:
		BOOL AddAnimations(const CHAR *szName);                                                    // ��Ӷ���
		BOOL AddAnimations(DWORD dwName);                                                          // ��Ӷ���
		BOOL AddAnimations(const CSkeletonPtr &ptrAnimSkeleton);                                   // ��Ӷ���

		CAnimationState* GetAnimationState(const CHAR *szName) const;                              // ��ö���
		CAnimationState* GetAnimationState(DWORD dwName) const;                                    // ��ö���
		const AnimationStateMap& GetAnimationStates(VOID) const;                                   // ������ж���

	protected:
		VOID FreeAnimationStates(VOID);                                                            // �ͷŶ�������

	public:
		VOID SetBoneCustomBlend(const CHAR *szBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT weightScale, FLOAT weightPosition, FLOAT weightOrientation); // �����Զ���������(ģ������ϵ)
		VOID SetBoneCustomBlend(DWORD dwBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT weightScale, FLOAT weightPosition, FLOAT weightOrientation); // �����Զ���������(ģ������ϵ)

		VOID GetBoneCustomBlend(const CHAR *szBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT *weightScale, FLOAT *weightPosition, FLOAT *weightOrientation) const; // ����Զ���������(ģ������ϵ)
		VOID GetBoneCustomBlend(DWORD dwBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT *weightScale, FLOAT *weightPosition, FLOAT *weightOrientation) const; // ����Զ���������(ģ������ϵ)

		VOID SetBoneCustomOrientation(const CHAR *szBoneName, QUAT *orientation);                  // �����Զ����������(ģ������ϵ)
		VOID SetBoneCustomOrientation(DWORD dwBoneName, QUAT *orientation);                        // �����Զ����������(ģ������ϵ)

		VOID GetBoneCustomOrientation(const CHAR *szBoneName, QUAT *orientation) const;            // ����Զ����������(ģ������ϵ)
		VOID GetBoneCustomOrientation(DWORD dwBoneName, QUAT *orientation) const;                  // ����Զ����������(ģ������ϵ)

		MATRIX4* GetBoneTransform(INT indexBone, MATRIX4 *mtxTransform) const;                     // ��ñ任����

	public:
		VOID NextFrame(FLOAT deltaTime);                                                           // ���µ���һ֡

	protected:
		VOID UpdateBone(BONE *pBoneBuffer, const INT *pBoneParents, INT indexBone) const;          // ���¹���
		VOID ConnectBone(BONE *pBone, const BONE *pBoneChild, const BONE *pBoneParent) const;      // ���ӹ�������

	public:
		virtual VOID Render(const CMaterial *pMaterial) const;                                     // ��Ⱦ
		virtual VOID RenderOcclude(VOID) const;                                                    // ��Ⱦ�ڵ�


		// ����
	protected:
		MATRIX4 *m_boneTransforms;                                                                 // �����任���󼯺�

		BONE_BLEND *m_boneCustomBlends;                                                            // �Զ���������(ģ������ϵ)����
		CustomBlendMap m_boneCustomBlendMap;                                                       // �Զ���������(ģ������ϵ)ӳ��

		QUAT *m_boneCustomOrientations;                                                            // �Զ����������(ģ������ϵ)����
		CustomOrientationMap m_boneCustomOrientationsMap;                                          // �Զ����������(ģ������ϵ)ӳ��

	protected:
		AnimationStateMap m_animationStates;                                                       // ��������
	};

}

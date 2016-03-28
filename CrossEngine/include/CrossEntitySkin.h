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


		// 构造/析构函数
	protected:
		CAnimationState(const CSkeletonPtr &ptrSkeleton, INT indexAnim);
		virtual ~CAnimationState(VOID);


		// 方法
	public:
		const CHAR* GetName(VOID) const;                                                           // 获得动画名
		DWORD GetHashName(VOID) const;                                                             // 获得动画名

		INT GetBoneCount(VOID) const;                                                              // 获得骨骼数
		FLOAT GetLength(VOID) const;                                                               // 获得时间长度

		const BONE_HIERARCHY* GetSkeletonHierarchy(VOID) const;                                    // 获得骨架结构

	public:
		VOID SetWeight(FLOAT weight);                                                              // 设置动画权重
		VOID ModulateWeight(FLOAT weight);                                                         // 调制动画权重

		VOID SetBoneWeightByName(const CHAR *szName, FLOAT weight);                                // 设置骨骼权重
		VOID SetBoneWeightByName(DWORD dwName, FLOAT weight);                                      // 设置骨骼权重
		VOID SetBoneWeightByIndex(INT indexBone, FLOAT weight);                                    // 设置骨骼权重

		FLOAT GetBoneWeightByName(const CHAR *szName) const;                                       // 获得骨骼权重
		FLOAT GetBoneWeightByName(DWORD dwName) const;                                             // 获得骨骼权重
		FLOAT GetBoneWeightByIndex(INT indexBone) const;                                           // 获得骨骼权重

	public:
		VOID SetSpeed(FLOAT speed);                                                                // 设置播放速度
		VOID SetTimePosition(FLOAT t);                                                             // 获得当前时间位置

		VOID SetBeginTime(FLOAT beginTime);                                                        // 设置开始时间
		VOID SetEndTime(FLOAT endTime);                                                            // 设置终止时间

		FLOAT GetSpeed(VOID) const;                                                                // 获得播放速度
		FLOAT GetTimePosition(VOID) const;                                                         // 设置当前时间位置

		FLOAT GetBeginTime(VOID) const;                                                            // 获得开始时间
		FLOAT GetEndTime(VOID) const;                                                              // 获得终止时间

	public:
		VOID SetEnable(BOOL bEnable);                                                              // 设置动画状态
		BOOL IsEnable(VOID) const;                                                                 // 获得动画状态

		VOID Play(BOOL bLoop, FLOAT speed = 1.0f);                                                 // 播放动画
		VOID Stop(VOID);                                                                           // 停止播放

		VOID Pause(VOID);                                                                          // 暂停播放
		VOID Resume(VOID);                                                                         // 继续播放

		BOOL IsPlaying(VOID) const;                                                                // 播放状态

	public:
		VOID NextFrame(FLOAT deltaTime);                                                           // 更新到下一帧
		VOID GetFrameData(BONE *pBones) const;                                                     // 获得帧数据


		// 属性
	protected:
		BOOL m_bEnable;                                                                            // 动画状态

		BOOL m_bPlaying;                                                                           // 播放动画
		BOOL m_bPause;                                                                             // 暂停播放动画
		BOOL m_bLoop;                                                                              // 循环播放动画

		FLOAT m_speed;                                                                             // 播放速度

		FLOAT m_beginTime;                                                                         // 开始时间
		FLOAT m_endTime;                                                                           // 结束时间

		FLOAT m_currTime;                                                                          // 当前时间
		FLOAT m_forwardTime;                                                                       // 正向播放时间
		FLOAT m_backwardTime;                                                                      // 反向播放时间

		FLOAT *m_weights;                                                                          // 骨骼权重
		CAnimSkeleton *m_pAnimation;                                                               // 动画指针

	protected:
		CSkeletonPtr m_ptrSkeleton;                                                                // 骨骼
	};

	class _CrossExport CEntitySkin : public CEntityMesh
	{
		friend class CSceneManager;


		// 数据结构
	public:
		typedef std::map<DWORD, CAnimationState*> AnimationStateMap;                               // 动画状态集合
		typedef std::map<DWORD, BONE_BLEND*> CustomBlendMap;                                       // 自定义混合(模型坐标系)集合
		typedef std::map<DWORD, QUAT*> CustomOrientationMap;                                       // 自定义朝向(模型坐标系)集合


		// 构造/析构函数
	protected:
		CEntitySkin(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntitySkin(VOID);


		// 方法
	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型
		virtual BOOL IsRenderable(VOID) const;                                                     // 获得可渲染

	public:
		virtual BOOL SetMesh(const CMeshPtr &ptrMesh);                                             // 设置网格

	public:
		BOOL AddAnimations(const CHAR *szName);                                                    // 添加动画
		BOOL AddAnimations(DWORD dwName);                                                          // 添加动画
		BOOL AddAnimations(const CSkeletonPtr &ptrAnimSkeleton);                                   // 添加动画

		CAnimationState* GetAnimationState(const CHAR *szName) const;                              // 获得动画
		CAnimationState* GetAnimationState(DWORD dwName) const;                                    // 获得动画
		const AnimationStateMap& GetAnimationStates(VOID) const;                                   // 获得所有动画

	protected:
		VOID FreeAnimationStates(VOID);                                                            // 释放动画集合

	public:
		VOID SetBoneCustomBlend(const CHAR *szBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT weightScale, FLOAT weightPosition, FLOAT weightOrientation); // 设置自定义骨骼混合(模型坐标系)
		VOID SetBoneCustomBlend(DWORD dwBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT weightScale, FLOAT weightPosition, FLOAT weightOrientation); // 设置自定义骨骼混合(模型坐标系)

		VOID GetBoneCustomBlend(const CHAR *szBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT *weightScale, FLOAT *weightPosition, FLOAT *weightOrientation) const; // 获得自定义骨骼混合(模型坐标系)
		VOID GetBoneCustomBlend(DWORD dwBoneName, VEC3 *scale, VEC3 *position, QUAT *orientation, FLOAT *weightScale, FLOAT *weightPosition, FLOAT *weightOrientation) const; // 获得自定义骨骼混合(模型坐标系)

		VOID SetBoneCustomOrientation(const CHAR *szBoneName, QUAT *orientation);                  // 设置自定义骨骼朝向(模型坐标系)
		VOID SetBoneCustomOrientation(DWORD dwBoneName, QUAT *orientation);                        // 设置自定义骨骼朝向(模型坐标系)

		VOID GetBoneCustomOrientation(const CHAR *szBoneName, QUAT *orientation) const;            // 获得自定义骨骼朝向(模型坐标系)
		VOID GetBoneCustomOrientation(DWORD dwBoneName, QUAT *orientation) const;                  // 获得自定义骨骼朝向(模型坐标系)

		MATRIX4* GetBoneTransform(INT indexBone, MATRIX4 *mtxTransform) const;                     // 获得变换矩阵

	public:
		VOID NextFrame(FLOAT deltaTime);                                                           // 更新到下一帧

	protected:
		VOID UpdateBone(BONE *pBoneBuffer, const INT *pBoneParents, INT indexBone) const;          // 更新骨骼
		VOID ConnectBone(BONE *pBone, const BONE *pBoneChild, const BONE *pBoneParent) const;      // 父子骨骼及联

	public:
		virtual VOID Render(const CMaterial *pMaterial) const;                                     // 渲染
		virtual VOID RenderOcclude(VOID) const;                                                    // 渲染遮挡


		// 属性
	protected:
		MATRIX4 *m_boneTransforms;                                                                 // 骨骼变换矩阵集合

		BONE_BLEND *m_boneCustomBlends;                                                            // 自定义骨骼混合(模型坐标系)集合
		CustomBlendMap m_boneCustomBlendMap;                                                       // 自定义骨骼混合(模型坐标系)映射

		QUAT *m_boneCustomOrientations;                                                            // 自定义骨骼朝向(模型坐标系)集合
		CustomOrientationMap m_boneCustomOrientationsMap;                                          // 自定义骨骼朝向(模型坐标系)映射

	protected:
		AnimationStateMap m_animationStates;                                                       // 动画集合
	};

}

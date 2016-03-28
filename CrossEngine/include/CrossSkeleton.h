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

	typedef struct {
		BOOL bUpdate;                                                                              // 更新标记

		VEC3 scale;                                                                                // 缩放
		VEC3 position;                                                                             // 位置
		QUAT orientation;                                                                          // 旋转
	} BONE;

	typedef struct {
		FLOAT weightScale;                                                                         // 缩放权重
		FLOAT weightPosition;                                                                      // 位置权重
		FLOAT weightOrientation;                                                                   // 旋转权重

		VEC3 scale;                                                                                // 缩放
		VEC3 position;                                                                             // 位置
		QUAT orientation;                                                                          // 旋转
	} BONE_BLEND;

	typedef struct {
		CHAR szName[64];                                                                           // 骨骼名
		DWORD dwName;                                                                              // 骨骼名
	} BONE_NAME;

	typedef struct {
		INT *pParentIDs;                                                                           // 骨骼父节点索引集合
		BONE_NAME *pBoneNames;                                                                     // 骨骼名集合
		BONE *pBoneWorld2Locals;                                                                   // 初始骨骼世界坐标变换到本地坐标集合
		BONE *pBoneLocal2Parents;                                                                  // 初始骨骼本地坐标变换到父亲坐标集合
	} BONE_HIERARCHY;

	class _CrossExport CAnimSkeleton
	{
		friend class CSkeleton;


		// 构造/析构函数
	protected:
		CAnimSkeleton(VOID);
		virtual ~CAnimSkeleton(VOID);


		// 方法
	public:
		const CHAR* GetName(VOID) const;                                                           // 获得名称
		DWORD GetHashName(VOID) const;                                                             // 获得名称

		FLOAT GetLength(VOID) const;                                                               // 获得时长

		INT GetKeyFrameCount(VOID) const;                                                          // 获得关键帧数
		FLOAT GetKeyFrameTime(INT indexFrame) const;                                               // 获得关键帧时间
		const BONE* GetKeyFrameData(INT indexFrame) const;                                         // 获得关键帧数据


		// 属性
	protected:
		CHAR *m_szName;                                                                            // 名称
		DWORD m_dwName;                                                                            // 名称

		FLOAT m_length;                                                                            // 时长(秒)

		INT m_numKeyFrames;                                                                        // 关键帧数
		FLOAT *m_pTimeKeyFrames;                                                                   // 关键帧时间
		BONE **m_ppDataKeyFrames;                                                                  // 关键帧数据
	};

	class _CrossExport CSkeleton : public CResource
	{
		friend class CSkeletonPtr;
		friend class CSkeletonManager;


		// 构造/析构函数
	protected:
		CSkeleton(CResourceManager *pResourceManager);
		virtual ~CSkeleton(VOID);


		// 方法
	public:
		virtual VOID Init(VOID);                                                                   // 初始化资源
		virtual VOID Free(VOID);                                                                   // 释放资源

	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型
		virtual BOOL IsValid(VOID) const;                                                          // 有效性检查

	public:
		virtual BOOL CopyFrom(const CResource *pCopyFrom);                                         // 复制资源
		virtual BOOL LoadFromStream(CStream *pStream);                                             // 从数据流加载资源

	protected:
		VOID LoadHierarchy(CStream *pStream);                                                      // 加载骨架
		VOID LoadAnimation(CStream *pStream);                                                      // 加载动画

	public:
		INT GetBoneCount(VOID) const;                                                              // 获得骨骼数
		INT GetAnimationCount(VOID) const;                                                         // 获得动画数

		const CAnimSkeleton* GetAnimation(INT index) const;                                        // 获得动画
		const BONE_HIERARCHY* GetBoneHierarchy(VOID) const;                                        // 获得骨架


		// 属性
	protected:
		INT m_numBones;                                                                            // 骨骼数
		INT m_numAnimations;                                                                       // 动画数

		BONE_HIERARCHY m_hierarchy;                                                                // 骨架
		CAnimSkeleton *m_pAnimations;                                                              // 动画集合
	};

	class _CrossExport CSkeletonPtr : public CSharedPtr<CSkeleton>
	{
		// 构造/析构函数
	public:
		CSkeletonPtr(VOID) : CSharedPtr<CSkeleton>() {}
		CSkeletonPtr(const CSkeleton *pSkeleton) : CSharedPtr<CSkeleton>(pSkeleton) {}
		CSkeletonPtr(const CSkeletonPtr &ptrSkeleton) : CSharedPtr<CSkeleton>(ptrSkeleton) {}
		CSkeletonPtr(const CResourcePtr &ptrResource) { Set(static_cast<CSkeleton*>(ptrResource.GetPointer()), ptrResource.GetRefCount()); }
		virtual ~CSkeletonPtr(VOID) {}


		// 方法
	protected:
		virtual VOID FreePointer(VOID)
		{
			if (m_pPointer) {
				m_pPointer->GetResourceManager()->DestroyResource(m_pPointer);
			}
		}

	public:
		CSkeletonPtr& operator = (const CResourcePtr &ptrResource)
		{
			Set(static_cast<CSkeleton*>(ptrResource.GetPointer()), ptrResource.GetRefCount());
			return *this;
		}

		operator CResource* ()
		{
			return (CResource *)m_pPointer;
		}

		operator const CResource* () const
		{
			return (const CResource *)m_pPointer;
		}
	};

}

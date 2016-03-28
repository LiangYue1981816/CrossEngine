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

	class _CrossExport CEffect : public CResource
	{
		friend class CEffectPtr;
		friend class CEffectManager;


		// 数据结构
	public:
		typedef struct {
			CHAR szName[_MAX_STRING];                                                              // 名称

			FLOAT beginTime;                                                                       // 粒子系统开始时间
			FLOAT durationTime;                                                                    // 粒子系统持续时间

			CParticleSystem *pParticleSystem;                                                      // 粒子系统
		} PARTICLESYSTEM;

		typedef struct {
			CHAR szName[_MAX_STRING];                                                              // 名称

			FLOAT beginTime;                                                                       // 声音开始时间
			FLOAT durationTime;                                                                    // 声音持续时间

			BOOL bLoop;                                                                            // 循环播放
			BOOL b3D;                                                                              // 3D音效
			FLOAT volume;                                                                          // 音量

			CSoundPtr ptrSound;                                                                    // 声音源
			FMOD_CHANNEL *pChannel;                                                                // 声音通道
		} SOUND;

		typedef struct {
			CHAR szName[_MAX_STRING];                                                              // 名称

			FLOAT beginTime;                                                                       // 震动开始时间
			FLOAT durationTime;                                                                    // 震动持续时间

			VEC3 minShake;                                                                         // 最小震动范围
			VEC3 maxShake;                                                                         // 最大震动范围

			FLOAT constantAttenuation;                                                             // 常量衰减系数
			FLOAT linearAttenuation;                                                               // 线性衰减系数
			FLOAT quadraticAttenuation;                                                            // 平方衰减系数
		} SHAKE;

		typedef std::map<DWORD, PARTICLESYSTEM*> ParticleSystemMap;                                // 粒子系统集合
		typedef std::map<DWORD, SOUND*> SoundMap;                                                  // 声音集合
		typedef std::map<DWORD, SHAKE*> ShakeMap;                                                  // 震动集合


		// 构造/析构函数
	protected:
		CEffect(CResourceManager *pResourceManager);
		virtual ~CEffect(VOID);


		// 方法
	public:
		virtual VOID Init(VOID);                                                                   // 初始化资源
		virtual VOID Free(VOID);                                                                   // 释放资源

	public:
		virtual TYPE GetType(VOID) const;                                                          // 获得类型
		virtual BOOL IsValid(VOID) const;                                                          // 有效性检查

	public:
		virtual BOOL LoadFromStream(CStream *pStream);                                             // 从数据流加载资源
		virtual BOOL SaveToFileStream(FILE *pFile);                                                // 保存到文件数据流

	protected:
		VOID LoadAABB(TiXmlNode *pEffectNode);                                                     // 加载包围盒
		VOID LoadParticleSystems(TiXmlNode *pEffectNode);                                          // 加载粒子系统
		VOID LoadSounds(TiXmlNode *pEffectNode);                                                   // 加载声音
		VOID LoadShakes(TiXmlNode *pEffectNode);                                                   // 加载震动

		VOID SaveAABB(TiXmlNode *pEffectNode);                                                     // 保存包围盒
		VOID SaveParticleSystems(TiXmlNode *pEffectNode);                                          // 保存粒子系统
		VOID SaveSounds(TiXmlNode *pEffectNode);                                                   // 保存声音
		VOID SaveShakes(TiXmlNode *pEffectNode);                                                   // 保存震动

	public:
		VOID SetAABB(FLOAT width, FLOAT height, FLOAT length);                                     // 设置包围盒尺寸
		VOID SetAABBCenter(FLOAT x, FLOAT y, FLOAT z);                                             // 设置包围盒中心
		const AABB* GetAABB(VOID) const;                                                           // 获得包围盒

	public:
		PARTICLESYSTEM* CreateParticleSystem(const CHAR *szName);                                  // 创建粒子系统
		VOID DestroyParticleSystem(const CHAR *szName);                                            // 销毁粒子系统
		PARTICLESYSTEM* GetParticleSystem(const CHAR *szName) const;                               // 获得粒子系统

		SOUND* CreateSound(const CHAR *szName);                                                    // 创建声音
		VOID DestroySound(const CHAR *szName);                                                     // 销毁声音
		SOUND* GetSound(const CHAR *szName) const;                                                 // 获得声音

		SHAKE* CreateShake(const CHAR *szName);                                                    // 创建震动
		VOID DestroyShake(const CHAR *szName);                                                     // 销毁震动
		SHAKE* GetShake(const CHAR *szName) const;                                                 // 获得震动

		const ParticleSystemMap& GetParticleSystems(VOID) const;                                   // 获得粒子系统集合
		const SoundMap& GetSounds(VOID) const;                                                     // 获得声音集合
		const ShakeMap& GetShakes(VOID) const;                                                     // 获得震动集合


		// 属性
	protected:
		AABB m_aabb;                                                                               // 包围盒

		ParticleSystemMap m_particleSystems;                                                       // 粒子系统集合
		SoundMap m_sounds;                                                                         // 声音集合
		ShakeMap m_shakes;                                                                         // 震动集合
	};

	class _CrossExport CEffectPtr : public CSharedPtr<CEffect>
	{
		// 构造/析构函数
	public:
		CEffectPtr(VOID) : CSharedPtr<CEffect>() {}
		CEffectPtr(const CEffect *pEffect) : CSharedPtr<CEffect>(pEffect) {}
		CEffectPtr(const CEffectPtr &ptrEffect) : CSharedPtr<CEffect>(ptrEffect) {}
		CEffectPtr(const CResourcePtr &ptrResource) { Set(static_cast<CEffect*>(ptrResource.GetPointer()), ptrResource.GetRefCount()); }
		virtual ~CEffectPtr(VOID) {}


		// 方法
	protected:
		virtual VOID FreePointer(VOID)
		{
			if (m_pPointer) {
				m_pPointer->GetResourceManager()->DestroyResource(m_pPointer);
			}
		}

	public:
		CEffectPtr& operator = (const CResourcePtr &ptrResource)
		{
			Set(static_cast<CEffect*>(ptrResource.GetPointer()), ptrResource.GetRefCount());
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

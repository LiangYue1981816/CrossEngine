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

	class _CrossExport CParticleSystem
	{
		// 数据结构
	public:
		typedef std::vector<CEmitterBase*> EmitterSet;                                             // 发射器集合
		typedef std::vector<CAffectorBase*> AffectorSet;                                           // 影响器集合


		// 构造/析构函数
	public:
		CParticleSystem(VOID);
		virtual ~CParticleSystem(VOID);


		// 方法
	public:
		virtual VOID Init(VOID);                                                                   // 初始化
		virtual VOID Free(VOID);                                                                   // 释放

		virtual VOID LoadXML(TiXmlNode *pParticleSystemNode);                                      // 加载XML
		virtual VOID SaveXML(TiXmlNode *pParticleSystemNode) const;                                // 保存XML

	protected:
		VOID LoadGfx(TiXmlNode *pParticleSystemNode);                                              // 加载图形
		VOID LoadParticle(TiXmlNode *pParticleSystemNode);                                         // 加载粒子
		VOID LoadMaterial(TiXmlNode *pParticleSystemNode);                                         // 加载材质
		VOID LoadEmitters(TiXmlNode *pParticleSystemNode);                                         // 加载发射器
		VOID LoadAffectors(TiXmlNode *pParticleSystemNode);                                        // 加载影响器

		VOID SaveGfx(TiXmlNode *pParticleSystemNode) const;                                        // 保存图形
		VOID SaveParticle(TiXmlNode *pParticleSystemNode) const;                                   // 保存粒子
		VOID SaveMaterial(TiXmlNode *pParticleSystemNode) const;                                   // 保存材质
		VOID SaveEmitters(TiXmlNode *pParticleSystemNode) const;                                   // 保存发射器
		VOID SaveAffectors(TiXmlNode *pParticleSystemNode) const;                                  // 保存影响器

	public:
		VOID SetParticleQuota(INT quota);                                                          // 设置粒子总量

		INT GetParticleQuota(VOID) const;                                                          // 获得粒子总量
		INT GetActiveParticleCount(VOID) const;                                                    // 获得活动粒子数
		INT GetFreeParticleCount(VOID) const;                                                      // 获得空闲粒子数

	public:
		CGfxBase* CreateGfx(CGfxBase::TYPE type);                                                  // 创建图形
		CGfxBase* GetGfx(VOID) const;                                                              // 获得图形

	public:
		CEmitterBase* CreateEmitter(CEmitterBase::TYPE type);                                      // 创建发射器
		VOID DestroyEmitter(CEmitterBase *pEmitter);                                               // 销毁发射器

		INT GetEmitterCount(VOID) const;                                                           // 获得发射器数量
		CEmitterBase* GetEmitter(INT indexEmitter) const;                                          // 获得发射器

	public:
		CAffectorBase* CreateAffector(CAffectorBase::TYPE type);                                   // 创建影响器
		VOID DestroyAffector(CAffectorBase *pAffector);                                            // 销毁影响器

		INT GetAffectorCount(VOID) const;                                                          // 获得影响器数量
		CAffectorBase* GetAffector(INT indexAffector) const;                                       // 获得影响器

	public:
		BOOL SetMaterial(const CHAR *szName, BOOL bClone);                                         // 设置材质
		BOOL SetTexture(const CHAR *szName, INT indexTexUnit);                                     // 设置纹理单元纹理

		const CMaterialPtr& GetMaterial(VOID) const;                                               // 获得材质
		const CTexturePtr& GetTexture(INT indexTexUnit) const;                                     // 获得纹理单元纹理

		VOID SetRenderID(INT renderID);                                                            // 设置渲染ID
		VOID SetBlend(UINT srcFactor, UINT desFactor);                                             // 设置混合模式
		VOID SetTexWarpU(INT indexTexUnit, UINT warp);                                             // 设置纹理横向寻址方式
		VOID SetTexWarpV(INT indexTexUnit, UINT warp);                                             // 设置纹理纵向寻址方式

		INT GetRenderID(VOID) const;                                                               // 获得渲染ID
		UINT GetBlendSrcFactor(VOID) const;                                                        // 获得混合模式
		UINT GetBlendDesFactor(VOID) const;                                                        // 获得混合模式
		UINT GetTexWarpU(INT indexTexUnit) const;                                                  // 获得纹理横向寻址方式
		UINT GetTexWarpV(INT indexTexUnit) const;                                                  // 获得纹理纵向寻址方式

	public:
		VOID SetScale(FLOAT x, FLOAT y, FLOAT z);                                                  // 设置缩放
		VOID SetScale(const VEC3 *scale);                                                          // 设置缩放
		const VEC3* GetScale(VOID) const;                                                          // 获得缩放

		VOID SetPosition(FLOAT x, FLOAT y, FLOAT z);                                               // 设置位置
		VOID SetPosition(const VEC3 *position);                                                    // 设置位置
		const VEC3* GetPosition(VOID) const;                                                       // 获得位置

		VOID SetDirection(FLOAT x, FLOAT y, FLOAT z);                                              // 设置朝向
		VOID SetDirection(const VEC3 *direction);                                                  // 设置朝向
		VOID SetOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w);                                   // 设置朝向
		VOID SetOrientation(const QUAT *orientation);                                              // 设置朝向
		const QUAT* GetOrientation(VOID) const;                                                    // 获得朝向

	public:
		VOID Play(BOOL bImmediately);                                                              // 播放
		VOID Stop(BOOL bImmediately = FALSE);                                                      // 停止

		VOID Pause(VOID);                                                                          // 暂停
		VOID Resume(VOID);                                                                         // 继续

		BOOL IsPlaying(VOID);                                                                      // 播放状态
		BOOL IsPlayFinish(VOID);                                                                   // 播放结束

	public:
		VOID Update(FLOAT deltaTime);                                                              // 更新
		VOID UpdateGfx(const CEntityCamera *pCamera);                                              // 更新图形

	public:
		VOID Render(const CEntityCamera *pCamera);                                                 // 渲染

	protected:
		VOID UpdateMaterial(FLOAT deltaTime);                                                      // 更新材质
		VOID UpdateParticles(FLOAT deltaTime);                                                     // 更新粒子
		VOID UpdateEmitter(FLOAT deltaTime);                                                       // 更新发射器
		VOID UpdateAffector(FLOAT deltaTime);                                                      // 更新影响器


		// 属性
	protected:
		CParticle::TYPE m_particleType;                                                            // 粒子类型

		INT m_numParticleQuota;                                                                    // 粒子总数
		INT m_numActiveParticles;                                                                  // 活动粒子数

		CGfxBase *m_pGfx;                                                                          // 图形
		CParticle *m_pParticles;                                                                   // 粒子池
		CParticle *m_pActiveParticleList;                                                          // 活动粒子链表
		CParticle *m_pFreeParticleList;                                                            // 空闲粒子链表

		EmitterSet m_emitters;                                                                     // 发射器集合
		AffectorSet m_affectors;                                                                   // 影响器集合

		VEC3 m_scale;                                                                              // 缩放
		VEC3 m_position;                                                                           // 位置
		QUAT m_orientation;                                                                        // 朝向

	protected:
		CMaterialPtr m_ptrMaterial;                                                                // 材质
		BOOL m_bCloneMaterial;                                                                     // 粒子材质需克隆
		BOOL m_bUpdateMaterial;                                                                    // 粒子材质需更新
		CHAR m_szMaterialName[_MAX_STRING];                                                        // 粒子材质名

		CTexturePtr m_ptrTextures[MAX_TEXUNITS];                                                   // 纹理单元
		UINT m_texWrapU[MAX_TEXUNITS];                                                             // 纹理坐标横向环绕方式
		UINT m_texWrapV[MAX_TEXUNITS];                                                             // 纹理坐标纵向环绕方式

		INT m_renderID;                                                                            // 渲染ID
		UINT m_blendSrcFactor;                                                                     // 混合模式
		UINT m_blendDesFactor;                                                                     // 混合模式

	protected:
		BOOL m_bPlaying;                                                                           // 播放状态
		BOOL m_bPause;                                                                             // 暂停播放状态
	};

}

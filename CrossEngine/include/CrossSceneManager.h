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

	class _CrossExport CEntity;
	class _CrossExport CSceneManager
	{
		friend class CSceneSystem;


		// ���ݽṹ
	public:
		typedef std::map<DWORD, CEntity*> EntityMap;                                               // ʵ�弯��
		typedef std::map<DWORD, CSceneNode*> SceneNodeMap;                                         // �ڵ㼯��


		// ����/��������
	protected:
		CSceneManager(VOID);
		virtual ~CSceneManager(VOID);


		// ����
	public:
		virtual VOID Init(VOID);                                                                   // ��ʼ������������
		virtual VOID Free(VOID);                                                                   // �ͷų���������

	private:
		virtual VOID FreeNodes(VOID);                                                              // �ͷŽ�㼯��
		virtual VOID FreeEntities(EntityMap &entities);                                            // �ͷ�ʵ�弯��

	public:
		virtual BOOL LoadFromFile(const CHAR *szFileName);                                         // ���ļ�������Դ
		virtual BOOL LoadFromZip(const CHAR *szZipName, const CHAR *szFileName);                   // ��ѹ����������Դ
		virtual BOOL LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName);                      // ��ѹ�����ļ�����������
		virtual BOOL LoadFromStream(CStream *pStream);                                             // ��������������Դ

	protected:
		virtual BOOL CreateRenderScene(VOID);                                                      // ������Ⱦ����
		virtual BOOL CreatePhysicsScene(VOID);                                                     // ����������

	public:
		virtual VOID SetVisibleFlags(DWORD dwFlags);                                               // ���ÿɼ��Ա�ʶ
		virtual DWORD GetVisibleFlags(VOID) const;                                                 // ��ÿɼ��Ա�ʶ

	public:
		virtual CSceneNode* GetRootNode(VOID) const;                                               // ��ø��ڵ�

		virtual CSceneNode* GetMainCameraNode(VOID) const;                                         // ���������ڵ�
		virtual CEntityCamera* GetMainCameraEntity(VOID) const;                                    // ��������ʵ��

		virtual CSceneNode* GetMainShadowNode(VOID) const;                                         // �������Ӱ�ڵ�
		virtual CEntityShadow* GetMainShadowEntity(VOID) const;                                    // �������Ӱʵ��

	public:
		virtual CPhysicsManager& GetPhysics(VOID);                                                 // �����������

	public:
		virtual CSceneNode* CreateNode(const CHAR *szName);                                        // �����ڵ�
		virtual CSceneNode* CreateNode(DWORD dwName);                                              // �����ڵ�

		virtual VOID DestroyNode(CSceneNode *pNode);                                               // ���ٽڵ�

		virtual CSceneNode* GetNode(const CHAR *szName) const;                                     // ��ýڵ�
		virtual CSceneNode* GetNode(DWORD dwName) const;                                           // ��ýڵ�

	public:
		virtual CEntity* CreateEntity(const CHAR *szName, CEntity::TYPE type);                     // ����ʵ��
		virtual CEntity* CreateEntity(DWORD dwName, CEntity::TYPE type);                           // ����ʵ��

		virtual VOID DestroyEntity(CEntity *pEntity);                                              // ����ʵ��

		virtual CEntity* GetEntity(const CHAR *szName, CEntity::TYPE type) const;                  // ���ʵ��
		virtual CEntity* GetEntity(DWORD dwName, CEntity::TYPE type) const;                        // ���ʵ��

	public:
		virtual BOOL IsOccluded(INT index) const;                                                  // �ڵ��ж�
		virtual BOOL GetLightParams(DWORD dwChannel, const VEC3 *position, VEC4 *direction,        // ��ù��ղ���
			VEC3 *ambient = NULL, 
			VEC3 *diffuse = NULL, 
			VEC3 *specular = NULL, 
			VEC3 *rim = NULL, 
			VEC3 *skyLower = NULL, 
			VEC3 *skyUpper = NULL, 
			VEC3 *indirectUp = NULL, 
			VEC3 *indirectDown = NULL, 
			VEC3 *indirectLeft = NULL, 
			VEC3 *indirectRight = NULL, 
			VEC3 *indirectFront = NULL,
			VEC3 *indirectBack = NULL) const;

	public:
		virtual VOID AddSceneRenderQuene(const CEntityCamera *pCamera, CEntityRenderable *pEntity);// ��ӳ�����Ⱦ����
		virtual VOID AddSceneRenderQuene(const CEntityCamera *pCamera, CEntityRenderable *pEntity, CMaterial *pMaterial); // ��ӳ�����Ⱦ����
		virtual VOID AddLightRenderQuene(const CEntityCamera *pCamera, CEntityLight *pEntity);     // ��ӹ�Դ��Ⱦ����

	public:
		virtual VOID Update(FLOAT deltaTime);                                                      // ����
		virtual VOID Render(CEntityCamera *pCamera = NULL);                                        // ��Ⱦ


		// ����
	protected:
		DWORD m_dwVisibleFlags;                                                                    // �ɼ���ʶ

	protected:
		EntityMap m_meshs;                                                                         // ���񼯺�
		EntityMap m_skins;                                                                         // ��Ƥ����
		EntityMap m_effects;                                                                       // ��Ч����
		EntityMap m_halos;                                                                         // ���μ���
		EntityMap m_lights;                                                                        // ��Դ����

		SceneNodeMap m_nodes;                                                                      // �ڵ㼯��

	protected:
		CSceneNode *m_pRootNode;                                                                   // ���ڵ�
		CSceneNode *m_pMainCameraNode;                                                             // ������ڵ�
		CSceneNode *m_pMainShadowNode;                                                             // ����Ӱ�ڵ�
		CEntityCamera *m_pMainCameraEntity;                                                        // �����ʵ��
		CEntityShadow *m_pMainShadowEntity;                                                        // ����Ӱʵ��

	protected:
		CLightGrid *m_pLightGrid;                                                                  // ��������
		COctreeCulling *m_pOctreeCulling;                                                          // �˲����޳�
		COcclusionCulling *m_pOcclusionCulling;                                                    // �ڵ��޳�

	protected:
		CPhysicsManager m_physics;                                                                 // ��������

	protected:
		CStream m_stream;                                                                          // ������
	};

}

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
	class _CrossExport CSceneNode
	{
		friend class CEntity;
		friend class CEntityLight;
		friend class CEntityRenderable;
		friend class CSceneManager;


		// ���ݽṹ
	public:
		typedef std::map<DWORD, CEntity*> EntityMap;                                               // ʵ�弯��
		typedef std::map<DWORD, CSceneNode*> SceneNodeMap;                                         // �����ڵ㼯��


		// ����/��������
	protected:
		CSceneNode(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CSceneNode(VOID);


		// ����
	public:
		DWORD GetName(VOID) const;                                                                 // �������

		CSceneNode* GetParentNode(VOID) const;                                                     // ��ø��ڵ�
		CSceneManager* GetSceneManager(VOID) const;                                                // ��ó���������

		const AABB* GetAABBCurrent(VOID) const;                                                    // ��õ�ǰ��Χ��
		const AABB* GetAABBCascade(VOID) const;                                                    // ��ü�����Χ��

	public:
		VOID SetVisible(BOOL bVisible, BOOL bCascade);                                             // ���ÿɼ���
		BOOL IsVisible(VOID) const;                                                                // ��ÿɼ���

		VOID SetRenderable(BOOL bNeedRender, BOOL bCascade);                                       // ���ÿ���Ⱦ��
		BOOL IsRenderable(VOID) const;                                                             // ��ÿ���Ⱦ��

	public:
		BOOL AttachEntity(CEntity *pEntity);                                                       // ����ʵ��
		VOID DetachEntity(CEntity *pEntity);                                                       // ж��ʵ��
		VOID DetachAllEntity(VOID);                                                                // ж������ʵ��

		CEntity* QueryEntity(const CHAR *szName) const;                                            // ��ѯ����ʵ��
		CEntity* QueryEntity(DWORD dwName) const;                                                  // ��ѯ����ʵ��

		CEntity* GetEntityListHead(VOID) const;                                                    // ���ʵ������ͷ

	public:
		BOOL AttachNode(CSceneNode *pNode);                                                        // ����ڵ�
		VOID DetachNode(CSceneNode *pNode);                                                        // ж�ؽڵ�
		VOID DetachAllNode(VOID);                                                                  // ж�����нڵ�

		CSceneNode* QueryChildNode(const CHAR *szName) const;                                      // ��ѯ�ӽڵ�
		CSceneNode* QueryChildNode(DWORD dwName) const;                                            // ��ѯ�ӽڵ�

		CSceneNode* GetNodeListHead(VOID) const;                                                   // ��ýڵ�����ͷ

	public:
		VOID Identity(VOID);                                                                       // ��ʼ��

		VOID SetLocalScale(FLOAT x, FLOAT y, FLOAT z);                                             // ���ñ�������
		VOID SetLocalPosition(FLOAT x, FLOAT y, FLOAT z);                                          // ���ñ���λ��
		VOID SetLocalOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w);                              // ���ñ��س���
		VOID SetLocalDirection(FLOAT x, FLOAT y, FLOAT z, FLOAT upx = 0.0f, FLOAT upy = 1.0f, FLOAT upz = 0.0f); // ���ñ��س���

		VOID SetWorldScale(FLOAT x, FLOAT y, FLOAT z);                                             // ������������
		VOID SetWorldPosition(FLOAT x, FLOAT y, FLOAT z);                                          // ��������λ��
		VOID SetWorldOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w);                              // �������糯��
		VOID SetWorldDirection(FLOAT x, FLOAT y, FLOAT z, FLOAT upx = 0.0f, FLOAT upy = 1.0f, FLOAT upz = 0.0f); // ���ñ��س���

		const VEC3* GetLocalScale(VOID);                                                           // ��ñ�������
		const VEC3* GetLocalPosition(VOID);                                                        // ��ñ���λ��
		const QUAT* GetLocalOrientation(VOID);                                                     // ��ñ��س���
		const MATRIX4* GetLocalMatrix(VOID);                                                       // ��ñ��ر任����

		const VEC3* GetWorldScale(VOID);                                                           // �����������
		const VEC3* GetWorldPosition(VOID);                                                        // �������λ��
		const QUAT* GetWorldOrientation(VOID);                                                     // ������糯��
		const MATRIX4* GetWorldMatrix(VOID);                                                       // �������任����

	protected:
		VOID UpdateTransformImmediately(VOID);                                                     // �������±任
		VOID UpdateTransform(BOOL bParentUpdate = FALSE);                                          // ���±任
		BOOL UpdateAABB(BOOL bParentUpdate = FALSE);                                               // ���°�Χ��
		VOID UpdateEntity(FLOAT deltaTime);                                                        // ����ʵ��
		VOID UpdateRenderQuene(const CEntityCamera *pCamera);                                      // ������Ⱦ����


		// ����
	public:
		VOID *userData;                                                                            // �û�����

	protected:
		DWORD m_dwName;                                                                            // ����

		BOOL m_bVisible;                                                                           // �ɼ���
		BOOL m_bNeedRender;                                                                        // ��Ҫ��Ⱦ��
		BOOL m_bNeedUpdateAABB;                                                                    // ��Ҫ���°�Χ��
		BOOL m_bNeedUpdateTransform;                                                               // ��Ҫ���±任
		DWORD m_dwUpdateTransformCount;                                                            // ��ǰ�ڵ���±任����
		DWORD m_dwParentUpdateTransformCount;                                                      // ���׽ڵ���±任����

		VEC3 m_localScale;                                                                         // ��������
		VEC3 m_localPosition;                                                                      // ����λ��
		QUAT m_localOrientation;                                                                   // ���س���
		MATRIX4 m_mtxLocal;                                                                        // ���ر任����

		VEC3 m_worldScale;                                                                         // ��������
		VEC3 m_worldPosition;                                                                      // ����λ��
		QUAT m_worldOrientation;                                                                   // ���糯��
		MATRIX4 m_mtxWorld;                                                                        // ����任����

		AABB m_aabbCurrent;                                                                        // ��ǰ��Χ��
		AABB m_aabbCascade;                                                                        // ������Χ��

	protected:
		EntityMap m_entities;                                                                      // ʵ�弯��
		SceneNodeMap m_nodes;                                                                      // �ڵ㼯��

		CEntity *m_pEntityHead;                                                                    // ʵ������
		CSceneNode *m_pNodeHead;                                                                   // �ڵ�����

	protected:
		CSceneNode *m_pParentNode;                                                                 // ���ڵ�ָ��
		CSceneManager *m_pSceneManager;                                                            // ����������

	public:
		CSceneNode *pNext;                                                                         // �ڵ�����
	};

}

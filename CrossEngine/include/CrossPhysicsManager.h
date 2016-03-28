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

	class _CrossExport CPhysicsManager
	{
		friend class CSceneManager;


		// ���ݽṹ
	public:
		typedef std::map<DWORD, PxActor*> PxActorMap;                                              // ��ɫ����


		// ����/��������
	protected:
		CPhysicsManager(VOID);
		virtual ~CPhysicsManager(VOID);


		// ����
	public:
		VOID CreateScene(VOID);                                                                    // ��������
		VOID DestroyScene(VOID);                                                                   // ���ٳ���

		PxScene* GetScene(VOID) const;                                                             // ��ó���
		PxControllerManager* GetControllerManager(VOID) const;                                     // ��ÿ�����������

	public:
		PxActor* CreateRigidActor(const CHAR *szName, const PxTransform &pxGlobalPose, BOOL bDynamic, BOOL bKinematic = FALSE, BOOL bEnableCCD = FALSE); // ���������ɫ
		PxActor* CreateRigidActor(DWORD dwName, const PxTransform &pxGlobalPose, BOOL bDynamic, BOOL bKinematic = FALSE, BOOL bEnableCCD = FALSE); // ���������ɫ
		VOID DestroyActor(DWORD dwName);                                                           // ���ٽ�ɫ
		PxActor* GetActor(DWORD dwName);                                                           // ��ý�ɫ

	public:
		VOID EnableQuery(DWORD dwName, BOOL bEnable);                                              // ���ò�ѯ״̬
		VOID EnableSimulation(DWORD dwName, BOOL bEnable);                                         // ����ģ��״̬
		VOID EnableKinematic(DWORD dwName, BOOL bEnable);                                          // �����˶�״̬
		VOID EnableCCD(DWORD dwName, BOOL bEnable);                                                // ����CCD״̬

	public:
		DWORD RayCastScene(DWORD dwQueryFlags, const RAY *pRay, FLOAT *length, VEC3 *position, VEC3 *normal, VOID **ppActorData, VOID **ppShapeData) const; // ���߼�ⳡ��

	public:
		VOID Begin(VOID);                                                                          // ����ʼ
		VOID End(PxReal deltaTime);                                                                // �������

		
		// ����
	protected:
		PxActorMap m_actors;                                                                       // ��ɫ����

	protected:
		PxScene *m_pPxScene;                                                                       // ������
		PxControllerManager *m_pPxControllerManager;                                               // ������ƹ�����
		PxDefaultCpuDispatcher *m_pPxCpuDispatcher;                                                // ������
	};

}

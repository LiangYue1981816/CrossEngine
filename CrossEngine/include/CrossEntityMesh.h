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

	class _CrossExport CEntityMesh : public CEntityRenderable
	{
		friend class CSceneManager;


		// ����/��������
	protected:
		CEntityMesh(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntityMesh(VOID);


		// ����
	public:
		virtual TYPE GetType(VOID) const;                                                          // �������
		virtual BOOL IsRenderable(VOID) const;                                                     // ��ÿ���Ⱦ

	public:
		virtual BOOL SetMesh(const CHAR *szName);                                                  // ��������
		virtual BOOL SetMesh(DWORD dwName);                                                        // ��������
		virtual BOOL SetMesh(const CMeshPtr &ptrMesh);                                             // ��������

		BOOL SetRenderSubMesh(const CHAR *szName);                                                 // ������Ⱦ������
		BOOL SetRenderSubMesh(DWORD dwName);                                                       // ������Ⱦ������

		BOOL SetPhysicsSubMesh(const CHAR *szName);                                                // ��������������
		BOOL SetPhysicsSubMesh(DWORD dwName);                                                      // ��������������

		const CMeshPtr& GetMesh(VOID) const;                                                       // �������
		const CSubMesh* GetRenderSubMesh(VOID) const;                                              // �����Ⱦ������
		const CSubMesh* GetPhysicsSubMesh(VOID) const;                                             // �������������

	public:
		BOOL SetDiffuseMaterial(const CHAR *szName);                                               // �������������
		BOOL SetDiffuseMaterial(DWORD dwName);                                                     // �������������
		BOOL SetDiffuseMaterial(const CMaterialPtr &ptrMaterial);                                  // �������������

		BOOL SetPhysicsMaterial(const CHAR *szName);                                               // �����������
		BOOL SetPhysicsMaterial(DWORD dwName);                                                     // �����������
		BOOL SetPhysicsMaterial(const CMaterialPtr &ptrMaterial);                                  // �����������

		BOOL SetLightMapTexture(const CHAR *szName, FLOAT offsetu = 0.0f, FLOAT offsetv = 0.0f, FLOAT scaleu = 1.0f, FLOAT scalev = 1.0f); // ���ù�����ͼ����
		BOOL SetLightMapTexture(DWORD dwName, FLOAT offsetu = 0.0f, FLOAT offsetv = 0.0f, FLOAT scaleu = 1.0f, FLOAT scalev = 1.0f); // ���ù�����ͼ����
		BOOL SetLightMapTexture(const CTexturePtr &ptrTexture, FLOAT offsetu = 0.0f, FLOAT offsetv = 0.0f, FLOAT scaleu = 1.0f, FLOAT scalev = 1.0f); // ���ù�����ͼ����

		const CMaterialPtr& GetMaterial(VOID) const;                                               // ��ò���
		const CMaterialPtr& GetPhysicsMaterial(VOID) const;                                        // ����������
		const CTexturePtr& GetLightMapTexture(VOID) const;                                         // ��ù�����ͼ����
		
	public:
		BOOL SetTextureMatrix(INT indexTexUnit, const MATRIX4 *matrix);                            // �����������
		const MATRIX4* GetTextureMatrix(INT indexTexUnit) const;                                   // ����������
		const MATRIX4* GetLightMapTextureMatrix(VOID) const;                                       // ��ù�����ͼ�任����

	public:
		VOID SetEnablePhysicsQuery(BOOL bEnable);                                                  // ���������ѯ״̬
		VOID SetEnablePhysicsSimulation(BOOL bEnable);                                             // ��������ģ��״̬
		VOID SetPhysicsSimulationTypeIndex(INT index);                                             // ��������ģ����������

		BOOL IsEnablePhysicsQuery(VOID) const;                                                     // ��������ѯ״̬
		BOOL IsEnablePhysicsSimulation(VOID) const;                                                // �������ģ��״̬
		INT GetPhysicsSimulationTypeIndex(VOID) const;                                             // �������ģ����������

	public:
		virtual VOID Render(const CMaterial *pMaterial) const;                                     // ��Ⱦ
		virtual VOID RenderOcclude(VOID) const;                                                    // ��Ⱦ�ڵ�


		// ����
	protected:
		DWORD m_dwRenderMeshName;                                                                  // ��Ⱦ������
		DWORD m_dwPhysicsMeshName;                                                                 // ����������

	protected:
		CMeshPtr m_ptrMesh;                                                                        // ����
		CMaterialPtr m_ptrDiffuseMaterial;                                                         // ���������
		CMaterialPtr m_ptrPhysicsMaterial;                                                         // �������
		CTexturePtr m_ptrLightMapTexture;                                                          // ������ͼ����

		MATRIX4 m_mtxTextures[MAX_TEXUNITS];                                                       // �������

	protected:
		BOOL m_bEnablePhysicsQuery;                                                                // �����ѯ״̬
		BOOL m_bEnablePhysicsSimulation;                                                           // ����ģ��״̬
		INT m_nPhysicsSimulationTypeIndex;                                                         // ����ģ����������
	};

}

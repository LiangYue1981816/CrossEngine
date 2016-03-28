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

	class _CrossExport CEntity
	{
		friend class CSceneNode;
		friend class CSceneManager;


		// ���ݽṹ
	public:
		typedef enum { CAMERA, MESH, SKIN, EFFECT, HALO, LIGHT, SHADOW } TYPE;                     // ʵ������


		// ����/��������
	protected:
		CEntity(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntity(VOID);


		// ����
	public:
		FLOAT GetTime(VOID) const;                                                                 // ���ʱ��
		DWORD GetName(VOID) const;                                                                 // �������

		CSceneNode* GetSceneNode(VOID) const;                                                      // ��ó����ڵ�
		CSceneManager* GetSceneManager(VOID) const;                                                // ��ó���������

	public:
		virtual TYPE GetType(VOID) const = 0;                                                      // �������
		virtual BOOL IsRenderable(VOID) const = 0;                                                 // ��ÿ���Ⱦ

	public:
		virtual VOID Update(FLOAT deltaTime);                                                      // ����
		virtual VOID UpdateInstance(FLOAT deltaTime);                                              // ����

	public:
		virtual VOID UpdateRenderQuene(const CEntityCamera *pCamera) {}                            // ������Ⱦ����


		// ����
	public:
		VOID *userData;                                                                            // �û�����

	protected:
		FLOAT m_time;                                                                              // ʱ��
		DWORD m_dwName;                                                                            // ����

	protected:
		CSceneNode *m_pSceneNode;                                                                  // �����ڵ�
		CSceneManager *m_pSceneManager;                                                            // ����������

	protected:
		DWORD m_dwUpdateFrameCount;                                                                // ����֡����

	public:
		CEntity *pNext;                                                                            // ʵ������
	};

}

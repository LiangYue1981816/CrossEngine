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

	class _CrossExport CAffectorBase
	{
		friend class CParticleSystem;


		// ���ݽṹ
	public:
		typedef enum { BASE, FORCE, COLOR, SCALE, ROTATE, RANDOM, TEXTURE, VELOCITY, KEEPLOCAL } TYPE;   // Ӱ��������


		// ����/��������
	protected:
		CAffectorBase(CParticleSystem *pParent);
		virtual ~CAffectorBase(VOID);


		// ����
	public:
		virtual TYPE GetType(VOID) const { return BASE;  }                                         // �������
		virtual CParticleSystem* GetParticleSystem(VOID) const;                                    // �������ϵͳ

		virtual VOID Init(VOID);                                                                   // ��ʼ��
		virtual VOID Free(VOID);                                                                   // �ͷ�

		virtual VOID LoadXML(TiXmlNode *pAffectorNode);                                            // ����XML
		virtual VOID SaveXML(TiXmlNode *pAffectorNode) const;                                      // ����XML

	protected:
		VOID LoadBeginTime(TiXmlNode *pAffectorNode);                                              // ���ؿ�ʼʱ��
		VOID LoadEndTime(TiXmlNode *pAffectorNode);                                                // ���ؽ���ʱ��
		VOID LoadStepTime(TiXmlNode *pAffectorNode);                                               // ���ز���ʱ��

		VOID SaveBeginTime(TiXmlNode *pAffectorNode, const CAffectorBase *pDefaultAffector) const; // ���濪ʼʱ��
		VOID SaveEndTime(TiXmlNode *pAffectorNode, const CAffectorBase *pDefaultAffector) const;   // �������ʱ��
		VOID SaveStepTime(TiXmlNode *pAffectorNode, const CAffectorBase *pDefaultAffector) const;  // ���沽��ʱ��


	public:
		VOID SetEnable(BOOL bEnable);                                                              // ��������״̬
		BOOL IsEnable(VOID) const;                                                                 // �������״̬

	public:
		VOID SetStep(FLOAT step);                                                                  // ������Ӧ���
		FLOAT GetStep(VOID) const;                                                                 // �����Ӧ���

		VOID SetBeginTime(FLOAT minTime, FLOAT maxTime);                                           // ���ÿ�ʼʱ��
		FLOAT GetMinBeginTime(VOID) const;                                                         // �����С��ʼʱ��
		FLOAT GetMaxBeginTime(VOID) const;                                                         // ������ʼʱ��

		VOID SetEndTime(FLOAT minTime, FLOAT maxTime);                                             // ���ý���ʱ��
		FLOAT GetMinEndTime(VOID) const;                                                           // �����С����ʱ��
		FLOAT GetMaxEndTime(VOID) const;                                                           // ���������ʱ��

	public:
		const VEC3* GetWorldScale(VOID);                                                           // �����������
		const VEC3* GetWorldPosition(VOID);                                                        // �������λ��
		const QUAT* GetWorldOrientation(VOID);                                                     // ������糯��

	public:
		VOID Play(VOID);                                                                           // ����
		VOID Stop(VOID);                                                                           // ֹͣ

		VOID Pause(VOID);                                                                          // ��ͣ
		VOID Resume(VOID);                                                                         // ����

		BOOL IsPlaying(VOID) const;                                                                // ����״̬

	public:
		VOID Update(FLOAT deltaTime);                                                              // ����

	public:
		BOOL IsCanAffect(const CParticle *pParticle);                                              // ʩ��Ӱ����
		virtual VOID Affect(CParticle *pParticle, FLOAT deltaTime) {}                              // Ӱ������


		// ����
	protected:
		BOOL m_bEnable;                                                                            // ����״̬
		BOOL m_bActive;                                                                            // �״̬

		BOOL m_bPlaying;                                                                           // ����״̬
		BOOL m_bPause;                                                                             // ��ͣ����״̬

		FLOAT m_beginTime;                                                                         // ��ʼʱ��
		FLOAT m_endTime;                                                                           // ����ʱ��

	protected:
		FLOAT m_step;                                                                              // ��Ӧ���, ÿʱ������Ӧһ��
		FLOAT m_delta;                                                                             // �����ʱ

		FLOAT m_minBeginTime;                                                                      // ��С��ʼʱ��
		FLOAT m_maxBeginTime;                                                                      // ���ʼʱ��

		FLOAT m_minEndTime;                                                                        // ��С����ʱ��
		FLOAT m_maxEndTime;                                                                        // ������ʱ��

	protected:
		CParticleSystem *m_pParent;                                                                // ����ϵͳ
	};

}

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

	class _CrossExport CEmitterBase
	{
		friend class CParticleSystem;


		// ���ݽṹ
	public:
		typedef enum { BASE, POINT, BOX, SPHERE } TYPE;                                            // ����������


		// ����/��������
	protected:
		CEmitterBase(CParticleSystem *pParent);
		virtual ~CEmitterBase(VOID);


		// ����
	public:
		virtual TYPE GetType(VOID) const { return BASE;  };                                        // �������
		virtual CParticleSystem* GetParticleSystem(VOID) const;                                    // �������ϵͳ

		virtual VOID Init(VOID);                                                                   // ��ʼ��
		virtual VOID Free(VOID);                                                                   // �ͷ�

		virtual VOID LoadXML(TiXmlNode *pEmitterNode);                                             // ����XML
		virtual VOID SaveXML(TiXmlNode *pEmitterNode) const;                                       // ����XML

	protected:
		VOID LoadEmitRate(TiXmlNode *pEmitterNode);                                                // ���ط�����
		VOID LoadBeginTime(TiXmlNode *pEmitterNode);                                               // ���ؿ�ʼʱ��
		VOID LoadDurationTime(TiXmlNode *pEmitterNode);                                            // ���س���ʱ��
		VOID LoadRadianX(TiXmlNode *pEmitterNode);                                                 // ������X�ᷢ��ƫ��
		VOID LoadRadianY(TiXmlNode *pEmitterNode);                                                 // ������Y�ᷢ��ƫ��
		VOID LoadRadianZ(TiXmlNode *pEmitterNode);                                                 // ������Z�ᷢ��ƫ��
		VOID LoadKeepLocal(TiXmlNode *pEmitterNode);                                               // ���ر��ֱ���λ��״̬
		VOID LoadLiveTime(TiXmlNode *pEmitterNode);                                                // ��������ʱ��
		VOID LoadVelocity(TiXmlNode *pEmitterNode);                                                // �����ٶ�
		VOID LoadSelfRadian(TiXmlNode *pEmitterNode);                                              // ������ת��
		VOID LoadScale(TiXmlNode *pEmitterNode);                                                   // ��������
		VOID LoadColor(TiXmlNode *pEmitterNode);                                                   // ������ɫ
		VOID LoadUVOffset(TiXmlNode *pEmitterNode);                                                // ��������ƫ��

		VOID SaveEmitRate(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;     // ���淢����
		VOID SaveBeginTime(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;    // ���濪ʼʱ��
		VOID SaveDurationTime(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const; // �������ʱ��
		VOID SaveRadianX(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;      // ������X�ᷢ��ƫ��
		VOID SaveRadianY(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;      // ������Y�ᷢ��ƫ��
		VOID SaveRadianZ(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;      // ������Z�ᷢ��ƫ��
		VOID SaveKeepLocal(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;    // ���汣�ֱ���λ��״̬
		VOID SaveLiveTime(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;     // ��������ʱ��
		VOID SaveVelocity(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;     // �����ٶ�
		VOID SaveSelfRadian(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;   // ������ת��
		VOID SaveScale(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;        // ��������
		VOID SaveColor(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;        // ������ɫ
		VOID SaveUVOffset(TiXmlNode *pEmitterNode, const CEmitterBase *pDefaultEmitter) const;     // ��������ƫ��

	public:
		VOID SetEnable(BOOL bEnable);                                                              // ��������״̬
		BOOL IsEnable(VOID) const;                                                                 // �������״̬
		BOOL IsActive(VOID) const;                                                                 // ��û״̬

	public:
		VOID SetEmitRate(INT rate);                                                                // ���÷�����
		INT GetEmitRate(VOID) const;                                                               // ��÷�����

		VOID SetBeginTime(FLOAT minTime, FLOAT maxTime);                                           // ���ÿ�ʼʱ��
		FLOAT GetMinBeginTime(VOID) const;                                                         // �����С��ʼʱ��
		FLOAT GetMaxBeginTime(VOID) const;                                                         // ������ʼʱ��

		VOID SetDurationTime(FLOAT minTime, FLOAT maxTime);                                        // ���ó���ʱ��
		FLOAT GetMinDurationTime(VOID) const;                                                      // �����С����ʱ��
		FLOAT GetMaxDurationTime(VOID) const;                                                      // ���������ʱ��

		VOID SetRadianX(FLOAT minRadian, FLOAT maxRadian);                                         // ������X�ᷢ��ƫ��
		FLOAT GetMinRadianX(VOID) const;                                                           // �����С��X�ᷢ��ƫ��
		FLOAT GetMaxRadianX(VOID) const;                                                           // ��������X�ᷢ��ƫ��

		VOID SetRadianY(FLOAT minRadian, FLOAT maxRadian);                                         // ������Y�ᷢ��ƫ��
		FLOAT GetMinRadianY(VOID) const;                                                           // �����С��Y�ᷢ��ƫ��
		FLOAT GetMaxRadianY(VOID) const;                                                           // ��������Y�ᷢ��ƫ��

		VOID SetRadianZ(FLOAT minRadian, FLOAT maxRadian);                                         // ������Z�ᷢ��ƫ��
		FLOAT GetMinRadianZ(VOID) const;                                                           // �����С��Z�ᷢ��ƫ��
		FLOAT GetMaxRadianZ(VOID) const;                                                           // ��������Z�ᷢ��ƫ��

	public:
		VOID SetKeepLocal(BOOL bKeepLocal);                                                        // ���ñ��ֱ���λ��״̬
		BOOL IsKeepLocal(VOID) const;                                                              // ��ñ��ֱ���λ��״̬

		VOID SetLiveTime(FLOAT minTime, FLOAT maxTime);                                            // ��������ʱ��
		FLOAT GetMinLiveTime(VOID) const;                                                          // �����С����ʱ��
		FLOAT GetMaxLiveTime(VOID) const;                                                          // ����������ʱ��

		VOID SetVelocity(FLOAT minVelocity, FLOAT maxVelocity);                                    // �����ٶ�
		FLOAT GetMinVelocity(VOID) const;                                                          // �����С�ٶ�
		FLOAT GetMaxVelocity(VOID) const;                                                          // ��������ٶ�

		VOID SetSelfRadian(FLOAT minRadian, FLOAT maxRadian);                                      // ������תƫ��
		FLOAT GetMinSelfRadian(VOID) const;                                                        // �����С��תƫ��
		FLOAT GetMaxSelfRadian(VOID) const;                                                        // ��������תƫ��

		VOID SetScale(FLOAT minScaleX, FLOAT minScaleY, FLOAT minScaleZ, FLOAT maxScaleX, FLOAT maxScaleY, FLOAT maxScaleZ); // ��������
		VOID SetScale(const VEC3* minScale, const VEC3 *maxScale);                                 // ��������
		const VEC3* GetMinScale(VOID) const;                                                       // �����С����
		const VEC3* GetMaxScale(VOID) const;                                                       // ����������

		VOID SetColor(FLOAT minColorR, FLOAT minColorG, FLOAT minColorB, FLOAT minColorA, FLOAT maxColorR, FLOAT maxColorG, FLOAT maxColorB, FLOAT maxColorA); // ������ɫ
		VOID SetColor(const VEC4* minColor, const VEC4 *maxColor);                                 // ������ɫ
		const VEC4* GetMinColor(VOID) const;                                                       // �����С��ɫ
		const VEC4* GetMaxColor(VOID) const;                                                       // ��������ɫ

		VOID SetUVOffset(FLOAT minUVOffsetX, FLOAT minUVOffsetY, FLOAT maxUVOffsetX, FLOAT maxUVOffsetY); // ��������ƫ��
		VOID SetUVOffset(const VEC2* minUVOffset, const VEC2 *maxUVOffset);                        // ��������ƫ��
		const VEC2* GetMinUVOffset(VOID) const;                                                    // �����С����ƫ��
		const VEC2* GetMaxUVOffset(VOID) const;                                                    // ����������ƫ��

	public:
		VOID SetPosition(FLOAT x, FLOAT y, FLOAT z);                                               // ����λ��
		VOID SetPosition(const VEC3 *position);                                                    // ����λ��
		const VEC3* GetPosition(VOID) const;                                                       // ���λ��

		VOID SetDirection(FLOAT x, FLOAT y, FLOAT z);                                              // ���ó���
		VOID SetDirection(const VEC3 *direction);                                                  // ���ó���
		VOID SetOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w);                                   // ���ó���
		VOID SetOrientation(const QUAT *orientation);                                              // ���ó���
		const QUAT* GetOrientation(VOID) const;                                                    // ��ó���

		const VEC3* GetWorldScale(VOID);                                                           // �����������
		const VEC3* GetWorldPosition(VOID);                                                        // �������λ��
		const QUAT* GetWorldOrientation(VOID);                                                     // ������糯��

		INT GetEmitMount(VOID) const;                                                              // ��÷�����

	public:
		VOID Play(VOID);                                                                           // ����
		VOID Stop(VOID);                                                                           // ֹͣ

		VOID Pause(VOID);                                                                          // ��ͣ
		VOID Resume(VOID);                                                                         // ����

		BOOL IsPlaying(VOID) const;                                                                // ����״̬

	public:
		VOID Update(FLOAT deltaTime);                                                              // ����

	public:
		virtual BOOL Emit(CParticle *pParticle, const VEC3 *localPosition);                        // ��������
		virtual BOOL Emit(CParticle *pParticle) { return FALSE; }                                  // ��������


		// ����
	protected:
		BOOL m_bEnable;                                                                            // ����״̬
		BOOL m_bActive;                                                                            // �״̬

		BOOL m_bPlaying;                                                                           // ����״̬
		BOOL m_bPause;                                                                             // ��ͣ����״̬

		FLOAT m_emitMount;                                                                         // ������
		FLOAT m_emitDeltaTime;                                                                     // ������ʱ��

		FLOAT m_currTime;                                                                          // ��ǰʱ��
		FLOAT m_beginTime;                                                                         // ��ʼʱ��
		FLOAT m_durationTime;                                                                      // ����ʱ��

		VEC3 m_worldPosition;                                                                      // ��������ϵλ��
		QUAT m_worldOrientation;                                                                   // ��������ϵ����

	protected:
		INT m_emitRate;                                                                            // ������������

		FLOAT m_minBeginTime;                                                                      // ��������С��ʼʱ��
		FLOAT m_maxBeginTime;                                                                      // ���������ʼʱ��

		FLOAT m_minDurationTime;                                                                   // ��������С����ʱ��
		FLOAT m_maxDurationTime;                                                                   // ������������ʱ��

		FLOAT m_minRadianX;                                                                        // ��������С��X�ᷢ��Ƕ�
		FLOAT m_maxRadianX;                                                                        // �����������X�ᷢ��Ƕ�

		FLOAT m_minRadianY;                                                                        // ��������С��Y�ᷢ��Ƕ�
		FLOAT m_maxRadianY;                                                                        // �����������Y�ᷢ��Ƕ�

		FLOAT m_minRadianZ;                                                                        // ��������С��Z�ᷢ��Ƕ�
		FLOAT m_maxRadianZ;                                                                        // �����������Z�ᷢ��Ƕ�

		VEC3 m_position;                                                                           // ������λ��
		QUAT m_orientation;                                                                        // ����������

	protected:
		BOOL m_bKeepLocal;                                                                         // ���ӱ��ֱ���λ��״̬

		FLOAT m_minLiveTime;                                                                       // ������С��������ʱ��
		FLOAT m_maxLiveTime;                                                                       // ���������������ʱ��

		FLOAT m_minVelocity;                                                                       // ������С�ٶ�
		FLOAT m_maxVelocity;                                                                       // ��������ٶ�

		FLOAT m_minSelfRadian;                                                                     // ������С��ת�Ƕ�
		FLOAT m_maxSelfRadian;                                                                     // ���������ת�Ƕ�

		VEC3 m_minScale;                                                                           // ������С����
		VEC3 m_maxScale;                                                                           // �����������

		VEC4 m_minColor;                                                                           // ������С��ɫ
		VEC4 m_maxColor;                                                                           // ���������ɫ

		VEC2 m_minUVOffset;                                                                        // ������С����ƫ��
		VEC2 m_maxUVOffset;                                                                        // �����������ƫ��

	protected:
		CParticleSystem *m_pParent;                                                                // ����ϵͳ
	};

}

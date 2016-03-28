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

	class _CrossExport CEmitterBase;
	class _CrossExport CAffectorBase;
	class _CrossExport CParticle
	{
		friend class CParticleSystem;


		// ���ݽṹ
	public:
		typedef enum { VIRTUAL, PARTICLE_SYSTEM } TYPE;                                            // ��������


		// ����/��������
	protected:
		CParticle(VOID);
		virtual ~CParticle(VOID);

		VOID Reset(VOID);                                                                          // ����


		// ����
	public:
		FLOAT currTime;                                                                            // ��ǰʱ��
		FLOAT liveTime;                                                                            // ����ʱ��

		BOOL bKeepLocal;                                                                           // ���ֱ�������
		                                                                                           // ˵��: ����ڷ������ķ���������ϵ

		VEC3 localScale;                                                                           // ����
		VEC3 localPosition;                                                                        // λ��
		QUAT localOrientation;                                                                     // ����
		VEC3 localVelocity;                                                                        // �ٶ�

		FLOAT radian;                                                                              // ��ת�Ƕ�

		VEC4 color;                                                                                // ��ɫ
		VEC2 uvOffset;                                                                             // ��������ƫ��

		VEC2 texScrollOffset;                                                                      // �������ƫ��
		VEC2 texSequenceScale;                                                                     // ����֡��������
		VEC2 texSequenceOffset;                                                                    // ����֡����ƫ��

	public:
		std::map<CAffectorBase*, VEC3> deltaScale;                                                 // ���ű仯��
		std::map<CAffectorBase*, VEC4> deltaColor;                                                 // ��ɫ�仯��
		std::map<CAffectorBase*, VEC3> deltaForce;                                                 // ���仯��

		std::map<CAffectorBase*, FLOAT> deltaRotateSpeed;                                          // ��ת�ٶȱ仯��
		std::map<CAffectorBase*, FLOAT> deltaVelocityRatios;                                       // �ٶȱ��ʱ仯��

		std::map<CAffectorBase*, FLOAT> texSequenceTime;                                           // ����֡����ʱ��

	public:
		CEmitterBase *pEmitter;                                                                    // ������ָ��
		CParticleSystem *pEmitParticleSystem;                                                      // ����������ϵͳ

	public:
		CParticle *pNext;                                                                          // ������һ������
	};

}

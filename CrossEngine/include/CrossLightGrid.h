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

	class _CrossExport CLightGrid
	{
		friend class CSceneManager;


		static const DWORD LIGHT_CHANNEL = 0x00000001;                                             // ����ͨ��


		// ���ݽṹ
	public:
		typedef struct {
			BYTE direction[2];                                                                     // ����
			BYTE ambient[3];                                                                       // ��������
			BYTE diffuse[3];                                                                       // ���������
			BYTE indirectup[3];                                                                    // ��ӹ�����
			BYTE indirectdown[3];                                                                  // ��ӹ�����
			BYTE indirectleft[3];                                                                  // ��ӹ�����
			BYTE indirectright[3];                                                                 // ��ӹ�����
			BYTE indirectfront[3];                                                                 // ��ӹ���ǰ
			BYTE indirectback[3];                                                                  // ��ӹ��պ�
		} LIGHT_POINT;

		typedef struct {
			AABB aabb;                                                                             // ��Χ������

			FLOAT stepx;                                                                           // X���򲽽�
			FLOAT stepy;                                                                           // Y���򲽽�
			FLOAT stepz;                                                                           // Z���򲽽�

			LIGHT_POINT ***pppLightPoints;                                                         // ��㼯��
		} LIGHT_VOLUME;

		typedef std::vector<LIGHT_VOLUME*> LightVolumeSet;                                         // �����弯��


		// ����/��������
	protected:
		CLightGrid(CSceneManager *pSceneManager);
		virtual ~CLightGrid(VOID);


		// ����
	public:
		VOID Init(VOID);                                                                           // ��ʼ��
		VOID Free(VOID);                                                                           // �ͷ�

	public:
		BOOL LoadFromStream(CStream *pStream);                                                     // ��������������Դ

	public:
		BOOL GetLightParams(DWORD dwChannel, const VEC3 *position, VEC4 *direction, VEC3 *ambient, VEC3 *diffuse, VEC3 *specular, VEC3 *rim, VEC3 *skyLower, VEC3 *skyUpper, VEC3 *indirectUp, VEC3 *indirectDown, VEC3 *indirectLeft, VEC3 *indirectRight, VEC3 *indirectFront, VEC3 *indirectBack) const; // ��ù��ղ���


		// ����
	protected:
		LightVolumeSet m_volumes;                                                                  // �����弯��

	protected:
		CSceneManager *m_pSceneManager;                                                            // ����������
	};

}

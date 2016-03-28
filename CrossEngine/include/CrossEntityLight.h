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

	class _CrossExport CEntityLight : public CEntity
	{
		friend class CSceneManager;


		// ���ݽṹ
	public:
		typedef enum { LIGHT_POINT, LIGHT_DIRECTIONAL } LIGHT_TYPE;                                // ��Դ����


		// ����/��������
	protected:
		CEntityLight(DWORD dwName, CSceneManager *pSceneManager);
		virtual ~CEntityLight(VOID);


		// ����
	public:
		virtual TYPE GetType(VOID) const;                                                          // �������
		virtual BOOL IsRenderable(VOID) const;                                                     // ��ÿ���Ⱦ

	public:
		VOID SetLightType(LIGHT_TYPE type);                                                        // ��������
		VOID SetRadius(FLOAT radius);                                                              // ���ð뾶
		VOID SetAttenuation(FLOAT k0, FLOAT k1, FLOAT k2);                                         // ����˥��
		VOID SetAmbient(FLOAT r, FLOAT g, FLOAT b);                                                // ���û�����
		VOID SetDiffuse(FLOAT r, FLOAT g, FLOAT b);                                                // �����������
		VOID SetSpecular(FLOAT r, FLOAT g, FLOAT b);                                               // ���þ����
		VOID SetRim(FLOAT r, FLOAT g, FLOAT b);                                                    // ����������
		VOID SetSkyLower(FLOAT r, FLOAT g, FLOAT b);                                               // �������
		VOID SetSkyUpper(FLOAT r, FLOAT g, FLOAT b);                                               // �������

		LIGHT_TYPE GetLightType(VOID) const;                                                       // �������
		FLOAT GetRadius(VOID) const;                                                               // ��ð뾶
		const FLOAT* GetAttenuation(VOID) const;                                                   // ���˥��
		const FLOAT* GetAmbient(VOID) const;                                                       // ��û�����
		const FLOAT* GetDiffuse(VOID) const;                                                       // ����������
		const FLOAT* GetSpecular(VOID) const;                                                      // ��þ����
		const FLOAT* GetRim(VOID) const;                                                           // ���������
		const FLOAT* GetSkyLower(VOID) const;                                                      // ������
		const FLOAT* GetSkyUpper(VOID) const;                                                      // ������

	public:
		const AABB* GetAABB(VOID);                                                                 // ��ð�Χ��

	public:
		VOID UpdateRenderQuene(const CEntityCamera *pCamera);                                      // ������Ⱦ����
		VOID RenderQuery(VOID) const;                                                              // ��Ⱦ��ѯ
		VOID RenderLighting(VOID) const;                                                           // ��Ⱦ����

	public:
		UINT GetQuery(VOID) const;                                                                 // ����ڵ���ѯ


		// ����
	protected:
		AABB m_aabbLocal;                                                                          // ��Χ��
		AABB m_aabbWorld;                                                                          // ��Χ��

	protected:
		LIGHT_TYPE m_type;                                                                         // ��Դ����
		FLOAT m_radius;                                                                            // �뾶
		FLOAT m_attenuation[3];                                                                    // ˥��
		FLOAT m_ambient[3];                                                                        // ������
		FLOAT m_diffuse[3];                                                                        // �������
		FLOAT m_specular[3];                                                                       // �����
		FLOAT m_rim[3];                                                                            // ������
		FLOAT m_skyLower[3];                                                                       // ���
		FLOAT m_skyUpper[3];                                                                       // ���

	protected:
		UINT m_query;                                                                              // �ڵ���ѯ
	};

}

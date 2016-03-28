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

	class _CrossExport CMatPhysics
	{
		friend class CMaterial;


		// ����/��������
	protected:
		CMatPhysics(CMaterial *pMaterial);
		virtual ~CMatPhysics(VOID);


		// ����
	public:
		VOID Init(VOID);                                                                           // ��ʼ����Դ
		VOID Free(VOID);                                                                           // �ͷ���Դ

		const CMaterial* GetParentMaterial(VOID) const;                                            // ��ø�����

	public:
		VOID SetType(MATERIAL_TYPE type);                                                          // ��������
		MATERIAL_TYPE GetType(VOID) const;                                                         // �������

	protected:
		VOID LoadXML(TiXmlNode *pPhysicsNode);                                                     // ����XML
		VOID SaveXML(TiXmlNode *pPhysicsNode) const;                                               // ����XML


		// ����
	protected:
		MATERIAL_TYPE m_type;                                                                      // ����

	protected:
		CMaterial *m_pParentMaterial;                                                              // ������
	};

	class _CrossExport CEntityRenderable;
	class _CrossExport CMatPass
	{
		friend class CMatPassGroup;


		// ����/��������
	protected:
		CMatPass(CMaterial *pMaterial);
		virtual ~CMatPass(VOID);


		// ����
	public:
		VOID Init(VOID);                                                                           // ��ʼ����Դ
		VOID Free(VOID);                                                                           // �ͷ���Դ

		const CMaterial* GetParentMaterial(VOID) const;                                            // ��ø�����

	public:
		CRenderer::PASS* GetPass(VOID) const;                                                      // �����Ⱦͨ·

		VOID SetRenderID(DWORD id);                                                                // ������ȾID
		DWORD GetRenderID(VOID) const;                                                             // �����ȾID

		VOID SetEnableCullFace(BOOL bEnable);                                                      // �������޳�״̬
		VOID SetCullFaceMode(UINT mode);                                                           // �������޳�ģʽ
		VOID SetFrontFaceMode(UINT mode);                                                          // �����涥��˳��ģʽ
		BOOL IsEnableCullFace(VOID) const;                                                         // ������޳�״̬
		UINT GetCullFaceMode(VOID) const;                                                          // ������޳�ģʽ
		UINT GetFrontFaceMode(VOID) const;                                                         // ����涥��˳��ģʽ

		VOID SetEnableDepthTest(BOOL bEnable);                                                     // ������Ȳ���״̬
		VOID SetEnableDepthWrite(BOOL bEnable);                                                    // ����д���״̬
		VOID SetDepthFunc(UINT func);                                                              // ������Ȳ��Է���
		BOOL IsEnableDepthTest(VOID) const;                                                        // �����Ȳ���״̬
		BOOL IsEnableDepthWrite(VOID) const;                                                       // ���д���״̬
		UINT GetDepthFunc(VOID) const;                                                             // �����Ȳ��Է���

		VOID SetEnableAlphaTest(BOOL bEnable);                                                     // ����͸������״̬
		VOID SetAlphaFunc(UINT func);                                                              // ����͸�����Է���
		VOID SetAlphaRef(FLOAT ref);                                                               // ����͸�����Լ�����
		BOOL IsEnableAlphaTest(VOID) const;                                                        // ���͸������״̬
		UINT GetAlphaFunc(VOID) const;                                                             // ���͸�����Է���
		FLOAT GetAlphaRef(VOID) const;                                                             // ���͸�����Բ���

		VOID SetEnableBlend(BOOL bEnable);                                                         // ���û��״̬
		VOID SetBlendMode(UINT color, UINT alpha);                                                 // ���û��ģʽ
		VOID SetBlendFunc(UINT srcColor, UINT srcAlpha, UINT desColor, UINT desAlpha);             // ���û�ϲ���
		BOOL IsEnableBlend(VOID) const;                                                            // ��û��״̬
		UINT GetBlendModeColor(VOID) const;                                                        // �����ɫ���ģʽ
		UINT GetBlendModeAlpha(VOID) const;                                                        // ���͸�����ģʽ
		UINT GetBlendFuncSrcColor(VOID) const;                                                     // �����ɫ��ϲ���
		UINT GetBlendFuncSrcAlpha(VOID) const;                                                     // ���͸����ϲ���
		UINT GetBlendFuncDesColor(VOID) const;                                                     // �����ɫ��ϲ���
		UINT GetBlendFuncDesAlpha(VOID) const;                                                     // ���Ŀ���ϲ���

		VOID SetEnableLighting(BOOL bEnable);                                                      // ���ù���״̬
		VOID SetAmbient(FLOAT r, FLOAT g, FLOAT b, FLOAT a);                                       // ���û�����
		VOID SetDiffuse(FLOAT r, FLOAT g, FLOAT b, FLOAT a);                                       // �����������
		VOID SetEmission(FLOAT r, FLOAT g, FLOAT b, FLOAT a);                                      // �����Է���
		VOID SetSpecular(FLOAT r, FLOAT g, FLOAT b, FLOAT a);                                      // ���ø߹�
		VOID SetShininess(FLOAT shininess);                                                        // ���ø߹⼶��
		BOOL IsEnableLighting(VOID) const;                                                         // ��ù���״̬
		const FLOAT* GetAmbient(VOID) const;                                                       // ��û�����
		const FLOAT* GetDiffuse(VOID) const;                                                       // ����������
		const FLOAT* GetEmission(VOID) const;                                                      // ����Է���
		const FLOAT* GetSpecular(VOID) const;                                                      // ��ø߹�
		FLOAT GetShininess(VOID) const;                                                            // ��ø߹⼶��

		BOOL SetTexture(INT indexTexUnit, const CHAR *szName);                                     // ��������Ԫ
		VOID SetTextureParam(INT indexTexUnit, UINT minFilter, UINT magFilter, UINT warpu, UINT warpv); // �����������
		VOID SetTextureScrollU(INT indexTexUnit, FLOAT scrollu);                                   // �����������
		VOID SetTextureScrollV(INT indexTexUnit, FLOAT scrollv);                                   // �����������
		VOID SetTextureSequence(INT indexTexUnit, INT cols, INT rows, FLOAT duration);             // ����������
		const CTexturePtr& GetTexture(INT indexTexUnit) const;                                     // �������
		UINT GetTextureMinFilter(INT indexTexUnit) const;                                          // ����������
		UINT GetTextureMagFilter(INT indexTexUnit) const;                                          // ����������
		UINT GetTextureWarpU(INT indexTexUnit) const;                                              // ����������
		UINT GetTextureWarpV(INT indexTexUnit) const;                                              // ����������
		FLOAT GetTextureScrollU(INT indexTexUnit) const;                                           // ��������������
		FLOAT GetTextureScrollV(INT indexTexUnit) const;                                           // ��������������
		INT GetTextureSequenceCols(INT indexTexUnit) const;                                        // �����������֡����
		INT GetTextureSequenceRows(INT indexTexUnit) const;                                        // �����������֡����
		FLOAT GetTextureSequenceDuration(INT indexTexUnit) const;                                  // �����������֡����

		BOOL SetShaderVertex(const CHAR *szName);                                                  // ���ö�����ɫ��
		BOOL SetShaderFragment(const CHAR *szName);                                                // ����ƬԴ��ɫ��
		const CShaderPtr& GetShaderVertex(VOID) const;                                             // ��ö�����ɫ��
		const CShaderPtr& GetShaderFragment(VOID) const;                                           // ���ƬԴ��ɫ��

	public:
		UINT64 Evaluate(VOID) const;                                                               // ��������ֵ
		BOOL IsTransparence(VOID) const;                                                           // ���͸��״̬

	protected:
		VOID LoadXML(TiXmlNode *pPassNode);                                                        // ����XML
		VOID LoadRenderID(TiXmlNode *pPassNode);                                                   // ������ȾID
		VOID LoadLighting(TiXmlNode *pPassNode);                                                   // ���ع��ղ���
		VOID LoadCull(TiXmlNode *pPassNode);                                                       // �����޳�����
		VOID LoadBlend(TiXmlNode *pPassNode);                                                      // ���ػ�ϲ���
		VOID LoadDepth(TiXmlNode *pPassNode);                                                      // ������Ȳ���
		VOID LoadAlpha(TiXmlNode *pPassNode);                                                      // ����͸������
		VOID LoadTextureUnits(TiXmlNode *pPassNode);                                               // ��������Ԫ
		VOID LoadShaderVertex(TiXmlNode *pPassNode);                                               // ���ض�����ɫ��
		VOID LoadShaderFragment(TiXmlNode *pPassNode);                                             // ����Ƭ����ɫ��
		VOID LoadShaderParams(TiXmlNode *pShaderNode);                                             // ������ɫ������
		VOID SetShaderParams(const CRenderer::UniformLocationMap& params);                         // ������ɫ������
		VOID CreateProgram(VOID);                                                                  // ������ɫ������

	protected:
		VOID SaveXML(TiXmlNode *pPassNode) const;                                                  // ����XML
		VOID SaveRenderID(TiXmlNode *pPassNode) const;                                             // ������ȾID
		VOID SaveLighting(TiXmlNode *pPassNode, const CRenderer::PASS_LIGHTING *pDefault) const;   // ������ղ���
		VOID SaveCull(TiXmlNode *pPassNode, const CRenderer::PASS_CULL *pDefault) const;           // �����޳�����
		VOID SaveBlend(TiXmlNode *pPassNode, const CRenderer::PASS_BLEND *pDefault) const;         // �����ϲ���
		VOID SaveDepth(TiXmlNode *pPassNode, const CRenderer::PASS_DEPTH *pDefault) const;         // ������Ȳ���
		VOID SaveAlpha(TiXmlNode *pPassNode, const CRenderer::PASS_ALPHA *pDefault) const;         // ����͸������
		VOID SaveTextureUnits(TiXmlNode *pPassNode) const;                                         // ��������Ԫ
		VOID SaveShaderVertex(TiXmlNode *pPassNode) const;                                         // ���涥����ɫ��
		VOID SaveShaderFragment(TiXmlNode *pPassNode) const;                                       // ����Ƭ����ɫ��
		VOID SaveShaderParams(TiXmlNode *pShaderNode, const CRenderer::UniformLocationMap& refParams) const; // ������ɫ������


		// ����
	protected:
		DWORD m_id;                                                                                // ��ȾID
		CRenderer::PASS m_pass;                                                                    // ��Ⱦͨ·

	protected:
		CShaderPtr m_ptrShaderVertex;                                                              // ������ɫ��
		CShaderPtr m_ptrShaderFragment;                                                            // ������ɫ��
		CTexturePtr m_ptrTextures[MAX_TEXUNITS];                                                   // ����

	protected:
		CMaterial *m_pParentMaterial;                                                              // ������

		// ��Ⱦ����
	public:
		CMatPass *pRenderNext;                                                                     // ͨ·����
		CEntityRenderable *pEntityHead;                                                            // ʵ����Ⱦ����ͷ
	};

	class _CrossExport CMatPassGroup
	{
		friend class CMatSolution;


		// ���ݽṹ
	public:
		typedef std::vector<CMatPass*> PassSet;                                                    // ��Ⱦͨ·����


		// ����/��������
	protected:
		CMatPassGroup(const CHAR *szName, CMaterial *pMaterial);
		virtual ~CMatPassGroup(VOID);


		// ����
	public:
		VOID Init(VOID);                                                                           // ��ʼ��
		VOID Init(const CHAR *szName);                                                             // ��ʼ����Դ
		VOID Free(VOID);                                                                           // �ͷ���Դ

		const CHAR* GetName(VOID) const;                                                           // �������
		const CMaterial* GetParentMaterial(VOID) const;                                            // ��ø�����

	public:
		INT GetPassCount(VOID) const;                                                              // �����Ⱦͨ·��
		const CMatPass* GetPass(INT index) const;                                                  // �����Ⱦͨ·
		const PassSet& GetPasses(VOID) const;                                                      // �����Ⱦͨ·����

	protected:
		VOID LoadXML(TiXmlNode *pPassGroupNode);                                                   // ����XML
		VOID SaveXML(TiXmlNode *pPassGroupNode) const;                                             // ����XML


		// ����
	protected:
		CHAR m_szName[_MAX_STRING];                                                                // ��Ⱦͨ·������
		PassSet m_passes;                                                                          // ��Ⱦͨ·����

	protected:
		CMaterial *m_pParentMaterial;                                                              // ������
	};

	class _CrossExport CMatSolution
	{
		friend class CMaterial;


		// ���ݽṹ
	public:
		typedef std::map<DWORD, CMatPassGroup*> PassGroupMap;                                      // ��Ⱦͨ·�鼯��


		// ����/��������
	protected:
		CMatSolution(const CHAR *szName, CMaterial *pMaterial);
		virtual ~CMatSolution(VOID);


		// ����
	public:
		VOID Init(VOID);                                                                           // ��ʼ��
		VOID Init(const CHAR *szName);                                                             // ��ʼ����Դ
		VOID Free(VOID);                                                                           // �ͷ���Դ

		const CHAR* GetName(VOID) const;                                                           // �������
		const CMaterial* GetParentMaterial(VOID) const;                                            // ��ø�����

	public:
		const CMatPassGroup* GetPassGroup(const CHAR *szName) const;                               // �����Ⱦͨ·��
		const CMatPassGroup* GetPassGroup(DWORD dwName) const;                                     // �����Ⱦͨ·��
		const PassGroupMap& GetPassGroups(VOID) const;                                             // �����Ⱦͨ·�鼯��

	protected:
		VOID LoadXML(TiXmlNode *pSolutionNode);                                                    // ����XML
		VOID SaveXML(TiXmlNode *pSolutionNode) const;                                              // ����XML


		// ����
	protected:
		CHAR m_szName[_MAX_STRING];                                                                // ���ʷ�����
		PassGroupMap m_groups;                                                                     // ��Ⱦͨ·�鼯��

	protected:
		CMaterial *m_pParentMaterial;                                                              // ������
	};

	class _CrossExport CMaterial : public CResource
	{
		friend class CMaterialPtr;
		friend class CMaterialManager;


		// ���ݽṹ
	public:
		typedef std::map<DWORD, CMatSolution*> SolutionMap;                                        // �����������


		// ����/��������
	protected:
		CMaterial(CResourceManager *pResourceManager);
		virtual ~CMaterial(VOID);


		// ����
	public:
		virtual VOID Init(VOID);                                                                   // ��ʼ����Դ
		virtual VOID Free(VOID);                                                                   // �ͷ���Դ

	public:
		virtual TYPE GetType(VOID) const;                                                          // �������
		virtual BOOL IsValid(VOID) const;                                                          // ��Ч�Լ��

	public:
		virtual BOOL LoadFromStream(CStream *pStream);                                             // ��������������Դ
		virtual BOOL SaveToFileStream(FILE *pFile);                                                // ���浽�ļ�������

	protected:
		VOID LoadPhysics(TiXmlNode *pMaterialNode);                                                // ��������
		VOID LoadSolutions(TiXmlNode *pMaterialNode);                                              // ���ز��ʷ���

		VOID SavePhysics(TiXmlNode *pMaterialNode);                                                // ��������
		VOID SaveSolutions(TiXmlNode *pMaterialNode);                                              // ������Ⱦ����

	public:
		const CMatPhysics* GetPhysics(VOID) const;                                                 // �������
		const CMatSolution* GetSolution(const CHAR *szName) const;                                 // ��ò��ʷ���
		const CMatSolution* GetSolution(DWORD dwName) const;                                       // ��ò��ʷ���
		const SolutionMap& GetSolutions(VOID) const;                                               // ��ò��ʷ�������


		// ����
	protected:
		CMatPhysics m_physics;                                                                     // ��������
		SolutionMap m_solutions;                                                                   // ��������
	};

	class _CrossExport CMaterialPtr : public CSharedPtr<CMaterial>
	{
		// ����/��������
	public:
		CMaterialPtr(VOID) : CSharedPtr<CMaterial>() {}
		CMaterialPtr(const CMaterial *pMaterial) : CSharedPtr<CMaterial>(pMaterial) {}
		CMaterialPtr(const CMaterialPtr &ptrMaterial) : CSharedPtr<CMaterial>(ptrMaterial) {}
		CMaterialPtr(const CResourcePtr &ptrResource) { Set(static_cast<CMaterial*>(ptrResource.GetPointer()), ptrResource.GetRefCount()); }
		virtual ~CMaterialPtr(VOID) {}


		// ����
	protected:
		virtual VOID FreePointer(VOID)
		{
			if (m_pPointer) {
				m_pPointer->GetResourceManager()->DestroyResource(m_pPointer);
			}
		}

	public:
		CMaterialPtr& operator = (const CResourcePtr &ptrResource)
		{
			Set(static_cast<CMaterial*>(ptrResource.GetPointer()), ptrResource.GetRefCount());
			return *this;
		}

		operator CResource* ()
		{
			return (CResource *)m_pPointer;
		}

		operator const CResource* () const
		{
			return (const CResource *)m_pPointer;
		}
	};

}

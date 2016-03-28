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

	class _CrossExport CRenderStage
	{
		friend class CRenderFlow;
		friend class CRenderSolution;


		// ���ݽṹ
	public:
		typedef enum { SCENE, LIGHT, SCREEN, SHADOW_CAST, SHADOW_RECEIVE } RENDERSTATE_TYPE;       // ��Ⱦ��������
		typedef enum { DEPTH, COLOR } ATTACHMENT_TYPE;                                             // ����ҽ�����

		typedef struct {
			DWORD dwCategoryName;                                                                  // ������
			DWORD dwTextureName;                                                                   // ������

			UINT channel;                                                                          // ͨ��
			UINT minFilter;                                                                        // �˲���ʽ
			UINT magFilter;                                                                        // �˲���ʽ
			UINT wrapu;                                                                            // �������껷�Ʒ�ʽ
			UINT wrapv;                                                                            // �������껷�Ʒ�ʽ

			ATTACHMENT_TYPE attachmentType;                                                        // ����ҽ�����
		} TEXTURE;

		typedef std::vector<TEXTURE*> TextureSet;                                                  // ������


		// ����/��������
	protected:
		CRenderStage(CRenderFlow *pRenderFlow);
		virtual ~CRenderStage(VOID);


		// ����
	protected:
		VOID Init(VOID);                                                                           // ��ʼ��
		VOID Free(VOID);                                                                           // �ͷ�

	protected:
		VOID LoadFromStream(TiXmlNode *pRenderStageNode);                                          // ������Ⱦ����
		VOID LoadName(TiXmlNode *pRenderStageNode);                                                // ��������
		VOID LoadType(TiXmlNode *pRenderStageNode);                                                // ��������
		VOID LoadEnable(TiXmlNode *pRenderStageNode);                                              // ����״̬
		VOID LoadCallback(TiXmlNode *pRenderStageNode);                                            // ���ػص�
		VOID LoadClearBuffer(TiXmlNode *pRenderStageNode);                                         // ����������
		VOID LoadMaterial(TiXmlNode *pRenderStageNode);                                            // ���ز���
		VOID LoadInputTextures(TiXmlNode *pRenderStageNode);                                       // ������������
		VOID LoadOutputTextures(TiXmlNode *pRenderStageNode);                                      // �����������
		VOID LoadShaderVertex(TiXmlNode *pRenderStageNode);                                        // ���ض�����ɫ��
		VOID LoadShaderFragment(TiXmlNode *pRenderStageNode);                                      // ����Ƭ����ɫ��
		VOID LoadShaderParams(TiXmlNode *pShaderNode);                                             // ������ɫ������
		VOID SetShaderParams(const CRenderer::UniformLocationMap& params);                         // ������ɫ������

	protected:
		VOID Create(VOID);                                                                         // ����
		VOID Destroy(VOID);                                                                        // ����

	public:
		VOID SetEnable(BOOL bEnable);                                                              // ��������״̬
		BOOL IsEnable(VOID) const;                                                                 // �������״̬

		const CHAR* GetName(VOID) const;                                                           // �������
		DWORD GetRenderTime(VOID) const;                                                           // �����Ⱦʱ��

		VOID Render(DWORD dwSceneManager);                                                         // ��Ⱦ

	protected:
		VOID Begin(VOID);                                                                          // ��ʼ��Ⱦ
		VOID End(VOID);                                                                            // ������Ⱦ
		VOID RenderScene(const CEntityCamera *pCamera);                                            // ��Ⱦ����
		VOID RenderCastShadow(const CEntityShadow *pShadow);                                       // ��Ⱦ������Ӱ
		VOID RenderReceiveShadow(const CEntityCamera *pCamera, const CEntityShadow *pShadow);      // ��Ⱦ������Ӱ
		VOID RenderLighting(const CEntityCamera *pCamera);                                         // ��Ⱦ����
		VOID RenderScreen(VOID);                                                                   // ��Ⱦ��Ļ


		// ����
	protected:
		BOOL m_bEnable;                                                                            // ״̬
		CHAR m_szName[_MAX_STRING];                                                                // ����
		RENDERSTATE_TYPE m_type;                                                                   // ����

	protected:
		DWORD m_dwBeginRenderCallback;                                                             // ��Ⱦǰ�ص�������
		DWORD m_dwEndRenderCallback;                                                               // ��Ⱦ��ص�������

		FLOAT m_clearDepth;                                                                        // �����ֵ
		FLOAT m_clearColor[3];                                                                     // ����ɫֵ

		BOOL m_bClearColorBuffer;                                                                  // �����ɫ����
		BOOL m_bClearDepthBuffer;                                                                  // �����Ȼ���

		DWORD m_dwMatBeginRenderID;                                                                // ������Ⱦ��ʼID
		DWORD m_dwMatEndRenderID;                                                                  // ������Ⱦ����ID
		DWORD m_dwMatSolution;                                                                     // ������Ⱦ������
		DWORD m_dwMatPassGroup;                                                                    // ������Ⱦͨ·����

		TextureSet m_inputTextures;                                                                // ��������
		TextureSet m_outputTextures;                                                               // �������

		UINT m_vertex;                                                                             // ������ɫ��
		UINT m_fragment;                                                                           // Ƭ����ɫ��
		CRenderer::UniformLocationMap m_params;                                                    // ��ɫ����������

		UINT m_frameBuffer;                                                                        // ֡����
		UINT m_frameBufferWidth;                                                                   // ֡�����
		UINT m_frameBufferHeight;                                                                  // ֡�����

		UINT m_numDraws;                                                                           // ��·�����
		UINT m_drawBuffers[MAX_DRAW_BUFFERS];                                                      // ��·�������

	protected:
		INT m_lastViewportX;                                                                       // �ӿ�λ��X
		INT m_lastViewportY;                                                                       // �ӿ�λ��Y
		INT m_lastViewportWidth;                                                                   // �ӿڿ�
		INT m_lastViewportHeight;                                                                  // �ӿڸ�
		INT m_lastFrameBuffer;                                                                     // FBO

	protected:
		DWORD m_dwRenderTime;                                                                      // ��Ⱦʱ��

	protected:
		CRenderFlow *m_pRenderFlow;                                                                // ��Ⱦ����
	};

	class _CrossExport CRenderTarget
	{
		friend class CRenderFlow;


		// ���ݽṹ
	public:
		typedef struct {
			UINT texture;                                                                          // ����
			UINT type;                                                                             // ��������
			UINT format;                                                                           // �����ʽ
			UINT internalformat;                                                                   // �ڲ������ʽ
		} TEXTURE_TARGET;

		typedef std::map<DWORD, TEXTURE_TARGET*> TextureTargetMap;                                 // ������Ⱦ���󼯺�


		// ����/��������
	protected:
		CRenderTarget(CRenderFlow *pRenderFlow);
		virtual ~CRenderTarget(VOID);


		// ����
	protected:
		VOID Init(VOID);                                                                           // ��ʼ��
		VOID Free(VOID);                                                                           // �ͷ�

	protected:
		VOID LoadFromStream(TiXmlNode *pRenderTargetNode);                                         // ������Ⱦ����
		VOID LoadResolution(TiXmlNode *pRenderTargetNode);                                         // ���طֱ���
		VOID LoadTextureTargets(TiXmlNode *pRenderTargetNode);                                     // �����������

	protected:
		VOID Create(VOID);                                                                         // ����
		VOID Destroy(VOID);                                                                        // ����

	public:
		INT GetWidth(VOID) const;                                                                  // ��ÿ�
		INT GetHeight(VOID) const;                                                                 // ��ø�

		UINT GetDepthRenderBuffer(VOID) const;                                                     // ��������Ⱦ����
		TEXTURE_TARGET* GetTextureTarget(const CHAR *szName) const;                                // ���������Ⱦ����
		TEXTURE_TARGET* GetTextureTarget(DWORD dwName) const;                                      // ���������Ⱦ����


		// ����
	protected:
		INT m_width;                                                                               // ������Ⱦ�����
		INT m_height;                                                                              // ������Ⱦ�����

		UINT m_depthRenderBuffer;                                                                  // �����Ⱦ����
		TextureTargetMap m_textureTargets;                                                         // ������Ⱦ����

	protected:
		CRenderFlow *m_pRenderFlow;                                                                // ��Ⱦ����
	};

	class _CrossExport CRenderSolution
	{
		friend class CRenderFlow;


		// ���ݽṹ
	public:
		typedef std::vector<CRenderStage*> RenderStageSet;                                         // ��Ⱦ���̼���
		typedef std::map<DWORD, CRenderStage*> RenderStageMap;                                     // ��Ⱦ���̼���


		// ����/��������
	protected:
		CRenderSolution(CRenderFlow *pRenderFlow);
		virtual ~CRenderSolution(VOID);


		// ����
	protected:
		VOID Init(VOID);                                                                           // ��ʼ��
		VOID Free(VOID);                                                                           // �ͷ�

	protected:
		VOID LoadFromStream(TiXmlNode *pRenderSolutionNode);                                       // ������Ⱦ����

	protected:
		VOID Create(VOID);                                                                         // ����
		VOID Destroy(VOID);                                                                        // ����

	public:
		CRenderStage* GetRenderStage(const CHAR *szName) const;                                    // �����Ⱦ����
		CRenderStage* GetRenderStage(DWORD dwName) const;                                          // �����Ⱦ����

	protected:
		VOID Render(DWORD dwSceneManager);                                                         // ��Ⱦ


		// ����
	protected:
		RenderStageSet m_renderStageSet;                                                           // ��Ⱦ���̼���
		RenderStageMap m_renderStageMap;                                                           // ��Ⱦ���̼���

	protected:
		CRenderFlow *m_pRenderFlow;                                                                // ��Ⱦ����
	};

	typedef VOID(*RenderCallback)(CRenderStage *pRenderStage);

	class _CrossExport CRenderFlow
	{
		friend class CRenderSystem;


		// ���ݽṹ
	public:
		typedef std::map<DWORD, CRenderTarget*> RenderTargetMap;                                   // ��Ⱦ���󼯺�
		typedef std::map<DWORD, CRenderSolution*> RenderSolutionMap;                               // ��Ⱦ��������
		typedef std::map<DWORD, RenderCallback> RenderCallbackMap;                                 // ��Ⱦ�ص���������


		// ����/��������
	protected:
		CRenderFlow(VOID);
		virtual ~CRenderFlow(VOID);


		// ����
	public:
		VOID Init(VOID);                                                                           // ��ʼ����Դ
		VOID Free(VOID);                                                                           // �ͷ���Դ

		BOOL LoadFromFile(const CHAR *szFileName, FLOAT scalePPI = 1.0f);                          // ���ļ�������Դ
		BOOL LoadFromZip(const CHAR *szZipName, const CHAR *szFileName, FLOAT scalePPI = 1.0f);    // ��ѹ����������Դ
		BOOL LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName, FLOAT scalePPI = 1.0f);       // ��ѹ�����ļ�����������
		BOOL LoadFromStream(CStream *pStream, FLOAT scalePPI = 1.0f);                              // ��������������Դ

		BOOL Reload(FLOAT scalePPI = 1.0f);                                                        // ���¼�����Դ

	protected:
		VOID LoadRenderTargets(TiXmlNode *pRenderFlowNode);                                        // ������Ⱦ����
		VOID LoadRenderSolutions(TiXmlNode *pRenderFlowNode);                                      // ������Ⱦ����

	protected:
		VOID Create(VOID);                                                                         // ����
		VOID Destroy(VOID);                                                                        // ����

	public:
		FLOAT GetScalePPI(VOID) const;                                                             // ���PPI���ű�
		CRenderSolution* GetRenderSolution(DWORD dwName) const;                                    // �����Ⱦ����
		CRenderTarget* GetRenderTarget(DWORD dwName) const;                                        // �����Ⱦ����

	public:
		BOOL SetRenderCallback(DWORD dwName, RenderCallback fnCallback);                           // ������Ⱦ�ص�����
		RenderCallback GetRenderCallback(DWORD dwName);                                            // �����Ⱦ�ص�����

	public:
		VOID Render(DWORD dwSceneManager, DWORD dwSolution);                                       // ��Ⱦ


		// ����
	protected:
		FLOAT m_scalePPI;                                                                          // PPI���ű�

		RenderTargetMap m_renderTargets;                                                           // ��Ⱦ���󼯺�
		RenderSolutionMap m_renderSolutions;                                                       // ��Ⱦ��������
		RenderCallbackMap m_renderCallbacks;                                                       // ��Ⱦ�ص���������

	protected:
		CStream m_stream;                                                                          // ������
	};

}

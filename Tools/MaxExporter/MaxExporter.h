#pragma once

#pragma warning(disable : 4996)
#pragma warning(disable : 4103)


#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include "resource.h"

#include "max.h"
#include "decomp.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

#include "IGame\IGame.h"
#include "IGame\IGameObject.h"
#include "IGame\IGameProperty.h"
#include "IGame\IGameControl.h"
#include "IGame\IGameModifier.h"
#include "IGame\IConversionManager.h"
#include "IGame\IGameError.h"
#include "IGame\IGameFX.h"


#define MAXEXPORT_CLASS_ID             Class_ID(0x2a092712, 0x3623219)

#define LIBRARY_DESC                   "Cross Exporter"
#define CLASS_NAME                     "Cross Exporter"
#define CATEGORY_NAME                  "Cross Exporter"
#define INTERNAL_NAME                  "Cross Exporter"

#define EXT_COUNT                      3
#define EXT_NAME1                      "x"
#define EXT_NAME2                      "xml"
#define EXT_NAME3                      "mesh"
#define FULL_DESC                      "Cross Exporter"
#define SHORT_DESC                     "Cross Exporter"
#define AUTHOR_NAME                    "Liang Yue"
#define COPYRIGHT_MESSAGE              ""
#define OTHER_MESSAGE_1                ""
#define OTHER_MESSAGE_2                ""
#define VERSION                        100

#define PARAM_FILE_NAME                "CrossExporter.ini"

#define VERTEX_ANIM_KEYWORD            "AnimVertex"
#define SKELETON_ANIM_KEYWORD          "AnimSkeleton"

#define ALIGN_4BYTE(a)                 ((((a) + 3) / 4) * 4)
#define ALIGN_16BYTE(a)                ((((a) + 15) / 16) * 16)


extern HINSTANCE hInstance;
extern INT_PTR CALLBACK MainPanelProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


typedef enum {
	LIGHTING_NONE = 0,                                                                             // �޹���
	LIGHTING_SIMPLE,                                                                               // �򵥹���
	LIGHTING_ADVANCED                                                                              // �߼�����
} LIGHTING_PARAM;

typedef struct {
	LIGHTING_PARAM lighting;                                                                       // ���ղ���
	BOOL bLightmap;                                                                                // ������ͼ
	BOOL bEnvmap;                                                                                  // ������ͼ
	BOOL bShadowmap;                                                                               // ��Ӱͼ
} SOLUTION_PARAMS;

class CMaxExporter : public SceneExport
{
public:
	CMaxExporter(void)
	{
		sprintf(m_szExtName[0], "%s", EXT_NAME1);
		sprintf(m_szExtName[1], "%s", EXT_NAME2);
		sprintf(m_szExtName[2], "%s", EXT_NAME3);

		nRenderID = 100;
		bExportKeyFrame = TRUE;
		bExportSubMesh = FALSE;
		bInWorldSpace = FALSE;
		bOptimizeSkeleton = TRUE;
		bGPUSkinning = FALSE;
		lowSolutionParams.lighting = LIGHTING_NONE;
		lowSolutionParams.bLightmap = FALSE;
		lowSolutionParams.bEnvmap = FALSE;
		lowSolutionParams.bShadowmap = FALSE;
		mediumSolutionParams.lighting = LIGHTING_NONE;
		mediumSolutionParams.bLightmap = FALSE;
		mediumSolutionParams.bEnvmap = FALSE;
		mediumSolutionParams.bShadowmap = FALSE;
		highSolutionParams.lighting = LIGHTING_NONE;
		highSolutionParams.bLightmap = FALSE;
		highSolutionParams.bEnvmap = FALSE;
		highSolutionParams.bShadowmap = FALSE;
		ultraSolutionParams.lighting = LIGHTING_NONE;
		ultraSolutionParams.bLightmap = FALSE;
		ultraSolutionParams.bEnvmap = FALSE;
		ultraSolutionParams.bShadowmap = FALSE;
		extremeSolutionParams.lighting = LIGHTING_NONE;
		extremeSolutionParams.bLightmap = FALSE;
		extremeSolutionParams.bEnvmap = FALSE;
		extremeSolutionParams.bShadowmap = FALSE;
		cs = IGameConversionManager::IGAME_D3D;

		memset(szCommand, 0, sizeof(szCommand));
		memset(szExternalSkeleton, 0, sizeof(szExternalSkeleton));
	}
	virtual ~CMaxExporter(void) {}


	// SceneExport �麯��ʵ��
public:
	int ExtCount(void) { return EXT_COUNT; }                                                       // ��չ����
	const char* Ext(int n) { return m_szExtName[n]; }                    					       // ��չ��
	const char* LongDesc(void) { return FULL_DESC; }                                               // �ļ���ϸ����
	const char* ShortDesc(void) { return SHORT_DESC; }                                             // �ļ��������
	const char* AuthorName(void) { return AUTHOR_NAME; }                                           // ������
	const char* CopyrightMessage(void) { return COPYRIGHT_MESSAGE; }                               // ��Ȩ��Ϣ
	const char* OtherMessage1(void) { return OTHER_MESSAGE_1; }                                    // ������Ϣ1
	const char* OtherMessage2(void) { return OTHER_MESSAGE_2; }                                    // ������Ϣ2
	unsigned int Version(void) { return VERSION; }                                                 // �汾��
	void ShowAbout(HWND hWnd) {}                                                                   // ��ʾ���ڶԻ���

	int DoExport(const char *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0); // ������ں���

private:
	void LoadParams(char * const szFileName);                                                      // ���ز���
	void SaveParams(char * const szFileName) const;                                                // �������

private:
	char m_szExtName[EXT_COUNT][_MAX_EXT];                                                         // ��չ��

public:
	char szCommand[_MAX_PATH];                                                                     // ������ִ�е�������
	char szExternalSkeleton[_MAX_PATH];                                                            // �ⲿ����
	int nRenderID;                                                                                 // ��ȾID
	BOOL bExportKeyFrame;                                                                          // �����ؼ�֡
	BOOL bExportSubMesh;                                                                           // ����������
	BOOL bInWorldSpace;                                                                            // ģ��������ռ�
	BOOL bOptimizeSkeleton;                                                                        // �Ż�����
	BOOL bGPUSkinning;                                                                             // Ӳ����Ƥ
	SOLUTION_PARAMS lowSolutionParams;                                                             // ��Ч����Ⱦ��������
	SOLUTION_PARAMS mediumSolutionParams;                                                          // ��Ч����Ⱦ��������
	SOLUTION_PARAMS highSolutionParams;                                                            // ��Ч����Ⱦ��������
	SOLUTION_PARAMS ultraSolutionParams;                                                           // ����Ч����Ⱦ��������
	SOLUTION_PARAMS extremeSolutionParams;                                                         // ����Ч����Ⱦ��������
	IGameConversionManager::CoordSystem cs;                                                        // ����ϵͳ
};

class CMaxExporterClassDesc : public ClassDesc2
{
public:
	int IsPublic() { return TRUE; }                                                                // Controls if the plug-in shows up in lists from the user to choose from.
	void* Create(BOOL loading = FALSE) { return new CMaxExporter(); }

	HINSTANCE HInstance() { return hInstance; }

	const char* ClassName() { return CLASS_NAME; }                                                 // This method returns the name of the class. This name appears in the button for the plug-in in the 3ds Max user interface.
	const char* InternalName() { return INTERNAL_NAME; }                                           // This method is available in release 3.0 and later only.
	const char* Category() { return CATEGORY_NAME; }                                               // This methods returns a string describing the category a plug-in fits into.

	Class_ID ClassID() { return MAXEXPORT_CLASS_ID; }                                              // This method must return the unique ID for the object.
	SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }                                     // This method returns a system defined constant describing the class this plug-in class was derived from.
};

class CErrorProc : public IGameErrorCallBack
{
public:
	void ErrorProc(IGameError error) {                                                             // Define a callback for error reporting.
		char * buf = GetLastIGameErrorText();
		DebugPrint("ErrorCode = %d ErrorText = %s\n", error,buf);
	}
};

#include "Model.h"
#include "Functions.h"
#include "Resource.h"

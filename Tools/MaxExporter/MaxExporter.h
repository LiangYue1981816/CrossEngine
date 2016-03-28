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
	LIGHTING_NONE = 0,                                                                             // 无光照
	LIGHTING_SIMPLE,                                                                               // 简单光照
	LIGHTING_ADVANCED                                                                              // 高级光照
} LIGHTING_PARAM;

typedef struct {
	LIGHTING_PARAM lighting;                                                                       // 光照参数
	BOOL bLightmap;                                                                                // 光照贴图
	BOOL bEnvmap;                                                                                  // 环境贴图
	BOOL bShadowmap;                                                                               // 阴影图
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


	// SceneExport 虚函数实现
public:
	int ExtCount(void) { return EXT_COUNT; }                                                       // 扩展名数
	const char* Ext(int n) { return m_szExtName[n]; }                    					       // 扩展名
	const char* LongDesc(void) { return FULL_DESC; }                                               // 文件详细描述
	const char* ShortDesc(void) { return SHORT_DESC; }                                             // 文件简短描述
	const char* AuthorName(void) { return AUTHOR_NAME; }                                           // 作者名
	const char* CopyrightMessage(void) { return COPYRIGHT_MESSAGE; }                               // 版权信息
	const char* OtherMessage1(void) { return OTHER_MESSAGE_1; }                                    // 其他信息1
	const char* OtherMessage2(void) { return OTHER_MESSAGE_2; }                                    // 其他信息2
	unsigned int Version(void) { return VERSION; }                                                 // 版本号
	void ShowAbout(HWND hWnd) {}                                                                   // 显示关于对话框

	int DoExport(const char *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0); // 导出入口函数

private:
	void LoadParams(char * const szFileName);                                                      // 加载参数
	void SaveParams(char * const szFileName) const;                                                // 保存参数

private:
	char m_szExtName[EXT_COUNT][_MAX_EXT];                                                         // 扩展名

public:
	char szCommand[_MAX_PATH];                                                                     // 导出后执行的命令行
	char szExternalSkeleton[_MAX_PATH];                                                            // 外部骨骼
	int nRenderID;                                                                                 // 渲染ID
	BOOL bExportKeyFrame;                                                                          // 导出关键帧
	BOOL bExportSubMesh;                                                                           // 导出子网格
	BOOL bInWorldSpace;                                                                            // 模型在世界空间
	BOOL bOptimizeSkeleton;                                                                        // 优化骨骼
	BOOL bGPUSkinning;                                                                             // 硬件蒙皮
	SOLUTION_PARAMS lowSolutionParams;                                                             // 低效果渲染方案参数
	SOLUTION_PARAMS mediumSolutionParams;                                                          // 中效果渲染方案参数
	SOLUTION_PARAMS highSolutionParams;                                                            // 高效果渲染方案参数
	SOLUTION_PARAMS ultraSolutionParams;                                                           // 超高效果渲染方案参数
	SOLUTION_PARAMS extremeSolutionParams;                                                         // 极致效果渲染方案参数
	IGameConversionManager::CoordSystem cs;                                                        // 坐标系统
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

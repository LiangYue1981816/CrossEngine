#include "MaxExporter.h"


//
// 导出入口函数
//
int CMaxExporter::DoExport(const char *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts/* = FALSE*/, DWORD options/* = 0*/)
{
	//
	// 1. 参数对话框
	//
	char szParamFileName[_MAX_PATH];

	sprintf(szParamFileName, "%s\\%s", GetCOREInterface()->GetDir(APP_PLUGCFG_DIR), PARAM_FILE_NAME);
	LoadParams(szParamFileName);

	if (!DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAIN_PANEL), GetActiveWindow(), MainPanelProc, (LPARAM)this)) {
		return IMPEXP_SUCCESS;
	}

	SaveParams(szParamFileName);

	//
	// 2. 导出
	//
	IGameScene *pGameScene = GetIGameInterface();

	CModel model(i, pGameScene, cs);
	model.DoExport((char *)name, szCommand, nRenderID, bExportKeyFrame, bExportSubMesh, bInWorldSpace, szExternalSkeleton, bOptimizeSkeleton, bGPUSkinning, lowSolutionParams, mediumSolutionParams, highSolutionParams, ultraSolutionParams, extremeSolutionParams);
	pGameScene->ReleaseIGame();

	return IMPEXP_SUCCESS;
}

//
// 加载参数
//
void CMaxExporter::LoadParams(char * const szFileName)
{
	FILE *pFile = fopen(szFileName, "r");
	if (!pFile) return;

	fscanf(pFile, "%d", &cs);
	fscanf(pFile, "%d", &nRenderID);
	fscanf(pFile, "%d", &bExportKeyFrame);
	fscanf(pFile, "%d", &bExportSubMesh);
	fscanf(pFile, "%d", &bInWorldSpace);
	fscanf(pFile, "%d", &bOptimizeSkeleton);
	fscanf(pFile, "%d", &bGPUSkinning);
	fscanf(pFile, "%d %d %d %d", &lowSolutionParams.lighting, &lowSolutionParams.bLightmap, &lowSolutionParams.bEnvmap, &lowSolutionParams.bShadowmap);
	fscanf(pFile, "%d %d %d %d", &mediumSolutionParams.lighting, &mediumSolutionParams.bLightmap, &mediumSolutionParams.bEnvmap, &mediumSolutionParams.bShadowmap);
	fscanf(pFile, "%d %d %d %d", &highSolutionParams.lighting, &highSolutionParams.bLightmap, &highSolutionParams.bEnvmap, &highSolutionParams.bShadowmap);
	fscanf(pFile, "%d %d %d %d", &ultraSolutionParams.lighting, &ultraSolutionParams.bLightmap, &ultraSolutionParams.bEnvmap, &ultraSolutionParams.bShadowmap);
	fscanf(pFile, "%d %d %d %d", &extremeSolutionParams.lighting, &extremeSolutionParams.bLightmap, &extremeSolutionParams.bEnvmap, &extremeSolutionParams.bShadowmap);
	fscanf(pFile, "%s", szCommand);

	fclose(pFile);
}

//
// 保存参数
//
void CMaxExporter::SaveParams(char * const szFileName) const
{
	FILE *pFile = fopen(szFileName, "w");
	if (!pFile) return;

	fprintf(pFile, "%d\n", cs);
	fprintf(pFile, "%d\n", nRenderID);
	fprintf(pFile, "%d\n", bExportKeyFrame);
	fprintf(pFile, "%d\n", bExportSubMesh);
	fprintf(pFile, "%d\n", bInWorldSpace);
	fprintf(pFile, "%d\n", bOptimizeSkeleton);
	fprintf(pFile, "%d\n", bGPUSkinning);
	fprintf(pFile, "%d %d %d %d\n", (int)lowSolutionParams.lighting, lowSolutionParams.bLightmap, lowSolutionParams.bEnvmap, lowSolutionParams.bShadowmap);
	fprintf(pFile, "%d %d %d %d\n", (int)mediumSolutionParams.lighting, mediumSolutionParams.bLightmap, mediumSolutionParams.bEnvmap, mediumSolutionParams.bShadowmap);
	fprintf(pFile, "%d %d %d %d\n", (int)highSolutionParams.lighting, highSolutionParams.bLightmap, highSolutionParams.bEnvmap, highSolutionParams.bShadowmap);
	fprintf(pFile, "%d %d %d %d\n", (int)ultraSolutionParams.lighting, ultraSolutionParams.bLightmap, ultraSolutionParams.bEnvmap, ultraSolutionParams.bShadowmap);
	fprintf(pFile, "%d %d %d %d\n", (int)extremeSolutionParams.lighting, extremeSolutionParams.bLightmap, extremeSolutionParams.bEnvmap, extremeSolutionParams.bShadowmap);
	fprintf(pFile, "%s\n", szCommand);

	fclose(pFile);
}

#include "MaxExporter.h"


INT_PTR CALLBACK MainPanelProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CMaxExporter *pMaxExporter = NULL;

	switch(message) {
	case WM_INITDIALOG:
		pMaxExporter = (CMaxExporter *)lParam;
		_ASSERT(pMaxExporter);

		// ��ʼ������λ��
		RECT rcMax;
		RECT rcWindow;

		int widthMax, heightMax;
		int widthWin, heightWin;

		char szRenderID[128];
		memset(szRenderID, 0, sizeof(szRenderID));

		GetWindowRect(GetActiveWindow(), &rcMax);
		GetWindowRect(hDlg, &rcWindow);

		widthMax = rcMax.right - rcMax.left + 1;
		heightMax = rcMax.bottom - rcMax.top + 1;

		widthWin = rcWindow.right - rcWindow.left + 1;
		heightWin = rcWindow.bottom - rcWindow.top + 1;

		SetWindowPos(hDlg, NULL, (widthMax - widthWin)/2, (heightMax - heightWin)/2, widthWin, heightWin, SWP_SHOWWINDOW);

		// ��ʼ������ϵͳ
		switch (pMaxExporter->cs) {
		case IGameConversionManager::IGAME_OGL:
			CheckDlgButton(hDlg, IDC_RADIO_COORD_SYSTEM_OGL, BST_CHECKED);
			break;
		case IGameConversionManager::IGAME_D3D:
			CheckDlgButton(hDlg, IDC_RADIO_COORD_SYSTEM_D3D, BST_CHECKED);
			break;
		case IGameConversionManager::IGAME_MAX:
			CheckDlgButton(hDlg, IDC_RADIO_COORD_SYSTEM_MAX, BST_CHECKED);
			break;
		}

		// ��ʼ����������
		if (pMaxExporter->bExportKeyFrame) {
			CheckDlgButton(hDlg, IDC_RADIO_KEY_FRAME, BST_CHECKED);
		}
		else {
			CheckDlgButton(hDlg, IDC_RADIO_ALL_FRAME, BST_CHECKED);
		}

		sprintf(szRenderID, "%d", pMaxExporter->nRenderID);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT_RENDER_ID), szRenderID);

		if (pMaxExporter->bExportSubMesh) {
			CheckDlgButton(hDlg, IDC_CHECK_SUB_MESH, BST_CHECKED);
		}

		if (pMaxExporter->bInWorldSpace) {
			CheckDlgButton(hDlg, IDC_CHECK_WORLD_SPACE, BST_CHECKED);
		}

		if (pMaxExporter->bOptimizeSkeleton) {
			CheckDlgButton(hDlg, IDC_CHECK_OPTIMIZE_SKELETON, BST_CHECKED);
		}

		if (pMaxExporter->bGPUSkinning) {
			CheckDlgButton(hDlg, IDC_CHECK_GPU_SKINNING, BST_CHECKED);
		}

		if (pMaxExporter->lowSolutionParams.lighting == LIGHTING_NONE) {
			CheckDlgButton(hDlg, IDC_RADIO_LOW_NONE_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->lowSolutionParams.lighting == LIGHTING_SIMPLE) {
			CheckDlgButton(hDlg, IDC_RADIO_LOW_SIMPLE_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->lowSolutionParams.lighting == LIGHTING_ADVANCED) {
			CheckDlgButton(hDlg, IDC_RADIO_LOW_ADVANCED_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->lowSolutionParams.bLightmap) {
			CheckDlgButton(hDlg, IDC_CHECK_LOW_LIGHTMAP, BST_CHECKED);
		}
		if (pMaxExporter->lowSolutionParams.bEnvmap) {
			CheckDlgButton(hDlg, IDC_CHECK_LOW_ENVMAP, BST_CHECKED);
		}
		if (pMaxExporter->lowSolutionParams.bShadowmap) {
			CheckDlgButton(hDlg, IDC_CHECK_LOW_SHADOWMAP, BST_CHECKED);
		}

		if (pMaxExporter->mediumSolutionParams.lighting == LIGHTING_NONE) {
			CheckDlgButton(hDlg, IDC_RADIO_MEDIUM_NONE_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->mediumSolutionParams.lighting == LIGHTING_SIMPLE) {
			CheckDlgButton(hDlg, IDC_RADIO_MEDIUM_SIMPLE_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->mediumSolutionParams.lighting == LIGHTING_ADVANCED) {
			CheckDlgButton(hDlg, IDC_RADIO_MEDIUM_ADVANCED_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->mediumSolutionParams.bLightmap) {
			CheckDlgButton(hDlg, IDC_CHECK_MEDIUM_LIGHTMAP, BST_CHECKED);
		}
		if (pMaxExporter->mediumSolutionParams.bEnvmap) {
			CheckDlgButton(hDlg, IDC_CHECK_MEDIUM_ENVMAP, BST_CHECKED);
		}
		if (pMaxExporter->mediumSolutionParams.bShadowmap) {
			CheckDlgButton(hDlg, IDC_CHECK_MEDIUM_SHADOWMAP, BST_CHECKED);
		}

		if (pMaxExporter->highSolutionParams.lighting == LIGHTING_NONE) {
			CheckDlgButton(hDlg, IDC_RADIO_HIGH_NONE_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->highSolutionParams.lighting == LIGHTING_SIMPLE) {
			CheckDlgButton(hDlg, IDC_RADIO_HIGH_SIMPLE_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->highSolutionParams.lighting == LIGHTING_ADVANCED) {
			CheckDlgButton(hDlg, IDC_RADIO_HIGH_ADVANCED_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->highSolutionParams.bLightmap) {
			CheckDlgButton(hDlg, IDC_CHECK_HIGH_LIGHTMAP, BST_CHECKED);
		}
		if (pMaxExporter->highSolutionParams.bEnvmap) {
			CheckDlgButton(hDlg, IDC_CHECK_HIGH_ENVMAP, BST_CHECKED);
		}
		if (pMaxExporter->highSolutionParams.bShadowmap) {
			CheckDlgButton(hDlg, IDC_CHECK_HIGH_SHADOWMAP, BST_CHECKED);
		}

		if (pMaxExporter->ultraSolutionParams.lighting == LIGHTING_NONE) {
			CheckDlgButton(hDlg, IDC_RADIO_ULTRA_NONE_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->ultraSolutionParams.lighting == LIGHTING_SIMPLE) {
			CheckDlgButton(hDlg, IDC_RADIO_ULTRA_SIMPLE_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->ultraSolutionParams.lighting == LIGHTING_ADVANCED) {
			CheckDlgButton(hDlg, IDC_RADIO_ULTRA_ADVANCED_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->ultraSolutionParams.bLightmap) {
			CheckDlgButton(hDlg, IDC_CHECK_ULTRA_LIGHTMAP, BST_CHECKED);
		}
		if (pMaxExporter->ultraSolutionParams.bEnvmap) {
			CheckDlgButton(hDlg, IDC_CHECK_ULTRA_ENVMAP, BST_CHECKED);
		}
		if (pMaxExporter->ultraSolutionParams.bShadowmap) {
			CheckDlgButton(hDlg, IDC_CHECK_ULTRA_SHADOWMAP, BST_CHECKED);
		}

		if (pMaxExporter->extremeSolutionParams.lighting == LIGHTING_NONE) {
			CheckDlgButton(hDlg, IDC_RADIO_EXTREME_NONE_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->extremeSolutionParams.lighting == LIGHTING_SIMPLE) {
			CheckDlgButton(hDlg, IDC_RADIO_EXTREME_SIMPLE_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->extremeSolutionParams.lighting == LIGHTING_ADVANCED) {
			CheckDlgButton(hDlg, IDC_RADIO_EXTREME_ADVANCED_LIGHTING, BST_CHECKED);
		}
		if (pMaxExporter->extremeSolutionParams.bLightmap) {
			CheckDlgButton(hDlg, IDC_CHECK_EXTREME_LIGHTMAP, BST_CHECKED);
		}
		if (pMaxExporter->extremeSolutionParams.bEnvmap) {
			CheckDlgButton(hDlg, IDC_CHECK_EXTREME_ENVMAP, BST_CHECKED);
		}
		if (pMaxExporter->extremeSolutionParams.bShadowmap) {
			CheckDlgButton(hDlg, IDC_CHECK_EXTREME_SHADOWMAP, BST_CHECKED);
		}

		// ��ʼ��������
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT_COMMAND), pMaxExporter->szCommand);

		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		// ����ϵͳ
		case IDC_RADIO_COORD_SYSTEM_OGL:
			pMaxExporter->cs = IGameConversionManager::IGAME_OGL;
			break;
		case IDC_RADIO_COORD_SYSTEM_D3D:
			pMaxExporter->cs = IGameConversionManager::IGAME_D3D;
			break;
		case IDC_RADIO_COORD_SYSTEM_MAX:
			pMaxExporter->cs = IGameConversionManager::IGAME_MAX;
			break;

		// ��������
		case IDC_RADIO_KEY_FRAME:
			pMaxExporter->bExportKeyFrame = TRUE;
			break;
		case IDC_RADIO_ALL_FRAME:
			pMaxExporter->bExportKeyFrame = FALSE;
			break;

		// ����������
		case IDC_CHECK_SUB_MESH:
			pMaxExporter->bExportSubMesh = pMaxExporter->bExportSubMesh ? FALSE : TRUE;
			break;

		// ��������ռ�����
		case IDC_CHECK_WORLD_SPACE:
			pMaxExporter->bInWorldSpace = pMaxExporter->bInWorldSpace ? FALSE : TRUE;
			break;

		// �Ż�����
		case IDC_CHECK_OPTIMIZE_SKELETON:
			pMaxExporter->bOptimizeSkeleton = pMaxExporter->bOptimizeSkeleton ? FALSE : TRUE;
			break;

		// GPU��Ƥ
		case IDC_CHECK_GPU_SKINNING:
			pMaxExporter->bGPUSkinning = pMaxExporter->bGPUSkinning ? FALSE : TRUE;
			break;

		// ʵʱ����
		case IDC_RADIO_LOW_NONE_LIGHTING:
			pMaxExporter->lowSolutionParams.lighting = LIGHTING_NONE;
			break;
		case IDC_RADIO_LOW_SIMPLE_LIGHTING:
			pMaxExporter->lowSolutionParams.lighting = LIGHTING_SIMPLE;
			break;
		case IDC_RADIO_LOW_ADVANCED_LIGHTING:
			pMaxExporter->lowSolutionParams.lighting = LIGHTING_ADVANCED;
			break;
		case IDC_RADIO_MEDIUM_NONE_LIGHTING:
			pMaxExporter->mediumSolutionParams.lighting = LIGHTING_NONE;
			break;
		case IDC_RADIO_MEDIUM_SIMPLE_LIGHTING:
			pMaxExporter->mediumSolutionParams.lighting = LIGHTING_SIMPLE;
			break;
		case IDC_RADIO_MEDIUM_ADVANCED_LIGHTING:
			pMaxExporter->mediumSolutionParams.lighting = LIGHTING_ADVANCED;
			break;
		case IDC_RADIO_HIGH_NONE_LIGHTING:
			pMaxExporter->highSolutionParams.lighting = LIGHTING_NONE;
			break;
		case IDC_RADIO_HIGH_SIMPLE_LIGHTING:
			pMaxExporter->highSolutionParams.lighting = LIGHTING_SIMPLE;
			break;
		case IDC_RADIO_HIGH_ADVANCED_LIGHTING:
			pMaxExporter->highSolutionParams.lighting = LIGHTING_ADVANCED;
			break;
		case IDC_RADIO_ULTRA_NONE_LIGHTING:
			pMaxExporter->ultraSolutionParams.lighting = LIGHTING_NONE;
			break;
		case IDC_RADIO_ULTRA_SIMPLE_LIGHTING:
			pMaxExporter->ultraSolutionParams.lighting = LIGHTING_SIMPLE;
			break;
		case IDC_RADIO_ULTRA_ADVANCED_LIGHTING:
			pMaxExporter->ultraSolutionParams.lighting = LIGHTING_ADVANCED;
			break;
		case IDC_RADIO_EXTREME_NONE_LIGHTING:
			pMaxExporter->extremeSolutionParams.lighting = LIGHTING_NONE;
			break;
		case IDC_RADIO_EXTREME_SIMPLE_LIGHTING:
			pMaxExporter->extremeSolutionParams.lighting = LIGHTING_SIMPLE;
			break;
		case IDC_RADIO_EXTREME_ADVANCED_LIGHTING:
			pMaxExporter->extremeSolutionParams.lighting = LIGHTING_ADVANCED;
			break;

		// ������ͼ
		case IDC_CHECK_LOW_LIGHTMAP:
			pMaxExporter->lowSolutionParams.bLightmap = pMaxExporter->lowSolutionParams.bLightmap ? FALSE : TRUE;
			break;
		case IDC_CHECK_MEDIUM_LIGHTMAP:
			pMaxExporter->mediumSolutionParams.bLightmap = pMaxExporter->mediumSolutionParams.bLightmap ? FALSE : TRUE;
			break;
		case IDC_CHECK_HIGH_LIGHTMAP:
			pMaxExporter->highSolutionParams.bLightmap = pMaxExporter->highSolutionParams.bLightmap ? FALSE : TRUE;
			break;
		case IDC_CHECK_ULTRA_LIGHTMAP:
			pMaxExporter->ultraSolutionParams.bLightmap = pMaxExporter->ultraSolutionParams.bLightmap ? FALSE : TRUE;
			break;
		case IDC_CHECK_EXTREME_LIGHTMAP:
			pMaxExporter->extremeSolutionParams.bLightmap = pMaxExporter->extremeSolutionParams.bLightmap ? FALSE : TRUE;
			break;

		// ������ͼ
		case IDC_CHECK_LOW_ENVMAP:
			pMaxExporter->lowSolutionParams.bEnvmap = pMaxExporter->lowSolutionParams.bEnvmap ? FALSE : TRUE;
			break;
		case IDC_CHECK_MEDIUM_ENVMAP:
			pMaxExporter->mediumSolutionParams.bEnvmap = pMaxExporter->mediumSolutionParams.bEnvmap ? FALSE : TRUE;
			break;
		case IDC_CHECK_HIGH_ENVMAP:
			pMaxExporter->highSolutionParams.bEnvmap = pMaxExporter->highSolutionParams.bEnvmap ? FALSE : TRUE;
			break;
		case IDC_CHECK_ULTRA_ENVMAP:
			pMaxExporter->ultraSolutionParams.bEnvmap = pMaxExporter->ultraSolutionParams.bEnvmap ? FALSE : TRUE;
			break;
		case IDC_CHECK_EXTREME_ENVMAP:
			pMaxExporter->extremeSolutionParams.bEnvmap = pMaxExporter->extremeSolutionParams.bEnvmap ? FALSE : TRUE;
			break;

		// ��Ӱͼ
		case IDC_CHECK_LOW_SHADOWMAP:
			pMaxExporter->lowSolutionParams.bShadowmap = pMaxExporter->lowSolutionParams.bShadowmap ? FALSE : TRUE;
			break;
		case IDC_CHECK_MEDIUM_SHADOWMAP:
			pMaxExporter->mediumSolutionParams.bShadowmap = pMaxExporter->mediumSolutionParams.bShadowmap ? FALSE : TRUE;
			break;
		case IDC_CHECK_HIGH_SHADOWMAP:
			pMaxExporter->highSolutionParams.bShadowmap = pMaxExporter->highSolutionParams.bShadowmap ? FALSE : TRUE;
			break;
		case IDC_CHECK_ULTRA_SHADOWMAP:
			pMaxExporter->ultraSolutionParams.bShadowmap = pMaxExporter->ultraSolutionParams.bShadowmap ? FALSE : TRUE;
			break;
		case IDC_CHECK_EXTREME_SHADOWMAP:
			pMaxExporter->extremeSolutionParams.bShadowmap = pMaxExporter->extremeSolutionParams.bShadowmap ? FALSE : TRUE;
			break;

		// ����
		case IDOK:
			GetWindowText(GetDlgItem(hDlg, IDC_EDIT_RENDER_ID), szRenderID, sizeof(szRenderID));
			GetWindowText(GetDlgItem(hDlg, IDC_EDIT_COMMAND), pMaxExporter->szCommand, sizeof(pMaxExporter->szCommand));
			GetWindowText(GetDlgItem(hDlg, IDC_EDIT_EXTERNAL_SKELETON), pMaxExporter->szExternalSkeleton, sizeof(pMaxExporter->szExternalSkeleton));
			pMaxExporter->nRenderID = atoi(szRenderID);
			EndDialog(hDlg, 1);
			return TRUE;

		// ȡ������
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}

		break;
	}

	return FALSE;
}

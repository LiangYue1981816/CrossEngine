#include "MaxExporter.h"


//
// 导出
//
void CModel::DoExport(const char *szName, const char *szCommand, int nRenderID, BOOL bExportKeyFrame, BOOL bExportSubMesh, BOOL bInWorldSpcae, char *szExternalSkeleton, BOOL bOptimizeSkeleton, BOOL bGUPSkinning, SOLUTION_PARAMS low, SOLUTION_PARAMS medium, SOLUTION_PARAMS high, SOLUTION_PARAMS ultra, SOLUTION_PARAMS extreme)
{
	Free();

	//
	// 1. 初始化
	//
	IGameConversionManager *pConversionManager = GetConversionManager();
	pConversionManager->SetCoordSystem(m_coordSystem);

	m_pGameScene->InitialiseIGame(false);

	//
	// 2. 导出模型数据
	//
	char szPath[_MAX_PATH];
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	char szFName[_MAX_FNAME];
	char szExt[_MAX_EXT];

	_splitpath(szName, szDrive, szDir, szFName, szExt);
	sprintf(szPath, "%s%s", szDrive, szDir);

	// 注意: 导出顺序不要随意调整, 他们之间有依赖关系!!!
	DoExportMaterial(szPath);
	DoExportBoneHierarchy();
	DoExportMesh(bInWorldSpcae);	
	DoExportAnimList();
	DoExportAnimVertex();
	DoExportAnimSkeleton(bExportKeyFrame);
	OptimizeSkeleton(bOptimizeSkeleton);

	//
	// 3. 写入模型数据
	//
	char szMaterialFileName[_MAX_FNAME];
	char szMeshFileName[_MAX_FNAME];
	char szSkeletonFileName[_MAX_FNAME];
	char szSkeletonBinFileName[_MAX_FNAME];
	char szExternalCommand[_MAX_PATH];

	// 3.1. D3D .x
	if (!stricmp(&szExt[1], EXT_NAME1)) {
		sprintf(szMeshFileName, "%s%s.mesh.x", szPath, szFName);
		sprintf(szSkeletonFileName, "%s%s.skeleton.x", szPath, szFName);

		// 3.1.1. 输出网格
		D3DX_WriteMesh(szMeshFileName, bOptimizeSkeleton);

		// 3.1.2. 输出骨骼动画
		if (m_pBoneHierarchy->GetBoneCount(FALSE) > 0) {
			D3DX_WriteAnimSkeleton(szSkeletonFileName, bOptimizeSkeleton);
		}
	}
	// 3.2. Ogre .xml
	else if (!stricmp(&szExt[1], EXT_NAME2)) {
		sprintf(szMaterialFileName, "%s%s.material", szPath, szFName);
		sprintf(szMeshFileName, "%s%s.mesh.xml", szPath, szFName);
		sprintf(szSkeletonFileName, "%s%s.skeleton.xml", szPath, szFName);
		sprintf(szSkeletonBinFileName, "%s.skeleton", szFName);

		// 3.2.1. 输出网格和材质
		//        说明: 导出插件以xml形式输出网格和骨骼动画, 在输出完成之后调用(可选择)"OgreXmlConverter.exe"
		//              完成二进制转换. 该转换工具转换后会自动将.xml扩展名转换为.mesh/.skeleton, 因此在输出网
		//              格的时候直接使用转换后的骨骼名，即xxx.skeleton
		OgreXML_WriteMaterial(szFName, szMaterialFileName);
		OgreXML_WriteMesh(szFName, szMeshFileName, szSkeletonBinFileName);

		// 3.2.2. 输出骨骼动画
		if (m_pBoneHierarchy->GetBoneCount(FALSE) > 0) {
			OgreXML_WriteAnimSkeleton(szSkeletonFileName, bOptimizeSkeleton);
		}
	}
	// 3.3. Cross .Mesh
	else if (!stricmp(&szExt[1], EXT_NAME3)) {
		// 3.3.1. 骨骼动画名
		//        说明: 导出插件可以选择使用外部骨骼动画, 最大限度共用已有动画资源. 例如, 在游戏中很多角色动
		//              画都是一致的, 这样没必要每个角色都导出动画, 而是只导出某一个角色的动画并指定其他角色
		//              使用该动画即可.
		BOOL bExternalSkeleton = strlen(szExternalSkeleton) > 0 ? TRUE : FALSE;

		if (bExternalSkeleton) {
			memset(szSkeletonFileName, 0, sizeof(szSkeletonFileName));
			strcpy(szSkeletonBinFileName, szExternalSkeleton);
		}
		else {
			sprintf(szSkeletonFileName, "%s%s.skeleton", szPath, szFName);
			sprintf(szSkeletonBinFileName, "%s.skeleton", szFName);
		}

		// 3.3.2. 输出网格和材质
		Cross_WriteMaterial(szFName, szPath, nRenderID, bGUPSkinning, low, medium, high, ultra, extreme);

		if (bExportSubMesh) {
			// 3.3.2.1. 收集并归类子网格
			//          归类规则:
			//          1. 子网格名以第一个"."分为两段, 例如Body.Lod0.Mesh会被分割为Body和Lod0.Mesh两个名称
			//          2. 当出现多个相同主名的子网格, 这些相同主名的子网格会导出到同一个网格文件上, 其中包
			//             括多个网格. 例如Body.Render,Body.Physic会被导出到Body.mesh文件, 其中包括mesh和
			//             physic两个子网格.
			map<string, vector<CSubMesh*>> subMeshs;

			for (vector<CSubMesh*>::const_iterator itSubMesh = m_pSkinMesh->subMeshs.begin(); itSubMesh != m_pSkinMesh->subMeshs.end(); ++itSubMesh) {
				CSubMesh *pSubMesh = *itSubMesh;
				_ASSERT(pSubMesh);

				char szFirstName[_MAX_PATH];

				memset(szFirstName, 0, sizeof(szFirstName));
				strcpy(szFirstName, pSubMesh->szName);

				if (char *pSecondName = strchr(szFirstName, '.')) {
					*pSecondName++ = 0;

					memset(pSubMesh->szName, 0, sizeof(pSubMesh->szName));
					strcpy(pSubMesh->szName, pSecondName);
				}

				subMeshs[szFirstName].push_back(pSubMesh);
			}

			// 3.3.2.2. 输出子网格
			for (map<string, vector<CSubMesh*>>::const_iterator itSubMesh = subMeshs.begin(); itSubMesh != subMeshs.end(); ++itSubMesh) {
				sprintf(szMeshFileName, "%s%s.%s.mesh", szPath, szFName, itSubMesh->first.c_str());
				Cross_WriteMesh(szFName, szMeshFileName, szSkeletonBinFileName, itSubMesh->second);
			}

			// 3.3.2.3. 输出子网格包描述
			char szPackageFileName[_MAX_PATH];
			sprintf(szPackageFileName, "%s%s.xml", szPath, szFName);

			FILE *pFile = fopen(szPackageFileName, "wb");
			{
				fprintf(pFile, "<package name=\"%s\">\n", szFName);
				{
					for (map<string, vector<CSubMesh*>>::const_iterator itSubMesh = subMeshs.begin(); itSubMesh != subMeshs.end(); ++itSubMesh) {
						CSubMesh *pSubMesh = itSubMesh->second[0];
						_ASSERT(pSubMesh);

						Point3 scale = pSubMesh->matrix.Scaling();
						Point3 position = pSubMesh->matrix.Translation();
						Quat orientation = pSubMesh->matrix.Rotation().Conjugate();

						fprintf(pFile, "\t<mesh name=\"%s.%s.mesh\" scale=\"%f %f %f\" position=\"%f %f %f\" orientation=\"%f %f %f %f\" />\n",
							szFName, itSubMesh->first.c_str(),
							scale.x, scale.y, scale.z,
							position.x, position.y, position.z,
							orientation.x, orientation.y, orientation.z, orientation.w);
					}
				}
				fprintf(pFile, "</package>");
			}
			fclose(pFile);
		}
		else {
			sprintf(szMeshFileName, "%s%s.mesh", szPath, szFName);
			Cross_WriteMesh(szFName, szMeshFileName, szSkeletonBinFileName, m_pSkinMesh->subMeshs);
		}

		// 3.3.3. 输出骨骼动画
		if (m_pBoneHierarchy->GetBoneCount(FALSE) > 0 && !bExternalSkeleton) {
			Cross_WriteAnimSkeleton(szSkeletonFileName, bOptimizeSkeleton);
		}
	}

	//
	// 4. 执行外部命令
	//
	if (strlen(szCommand)) {
		sprintf(szExternalCommand, "%s %s", szCommand, szMeshFileName);
		system(szExternalCommand);

		sprintf(szExternalCommand, "%s %s", szCommand, szSkeletonFileName);
		system(szExternalCommand);
	}
}

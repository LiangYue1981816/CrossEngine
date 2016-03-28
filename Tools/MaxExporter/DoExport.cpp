#include "MaxExporter.h"


//
// ����
//
void CModel::DoExport(const char *szName, const char *szCommand, int nRenderID, BOOL bExportKeyFrame, BOOL bExportSubMesh, BOOL bInWorldSpcae, char *szExternalSkeleton, BOOL bOptimizeSkeleton, BOOL bGUPSkinning, SOLUTION_PARAMS low, SOLUTION_PARAMS medium, SOLUTION_PARAMS high, SOLUTION_PARAMS ultra, SOLUTION_PARAMS extreme)
{
	Free();

	//
	// 1. ��ʼ��
	//
	IGameConversionManager *pConversionManager = GetConversionManager();
	pConversionManager->SetCoordSystem(m_coordSystem);

	m_pGameScene->InitialiseIGame(false);

	//
	// 2. ����ģ������
	//
	char szPath[_MAX_PATH];
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	char szFName[_MAX_FNAME];
	char szExt[_MAX_EXT];

	_splitpath(szName, szDrive, szDir, szFName, szExt);
	sprintf(szPath, "%s%s", szDrive, szDir);

	// ע��: ����˳��Ҫ�������, ����֮����������ϵ!!!
	DoExportMaterial(szPath);
	DoExportBoneHierarchy();
	DoExportMesh(bInWorldSpcae);	
	DoExportAnimList();
	DoExportAnimVertex();
	DoExportAnimSkeleton(bExportKeyFrame);
	OptimizeSkeleton(bOptimizeSkeleton);

	//
	// 3. д��ģ������
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

		// 3.1.1. �������
		D3DX_WriteMesh(szMeshFileName, bOptimizeSkeleton);

		// 3.1.2. �����������
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

		// 3.2.1. �������Ͳ���
		//        ˵��: ���������xml��ʽ�������͹�������, ��������֮�����(��ѡ��)"OgreXmlConverter.exe"
		//              ��ɶ�����ת��. ��ת������ת������Զ���.xml��չ��ת��Ϊ.mesh/.skeleton, ����������
		//              ���ʱ��ֱ��ʹ��ת����Ĺ���������xxx.skeleton
		OgreXML_WriteMaterial(szFName, szMaterialFileName);
		OgreXML_WriteMesh(szFName, szMeshFileName, szSkeletonBinFileName);

		// 3.2.2. �����������
		if (m_pBoneHierarchy->GetBoneCount(FALSE) > 0) {
			OgreXML_WriteAnimSkeleton(szSkeletonFileName, bOptimizeSkeleton);
		}
	}
	// 3.3. Cross .Mesh
	else if (!stricmp(&szExt[1], EXT_NAME3)) {
		// 3.3.1. ����������
		//        ˵��: �����������ѡ��ʹ���ⲿ��������, ����޶ȹ������ж�����Դ. ����, ����Ϸ�кܶ��ɫ��
		//              ������һ�µ�, ����û��Ҫÿ����ɫ����������, ����ֻ����ĳһ����ɫ�Ķ�����ָ��������ɫ
		//              ʹ�øö�������.
		BOOL bExternalSkeleton = strlen(szExternalSkeleton) > 0 ? TRUE : FALSE;

		if (bExternalSkeleton) {
			memset(szSkeletonFileName, 0, sizeof(szSkeletonFileName));
			strcpy(szSkeletonBinFileName, szExternalSkeleton);
		}
		else {
			sprintf(szSkeletonFileName, "%s%s.skeleton", szPath, szFName);
			sprintf(szSkeletonBinFileName, "%s.skeleton", szFName);
		}

		// 3.3.2. �������Ͳ���
		Cross_WriteMaterial(szFName, szPath, nRenderID, bGUPSkinning, low, medium, high, ultra, extreme);

		if (bExportSubMesh) {
			// 3.3.2.1. �ռ�������������
			//          �������:
			//          1. ���������Ե�һ��"."��Ϊ����, ����Body.Lod0.Mesh�ᱻ�ָ�ΪBody��Lod0.Mesh��������
			//          2. �����ֶ����ͬ������������, ��Щ��ͬ������������ᵼ����ͬһ�������ļ���, ���а�
			//             ���������. ����Body.Render,Body.Physic�ᱻ������Body.mesh�ļ�, ���а���mesh��
			//             physic����������.
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

			// 3.3.2.2. ���������
			for (map<string, vector<CSubMesh*>>::const_iterator itSubMesh = subMeshs.begin(); itSubMesh != subMeshs.end(); ++itSubMesh) {
				sprintf(szMeshFileName, "%s%s.%s.mesh", szPath, szFName, itSubMesh->first.c_str());
				Cross_WriteMesh(szFName, szMeshFileName, szSkeletonBinFileName, itSubMesh->second);
			}

			// 3.3.2.3. ��������������
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

		// 3.3.3. �����������
		if (m_pBoneHierarchy->GetBoneCount(FALSE) > 0 && !bExternalSkeleton) {
			Cross_WriteAnimSkeleton(szSkeletonFileName, bOptimizeSkeleton);
		}
	}

	//
	// 4. ִ���ⲿ����
	//
	if (strlen(szCommand)) {
		sprintf(szExternalCommand, "%s %s", szCommand, szMeshFileName);
		system(szExternalCommand);

		sprintf(szExternalCommand, "%s %s", szCommand, szSkeletonFileName);
		system(szExternalCommand);
	}
}

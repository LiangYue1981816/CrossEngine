#include "MaxExporter.h"


//
// 导出材质
//
void CModel::DoExportMaterial(const char *szPath)
{
	for (int indexMaterial = 0; indexMaterial < m_pGameScene->GetRootMaterialCount(); indexMaterial++) {
		IGameMaterial *pRootMaterial = m_pGameScene->GetRootMaterial(indexMaterial);
		_ASSERT(pRootMaterial);

		//
		// 1. 多维子材质
		//
		if (pRootMaterial->GetSubMaterialCount()) {
			for (int indexMaterial = 0; indexMaterial < pRootMaterial->GetSubMaterialCount(); indexMaterial++) {
				IGameMaterial *pSubMaterial = pRootMaterial->GetSubMaterial(indexMaterial);
				_ASSERT(pSubMaterial);

				DoExportMaterial(szPath, pSubMaterial, pRootMaterial);
			}
		}
		//
		// 2. 单材质
		//
		else {
			DoExportMaterial(szPath, pRootMaterial, NULL);
		}
	}
}

//
// 导出材质
//
void CModel::DoExportMaterial(const char *szPath, IGameMaterial *pMaxMaterial, IGameMaterial *pMaxParentMaterial)
{
	_ASSERT(szPath);
	_ASSERT(pMaxMaterial);

	CMaterial *pMaterial = new CMaterial(pMaxMaterial);
	_ASSERT(pMaterial);

	//
	// 1. 材质名
	//    注意: 对于多维子材质的子材质而言, 他的名字是与父材质的组合
	//
	if (pMaxParentMaterial) {
		sprintf(pMaterial->szName, "%s%s", pMaxParentMaterial->GetMaterialName(), pMaxMaterial->GetMaterialName());
	}
	else {
		sprintf(pMaterial->szName, "%s", pMaxMaterial->GetMaterialName());
	}

	ReplaceInvalidChar(pMaterial->szName);

	//
	// 2. 材质数据
	//
	if (Mtl *pMtlMaxMaterial = pMaxMaterial->GetMaxMaterial()) {
		if (pMtlMaxMaterial->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) {
			pMaterial->isTwoSided = ((StdMat *)pMtlMaxMaterial)->GetTwoSided();
		}
	}

	IGameProperty *pPropAmbient = pMaxMaterial->GetAmbientData();
	IGameProperty *pPropDiffuse = pMaxMaterial->GetDiffuseData();
	IGameProperty *pPropEmissive = pMaxMaterial->GetEmissiveData();
	IGameProperty *pPropSpecular = pMaxMaterial->GetSpecularData();
	IGameProperty *pPropSpecularLevel = pMaxMaterial->GetSpecularLevelData();

	if (pPropAmbient) pPropAmbient->GetPropertyValue(pMaterial->ambient);
	if (pPropDiffuse) pPropDiffuse->GetPropertyValue(pMaterial->diffuse);
	if (pPropEmissive) pPropEmissive->GetPropertyValue(pMaterial->emissive);
	if (pPropSpecular) pPropSpecular->GetPropertyValue(pMaterial->specular);
	if (pPropSpecularLevel) pPropSpecularLevel->GetPropertyValue(pMaterial->specularLevel);

	//
	// 3. 纹理
	//
	char szFName[_MAX_FNAME];
	char szExtName[_MAX_EXT];
	char szTexFileName[_MAX_PATH];

	for (int indexTexture = 0; indexTexture < pMaxMaterial->GetNumberOfTextureMaps(); indexTexture++) {
		IGameTextureMap* pTexMap = pMaxMaterial->GetIGameTextureMap(indexTexture);
		_ASSERT(pTexMap);

		if (!stricmp("bitmap", pTexMap->GetTextureClass())) {
			int texType = pTexMap->GetStdMapSlot();
			_splitpath(pTexMap->GetBitmapFileName(), NULL, NULL, szFName, szExtName);

			// 3.1. 获得纹理单元
			sprintf(szTexFileName, "%s%s", szFName, szExtName);
			ReplaceInvalidChar(szTexFileName);

			pMaterial->texUnits[texType] = szTexFileName;

			// 3.2. 拷贝出纹理
			sprintf(szTexFileName, "%s%s%s", szPath, szFName, szExtName);
			ReplaceInvalidChar(szTexFileName);

			CopyFile(pTexMap->GetBitmapFileName(), szTexFileName, FALSE);
		}
	}

	//
	// 4. 添加到材质集合
	//
	m_materials.push_back(pMaterial);
}

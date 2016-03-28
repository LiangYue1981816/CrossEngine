#include "MaxExporter.h"


//
// 导出网格
// 注意: 这里只导出位于根节点的网格, 不导出网格的子网格
//
void CModel::DoExportMesh(BOOL bInWorldSpcae)
{
	//
	// 1. 导出位于根节点的所有网格
	//
	m_pSkinMesh = new CMesh;
	_ASSERT(m_pSkinMesh);

	//
	// 2. 导出网格网格
	//    注意: 以第0贞的状态作为蒙皮网格
	//
	m_pGameScene->SetStaticFrame(0);

	for (int indexNode = 0; indexNode < m_pGameScene->GetTopLevelNodeCount(); indexNode++) {
		IGameNode *pNode = m_pGameScene->GetTopLevelNode(indexNode);
		_ASSERT(pNode);

		DoExportMesh(pNode, m_pSkinMesh, bInWorldSpcae);
	}
}

//
// 导出顶点动画
//
void CModel::DoExportAnimVertex(void)
{
	//
	// 1. 导出每个动画
	//
	for (vector<CAnimVertex*>::const_iterator itAnim = m_animVertexs.begin(); itAnim != m_animVertexs.end(); ++itAnim) {
		CAnimVertex *pAnim = *itAnim;
		_ASSERT(pAnim);

		//
		// 2. 导出动画每一贞
		//
		for (int indexFrame = pAnim->pAnimInfo->startFrame; indexFrame <= pAnim->pAnimInfo->endFrame; indexFrame++) {
			CMesh *pMeshFrame = new CMesh;
			_ASSERT(pMeshFrame);

			m_pGameScene->SetStaticFrame(indexFrame);

			for (int indexNode = 0; indexNode < m_pGameScene->GetTopLevelNodeCount(); indexNode++) {
				IGameNode *pNode = m_pGameScene->GetTopLevelNode(indexNode);
				_ASSERT(pNode);

				DoExportMesh(pNode, pMeshFrame, FALSE);
			}

			pAnim->frames.push_back(pMeshFrame);
		}
	}
}

//
// 导出网格
//
void CModel::DoExportMesh(IGameNode *pNode, CMesh *pMesh, BOOL bInWorldSpcae)
{
	_ASSERT(pNode);
	_ASSERT(pMesh);

	//
	// 1. 导出该节点网格
	//
	if (CSubMesh *pSubMesh = DoExportSubMesh(pNode, bInWorldSpcae)) {
		pMesh->subMeshs.push_back(pSubMesh);
	}

	//
	// 2. 导出子节点网格
	//
	for (int indexChildNode = 0; indexChildNode < pNode->GetChildCount(); indexChildNode++) {
		IGameNode *pChildNode = pNode->GetNodeChild(indexChildNode);
		_ASSERT(pChildNode);

		DoExportMesh(pChildNode, pMesh, bInWorldSpcae);
	}
}

//
// 导出子网格
//
CSubMesh* CModel::DoExportSubMesh(IGameNode *pNode, BOOL bInWorldSpcae)
{
	_ASSERT(pNode);

	//
	// 1. 检查节点类型, 不处理隐藏网格
	//
	if (pNode->IsNodeHidden() ||
		pNode->GetIGameObject()->GetIGameType() != IGameObject::IGAME_MESH) {
		return NULL;
	}

	//
	// 2. 初始化
	//
	IGameMesh *pMesh = (IGameMesh *)pNode->GetIGameObject();
	IGameMaterial *pParentMaterial = pNode->GetNodeMaterial();
	_ASSERT(pMesh);

	pMesh->SetCreateOptimizedNormalList();
	pMesh->InitializeData();
	pMesh->InitializeBinormalData();

	if (pMesh->GetNumberOfFaces() <= 0 || pMesh->GetNumberOfVerts() <= 0) {
		return NULL;
	}

	//
	// 3. 导出网格数据
	//
	CSubMesh *pSubMesh = new CSubMesh(pMesh);
	_ASSERT(pSubMesh);

	Tab<int> mapChannels = pMesh->GetActiveMapChannelNum();
	pSubMesh->nNumTexChannel = mapChannels.Count();

	sprintf(pSubMesh->szName, "%s", pNode->GetName());

	if (pParentMaterial) {
		strcpy(pSubMesh->szMaterialName, pParentMaterial->GetMaterialName());
	}
	else {
		strcpy(pSubMesh->szMaterialName, "Null");
	}

	ReplaceInvalidChar(pSubMesh->szName);
	ReplaceInvalidChar(pSubMesh->szMaterialName);

	if (CMaterial *pMaterial = QueryMaterial(pSubMesh->szMaterialName)) {
		if (pMaterial->texUnits.find(ID_LM) != pMaterial->texUnits.end()) {
			strcpy(pSubMesh->szLightMapName, pMaterial->texUnits[ID_LM].c_str());
		}
	}

	pSubMesh->matrix = bInWorldSpcae ? pNode->GetWorldTM() : pNode->GetObjectTM();

	//
	// 3.1. 导出网格数据
	//
	for (int indexFace = 0; indexFace < pMesh->GetNumberOfFaces(); indexFace++) {
		FaceEx *pFaceEx = pMesh->GetFace(indexFace);
		_ASSERT(pFaceEx);

		CFace *pFace = new CFace(pFaceEx);
		_ASSERT(pFace);

		//
		// 3.1.1. 顶点数据, 剔除重复顶点重建三角面索引
		//
		for (int indexVertex = 0; indexVertex < 3; indexVertex++) {
			CVertex vertex;

			vertex.indexPosition = pFaceEx->vert[indexVertex];
			vertex.indexNormal = pFaceEx->norm[indexVertex];
			vertex.indexBinormal = pMesh->GetFaceVertexTangentBinormal(indexFace, indexVertex);

			vertex.position = pMesh->GetVertex(vertex.indexPosition, bInWorldSpcae ? false : true);
			vertex.normal = pMesh->GetNormal(vertex.indexNormal, bInWorldSpcae ? false : true);
			vertex.binormal = pMesh->GetBinormal(vertex.indexBinormal);

			if (pFaceEx->color[indexVertex] >= 0 && pFaceEx->color[indexVertex] < (DWORD)pMesh->GetNumberOfColorVerts() &&
				pFaceEx->alpha[indexVertex] >= 0 && pFaceEx->alpha[indexVertex] < (DWORD)pMesh->GetNumberOfAlphaVerts()) {
				float alpha;
				Point3 color;

				vertex.indexColor = pFaceEx->color[indexVertex];
				vertex.indexAlpha = pFaceEx->alpha[indexVertex];

				color = pMesh->GetColorVertex(vertex.indexColor);
				alpha = pMesh->GetAlphaVertex(vertex.indexAlpha);

				vertex.color.x = color.x;
				vertex.color.y = color.y;
				vertex.color.z = color.z;
				vertex.color.w = alpha;
			}

			for (int indexChannel = 0; indexChannel < mapChannels.Count(); indexChannel++) {
				Point3 texCoord;
				DWORD texMapping[3];

				if (pMesh->GetMapFaceIndex(mapChannels[indexChannel], indexFace, texMapping)) {
					if (pMesh->GetMapVertex(mapChannels[indexChannel], texMapping[indexVertex], texCoord)) {
						if (fabs(texCoord.z) < 0.001f) { // 注意: 过滤掉非纹理坐标!!!
							vertex.indexTexCoords.push_back(texMapping[indexVertex]);
							vertex.texCoords.push_back(texCoord);
						}
					}
				}
			}

			pFace->indexVertices[indexVertex] = pSubMesh->QueryVertexID(vertex);

			if (pFace->indexVertices[indexVertex] < 0) {
				pFace->indexVertices[indexVertex] = (int)pSubMesh->vertices.size();

				CVertex *pVertex = new CVertex(vertex);
				_ASSERT(pVertex);

				pSubMesh->vertices.push_back(pVertex);
			}
		}

		//
		// 3.1.2. 三角面材质
		//      注意: 在.X文件格式中三角面材质索引指的并不是整个max文件的材质
		//            索引, 而是三角面所在网格使用的材质集合的索引
		//
		if (IGameMaterial *pSubMaterial = pMesh->GetMaterialFromFace(pFaceEx)) {
			char szMaterialName[_MAX_PATH];

			if (pParentMaterial == pSubMaterial) {
				sprintf(szMaterialName, "%s", pSubMaterial->GetMaterialName());
			}
			else {
				_ASSERT(pParentMaterial);
				sprintf(szMaterialName, "%s%s", pParentMaterial->GetMaterialName(), pSubMaterial->GetMaterialName());
			}

			ReplaceInvalidChar(szMaterialName);

			if (CMaterial *pMaterial = QueryMaterial(szMaterialName)) {
				pMaterial->isValid = TRUE;
			}

			// 在当前网格材质集合中查找三角面使用的材质, 若三角面使用的是新材质
			// 则添加该材质到网格材质集合
			if ((pFace->indexMaterial = pSubMesh->QueryMaterialID(szMaterialName)) < 0) {
				pFace->indexMaterial = (int)pSubMesh->materials.size();
				pSubMesh->materials.push_back(QueryMaterial(szMaterialName));
			}
		}

		// 3.1.4. 添加三角面
		pSubMesh->faces.push_back(pFace);
	}

	//
	// 4. 导出网格数据
	//
	if (IGameSkin* pSkin = (IGameSkin *)pMesh->GetIGameSkin()) {
		pSubMesh->isSkinMesh = TRUE;

		for (int indexVertex = 0; indexVertex < (int)pSubMesh->vertices.size(); indexVertex++) {
			CVertex *pVertex = pSubMesh->vertices[indexVertex];
			_ASSERT(pVertex);

			// 4.1. 纪录顶点蒙皮信息
			int numBones = 0;

			switch (pSkin->GetVertexType(pVertex->indexPosition)) {
			case IGameSkin::IGAME_RIGID:
				numBones = 1;
				break;
			case IGameSkin::IGAME_RIGID_BLENDED:
				numBones = pSkin->GetNumberOfBones(pVertex->indexPosition);
				break;
			}

			for (int indexBone = 0; indexBone < numBones; indexBone++) {
				CSkinInfo skinInfo;
				IGameNode *pBoneNode = pSkin->GetIGameBone(pVertex->indexPosition, indexBone);

				char szBoneName[_MAX_PATH];

				sprintf(szBoneName, "%s", pBoneNode->GetName());
				ReplaceInvalidChar(szBoneName);

				skinInfo.boneID = m_pBoneHierarchy->QueryBoneID(szBoneName);
				skinInfo.weight = pSkin->GetWeight(pVertex->indexPosition, indexBone);

				if (skinInfo.boneID >= 0) {
					pVertex->skinInfos.push_back(skinInfo);
				}
			}

			pVertex->NormalizeSkins();
			pSubMesh->nMaxSkinWeightsPerVertex = max(pSubMesh->nMaxSkinWeightsPerVertex, (int)pVertex->skinInfos.size());

			// 4.2. 记录影响网格顶点的骨骼, 以及被骨骼影响的顶点列表
			//      注意: 在NormalizeSkins后最多只保留权重最大的4根骨骼, 因此必须
			//            在NormalizeSkins之后统计骨骼影响的网格顶点列表
			for (vector<CSkinInfo>::const_iterator itSkin = pVertex->skinInfos.begin(); itSkin != pVertex->skinInfos.end(); ++itSkin) {
				CBone *pBone = m_pBoneHierarchy->bones[itSkin->boneID];
				_ASSERT(pBone);

				pBone->SetUsed();

				vector<int>* pBoneVertices = NULL;
				map<CBone*,vector<int>*>::const_iterator itBone = pSubMesh->bones.find(pBone);

				if (itBone == pSubMesh->bones.end()) {
					pBoneVertices = new vector<int>;
					_ASSERT(pBoneVertices);

					pSubMesh->bones[pBone] = pBoneVertices;
				}
				else {
					pBoneVertices = itBone->second;
				}

				pBoneVertices->push_back(indexVertex);
			}
		}
	}

	return pSubMesh;
}

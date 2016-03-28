#include "MaxExporter.h"


//
// ��������
// ע��: ����ֻ����λ�ڸ��ڵ������, �����������������
//
void CModel::DoExportMesh(BOOL bInWorldSpcae)
{
	//
	// 1. ����λ�ڸ��ڵ����������
	//
	m_pSkinMesh = new CMesh;
	_ASSERT(m_pSkinMesh);

	//
	// 2. ������������
	//    ע��: �Ե�0���״̬��Ϊ��Ƥ����
	//
	m_pGameScene->SetStaticFrame(0);

	for (int indexNode = 0; indexNode < m_pGameScene->GetTopLevelNodeCount(); indexNode++) {
		IGameNode *pNode = m_pGameScene->GetTopLevelNode(indexNode);
		_ASSERT(pNode);

		DoExportMesh(pNode, m_pSkinMesh, bInWorldSpcae);
	}
}

//
// �������㶯��
//
void CModel::DoExportAnimVertex(void)
{
	//
	// 1. ����ÿ������
	//
	for (vector<CAnimVertex*>::const_iterator itAnim = m_animVertexs.begin(); itAnim != m_animVertexs.end(); ++itAnim) {
		CAnimVertex *pAnim = *itAnim;
		_ASSERT(pAnim);

		//
		// 2. ��������ÿһ��
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
// ��������
//
void CModel::DoExportMesh(IGameNode *pNode, CMesh *pMesh, BOOL bInWorldSpcae)
{
	_ASSERT(pNode);
	_ASSERT(pMesh);

	//
	// 1. �����ýڵ�����
	//
	if (CSubMesh *pSubMesh = DoExportSubMesh(pNode, bInWorldSpcae)) {
		pMesh->subMeshs.push_back(pSubMesh);
	}

	//
	// 2. �����ӽڵ�����
	//
	for (int indexChildNode = 0; indexChildNode < pNode->GetChildCount(); indexChildNode++) {
		IGameNode *pChildNode = pNode->GetNodeChild(indexChildNode);
		_ASSERT(pChildNode);

		DoExportMesh(pChildNode, pMesh, bInWorldSpcae);
	}
}

//
// ����������
//
CSubMesh* CModel::DoExportSubMesh(IGameNode *pNode, BOOL bInWorldSpcae)
{
	_ASSERT(pNode);

	//
	// 1. ���ڵ�����, ��������������
	//
	if (pNode->IsNodeHidden() ||
		pNode->GetIGameObject()->GetIGameType() != IGameObject::IGAME_MESH) {
		return NULL;
	}

	//
	// 2. ��ʼ��
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
	// 3. ������������
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
	// 3.1. ������������
	//
	for (int indexFace = 0; indexFace < pMesh->GetNumberOfFaces(); indexFace++) {
		FaceEx *pFaceEx = pMesh->GetFace(indexFace);
		_ASSERT(pFaceEx);

		CFace *pFace = new CFace(pFaceEx);
		_ASSERT(pFace);

		//
		// 3.1.1. ��������, �޳��ظ������ؽ�����������
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
						if (fabs(texCoord.z) < 0.001f) { // ע��: ���˵�����������!!!
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
		// 3.1.2. ���������
		//      ע��: ��.X�ļ���ʽ���������������ָ�Ĳ���������max�ļ��Ĳ���
		//            ����, ������������������ʹ�õĲ��ʼ��ϵ�����
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

			// �ڵ�ǰ������ʼ����в���������ʹ�õĲ���, ��������ʹ�õ����²���
			// ����Ӹò��ʵ�������ʼ���
			if ((pFace->indexMaterial = pSubMesh->QueryMaterialID(szMaterialName)) < 0) {
				pFace->indexMaterial = (int)pSubMesh->materials.size();
				pSubMesh->materials.push_back(QueryMaterial(szMaterialName));
			}
		}

		// 3.1.4. ���������
		pSubMesh->faces.push_back(pFace);
	}

	//
	// 4. ������������
	//
	if (IGameSkin* pSkin = (IGameSkin *)pMesh->GetIGameSkin()) {
		pSubMesh->isSkinMesh = TRUE;

		for (int indexVertex = 0; indexVertex < (int)pSubMesh->vertices.size(); indexVertex++) {
			CVertex *pVertex = pSubMesh->vertices[indexVertex];
			_ASSERT(pVertex);

			// 4.1. ��¼������Ƥ��Ϣ
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

			// 4.2. ��¼Ӱ�����񶥵�Ĺ���, �Լ�������Ӱ��Ķ����б�
			//      ע��: ��NormalizeSkins�����ֻ����Ȩ������4������, ��˱���
			//            ��NormalizeSkins֮��ͳ�ƹ���Ӱ������񶥵��б�
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

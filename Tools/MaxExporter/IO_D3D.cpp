#include "MaxExporter.h"


//
// 输出网格
//
void CModel::D3DX_WriteMesh(const char *szFileName, BOOL bOptimizeSkeleton) const
{
	FILE *pFile = fopen(szFileName, "wb");

	fprintf(pFile, "xof 0303txt 0032\n\n");

	//
	// 1. 材质
	//
	for (vector<CMaterial*>::const_iterator itMaterial = m_materials.begin(); itMaterial != m_materials.end(); ++itMaterial) {
		CMaterial *pMaterial = *itMaterial;
		_ASSERT(pMaterial);

		fprintf(pFile, "Material %s {\n", pMaterial->szName);
		{
			fprintf(pFile, "\t1.000000;1.000000;1.000000;1.000000;;\n");
			fprintf(pFile, "\t%6.6f;\n", pMaterial->specularLevel);
			fprintf(pFile, "\t%6.6f;%6.6f;%6.6f;;\n", pMaterial->specular.x, pMaterial->specular.y, pMaterial->specular.z);
			fprintf(pFile, "\t%6.6f;%6.6f;%6.6f;;\n", pMaterial->emissive.x, pMaterial->emissive.y, pMaterial->emissive.z);

			map<int,string>::const_iterator itTexMap = pMaterial->texUnits.find(ID_DI);

			if (itTexMap != pMaterial->texUnits.end()) {
				fprintf(pFile, "\n");
				fprintf(pFile, "\tTextureFilename {\n");
				fprintf(pFile, "\t\t\"%s\";\n", itTexMap->second.c_str());
				fprintf(pFile, "\t}\n");
			}
		}
		fprintf(pFile, "}\n\n");
	}

	//
	// 2. 骨骼贞
	//
	for (int indexBone = 0; indexBone < (int)m_pBoneHierarchy->bones.size(); indexBone++) {
		CBone *pBoneInit = m_pBoneHierarchy->bones[indexBone];
		_ASSERT(pBoneInit);

		if (pBoneInit->isRoot) {
			D3DX_WriteBoneFrame(pFile, pBoneInit, "", bOptimizeSkeleton);
		}
	}

	fprintf(pFile, "\n");

	//
	// 3. 网格贞
	//
	for (vector<CSubMesh*>::const_iterator itSubMesh = m_pSkinMesh->subMeshs.begin(); itSubMesh != m_pSkinMesh->subMeshs.end(); ++itSubMesh) {
		CSubMesh *pSubMesh = *itSubMesh;
		_ASSERT(pSubMesh);

		fprintf(pFile, "Frame %s {\n", pSubMesh->szName);
		{
			//
			// 3.1. 网格贞初始矩阵
			//
			fprintf(pFile, "\tFrameTransformMatrix {\n");
			{
				fprintf(pFile, "\t\t%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f;;\n",
					1.0f, 0.0f, 0.0f, 0.0f, 
					0.0f, 1.0f, 0.0f, 0.0f, 
					0.0f, 0.0f, 1.0f, 0.0f, 
					0.0f, 0.0f, 0.0f, 1.0f);
			}
			fprintf(pFile, "\t}\n\n");

			//
			// 3.2. 网格
			//
			fprintf(pFile, "\tMesh {\n");
			{
				//
				// 3.2.1. 顶点坐标集合
				//
				fprintf(pFile, "\t\t%d;\n", pSubMesh->vertices.size());
				for (vector<CVertex*>::const_iterator itVertex = pSubMesh->vertices.begin(); itVertex != pSubMesh->vertices.end(); ++itVertex) {
					CVertex *pVertex = *itVertex;
					_ASSERT(pVertex);

					if (itVertex != pSubMesh->vertices.begin()) fprintf(pFile, ",\n");
					fprintf(pFile, "\t\t%6.6f;%6.6f;%6.6f;", pVertex->position.x, pVertex->position.y, pVertex->position.z);
				}
				fprintf(pFile, ";\n");

				//
				// 3.2.2. 三角面集合
				//
				fprintf(pFile, "\t\t%d;\n", pSubMesh->faces.size());
				for (vector<CFace*>::const_iterator itFace = pSubMesh->faces.begin(); itFace != pSubMesh->faces.end(); ++itFace) {
					CFace *pFace = *itFace;
					_ASSERT(pFace);

					if (itFace != pSubMesh->faces.begin()) fprintf(pFile, ",\n");
					fprintf(pFile, "\t\t3;%d,%d,%d;", pFace->indexVertices[0], pFace->indexVertices[1], pFace->indexVertices[2]);
				}
				fprintf(pFile, ";\n\n");

				//
				// 3.2.3. 顶点法线集合
				//
				fprintf(pFile, "\t\tMeshNormals {\n");
				{
					fprintf(pFile, "\t\t\t%d;\n", pSubMesh->vertices.size());
					for (vector<CVertex*>::const_iterator itVertex = pSubMesh->vertices.begin(); itVertex != pSubMesh->vertices.end(); ++itVertex) {
						CVertex *pVertex = *itVertex;
						_ASSERT(pVertex);

						if (itVertex != pSubMesh->vertices.begin()) fprintf(pFile, ",\n");
						fprintf(pFile, "\t\t\t%6.6f;%6.6f;%6.6f;", pVertex->normal.x, pVertex->normal.y, pVertex->normal.z);
					}
					fprintf(pFile, ";\n");

					fprintf(pFile, "\t\t\t%d;\n", pSubMesh->faces.size());
					for (vector<CFace*>::const_iterator itFace = pSubMesh->faces.begin(); itFace != pSubMesh->faces.end(); ++itFace) {
						CFace *pFace = *itFace;
						_ASSERT(pFace);

						if (itFace != pSubMesh->faces.begin()) fprintf(pFile, ",\n");
						fprintf(pFile, "\t\t\t3;%d,%d,%d;", pFace->indexVertices[0], pFace->indexVertices[1], pFace->indexVertices[2]);
					}
					fprintf(pFile, ";\n");
				}
				fprintf(pFile, "\t\t}\n\n");

				//
				// 3.2.4. 顶点纹理坐标集合
				//
				if (pSubMesh->nNumTexChannel) {
					fprintf(pFile, "\t\tMeshTextureCoords {\n");
					{
						fprintf(pFile, "\t\t\t%d;\n", pSubMesh->vertices.size());
						for (vector<CVertex*>::const_iterator itVertex = pSubMesh->vertices.begin(); itVertex != pSubMesh->vertices.end(); ++itVertex) {
							CVertex *pVertex = *itVertex;
							_ASSERT(pVertex);

							if (itVertex != pSubMesh->vertices.begin()) fprintf(pFile, ",\n");
							fprintf(pFile, "\t\t\t%6.6f;%6.6f;", pVertex->texCoords[0].x, 1.0f - pVertex->texCoords[0].y);
						}
						fprintf(pFile, ";\n");
					}
					fprintf(pFile, "\t\t}\n\n");
				}

				//
				// 3.2.5. 三角面材质列表
				//
				fprintf(pFile, "\t\tMeshMaterialList {\n");
				{
					fprintf(pFile, "\t\t\t%d;\n", pSubMesh->materials.size());
					fprintf(pFile, "\t\t\t%d;\n", pSubMesh->faces.size());

					for (vector<CFace*>::const_iterator itFace = pSubMesh->faces.begin(); itFace != pSubMesh->faces.end(); ++itFace) {
						CFace *pFace = *itFace;
						_ASSERT(pFace);

						if (itFace != pSubMesh->faces.begin()) fprintf(pFile, ",\n");
						fprintf(pFile, "\t\t\t%d", pFace->indexMaterial);
					}
					fprintf(pFile, ";\n\n");

					for (vector<CMaterial*>::const_iterator itMaterial = pSubMesh->materials.begin(); itMaterial != pSubMesh->materials.end(); ++itMaterial) {
						CMaterial *pMaterial = *itMaterial;
						_ASSERT(pMaterial);

						fprintf(pFile, "\t\t\t{ %s }\n", pMaterial->szName);
					}
				}
				fprintf(pFile, "\t\t}\n\n");

				//
				// 3.2.6. 蒙皮
				//
				if (!pSubMesh->bones.empty()) {
					fprintf(pFile, "\t\tXSkinMeshHeader {\n");
					{
						fprintf(pFile, "\t\t\t%d;\n", pSubMesh->nMaxSkinWeightsPerVertex);
						fprintf(pFile, "\t\t\t%d;\n", pSubMesh->nMaxSkinWeightsPerFace);
						fprintf(pFile, "\t\t\t%d;\n", pSubMesh->bones.size());
					}
					fprintf(pFile, "\t\t}\n\n");

					for (map<CBone*,vector<int>*>::const_iterator itBone = pSubMesh->bones.begin(); itBone != pSubMesh->bones.end(); ++itBone) {
						CBone *pBone = itBone->first;
						vector<int>* pBoneVertices = itBone->second;

						_ASSERT(pBone);
						_ASSERT(pBoneVertices);

						fprintf(pFile, "\t\tSkinWeights {\n");
						{
							fprintf(pFile, "\t\t\t\"%s\";\n", pBone->szName);
							fprintf(pFile, "\t\t\t%d;\n", pBoneVertices->size());

							for (vector<int>::const_iterator itVertexID = pBoneVertices->begin(); itVertexID != pBoneVertices->end(); ++itVertexID) {
								if (itVertexID != pBoneVertices->begin()) fprintf(pFile, ",\n");
								fprintf(pFile, "\t\t\t%d", *itVertexID);
							}
							fprintf(pFile, ";\n");

							for (vector<int>::const_iterator itVertexID = pBoneVertices->begin(); itVertexID != pBoneVertices->end(); ++itVertexID) {
								CVertex *pVertex = pSubMesh->vertices[*itVertexID];
								_ASSERT(pVertex);

								for (vector<CSkinInfo>::const_iterator itSkin = pVertex->skinInfos.begin(); itSkin != pVertex->skinInfos.end(); ++itSkin) {
									if (m_pBoneHierarchy->bones[itSkin->boneID] == pBone) {
										if (itVertexID != pBoneVertices->begin()) fprintf(pFile, ",\n");
										fprintf(pFile, "\t\t\t%6.6f", itSkin->weight);
										break;
									}
								}
							}
							fprintf(pFile, ";\n");

							GMatrix matrixOffset = pBone->matWorld.Inverse();
							Point4 row0 = matrixOffset.GetRow(0);
							Point4 row1 = matrixOffset.GetRow(1);
							Point4 row2 = matrixOffset.GetRow(2);
							Point4 row3 = matrixOffset.GetRow(3);

							fprintf(pFile, "\t\t\t%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f;;\n",
								row0.x, row0.y, row0.z, row0.w, 
								row1.x, row1.y, row1.z, row1.w, 
								row2.x, row2.y, row2.z, row2.w, 
								row3.x, row3.y, row3.z, row3.w);
						}
						fprintf(pFile, "\t\t}\n\n");
					}
				}
			}
			fprintf(pFile, "\t}\n");
		}
		fprintf(pFile, "}\n\n");
	}

	fclose(pFile);
}

//
// 输出骨骼动画
//
void CModel::D3DX_WriteAnimSkeleton(const char *szFileName, BOOL bOptimizeSkeleton) const
{
	FILE *pFile = fopen(szFileName, "a+");

	for (vector<CAnimSkeleton*>::const_iterator itAnim = m_animSkeletons.begin(); itAnim != m_animSkeletons.end(); ++itAnim) {
		CAnimSkeleton *pAnim = *itAnim;
		_ASSERT(pAnim);

		int step = m_pGameScene->GetSceneTicks()*pAnim->pAnimInfo->fps/30;

		fprintf(pFile, "AnimationSet %s {\n", pAnim->pAnimInfo->szName);
		{
			for (int indexBone = 0; indexBone < (int)m_pBoneHierarchy->bones.size(); indexBone++) {
				CBone *pBoneInit = m_pBoneHierarchy->bones[indexBone];
				_ASSERT(pBoneInit);

				if (bOptimizeSkeleton && !pBoneInit->isUsed) {
					continue;;
				}

				fprintf(pFile, "\tAnimation Anim-%s {\n", pBoneInit->szName);
				{
					fprintf(pFile, "\t\t{ %s }\n", pBoneInit->szName);
					fprintf(pFile, "\t\tAnimationKey {\n");
					{
						fprintf(pFile, "\t\t\t4;\n");
						fprintf(pFile, "\t\t\t%d;\n", pAnim->frames.size());

						for (int indexFrame = 0; indexFrame < (int)pAnim->frames.size(); indexFrame++) {
							CBone *pBone = pAnim->frames[indexFrame]->bones[indexBone];
							_ASSERT(pBone);

							Point4 row0 = pBone->matLocal.GetRow(0);
							Point4 row1 = pBone->matLocal.GetRow(1);
							Point4 row2 = pBone->matLocal.GetRow(2);
							Point4 row3 = pBone->matLocal.GetRow(3);

							if (indexFrame != 0) fprintf(pFile, ",\n");
							fprintf(pFile, "\t\t\t%d;16;%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f;;", indexFrame*step,
								row0.x, row0.y, row0.z, row0.w, 
								row1.x, row1.y, row1.z, row1.w, 
								row2.x, row2.y, row2.z, row2.w, 
								row3.x, row3.y, row3.z, row3.w);
						}

						fprintf(pFile, ";\n");
					}
					fprintf(pFile, "\t\t}\n");
				}
				fprintf(pFile, "\t}\n");
			}
		}
		fprintf(pFile, "}\n\n");
	}

	fclose(pFile);
}

//
// 输出骨骼贞
//
void CModel::D3DX_WriteBoneFrame(FILE *pFile, CBone *pBone, char *szIndent, BOOL bOptimizeSkeleton) const
{
	//
	// 1. 优化骨骼
	//
	if (bOptimizeSkeleton && !pBone->isUsed) {
		return;
	}

	//
	// 2. 输出骨骼贞信息
	//
	fprintf(pFile, "%sFrame %s {\n", szIndent, pBone->szName);
	{
		fprintf(pFile, "%s\tFrameTransformMatrix {\n", szIndent);
		{
			Point4 row0 = pBone->matLocal.GetRow(0);
			Point4 row1 = pBone->matLocal.GetRow(1);
			Point4 row2 = pBone->matLocal.GetRow(2);
			Point4 row3 = pBone->matLocal.GetRow(3);

			fprintf(pFile, "%s\t\t%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f,%6.6f;;\n",
				szIndent,
				row0.x, row0.y, row0.z, row0.w, 
				row1.x, row1.y, row1.z, row1.w, 
				row2.x, row2.y, row2.z, row2.w, 
				row3.x, row3.y, row3.z, row3.w);
		}
		fprintf(pFile, "%s\t}\n", szIndent);
	}

	//
	// 3. 输出子骨骼
	//
	char szChildIndent[_MAX_PATH];
	sprintf(szChildIndent, "%s\t", szIndent);

	for (int indexChildBone = 0; indexChildBone < (int)pBone->childBones.size(); indexChildBone++) {
		CBone *pChildBone = pBone->childBones[indexChildBone];
		_ASSERT(pChildBone);

		D3DX_WriteBoneFrame(pFile, pChildBone, szChildIndent, bOptimizeSkeleton);
	}

	fprintf(pFile, "%s}\n", szIndent);
}

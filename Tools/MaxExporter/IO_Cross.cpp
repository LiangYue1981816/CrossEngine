#include "MaxExporter.h"


//
// 输出材质
//
void Cross_WriteMaterialSolution(FILE *pFile, const char *szSolution, CMaterial *pMaterial, int nRenderID, BOOL bGUPSkinning, SOLUTION_PARAMS params)
{
	BOOL bSimple = TRUE;
	char szVertexShaderName[_MAX_PATH] = {0};
	char szFragmentShaderName[_MAX_PATH] = {0};

	if (bGUPSkinning) {
		bSimple = FALSE;

		strcat(szVertexShaderName, "Skinning");
		strcat(szFragmentShaderName, "Skinning");
	}

	if (params.lighting != LIGHTING_NONE) {
		bSimple = FALSE;

		strcat(szVertexShaderName, "Lighting");
		strcat(szFragmentShaderName, "Lighting");
	}

	if (params.bEnvmap) {
		bSimple = FALSE;

		strcat(szVertexShaderName, "EnvMap");
		strcat(szFragmentShaderName, "EnvMap");
	}

	if (params.bLightmap && !bGUPSkinning) {
		bSimple = FALSE;

		strcat(szVertexShaderName, "LightMap");
		strcat(szFragmentShaderName, "LightMap");
	}

	if (params.lighting == LIGHTING_ADVANCED) {
		bSimple = FALSE;

		strcat(szVertexShaderName, "NormalMap");
		strcat(szFragmentShaderName, "NormalMap");

		strcat(szVertexShaderName, "SpecularMap");
		strcat(szFragmentShaderName, "SpecularMap");
	}

	if (params.bShadowmap) {
		bSimple = FALSE;

		strcat(szVertexShaderName, "ShadowMap");
		strcat(szFragmentShaderName, "ShadowMap");
	}

	if (bSimple) {
		strcat(szVertexShaderName, "Diffuse");
		strcat(szFragmentShaderName, "Diffuse");
	}

	strcat(szVertexShaderName, ".Vertex.shader");
	strcat(szFragmentShaderName, ".Fragment.shader");

	fprintf(pFile, "\t<Solution name=\"%s\">\n", szSolution);
	{
		fprintf(pFile, "\t\t<PassGroup name=\"FrameBuffer\">\n");
		{
			fprintf(pFile, "\t\t\t<Pass>\n");
			{
				fprintf(pFile, "\t\t\t\t<RenderID id=\"%d\" />\n", nRenderID);

				fprintf(pFile, "\t\t\t\t<ShaderVertex name=\"%s\" />\n", szVertexShaderName);
				fprintf(pFile, "\t\t\t\t<ShaderFragment name=\"%s\" />\n", szFragmentShaderName);

				if (pMaterial->isTwoSided) {
					fprintf(pFile, "\t\t\t\t<Cull enable=\"GL_FALSE\" />\n");
				}

				if (params.lighting != LIGHTING_NONE) {
					fprintf(pFile, "\t\t\t\t<Lighting enable=\"GL_TRUE\" />\n");
				}

				if (pMaterial->texUnits.find(ID_DI) != pMaterial->texUnits.end()) {
					fprintf(pFile, "\t\t\t\t<Texture channel=\"0\" name=\"%s\" minfilter=\"GL_LINEAR_MIPMAP_LINEAR\" magfilter=\"GL_LINEAR\" wraps=\"GL_REPEAT\" wrapt=\"GL_REPEAT\" />\n", pMaterial->texUnits[ID_DI].c_str());
				}

				if (pMaterial->texUnits.find(ID_BU) != pMaterial->texUnits.end() && params.lighting == LIGHTING_ADVANCED) {
					fprintf(pFile, "\t\t\t\t<Texture channel=\"2\" name=\"%s\" minfilter=\"GL_LINEAR_MIPMAP_LINEAR\" magfilter=\"GL_LINEAR\" wraps=\"GL_REPEAT\" wrapt=\"GL_REPEAT\" />\n", pMaterial->texUnits[ID_BU].c_str());
				}

				if (pMaterial->texUnits.find(ID_SP) != pMaterial->texUnits.end() && params.lighting == LIGHTING_ADVANCED) {
					fprintf(pFile, "\t\t\t\t<Texture channel=\"3\" name=\"%s\" minfilter=\"GL_LINEAR_MIPMAP_LINEAR\" magfilter=\"GL_LINEAR\" wraps=\"GL_REPEAT\" wrapt=\"GL_REPEAT\" />\n", pMaterial->texUnits[ID_SP].c_str());
				}
			}
			fprintf(pFile, "\t\t\t</Pass>\n");
		}
		fprintf(pFile, "\t\t</PassGroup>\n");
	}
	fprintf(pFile, "\t</Solution>\n");
}

void CModel::Cross_WriteMaterial(const char *szName, const char *szPathName, int nRenderID, BOOL bGUPSkinning, SOLUTION_PARAMS low, SOLUTION_PARAMS medium, SOLUTION_PARAMS high, SOLUTION_PARAMS ultra, SOLUTION_PARAMS extreme) const
{
	for (vector<CMaterial*>::const_iterator itMaterial = m_materials.begin(); itMaterial != m_materials.end(); ++itMaterial) {
		CMaterial *pMaterial = *itMaterial;
		_ASSERT(pMaterial);

		if (!pMaterial->isValid) {
			continue;
		}

		char szFileName[_MAX_PATH];
		sprintf(szFileName, "%s%s.%s.material", szPathName, szName, pMaterial->szName);

		FILE *pFile = fopen(szFileName, "wb");
		if (!pFile) continue;

		fprintf(pFile, "<Material>\n");
		{
			Cross_WriteMaterialSolution(pFile, "Low", pMaterial, nRenderID, bGUPSkinning, low);
			Cross_WriteMaterialSolution(pFile, "Medium", pMaterial, nRenderID, bGUPSkinning, medium);
			Cross_WriteMaterialSolution(pFile, "High", pMaterial, nRenderID, bGUPSkinning, high);
			Cross_WriteMaterialSolution(pFile, "Ultra", pMaterial, nRenderID, bGUPSkinning, ultra);
			Cross_WriteMaterialSolution(pFile, "Extreme", pMaterial, nRenderID, bGUPSkinning, extreme);
		}
		fprintf(pFile, "</Material>");

		fclose(pFile);
	}
}

//
// 输出网格
//
void CModel::Cross_WriteMesh(const char *szName, const char *szFileName, const char *szSkeletonAnimName, const vector<CSubMesh*> &subMeshs)
{
	FILE *pFile = fopen(szFileName, "wb");

	//
	// 1. 写入包围盒
	//
	Box3 aabb(Point3( FLT_MAX, FLT_MAX, FLT_MAX), Point3(-FLT_MAX, -FLT_MAX, -FLT_MAX));

	for (vector<CSubMesh*>::const_iterator itSubMesh = subMeshs.begin(); itSubMesh != subMeshs.end(); ++itSubMesh) {
		CSubMesh *pSubMesh = *itSubMesh;
		_ASSERT(pSubMesh);

		for (vector<CVertex*>::const_iterator itVertex = pSubMesh->vertices.begin(); itVertex != pSubMesh->vertices.end(); ++itVertex) {
			CVertex *pVertex = *itVertex;
			_ASSERT(pVertex);

			if (aabb.pmin.x > pVertex->position.x) aabb.pmin.x = pVertex->position.x;
			if (aabb.pmin.y > pVertex->position.y) aabb.pmin.y = pVertex->position.y;
			if (aabb.pmin.z > pVertex->position.z) aabb.pmin.z = pVertex->position.z;
			if (aabb.pmax.x < pVertex->position.x) aabb.pmax.x = pVertex->position.x;
			if (aabb.pmax.y < pVertex->position.y) aabb.pmax.y = pVertex->position.y;
			if (aabb.pmax.z < pVertex->position.z) aabb.pmax.z = pVertex->position.z;
		}
	}

	Point3 center = aabb.Center();
	fwrite(&center.x, sizeof(center.x), 1, pFile);
	fwrite(&center.y, sizeof(center.y), 1, pFile);
	fwrite(&center.z, sizeof(center.z), 1, pFile);
	fwrite(&aabb.pmin.x, sizeof(aabb.pmin.x), 1, pFile);
	fwrite(&aabb.pmin.y, sizeof(aabb.pmin.y), 1, pFile);
	fwrite(&aabb.pmin.z, sizeof(aabb.pmin.z), 1, pFile);
	fwrite(&aabb.pmax.x, sizeof(aabb.pmax.x), 1, pFile);
	fwrite(&aabb.pmax.y, sizeof(aabb.pmax.y), 1, pFile);
	fwrite(&aabb.pmax.z, sizeof(aabb.pmax.z), 1, pFile);

	//
	// 2. 写入子网格
	//
	int numSubMesh = (int)subMeshs.size();
	fwrite(&numSubMesh, sizeof(numSubMesh), 1, pFile);

	for (vector<CSubMesh*>::const_iterator itSubMesh = subMeshs.begin(); itSubMesh != subMeshs.end(); ++itSubMesh) {
		CSubMesh *pSubMesh = *itSubMesh;
		_ASSERT(pSubMesh);

		// 2.1. 写入子网格名
		DWORD dwSubMeshName = HashValue(pSubMesh->szName);
		fwrite(&dwSubMeshName, sizeof(dwSubMeshName), 1, pFile);

		int len = (int)strlen(pSubMesh->szName) + 1;
		int alignLen = ALIGN_4BYTE(len);
		fwrite(&alignLen, sizeof(alignLen), 1, pFile);
		fwrite(pSubMesh->szName, sizeof(*pSubMesh->szName), alignLen, pFile);

		// 2.2. 写入材质和光照贴图名
		char szMaterialName[_MAX_PATH];
		sprintf(szMaterialName, "%s.%s.material", szName, pSubMesh->szMaterialName);

		DWORD dwMaterialName = HashValue(szMaterialName);
		DWORD dwLightMapName = HashValue(pSubMesh->szLightMapName);

		fwrite(&dwMaterialName, sizeof(dwMaterialName), 1, pFile);
		fwrite(&dwLightMapName, sizeof(dwLightMapName), 1, pFile);

		// 2.3. 写入蒙皮网格标志
		fwrite(&pSubMesh->isSkinMesh, sizeof(pSubMesh->isSkinMesh), 1, pFile);

		// 2.4. 写入顶点数据
		int numVertices = (int)pSubMesh->vertices.size();
		fwrite(&numVertices, sizeof(numVertices), 1, pFile);

		for (vector<CVertex*>::const_iterator itVertex = pSubMesh->vertices.begin(); itVertex != pSubMesh->vertices.end(); ++itVertex) {
			CVertex *pVertex = *itVertex;
			_ASSERT(pVertex);

			float indices[4] = {0.0f, 0.0f, 0.0f, 0.0f};
			float weights[4] = {0.0f, 0.0f, 0.0f, 0.0f};

			for (int index = 0; index < (int)pVertex->skinInfos.size(); index++) {
				indices[index] = 1.0f*pVertex->skinInfos[index].boneID;
				weights[index] = 1.0f*pVertex->skinInfos[index].weight;
			}

			// 2.4.1. 位置
			fwrite(&pVertex->position.x, sizeof(pVertex->position.x), 1, pFile);
			fwrite(&pVertex->position.y, sizeof(pVertex->position.y), 1, pFile);
			fwrite(&pVertex->position.z, sizeof(pVertex->position.z), 1, pFile);

			// 2.4.2. 法线
			fwrite(&pVertex->normal.x, sizeof(pVertex->normal.x), 1, pFile);
			fwrite(&pVertex->normal.y, sizeof(pVertex->normal.y), 1, pFile);
			fwrite(&pVertex->normal.z, sizeof(pVertex->normal.z), 1, pFile);

			// 2.4.3. 副法线
			fwrite(&pVertex->binormal.x, sizeof(pVertex->binormal.x), 1, pFile);
			fwrite(&pVertex->binormal.y, sizeof(pVertex->binormal.y), 1, pFile);
			fwrite(&pVertex->binormal.z, sizeof(pVertex->binormal.z), 1, pFile);

			// 2.4.4. 颜色
			fwrite(&pVertex->color.x, sizeof(pVertex->color.x), 1, pFile);
			fwrite(&pVertex->color.y, sizeof(pVertex->color.y), 1, pFile);
			fwrite(&pVertex->color.z, sizeof(pVertex->color.z), 1, pFile);
			fwrite(&pVertex->color.w, sizeof(pVertex->color.w), 1, pFile);

			// 2.4.5. 纹理坐标
			if (pVertex->texCoords.size() > 0) {
				if (pVertex->texCoords.size() == 1) {
					pVertex->texCoords[0].y = 1.0f - pVertex->texCoords[0].y;

					fwrite(&pVertex->texCoords[0].x, sizeof(pVertex->texCoords[0].x), 1, pFile);
					fwrite(&pVertex->texCoords[0].y, sizeof(pVertex->texCoords[0].y), 1, pFile);

					fwrite(&pVertex->texCoords[0].x, sizeof(pVertex->texCoords[0].x), 1, pFile);
					fwrite(&pVertex->texCoords[0].y, sizeof(pVertex->texCoords[0].y), 1, pFile);
				}
				else {
					pVertex->texCoords[0].y = 1.0f - pVertex->texCoords[0].y;
					pVertex->texCoords[1].y = 1.0f - pVertex->texCoords[1].y;

					fwrite(&pVertex->texCoords[0].x, sizeof(pVertex->texCoords[0].x), 1, pFile);
					fwrite(&pVertex->texCoords[0].y, sizeof(pVertex->texCoords[0].y), 1, pFile);

					fwrite(&pVertex->texCoords[1].x, sizeof(pVertex->texCoords[1].x), 1, pFile);
					fwrite(&pVertex->texCoords[1].y, sizeof(pVertex->texCoords[1].y), 1, pFile);
				}
			}
			// 2.4.6. 纹理坐标占位
			else {
				float defaultS = 0.0f;
				float defaultT = 0.0f;

				fwrite(&defaultS, sizeof(defaultS), 1, pFile);
				fwrite(&defaultT, sizeof(defaultT), 1, pFile);

				fwrite(&defaultS, sizeof(defaultS), 1, pFile);
				fwrite(&defaultT, sizeof(defaultT), 1, pFile);
			}

			// 2.4.7. 蒙皮
			fwrite(indices, sizeof(indices), 1, pFile);
			fwrite(weights, sizeof(weights), 1, pFile);
		}

		// 2.4. 写入三角面数据
		int numFaces = (int)pSubMesh->faces.size();
		fwrite(&numFaces, sizeof(numFaces), 1, pFile);

		for (vector<CFace*>::const_iterator itFace = pSubMesh->faces.begin(); itFace != pSubMesh->faces.end(); ++itFace) {
			CFace *pFace = *itFace;
			_ASSERT(pFace);

			DWORD index0 = pFace->indexVertices[0];
			DWORD index1 = pFace->indexVertices[1];
			DWORD index2 = pFace->indexVertices[2];

			fwrite(&index0, sizeof(index0), 1, pFile);
			fwrite(&index1, sizeof(index1), 1, pFile);
			fwrite(&index2, sizeof(index2), 1, pFile);
		}
	}

	//
	// 3. 写入骨骼文件名
	//
	DWORD dwSkeletonName = m_pBoneHierarchy->GetBoneCount(FALSE) > 0 ? HashValue(szSkeletonAnimName) : 0xffffffff;
	fwrite(&dwSkeletonName, sizeof(dwSkeletonName), 1, pFile);

	fclose(pFile);
}

//
// 输出骨骼动画
//
void CModel::Cross_WriteAnimSkeleton(const char *szFileName, BOOL bOptimizeSkeleton) const
{
	FILE *pFile = fopen(szFileName, "wb");

	//
	// 1. 写入骨架数据
	//
	// 1.1. 写入骨骼数
	int numBones = m_pBoneHierarchy->GetBoneCount(bOptimizeSkeleton);
	fwrite(&numBones, sizeof(numBones), 1, pFile);

	// 1.2. 骨骼父子关系
	for (int indexBone = 0; indexBone < (int)m_pBoneHierarchy->bones.size(); indexBone++) {
		CBone *pBoneInit = m_pBoneHierarchy->bones[indexBone];
		_ASSERT(pBoneInit);

		if (bOptimizeSkeleton && !pBoneInit->isUsed) {
			continue;;
		}

		int parentID = m_pBoneHierarchy->QueryBoneID(pBoneInit->szParentName);
		fwrite(&parentID, sizeof(parentID), 1, pFile);
	}

	// 1.3. 写入骨骼名
	for (int indexBone = 0; indexBone < (int)m_pBoneHierarchy->bones.size(); indexBone++) {
		CBone *pBoneInit = m_pBoneHierarchy->bones[indexBone];
		_ASSERT(pBoneInit);

		if (bOptimizeSkeleton && !pBoneInit->isUsed) {
			continue;;
		}

		DWORD dwBoneName = HashValue(pBoneInit->szName);

		fwrite(pBoneInit->szName, sizeof(pBoneInit->szName), 1, pFile);
		fwrite(&dwBoneName, sizeof(dwBoneName), 1, pFile);
	}

	// 1.4. 初始骨骼
	for (int indexBone = 0; indexBone < (int)m_pBoneHierarchy->bones.size(); indexBone++) {
		CBone *pBoneInit = m_pBoneHierarchy->bones[indexBone];
		_ASSERT(pBoneInit);

		if (bOptimizeSkeleton && !pBoneInit->isUsed) {
			continue;;
		}

		BOOL bUpdated = FALSE;

		GMatrix matrix = pBoneInit->matWorld.Inverse();
		Point3 scale = matrix.Scaling();
		Point3 position = matrix.Translation();
		Quat orientation = matrix.Rotation().Conjugate();

		fwrite(&bUpdated, sizeof(bUpdated), 1, pFile);

		fwrite(&scale.x, sizeof(scale.x), 1, pFile);
		fwrite(&scale.y, sizeof(scale.y), 1, pFile);
		fwrite(&scale.z, sizeof(scale.z), 1, pFile);

		fwrite(&position.x, sizeof(position.x), 1, pFile);
		fwrite(&position.y, sizeof(position.y), 1, pFile);
		fwrite(&position.z, sizeof(position.z), 1, pFile);

		fwrite(&orientation.x, sizeof(orientation.x), 1, pFile);
		fwrite(&orientation.y, sizeof(orientation.y), 1, pFile);
		fwrite(&orientation.z, sizeof(orientation.z), 1, pFile);
		fwrite(&orientation.w, sizeof(orientation.w), 1, pFile);
	}

	for (int indexBone = 0; indexBone < (int)m_pBoneHierarchy->bones.size(); indexBone++) {
		CBone *pBoneInit = m_pBoneHierarchy->bones[indexBone];
		_ASSERT(pBoneInit);

		if (bOptimizeSkeleton && !pBoneInit->isUsed) {
			continue;;
		}

		BOOL bUpdated = FALSE;

		GMatrix matrix = pBoneInit->matLocal;
		Point3 scale = matrix.Scaling();
		Point3 position = matrix.Translation();
		Quat orientation = matrix.Rotation().Conjugate();

		fwrite(&bUpdated, sizeof(bUpdated), 1, pFile);

		fwrite(&scale.x, sizeof(scale.x), 1, pFile);
		fwrite(&scale.y, sizeof(scale.y), 1, pFile);
		fwrite(&scale.z, sizeof(scale.z), 1, pFile);

		fwrite(&position.x, sizeof(position.x), 1, pFile);
		fwrite(&position.y, sizeof(position.y), 1, pFile);
		fwrite(&position.z, sizeof(position.z), 1, pFile);

		fwrite(&orientation.x, sizeof(orientation.x), 1, pFile);
		fwrite(&orientation.y, sizeof(orientation.y), 1, pFile);
		fwrite(&orientation.z, sizeof(orientation.z), 1, pFile);
		fwrite(&orientation.w, sizeof(orientation.w), 1, pFile);
	}

	//
	// 2. 写入骨骼动画数据
	//
	// 2.1. 写入动画数
	int numAnimations = (int)m_animSkeletons.size();
	fwrite(&numAnimations, sizeof(numAnimations), 1, pFile);

	// 2.2. 写入动画
	for (vector<CAnimSkeleton*>::const_iterator itAnim = m_animSkeletons.begin(); itAnim != m_animSkeletons.end(); ++itAnim) {
		CAnimSkeleton *pAnimSkeleton = *itAnim;
		_ASSERT(pAnimSkeleton);

		// 2.2.1. 写入动画信息
		DWORD dwAnimHashName = HashValue(pAnimSkeleton->pAnimInfo->szName);
		int len = (int)strlen(pAnimSkeleton->pAnimInfo->szName) + 1;
		int alignLen = ALIGN_4BYTE(len);
		int numKeyFrames = (int)pAnimSkeleton->frames.size();
		float timeLength = 1.0f*(pAnimSkeleton->pAnimInfo->endFrame - pAnimSkeleton->pAnimInfo->startFrame + 1)/pAnimSkeleton->pAnimInfo->fps;

		fwrite(&alignLen, sizeof(alignLen), 1, pFile);
		fwrite(pAnimSkeleton->pAnimInfo->szName, sizeof(*pAnimSkeleton->pAnimInfo->szName), alignLen, pFile);
		fwrite(&dwAnimHashName, sizeof(dwAnimHashName), 1, pFile);
		fwrite(&timeLength, sizeof(timeLength), 1, pFile);
		fwrite(&numKeyFrames, sizeof(numKeyFrames), 1, pFile);

		// 2.2.2. 写入动画关键帧时间数据
		for (vector<CBoneFrame*>::const_iterator itFrame = pAnimSkeleton->frames.begin(); itFrame != pAnimSkeleton->frames.end(); ++itFrame) {
			CBoneFrame *pFrame = *itFrame;
			_ASSERT(pFrame);

			fwrite(&pFrame->tick, sizeof(pFrame->tick), 1, pFile);
		}

		// 2.2.3. 写入动画关键帧骨骼数据
		BYTE headerData = 0xcc;
		int headerSize = numKeyFrames*sizeof(UINT64);

		for (int index = 0; index < headerSize; index++) {
			fwrite(&headerData, sizeof(headerData), 1, pFile);
		}

		for (vector<CBoneFrame*>::const_iterator itFrame = pAnimSkeleton->frames.begin(); itFrame != pAnimSkeleton->frames.end(); ++itFrame) {
			CBoneFrame *pFrame = *itFrame;
			_ASSERT(pFrame);

			for (int indexBone = 0; indexBone < (int)m_pBoneHierarchy->bones.size(); indexBone++) {
				CBone *pBoneAnim = pFrame->bones[indexBone];
				CBone *pBoneInit = m_pBoneHierarchy->bones[indexBone];
				_ASSERT(pBoneAnim);
				_ASSERT(pBoneInit);

				if (bOptimizeSkeleton && !pBoneInit->isUsed) {
					continue;;
				}

				BOOL bUpdated = FALSE;

				GMatrix matrix = pBoneAnim->matLocal*pBoneInit->matLocal.Inverse();
				Point3 scale = matrix.Scaling();
				Point3 position = matrix.Translation();
				Quat orientation = matrix.Rotation().Conjugate();

				fwrite(&bUpdated, sizeof(bUpdated), 1, pFile);

				fwrite(&scale.x, sizeof(scale.x), 1, pFile);
				fwrite(&scale.y, sizeof(scale.y), 1, pFile);
				fwrite(&scale.z, sizeof(scale.z), 1, pFile);

				fwrite(&position.x, sizeof(position.x), 1, pFile);
				fwrite(&position.y, sizeof(position.y), 1, pFile);
				fwrite(&position.z, sizeof(position.z), 1, pFile);

				fwrite(&orientation.x, sizeof(orientation.x), 1, pFile);
				fwrite(&orientation.y, sizeof(orientation.y), 1, pFile);
				fwrite(&orientation.z, sizeof(orientation.z), 1, pFile);
				fwrite(&orientation.w, sizeof(orientation.w), 1, pFile);
			}
		}
	}

	fclose(pFile);
}

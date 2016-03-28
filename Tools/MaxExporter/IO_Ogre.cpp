#include "MaxExporter.h"


//
// 输出材质
//
void CModel::OgreXML_WriteMaterial(const char *szName, const char *szFileName) const
{
	FILE *pFile = fopen(szFileName, "wb");

	for (vector<CMaterial*>::const_iterator itMaterial = m_materials.begin(); itMaterial != m_materials.end(); ++itMaterial) {
		CMaterial *pMaterial = *itMaterial;
		_ASSERT(pMaterial);

		fprintf(pFile, "material %s_%s\n", szName, pMaterial->szName);
		fprintf(pFile, "{\n");
		{
			fprintf(pFile, "\ttechnique\n");
			fprintf(pFile, "\t{\n");
			{
				fprintf(pFile, "\t\tpass\n");
				fprintf(pFile, "\t\t{\n");
				{
					fprintf(pFile, "\t\t\tambient %6.6f %6.6f %6.6f\n", pMaterial->ambient[0], pMaterial->ambient[1], pMaterial->ambient[2]);
					fprintf(pFile, "\t\t\tdiffuse %6.6f %6.6f %6.6f\n", pMaterial->diffuse[0], pMaterial->diffuse[1], pMaterial->diffuse[2]);
					fprintf(pFile, "\t\t\temissive %6.6f %6.6f %6.6f\n", pMaterial->emissive[0], pMaterial->emissive[1], pMaterial->emissive[2]);
					fprintf(pFile, "\t\t\tspecular %6.6f %6.6f %6.6f %6.6f\n", pMaterial->specular[0], pMaterial->specular[1], pMaterial->specular[2], pMaterial->specularLevel*100);

					for (map<int,string>::const_iterator itTexUnit = pMaterial->texUnits.begin(); itTexUnit != pMaterial->texUnits.end(); ++itTexUnit) {
						fprintf(pFile, "\t\t\t\n");

						fprintf(pFile, "\t\t\ttexture_unit\n");
						fprintf(pFile, "\t\t\t{\n");
						{
							fprintf(pFile, "\t\t\t\ttexture %s\n", itTexUnit->second.c_str());
							if (itTexUnit != pMaterial->texUnits.begin()) {
								fprintf(pFile, "\t\t\t\tcolour_op modulate\n");
							}
						}
						fprintf(pFile, "\t\t\t}\n");
					}
				}
				fprintf(pFile, "\t\t}\n");
			}
			fprintf(pFile, "\t}\n");
		}
		fprintf(pFile, "}\n");
	}

	fclose(pFile);
}

//
// 输出网格
//
void CModel::OgreXML_WriteMesh(const char *szName, const char *szFileName, const char *szSkeletonAnimName) const
{
	FILE *pFile = fopen(szFileName, "wb");

	fprintf(pFile, "<mesh>\n");
	{
		fprintf(pFile, "\t<submeshes>\n");
		{
			for (vector<CSubMesh*>::const_iterator itSubMesh = m_pSkinMesh->subMeshs.begin(); itSubMesh != m_pSkinMesh->subMeshs.end(); ++itSubMesh) {
				CSubMesh *pSubMesh = *itSubMesh;
				_ASSERT(pSubMesh);

				if (!pSubMesh->faces.size() || !pSubMesh->vertices.size()) {
					continue;
				}

				fprintf(pFile, "\t\t<submesh material=\"%s_%s\" usesharedvertices=\"false\" use32bitindexes=\"false\" operationtype=\"triangle_list\">\n", szName, pSubMesh->szMaterialName);
				{
					fprintf(pFile, "\t\t\t<faces count=\"%d\">\n", pSubMesh->faces.size());
					{
						for (vector<CFace*>::const_iterator itFace = pSubMesh->faces.begin(); itFace != pSubMesh->faces.end(); ++itFace) {
							CFace *pFace = *itFace;
							_ASSERT(pFace);

							fprintf(pFile, "\t\t\t\t<face v1=\"%d\" v2=\"%d\" v3=\"%d\" />\n", pFace->indexVertices[0], pFace->indexVertices[1], pFace->indexVertices[2]);
						}
					}
					fprintf(pFile, "\t\t\t</faces>\n");

					fprintf(pFile, "\t\t\t<geometry vertexcount=\"%d\">\n", pSubMesh->vertices.size());
					fprintf(pFile, "\t\t\t\t<vertexbuffer positions=\"true\" normals=\"true\" colours_diffuse=\"true\" texture_coords=\"%d\" texture_coords_dimensions_0=\"2\">\n", pSubMesh->nNumTexChannel);
					{
						for (vector<CVertex*>::const_iterator itVertex = pSubMesh->vertices.begin(); itVertex != pSubMesh->vertices.end(); ++itVertex) {
							CVertex *pVertex = *itVertex;
							_ASSERT(pVertex);
							_ASSERT(pVertex->texCoords.size() == pSubMesh->nNumTexChannel);

							fprintf(pFile, "\t\t\t\t\t<vertex>\n");
							fprintf(pFile, "\t\t\t\t\t\t<position x=\"%6.6f\" y=\"%6.6f\" z=\"%6.6f\" />\n", pVertex->position.x, pVertex->position.y, pVertex->position.z);
							fprintf(pFile, "\t\t\t\t\t\t<colour_diffuse value=\"%6.6f %6.6f %6.6f\" />\n", pVertex->color.x, pVertex->color.y, pVertex->color.z);
							fprintf(pFile, "\t\t\t\t\t\t<normal x=\"%6.6f\" y=\"%6.6f\" z=\"%6.6f\" />\n", pVertex->normal.x, pVertex->normal.y, pVertex->normal.z);
							for (int indexTexCoord = 0; indexTexCoord < (int)pVertex->texCoords.size(); indexTexCoord++) {
								fprintf(pFile, "\t\t\t\t\t\t<texcoord u=\"%6.6f\" v=\"%6.6f\" />\n", pVertex->texCoords[indexTexCoord].x, 1.0f - pVertex->texCoords[indexTexCoord].y);
							}
							fprintf(pFile, "\t\t\t\t\t</vertex>\n");
						}
					}
					fprintf(pFile, "\t\t\t\t</vertexbuffer>\n");
					fprintf(pFile, "\t\t\t</geometry>\n");

					if (pSubMesh->isSkinMesh) {
						fprintf(pFile, "\t\t\t<boneassignments>\n");
						{
							for (int indexVertex = 0; indexVertex < (int)pSubMesh->vertices.size(); indexVertex++) {
								CVertex *pVertex = pSubMesh->vertices[indexVertex];
								_ASSERT(pVertex);

								for (vector<CSkinInfo>::const_iterator itSkinInfo = pVertex->skinInfos.begin(); itSkinInfo != pVertex->skinInfos.end(); ++itSkinInfo) {
									fprintf(pFile, "\t\t\t\t<vertexboneassignment vertexindex=\"%d\" boneindex=\"%d\" weight=\"%6.6f\" />\n", indexVertex, itSkinInfo->boneID, itSkinInfo->weight);
								}
							}
						}
						fprintf(pFile, "\t\t\t</boneassignments>\n");
					}
				}
				fprintf(pFile, "\t\t</submesh>\n");
			}
		}
		fprintf(pFile, "\t</submeshes>\n");

		fprintf(pFile, "\t<submeshnames>\n");
		{
			for (int indexSubMesh = 0; indexSubMesh < (int)m_pSkinMesh->subMeshs.size(); indexSubMesh++) {
				CSubMesh *pSubMesh = m_pSkinMesh->subMeshs[indexSubMesh];
				_ASSERT(pSubMesh);

				fprintf(pFile, "\t\t<submeshname name=\"%s\" index=\"%d\" />\n", pSubMesh->szName, indexSubMesh);
			}
		}
		fprintf(pFile, "\t</submeshnames>\n");

		if (!m_pBoneHierarchy->bones.empty() && m_animVertexs.empty()) {
			fprintf(pFile, "\t<skeletonlink name=\"%s\" />\n", szSkeletonAnimName);
		}

		if (!m_animVertexs.empty()) {
			fprintf(pFile, "\t<animations>\n");
			{
				for (vector<CAnimVertex*>::const_iterator itAnim = m_animVertexs.begin(); itAnim != m_animVertexs.end(); ++itAnim) {
					CAnimVertex *pAnim = *itAnim;
					_ASSERT(pAnim);

					float length = (pAnim->pAnimInfo->endFrame - pAnim->pAnimInfo->startFrame + 1.0f)/pAnim->pAnimInfo->fps;

					fprintf(pFile, "\t\t<animation name=\"%s\" length=\"%6.6f\">\n", pAnim->pAnimInfo->szName, length);
					fprintf(pFile, "\t\t\t<tracks>\n");
					{
						for (int indexSubMesh = 0; indexSubMesh < (int)m_pSkinMesh->subMeshs.size(); indexSubMesh++) {
							fprintf(pFile, "\t\t\t\t<track target=\"submesh\" index=\"%d\" type=\"morph\">\n", indexSubMesh);
							fprintf(pFile, "\t\t\t\t\t<keyframes>\n");
							{
								for (int indexFrame = 0; indexFrame < (int)pAnim->frames.size(); indexFrame++) {
									CSubMesh *pSubMesh = pAnim->frames[indexFrame]->subMeshs[indexSubMesh];
									_ASSERT(pSubMesh);

									float time = length*indexFrame/pAnim->frames.size();

									fprintf(pFile, "\t\t\t\t\t\t<keyframe time=\"%6.6f\">\n", time);
									{
										for (vector<CVertex*>::const_iterator itVertex = pSubMesh->vertices.begin(); itVertex != pSubMesh->vertices.end(); ++itVertex) {
											CVertex *pVertex = *itVertex;
											_ASSERT(pVertex);

											fprintf(pFile, "\t\t\t\t\t\t\t<position x=\"%6.6f\" y=\"%6.6f\" z=\"%6.6f\" />\n", pVertex->position.x, pVertex->position.y, pVertex->position.z);
										}
									}
									fprintf(pFile, "\t\t\t\t\t\t</keyframe>\n");
								}
							}
							fprintf(pFile, "\t\t\t\t\t</keyframes>\n");
							fprintf(pFile, "\t\t\t\t</track>\n");
						}
					}
					fprintf(pFile, "\t\t\t</tracks>\n");
					fprintf(pFile, "\t\t</animation>\n");
				}
			}
			fprintf(pFile, "\t</animations>\n");
		}
	}
	fprintf(pFile, "</mesh>");

	fclose(pFile);
}

//
// 输出骨骼动画
//
void CModel::OgreXML_WriteAnimSkeleton(const char *szFileName, BOOL bOptimizeSkeleton) const
{
	FILE *pFile = fopen(szFileName, "wb");

	fprintf(pFile, "<skeleton>\n");
	{
		fprintf(pFile, "\t<bones>\n");
		{
			for (int indexBone = 0; indexBone < (int)m_pBoneHierarchy->bones.size(); indexBone++) {
				CBone *pBoneInit = m_pBoneHierarchy->bones[indexBone];
				_ASSERT(pBoneInit);

				if (bOptimizeSkeleton && !pBoneInit->isUsed) {
					continue;
				}

				fprintf(pFile, "\t\t<bone id=\"%d\" name=\"%s\">\n", pBoneInit->id, pBoneInit->szName);
				{
					Point3 position = pBoneInit->matLocal.Translation();
					AngAxis aa(pBoneInit->matLocal.ExtractMatrix3());

					if (aa.axis.Length() < 0.001f || fabs(aa.angle) < 0.001f) {
						aa.axis = Point3(0.0f, 1.0f, 0.0f);
						aa.angle = 0.0f;
					}
					else {
						aa.axis = aa.axis.Normalize();
					}

					fprintf(pFile, "\t\t\t<position x=\"%6.6f\" y=\"%6.6f\" z=\"%6.6f\" />\n", position.x, position.y, position.z);
					fprintf(pFile, "\t\t\t<rotation angle=\"%6.6f\">\n", -aa.angle);
					{
						fprintf(pFile, "\t\t\t\t<axis x=\"%6.6f\" y=\"%6.6f\" z=\"%6.6f\" />\n", aa.axis.x, aa.axis.y, aa.axis.z);
					}
					fprintf(pFile, "\t\t\t</rotation>\n");
				}
				fprintf(pFile, "\t\t</bone>\n");
			}
		}
		fprintf(pFile, "\t</bones>\n");

		fprintf(pFile, "\t<bonehierarchy>\n");
		{
			for (int indexBone = 0; indexBone < (int)m_pBoneHierarchy->bones.size(); indexBone++) {
				CBone *pBoneInit = m_pBoneHierarchy->bones[indexBone];
				_ASSERT(pBoneInit);

				if (bOptimizeSkeleton && !pBoneInit->isUsed) {
					continue;
				}

				if (!pBoneInit->isRoot) {
					fprintf(pFile, "\t\t<boneparent bone=\"%s\" parent=\"%s\" />\n", pBoneInit->szName, pBoneInit->szParentName);
				}
			}
		}
		fprintf(pFile, "\t</bonehierarchy>\n");

		if (!m_animSkeletons.empty()) {
			fprintf(pFile, "\t<animations>\n");
			{
				for (vector<CAnimSkeleton*>::const_iterator itAnim = m_animSkeletons.begin(); itAnim != m_animSkeletons.end(); ++itAnim) {
					CAnimSkeleton *pAnim = *itAnim;
					_ASSERT(pAnim);

					float length = (pAnim->pAnimInfo->endFrame - pAnim->pAnimInfo->startFrame + 1.0f)/pAnim->pAnimInfo->fps;

					fprintf(pFile, "\t\t<animation name=\"%s\" length=\"%6.6f\">\n", pAnim->pAnimInfo->szName, length);
					fprintf(pFile, "\t\t\t<tracks>\n");
					{
						for (int indexBone = 0; indexBone < (int)m_pBoneHierarchy->bones.size(); indexBone++) {
							CBone *pBoneInit = m_pBoneHierarchy->bones[indexBone];
							_ASSERT(pBoneInit);

							if (bOptimizeSkeleton && !pBoneInit->isUsed) {
								continue;
							}

							fprintf(pFile, "\t\t\t\t<track bone=\"%s\">\n", m_pBoneHierarchy->bones[indexBone]->szName);
							fprintf(pFile, "\t\t\t\t\t<keyframes>\n");
							{
								for (int indexFrame = 0; indexFrame < (int)pAnim->frames.size(); indexFrame++) {
									// 注意: Ogre保存的骨骼动画与一般意义上的骨骼动画不同
									//       Ogre保存的骨骼动画是当前骨骼与该骨骼在初始状态时的偏差
									// 公式:
									//       matLocal = matDelta*matLocalInit
									//       matDelta = matLocal*matLocalInitInv

									CBone *pBone = pAnim->frames[indexFrame]->bones[indexBone];
									_ASSERT(pBone);

									Point3 position = pBone->matLocal.Translation() - pBoneInit->matLocal.Translation();
									AngAxis aa((pBone->matLocal*pBoneInit->matLocal.Inverse()).ExtractMatrix3());

									if (aa.axis.Length() < 0.001 || fabs(aa.angle) < 0.001) {
										aa.axis = Point3(0.0f, 1.0f, 0.0f);
										aa.angle = 0.0f;
									}
									else {
										aa.axis = aa.axis.Normalize();
									}

									float time = length*indexFrame/pAnim->frames.size();

									fprintf(pFile, "\t\t\t\t\t\t<keyframe time=\"%6.6f\">\n", time);
									fprintf(pFile, "\t\t\t\t\t\t\t<translate x=\"%6.6f\" y=\"%6.6f\" z=\"%6.6f\" />\n", position.x, position.y, position.z);
									fprintf(pFile, "\t\t\t\t\t\t\t<rotate angle=\"%6.6f\">\n", -aa.angle);
									{
										fprintf(pFile, "\t\t\t\t\t\t\t\t<axis x=\"%6.6f\" y=\"%6.6f\" z=\"%6.6f\" />\n", aa.axis.x, aa.axis.y, aa.axis.z);
									}
									fprintf(pFile, "\t\t\t\t\t\t\t</rotate>\n");
									fprintf(pFile, "\t\t\t\t\t\t</keyframe>\n");
								}
							}
							fprintf(pFile, "\t\t\t\t\t</keyframes>\n");
							fprintf(pFile, "\t\t\t\t</track>\n");
						}
					}
					fprintf(pFile, "\t\t\t</tracks>\n");
					fprintf(pFile, "\t\t</animation>\n");
				}
			}
			fprintf(pFile, "\t</animations>\n");
		}
	}
	fprintf(pFile, "</skeleton>\n");
	fclose(pFile);
}

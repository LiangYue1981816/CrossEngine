#include "MaxExporter.h"


void CModel::OptimizeSkeleton(BOOL bOptimizeSkeleton)
{
	if (bOptimizeSkeleton) {
		for (int indexCurrBone = 0; indexCurrBone < (int)m_pBoneHierarchy->bones.size(); indexCurrBone++) {
			CBone *pCurrBone = m_pBoneHierarchy->bones[indexCurrBone];
			_ASSERT(pCurrBone);

			pCurrBone->id = -1;

			for (int indexPrevBone = 0; indexPrevBone <= indexCurrBone; indexPrevBone++) {
				CBone *pPrevBone = m_pBoneHierarchy->bones[indexPrevBone];
				_ASSERT(pPrevBone);

				if (pPrevBone->isUsed) {
					pCurrBone->id++;
				}
			}
		}

		for (int indexSubMesh = 0; indexSubMesh < (int)m_pSkinMesh->subMeshs.size(); indexSubMesh++) {
			CSubMesh *pSubMesh = m_pSkinMesh->subMeshs[indexSubMesh];
			_ASSERT(pSubMesh);

			if (!pSubMesh->isSkinMesh) {
				continue;
			}

			for (int indexVertex = 0; indexVertex < (int)pSubMesh->vertices.size(); indexVertex++) {
				CVertex *pVertex = pSubMesh->vertices[indexVertex];
				_ASSERT(pVertex);

				for (int indexSkin = 0; indexSkin < (int)pVertex->skinInfos.size(); indexSkin++) {
					CBone *pBone = m_pBoneHierarchy->bones[pVertex->skinInfos[indexSkin].boneID];
					_ASSERT(pBone);
					_ASSERT(pBone->isUsed);

					pVertex->skinInfos[indexSkin].boneID = pBone->id;
				}
			}
		}
	}
}

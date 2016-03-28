#include "MaxExporter.h"


//
// ����
//
CMaterial::CMaterial(IGameMaterial *pMaterial)
{
	_ASSERT(pMaterial);
	pMaxMaterial = pMaterial;

	isValid = FALSE;

	memset(szName, 0, sizeof(szName));

	isTwoSided = FALSE;

	ambient = Point3(1.0f, 1.0f, 1.0f);
	diffuse = Point3(1.0f, 1.0f, 1.0f);
	emissive = Point3(1.0f, 1.0f, 1.0f);
	specular = Point3(1.0f, 1.0f, 1.0f);
	specularLevel = 1.0f;

	texUnits.clear();
}

CMaterial::~CMaterial(void)
{
	texUnits.clear();
}

//
// ��Ƥ
//
CSkinInfo::CSkinInfo(void)
{
	boneID = 0;
	weight = 1.0f;
}

CSkinInfo::~CSkinInfo(void)
{

}

//
// ����
//
CVertex::CVertex(void)
{
	indexPosition = -1;
	indexNormal = -1;
	indexBinormal = -1;
	indexColor = -1;
	indexAlpha = -1;
	indexTexCoords.clear();

	position = Point3(0.0f, 0.0f, 0.0f);
	normal = Point3(0.0f, 0.0f, 0.0f);
	binormal = Point3(0.0f, 0.0f, 0.0f);
	color = Point4(1.0f, 1.0f, 1.0f, 1.0f);
	texCoords.clear();
	skinInfos.clear();
}

CVertex::CVertex(const CVertex &vertex)
{
	*this = vertex;
}

CVertex::~CVertex(void)
{
	texCoords.clear();
	skinInfos.clear();
}

CVertex& CVertex::operator = (const CVertex &vertex)
{
	indexPosition = vertex.indexPosition;
	indexNormal = vertex.indexNormal;
	indexBinormal = vertex.indexBinormal;
	indexColor = vertex.indexColor;
	indexAlpha = vertex.indexAlpha;
	indexTexCoords = vertex.indexTexCoords;

	position = vertex.position;
	normal = vertex.normal;
	binormal = vertex.binormal;
	color = vertex.color;
	texCoords = vertex.texCoords;
	skinInfos = vertex.skinInfos;

	return *this;
}

bool CVertex::operator == (const CVertex &vertex) const
{
	//
	// 1. ���������������
	//
	if (indexPosition != vertex.indexPosition) {
		return false;
	}

	//
	// 2. ���㷨��/�������������
	//
	if (indexNormal != vertex.indexNormal) {
		return false;
	}

	//
	// 3. ������ɫ�������
	//
	/*
	if (indexColor != vertex.indexColor) {
		return false;
	}

	if (indexAlpha != vertex.indexAlpha) {
		return false;
	}
	*/

	//
	// 4. ���������������
	//
	if (indexTexCoords.size() != vertex.indexTexCoords.size()) {
		return false;
	}

	for (int index = 0; index < (int)indexTexCoords.size(); index++) {
		if (indexTexCoords[index] != vertex.indexTexCoords[index]) {
			return false;
		}
	}

	return true;
}

bool CompareSkin(const CSkinInfo &skinInfo0, const CSkinInfo &skinInfo1)
{   
	return skinInfo0.weight > skinInfo1.weight;   
}

void CVertex::NormalizeSkins(void)
{
	//
	// 1. ����Ȩ���ɴ�С����
	//
	sort(skinInfos.begin(), skinInfos.end(), CompareSkin);

	//
	// 2. ����ǰ4����Ƥ��Ϣ
	//
	while (!skinInfos.empty()) {
		vector<CSkinInfo>::const_iterator itSkinInfo = --skinInfos.end();

		if (skinInfos.size() > 4 || itSkinInfo->weight < 0.001f) {
			skinInfos.erase(itSkinInfo);
			continue;
		}

		break;
	}

	//
	// 3. ��λ��Ȩ��
	//
	float totalWeight = 0.0f;

	for (int indexSkin = 0; indexSkin < (int)skinInfos.size(); indexSkin++) {
		totalWeight += skinInfos[indexSkin].weight;
	}

	for (int indexSkin = 0; indexSkin < (int)skinInfos.size(); indexSkin++) {
		skinInfos[indexSkin].weight /= totalWeight;
	}
}

//
// ������
//
CFace::CFace(FaceEx *pFace)
{
	_ASSERT(pFace);
	pMaxFace = pFace;

	indexMaterial = 0;
	indexVertices[0] = 0;
	indexVertices[1] = 0;
	indexVertices[2] = 0;

	normal = Point3(0.0f, 0.0f, 0.0f);
}

CFace::~CFace(void)
{

}

//
// ������
//
CSubMesh::CSubMesh(IGameMesh *pMesh)
{
	_ASSERT(pMesh);
	pMaxMesh = pMesh;

	memset(szName, 0, sizeof(szName));
	memset(szMaterialName, 0, sizeof(szMaterialName));
	memset(szLightMapName, 0, sizeof(szLightMapName));

	isSkinMesh = FALSE;
	nMaxSkinWeightsPerVertex = 0;
	nMaxSkinWeightsPerFace = 0;

	nNumTexChannel = 1;

	matrix.SetIdentity();

	vertices.clear();
	faces.clear();

	materials.clear();
	bones.clear();
}

CSubMesh::~CSubMesh(void)
{
	for (vector<CVertex*>::const_iterator itVertex = vertices.begin(); itVertex != vertices.end(); ++itVertex) {
		delete *itVertex;
	}

	for (vector<CFace*>::const_iterator itFace = faces.begin(); itFace != faces.end(); ++itFace) {
		delete *itFace;
	}

	for (map<CBone*,vector<int>*>::const_iterator itBone = bones.begin(); itBone != bones.end(); ++itBone) {
		delete itBone->second;
	}

	vertices.clear();
	faces.clear();

	materials.clear();
	bones.clear();
}

int CSubMesh::QueryVertexID(const CVertex &vertex) const
{
	for (int indexVertex = 0; indexVertex < (int)vertices.size(); indexVertex++) {
		CVertex *pVertex = vertices[indexVertex];
		_ASSERT(pVertex);

		if (vertex == *pVertex) {
			return indexVertex;
		}
	}

	return -1;
}

CVertex* CSubMesh::QueryVertex(const CVertex &vertex) const
{
	for (vector<CVertex*>::const_iterator itVertex = vertices.begin(); itVertex != vertices.end(); ++itVertex) {
		CVertex *pVertex = *itVertex;
		_ASSERT(pVertex);

		if (vertex == *pVertex) {
			return pVertex;
		}
	}

	return NULL;
}

int CSubMesh::QueryMaterialID(const char *szMaterialName) const
{
	for (int indexMaterial = 0; indexMaterial < (int)materials.size(); indexMaterial++) {
		CMaterial *pMaterial = materials[indexMaterial];
		_ASSERT(pMaterial);

		if (!stricmp(pMaterial->szName, szMaterialName)) {
			return indexMaterial;
		}
	}

	return -1;
}

CMaterial* CSubMesh::QueryMaterial(const char *szMaterialName) const
{
	for (vector<CMaterial*>::const_iterator itMaterial = materials.begin(); itMaterial != materials.end(); ++itMaterial) {
		CMaterial *pMaterial = *itMaterial;
		_ASSERT(pMaterial);

		if (!stricmp(pMaterial->szName, szMaterialName)) {
			return pMaterial;
		}
	}

	return NULL;
}

//
// ����
//
CMesh::CMesh(void)
{
	subMeshs.clear();
}

CMesh::~CMesh(void)
{
	for (vector<CSubMesh*>::const_iterator itSubMesh = subMeshs.begin(); itSubMesh != subMeshs.end(); ++itSubMesh) {
		delete *itSubMesh;
	}

	subMeshs.clear();
}

//
// ����
//
CBone::CBone(IGameNode *pBone)
{
	_ASSERT(pBone);
	pMaxBone = pBone;

	memset(szName, 0, sizeof(szName));
	memset(szParentName, 0, sizeof(szParentName));

	id = -1;
	isUsed = FALSE;
	isRoot = FALSE;
	isDummy = FALSE;

	pParentBone = NULL;

	matLocal.SetIdentity();
	matWorld.SetIdentity();

	childBones.clear();
}

CBone::~CBone(void)
{
	childBones.clear();
}

void CBone::SetUsed(void)
{
	isUsed = TRUE;

	if (pParentBone) {
		pParentBone->SetUsed();
	}
}

//
// �����
//
CBoneFrame::CBoneFrame(void)
	: tick(0.0f)
{
	bones.clear();
}

CBoneFrame::CBoneFrame(float t)
	: tick(t)
{
	bones.clear();
}

CBoneFrame::~CBoneFrame(void)
{
	for (vector<CBone*>::const_iterator itBone = bones.begin(); itBone != bones.end(); ++itBone) {
		delete *itBone;
	}

	bones.clear();
}

int CBoneFrame::QueryBoneID(const char *szBoneName) const
{
	for (int indexBone = 0; indexBone < (int)bones.size(); indexBone++) {
		CBone *pBone = bones[indexBone];
		_ASSERT(pBone);

		if (!stricmp(szBoneName, pBone->szName)) {
			return pBone->id;
		}
	}

	return -1;
}

CBone* CBoneFrame::QueryBone(const char *szBoneName) const
{
	for (vector<CBone*>::const_iterator itBone = bones.begin(); itBone != bones.end(); ++itBone) {
		CBone *pBone = *itBone;
		_ASSERT(pBone);

		if (!stricmp(szBoneName, pBone->szName)) {
			return pBone;
		}
	}

	return NULL;
}

int CBoneFrame::GetBoneCount(BOOL bOptimizeSkeleton) const
{
	int numBones = 0;

	for (vector<CBone*>::const_iterator itBone = bones.begin(); itBone != bones.end(); ++itBone) {
		CBone *pBone = *itBone;
		_ASSERT(pBone);

		if (bOptimizeSkeleton) {
			if (pBone->isUsed) {
				numBones++;
			}
		}
		else {
			numBones++;
		}
	}

	return numBones;
}

//
// ������Ϣ
//
CAnimationInfo::CAnimationInfo(void)
{
	memset(szName, 0, sizeof(szName));

	fps = 30;
	startFrame = 0;
	endFrame = 0;
}

CAnimationInfo::~CAnimationInfo(void)
{

}

//
// ���㶯��
//
CAnimVertex::CAnimVertex(void)
{
	pAnimInfo = NULL;
	frames.clear();
}

CAnimVertex::~CAnimVertex(void)
{
	if (pAnimInfo) {
		delete pAnimInfo;
	}

	for (vector<CMesh*>::const_iterator itFrame = frames.begin(); itFrame != frames.end(); ++itFrame) {
		delete *itFrame;
	}

	frames.clear();
}

//
// ��������
//
CAnimSkeleton::CAnimSkeleton(void)
{
	pAnimInfo = NULL;
	frames.clear();
}

CAnimSkeleton::~CAnimSkeleton(void)
{
	if (pAnimInfo) {
		delete pAnimInfo;
	}

	for (vector<CBoneFrame*>::const_iterator itFrame = frames.begin(); itFrame != frames.end(); ++itFrame) {
		delete *itFrame;
	}

	frames.clear();
}

//
// ģ��
//
CModel::CModel(Interface *pInterface, IGameScene *pGameScene, IGameConversionManager::CoordSystem cs)
{
	_ASSERT(pInterface);
	_ASSERT(pGameScene);

	m_pSkinMesh = NULL;
	m_pBoneHierarchy = NULL;

	m_coordSystem = cs;
	m_pInterface = pInterface;
	m_pGameScene = pGameScene;

	m_materials.clear();

	m_keyFrames.clear();
	m_boneNodes.clear();

	m_animVertexs.clear();
	m_animSkeletons.clear();
}

CModel::~CModel(void)
{
	Free();
}

void CModel::Free(void)
{
	if (m_pSkinMesh) {
		delete m_pSkinMesh;
	}

	if (m_pBoneHierarchy) {
		delete m_pBoneHierarchy;
	}

	for (vector<CMaterial*>::const_iterator itMaterial = m_materials.begin(); itMaterial != m_materials.end(); ++itMaterial) {
		delete *itMaterial;
	}

	for (vector<CAnimVertex*>::const_iterator itAnim = m_animVertexs.begin(); itAnim != m_animVertexs.end(); ++itAnim) {
		delete *itAnim;
	}

	for (vector<CAnimSkeleton*>::const_iterator itAnim = m_animSkeletons.begin(); itAnim != m_animSkeletons.end(); ++itAnim) {
		delete *itAnim;
	}

	m_materials.clear();

	m_keyFrames.clear();
	m_boneNodes.clear();

	m_animVertexs.clear();
	m_animSkeletons.clear();

	m_pSkinMesh = NULL;
	m_pBoneHierarchy = NULL;
}

int CModel::QueryMaterialID(const char *szMaterialName) const
{
	for (int indexMaterial = 0; indexMaterial < (int)m_materials.size(); indexMaterial++) {
		CMaterial *pMaterial = m_materials[indexMaterial];
		_ASSERT(pMaterial);

		if (!stricmp(pMaterial->szName, szMaterialName)) {
			return indexMaterial;
		}
	}

	return -1;
}

CMaterial* CModel::QueryMaterial(const char *szMaterialName) const
{
	for (vector<CMaterial*>::const_iterator itMaterial = m_materials.begin(); itMaterial != m_materials.end(); ++itMaterial) {
		CMaterial *pMaterial = *itMaterial;
		_ASSERT(pMaterial);

		if (!stricmp(pMaterial->szName, szMaterialName)) {
			return pMaterial;
		}
	}

	return NULL;
}

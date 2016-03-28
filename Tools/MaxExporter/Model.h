#pragma once

#include "MaxExporter.h"


//
// ����
// ע��: 1. ��ά�Ӳ��ʻᱻ��������������Ƕ��
//       2. �����������������Ϣ
//
//#define ID_AM 0   // ambient
//#define ID_DI 1   // diffuse
//#define ID_SP 2   // specular
//#define ID_SH 3   // shininesNs
//#define ID_SS 4   // shininess strength
//#define ID_SI 5   // self-illumination
//#define ID_OP 6   // opacity
//#define ID_FI 7   // filter color
//#define ID_BU 8   // bump 
//#define ID_RL 9   // reflection
//#define ID_RR 10  // refraction 
//#define ID_DP 11  // displacement
  #define ID_LM 11  // lightmap

class CMaterial
{
public:
	CMaterial(IGameMaterial *pMaterial);
	virtual ~CMaterial(void);

public:
	BOOL isValid;                                                                                  // ��Ч��

	char szName[_MAX_PATH];                                                                        // ������
	                                                                                               // ע��: 1. ��������������
	                                                                                               //       2. �������������ո�
	                                                                                               //       3. ��λ�Ӳ���������丸�����������
	                                                                                               //       4. ����������ĸ����Ϊ����

	BOOL isTwoSided;                                                                               // ˫�����

	Point3 ambient;                                                                                // ������
	Point3 diffuse;                                                                                // �������
	Point3 emissive;                                                                               // ������
	Point3 specular;                                                                               // �����
	float specularLevel;                                                                           // �����ȼ�
	map<int,string> texUnits;                                                                      // ����Ԫ
	                                                                                               // ����Ԫ���ռ������г���������������(��������ڵĻ�), �ڵ���ʱ
	                                                                                               // ��һ����ȫ������, �Ӿ���ĵ�����ʽ����.

	IGameMaterial *pMaxMaterial;                                                                   // Max����ָ��
};

//
// ��Ƥ
//
class CSkinInfo
{
public:
	CSkinInfo(void);
	virtual ~CSkinInfo(void);

public:
	int boneID;                                                                                    // ����ID
                                                                                                   // ע��: ��IDָ��m_pBoneHierarchy.bones
	float weight;                                                                                  // ����Ȩ��
};

//
// ����
// ע��: 1. ÿ���������ֻ������4����Ƥ��Ϣ
//       2. ÿ������ֻʹ��һ����������
//
class CVertex
{
public:
	CVertex(void);
	CVertex(const CVertex &vertex);
	virtual ~CVertex(void);

public:
	CVertex& operator = (const CVertex &vertex);                                                   // ��ֵ����
	bool operator == (const CVertex &vertex) const;                                                // ���ڲ���

	void NormalizeSkins(void);                                                                     // �淶����Ƥ��Ϣ
	                                                                                               // ע��: ���ֻ����4����������, ������4��������ɾ����С
	                                                                                               //       �Ĳ�����Ȩ�ع淶��

public:
	int indexPosition;                                                                             // ������������
	int indexNormal;                                                                               // ���㷨������
	int indexBinormal;                                                                             // ���㸱��������
	int indexColor;                                                                                // ������ɫ����
	int indexAlpha;                                                                                // ����͸������
	vector<int> indexTexCoords;                                                                    // ����������������

	Point3 position;                                                                               // ��������, ע��: ���������������������ϵ
	Point3 normal;                                                                                 // ���㷨��, ע��: ���㷨�����������������ϵ, Max�л�õĶ��㷨������ʱ��������, ��Ҫ���¼��㷨����
	Point3 binormal;                                                                               // ���㸱����
	Point4 color;                                                                                  // ������ɫ
	vector<Point3> texCoords;                                                                      // ������������

	vector<CSkinInfo> skinInfos;                                                                   // ��Ƥ��Ϣ
};

//
// ������
//
class CFace
{
public:
	CFace(FaceEx *pFace);
	virtual ~CFace(void);

public:
	int indexMaterial;                                                                             // ��������, ע��: ����Ĳ�������ָ�����CSubMesh::materials
	int indexVertices[3];                                                                          // ��������, ע��: ����Ķ�������ָ�����CSubMesh::vertices
	Point3 normal;                                                                                 // �淨����, �������¼��㶥�㷨����

	FaceEx *pMaxFace;                                                                              // Max������ָ��
};

//
// ����
//
class CBone
{
public:
	CBone(IGameNode *pBone);
	virtual ~CBone(void);

public:
	void SetUsed(void);                                                                            // ���ù���ʹ��״̬
	                                                                                               // �Ե����ϱ���ֱ��������

public:
	char szName[64];                                                                               // ������, ˵��: �������ƹ�������಻����64���ַ�
	char szParentName[64];                                                                         // ��������, ˵��: �������ƹ�������಻����64���ַ�

	int id;                                                                                        // ����id

	BOOL isUsed;                                                                                   // ʹ��״̬
	BOOL isRoot;                                                                                   // ���ڵ�
	BOOL isDummy;                                                                                  // �Ҽ�

	GMatrix matLocal;                                                                              // �ڸ�����ϵ�еı任����
	GMatrix matWorld;                                                                              // ����������ϵ�еı任����

	CBone* pParentBone;                                                                            // ������
	vector<CBone*> childBones;                                                                     // �ӹ�������

	IGameNode *pMaxBone;                                                                           // Max����ָ��
};

//
// ������
//
class CSubMesh
{
public:
	CSubMesh(IGameMesh *pMaxMesh);
	virtual ~CSubMesh(void);

public:
	int QueryVertexID(const CVertex &vertex) const;                                                // ��ѯ��������
	int QueryMaterialID(const char *szMaterialName) const;                                         // ��ѯ��������

	CVertex* QueryVertex(const CVertex &vertex) const;                                             // ��ѯ����
	CMaterial* QueryMaterial(const char *szMaterialName) const;                                    // ��ѯ����

public:
	char szName[_MAX_PATH];                                                                        // ������
	char szMaterialName[_MAX_PATH];                                                                // ������, ע��: ����Ĳ���һ���Ӧ������
	char szLightMapName[_MAX_PATH];                                                                // ������ͼ��

	BOOL isSkinMesh;                                                                               // ��Ƥ����
	int nMaxSkinWeightsPerVertex;                                                                  // һ����������ܵ�����Ӱ�����������,�����ڼ��㹲ͬ����ʱ���ٱ�������
	int nMaxSkinWeightsPerFace;                                                                    // һ������������ܵ�����Ӱ�����������, ������ֶ�Ӳ�������ϼ�������˻���Ҫ��

	int nNumTexChannel;                                                                            // ����ͨ��

	GMatrix matrix;                                                                                // �����ʼ�任����, Vertex_Local * Matrix = Vertex_World
	vector<CVertex*> vertices;                                                                     // ���㼯��
	vector<CFace*> faces;                                                                          // �����漯��

	vector<CMaterial*> materials;                                                                  // �����õ��Ĳ��ʼ���, ע��: ���ʼ����еĲ���ָ��ָ��CModel::m_materials
	map<CBone*,vector<int>*> bones;                                                                // Ӱ������Ĺ�������, ע��: ���������еĹ���ָ��ָ��CModel::m_pBoneHierarchy

	IGameMesh *pMaxMesh;                                                                           // Max����ָ��
};

//
// ����
//
class CMesh
{
public:
	CMesh(void);
	virtual ~CMesh(void);

public:
	vector<CSubMesh*> subMeshs;                                                                    // �����񼯺�
};

//
// ������
//
class CBoneFrame
{
public:
	CBoneFrame(void);
	CBoneFrame(float t);
	virtual ~CBoneFrame(void);

public:
	int QueryBoneID(const char *szBoneName) const;                                                 // ��ѯ����ID
	CBone* QueryBone(const char *szBoneName) const;                                                // ��ѯ����

	int GetBoneCount(BOOL bOptimizeSkeleton) const;                                                // ��ù�����

public:
	float tick;                                                                                    // ʱ���
	vector<CBone*> bones;                                                                          // ��������
};

//
// ������Ϣ
//
class CAnimationInfo
{
public:
	CAnimationInfo(void);
	virtual ~CAnimationInfo(void);

public:
	char szName[_MAX_PATH];                                                                        // ������

	int fps;                                                                                       // ����
	int startFrame;                                                                                // ��ʼ��
	int endFrame;                                                                                  // ��ֹ��
};

//
// ���㶯��
//
class CAnimVertex
{
public:
	CAnimVertex(void);
	virtual ~CAnimVertex(void);

public:
	CAnimationInfo *pAnimInfo;                                                                     // ������Ϣ
	vector<CMesh*> frames;                                                                         // ����������
};

//
// ��������
//
class CAnimSkeleton
{
public:
	CAnimSkeleton(void);
	virtual ~CAnimSkeleton(void);

public:
	CAnimationInfo *pAnimInfo;                                                                     // ������Ϣ
	vector<CBoneFrame*> frames;                                                                    // ����������
};


//
// ģ��
//
class CModel
{
public:
	CModel(Interface *pInterface, IGameScene *pGameScene, IGameConversionManager::CoordSystem cs);
	virtual ~CModel(void);

public:
	void Free(void);                                                                               // �ͷ�
	void DoExport(const char *szName, const char *szCommand, int nRenderID, BOOL bExportKeyFrame, BOOL bExportSubMesh, BOOL bInWorldSpcae, char *szExternalSkeleton, BOOL bOptimizeSkeleton, BOOL bGUPSkinning, SOLUTION_PARAMS low, SOLUTION_PARAMS medium, SOLUTION_PARAMS high, SOLUTION_PARAMS ultra, SOLUTION_PARAMS extreme); // ����

private:
	void DoExportMaterial(const char *szPath);                                                     // ��������
	void DoExportBoneHierarchy(void);                                                              // ���������ṹ
	void DoExportMesh(BOOL bInWorldSpcae);                                                         // ��������
	void DoExportKeyFrames(void);                                                                  // �����ؼ�֡
	void DoExportAnimList(void);                                                                   // ���������б�
	void DoExportAnimVertex(void);                                                                 // �������㶯��
	void DoExportAnimSkeleton(BOOL bExportKeyFrame);                                               // ������������
	void OptimizeSkeleton(BOOL bOptimizeSkeleton);                                                 // �Ż�����

private:
	void DoExportMaterial(const char *szPath, IGameMaterial *pMaxMaterial, IGameMaterial *pMaxParentMaterial); // ��������
	void DoExportBone(IGameNode *pNode, IGameNode *pParentNode, CBone *pParentBone, CBoneFrame *pBoneFrame); // ���������
	void DoExportMesh(IGameNode *pNode, CMesh *pMesh, BOOL bInWorldSpcae);                         // ��������
	CSubMesh* DoExportSubMesh(IGameNode *pNode, BOOL bInWorldSpcae);                               // ����������
	CAnimationInfo* DoExportAnimInfo(const char *szKey, const char *szValue, const char *szKeyWorld); // ����������Ϣ

private:
	int QueryMaterialID(const char *szMaterialName) const;                                         // ��ѯ��������
	CMaterial* QueryMaterial(const char *szMaterialName) const;                                    // ��ѯ����

private:
	void OgreXML_WriteMaterial(const char *szName, const char *szFileName) const;                  // �������
	void OgreXML_WriteMesh(const char *szName, const char *szFileName, const char *szSkeletonAnimName) const; // �������
	void OgreXML_WriteAnimSkeleton(const char *szFileName, BOOL bOptimizeSkeleton) const;          // �����������

private:
	void D3DX_WriteMesh(const char *szFileName, BOOL bOptimizeSkeleton) const;                     // �������
	void D3DX_WriteAnimSkeleton(const char *szFileName, BOOL bOptimizeSkeleton) const;             // �����������
	void D3DX_WriteBoneFrame(FILE *pFile, CBone *pBone, char *szIndent, BOOL bOptimizeSkeleton) const; // ���������

private:
	void Cross_WriteMaterial(const char *szName, const char *szPathName, int nRenderID, BOOL bGUPSkinning, SOLUTION_PARAMS low, SOLUTION_PARAMS medium, SOLUTION_PARAMS high, SOLUTION_PARAMS ultra, SOLUTION_PARAMS extreme) const; // �������
	void Cross_WriteMesh(const char *szName, const char *szFileName, const char *szSkeletonAnimName, const vector<CSubMesh*> &subMeshs); // �������
	void Cross_WriteAnimSkeleton(const char *szFileName, BOOL bOptimizeSkeleton) const;             // �����������

private:
	Interface *m_pInterface;                                                                       // �ӿ�
	IGameScene *m_pGameScene;                                                                      // ����
	IGameConversionManager::CoordSystem m_coordSystem;                                             // ����ϵͳ

	CMesh *m_pSkinMesh;                                                                            // ��Ƥ����
	CBoneFrame *m_pBoneHierarchy;                                                                  // ������ϵ
	vector<CMaterial*> m_materials;                                                                // ���ʼ���

	map<int, int> m_keyFrames;                                                                     // �ؼ�֡����
	vector<IGameNode*> m_boneNodes;                                                                // �����ڵ㼯��

	vector<CAnimVertex*> m_animVertexs;                                                            // ���㶯������
	vector<CAnimSkeleton*> m_animSkeletons;                                                        // ������������
};

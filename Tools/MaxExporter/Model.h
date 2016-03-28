#pragma once

#include "MaxExporter.h"


//
// 材质
// 注意: 1. 多维子材质会被单独导出而不会嵌套
//       2. 不导出纹理动画相关信息
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
	BOOL isValid;                                                                                  // 有效性

	char szName[_MAX_PATH];                                                                        // 材质名
	                                                                                               // 注意: 1. 材质名不能重名
	                                                                                               //       2. 材质名不包括空格
	                                                                                               //       3. 多位子材质名会和其父材质组合起来
	                                                                                               //       4. 材质名首字母不能为数字

	BOOL isTwoSided;                                                                               // 双面材质

	Point3 ambient;                                                                                // 环境光
	Point3 diffuse;                                                                                // 漫反射光
	Point3 emissive;                                                                               // 反射率
	Point3 specular;                                                                               // 镜面光
	float specularLevel;                                                                           // 镜面光等级
	map<int,string> texUnits;                                                                      // 纹理单元
	                                                                                               // 纹理单元会收集上面列出的所有类型纹理(如果它存在的话), 在导出时
	                                                                                               // 不一定会全部导出, 视具体的到处格式而定.

	IGameMaterial *pMaxMaterial;                                                                   // Max材质指针
};

//
// 蒙皮
//
class CSkinInfo
{
public:
	CSkinInfo(void);
	virtual ~CSkinInfo(void);

public:
	int boneID;                                                                                    // 骨骼ID
                                                                                                   // 注意: 该ID指向m_pBoneHierarchy.bones
	float weight;                                                                                  // 骨骼权重
};

//
// 顶点
// 注意: 1. 每个顶点最大只保留其4个蒙皮信息
//       2. 每个顶点只使用一个纹理坐标
//
class CVertex
{
public:
	CVertex(void);
	CVertex(const CVertex &vertex);
	virtual ~CVertex(void);

public:
	CVertex& operator = (const CVertex &vertex);                                                   // 赋值操作
	bool operator == (const CVertex &vertex) const;                                                // 等于操作

	void NormalizeSkins(void);                                                                     // 规范化蒙皮信息
	                                                                                               // 注意: 最多只允许4个骨骼控制, 若多余4个骨骼则删除较小
	                                                                                               //       的并进行权重规范化

public:
	int indexPosition;                                                                             // 顶点坐标索引
	int indexNormal;                                                                               // 顶点法向索引
	int indexBinormal;                                                                             // 顶点副法向索引
	int indexColor;                                                                                // 顶点颜色索引
	int indexAlpha;                                                                                // 顶点透明索引
	vector<int> indexTexCoords;                                                                    // 顶点纹理坐标索引

	Point3 position;                                                                               // 顶点坐标, 注意: 顶点坐标相对于网格坐标系
	Point3 normal;                                                                                 // 顶点法向, 注意: 顶点法向量相对于网格坐标系, Max中获得的顶点法向量有时存在问题, 需要重新计算法向量
	Point3 binormal;                                                                               // 顶点副法向
	Point4 color;                                                                                  // 顶点颜色
	vector<Point3> texCoords;                                                                      // 顶点纹理坐标

	vector<CSkinInfo> skinInfos;                                                                   // 蒙皮信息
};

//
// 三角面
//
class CFace
{
public:
	CFace(FaceEx *pFace);
	virtual ~CFace(void);

public:
	int indexMaterial;                                                                             // 材质索引, 注意: 这里的材质索引指向的是CSubMesh::materials
	int indexVertices[3];                                                                          // 顶点索引, 注意: 这里的顶点索引指向的是CSubMesh::vertices
	Point3 normal;                                                                                 // 面法向量, 用于重新计算顶点法向量

	FaceEx *pMaxFace;                                                                              // Max面数据指针
};

//
// 骨骼
//
class CBone
{
public:
	CBone(IGameNode *pBone);
	virtual ~CBone(void);

public:
	void SetUsed(void);                                                                            // 设置骨骼使用状态
	                                                                                               // 自底向上遍历直到根骨骼

public:
	char szName[64];                                                                               // 骨骼名, 说明: 引擎限制骨骼名最多不超过64个字符
	char szParentName[64];                                                                         // 父骨骼名, 说明: 引擎限制骨骼名最多不超过64个字符

	int id;                                                                                        // 骨骼id

	BOOL isUsed;                                                                                   // 使用状态
	BOOL isRoot;                                                                                   // 根节点
	BOOL isDummy;                                                                                  // 挂件

	GMatrix matLocal;                                                                              // 在父坐标系中的变换矩阵
	GMatrix matWorld;                                                                              // 在世界坐标系中的变换矩阵

	CBone* pParentBone;                                                                            // 父骨骼
	vector<CBone*> childBones;                                                                     // 子骨骼集合

	IGameNode *pMaxBone;                                                                           // Max骨骼指针
};

//
// 子网格
//
class CSubMesh
{
public:
	CSubMesh(IGameMesh *pMaxMesh);
	virtual ~CSubMesh(void);

public:
	int QueryVertexID(const CVertex &vertex) const;                                                // 查询顶点索引
	int QueryMaterialID(const char *szMaterialName) const;                                         // 查询材质索引

	CVertex* QueryVertex(const CVertex &vertex) const;                                             // 查询顶点
	CMaterial* QueryMaterial(const char *szMaterialName) const;                                    // 查询材质

public:
	char szName[_MAX_PATH];                                                                        // 网格名
	char szMaterialName[_MAX_PATH];                                                                // 材质名, 注意: 这里的材质一般对应父材质
	char szLightMapName[_MAX_PATH];                                                                // 光照贴图名

	BOOL isSkinMesh;                                                                               // 蒙皮网格
	int nMaxSkinWeightsPerVertex;                                                                  // 一个顶点可以受到骨骼影响的最大骨骼数,可用于计算共同作用时减少遍历次数
	int nMaxSkinWeightsPerFace;                                                                    // 一个三角面可以受到骨骼影响的最大骨骼数, 这个数字对硬件顶点混合计算提出了基本要求

	int nNumTexChannel;                                                                            // 纹理通道

	GMatrix matrix;                                                                                // 网格初始变换矩阵, Vertex_Local * Matrix = Vertex_World
	vector<CVertex*> vertices;                                                                     // 顶点集合
	vector<CFace*> faces;                                                                          // 三角面集合

	vector<CMaterial*> materials;                                                                  // 网格用到的材质集合, 注意: 材质集合中的材质指针指向CModel::m_materials
	map<CBone*,vector<int>*> bones;                                                                // 影响网格的骨骼集合, 注意: 骨骼集合中的骨骼指针指向CModel::m_pBoneHierarchy

	IGameMesh *pMaxMesh;                                                                           // Max网格指针
};

//
// 网格
//
class CMesh
{
public:
	CMesh(void);
	virtual ~CMesh(void);

public:
	vector<CSubMesh*> subMeshs;                                                                    // 子网格集合
};

//
// 骨骼贞
//
class CBoneFrame
{
public:
	CBoneFrame(void);
	CBoneFrame(float t);
	virtual ~CBoneFrame(void);

public:
	int QueryBoneID(const char *szBoneName) const;                                                 // 查询骨骼ID
	CBone* QueryBone(const char *szBoneName) const;                                                // 查询骨骼

	int GetBoneCount(BOOL bOptimizeSkeleton) const;                                                // 获得骨骼数

public:
	float tick;                                                                                    // 时间戳
	vector<CBone*> bones;                                                                          // 骨骼集合
};

//
// 动画信息
//
class CAnimationInfo
{
public:
	CAnimationInfo(void);
	virtual ~CAnimationInfo(void);

public:
	char szName[_MAX_PATH];                                                                        // 动画名

	int fps;                                                                                       // 贞率
	int startFrame;                                                                                // 起始贞
	int endFrame;                                                                                  // 终止贞
};

//
// 顶点动画
//
class CAnimVertex
{
public:
	CAnimVertex(void);
	virtual ~CAnimVertex(void);

public:
	CAnimationInfo *pAnimInfo;                                                                     // 动画信息
	vector<CMesh*> frames;                                                                         // 动画贞数据
};

//
// 骨骼动画
//
class CAnimSkeleton
{
public:
	CAnimSkeleton(void);
	virtual ~CAnimSkeleton(void);

public:
	CAnimationInfo *pAnimInfo;                                                                     // 动画信息
	vector<CBoneFrame*> frames;                                                                    // 动画贞数据
};


//
// 模型
//
class CModel
{
public:
	CModel(Interface *pInterface, IGameScene *pGameScene, IGameConversionManager::CoordSystem cs);
	virtual ~CModel(void);

public:
	void Free(void);                                                                               // 释放
	void DoExport(const char *szName, const char *szCommand, int nRenderID, BOOL bExportKeyFrame, BOOL bExportSubMesh, BOOL bInWorldSpcae, char *szExternalSkeleton, BOOL bOptimizeSkeleton, BOOL bGUPSkinning, SOLUTION_PARAMS low, SOLUTION_PARAMS medium, SOLUTION_PARAMS high, SOLUTION_PARAMS ultra, SOLUTION_PARAMS extreme); // 导出

private:
	void DoExportMaterial(const char *szPath);                                                     // 导出材质
	void DoExportBoneHierarchy(void);                                                              // 导出骨骼结构
	void DoExportMesh(BOOL bInWorldSpcae);                                                         // 导出网格
	void DoExportKeyFrames(void);                                                                  // 导出关键帧
	void DoExportAnimList(void);                                                                   // 导出动画列表
	void DoExportAnimVertex(void);                                                                 // 导出顶点动画
	void DoExportAnimSkeleton(BOOL bExportKeyFrame);                                               // 导出骨骼动画
	void OptimizeSkeleton(BOOL bOptimizeSkeleton);                                                 // 优化骨骼

private:
	void DoExportMaterial(const char *szPath, IGameMaterial *pMaxMaterial, IGameMaterial *pMaxParentMaterial); // 导出材质
	void DoExportBone(IGameNode *pNode, IGameNode *pParentNode, CBone *pParentBone, CBoneFrame *pBoneFrame); // 导出贞骨骼
	void DoExportMesh(IGameNode *pNode, CMesh *pMesh, BOOL bInWorldSpcae);                         // 导出网格
	CSubMesh* DoExportSubMesh(IGameNode *pNode, BOOL bInWorldSpcae);                               // 导出子网格
	CAnimationInfo* DoExportAnimInfo(const char *szKey, const char *szValue, const char *szKeyWorld); // 导出动画信息

private:
	int QueryMaterialID(const char *szMaterialName) const;                                         // 查询材质索引
	CMaterial* QueryMaterial(const char *szMaterialName) const;                                    // 查询材质

private:
	void OgreXML_WriteMaterial(const char *szName, const char *szFileName) const;                  // 输出材质
	void OgreXML_WriteMesh(const char *szName, const char *szFileName, const char *szSkeletonAnimName) const; // 输出网格
	void OgreXML_WriteAnimSkeleton(const char *szFileName, BOOL bOptimizeSkeleton) const;          // 输出骨骼动画

private:
	void D3DX_WriteMesh(const char *szFileName, BOOL bOptimizeSkeleton) const;                     // 输出网格
	void D3DX_WriteAnimSkeleton(const char *szFileName, BOOL bOptimizeSkeleton) const;             // 输出骨骼动画
	void D3DX_WriteBoneFrame(FILE *pFile, CBone *pBone, char *szIndent, BOOL bOptimizeSkeleton) const; // 输出骨骼贞

private:
	void Cross_WriteMaterial(const char *szName, const char *szPathName, int nRenderID, BOOL bGUPSkinning, SOLUTION_PARAMS low, SOLUTION_PARAMS medium, SOLUTION_PARAMS high, SOLUTION_PARAMS ultra, SOLUTION_PARAMS extreme) const; // 输出材质
	void Cross_WriteMesh(const char *szName, const char *szFileName, const char *szSkeletonAnimName, const vector<CSubMesh*> &subMeshs); // 输出网格
	void Cross_WriteAnimSkeleton(const char *szFileName, BOOL bOptimizeSkeleton) const;             // 输出骨骼动画

private:
	Interface *m_pInterface;                                                                       // 接口
	IGameScene *m_pGameScene;                                                                      // 场景
	IGameConversionManager::CoordSystem m_coordSystem;                                             // 坐标系统

	CMesh *m_pSkinMesh;                                                                            // 蒙皮网格
	CBoneFrame *m_pBoneHierarchy;                                                                  // 骨骼关系
	vector<CMaterial*> m_materials;                                                                // 材质集合

	map<int, int> m_keyFrames;                                                                     // 关键帧集合
	vector<IGameNode*> m_boneNodes;                                                                // 骨骼节点集合

	vector<CAnimVertex*> m_animVertexs;                                                            // 顶点动画集合
	vector<CAnimSkeleton*> m_animSkeletons;                                                        // 骨骼动画集合
};

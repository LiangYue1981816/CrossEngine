/****************************************************************************
Copyright (c) 2015 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "_CrossEngine.h"


namespace CrossEngine {

	CSceneNode::CSceneNode(DWORD dwName, CSceneManager *pSceneManager)
		: userData(NULL)

		, m_dwName(dwName)
		, m_pParentNode(NULL)
		, m_pSceneManager(pSceneManager)

		, m_bVisible(TRUE)
		, m_bNeedRender(TRUE)
		, m_bNeedUpdateAABB(TRUE)
		, m_bNeedUpdateTransform(TRUE)
		, m_dwUpdateTransformCount(0)
		, m_dwParentUpdateTransformCount(0)

		, m_pNodeHead(NULL)
		, m_pEntityHead(NULL)

		, pNext(NULL)
	{
		Identity();

		AABBZero(&m_aabbCurrent);
		AABBZero(&m_aabbCascade);
	}

	CSceneNode::~CSceneNode(VOID)
	{
		if (m_pParentNode) {
			m_pParentNode->DetachNode(this);
		}

		DetachAllNode();
		DetachAllEntity();
	}

	//
	// 获得名称
	//
	DWORD CSceneNode::GetName(VOID) const
	{
		return m_dwName;
	}

	//
	// 获得父节点
	//
	CSceneNode* CSceneNode::GetParentNode(VOID) const
	{
		return m_pParentNode;
	}

	//
	// 获得场景管理器
	//
	CSceneManager* CSceneNode::GetSceneManager(VOID) const
	{
		return m_pSceneManager;
	}

	//
	// 获得当前包围盒
	//
	const AABB* CSceneNode::GetAABBCurrent(VOID) const
	{
		return &m_aabbCurrent;
	}

	//
	// 获得级联包围盒
	//
	const AABB* CSceneNode::GetAABBCascade(VOID) const
	{
		return &m_aabbCascade;
	}

	//
	// 设置可见性
	//
	VOID CSceneNode::SetVisible(BOOL bVisible, BOOL bCascade)
	{
		//
		// 1. 设置当前节点可见性
		//
		m_bVisible = bVisible;

		//
		// 2. 设置子节点可见性
		//
		if (bCascade) {
			if (CSceneNode *pNode = m_pNodeHead) {
				do {
					pNode->SetVisible(bVisible, bCascade);
				} while (pNode = pNode->pNext);
			}
		}
	}

	//
	// 获得可见性
	//
	BOOL CSceneNode::IsVisible(VOID) const
	{
		if (m_bVisible) {
			if (m_pParentNode) {
				return m_pParentNode->IsVisible();
			}
			else {
				return TRUE;
			}
		}
		else {
			return FALSE;
		}
	}

	//
	// 设置可渲染性
	//
	VOID CSceneNode::SetRenderable(BOOL bNeedRender, BOOL bCascade)
	{
		//
		// 1. 设置当前节点可渲染性
		//
		m_bNeedRender = bNeedRender;

		//
		// 2. 设置子节点可渲染性
		//
		if (bCascade) {
			if (CSceneNode *pNode = m_pNodeHead) {
				do {
					pNode->SetRenderable(bNeedRender, bCascade);
				} while (pNode = pNode->pNext);
			}
		}
	}

	//
	// 获得可渲染性
	//
	BOOL CSceneNode::IsRenderable(VOID) const
	{
		return m_bNeedRender;
	}

	//
	// 挂入实体
	//
	BOOL CSceneNode::AttachEntity(CEntity *pEntity)
	{
		//
		// 1. 参数安全检查
		//
		if (pEntity == NULL) {
			return FALSE;
		}

		if (pEntity->m_pSceneNode != NULL) {
			return FALSE;
		}

		if (pEntity->m_pSceneManager != m_pSceneManager) {
			return FALSE;
		}

		EntityMap::const_iterator itEntity = m_entities.find(pEntity->m_dwName);

		if (itEntity != m_entities.end()) {
			return FALSE;
		}

		//
		// 2. 保存场景节点指针
		//
		pEntity->m_pSceneNode = this;

		//
		// 3. 添加到实体链表
		//
		pEntity->pNext = m_pEntityHead;
		m_pEntityHead = pEntity;

		//
		// 4. 添加到实体集合
		//
		m_entities[pEntity->m_dwName] = pEntity;

		//
		// 5. 标记更新AABB
		//
		m_bNeedUpdateAABB = TRUE;

		return TRUE;
	}

	//
	// 卸载实体
	//
	VOID CSceneNode::DetachEntity(CEntity *pEntity)
	{
		//
		// 1. 参数安全检查
		//
		if (pEntity == NULL) {
			return;
		}

		if (pEntity->m_pSceneNode != this) {
			return;
		}

		if (pEntity->m_pSceneManager != m_pSceneManager) {
			return;
		}

		EntityMap::const_iterator itEntity = m_entities.find(pEntity->m_dwName);

		if (itEntity == m_entities.end()) {
			return;
		}

		//
		// 2. 清空场景节点指针
		//
		pEntity->m_pSceneNode = NULL;

		//
		// 3. 从实体链表删除
		//
		if (m_pEntityHead == pEntity) {
			m_pEntityHead = pEntity->pNext;
		}
		else {
			CEntity *pEntityLast = m_pEntityHead;
			CEntity *pEntityCurrent = m_pEntityHead->pNext;

			do {
				if (pEntityCurrent == pEntity) {
					pEntityLast->pNext = pEntity->pNext;
					break;
				}

				pEntityLast = pEntityCurrent;
			} while (pEntityCurrent = pEntityCurrent->pNext);
		}

		pEntity->pNext = NULL;

		//
		// 4. 从实体集合删除
		//
		m_entities.erase(itEntity);

		//
		// 5. 标记更新AABB
		//
		m_bNeedUpdateAABB = TRUE;
	}

	//
	// 卸载所有实体
	//
	VOID CSceneNode::DetachAllEntity(VOID)
	{
		//
		// 1. 卸载所有实体
		//
		if (CEntity *pEntity = m_pEntityHead) {
			CEntity *pEntityNext = NULL;
			do {
				pEntityNext = pEntity->pNext;
				pEntity->pNext = NULL;
				pEntity->m_pSceneNode = NULL;
			} while (pEntity = pEntityNext);
		}

		m_entities.clear();
		m_pEntityHead = NULL;

		//
		// 2. 标记更新AABB
		//
		m_bNeedUpdateAABB = TRUE;
	}

	//
	// 查询挂入实体
	//
	CEntity* CSceneNode::QueryEntity(const CHAR *szName) const
	{
		ASSERT(szName);
		return QueryEntity(HashValue(szName));
	}

	//
	// 查询挂入实体
	//
	CEntity* CSceneNode::QueryEntity(DWORD dwName) const
	{
		EntityMap::const_iterator itEntity = m_entities.find(dwName);
		return itEntity != m_entities.end() ? itEntity->second : NULL;
	}

	//
	// 获得实体链表头
	//
	CEntity* CSceneNode::GetEntityListHead(VOID) const
	{
		return m_pEntityHead;
	}

	//
	// 挂入节点
	//
	BOOL CSceneNode::AttachNode(CSceneNode *pNode)
	{
		//
		// 1. 参数安全检查
		//
		if (pNode == this) {
			return FALSE;
		}

		if (pNode == NULL) {
			return FALSE;
		}

		if (pNode->m_pParentNode != NULL) {
			return FALSE;
		}

		if (pNode->m_pSceneManager != m_pSceneManager) {
			return FALSE;
		}

		SceneNodeMap::const_iterator itNode = m_nodes.find(pNode->m_dwName);

		if (itNode != m_nodes.end()) {
			return FALSE;
		}

		//
		// 2. 保存场景节点指针
		//
		pNode->m_pParentNode = this;

		//
		// 3. 添加到节点链表
		//
		pNode->pNext = m_pNodeHead;
		m_pNodeHead = pNode;

		//
		// 4. 添加到节点集合
		//
		m_nodes[pNode->m_dwName] = pNode;

		//
		// 5. 标记更新AABB
		//
		m_bNeedUpdateAABB = TRUE;

		return TRUE;
	}

	//
	// 卸载节点
	//
	VOID CSceneNode::DetachNode(CSceneNode *pNode)
	{
		//
		// 1. 参数安全检查
		//
		if (pNode == this) {
			return;
		}

		if (pNode == NULL) {
			return;
		}

		if (pNode->m_pParentNode != this) {
			return;
		}

		if (pNode->m_pSceneManager != m_pSceneManager) {
			return;
		}

		SceneNodeMap::const_iterator itNode = m_nodes.find(pNode->m_dwName);

		if (itNode == m_nodes.end()) {
			return;
		}

		//
		// 2. 清空场景节点指针
		//
		pNode->m_pParentNode = NULL;

		//
		// 3. 从节点链表删除
		//
		if (m_pNodeHead == pNode) {
			m_pNodeHead = pNode->pNext;
		}
		else {
			CSceneNode *pNodeLast = m_pNodeHead;
			CSceneNode *pNodeCurrent = m_pNodeHead->pNext;

			do {
				if (pNodeCurrent == pNode) {
					pNodeLast->pNext = pNode->pNext;
					break;
				}

				pNodeLast = pNodeCurrent;
			} while (pNodeCurrent = pNodeCurrent->pNext);
		}

		pNode->pNext = NULL;

		//
		// 4. 从节点集合删除
		//
		m_nodes.erase(itNode);

		//
		// 5. 标记更新AABB
		//
		m_bNeedUpdateAABB = TRUE;
	}

	//
	// 卸载所有节点
	//
	VOID CSceneNode::DetachAllNode(VOID)
	{
		//
		// 1. 卸载所有节点
		//
		if (CSceneNode *pNode = m_pNodeHead) {
			CSceneNode *pNodeNext = NULL;
			do {
				pNodeNext = pNode->pNext;
				pNode->pNext = NULL;
				pNode->m_pParentNode = NULL;
			} while (pNode = pNodeNext);
		}

		m_nodes.clear();
		m_pNodeHead = NULL;

		//
		// 2. 标记更新AABB
		//
		m_bNeedUpdateAABB = TRUE;
	}

	//
	// 查询子节点
	//
	CSceneNode* CSceneNode::QueryChildNode(const CHAR *szName) const
	{
		ASSERT(szName);
		return QueryChildNode(HashValue(szName));
	}

	//
	// 查询子节点
	//
	CSceneNode* CSceneNode::QueryChildNode(DWORD dwName) const
	{
		SceneNodeMap::const_iterator itNode = m_nodes.find(dwName);
		return itNode != m_nodes.end() ? itNode->second : NULL;
	}

	//
	// 获得节点链表头
	//
	CSceneNode* CSceneNode::GetNodeListHead(VOID) const
	{
		return m_pNodeHead;
	}

	//
	// 初始化
	//
	VOID CSceneNode::Identity(VOID)
	{
		m_bNeedUpdateAABB = TRUE;
		m_bNeedUpdateTransform = TRUE;

		Vec3Set(&m_localScale, 1.0f, 1.0f, 1.0f);
		Vec3Set(&m_localPosition, 0.0f, 0.0f, 0.0f);
		QuatSet(&m_localOrientation, 0.0f, 0.0f, 0.0f, 1.0f);
		MtxIdentity(&m_mtxLocal);
	}

	//
	// 设置本地缩放
	//
	VOID CSceneNode::SetLocalScale(FLOAT x, FLOAT y, FLOAT z)
	{
		m_bNeedUpdateAABB = TRUE;
		m_bNeedUpdateTransform = TRUE;

		Vec3Set(&m_localScale, x, y, z);
	}

	//
	// 设置本地位置
	//
	VOID CSceneNode::SetLocalPosition(FLOAT x, FLOAT y, FLOAT z)
	{
		m_bNeedUpdateAABB = TRUE;
		m_bNeedUpdateTransform = TRUE;

		Vec3Set(&m_localPosition, x, y, z);
	}

	//
	// 设置本地朝向
	//
	VOID CSceneNode::SetLocalOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w)
	{
		m_bNeedUpdateAABB = TRUE;
		m_bNeedUpdateTransform = TRUE;

		QuatSet(&m_localOrientation, x, y, z, w);
	}

	//
	// 设置本地朝向
	//
	VOID CSceneNode::SetLocalDirection(FLOAT x, FLOAT y, FLOAT z, FLOAT upx, FLOAT upy, FLOAT upz)
	{
		m_bNeedUpdateAABB = TRUE;
		m_bNeedUpdateTransform = TRUE;

		VEC3 up;
		VEC3 direction;
		Vec3Set(&up, upx, upy, upz);
		Vec3Set(&direction, x, y, z);
		QuatDefLookAt(&m_localOrientation, &direction, &up);
	}

	//
	// 设置世界缩放
	//
	VOID CSceneNode::SetWorldScale(FLOAT x, FLOAT y, FLOAT z)
	{
		if (m_pParentNode) {
			// 算法:
			// worldScale = localScale * parentScale
			// localScale = worldScale / parentScale

			VEC3 localScale;
			VEC3 worldScale;
			Vec3Set(&worldScale, x, y, z);
			Vec3Div(&localScale, &worldScale, m_pParentNode->GetWorldScale());

			x = localScale[0];
			y = localScale[1];
			z = localScale[2];
		}

		SetLocalScale(x, y, z);
	}

	//
	// 设置世界位置
	//
	VOID CSceneNode::SetWorldPosition(FLOAT x, FLOAT y, FLOAT z)
	{
		if (m_pParentNode) {
			// 算法:
			// worldPosition = localPosition * parentMatrix;
			// localPosition = worldPosition * parentMatrixInv;

			MATRIX4 mtxParentInv;
			MtxInverse(&mtxParentInv, m_pParentNode->GetWorldMatrix());

			VEC3 localPosition;
			VEC3 worldPosition;
			Vec3Set(&worldPosition, x, y, z);
			Vec3MulMtx4x4(&localPosition, &worldPosition, &mtxParentInv);

			x = localPosition[0];
			y = localPosition[1];
			z = localPosition[2];
		}

		SetLocalPosition(x, y, z);
	}

	//
	// 设置世界朝向
	//
	VOID CSceneNode::SetWorldOrientation(FLOAT x, FLOAT y, FLOAT z, FLOAT w)
	{
		if (m_pParentNode) {
			// 算法:
			// worldOrientation = localOrientation * parentOrientation
			// localOrientation = worldOrientation * parentOrientationInv

			QUAT parentOrientationInv;
			QuatInverse(&parentOrientationInv, m_pParentNode->GetWorldOrientation());

			QUAT localOrientation;
			QUAT worldOrientation;
			QuatSet(&worldOrientation, x, y, z, w);
			QuatMul(&localOrientation, &worldOrientation, &parentOrientationInv);

			x = localOrientation[0];
			y = localOrientation[1];
			z = localOrientation[2];
			w = localOrientation[3];
		}

		SetLocalOrientation(x, y, z, w);
	}

	//
	// 设置本地朝向
	//
	VOID CSceneNode::SetWorldDirection(FLOAT x, FLOAT y, FLOAT z, FLOAT upx, FLOAT upy, FLOAT upz)
	{
		VEC3 up;
		VEC3 direction;
		QUAT worldOrientation;
		Vec3Set(&up, upx, upy, upz);
		Vec3Set(&direction, x, y, z);
		QuatDefLookAt(&worldOrientation, &direction, &up);

		SetWorldOrientation(worldOrientation[0], worldOrientation[1], worldOrientation[2], worldOrientation[3]);
	}

	//
	// 获得本地缩放
	//
	const VEC3* CSceneNode::GetLocalScale(VOID)
	{
		return &m_localScale;
	}

	//
	// 获得本地位置
	//
	const VEC3* CSceneNode::GetLocalPosition(VOID)
	{
		return &m_localPosition;
	}

	//
	// 获得本地朝向
	//
	const QUAT* CSceneNode::GetLocalOrientation(VOID)
	{
		return &m_localOrientation;
	}

	//
	// 获得本地变换矩阵
	//
	const MATRIX4* CSceneNode::GetLocalMatrix(VOID)
	{
		return &m_mtxLocal;
	}

	//
	// 获得世界缩放
	//
	const VEC3* CSceneNode::GetWorldScale(VOID)
	{
		UpdateTransformImmediately();
		return &m_worldScale;
	}

	//
	// 获得世界位置
	//
	const VEC3* CSceneNode::GetWorldPosition(VOID)
	{
		UpdateTransformImmediately();
		return &m_worldPosition;
	}

	//
	// 获得世界朝向
	//
	const QUAT* CSceneNode::GetWorldOrientation(VOID)
	{
		UpdateTransformImmediately();
		return &m_worldOrientation;
	}

	//
	// 获得世界变换矩阵
	//
	const MATRIX4* CSceneNode::GetWorldMatrix(VOID)
	{
		UpdateTransformImmediately();
		return &m_mtxWorld;
	}

	//
	// 立即更新变换
	// 说明: 自下而上更新
	//
	VOID CSceneNode::UpdateTransformImmediately(VOID)
	{
		//
		// 1. 更新父节点变换
		//
		if (m_pParentNode) {
			m_pParentNode->UpdateTransformImmediately();
		}

		//
		// 2. 更新当前节点变换矩阵
		//
		if (m_bNeedUpdateTransform || (m_pParentNode && m_pParentNode->m_dwUpdateTransformCount != m_dwParentUpdateTransformCount)) {
			if (m_bNeedUpdateTransform) {
				m_bNeedUpdateTransform = FALSE;

				MATRIX4 mtxScale;
				MATRIX4 mtxRotate;
				MATRIX4 mtxTranslate;
				MATRIX4 mtxScaleRotate;

				MtxDefScale(&mtxScale, m_localScale[0], m_localScale[1], m_localScale[2]);
				MtxDefTranslate(&mtxTranslate, m_localPosition[0], m_localPosition[1], m_localPosition[2]);
				QuatToMtxRotation(&mtxRotate, &m_localOrientation);

				// 级联顺序: scale * rotate * translate
				MtxMul(&mtxScaleRotate, &mtxScale, &mtxRotate);
				MtxMul(&m_mtxLocal, &mtxScaleRotate, &mtxTranslate);
			}

			if (m_pParentNode) {
				MtxMul(&m_mtxWorld, &m_mtxLocal, &m_pParentNode->m_mtxWorld);
			}
			else {
				MtxCopy(&m_mtxWorld, &m_mtxLocal);
			}

			MtxToScale(&m_worldScale, &m_mtxWorld);
			MtxToQuat(&m_worldOrientation, &m_mtxWorld);
			MtxToTranslate(&m_worldPosition, &m_mtxWorld);

			m_dwUpdateTransformCount++;
			m_dwParentUpdateTransformCount = m_pParentNode ? m_pParentNode->m_dwUpdateTransformCount : 0;
		}
	}

	//
	// 更新变换
	// 说明: 自上而下更新
	//
	VOID CSceneNode::UpdateTransform(BOOL bParentUpdate)
	{
		BOOL bUpdate = bParentUpdate || m_bNeedUpdateTransform ? TRUE : FALSE;

		//
		// 1. 更新当前节点变换矩阵
		//
		if (bUpdate) {
			if (m_bNeedUpdateTransform) {
				m_bNeedUpdateTransform = FALSE;

				MATRIX4 mtxScale;
				MATRIX4 mtxRotate;
				MATRIX4 mtxTranslate;
				MATRIX4 mtxScaleRotate;

				MtxDefScale(&mtxScale, m_localScale[0], m_localScale[1], m_localScale[2]);
				MtxDefTranslate(&mtxTranslate, m_localPosition[0], m_localPosition[1], m_localPosition[2]);
				QuatToMtxRotation(&mtxRotate, &m_localOrientation);

				// 级联顺序: scale * rotate * translate
				MtxMul(&mtxScaleRotate, &mtxScale, &mtxRotate);
				MtxMul(&m_mtxLocal, &mtxScaleRotate, &mtxTranslate);
			}

			if (m_pParentNode) {
				MtxMul(&m_mtxWorld, &m_mtxLocal, &m_pParentNode->m_mtxWorld);
			}
			else {
				MtxCopy(&m_mtxWorld, &m_mtxLocal);
			}

			MtxToScale(&m_worldScale, &m_mtxWorld);
			MtxToQuat(&m_worldOrientation, &m_mtxWorld);
			MtxToTranslate(&m_worldPosition, &m_mtxWorld);

			m_dwUpdateTransformCount++;
			m_dwParentUpdateTransformCount = m_pParentNode ? m_pParentNode->m_dwUpdateTransformCount : 0;
		}

		//
		// 2. 更新子节点变换
		//
		if (CSceneNode *pNode = m_pNodeHead) {
			do {
				pNode->UpdateTransform(bUpdate);
			} while (pNode = pNode->pNext);
		}
	}

	//
	// 更新包围盒
	//
	BOOL CSceneNode::UpdateAABB(BOOL bParentUpdate)
	{
		BOOL bUpdate = bParentUpdate || m_bNeedUpdateAABB ? TRUE : FALSE;

		//
		// 1. 可见性检查
		//
		if (m_bVisible == FALSE) {
			return FALSE;
		}

		//
		// 2. 初始化包围盒
		//
		BOOL bAABBCurrentValid = FALSE;
		BOOL bAABBCascadeValid = FALSE;

		Vec3Set(&m_aabbCurrent.minVertex,  FLT_MAX,  FLT_MAX,  FLT_MAX);
		Vec3Set(&m_aabbCurrent.maxVertex, -FLT_MAX, -FLT_MAX, -FLT_MAX);
		Vec3Set(&m_aabbCascade.minVertex,  FLT_MAX,  FLT_MAX,  FLT_MAX);
		Vec3Set(&m_aabbCascade.maxVertex, -FLT_MAX, -FLT_MAX, -FLT_MAX);

		//
		// 3. 更新当前节点包围盒
		//
		if (bUpdate) {
			if (CEntity *pEntity = m_pEntityHead) {
				do {
					if (pEntity->IsRenderable()) {
						const AABB *pAABB = (pEntity->GetType() == CEntity::LIGHT) ? 
							((CEntityLight*)pEntity)->GetAABB() : ((CEntityRenderable*)pEntity)->GetAABB();

						if (IsAABBValid(pAABB)) {
							bAABBCurrentValid = TRUE;
							if (m_aabbCurrent.minVertex[0] > pAABB->minVertex[0]) m_aabbCurrent.minVertex[0] = pAABB->minVertex[0];
							if (m_aabbCurrent.minVertex[1] > pAABB->minVertex[1]) m_aabbCurrent.minVertex[1] = pAABB->minVertex[1];
							if (m_aabbCurrent.minVertex[2] > pAABB->minVertex[2]) m_aabbCurrent.minVertex[2] = pAABB->minVertex[2];
							if (m_aabbCurrent.maxVertex[0] < pAABB->maxVertex[0]) m_aabbCurrent.maxVertex[0] = pAABB->maxVertex[0];
							if (m_aabbCurrent.maxVertex[1] < pAABB->maxVertex[1]) m_aabbCurrent.maxVertex[1] = pAABB->maxVertex[1];
							if (m_aabbCurrent.maxVertex[2] < pAABB->maxVertex[2]) m_aabbCurrent.maxVertex[2] = pAABB->maxVertex[2];
						}
					}
				} while (pEntity = pEntity->pNext);

				if (bAABBCurrentValid) {
					AABBNormalize(&m_aabbCurrent);
					Vec3Copy(&m_aabbCascade.minVertex, &m_aabbCurrent.minVertex);
					Vec3Copy(&m_aabbCascade.maxVertex, &m_aabbCurrent.maxVertex);
				}
				else {
					AABBZero(&m_aabbCurrent);
				}
			}
		}

		//
		// 4. 更新子节点包围盒
		//
		if (CSceneNode *pNode = m_pNodeHead) {
			do {
				if (pNode->UpdateAABB(bUpdate)) {
					if (IsAABBValid(&pNode->m_aabbCascade)) {
						bAABBCascadeValid = TRUE;
						if (m_aabbCascade.minVertex[0] > pNode->m_aabbCascade.minVertex[0]) m_aabbCascade.minVertex[0] = pNode->m_aabbCascade.minVertex[0];
						if (m_aabbCascade.minVertex[1] > pNode->m_aabbCascade.minVertex[1]) m_aabbCascade.minVertex[1] = pNode->m_aabbCascade.minVertex[1];
						if (m_aabbCascade.minVertex[2] > pNode->m_aabbCascade.minVertex[2]) m_aabbCascade.minVertex[2] = pNode->m_aabbCascade.minVertex[2];
						if (m_aabbCascade.maxVertex[0] < pNode->m_aabbCascade.maxVertex[0]) m_aabbCascade.maxVertex[0] = pNode->m_aabbCascade.maxVertex[0];
						if (m_aabbCascade.maxVertex[1] < pNode->m_aabbCascade.maxVertex[1]) m_aabbCascade.maxVertex[1] = pNode->m_aabbCascade.maxVertex[1];
						if (m_aabbCascade.maxVertex[2] < pNode->m_aabbCascade.maxVertex[2]) m_aabbCascade.maxVertex[2] = pNode->m_aabbCascade.maxVertex[2];
					}
				}
			} while (pNode = pNode->pNext);

			if (bAABBCurrentValid || bAABBCascadeValid) {
				AABBNormalize(&m_aabbCascade);
			}
			else {
				AABBZero(&m_aabbCascade);
			}
		}

		//
		// 5. 恢复更新AABB
		//
		m_bNeedUpdateAABB = FALSE;

		return TRUE;
	}

	//
	// 更新实体
	//
	VOID CSceneNode::UpdateEntity(FLOAT deltaTime)
	{
		//
		// 1. 可见性检查
		//
		if (m_bVisible == FALSE) {
			return;
		}

		//
		// 2. 更新当前实体集合
		//
		if (CEntity *pEntity = m_pEntityHead) {
			do {
				pEntity->Update(deltaTime);
			} while (pEntity = pEntity->pNext);
		}

		//
		// 3. 更新子节点实体集合
		//
		if (CSceneNode *pNode = m_pNodeHead) {
			do {
				pNode->UpdateEntity(deltaTime);
			} while (pNode = pNode->pNext);
		}
	}

	//
	// 更新渲染队列
	//
	VOID CSceneNode::UpdateRenderQuene(const CEntityCamera *pCamera)
	{
		ASSERT(pCamera);

		//
		// 1. 可见性检查
		//
		if (m_bVisible == FALSE) {
			return;
		}

		//
		// 2. 更新当前更新渲染队列
		//
		if (m_bNeedRender) {
			if (CEntity *pEntity = m_pEntityHead) {
				do {
					pEntity->UpdateRenderQuene(pCamera);
				} while (pEntity = pEntity->pNext);
			}
		}

		//
		// 3. 更新子节点更新渲染队列
		//
		if (CSceneNode *pNode = m_pNodeHead) {
			do {
				pNode->UpdateRenderQuene(pCamera);
			} while (pNode = pNode->pNext);
		}
	}

}

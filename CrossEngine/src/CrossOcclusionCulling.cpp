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

	COcclusionCulling::COcclusionCulling(CSceneManager *pSceneManager)
		: m_pSceneManager(pSceneManager)
		, m_pvs(NULL)
	{
		Init();
	}

	COcclusionCulling::~COcclusionCulling(VOID)
	{
		Free();
	}

	//
	// 初始化
	//
	VOID COcclusionCulling::Init(VOID)
	{
		m_pvs = NULL;
		m_volumes.clear();
	}

	//
	// 释放
	//
	VOID COcclusionCulling::Free(VOID)
	{

	}

	//
	// 从数据流加载资源
	//
	BOOL COcclusionCulling::LoadFromStream(CStream *pStream)
	{
		return TRUE;
	}

	//
	// 遮挡判断
	//
	BOOL COcclusionCulling::IsOccluded(INT index) const
	{
		static const BYTE offsetbits[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

		if (index == -1) {
			return FALSE;
		}

		if (m_pvs == NULL) {
			return FALSE;
		}

		return (m_pvs[index / 8] & offsetbits[index % 8]) ? FALSE : TRUE;
	}

	//
	// 更新剔除
	//
	VOID COcclusionCulling::UpdateCulling(const CEntityCamera *pCamera) const
	{
		ASSERT(pCamera);

		//
		// 1. 初始化当前潜在可视集
		//
		const_cast<BYTE*>(m_pvs) = NULL;

		//
		// 2. 定位当前潜在可视集
		//
		const VEC3 *position = pCamera->GetPosition();

		for (VisibleVolumeSet::const_iterator itVolume = m_volumes.begin(); itVolume != m_volumes.end(); ++itVolume) {
			const VISIBLE_VOLUME *pVolume = *itVolume;
			ASSERT(pVolume);

			if (IsPointInAABB(&pVolume->aabb, (*position)[0], (*position)[1], (*position)[2])) {
				INT x = FastFloor(((*position)[0] - pVolume->aabb.minVertex[0]) / pVolume->stepx);
				INT y = FastFloor(((*position)[1] - pVolume->aabb.minVertex[1]) / pVolume->stepy);
				INT z = FastFloor(((*position)[2] - pVolume->aabb.minVertex[2]) / pVolume->stepz);

				const_cast<BYTE*>(m_pvs) = pVolume->pppCells[x][y][z].pvs;
				return;
			}
		}
	}

}

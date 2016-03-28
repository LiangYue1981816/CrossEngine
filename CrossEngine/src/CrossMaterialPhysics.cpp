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

	CMatPhysics::CMatPhysics(CMaterial *pMaterial)
		: m_pParentMaterial(pMaterial)
	{
		Init();
	}

	CMatPhysics::~CMatPhysics(VOID)
	{
		Free();
	}

	//
	// 初始化资源
	//
	VOID CMatPhysics::Init(VOID)
	{
		m_type = MATERIAL_TYPE_DEFAULT;
	}

	//
	// 释放资源
	//
	VOID CMatPhysics::Free(VOID)
	{
		Init();
	}

	//
	// 获得父材质
	//
	const CMaterial* CMatPhysics::GetParentMaterial(VOID) const
	{
		return m_pParentMaterial;
	}

	//
	// 设置类型
	//
	VOID CMatPhysics::SetType(MATERIAL_TYPE type)
	{
		m_type = type;
	}

	//
	// 获得类型
	//
	MATERIAL_TYPE CMatPhysics::GetType(VOID) const
	{
		return m_type;
	}

	//
	// 加载XML
	//
	VOID CMatPhysics::LoadXML(TiXmlNode *pPhysicsNode)
	{
		ASSERT(pPhysicsNode);

		//
		// 1. 释放物理材质
		//
		Free();

		//
		// 2. 加载物理材质
		//
		if (TiXmlNode *pPhysicsTypeNode = pPhysicsNode->FirstChild("PhysicsType")) {
			SetType((MATERIAL_TYPE)pPhysicsTypeNode->ToElement()->AttributeInt("type"));
		}
	}

	//
	// 保存XML
	//
	VOID CMatPhysics::SaveXML(TiXmlNode *pPhysicsNode) const
	{
		ASSERT(pPhysicsNode);
		static const CMatPhysics defaultPhysics(NULL);

		if (m_type != defaultPhysics.m_type) {
			TiXmlElement *pPhysicsTypeNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("PhysicsType"); ASSERT(pPhysicsTypeNode);
			{
				pPhysicsTypeNode->SetAttributeInt("type", (int)m_type);
			}
			pPhysicsNode->LinkEndChild(pPhysicsTypeNode);
		}
	}

}

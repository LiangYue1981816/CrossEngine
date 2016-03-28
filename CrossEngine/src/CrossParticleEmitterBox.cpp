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

	CEmitterBox::CEmitterBox(CParticleSystem *pParent)
		: CEmitterBase(pParent)
	{
		Init();
	}

	CEmitterBox::~CEmitterBox(VOID)
	{
		Free();
	}

	//
	// �������
	//
	CEmitterBase::TYPE CEmitterBox::GetType(VOID) const
	{
		return CEmitterBase::BOX;
	}

	//
	// ��ʼ��
	//
	VOID CEmitterBox::Init(VOID)
	{
		m_width = 1.0f;
		m_height = 1.0f;
		m_length = 1.0f;

		CEmitterBase::Init();
	}

	//
	// �ͷ�
	//
	VOID CEmitterBox::Free(VOID)
	{
		CEmitterBase::Free();
	}

	//
	// ����XML
	//
	VOID CEmitterBox::LoadXML(TiXmlNode *pEmitterNode)
	{
		ASSERT(pEmitterNode);

		if (TiXmlNode *pWidthNode = pEmitterNode->FirstChild("Width")) {
			SetWidth(pWidthNode->ToElement()->AttributeFloat("value"));
		}

		if (TiXmlNode *pHeightNode = pEmitterNode->FirstChild("Height")) {
			SetHeight(pHeightNode->ToElement()->AttributeFloat("value"));
		}

		if (TiXmlNode *pLengthNode = pEmitterNode->FirstChild("Length")) {
			SetLength(pLengthNode->ToElement()->AttributeFloat("value"));
		}

		CEmitterBase::LoadXML(pEmitterNode);
	}

	//
	// ����XML
	//
	VOID CEmitterBox::SaveXML(TiXmlNode *pEmitterNode) const
	{
		ASSERT(pEmitterNode);

		TiXmlElement *pWidthNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Width"); ASSERT(pWidthNode);
		{
			pWidthNode->SetAttributeFloat("value", m_width);
		}
		pEmitterNode->LinkEndChild(pWidthNode);

		TiXmlElement *pHeightNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Height"); ASSERT(pHeightNode);
		{
			pHeightNode->SetAttributeFloat("value", m_height);
		}
		pEmitterNode->LinkEndChild(pHeightNode);

		TiXmlElement *pLengthNode = SAFE_NEW(MEMTYPE_HEAP) TiXmlElement("Length"); ASSERT(pLengthNode);
		{
			pLengthNode->SetAttributeFloat("value", m_length);
		}
		pEmitterNode->LinkEndChild(pLengthNode);

		CEmitterBase::SaveXML(pEmitterNode);
	}

	//
	// ��������
	//
	BOOL CEmitterBox::Emit(CParticle *pParticle)
	{
		VEC3 localPosition;
		Vec3Set(&localPosition,
			m_width * (FRandomValue() - 0.5f),
			m_height * (FRandomValue() - 0.5f), 
			m_length * (FRandomValue() - 0.5f));
		return CEmitterBase::Emit(pParticle, &localPosition);
	}

	//
	// ���úп�
	//
	VOID CEmitterBox::SetWidth(FLOAT width)
	{
		m_width = width;
	}

	//
	// ���úи�
	//
	VOID CEmitterBox::SetHeight(FLOAT height)
	{
		m_height = height;
	}

	//
	// ���úг�
	//
	VOID CEmitterBox::SetLength(FLOAT length)
	{
		m_length = length;
	}

	//
	// ��úп�
	//
	FLOAT CEmitterBox::GetWidth(VOID) const
	{
		return m_width;
	}

	//
	// ��úи�
	//
	FLOAT CEmitterBox::GetHeight(VOID) const
	{
		return m_height;
	}

	//
	// ��úг�
	//
	FLOAT CEmitterBox::GetLength(VOID) const
	{
		return m_length;
	}

}

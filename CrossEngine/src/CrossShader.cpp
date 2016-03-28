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

	CShader::CShader(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{
		Init();
	}

	CShader::~CShader(VOID)
	{
		Free();
	}

	//
	// ��ʼ����Դ
	//
	VOID CShader::Init(VOID)
	{
		m_shader = 0;
		m_params.clear();

		CResource::Init();
	}

	//
	// �ͷ���Դ
	//
	VOID CShader::Free(VOID)
	{
		Renderer()->DestroyShader(&m_shader);
		CResource::Free();
	}

	//
	// �������
	//
	CResource::TYPE CShader::GetType(VOID) const
	{
		return CResource::SHADER;
	}

	//
	// ��Ч�Լ��
	//
	BOOL CShader::IsValid(VOID) const
	{
		return m_shader > 0 ? TRUE : FALSE;
	}

	//
	// ������Դ
	// ע��: ��ֹ������Դ!!!
	//
	BOOL CShader::CopyFrom(const CResource *pCopyFrom)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//
	// ���ļ�������Դ
	//
	BOOL CShader::LoadFromFile(const CHAR *szFileName)
	{
		//
		// 1. ������ɫ��
		//
		BOOL rcode = CResource::LoadFromFile(szFileName);

		//
		// 2. �ͷ�������
		//
		m_stream.FreeData();

		return rcode;
	}

	//
	// ��ѹ����������Դ
	//
	BOOL CShader::LoadFromZip(const CHAR *szZipName, const CHAR *szFileName)
	{
		//
		// 1. ������ɫ��
		//
		BOOL rcode = CResource::LoadFromZip(szZipName, szFileName);

		//
		// 2. �ͷ�������
		//
		m_stream.FreeData();

		return rcode;
	}

	//
	// ��ѹ�����ļ�����������
	//
	BOOL CShader::LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName)
	{
		//
		// 1. ������ɫ��
		//
		BOOL rcode = CResource::LoadFromZip(pZipPack, szFileName);

		//
		// 2. �ͷ�������
		//
		m_stream.FreeData();

		return rcode;
	}

	//
	// ��������������Դ
	//
	BOOL CShader::LoadFromStream(CStream *pStream)
	{
		//
		// 1. ��������Ч�Լ��
		//
		if (pStream == NULL) {
			return FALSE;
		}

		if (pStream->IsValid() == FALSE) {
			return FALSE;
		}

		//
		// 2. �ͷ���ɫ��
		//
		Free();

		//
		// 3. ������ɫ��
		//
		try {
			//
			// 3.1. ��XML
			//
			TiXmlDocument doc;
			if (doc.LoadFile((CHAR *)pStream->GetCurrentAddress(), pStream->GetFreeSize()) == FALSE) {
				throw "Invalid xml format.";
			}

			//
			// 3.2. ��ýڵ�
			//
			TiXmlNode *pShaderNode = doc.FirstChild("Shader");
			if (pShaderNode == NULL) throw "Invalid shader format.";

			TiXmlNode *pSourceNode = pShaderNode->FirstChild("Source");
			TiXmlNode *pParamsNode = pShaderNode->FirstChild("Parameters");
			if (pSourceNode == NULL) throw "Invalid shader format.";

			//
			// 3.3. �����ɫ������
			//
			UINT type = 0;
			const CHAR *szType = pShaderNode->ToElement()->AttributeString("type");
			if      (szType && stricmp(szType, "vertex") == 0) type = GL_VERTEX_SHADER;
			else if (szType && stricmp(szType, "fragment") == 0) type = GL_FRAGMENT_SHADER;
			else throw "Invalid shader type.";

			//
			// 3.4. �����ɫ������
			//
			const CHAR *source = pSourceNode->ToElement()->GetText();
			if (source == NULL) throw "Invalid shader source code.";

			//
			// 3.5. �����ɫ������
			//
			if (pParamsNode) {
				if (TiXmlNode *pParamNode = pParamsNode->FirstChild()) {
					do {
						const CHAR *type = pParamNode->Value();
						const CHAR *name = pParamNode->ToElement()->AttributeString("name");
						const CHAR *value = pParamNode->ToElement()->AttributeString("value");
						if (type == NULL || name == NULL || value == NULL) throw "Invalid shader param format.";
						if (strlen(type) == 0) throw "Invalid shader param type.";
						if (strlen(name) == 0) throw "Invalid shader param name.";
						if (strlen(value) == 0) throw "Invalid shader param value.";

						CRenderer::UNIFORM_LOCATION param;
						memset(&param, 0, sizeof(param));

						strcpy(param.name, name);
						param.location = -1;

						if      (stricmp(type, "Uniform1i") == 0) { param.type = CRenderer::UNIFORM_TYPE_INT; sscanf(value, "%d", &param.value.nvalue[0]); }
						else if (stricmp(type, "Uniform2i") == 0) { param.type = CRenderer::UNIFORM_TYPE_INT2; sscanf(value, "%d %d", &param.value.nvalue[0], &param.value.nvalue[1]); }
						else if (stricmp(type, "Uniform3i") == 0) { param.type = CRenderer::UNIFORM_TYPE_INT3; sscanf(value, "%d %d %d", &param.value.nvalue[0], &param.value.nvalue[1], &param.value.nvalue[2]); }
						else if (stricmp(type, "Uniform4i") == 0) { param.type = CRenderer::UNIFORM_TYPE_INT4; sscanf(value, "%d %d %d %d", &param.value.nvalue[0], &param.value.nvalue[1], &param.value.nvalue[2], &param.value.nvalue[3]); }
						else if (stricmp(type, "Uniform1f") == 0) { param.type = CRenderer::UNIFORM_TYPE_FLOAT; sscanf(value, "%f", &param.value.fvalue[0]); }
						else if (stricmp(type, "Uniform2f") == 0) { param.type = CRenderer::UNIFORM_TYPE_FLOAT2; sscanf(value, "%f %f", &param.value.fvalue[0], &param.value.fvalue[1]); }
						else if (stricmp(type, "Uniform3f") == 0) { param.type = CRenderer::UNIFORM_TYPE_FLOAT3; sscanf(value, "%f %f %f", &param.value.fvalue[0], &param.value.fvalue[1], &param.value.fvalue[2]); }
						else if (stricmp(type, "Uniform4f") == 0) { param.type = CRenderer::UNIFORM_TYPE_FLOAT4; sscanf(value, "%f %f %f %f", &param.value.fvalue[0], &param.value.fvalue[1], &param.value.fvalue[2], &param.value.fvalue[3]); }
						else if (stricmp(type, "UniformMatrix2x2") == 0) { param.type = CRenderer::UNIFORM_TYPE_MATRIX2; sscanf(value, "%f %f %f %f", &param.value.fvalue[0], &param.value.fvalue[1], &param.value.fvalue[2], &param.value.fvalue[3]); }
						else if (stricmp(type, "UniformMatrix3x3") == 0) { param.type = CRenderer::UNIFORM_TYPE_MATRIX3; sscanf(value, "%f %f %f %f %f %f %f %f %f", &param.value.fvalue[0], &param.value.fvalue[1], &param.value.fvalue[2], &param.value.fvalue[3], &param.value.fvalue[4], &param.value.fvalue[5], &param.value.fvalue[6], &param.value.fvalue[7], &param.value.fvalue[8]); }
						else if (stricmp(type, "UniformMatrix4x4") == 0) { param.type = CRenderer::UNIFORM_TYPE_MATRIX4; sscanf(value, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", &param.value.fvalue[0], &param.value.fvalue[1], &param.value.fvalue[2], &param.value.fvalue[3], &param.value.fvalue[4], &param.value.fvalue[5], &param.value.fvalue[6], &param.value.fvalue[7], &param.value.fvalue[8], &param.value.fvalue[9], &param.value.fvalue[10], &param.value.fvalue[11], &param.value.fvalue[12], &param.value.fvalue[13], &param.value.fvalue[14], &param.value.fvalue[15]); }
						else throw "Invalid shader param type.";

						m_params[HashValue(name)] = param;
					} while (pParamNode = pParamsNode->IterateChildren(pParamNode));
				}
			}

			//
			// 3.6. ������ɫ��
			//
			m_shader = Renderer()->CreateShader(type, source);

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CShader::LoadFromStream: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// �����ɫ��
	//
	UINT CShader::GetShaderGL(VOID) const
	{
		return m_shader;
	}

	//
	// ��ò�������
	//
	const CRenderer::UniformLocationMap& CShader::GetParams(VOID) const
	{
		return m_params;
	}

}

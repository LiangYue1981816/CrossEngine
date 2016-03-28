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

	CTexture::CTexture(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{
		Init();
	}

	CTexture::~CTexture(VOID)
	{
		Free();
	}

	//
	// ��ʼ����Դ
	//
	VOID CTexture::Init(VOID)
	{
		m_type = UNKNOW;

		m_width = 0;
		m_height = 0;
		m_bitcount = 0;
		m_mipmaps = 0;
		m_ddsFormat = 0;
		m_pvrFormat = 0;
		m_pData = NULL;

		m_target = GL_TEXTURE_2D;
		m_texture = 0;

		CResource::Init();
	}

	//
	// �ͷ���Դ
	//
	VOID CTexture::Free(VOID)
	{
		Renderer()->DestroyTexture(&m_texture);
		CResource::Free();
	}

	//
	// �������
	//
	CResource::TYPE CTexture::GetType(VOID) const
	{
		return CResource::TEXTURE;
	}

	//
	// ��Ч�Լ��
	//
	BOOL CTexture::IsValid(VOID) const
	{
		return m_texture > 0 ? TRUE : FALSE;
	}

	//
	// ������Դ
	// ע��: ��ֹ������Դ!!!
	//
	BOOL CTexture::CopyFrom(const CResource *pCopyFrom)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//
	// ���ļ�������Դ
	//
	BOOL CTexture::LoadFromFile(const CHAR *szFileName)
	{
		//
		// 1. ��������
		//
		BOOL rcode = CResource::LoadFromFile(szFileName);

		//
		// 2. �ͷ�������
		//
		m_pData = NULL;
		m_stream.FreeData();

		return rcode;
	}

	//
	// ��ѹ����������Դ
	//
	BOOL CTexture::LoadFromZip(const CHAR *szZipName, const CHAR *szFileName)
	{
		//
		// 1. ��������
		//
		BOOL rcode = CResource::LoadFromZip(szZipName, szFileName);

		//
		// 2. �ͷ�������
		//
		m_pData = NULL;
		m_stream.FreeData();

		return rcode;
	}

	//
	// ��ѹ����������Դ
	//
	BOOL CTexture::LoadFromZip(ZZIP_DIR *pZipPack, const CHAR *szFileName)
	{
		//
		// 1. ��������
		//
		BOOL rcode = CResource::LoadFromZip(pZipPack, szFileName);

		//
		// 2. �ͷ�������
		//
		m_pData = NULL;
		m_stream.FreeData();

		return rcode;
	}

	//
	// ��������������Դ
	//
	BOOL CTexture::LoadFromStream(CStream *pStream)
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
		// 2. �ͷ�����
		//
		Free();

		//
		// 3. ��������
		//
		try {
			//
			// 3.1. ��������
			//
			LoadTexture(pStream);

			//
			// 3.2. ��������
			//
			CreateTexture();

			return TRUE;
		}
		catch (const CHAR *szError) {
			WriteLogE("Error CTexture::LoadFromStream: %s %s %d\n", szError, __FILE__, __LINE__);
			Free();

			return FALSE;
		}
	}

	//
	// ����BMP����
	//
	VOID CTexture::LoadBMP(CStream *pStream)
	{
		ASSERT(pStream);
		ASSERT(pStream->IsValid());

		//
		// 1. ��¼��ǰ������λ��
		//
		DWORD dwOffset = pStream->GetCurrentPosition();

		//
		// 2. ���������ļ�ͷ
		//
		pStream->ReadWORD();  // Type
		pStream->ReadDWORD(); // Size
		pStream->ReadWORD();  // Reserved1
		pStream->ReadWORD();  // Reserved2
		DWORD dwDataOffset = pStream->ReadDWORD();

		//
		// 3. ����������Ϣͷ
		//
		BITMAPINFOHEADER bmiHeader;
		memset(&bmiHeader, 0, sizeof(bmiHeader));

		bmiHeader.biSize = pStream->ReadDWORD();
		bmiHeader.biWidth = pStream->ReadDWORD();
		bmiHeader.biHeight = pStream->ReadDWORD();
		bmiHeader.biPlanes = pStream->ReadWORD();
		bmiHeader.biBitCount = pStream->ReadWORD();
		bmiHeader.biCompression = pStream->ReadDWORD();
		bmiHeader.biSizeImage = pStream->ReadDWORD();
		bmiHeader.biXPelsPerMeter = pStream->ReadDWORD();
		bmiHeader.biYPelsPerMeter = pStream->ReadDWORD();
		bmiHeader.biClrUsed = pStream->ReadDWORD();
		bmiHeader.biClrImportant = pStream->ReadDWORD();

		//
		// 4. �����ʽ���
		//    ע��: ֻ֧��8Bit,24Bit,32Bit��ѹ������
		//
		if (bmiHeader.biBitCount != 8 &&
			bmiHeader.biBitCount != 24 &&
			bmiHeader.biBitCount != 32) {
			throw "Bitmap not support bitcount.";
		}

		if (bmiHeader.biCompression != BI_RGB &&
			bmiHeader.biCompression != BI_BITFIELDS) {
			throw "Bitmap not support compressed.";
		}

		//
		// 5 ������������
		//
		pStream->Seek(dwOffset, CStream::SEEK_MODE_SET);
		pStream->Seek(dwDataOffset, CStream::SEEK_MODE_CUR);

		m_pData = (BYTE *)pStream->GetCurrentAddress();

		//
		// 6. ��������ȼ�
		//
		m_mipmaps = 0;

		if (bmiHeader.biSizeImage > (DWORD)ALIGN_4BYTE(bmiHeader.biWidth*bmiHeader.biHeight*bmiHeader.biBitCount / 8)) {
			DWORD dwBuffSize = 0;
			DWORD dwWidth = bmiHeader.biWidth;
			DWORD dwHeight = bmiHeader.biHeight;

			do {
				// ���������
				dwBuffSize += ALIGN_4BYTE(dwWidth*dwHeight*bmiHeader.biBitCount / 8);

				if (dwBuffSize > bmiHeader.biSizeImage) {
					break;
				}

				// �ֱ��ʼ��
				dwWidth = dwWidth >> 1;
				dwHeight = dwHeight >> 1;

				if (dwWidth == 0 && dwHeight == 0) {
					break;
				}

				if (dwWidth == 0) dwWidth = 1;
				if (dwHeight == 0) dwHeight = 1;

				// ����ȼ�����
				m_mipmaps++;
			} while (TRUE);
		}

		//
		// 7. ��ת����
		//    ˵��: ����DDS/PVR/ETCʹ�õ����������OpenGL��Y�������෴, 
		//          ��תBITMAP��ʽ������ﵽͳһ����Ŀ��.
		//
		{
			BYTE tmp;
			BYTE *pData = m_pData;

			DWORD dwWidth = bmiHeader.biWidth;
			DWORD dwHeight = bmiHeader.biHeight;
			DWORD dwBuffWidth = bmiHeader.biWidth*bmiHeader.biBitCount / 8;

			for (INT indexLevel = 0; indexLevel <= m_mipmaps; indexLevel++) {
				for (DWORD y = 0; y < dwHeight / 2; y++) {
					BYTE *pPixelTop = pData + dwBuffWidth*y;
					BYTE *pPixelBottom = pData + dwBuffWidth*(dwHeight - y - 1);

					for (DWORD x = 0; x < dwBuffWidth; x++) {
						SWAP(pPixelTop[x], pPixelBottom[x], tmp);
					}
				}

				pData += dwWidth*dwHeight*bmiHeader.biBitCount / 8;

				dwWidth = dwWidth >> 1;
				dwHeight = dwHeight >> 1;
				dwBuffWidth = dwBuffWidth >> 1;
			}
		}

		//
		// 8. RGB(A) => BGR(A)
		//    ˵��: iOS/Androidֻ֧��GL_RGB/GL_RGBA��ʽ, ��Ҫ����ת��
		//
		if (bmiHeader.biBitCount == 24 || bmiHeader.biBitCount == 32) {
			BYTE tmp;
			BYTE *pData = m_pData;

			DWORD dwWidth = bmiHeader.biWidth;
			DWORD dwHeight = bmiHeader.biHeight;
			DWORD dwBuffWidth = bmiHeader.biWidth*bmiHeader.biBitCount / 8;

			DWORD dwPixelOffset;
			switch (bmiHeader.biBitCount) {
			case 24: dwPixelOffset = 3; break;
			case 32: dwPixelOffset = 4; break;
			}

			for (INT indexLevel = 0; indexLevel <= m_mipmaps; indexLevel++) {
				for (DWORD y = 0; y < dwHeight; y++) {
					BYTE *pPixel = pData + dwBuffWidth*y;

					for (DWORD x = 0; x < dwWidth; x++, pPixel += dwPixelOffset) {
						SWAP(pPixel[0], pPixel[2], tmp);
					}
				}

				pData += dwWidth*dwHeight*bmiHeader.biBitCount / 8;

				dwWidth = dwWidth >> 1;
				dwHeight = dwHeight >> 1;
				dwBuffWidth = dwBuffWidth >> 1;
			}
		}

		//
		// 9. ����������Ϣ
		//
		m_type = BITMAP;
		m_target = GL_TEXTURE_2D;

		m_width = bmiHeader.biWidth;
		m_height = bmiHeader.biHeight;
		m_bitcount = bmiHeader.biBitCount;
	}

	//
	// ����DDSѹ������
	//
	VOID CTexture::LoadDDS(CStream *pStream)
	{
		ASSERT(pStream);
		ASSERT(pStream->IsValid());

		//
		// 1. ����DDS����
		//
		DWORD type;
		DDSURFACEDESC2 ddsd;

		pStream->Read(&type, sizeof(type), 1);
		pStream->Read(&ddsd, sizeof(ddsd), 1);

		m_pData = (BYTE *)pStream->GetCurrentAddress();

		//
		// 2. ����������Ϣ
		//
		switch (ddsd.ddpfPixelFormat.dwFourCC) {
		case FOURCC_DXT1: m_bitcount = 8;  m_ddsFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
		case FOURCC_DXT3: m_bitcount = 16; m_ddsFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
		case FOURCC_DXT5: m_bitcount = 16; m_ddsFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
		default: throw "Invalid dds format."; break;
		}

		m_type = DDS;
		m_target = (ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;

		m_width = ddsd.dwWidth;
		m_height = ddsd.dwHeight;
		m_mipmaps = ddsd.dwMipMapCount;
	}

	//
	// ����PVRѹ������
	//
	VOID CTexture::LoadPVR(CStream *pStream)
	{
		ASSERT(pStream);
		ASSERT(pStream->IsValid());

		//
		// 1. ����PVR����
		//
		PVRTCHEADER pvr;

		pStream->Read(&pvr, sizeof(pvr), 1);
		m_pData = (BYTE *)pStream->GetCurrentAddress();

		//
		// 2. ����������Ϣ
		//
		switch (pvr.dwFlags&PVRTEX_FLAGS_PIXELTYPE) {
		case PVRTEX_FLAGS_OGL_PVRTC2: case PVRTEX_FLAGS_MGLPT_PVRTC2: m_type = PVR; m_pvrFormat = pvr.dwBitMaskAlpha ? GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG; break;
		case PVRTEX_FLAGS_OGL_PVRTC4: case PVRTEX_FLAGS_MGLPT_PVRTC4: m_type = PVR; m_pvrFormat = pvr.dwBitMaskAlpha ? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG; break;
		case PVRTEX_FLAGS_ETC_RGB_4BPP: m_type = ETC; m_pvrFormat = GL_ETC1_RGB8_OES; break;
		default: throw "Invalid pvr format."; break;
		}

		m_target = (pvr.dwFlags & PVRTEX_FLAGS_CUBEMAP) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;

		m_width = pvr.dwWidth;
		m_height = pvr.dwHeight;
		m_bitcount = pvr.dwBpp;
		m_mipmaps = pvr.dwMipmaps;
	}

	//
	// ��������
	//
	VOID CTexture::LoadTexture(CStream *pStream)
	{
		ASSERT(pStream);
		ASSERT(pStream->IsValid());

		//
		// 1. ��¼��ǰ�ļ�λ��
		//
		DWORD dwOffset = pStream->GetCurrentPosition();

		//
		// 2. ������
		//
		CHAR szMask[4];
		pStream->Read(szMask, sizeof(szMask), 1);
		pStream->Seek(dwOffset, CStream::SEEK_MODE_SET);

		//
		// 3. ��������
		//
		if (strncmp(szMask, "BM", 2) == 0) {
			LoadBMP(pStream);
		}
		else if (strncmp(szMask, "DDS", 3) == 0) {
			LoadDDS(pStream);
		}
		else if (strncmp(szMask, "PVR!", 4) == 0) {
			LoadPVR(pStream);
		}
		else {
			throw "Invalid texture format.";
		}
	}

	//
	// ��������
	//
	VOID CTexture::CreateTexture(VOID)
	{
		switch (m_type) {
		case BITMAP:
			m_texture = Renderer()->CreateTexture2DMipmapsBMP(m_target, m_mipmaps, GetInternalFormatGL(), m_width, m_height, 0, GetFormatGL(), GetTypeGL(), m_bitcount, m_pData);
			if (m_texture == 0) throw "CreateTexture2DMipmapsBMP failed.";
			break;
		case DDS:
			m_texture = Renderer()->CreateTexture2DMipmapsDDS(m_target, m_mipmaps, m_ddsFormat, m_width, m_height, 0, m_bitcount, m_pData);
			if (m_texture == 0) throw "CreateTexture2DMipmapsDDS failed.";
			break;
		case PVR:
			m_texture = Renderer()->CreateTexture2DMipmapsPVR(m_target, m_mipmaps, m_pvrFormat, m_width, m_height, 0, m_bitcount, m_pData);
			if (m_texture == 0) throw "CreateTexture2DMipmapsPVR failed.";
			break;
		case ETC:
			m_texture = Renderer()->CreateTexture2DMipmapsETC(m_target, m_mipmaps, m_pvrFormat, m_width, m_height, 0, m_bitcount, m_pData);
			if (m_texture == 0) throw "CreateTexture2DMipmapsETC failed.";
			break;
		default:
			throw "Invalid texture type.";
			break;
		}
	}

	//
	// ���Ŀ��
	//
	UINT CTexture::GetTargetGL(VOID) const
	{
		return m_target;
	}

	//
	// �������
	//
	UINT CTexture::GetTextureGL(VOID) const
	{
		return m_texture;
	}

	//
	// ��������ڲ���ʽ
	//
	INT CTexture::GetInternalFormatGL(VOID) const
	{
		switch (m_bitcount) {
		case  8: return GL_ALPHA;
		case 16: return GL_LUMINANCE_ALPHA;
		case 24: return GL_RGB;
		case 32: return GL_RGBA;
		}

		return -1;
	}

	//
	// ��������ʽ
	//
	INT CTexture::GetFormatGL(VOID) const
	{
		switch (m_bitcount) {
		case  8: return GL_ALPHA;
		case 16: return GL_LUMINANCE_ALPHA;
		case 24: return GL_RGB;
		case 32: return GL_RGBA;
		}

		return -1;
	}

	//
	// �������������������
	//
	INT CTexture::GetTypeGL(VOID) const
	{
		return GL_UNSIGNED_BYTE;
	}

}

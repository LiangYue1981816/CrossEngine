/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
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

#include "ccConfig.h"
#include "CCSpriteBatchNode.h"
#include "CCSpriteAnimation.h"
#include "CCTextureCache.h"
#include "CCTexture2D.h"
#include "CCFileUtils.h"
#include "CCDirector.h"

namespace cocos2d {

	CCSpriteAnimation::~CCSpriteAnimation()
	{
		free();
	}

	bool CCSpriteAnimation::init()
	{
		if (!CCLayer::init()) {
			return false;
		}

		m_bPlaying = false;
		m_bPause = false;
		m_bLoop = false;

		m_indexFrame = 0;
		m_indexFrameTick = 0;
		m_deltaTimeTick = 0.0f;
		m_pAnimation = NULL;

		m_pobTexture = NULL;
		m_animations.clear();

		m_bOpacityModifyRGB = true;
		m_nOpacity = 255;
		m_sColor = m_sColorUnmodified = ccWHITE;

		m_sBlendFunc.src = CC_BLEND_SRC;
		m_sBlendFunc.dst = CC_BLEND_DST;

		CCLayer::setIsRelativeAnchorPoint(false);

		return true;
	}

	void CCSpriteAnimation::free()
	{
		m_stream.Free();

		for (AnimationMap::const_iterator itAnimation = m_animations.begin(); itAnimation != m_animations.end(); ++itAnimation) {
			ANIMATION *pAnimation = itAnimation->second;
			ASSERT(pAnimation);
			delete pAnimation;
		}

		m_animations.clear();
	}

	CCSpriteAnimation* CCSpriteAnimation::spriteWithBatchNode(CCSpriteBatchNode *batchNode, const char *pszAnimationFileName)
	{
		if (CCSpriteAnimation *pSpriteAnimation = CCSpriteAnimation::node()) {
			pSpriteAnimation->initWithBatchNode(batchNode, pszAnimationFileName);
			return pSpriteAnimation;
		}
		return NULL;
	}

	CCSpriteAnimation* CCSpriteAnimation::spriteWithFile(const char *pszTextureFileName, const char *pszAnimationFileName)
	{
		if (CCSpriteAnimation *pSpriteAnimation = CCSpriteAnimation::node()) {
			pSpriteAnimation->initWithFile(pszTextureFileName, pszAnimationFileName);
			return pSpriteAnimation;
		}
		return NULL;
	}

	bool CCSpriteAnimation::initWithFile(const char *pszTextureFileName, const char *pszAnimationFileName)
	{
		CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(pszTextureFileName);
		if (pTexture) {
			return initWithTexture(pTexture, pszAnimationFileName);
		}
		return false;
	}

	bool CCSpriteAnimation::initWithTexture(CCTexture2D *pTexture, const char *pszAnimationFileName)
	{
		free();

		std::string animationPathKey = pszAnimationFileName;
		CCFileUtils::ccRemoveHDSuffixFromFile(animationPathKey);
		animationPathKey = CCFileUtils::fullPathFromRelativePath(animationPathKey.c_str());

		bool bRet = false;
		if (m_stream.LoadFromFile(animationPathKey.c_str())) {
			int materialName = (int)m_stream.ReadDWORD();
			int numAnimations = (int)m_stream.ReadDWORD();

			for (int indexAnimation = 0; indexAnimation < numAnimations; indexAnimation++) {
				ANIMATION *pAnimation = new ANIMATION;
				ASSERT(pAnimation);

				m_animations[m_stream.ReadDWORD()] = pAnimation;

				int addrFModule = (int)m_stream.ReadDWORD();
				int addrAFrame = (int)m_stream.ReadDWORD();
				int addrAnimation = (int)m_stream.ReadDWORD();
				int addrNext = (int)m_stream.ReadDWORD();

				m_stream.Seek(addrAnimation, CrossEngine::CStream::SEEK_MODE_SET);
				pAnimation->numFrames = (int)m_stream.ReadDWORD();

				FRAME *pAFrameBase = NULL;
				MODULE *pFModuleBase = NULL;

				m_stream.Seek(addrAFrame, CrossEngine::CStream::SEEK_MODE_SET);
				pAFrameBase = (FRAME *)m_stream.GetCurrentAddress();

				m_stream.Seek(addrFModule, CrossEngine::CStream::SEEK_MODE_SET);
				pFModuleBase = (MODULE *)m_stream.GetCurrentAddress();

				ASSERT(pAFrameBase);
				ASSERT(pFModuleBase);

				pAnimation->pFrames = pAFrameBase;

				for (int indexAFrame = 0, indexFModule = 0; indexAFrame < pAnimation->numFrames; indexAFrame++) {
					FRAME *pFrame = &pAFrameBase[indexAFrame];
					ASSERT(pFrame);

					pFrame->pFModules = &pFModuleBase[indexFModule];
					indexFModule += pFrame->numFModules;
				}

				m_stream.Seek(addrNext, CrossEngine::CStream::SEEK_MODE_SET);
			}

			setTexture(pTexture);

			bRet = true;
		}
		return bRet;
	}

	bool CCSpriteAnimation::initWithBatchNode(CCSpriteBatchNode *batchNode, const char *pszAnimationFileName)
	{
		if (initWithTexture(batchNode->getTexture(), pszAnimationFileName))
		{
			return true;
		}
		return false;
	}

	void CCSpriteAnimation::setTexture(CCTexture2D *texture)
	{
		CC_SAFE_RELEASE(m_pobTexture);

		m_pobTexture = texture;
		if (texture) {
			texture->retain();
		}

		updateBlendFunc();
	}

	CCTexture2D* CCSpriteAnimation::getTexture(void)
	{
		return m_pobTexture;
	}

	void CCSpriteAnimation::setBlendFunc(ccBlendFunc blendFunc)
	{
		m_sBlendFunc = blendFunc;
	}

	ccBlendFunc CCSpriteAnimation::getBlendFunc(void)
	{
		return m_sBlendFunc;
	}

	void CCSpriteAnimation::updateBlendFunc(void)
	{
		if (!m_pobTexture || !m_pobTexture->getHasPremultipliedAlpha()) {
			m_sBlendFunc.src = GL_SRC_ALPHA;
			m_sBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
			setIsOpacityModifyRGB(false);
		}
		else {
			m_sBlendFunc.src = CC_BLEND_SRC;
			m_sBlendFunc.dst = CC_BLEND_DST;
			setIsOpacityModifyRGB(true);
		}
	}

	void CCSpriteAnimation::setOpacity(GLubyte opacity)
	{
		m_nOpacity = opacity;

		if (m_bOpacityModifyRGB) {
			setColor(m_sColorUnmodified);
		}
	}

	const ccColor3B& CCSpriteAnimation::getColor(void)
	{
		if (m_bOpacityModifyRGB) {
			return m_sColorUnmodified;
		}

		return m_sColor;
	}

	void CCSpriteAnimation::setColor(const ccColor3B& color)
	{
		m_sColor = m_sColorUnmodified = color;

		if (m_bOpacityModifyRGB) {
			m_sColor.r = color.r * m_nOpacity/255;
			m_sColor.g = color.g * m_nOpacity/255;
			m_sColor.b = color.b * m_nOpacity/255;
		}
	}

	void CCSpriteAnimation::setIsOpacityModifyRGB(bool bValue)
	{
		m_bOpacityModifyRGB = bValue;
	}

	bool CCSpriteAnimation::getIsOpacityModifyRGB(void)
	{
		return m_bOpacityModifyRGB;
	}

	void CCSpriteAnimation::play(const char *pszName, bool bLoop)
	{
		stop();

		if (m_pAnimation = m_animations[CrossEngine::HashValue(pszName)]) {
			m_bPlaying = true;
			m_bLoop = bLoop;
		}
	}

	void CCSpriteAnimation::stop(void)
	{
		m_bPlaying = false;
		m_bPause = false;
		m_bLoop = false;

		m_indexFrame = 0;
		m_indexFrameTick = 0;
		m_deltaTimeTick = 0.0f;
	}

	void CCSpriteAnimation::pause(void)
	{
		m_bPause = true;
	}

	void CCSpriteAnimation::resume(void)
	{
		m_bPause = false;
	}

	bool CCSpriteAnimation::isPlaying(void) const
	{
		return m_bPlaying;
	}

	void CCSpriteAnimation::update(ccTime dt)
	{
		static const float DELTA_TIME = 1.0f / 30;

		if (!m_pAnimation) {
			return;
		}

		m_deltaTimeTick += dt;

		if (m_deltaTimeTick > DELTA_TIME) {
			m_deltaTimeTick = 0.0f;

			const FRAME *pFrame = &m_pAnimation->pFrames[m_indexFrame];
			ASSERT(pFrame);

			if (m_indexFrameTick++ > pFrame->times) {
				m_indexFrame++;
				m_indexFrameTick = 0;
			}

			if (m_indexFrame >= m_pAnimation->numFrames) {
				if (m_bLoop) {
					m_indexFrame = 0;
				}
				else {
					m_indexFrame = m_pAnimation->numFrames - 1;
				}
			}
		}
	}

	void CCSpriteAnimation::draw()
	{
		CCNode::draw();

		if (m_pAnimation) {
			// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
			// Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
			// Unneeded states: -
			bool newBlend = m_sBlendFunc.src != CC_BLEND_SRC || m_sBlendFunc.dst != CC_BLEND_DST;
			if (newBlend) {
				CrossEngine::cglBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
			}

			if (m_pobTexture) {
				CrossEngine::cglBindTexture(GL_TEXTURE_2D, m_pobTexture->getName());
			}
			else {
				CrossEngine::cglBindTexture(GL_TEXTURE_2D, 0);
			}

			GLenum type = GL_INVALID_ENUM;
			switch (sizeof(CrossEngine::INDEX)) {
			case 1: type = GL_UNSIGNED_BYTE; break;
			case 2: type = GL_UNSIGNED_SHORT; break;
			case 4: type = GL_UNSIGNED_INT; break;
			}

			const ccColor3B& color = getColor();
			float r = color.r*1.0f;
			float g = color.g*1.0f;
			float b = color.b*1.0f;
			float a = m_nOpacity*1.0f;

			const FRAME *pFrame = &m_pAnimation->pFrames[m_indexFrame];
			ASSERT(pFrame);

			unsigned int aFlags = (BITBLT_NONE&(~BITBLT_ROT_90))^pFrame->flags;
			bool bFlipX = (aFlags&BITBLT_FLIP_X) ? TRUE : FALSE;
			bool bFlipY = (aFlags&BITBLT_FLIP_Y) ? TRUE : FALSE;

			CrossEngine::INDEX *pIndices = Renderer()->LockIndexBuffer(pFrame->numFModules * 6);
			CrossEngine::VERTEX *pVertices = Renderer()->LockVertexBuffer(pFrame->numFModules * 4);
			{
				// FModule
				// 0 ___ 3
				//  |   |
				//  |___|
				// 1     2

				int indexVertex = 0;
				int indexFModule = 0;

				for (int indexFModule = 0; indexFModule < pFrame->numFModules; indexFModule++) {
					MODULE *pFModule = &pFrame->pFModules[indexFModule];
					ASSERT(pFModule);

					bool bRot90 = (pFModule->flags&BITBLT_ROT_90) ? TRUE : FALSE;

					float ox = pFrame->ox + 0.0f;
					float oy = pFrame->oy + 0.0f;

					float width = pFModule->width + 0.0f;
					float height = pFModule->height + 0.0f;

					float x = bFlipX ?  (ox - pFModule->x - width)  :  (ox + pFModule->x);
					float y = bFlipY ? -(oy - pFModule->y - height) : -(oy + pFModule->y);

					float texCoords[8] = {
						bFlipX ? pFModule->texCoords[3][0] : pFModule->texCoords[0][0],
						bFlipY ? pFModule->texCoords[1][1] : pFModule->texCoords[0][1],

						bFlipX ? pFModule->texCoords[2][0] : pFModule->texCoords[1][0],
						bFlipY ? pFModule->texCoords[0][1] : pFModule->texCoords[1][1],

						bFlipX ? pFModule->texCoords[1][0] : pFModule->texCoords[2][0],
						bFlipY ? pFModule->texCoords[3][1] : pFModule->texCoords[2][1],

						bFlipX ? pFModule->texCoords[0][0] : pFModule->texCoords[3][0],
						bFlipY ? pFModule->texCoords[2][1] : pFModule->texCoords[3][1],
					};

					if (bRot90) {
						float tmp;

						if (bFlipX) {
							SWAP(texCoords[2*0+1], texCoords[2*3+1], tmp);
							SWAP(texCoords[2*1+1], texCoords[2*2+1], tmp);
						}

						if (bFlipY) {
							SWAP(texCoords[2*0+0], texCoords[2*2+0], tmp);
							SWAP(texCoords[2*1+0], texCoords[2*3+0], tmp);
						}
					}

					pVertices[indexVertex].position[0] = x;
					pVertices[indexVertex].position[1] = y;
					pVertices[indexVertex].position[2] = m_fVertexZ;
					pVertices[indexVertex].texCoordDiffuse[0] = texCoords[2 * 0 + 0];
					pVertices[indexVertex].texCoordDiffuse[1] = texCoords[2 * 0 + 1];
					pVertices[indexVertex].color[0] = r;
					pVertices[indexVertex].color[1] = g;
					pVertices[indexVertex].color[2] = b;
					pVertices[indexVertex].color[3] = a;
					indexVertex++;

					pVertices[indexVertex].position[0] = x;
					pVertices[indexVertex].position[1] = y - height;
					pVertices[indexVertex].position[2] = m_fVertexZ;
					pVertices[indexVertex].texCoordDiffuse[0] = texCoords[2 * 1 + 0];
					pVertices[indexVertex].texCoordDiffuse[1] = texCoords[2 * 1 + 1];
					pVertices[indexVertex].color[0] = r;
					pVertices[indexVertex].color[1] = g;
					pVertices[indexVertex].color[2] = b;
					pVertices[indexVertex].color[3] = a;
					indexVertex++;

					pVertices[indexVertex].position[0] = x + width;
					pVertices[indexVertex].position[1] = y - height;
					pVertices[indexVertex].position[2] = m_fVertexZ;
					pVertices[indexVertex].texCoordDiffuse[0] = texCoords[2 * 2 + 0];
					pVertices[indexVertex].texCoordDiffuse[1] = texCoords[2 * 2 + 1];
					pVertices[indexVertex].color[0] = r;
					pVertices[indexVertex].color[1] = g;
					pVertices[indexVertex].color[2] = b;
					pVertices[indexVertex].color[3] = a;
					indexVertex++;

					pVertices[indexVertex].position[0] = x + width;
					pVertices[indexVertex].position[1] = y;
					pVertices[indexVertex].position[2] = m_fVertexZ;
					pVertices[indexVertex].texCoordDiffuse[0] = texCoords[2 * 3 + 0];
					pVertices[indexVertex].texCoordDiffuse[1] = texCoords[2 * 3 + 1];
					pVertices[indexVertex].color[0] = r;
					pVertices[indexVertex].color[1] = g;
					pVertices[indexVertex].color[2] = b;
					pVertices[indexVertex].color[3] = a;
					indexVertex++;
				}

				indexVertex = 0;
				indexFModule = 0;

				while (indexFModule < pFrame->numFModules) {
					pIndices[indexVertex++] = 4*indexFModule + 0;
					pIndices[indexVertex++] = 4*indexFModule + 1;
					pIndices[indexVertex++] = 4*indexFModule + 2;

					pIndices[indexVertex++] = 4*indexFModule + 2;
					pIndices[indexVertex++] = 4*indexFModule + 3;
					pIndices[indexVertex++] = 4*indexFModule + 0;

					indexFModule++;
				}

				CrossEngine::cglVertexPointer(3, GL_FLOAT, sizeof(*pVertices), (GLvoid *)&pVertices->position);
				CrossEngine::cglColorPointer(4, GL_FLOAT, sizeof(*pVertices), (GLvoid *)&pVertices->color);
				CrossEngine::cglTexCoordPointer(2, GL_FLOAT, sizeof(*pVertices), (GLvoid *)&pVertices->texCoordDiffuse);

				CrossEngine::cglDrawElementsCompatible(GL_TRIANGLES, 6*pFrame->numFModules, type, pIndices);
			}
			Renderer()->UnlockVertexBuffer(pVertices);
			Renderer()->UnlockIndexBuffer(pIndices);

			if (newBlend) {
				CrossEngine::cglBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
			}
		}
	}

}

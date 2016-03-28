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

#ifndef __SPITE_NODE_CCSPRITEEX_H__
#define __SPITE_NODE_CCSPRITEEX_H__

#include "CCLayer.h"
#include "CCProtocols.h"
#include "CCTextureAtlas.h"
#include "ccTypes.h"

namespace cocos2d {

	class CC_DLL CCSpriteAnimation : public CCLayer, public CCTextureProtocol
	{
		typedef struct {
			int x;
			int y;

			int width;
			int height;

			unsigned int flags;

			float texCoords[4][2];
		} MODULE;

		typedef struct {
			int ox;
			int oy;

			int width;
			int height;

			int times;
			unsigned int flags;

			int numFModules;
			MODULE *pFModules;
		} FRAME;

		typedef struct {
			int numFrames;
			FRAME *pFrames;
		} ANIMATION;

		typedef std::map<unsigned int, ANIMATION*> AnimationMap;

		static const DWORD BITBLT_NONE   = 0x00000000;
		static const DWORD BITBLT_FLIP_X = 0x00000001;
		static const DWORD BITBLT_FLIP_Y = 0x00000002;
		static const DWORD BITBLT_ROT_90 = 0x00000004;

	public:
		LAYER_NODE_FUNC(CCSpriteAnimation);

		virtual ~CCSpriteAnimation();
		virtual bool init();
		virtual void free();
		virtual void draw();
		virtual void update(ccTime dt);

	public:
		static CCSpriteAnimation* spriteWithBatchNode(CCSpriteBatchNode *batchNode, const char *pszAnimationFileName);
		static CCSpriteAnimation* spriteWithFile(const char *pszTextureFileName, const char *pszAnimationFileName);
		bool initWithFile(const char *pszTextureFileName, const char *pszAnimationFileName);
		bool initWithTexture(CCTexture2D *pTexture, const char *pszAnimationFileName);
		bool initWithBatchNode(CCSpriteBatchNode *batchNode, const char *pszAnimationFileName);

	public:
		virtual void setTexture(CCTexture2D *texture);
		virtual CCTexture2D* getTexture(void);

		virtual void setBlendFunc(ccBlendFunc blendFunc);
		virtual ccBlendFunc getBlendFunc(void);
		virtual void updateBlendFunc(void);

	public:
		virtual void setIsRelativeAnchorPoint(bool var) {}
		virtual void setAnchorPoint(const CCPoint& var) {}
		virtual void setContentSize(const CCSize& var) {}
		virtual void setContentSizeInPixels(const CCSize& var) {}

	public:
		void setOpacity(GLubyte opacity);

		const ccColor3B& getColor(void);
		void setColor(const ccColor3B& color);

		void setIsOpacityModifyRGB(bool bValue);
		bool getIsOpacityModifyRGB(void);

	public:
		void play(const char *pszName, bool bLoop);
		void stop(void);

		void pause(void);
		void resume(void);

		bool isPlaying(void) const;

	private:
		ccBlendFunc m_sBlendFunc;
		CCTexture2D *m_pobTexture;
		AnimationMap m_animations;

		GLubyte m_nOpacity;
		bool m_bOpacityModifyRGB;
		ccColor3B m_sColor;
		ccColor3B m_sColorUnmodified;

	private:
		bool m_bPlaying;
		bool m_bPause;
		bool m_bLoop;

		int m_indexFrame;
		int m_indexFrameTick;
		float m_deltaTimeTick;
		ANIMATION *m_pAnimation;

	private:
		CrossEngine::CStream m_stream;
	};

}

#endif // __SPITE_NODE_CCSPRITEEX_H__

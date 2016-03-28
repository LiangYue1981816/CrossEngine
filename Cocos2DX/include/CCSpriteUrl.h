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

#ifndef __SPITE_NODE_CCSPRITE_URL_H__
#define __SPITE_NODE_CCSPRITE_URL_H__

#include "CCLayer.h"
#include "CCImage.h"
#include "CCSprite.h"
#include "CCString.h"
#include "pthread.h"
#include "semaphore.h"

namespace cocos2d {

	class CC_DLL CCBuffer
	{
	public:
		CCBuffer();
		~CCBuffer();

	public:
		bool allocate(size_t size);
		void free(void);

		bool isFull(void);
		size_t write(void *pData, size_t size);

		size_t getSize(void);
		void* getData(void);

	private:
		size_t m_offset;
		size_t m_totalSize;
		unsigned char *m_pBuffer;

	private:
		pthread_mutex_t m_mutex;
	};

	class CC_DLL CCSpriteUrl : public CCLayer
	{
	public:
		LAYER_NODE_FUNC(CCSpriteUrl);

		virtual bool init();
		virtual void update(ccTime dt);
		virtual void onEnter();
		virtual void onExit();

	public:
		static CCSpriteUrl* spriteWithFile(const char *pszFileName, const char *pszUrl, CCImage::EImageFormat format, int timeOut);

		bool initWithFile(const char *pszFileName);
		bool initWithFile(const char *pszFileName, const char *pszUrl, CCImage::EImageFormat format, int timeOut);
		bool initWithTexture(CCTexture2D *pTexture);
		bool initWithImageData(void *data, int size);

	public:
		int getTimeOut() const;
		const char* getUrl() const;
		CCBuffer& getBuffer();

	protected:
		int m_timeOut;
		CCString m_strUrl;
		CCImage::EImageFormat m_format;

	protected:
		CCBuffer m_buffer;
		pthread_t m_thread;
	};

}

#endif // __SPITE_NODE_CCSPRITE_URL_H__

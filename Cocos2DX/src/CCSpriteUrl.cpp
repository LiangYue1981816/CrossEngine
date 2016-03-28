#include "curl/curl.h"
#include "CCSpriteUrl.h"
#include "CCTextureCache.h"
#include "CCPointExtension.h"


namespace cocos2d {

	static size_t getFileSizeCallback(void *buffer, size_t size, size_t nmemb, void *stream)
	{
		return size*nmemb;
	}

	static size_t getFileSize(const char *pszUrl, int timeOut)
	{
		double size = 0;

		for (int i = 1; i >= 0; i--) {
			long httpCode;
			CURL *curl = NULL;
			CURLcode res = CURLE_OK;

			size = 0;

			curl_global_init(CURL_GLOBAL_DEFAULT);
			curl = curl_easy_init();

			do {
				curl_easy_setopt(curl, CURLOPT_URL, pszUrl);
				curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeOut);
				curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getFileSizeCallback);
				curl_easy_setopt(curl, CURLOPT_NOBODY, i);

				res = curl_easy_perform(curl);
				if (res != CURLE_OK) break;

				res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
				if (res != CURLE_OK || httpCode != 200) break;

				res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &size);
				if (res != CURLE_OK) break;
			} while (0);

			curl_easy_cleanup(curl);
			curl_global_cleanup();

			if (res == CURLE_OK && httpCode == 200) {
				break;
			}
		}

		return (size_t)size;
	}

	static size_t downloadCallback(void *buffer, size_t size, size_t nmemb, void *stream)
	{
		CCSpriteUrl *pSpriteUrl = (CCSpriteUrl *)stream;
		return pSpriteUrl->getBuffer().write(buffer, size*nmemb);
	}

	static bool download(CCSpriteUrl *pSpriteUrl, const char *pszUrl, int timeOut)
	{
		CURL *curl = NULL;
		CURLcode res = CURLE_OK;

		curl_global_init(CURL_GLOBAL_DEFAULT);
		curl = curl_easy_init();

		do {
			curl_easy_setopt(curl, CURLOPT_URL, pszUrl);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeOut);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, downloadCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, pSpriteUrl);

			res = curl_easy_perform(curl);
		} while (0);

		curl_easy_cleanup(curl);
		curl_global_cleanup();

		return res == CURLE_OK;
	}

	static void* loadThread(void *lParam)
	{
		CCSpriteUrl *pSpriteUrl = (CCSpriteUrl *)lParam;
		pSpriteUrl->retain();
		{
			do {
				bool rcode = false;
				size_t size = 0;

				size = getFileSize(pSpriteUrl->getUrl(), pSpriteUrl->getTimeOut());
				if (!size) break;

				rcode = pSpriteUrl->getBuffer().allocate(size);
				if (!rcode) break;

				rcode = download(pSpriteUrl, pSpriteUrl->getUrl(), pSpriteUrl->getTimeOut());
				if (!rcode) break;
			} while (0);
		}
		pSpriteUrl->release();
		return NULL;
	}


	CCBuffer::CCBuffer()
	{
		pthread_mutex_init(&m_mutex, NULL);

		m_offset = 0;
		m_totalSize = 0;
		m_pBuffer = NULL;
	}

	CCBuffer::~CCBuffer()
	{
		free();

		pthread_mutex_destroy(&m_mutex);
	}

	bool CCBuffer::allocate(size_t size)
	{
		pthread_mutex_lock(&m_mutex);
		{
			m_offset = 0;
			m_totalSize = size;
			m_pBuffer = (unsigned char *)::malloc(size);
		}
		pthread_mutex_unlock(&m_mutex);

		return m_pBuffer != NULL;
	}

	void CCBuffer::free(void)
	{
		pthread_mutex_lock(&m_mutex);
		{
			if (m_pBuffer) {
				::free(m_pBuffer);
			}

			m_offset = 0;
			m_totalSize = 0;
			m_pBuffer = NULL;
		}
		pthread_mutex_unlock(&m_mutex);
	}

	bool CCBuffer::isFull(void)
	{
		bool bIsFull = false;

		pthread_mutex_lock(&m_mutex);
		{
			if (m_offset == m_totalSize && m_offset != 0) {
				bIsFull = true;
			}
		}
		pthread_mutex_unlock(&m_mutex);

		return bIsFull;
	}

	size_t CCBuffer::write(void *pData, size_t size)
	{
		pthread_mutex_lock(&m_mutex);
		{
			if (m_offset < m_totalSize) {
				memcpy(&m_pBuffer[m_offset], pData, min(size, m_totalSize - m_offset));
				m_offset += min(size, m_totalSize - m_offset);
			}
		}
		pthread_mutex_unlock(&m_mutex);

		return size;
	}

	size_t CCBuffer::getSize(void)
	{
		size_t size = 0;

		pthread_mutex_lock(&m_mutex);
		{
			size = m_offset;
		}
		pthread_mutex_unlock(&m_mutex);

		return size;
	}

	void* CCBuffer::getData(void)
	{
		void *pData = NULL;

		pthread_mutex_lock(&m_mutex);
		{
			pData = m_pBuffer;
		}
		pthread_mutex_unlock(&m_mutex);

		return pData;
	}



	bool CCSpriteUrl::init()
	{
		if (!CCLayer::init()) {
			return false;
		}

		m_timeOut = 10;
		m_strUrl.m_sString = "";
		m_format = CCImage::kFmtJpg;

		return true;
	}

	void CCSpriteUrl::update(ccTime dt)
	{
		if (m_buffer.isFull()) {
			initWithImageData(m_buffer.getData(), m_buffer.getSize());
			m_buffer.free();
		}
	}

	void CCSpriteUrl::onEnter()
	{
		CCLayer::onEnter();

		m_buffer.free();
		pthread_create(&m_thread, NULL, loadThread, this);
	}

	void CCSpriteUrl::onExit()
	{
		CCLayer::onExit();

		m_buffer.free();
		pthread_kill(m_thread, 0);
	}

	CCSpriteUrl* CCSpriteUrl::spriteWithFile(const char *pszFileName, const char *pszUrl, CCImage::EImageFormat format, int timeOut)
	{
		if (CCSpriteUrl *pSpriteUrl = CCSpriteUrl::node()) {
			pSpriteUrl->initWithFile(pszFileName, pszUrl, format, timeOut);
			return pSpriteUrl;
		}
		return NULL;
	}

	bool CCSpriteUrl::initWithFile(const char *pszFileName, const char *pszUrl, CCImage::EImageFormat format, int timeOut)
	{
		m_format = format;
		m_timeOut = timeOut;
		m_strUrl.m_sString = pszUrl;

		if (CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->getImage(pszUrl)) {
			return initWithTexture(pTexture);
		}

		return initWithFile(pszFileName);
	}

	bool CCSpriteUrl::initWithFile(const char *pszFileName)
	{
		if (!pszFileName) {
			return false;
		}

		removeAllChildrenWithCleanup(true);

		CCSprite *pSprite = CCSprite::spriteWithFile(pszFileName);
		pSprite->setAnchorPoint(ccp(0.5f, 0.5f));
		pSprite->setPosition(ccp(pSprite->getContentSize().width/2.0f, pSprite->getContentSize().height/2.0f));
		addChild(pSprite);

		setIsRelativeAnchorPoint(true);
		setAnchorPoint(ccp(0.5f, 0.5f));
		setContentSize(pSprite->getContentSize());

		return true;
	}

	bool CCSpriteUrl::initWithTexture(CCTexture2D *pTexture)
	{
		if (!pTexture) {
			return false;
		}

		removeAllChildrenWithCleanup(true);

		CCSprite *pSprite = CCSprite::spriteWithTexture(pTexture);
		pSprite->setAnchorPoint(ccp(0.5f, 0.5f));
		pSprite->setPosition(ccp(pSprite->getContentSize().width/2.0f, pSprite->getContentSize().height/2.0f));
		addChild(pSprite);

		setIsRelativeAnchorPoint(true);
		setAnchorPoint(ccp(0.5f, 0.5f));
		setContentSize(pSprite->getContentSize());

		return true;
	}

	bool CCSpriteUrl::initWithImageData(void *data, int size)
	{
		if (!data || size <= 0) {
			return false;
		}

		CCImage image;
		image.initWithImageData(data, size, m_format);
		return initWithTexture(CCTextureCache::sharedTextureCache()->addUIImage(&image, m_strUrl.m_sString.c_str()));
	}

	int CCSpriteUrl::getTimeOut() const
	{
		return m_timeOut;
	}

	const char* CCSpriteUrl::getUrl() const
	{
		return m_strUrl.m_sString.c_str();
	}

	CCBuffer& CCSpriteUrl::getBuffer()
	{
		return m_buffer;
	}

}
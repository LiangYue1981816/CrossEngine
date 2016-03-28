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

#include "CCNS.h"
#include "CCDirector.h"
#include "CCScene.h"
#include "CCMutableArray.h"
#include "CCScheduler.h"
#include "ccMacros.h"
#include "CCTouchDispatcher.h"
#include "CCPointExtension.h"
#include "CCTransition.h"
#include "CCTextureCache.h"
#include "CCTransition.h"
#include "CCSpriteFrameCache.h"
#include "CCAutoreleasePool.h"
#include "platform/CCPlatform.h"
#include "CCLabelBMFont.h"
#include "CCActionManager.h"
#include "CCLabelAtlas.h"
#include "CCConfiguration.h"
#include "CCKeypadDispatcher.h"
#include "CCGL.h"
#include "CCAnimationCache.h"
#include "CCTouch.h"
#include "CCUserDefault.h"
#include "CCNotificationCenter.h"

#if CC_ENABLE_PROFILERS
#include "support/CCProfiling.h"
#endif // CC_ENABLE_PROFILERS

#include <string>

namespace cocos2d {

	// singleton stuff
	static CCDirector *s_sharedDirector = NULL;
	extern const char* cocos2dVersion(void);

	CCDirector* CCDirector::sharedDirector(void)
	{
		if (!s_sharedDirector)
		{
			s_sharedDirector = new CCDirector;
			s_sharedDirector->init();
		}

		return s_sharedDirector;
	}

	bool CCDirector::init(void)
	{
		CCLOG("cocos2d: %s", cocos2dVersion());

		// Notification node
		m_pNotificationNode = NULL;

		// Scenes
		m_subScenes.clear();

		m_pNextMainScene = NULL;
		m_pRunningMainScene = NULL;
		m_pobMainScenesStack = new CCMutableArray<CCScene*>();

		// Set default projection (3D)
		m_eProjection = kCCDirectorProjectionDefault;

		// projection delegate if "Custom" projection is used
		m_pProjectionDelegate = NULL;

		// FPS
		m_bDisplayFPS = false;
		m_uTotalFrames = m_uFrames = 0;
		m_pszFPS = new char[100];
		m_pLastUpdate = new struct cc_timeval();

		// paused
		m_bPaused = false;

		// purge
		m_obWinSizeInPixels = m_obWinSizeInPoints = CCSizeZero;

		// portrait mode default
		m_eDeviceOrientation = CCDeviceOrientationPortrait;
		m_fContentScaleFactor = 1.0f;

		// create autorelease pool
		CCPoolManager::getInstance()->push();

		return true;
	}

	CCDirector::CCDirector(void)
	{
		m_bLandscape = false;
		m_bDisplayFPS = false;
		m_fAccumDt = 0.0f;
		m_fFrameRate = 0.0f;
		m_fRecvData = 0.0f;
		m_fSendData = 0.0f;
		m_uHeartTime = 0;
		m_pFPSLabel = NULL;
		m_bPaused = false;
		m_uTotalFrames = 0;
		m_uFrames = 0;
		m_pNextMainScene = NULL;
		m_pRunningMainScene = NULL;
		m_pobMainScenesStack = NULL;
		m_bSendCleanupToMainScene = false;
		m_pLastUpdate = NULL;
		m_fDeltaTime = 0.0f;
		m_bNextDeltaTimeZero = false;
		m_eProjection = kCCDirectorProjection2D;
		m_fContentScaleFactor = 1.0f;
		m_pszFPS = NULL;
		m_pNotificationNode = NULL;
		m_pProjectionDelegate = NULL;
		m_eDeviceOrientation = kCCDeviceOrientationPortrait;
	}

	CCDirector::~CCDirector(void)
	{
		CCLOGINFO("cocos2d: deallocing %p", this);

#if CC_DIRECTOR_FAST_FPS
		CC_SAFE_RELEASE(m_pFPSLabel);
#endif 

		CC_SAFE_RELEASE(m_pNotificationNode);
		CC_SAFE_RELEASE(m_pRunningMainScene);
		CC_SAFE_RELEASE(m_pobMainScenesStack);

		// pop the autorelease pool
		CCPoolManager::getInstance()->pop();

		// delete m_pLastUpdate
		CC_SAFE_DELETE(m_pLastUpdate);

		CCKeypadDispatcher::purgeSharedDispatcher();

		// delete fps string
		delete []m_pszFPS;
	}

	void CCDirector::setGLDefaultValues(void)
	{
		setAlphaBlending(true);
		setDepthTest(true);
		setProjection(m_eProjection);

		// reset opengl default values
		/*
		CrossEngine::cglActiveTexture(GL_TEXTURE7);
		CrossEngine::cglDisable(GL_TEXTURE_2D);

		CrossEngine::cglActiveTexture(GL_TEXTURE6);
		CrossEngine::cglDisable(GL_TEXTURE_2D);

		CrossEngine::cglActiveTexture(GL_TEXTURE5);
		CrossEngine::cglDisable(GL_TEXTURE_2D);

		CrossEngine::cglActiveTexture(GL_TEXTURE4);
		CrossEngine::cglDisable(GL_TEXTURE_2D);

		CrossEngine::cglActiveTexture(GL_TEXTURE3);
		CrossEngine::cglDisable(GL_TEXTURE_2D);

		CrossEngine::cglActiveTexture(GL_TEXTURE2);
		CrossEngine::cglDisable(GL_TEXTURE_2D);

		CrossEngine::cglActiveTexture(GL_TEXTURE1);
		CrossEngine::cglDisable(GL_TEXTURE_2D);
		*/
		CrossEngine::cglActiveTexture(GL_TEXTURE0);
		CrossEngine::cglEnable(GL_TEXTURE_2D);

		CrossEngine::cglBindTexture(GL_TEXTURE_2D, 0);
		CrossEngine::cglBindBuffer(GL_ARRAY_BUFFER, 0);
		CrossEngine::cglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Update the scene
	void CCDirector::update(ccTime dt)
	{
		updateScene(dt);
	}

	void CCDirector::updateScene(ccTime dt)
	{
		if (!m_bPaused) {
			CCScheduler::sharedScheduler()->tick(dt);
		}

		// update main scene
		if (m_pRunningMainScene) {
			m_pRunningMainScene->update(dt);
		}

		// update sub scene
		std::map<CCScene*, CCScene*> subScenes = m_subScenes;
		for (std::map<CCScene*, CCScene*>::const_iterator itScene = subScenes.begin(); itScene != subScenes.end(); ++itScene) {
			itScene->second->update(dt);
		}
	}

	// Draw the Scene
	void CCDirector::drawScene(void)
	{
		CrossEngine::cglClearDepth(1.0f);
		CrossEngine::cglClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		CrossEngine::cglClear(GL_DEPTH_BUFFER_BIT);

		// calculate "global" dt
		calculateDeltaTime();

		/* to avoid flickr, nextScene MUST be here: after tick and before draw.
		XXX: Which bug is this one. It seems that it can't be reproduced with v0.9 */
		if (m_pNextMainScene)
		{
			setNextMainScene();
		}

		CrossEngine::cglPushMatrix();

		applyOrientation();

		// By default enable VertexArray, ColorArray, TextureCoordArray and Texture2D
		CC_ENABLE_DEFAULT_GL_STATES();

		// draw main scene
		if (m_pRunningMainScene)
		{
			m_pRunningMainScene->visit();
		}

		// draw the notifications node
		if (m_pNotificationNode)
		{
			m_pNotificationNode->visit();
		}

		// draw the sub scenes
		for (std::map<CCScene*, CCScene*>::const_iterator itScene = m_subScenes.begin(); itScene != m_subScenes.end(); ++itScene) {
			itScene->second->visit();
		}

		if (m_bDisplayFPS)
		{
			showFPS();
		}

#if CC_ENABLE_PROFILERS
		showProfilers();
#endif

		CC_DISABLE_DEFAULT_GL_STATES();

		CrossEngine::cglPopMatrix();

		m_uTotalFrames++;
	}

	void CCDirector::calculateDeltaTime(void)
	{
		struct cc_timeval now;

		if (CCTime::gettimeofdayCocos2d(&now, NULL) != 0)
		{
			CCLOG("error in gettimeofday");
			m_fDeltaTime = 0;
			return;
		}

		// new delta time
		if (m_bNextDeltaTimeZero)
		{
			m_fDeltaTime = 0;
			m_bNextDeltaTimeZero = false;
		}
		else
		{
			m_fDeltaTime = (now.tv_sec - m_pLastUpdate->tv_sec) + (now.tv_usec - m_pLastUpdate->tv_usec) / 1000000.0f;
			m_fDeltaTime = MAX(0, m_fDeltaTime);
		}

#ifdef DEBUG
		// If we are debugging our code, prevent big delta time
		if (m_fDeltaTime > 0.2f)
		{
			m_fDeltaTime = 1 / 60.0f;
		}
#endif

		*m_pLastUpdate = now;
	}

	void CCDirector::setViewport(int width, int height, float scaleSize)
	{
		m_fContentScaleFactor = scaleSize;
		m_obWinSizeInPoints.width = width;
		m_obWinSizeInPoints.height = height;
		m_obWinSizeInPixels = CCSizeMake(m_obWinSizeInPoints.width * m_fContentScaleFactor, m_obWinSizeInPoints.height * m_fContentScaleFactor);
	}

	void CCDirector::setNextDeltaTimeZero(bool bNextDeltaTimeZero)
	{
		m_bNextDeltaTimeZero = bNextDeltaTimeZero;
	}

	void CCDirector::setProjection(ccDirectorProjection kProjection)
	{
		CCSize size = m_obWinSizeInPixels;
		float zeye = this->getZEye();
		switch (kProjection)
		{
		case kCCDirectorProjection2D:
			CrossEngine::cglViewport(0, 0, (int)(size.width + 0.5f), (int)(size.height + 0.5f));
			CrossEngine::cglMatrixMode(GL_PROJECTION);
			CrossEngine::cglLoadIdentity();
			CrossEngine::cglOrtho(0, size.width, 0, size.height, -1024 * CC_CONTENT_SCALE_FACTOR(), 1024 * CC_CONTENT_SCALE_FACTOR());
			CrossEngine::cglMatrixMode(GL_MODELVIEW);
			CrossEngine::cglLoadIdentity();
			break;

		case kCCDirectorProjection3D:
			CrossEngine::cglViewport(0, 0, (int)(size.width + 0.5f), (int)(size.height + 0.5f));
			CrossEngine::cglMatrixMode(GL_PROJECTION);
			CrossEngine::cglLoadIdentity();
			CrossEngine::cglPerspective(60, (GLfloat)size.width/size.height, 0.5f, 1500.0f);
			CrossEngine::cglMatrixMode(GL_MODELVIEW);	
			CrossEngine::cglLoadIdentity();
			CrossEngine::cglLookAt(size.width/2, size.height/2, zeye, size.width/2, size.height/2, 0, 0.0f, 1.0f, 0.0f);				
			break;

		case kCCDirectorProjectionCustom:
			if (m_pProjectionDelegate)
			{
				m_pProjectionDelegate->updateProjection();
			}
			break;

		default:
			CCLOG("cocos2d: Director: unrecognized projecgtion");
			break;
		}

		m_eProjection = kProjection;
	}

	void CCDirector::purgeCachedData(void)
	{
		CCLabelBMFont::purgeCachedData();
		CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	}

	float CCDirector::getZEye(void)
	{
		return (m_obWinSizeInPixels.height / 1.1566f);	
	}

	void CCDirector::setAlphaBlending(bool bOn)
	{
		if (bOn)
		{
			CrossEngine::cglEnable(GL_BLEND);
			CrossEngine::cglBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
		}
		else
		{
			CrossEngine::cglDisable(GL_BLEND);
		}
	}

	void CCDirector::setDepthTest(bool bOn)
	{
		if (bOn)
		{
			CrossEngine::cglEnable(GL_DEPTH_TEST);
			CrossEngine::cglDepthMask(GL_TRUE);
			CrossEngine::cglDepthFunc(GL_LEQUAL);
			CrossEngine::cglClearDepth(1.0f);
		}
		else
		{
			CrossEngine::cglDisable(GL_DEPTH_TEST);
		}
	}

	CCPoint CCDirector::convertToGL(const CCPoint& obPoint)
	{
		CCSize s = m_obWinSizeInPoints;
		float newY = s.height - obPoint.y;
		float newX = s.width - obPoint.x;

		CCPoint ret = CCPointZero;
		switch (m_eDeviceOrientation)
		{
		case CCDeviceOrientationPortrait:
			ret = ccp(obPoint.x, newY);
			break;
		case CCDeviceOrientationPortraitUpsideDown:
			ret = ccp(newX, obPoint.y);
			break;
		case CCDeviceOrientationLandscapeLeft:
			ret.x = obPoint.y;
			ret.y = obPoint.x;
			break;
		case CCDeviceOrientationLandscapeRight:
			ret.x = newY;
			ret.y = newX;
			break;
		}

		return ret;
	}

	CCPoint CCDirector::convertToUI(const CCPoint& obPoint)
	{
		CCSize winSize = m_obWinSizeInPoints;
		float oppositeX = winSize.width - obPoint.x;
		float oppositeY = winSize.height - obPoint.y;
		CCPoint uiPoint = CCPointZero;

		switch (m_eDeviceOrientation)
		{
		case CCDeviceOrientationPortrait:
			uiPoint = ccp(obPoint.x, oppositeY);
			break;
		case CCDeviceOrientationPortraitUpsideDown:
			uiPoint = ccp(oppositeX, obPoint.y);
			break;
		case CCDeviceOrientationLandscapeLeft:
			uiPoint = ccp(obPoint.y, obPoint.x);
			break;
		case CCDeviceOrientationLandscapeRight:
			// Can't use oppositeX/Y because x/y are flipped
			uiPoint = ccp(winSize.width - obPoint.y, winSize.height - obPoint.x);
			break;
		}

		return uiPoint;
	}

	CCSize CCDirector::getWinSize(void)
	{
		CCSize s = m_obWinSizeInPoints;

		if (m_eDeviceOrientation == CCDeviceOrientationLandscapeLeft
			|| m_eDeviceOrientation == CCDeviceOrientationLandscapeRight)
		{
			// swap x,y in landspace mode
			CCSize tmp = s;
			s.width = tmp.height;
			s.height = tmp.width;
		}

		return s;
	}

	CCSize CCDirector::getWinSizeInPixels()
	{
		CCSize s = getWinSize();

		s.width *= CC_CONTENT_SCALE_FACTOR();
		s.height *= CC_CONTENT_SCALE_FACTOR();

		return s;
	}

	// return the current frame size
	CCSize CCDirector::getDisplaySizeInPixels(void)
	{
		return m_obWinSizeInPixels;
	}

	// scene management

	void CCDirector::runWithMainScene(CCScene *pScene)
	{
		CCAssert(pScene != NULL, "running scene should not be null");
		CCAssert(m_pRunningMainScene == NULL, "m_pRunningMainScene should be null");

		pushMainScene(pScene);
	}

	void CCDirector::replaceMainScene(CCScene *pScene)
	{
		CCAssert(pScene != NULL, "the scene should not be null");

		unsigned int index = m_pobMainScenesStack->count();

		m_bSendCleanupToMainScene = true;
		m_pobMainScenesStack->replaceObjectAtIndex(index - 1, pScene);

		m_pNextMainScene = pScene;
	}

	void CCDirector::pushMainScene(CCScene *pScene)
	{
		CCAssert(pScene, "the scene should not null");

		m_bSendCleanupToMainScene = false;

		m_pobMainScenesStack->addObject(pScene);
		m_pNextMainScene = pScene;
	}

	bool CCDirector::popMainScene(void)
	{
		CCAssert(m_pRunningMainScene != NULL, "running scene should not null");

		m_pobMainScenesStack->removeLastObject();
		unsigned int c = m_pobMainScenesStack->count();

		if (c != 0)
		{
			m_bSendCleanupToMainScene = true;
			m_pNextMainScene = m_pobMainScenesStack->getObjectAtIndex(c - 1);
		}

		return c == 0;
	}

	bool CCDirector::isLastMainScene(void)
	{
		return m_pobMainScenesStack->count() == 1;
	}

	bool CCDirector::addSubScene(CCScene *pScene)
	{
		std::map<CCScene*, CCScene*>::const_iterator itScene = m_subScenes.find(pScene);
		if (itScene != m_subScenes.end()) return false;

		m_subScenes[pScene] = pScene;

		pScene->retain();
		pScene->onEnter();
		pScene->onEnterTransitionDidFinish();

		return true;
	}

	void CCDirector::removeSubScene(CCScene *pScene, bool bRelease)
	{
		std::map<CCScene*, CCScene*>::iterator itScene = m_subScenes.find(pScene);
		if (itScene == m_subScenes.end()) return;

		itScene->second->onExit();
		itScene->second->cleanup();

		if (bRelease) {
			itScene->second->release();
		}

		m_subScenes.erase(itScene);
	}

	void CCDirector::clearSubScenes(void)
	{
		m_subScenes.clear();
	}

	void CCDirector::end()
	{
		exit(0);
	}

	void CCDirector::resetDirector()
	{
		// don't release the event handlers
		// They are needed in case the director is run again
		CCTouchDispatcher::sharedDispatcher()->removeAllDelegates();

		if (m_pRunningMainScene)
		{
			m_pRunningMainScene->onExit();
			m_pRunningMainScene->cleanup();
			m_pRunningMainScene->release();
		}

		m_pRunningMainScene = NULL;
		m_pNextMainScene = NULL;

		// remove all objects, but don't release it.
		// runWithMainScene might be executed after 'end'.
		m_pobMainScenesStack->removeAllObjects();

		CCObject* pProjectionDelegate = (CCObject*)m_pProjectionDelegate;
		CC_SAFE_RELEASE_NULL(pProjectionDelegate);

		// purge bitmap cache
		CCLabelBMFont::purgeCachedData();

		// purge all managers
		CCAnimationCache::purgeSharedAnimationCache();
		CCSpriteFrameCache::purgeSharedSpriteFrameCache();
		CCActionManager::sharedManager()->purgeSharedManager();
		CCScheduler::purgeSharedScheduler();
		CCTextureCache::purgeSharedTextureCache();
	}

	void CCDirector::setNextMainScene(void)
	{
		bool runningIsTransition = dynamic_cast<CCTransitionScene*>(m_pRunningMainScene) != NULL;
		bool newIsTransition = dynamic_cast<CCTransitionScene*>(m_pNextMainScene) != NULL;

		// If it is not a transition, call onExit/cleanup
		if (!newIsTransition)
		{
			if (m_pRunningMainScene)
			{
				m_pRunningMainScene->onExit();
			}

			// issue #709. the root node (scene) should receive the cleanup message too
			// otherwise it might be leaked.
			if (m_bSendCleanupToMainScene && m_pRunningMainScene)
			{
				m_pRunningMainScene->cleanup();
			}
		}

		if (m_pRunningMainScene)
		{
			m_pRunningMainScene->release();
		}
		m_pRunningMainScene = m_pNextMainScene;
		m_pNextMainScene->retain();
		m_pNextMainScene = NULL;

		if ((!runningIsTransition) && m_pRunningMainScene)
		{
			m_pRunningMainScene->onEnter();
			m_pRunningMainScene->onEnterTransitionDidFinish();
		}
	}

	void CCDirector::pause(void)
	{
		if (m_bPaused)
		{
			return;
		}

		m_bPaused = true;
	}

	void CCDirector::resume(void)
	{
		if (!m_bPaused)
		{
			return;
		}

		if (CCTime::gettimeofdayCocos2d(m_pLastUpdate, NULL) != 0)
		{
			CCLOG("cocos2d: Director: Error in gettimeofday");
		}

		m_bPaused = false;
		m_fDeltaTime = 0;
	}

#if CC_DIRECTOR_FAST_FPS
	// display the FPS using a LabelAtlas
	// updates the FPS every frame
	void CCDirector::showFPS(void)
	{
		m_uFrames++;
		m_fAccumDt += m_fDeltaTime;

		if (m_fAccumDt > CC_DIRECTOR_FPS_INTERVAL)
		{
			m_fFrameRate = m_uFrames / m_fAccumDt;
			m_uFrames = 0;
			m_fAccumDt = 0;

			sprintf(m_pszFPS, "%.1f %.2f %.2f %d", m_fFrameRate, m_fRecvData, m_fSendData, m_uHeartTime);
		}

		if (!m_pFPSLabel)
		{
			CCSize winSize = getWinSize();
			float scalex = winSize.width/1024;
			float scaley = winSize.height/600;

			m_pFPSLabel = CCLabelAtlas::labelWithString("00.0", "fps_images.png", 16, 24, '.');
			m_pFPSLabel->setScale(0.75f*min(scalex, scaley));
			m_pFPSLabel->retain();
		}

		m_pFPSLabel->setString(m_pszFPS);
		m_pFPSLabel->visit();
	}

	void CCDirector::setRecvData(float recvData)
	{
		m_fRecvData = recvData;
	}

	void CCDirector::setSendData(float sendData)
	{
		m_fSendData = sendData;
	}

	void CCDirector::setHeartTime(unsigned int heartTime)
	{
		m_uHeartTime = heartTime;
	}
#endif // CC_DIRECTOR_FAST_FPS


	void CCDirector::showProfilers()
	{
#if CC_ENABLE_PROFILERS
		m_fAccumDtForProfiler += m_fDeltaTime;
		if (m_fAccumDtForProfiler > 1.0f)
		{
			m_fAccumDtForProfiler = 0;
			CCProfiler::sharedProfiler()->displayTimers();
		}
#endif
	}

	/***************************************************
	* mobile platforms specific functions
	**************************************************/

	bool CCDirector::setDirectorType(ccDirectorType obDirectorType)
	{
		CC_UNUSED_PARAM(obDirectorType);
		// we only support CCDisplayLinkDirector
		CCDirector::sharedDirector();

		return true;
	}

	float CCDirector::getContentScaleFactor(void)
	{
		return m_fContentScaleFactor;
	}

	void CCDirector::setContentScaleFactor(float scaleFactor)
	{
		if (scaleFactor != m_fContentScaleFactor)
		{
			m_fContentScaleFactor = scaleFactor;
			m_obWinSizeInPixels = CCSizeMake(m_obWinSizeInPoints.width * scaleFactor, m_obWinSizeInPoints.height * scaleFactor);

			// update projection
			setProjection(m_eProjection);
		}
	}

	CCNode* CCDirector::getNotificationNode() 
	{ 
		return m_pNotificationNode; 
	}

	void CCDirector::setNotificationNode(CCNode *node)
	{
		CC_SAFE_RELEASE(m_pNotificationNode);
		m_pNotificationNode = node;
		CC_SAFE_RETAIN(m_pNotificationNode);
	}

	void CCDirector::applyOrientation(void)
	{
		CCSize s = m_obWinSizeInPixels;
		float w = s.width / 2;
		float h = s.height / 2;

		// XXX it's using hardcoded values.
		// What if the the screen size changes in the future?
		switch (m_eDeviceOrientation)
		{
		case CCDeviceOrientationPortrait:
			// nothing
			break;
		case CCDeviceOrientationPortraitUpsideDown:
			// upside down
			CrossEngine::cglTranslatef(w,h,0);
			CrossEngine::cglRotatef(180,0,0,1);
			CrossEngine::cglTranslatef(-w,-h,0);
			break;
		case CCDeviceOrientationLandscapeRight:
			CrossEngine::cglTranslatef(w,h,0);
			CrossEngine::cglRotatef(90,0,0,1);
			CrossEngine::cglTranslatef(-h,-w,0);
			break;
		case CCDeviceOrientationLandscapeLeft:
			CrossEngine::cglTranslatef(w,h,0);
			CrossEngine::cglRotatef(-90,0,0,1);
			CrossEngine::cglTranslatef(-h,-w,0);
			break;
		}
	}

	ccDeviceOrientation CCDirector::getDeviceOrientation(void)
	{
		return m_eDeviceOrientation;
	}

	void CCDirector::setDeviceOrientation(ccDeviceOrientation kDeviceOrientation)
	{
		m_eDeviceOrientation = kDeviceOrientation;
		setProjection(m_eProjection);
	}

} 

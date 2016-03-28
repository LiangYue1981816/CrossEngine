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

#ifndef __CCDIRECTOR_H__
#define __CCDIRECTOR_H__

#include "platform/CCPlatformMacros.h"
#include "CCObject.h"
#include "ccTypes.h"
#include "CCGeometry.h"
#include "CCMutableArray.h"
#include "CCGeometry.h"
#include "CCGL.h"

namespace cocos2d {

	/** @typedef ccDirectorProjection
	Possible OpenGL projections used by director
	*/
	typedef enum {
		/// sets a 2D projection (orthogonal projection)
		kCCDirectorProjection2D,

		/// sets a 3D projection with a fovy=60, znear=0.5f and zfar=1500.
		kCCDirectorProjection3D,

		/// it calls "updateProjection" on the projection delegate.
		kCCDirectorProjectionCustom,

		/// Detault projection is 3D projection
		kCCDirectorProjectionDefault = kCCDirectorProjection3D,

		// backward compatibility stuff
		CCDirectorProjection2D = kCCDirectorProjection2D,
		CCDirectorProjection3D = kCCDirectorProjection3D,
		CCDirectorProjectionCustom = kCCDirectorProjectionCustom,
	} ccDirectorProjection;

	/** @typedef ccDirectorType
	Possible Director Types.
	@since v0.8.2
	*/
	typedef enum {
		/** Will use a Director that triggers the main loop from an NSTimer object
		*
		* Features and Limitations:
		* - Integrates OK with UIKit objects
		* - It the slowest director
		* - The interval update is customizable from 1 to 60
		*/
		kCCDirectorTypeNSTimer,

		/** will use a Director that triggers the main loop from a custom main loop.
		*
		* Features and Limitations:
		* - Faster than NSTimer Director
		* - It doesn't integrate well with UIKit objects
		* - The interval update can't be customizable
		*/
		kCCDirectorTypeMainLoop,

		/** Will use a Director that triggers the main loop from a thread, but the main loop will be executed on the main thread.
		*
		* Features and Limitations:
		* - Faster than NSTimer Director
		* - It doesn't integrate well with UIKit objects
		* - The interval update can't be customizable
		*/
		kCCDirectorTypeThreadMainLoop,

		/** Will use a Director that synchronizes timers with the refresh rate of the display.
		*
		* Features and Limitations:
		* - Faster than NSTimer Director
		* - Only available on 3.1+
		* - Scheduled timers & drawing are synchronizes with the refresh rate of the display
		* - Integrates OK with UIKit objects
		* - The interval update can be 1/60, 1/30, 1/15
		*/	
		kCCDirectorTypeDisplayLink,

		/** Default director is the NSTimer directory */
		kCCDirectorTypeDefault = kCCDirectorTypeNSTimer,

		// backward compatibility stuff
		CCDirectorTypeNSTimer = kCCDirectorTypeNSTimer,
		CCDirectorTypeMainLoop = kCCDirectorTypeMainLoop,
		CCDirectorTypeThreadMainLoop = kCCDirectorTypeThreadMainLoop,
		CCDirectorTypeDisplayLink = kCCDirectorTypeDisplayLink,
		CCDirectorTypeDefault =kCCDirectorTypeDefault,
	} ccDirectorType;

	/** @typedef ccDeviceOrientation
	Possible device orientations
	*/
	typedef enum {
		/// Device oriented vertically, home button on the bottom
		kCCDeviceOrientationPortrait = 0, // UIDeviceOrientationPortrait,	
		/// Device oriented vertically, home button on the top
		kCCDeviceOrientationPortraitUpsideDown = 1, // UIDeviceOrientationPortraitUpsideDown,
		/// Device oriented horizontally, home button on the right
		kCCDeviceOrientationLandscapeLeft = 2, // UIDeviceOrientationLandscapeLeft,
		/// Device oriented horizontally, home button on the left
		kCCDeviceOrientationLandscapeRight = 3, // UIDeviceOrientationLandscapeRight,

		// Backward compatibility stuff
		CCDeviceOrientationPortrait = kCCDeviceOrientationPortrait,
		CCDeviceOrientationPortraitUpsideDown = kCCDeviceOrientationPortraitUpsideDown,
		CCDeviceOrientationLandscapeLeft = kCCDeviceOrientationLandscapeLeft,
		CCDeviceOrientationLandscapeRight = kCCDeviceOrientationLandscapeRight,
	} ccDeviceOrientation;

	class CCLabelAtlas;
	class CCScene;
	class CCProjectionProtocol;
	class CCNode;

	/**
	@brief Class that creates and handle the main Window and manages how
	and when to execute the Scenes.

	The CCDirector is also responsible for:
	- initializing the OpenGL context
	- setting the OpenGL pixel format (default on is RGB565)
	- setting the OpenGL buffer depth (default one is 0-bit)
	- setting the projection (default one is 3D)
	- setting the orientation (default one is Protrait)

	Since the CCDirector is a singleton, the standard way to use it is by calling:
	_ CCDirector::sharedDirector()->methodName();

	The CCDirector also sets the default OpenGL context:
	- GL_TEXTURE_2D is enabled
	- GL_VERTEX_ARRAY is enabled
	- GL_COLOR_ARRAY is enabled
	- GL_TEXTURE_COORD_ARRAY is enabled
	*/
	class CC_DLL CCDirector : public CCObject
	{
	public: 
		virtual bool init(void);
		virtual ~CCDirector(void);
		CCDirector(void);

		/** Get current running Scene. Director can only run one Scene at the time */
		inline CCScene* getRunningMainScene(void) { return m_pRunningMainScene; }

		/** Whether or not to display the FPS on the bottom-left corner */
		inline bool isDisplayFPS(void) { return m_bDisplayFPS; }
		/** Display the FPS on the bottom-left corner */
		inline void setDisplayFPS(bool bDisplayFPS) { m_bDisplayFPS = bDisplayFPS; }

		inline bool isNextDeltaTimeZero(void) { return m_bNextDeltaTimeZero; }
		void setNextDeltaTimeZero(bool bNextDeltaTimeZero);

		/** Whether or not the Director is paused */
		inline bool isPaused(void) { return m_bPaused; }

		/** How many frames were called since the director started */
		inline unsigned int getFrames(void) { return m_uFrames; }

		/** Set OpenGL viewport */
		void setViewport(int width, int height, float scaleSize);

		/** Sets an OpenGL projection
		@since v0.8.2
		*/
		inline ccDirectorProjection getProjection(void) { return m_eProjection; }
		void setProjection(ccDirectorProjection kProjection);    

		/** Whether or not the replaced scene will receive the cleanup message.
		If the new scene is pushed, then the old scene won't receive the "cleanup" message.
		If the new scene replaces the old one, the it will receive the "cleanup" message.
		@since v0.99.0
		*/
		inline bool isSendCleanupToMainScene(void) { return m_bSendCleanupToMainScene; }

		/** This object will be visited after the main scene is visited.
		This object MUST implement the "visit" selector.
		Useful to hook a notification object, like CCNotifications (http://github.com/manucorporat/CCNotifications)
		@since v0.99.5
		*/
		CCNode* getNotificationNode();
		void setNotificationNode(CCNode *node);

		/** returns the size of the OpenGL view in points.
		It takes into account any possible rotation (device orientation) of the window
		*/
		CCSize getWinSize(void);

		/** returns the size of the OpenGL view in pixels.
		It takes into account any possible rotation (device orientation) of the window.
		On Mac winSize and winSizeInPixels return the same value.
		*/
		CCSize getWinSizeInPixels(void);

		/** returns the display size of the OpenGL view in pixels.
		It doesn't take into account any possible rotation of the window.
		*/
		CCSize getDisplaySizeInPixels(void);

		/** converts a UIKit coordinate to an OpenGL coordinate
		Useful to convert (multi) touches coordinates to the current layout (portrait or landscape)
		*/
		CCPoint convertToGL(const CCPoint& obPoint);

		/** converts an OpenGL coordinate to a UIKit coordinate
		Useful to convert node points to window points for calls such as glScissor
		*/
		CCPoint convertToUI(const CCPoint& obPoint);

		/** get eye position */
		float getZEye(void);

		/**Enters the Director's main loop with the given Scene. 
		* Call it to run only your FIRST scene.
		* Don't call it if there is already a running scene.
		*/
		void runWithMainScene(CCScene *pScene);

		/**Suspends the execution of the running scene, pushing it on the stack of suspended scenes.
		* The new scene will be executed.
		* Try to avoid big stacks of pushed scenes to reduce memory allocation. 
		* ONLY call it if there is a running scene.
		*/
		void pushMainScene(CCScene *pScene);

		/**Pops out a scene from the queue.
		* This scene will replace the running one.
		* The running scene will be deleted. If there are no more scenes in the stack the execution is terminated.
		* ONLY call it if there is a running scene.
		*/
		bool popMainScene(void);

		/** Replaces the running scene with a new one. The running scene is terminated.
		* ONLY call it if there is a running scene.
		*/
		void replaceMainScene(CCScene *pScene);

		/** Judge current main scene is the last one
		*/
		bool isLastMainScene(void);

		/** Add sub scene
		*/
		bool addSubScene(CCScene *pScene);

		/** Remove sub scene
		*/
		void removeSubScene(CCScene *pScene, bool bRelease);

		/** Clear sub scenes
		*/
		void clearSubScenes(void);

		/** Ends the execution, releases the running scene.
		It doesn't remove the OpenGL view from its parent. You have to do it manually.
		*/

		/* end is key word of lua, use other name to export to lua. */
		inline void endToLua(void){end();}
		void end(void);

		/** Pauses the running scene.
		The running scene will be _drawed_ but all scheduled timers will be paused
		While paused, the draw rate will be 4 FPS to reduce CPU consumption
		*/
		void pause(void);

		/** Resumes the paused scene
		The scheduled timers will be activated again.
		The "delta time" will be 0 (as if the game wasn't paused)
		*/
		void resume(void);

		/** Update scene
		*/
		void update(ccTime dt);
		void updateScene(ccTime dt);

		/** Draw the scene.
		This method is called every frame. Don't call it manually.
		*/
		void drawScene(void);

		/** Removes cached all cocos2d cached data.
		It will purge the CCTextureCache, CCSpriteFrameCache, CCLabelBMFont cache
		@since v0.99.3
		*/
		void purgeCachedData(void);

		/** sets the OpenGL default values */
		void setGLDefaultValues(void);

		/** enables/disables OpenGL alpha blending */
		void setAlphaBlending(bool bOn);

		/** enables/disables OpenGL depth test */
		void setDepthTest(bool bOn);

		// Profiler
		void showProfilers(void);

		/** rotates the screen if an orientation different than Portrait is used */
		void applyOrientation(void);

		ccDeviceOrientation getDeviceOrientation(void);
		void setDeviceOrientation(ccDeviceOrientation kDeviceOrientation);

		/** The size in pixels of the surface. It could be different than the screen size.
		High-res devices might have a higher surface size than the screen size.
		Only available when compiled using SDK >= 4.0.
		@since v0.99.4
		*/
		void setContentScaleFactor(float scaleFactor);
		float getContentScaleFactor(void);

		/** There are 4 types of Director.
		- kCCDirectorTypeNSTimer (default)
		- kCCDirectorTypeMainLoop
		- kCCDirectorTypeThreadMainLoop
		- kCCDirectorTypeDisplayLink

		Each Director has it's own benefits, limitations.
		Now we only support DisplayLink director, so it has not effect. 

		This method should be called before any other call to the director.

		@since v0.8.2
		*/
		static bool setDirectorType(ccDirectorType obDirectorType);

	public:
		/** returns a shared instance of the director */
		static CCDirector* sharedDirector(void);
		void resetDirector();

	protected:
		void setNextMainScene(void);

	public:
#if CC_DIRECTOR_FAST_FPS
		void showFPS(void);
		void setRecvData(float recvData);
		void setSendData(float sendData);
		void setHeartTime(unsigned int heartTime);
#else
		void showFPS(void) {}
		void setRecvData(float recvData) {}
		void setSendData(float sendData) {}
		void setHeartTime(unsigned int heartTime) {}
#endif // CC_DIRECTOR_FAST_FPS

	protected:
		void calculateDeltaTime();

		/* landscape mode ? */
		bool m_bLandscape;
		bool m_bDisplayFPS;
		ccTime m_fAccumDt;
		ccTime m_fFrameRate;
		ccTime m_fRecvData;
		ccTime m_fSendData;
		unsigned int m_uHeartTime;
		CCLabelAtlas *m_pFPSLabel;

		/* is the running scene paused */
		bool m_bPaused;

		/* How many frames were called since the director started */
		unsigned int m_uTotalFrames;
		unsigned int m_uFrames;

		/* The running scene */
		CCScene *m_pRunningMainScene;

		/* will be the next 'runningScene' in the next frame
		nextScene is a weak reference. */
		CCScene *m_pNextMainScene;

		/* If YES, then "old" scene will receive the cleanup message */
		bool m_bSendCleanupToMainScene;

		/* scheduled scenes */
		CCMutableArray<CCScene*> *m_pobMainScenesStack;

		/* scenes */
		std::map<CCScene*, CCScene*> m_subScenes;

		/* last time the main loop was updated */
		struct cc_timeval *m_pLastUpdate;

		/* delta time since last tick to main loop */
		ccTime m_fDeltaTime;

		/* whether or not the next delta time will be zero */
		bool m_bNextDeltaTimeZero;

		/* projection used */
		ccDirectorProjection m_eProjection;

		/* window size in points */
		CCSize m_obWinSizeInPoints;

		/* window size in pixels */
		CCSize m_obWinSizeInPixels;

		/* content scale factor */
		float m_fContentScaleFactor;

		/* store the fps string */
		char *m_pszFPS;

		/* This object will be visited after the scene. Useful to hook a notification node */
		CCNode *m_pNotificationNode;

		/* Projection protocol delegate */
		CCProjectionProtocol *m_pProjectionDelegate;

		/* The device orientation */
		ccDeviceOrientation	m_eDeviceOrientation;

#if CC_ENABLE_PROFILERS
		ccTime m_fAccumDtForProfiler;
#endif
	};

}

#endif // __CCDIRECTOR_H__

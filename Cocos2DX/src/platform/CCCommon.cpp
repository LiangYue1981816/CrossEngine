/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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

#include "CCCommon.h"

#define MAX_LEN (cocos2d::kMaxLogLen + 1)

/****************************************************
* win32
***************************************************/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include <winsock2.h>
#include <windows.h>
#include "CCStdC.h"

namespace cocos2d {

	void CCLog(const char * pszFormat, ...)
	{
		char szBuf[MAX_LEN];

		va_list ap;
		va_start(ap, pszFormat);
		vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
		va_end(ap);

		WCHAR wszBuf[MAX_LEN] = {0};
		MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
		OutputDebugStringW(wszBuf);
		OutputDebugStringA("\n");
	}

	void CCMessageBox(const char * pszMsg, const char * pszTitle)
	{
		MessageBoxA(NULL, pszMsg, pszTitle, MB_OK);
	}

}

#endif  // CC_PLATFORM_WIN32

/****************************************************
* ios
***************************************************/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

// implement in CCCommon_iso.mm

#endif  // CC_PLATFORM_IOS

/****************************************************
* android
***************************************************/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <android/log.h>
#include <stdio.h>
#include <jni.h>

namespace cocos2d {

	void CCLog(const char * pszFormat, ...)
	{
		char buf[MAX_LEN];

		va_list args;
		va_start(args, pszFormat);    	
		vsprintf(buf, pszFormat, args);
		va_end(args);

		__android_log_print(ANDROID_LOG_DEBUG, "cocos2d-x debug info", "%s", buf);
	}

	void CCMessageBox(const char * pszMsg, const char * pszTitle)
	{
		//showMessageBoxJNI(pszMsg, pszTitle);
	}

}

#endif // CC_PLATFORM_ANDROID

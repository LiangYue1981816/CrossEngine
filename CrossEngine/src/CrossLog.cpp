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

	static BOOL bEnableLog = FALSE;
	static CHAR szLogFileName[_MAX_STRING];
	static CHAR szLogString[1024 * 1024];

	//
	// 设置日志状态
	//
	_CrossExport VOID EnableLog(BOOL bEnable, const CHAR *szFileName)
	{
		bEnableLog = bEnable;

		if (szFileName) {
			strcpy(szLogFileName, szFileName);
			remove(szLogFileName);
		}
		else {
			strcpy(szLogFileName, "");
		}
	}

	//
	// 输出信息日志
	//
	_CrossExport VOID WriteLogI(const CHAR *szFormat, ...)
	{
		ASSERT(szFormat);

		if (bEnableLog) {
			va_list vaList;
			va_start(vaList, szFormat);
			vsprintf(szLogString, szFormat, vaList);
			va_end(vaList);

			if (FILE *pFile = fopen(szLogFileName, "a+")) {
				fprintf(pFile, "I: %s", szLogString);
				fclose(pFile);
			}

#if defined _PLATFORM_ANDROID_

			LOGI("%s", szLogString);

#else

			printf("I: %s", szLogString);

#endif
		}
	}

	//
	// 输出警告日志
	//
	_CrossExport VOID WriteLogW(const CHAR *szFormat, ...)
	{
		ASSERT(szFormat);

		if (bEnableLog) {
			va_list vaList;
			va_start(vaList, szFormat);
			vsprintf(szLogString, szFormat, vaList);
			va_end(vaList);

			if (FILE *pFile = fopen(szLogFileName, "a+")) {
				fprintf(pFile, "W: %s", szLogString);
				fclose(pFile);
			}

#if defined _PLATFORM_ANDROID_

			LOGI("%s", szLogString);

#else

			printf("W: %s", szLogString);

#endif
		}
	}

	//
	// 输出错误日志
	//
	_CrossExport VOID WriteLogE(const CHAR *szFormat, ...)
	{
		ASSERT(szFormat);

		if (bEnableLog) {
			va_list vaList;
			va_start(vaList, szFormat);
			vsprintf(szLogString, szFormat, vaList);
			va_end(vaList);

			if (FILE *pFile = fopen(szLogFileName, "a+")) {
				fprintf(pFile, "E: %s", szLogString);
				fclose(pFile);
			}

#if defined _PLATFORM_ANDROID_

			LOGI("%s", szLogString);

#else

			printf("E: %s", szLogString);

#endif
		}
	}

}

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

#pragma once


#include "CrossEngine.h"


namespace CrossEngine {

	typedef struct {
		CHAR szName[32];                                                                           // 采样名称
		DWORD dwCallCount;                                                                         // 调用次数
		DWORD dwTotalTime;                                                                         // 调用时间
		DWORD dwTime;                                                                              // 调用时间
		DWORD dwTick;                                                                              // 时间戳
	} PROFILER_SAMPLE;

	typedef std::map<DWORD, PROFILER_SAMPLE> ProfilerSampleMap;                                    // 采样集合

	class _CrossExport CProfilerSample
	{
		// 构造/析构函数
	public:
		CProfilerSample(const CHAR *szName);
		virtual ~CProfilerSample(VOID);


		// 属性
	protected:
		CHAR m_szName[32];                                                                         // 采样名称
	};

#ifdef _PROFILER_
	#define PROFILER(name) CProfilerSample(name)
#else
	#define PROFILER(name)
#endif

	const ProfilerSampleMap& GetProfilerSamples(VOID);                                             // 获得采样集合
}

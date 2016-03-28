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
		CHAR szName[32];                                                                           // ��������
		DWORD dwCallCount;                                                                         // ���ô���
		DWORD dwTotalTime;                                                                         // ����ʱ��
		DWORD dwTime;                                                                              // ����ʱ��
		DWORD dwTick;                                                                              // ʱ���
	} PROFILER_SAMPLE;

	typedef std::map<DWORD, PROFILER_SAMPLE> ProfilerSampleMap;                                    // ��������

	class _CrossExport CProfilerSample
	{
		// ����/��������
	public:
		CProfilerSample(const CHAR *szName);
		virtual ~CProfilerSample(VOID);


		// ����
	protected:
		CHAR m_szName[32];                                                                         // ��������
	};

#ifdef _PROFILER_
	#define PROFILER(name) CProfilerSample(name)
#else
	#define PROFILER(name)
#endif

	const ProfilerSampleMap& GetProfilerSamples(VOID);                                             // ��ò�������
}

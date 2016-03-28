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

	class CProfiler
	{
		// 构造/析构函数
	public:
		CProfiler(VOID)
		{

		}

		~CProfiler(VOID)
		{
			WriteLogI("\n");
			WriteLogI("================= Profiler Information =================\n");
			for (ProfilerSampleMap::const_iterator itSample = m_samples.begin(); itSample != m_samples.end(); ++itSample) {
				WriteLogI("Sample: name = %s calls = %d total = %dms average = %dms\n", itSample->second.szName, itSample->second.dwCallCount, itSample->second.dwTotalTime, itSample->second.dwTotalTime / itSample->second.dwCallCount);
			}
			WriteLogI("======================================================\n");
		}


		// 方法
	public:
		inline VOID Begin(const CHAR *szName)
		{
			ASSERT(szName);

			DWORD dwName = HashValue(szName);
			ProfilerSampleMap::const_iterator itSample = m_samples.find(dwName);

			if (itSample == m_samples.end()) {
				strcpy(m_samples[dwName].szName, szName);
				m_samples[dwName].dwTime = 0;
				m_samples[dwName].dwCallCount = 0;
				m_samples[dwName].dwTotalTime = 0;
			}

			m_samples[dwName].dwTick = tick();
		}

		inline VOID End(const CHAR *szName)
		{
			ASSERT(szName);

			DWORD dwName = HashValue(szName);
			ProfilerSampleMap::const_iterator itSample = m_samples.find(dwName);
			if (itSample == m_samples.end()) return;

			m_samples[dwName].dwTime = tick() - m_samples[dwName].dwTick;
			m_samples[dwName].dwTotalTime += m_samples[dwName].dwTime;
			m_samples[dwName].dwCallCount += 1;
		}

		inline const ProfilerSampleMap& GetSamples(VOID) const
		{
			return m_samples;
		}


		// 属性
	protected:
		ProfilerSampleMap m_samples;
	};

	static CProfiler profiler;

	CProfilerSample::CProfilerSample(const CHAR *szName)
	{
		ASSERT(szName);
		strcpy(m_szName, szName);
		profiler.Begin(m_szName);
	}

	CProfilerSample::~CProfilerSample(VOID)
	{
		profiler.End(m_szName);
	}

	const ProfilerSampleMap& GetProfilerSamples(VOID)
	{
		return profiler.GetSamples();
	}

}

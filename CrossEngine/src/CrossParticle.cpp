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

	CParticle::CParticle(VOID)
		: pEmitter(NULL)
		, pEmitParticleSystem(NULL)
		, pNext(NULL)
	{
		Reset();
	}
	
	CParticle::~CParticle(VOID)
	{

	}

	//
	// ÖØÖÃ
	//
	VOID CParticle::Reset(VOID)
	{
		bKeepLocal = FALSE;

		currTime = 0.0f;
		liveTime = 0.0f;

		radian = 0.0f;

		Vec3Set(&localScale, 1.0f, 1.0f, 1.0f);
		Vec3Set(&localPosition, 0.0f, 0.0f, 0.0f);
		QuatSet(&localOrientation, 0.0f, 0.0f, 0.0f, 1.0f);
		Vec3Set(&localVelocity, 0.0f, 0.0f, 0.0f);

		Vec4Set(&color, 1.0f, 1.0f, 1.0f, 1.0f);
		Vec2Set(&uvOffset, 0.0f, 0.0f);

		Vec2Set(&texScrollOffset, 0.0f, 0.0f);
		Vec2Set(&texSequenceScale, 1.0f, 1.0f);
		Vec2Set(&texSequenceOffset, 0.0f, 0.0f);

		deltaScale.clear();
		deltaColor.clear();
		deltaForce.clear();

		deltaRotateSpeed.clear();
		deltaVelocityRatios.clear();

		texSequenceTime.clear();
	}

}

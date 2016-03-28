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

	CLightGrid::CLightGrid(CSceneManager *pSceneManager)
		: m_pSceneManager(pSceneManager)
	{
		Init();
	}

	CLightGrid::~CLightGrid(VOID)
	{
		Free();
	}

	//
	// 初始化
	//
	VOID CLightGrid::Init(VOID)
	{
		m_volumes.clear();
	}

	//
	// 释放
	//
	VOID CLightGrid::Free(VOID)
	{

	}

	//
	// 从数据流加载资源
	//
	BOOL CLightGrid::LoadFromStream(CStream *pStream)
	{
		return TRUE;
	}

	//
	// 获得光照参数
	//
	BOOL CLightGrid::GetLightParams(DWORD dwChannel, const VEC3 *position, VEC4 *direction, VEC3 *ambient, VEC3 *diffuse, VEC3 *specular, VEC3 *rim, VEC3 *skyLower, VEC3 *skyUpper, VEC3 *indirectUp, VEC3 *indirectDown, VEC3 *indirectLeft, VEC3 *indirectRight, VEC3 *indirectFront, VEC3 *indirectBack) const
	{
		ASSERT(position);

		//
		// 1. 通道检查
		//
		if ((LIGHT_CHANNEL & dwChannel) == 0) {
			return FALSE;
		}

		//
		// 2. 获得光照参数
		//
		for (LightVolumeSet::const_iterator itVolume = m_volumes.begin(); itVolume != m_volumes.end(); ++itVolume) {
			const LIGHT_VOLUME *pVolume = *itVolume;
			ASSERT(pVolume);

			if (IsPointInAABB(&pVolume->aabb, (*position)[0], (*position)[1], (*position)[2])) {
				FLOAT posx = ((*position)[0] - pVolume->aabb.minVertex[0]) / pVolume->stepx;
				FLOAT posy = ((*position)[1] - pVolume->aabb.minVertex[1]) / pVolume->stepy;
				FLOAT posz = ((*position)[2] - pVolume->aabb.minVertex[2]) / pVolume->stepz;

				INT x = FastFloor(posx);
				INT y = FastFloor(posy);
				INT z = FastFloor(posz);

				FLOAT factor;
				FLOAT factorx = posx - x;
				FLOAT factory = posy - y;
				FLOAT factorz = posz - z;

				VEC3 _direction;
				VEC3 _ambient;
				VEC3 _diffuse;
				VEC3 _indirectUp;
				VEC3 _indirectDown;
				VEC3 _indirectLeft;
				VEC3 _indirectRight;
				VEC3 _indirectFront;
				VEC3 _indirectBack;
				Vec3Zero(&_direction);
				Vec3Zero(&_ambient);
				Vec3Zero(&_diffuse);
				Vec3Zero(&_indirectUp);
				Vec3Zero(&_indirectDown);
				Vec3Zero(&_indirectLeft);
				Vec3Zero(&_indirectRight);
				Vec3Zero(&_indirectFront);
				Vec3Zero(&_indirectBack);

				for (INT xx = 0; xx <= 1; xx++) {
					factorx = 1.0f - factorx;

					for (INT yy = 0; yy <= 1; yy++) {
						factory = 1.0f - factory;

						for (INT zz = 0; zz <= 1; zz++) {
							factorz = 1.0f - factorz;
							factor  = factorx * factory * factorz;

							if (LIGHT_POINT *pLightPoint = &pVolume->pppLightPoints[x + xx][y + yy][z + zz]) {
								VEC3 __direction;
								VEC3 __ambient;
								VEC3 __diffuse;
								VEC3 __indirectUp;
								VEC3 __indirectDown;
								VEC3 __indirectLeft;
								VEC3 __indirectRight;
								VEC3 __indirectFront;
								VEC3 __indirectBack;

								Vec3LatLongToDirection(&__direction, pLightPoint->direction);
								Vec3BytesToColor(&__ambient, pLightPoint->ambient);
								Vec3BytesToColor(&__diffuse, pLightPoint->diffuse);
								Vec3BytesToColor(&__indirectUp, pLightPoint->indirectup);
								Vec3BytesToColor(&__indirectDown, pLightPoint->indirectdown);
								Vec3BytesToColor(&__indirectLeft, pLightPoint->indirectleft);
								Vec3BytesToColor(&__indirectRight, pLightPoint->indirectright);
								Vec3BytesToColor(&__indirectFront, pLightPoint->indirectfront);
								Vec3BytesToColor(&__indirectBack, pLightPoint->indirectback);

								Vec3Ma(&_direction, &_direction, &__direction, factor);
								Vec3Ma(&_ambient, &_ambient, &__ambient, factor);
								Vec3Ma(&_diffuse, &_diffuse, &__diffuse, factor);
								Vec3Ma(&_indirectUp, &_diffuse, &__indirectUp, factor);
								Vec3Ma(&_indirectDown, &_diffuse, &__indirectDown, factor);
								Vec3Ma(&_indirectLeft, &_diffuse, &__indirectLeft, factor);
								Vec3Ma(&_indirectRight, &_diffuse, &__indirectRight, factor);
								Vec3Ma(&_indirectFront, &_diffuse, &__indirectFront, factor);
								Vec3Ma(&_indirectBack, &_diffuse, &__indirectBack, factor);
							}
						}
					}
				}

				Vec3Normalize(&_direction);
				Vec3Clamp(&_ambient);
				Vec3Clamp(&_diffuse);
				Vec3Clamp(&_indirectUp);
				Vec3Clamp(&_indirectDown);
				Vec3Clamp(&_indirectLeft);
				Vec3Clamp(&_indirectRight);
				Vec3Clamp(&_indirectFront);
				Vec3Clamp(&_indirectBack);

				static const FLOAT specularFactor = 0.8f;
				static const FLOAT rimFactor = 0.2f;
				static const FLOAT skyLowerFactor = 0.2f;
				static const FLOAT skyUpperFactor = 0.4f;

				if (direction) Vec4Set(direction, _direction[0], _direction[1], _direction[2], 0.0f);
				if (ambient) Vec3Set(ambient, _ambient[0], _ambient[1], _ambient[2]);
				if (diffuse) Vec3Set(diffuse, _diffuse[0], _diffuse[1], _diffuse[2]);
				if (specular) Vec3Set(specular, specularFactor * _diffuse[0], specularFactor * _diffuse[1], specularFactor * _diffuse[2]);
				if (rim) Vec3Set(rim, rimFactor * _diffuse[0], rimFactor * _diffuse[1], rimFactor * _diffuse[2]);
				if (skyLower) Vec3Set(skyLower, skyLowerFactor * _diffuse[0], skyLowerFactor * _diffuse[1], skyLowerFactor * _diffuse[2]);
				if (skyUpper) Vec3Set(skyUpper, skyUpperFactor * _diffuse[0], skyUpperFactor * _diffuse[1], skyUpperFactor * _diffuse[2]);
				if (indirectUp) Vec3Set(indirectUp, _indirectUp[0], _indirectUp[1], _indirectUp[2]);
				if (indirectDown) Vec3Set(indirectDown, _indirectDown[0], _indirectDown[1], _indirectDown[2]);
				if (indirectLeft) Vec3Set(indirectLeft, _indirectLeft[0], _indirectLeft[1], _indirectLeft[2]);
				if (indirectRight) Vec3Set(indirectRight, _indirectRight[0], _indirectRight[1], _indirectRight[2]);
				if (indirectFront) Vec3Set(indirectFront, _indirectFront[0], _indirectFront[1], _indirectFront[2]);
				if (indirectBack) Vec3Set(indirectBack, _indirectBack[0], _indirectBack[1], _indirectBack[2]);

				return TRUE;
			}
		}

		return FALSE;
	}

}

//////////////////////////////////////////////////////////////////////////
// CrossEngine D3D Math
//
// Author: LiangYue
//========================================================================

#pragma once


#include "CrossEngine.h"


//===========================================================================
//
// Type definitions from D3D9
//
//===========================================================================
struct D3DXVECTOR2;
struct D3DXVECTOR3;
struct D3DXVECTOR4;
struct D3DXQUATERNION;
struct D3DXCOLOR;
struct D3DXMATRIX;


inline FLOAT D3DXVec2Length(const D3DXVECTOR2 *pV);
inline FLOAT D3DXVec2LengthSq(const D3DXVECTOR2 *pV);
inline FLOAT D3DXVec2Dot(const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2);
inline FLOAT D3DXVec2CCW(const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2);
inline D3DXVECTOR2* D3DXVec2Add(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2);
inline D3DXVECTOR2* D3DXVec2Subtract(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2);
inline D3DXVECTOR2* D3DXVec2Minimize(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2);
inline D3DXVECTOR2* D3DXVec2Maximize(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2);
inline D3DXVECTOR2* D3DXVec2Scale(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV, FLOAT s);
inline D3DXVECTOR2* D3DXVec2Lerp(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2, FLOAT s);
inline D3DXVECTOR2* D3DXVec2Normalize(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV);
inline D3DXVECTOR2* D3DXVec2BaryCentric(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2, const D3DXVECTOR2 *pV3, FLOAT f, FLOAT g);
inline D3DXVECTOR4* D3DXVec2Transform(D3DXVECTOR4 *pOut, const D3DXVECTOR2 *pV, const D3DXMATRIX *pM);
inline D3DXVECTOR2* D3DXVec2TransformCoord(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV, const D3DXMATRIX *pM);
inline D3DXVECTOR2* D3DXVec2TransformNormal(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV, const D3DXMATRIX *pM);
inline D3DXVECTOR4* D3DXVec2TransformArray(D3DXVECTOR4 *pOut, UINT OutStride, const D3DXVECTOR2 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n);
inline D3DXVECTOR2* D3DXVec2TransformCoordArray(D3DXVECTOR2 *pOut, UINT OutStride, const D3DXVECTOR2 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n);
inline D3DXVECTOR2* D3DXVec2TransformNormalArray(D3DXVECTOR2 *pOut, UINT OutStride, const D3DXVECTOR2 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n);

inline FLOAT D3DXVec3Length(const D3DXVECTOR3 *pV);
inline FLOAT D3DXVec3LengthSq(const D3DXVECTOR3 *pV);
inline FLOAT D3DXVec3Dot(const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2);
inline D3DXVECTOR3* D3DXVec3Cross(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2);
inline D3DXVECTOR3* D3DXVec3Add(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2);
inline D3DXVECTOR3* D3DXVec3Subtract(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2);
inline D3DXVECTOR3* D3DXVec3Minimize(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2);
inline D3DXVECTOR3* D3DXVec3Maximize(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2);
inline D3DXVECTOR3* D3DXVec3Scale(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, FLOAT s);
inline D3DXVECTOR3* D3DXVec3Lerp(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2, FLOAT s);
inline D3DXVECTOR3* D3DXVec3Normalize(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV);
inline D3DXVECTOR3* D3DXVec3BaryCentric(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2, const D3DXVECTOR3 *pV3, FLOAT f, FLOAT g);
inline D3DXVECTOR4* D3DXVec3Transform(D3DXVECTOR4 *pOut, const D3DXVECTOR3 *pV, const D3DXMATRIX *pM);
inline D3DXVECTOR3* D3DXVec3TransformCoord(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXMATRIX *pM);
inline D3DXVECTOR3* D3DXVec3TransformNormal(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXMATRIX *pM);
inline D3DXVECTOR4* D3DXVec3TransformArray(D3DXVECTOR4 *pOut, UINT OutStride, const D3DXVECTOR3 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n);
inline D3DXVECTOR3* D3DXVec3TransformCoordArray(D3DXVECTOR3 *pOut, UINT OutStride, const D3DXVECTOR3 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n);
inline D3DXVECTOR3* D3DXVec3TransformNormalArray(D3DXVECTOR3 *pOut, UINT OutStride, const D3DXVECTOR3 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n);
inline D3DXVECTOR3* D3DXVec3Project(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXMATRIX *pProjection, const D3DXMATRIX *pView, const D3DXMATRIX *pWorld, INT viewportX, INT viewportY, INT viewportWidth, INT viewportHeight);
inline D3DXVECTOR3* D3DXVec3ProjectArray(D3DXVECTOR3 *pOut, UINT OutStride,const D3DXVECTOR3 *pV, UINT VStride, const D3DXMATRIX *pProjection, const D3DXMATRIX *pView, const D3DXMATRIX *pWorld, INT viewportX, INT viewportY, INT viewportWidth, INT viewportHeight, UINT n);

inline FLOAT D3DXVec4Length(const D3DXVECTOR4 *pV);
inline FLOAT D3DXVec4LengthSq(const D3DXVECTOR4 *pV);
inline FLOAT D3DXVec4Dot(const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2);
inline D3DXVECTOR4* D3DXVec4Add(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2);
inline D3DXVECTOR4* D3DXVec4Subtract(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2);
inline D3DXVECTOR4* D3DXVec4Minimize(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2);
inline D3DXVECTOR4* D3DXVec4Maximize(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2);
inline D3DXVECTOR4* D3DXVec4Scale(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV, FLOAT s);
inline D3DXVECTOR4* D3DXVec4Lerp(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2, FLOAT s);
inline D3DXVECTOR4* D3DXVec4Normalize(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV);
inline D3DXVECTOR4* D3DXVec4BaryCentric(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2, const D3DXVECTOR4 *pV3, FLOAT f, FLOAT g);
inline D3DXVECTOR4* D3DXVec4Transform(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV, const D3DXMATRIX *pM);
inline D3DXVECTOR4* D3DXVec4TransformArray(D3DXVECTOR4 *pOut, UINT OutStride, const D3DXVECTOR4 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n);

inline D3DXMATRIX* D3DXMatrixIdentity(D3DXMATRIX *pOut);
inline BOOL D3DXMatrixIsIdentity(const D3DXMATRIX *pM);
inline HRESULT D3DXMatrixDecompose(D3DXVECTOR3 *pOutScale, D3DXQUATERNION *pOutRotation, D3DXVECTOR3 *pOutTranslation, const D3DXMATRIX *pM);
inline D3DXMATRIX* D3DXMatrixTranspose(D3DXMATRIX *pOut, const D3DXMATRIX *pM);
inline D3DXMATRIX* D3DXMatrixMultiply(D3DXMATRIX *pOut, const D3DXMATRIX *pM1, const D3DXMATRIX *pM2);
inline D3DXMATRIX* D3DXMatrixMultiplyTranspose(D3DXMATRIX *pOut, const D3DXMATRIX *pM1, const D3DXMATRIX *pM2);
inline D3DXMATRIX* D3DXMatrixInverse(D3DXMATRIX *pOut, FLOAT *pDeterminant, const D3DXMATRIX *pM);
inline D3DXMATRIX* D3DXMatrixScaling(D3DXMATRIX *pOut, FLOAT sx, FLOAT sy, FLOAT sz);
inline D3DXMATRIX* D3DXMatrixTranslation(D3DXMATRIX *pOut, FLOAT x, FLOAT y, FLOAT z);
inline D3DXMATRIX* D3DXMatrixRotationX(D3DXMATRIX *pOut, FLOAT Angle);
inline D3DXMATRIX* D3DXMatrixRotationY(D3DXMATRIX *pOut, FLOAT Angle);
inline D3DXMATRIX* D3DXMatrixRotationZ(D3DXMATRIX *pOut, FLOAT Angle);
inline D3DXMATRIX* D3DXMatrixRotationAxis(D3DXMATRIX *pOut, const D3DXVECTOR3 *pV, FLOAT Angle);
inline D3DXMATRIX* D3DXMatrixRotationQuaternion(D3DXMATRIX *pOut, const D3DXQUATERNION *pQ);
inline D3DXMATRIX* D3DXMatrixRotationYawPitchRoll(D3DXMATRIX *pOut, FLOAT Yaw, FLOAT Pitch, FLOAT Roll);
inline D3DXMATRIX* D3DXMatrixLookAtRH(D3DXMATRIX *pOut, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp);
inline D3DXMATRIX* D3DXMatrixLookAtLH(D3DXMATRIX *pOut, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp);
inline D3DXMATRIX* D3DXMatrixPerspectiveRH(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);
inline D3DXMATRIX* D3DXMatrixPerspectiveLH(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);
inline D3DXMATRIX* D3DXMatrixPerspectiveFovRH(D3DXMATRIX *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf);
inline D3DXMATRIX* D3DXMatrixPerspectiveFovLH(D3DXMATRIX *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf);
inline D3DXMATRIX* D3DXMatrixOrthoRH(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);
inline D3DXMATRIX* D3DXMatrixOrthoLH(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);

inline FLOAT D3DXQuaternionLength(const D3DXQUATERNION *pQ);
inline FLOAT D3DXQuaternionLengthSq(const D3DXQUATERNION *pQ);
inline FLOAT D3DXQuaternionDot(const D3DXQUATERNION *pQ1, const D3DXQUATERNION *pQ2);
inline D3DXQUATERNION* D3DXQuaternionIdentity(D3DXQUATERNION *pOut);
inline BOOL D3DXQuaternionIsIdentity(const D3DXQUATERNION *pQ);
inline D3DXQUATERNION* D3DXQuaternionConjugate(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ);
inline D3DXQUATERNION* D3DXQuaternionRotationMatrix(D3DXQUATERNION *pOut, const D3DXMATRIX *pM);
inline D3DXQUATERNION* D3DXQuaternionRotationAxis(D3DXQUATERNION *pOut, const D3DXVECTOR3 *pV, FLOAT Angle);
inline D3DXQUATERNION* D3DXQuaternionRotationYawPitchRoll(D3DXQUATERNION *pOut, FLOAT Yaw, FLOAT Pitch, FLOAT Roll);
inline D3DXQUATERNION* D3DXQuaternionMultiply(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ1, const D3DXQUATERNION *pQ2);
inline D3DXQUATERNION* D3DXQuaternionNormalize(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ);
inline D3DXQUATERNION* D3DXQuaternionInverse(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ);
inline D3DXQUATERNION* D3DXQuaternionExp(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ);
inline D3DXQUATERNION* D3DXQuaternionSlerp(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ1, const D3DXQUATERNION *pQ2, FLOAT t);
inline D3DXQUATERNION* D3DXQuaternionSquad(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ1, const D3DXQUATERNION *pA, const D3DXQUATERNION *pB, const D3DXQUATERNION *pC, FLOAT t);

inline D3DXCOLOR* D3DXColorNegative(D3DXCOLOR *pOut, const D3DXCOLOR *pC);
inline D3DXCOLOR* D3DXColorAdd(D3DXCOLOR *pOut, const D3DXCOLOR *pC1, const D3DXCOLOR *pC2);
inline D3DXCOLOR* D3DXColorSubtract(D3DXCOLOR *pOut, const D3DXCOLOR *pC1, const D3DXCOLOR *pC2);
inline D3DXCOLOR* D3DXColorScale(D3DXCOLOR *pOut, const D3DXCOLOR *pC, FLOAT s);
inline D3DXCOLOR* D3DXColorModulate(D3DXCOLOR *pOut, const D3DXCOLOR *pC1, const D3DXCOLOR *pC2);
inline D3DXCOLOR* D3DXColorLerp(D3DXCOLOR *pOut, const D3DXCOLOR *pC1, const D3DXCOLOR *pC2, FLOAT s);
inline D3DXCOLOR* D3DXColorAdjustSaturation(D3DXCOLOR *pOut, const D3DXCOLOR *pC, FLOAT s);
inline D3DXCOLOR* D3DXColorAdjustContrast(D3DXCOLOR *pOut, const D3DXCOLOR *pC, FLOAT c);


//===========================================================================
//
// General purpose utilities
//
//===========================================================================
#define D3DX_PI (3.14159265358979323846f)
#define D3DX_1BYPI (1.0f / D3DX_PI)

#define D3DXToRadian(degree) ((degree) * (D3DX_PI / 180.0f))
#define D3DXToDegree(radian) ((radian) * (180.0f / D3DX_PI))


//===========================================================================
//
// Vectors
//
//===========================================================================
// 2D Vector
typedef struct D3DXVECTOR2 : public CrossEngine::VEC2
{
public:
	D3DXVECTOR2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	D3DXVECTOR2(const D3DXVECTOR2 &v)
	{
		x = v.x;
		y = v.y;
	}

	D3DXVECTOR2(FLOAT fx, FLOAT fy)
	{
		x = fx;
		y = fy;
	}

	// assignment operators
	D3DXVECTOR2& operator += (const D3DXVECTOR2 &v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	D3DXVECTOR2& operator -= (const D3DXVECTOR2 &v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	D3DXVECTOR2& operator *= (FLOAT f)
	{
		x *= f;
		y *= f;
		return *this;
	}

	D3DXVECTOR2& operator /= (FLOAT f)
	{
		FLOAT fInv = 1.0f / f;
		x *= fInv;
		y *= fInv;
		return *this;
	}

	// unary operators
	D3DXVECTOR2 operator + () const
	{
		return *this;
	}

	D3DXVECTOR2 operator - () const
	{
		return D3DXVECTOR2(-x, -y);
	}

	// binary operators
	D3DXVECTOR2 operator + (const D3DXVECTOR2 &v) const
	{
		return D3DXVECTOR2(x + v.x, y + v.y);
	}

	D3DXVECTOR2 operator - (const D3DXVECTOR2 &v) const
	{
		return D3DXVECTOR2(x - v.x, y - v.y);
	}

	D3DXVECTOR2 operator * (FLOAT f) const
	{
		return D3DXVECTOR2(x * f, y * f);
	}

	D3DXVECTOR2 operator / (FLOAT f) const
	{
		FLOAT fInv = 1.0f / f;
		return D3DXVECTOR2(x * fInv, y * fInv);
	}

	friend D3DXVECTOR2 operator * (FLOAT f, const D3DXVECTOR2& v)
	{
		return D3DXVECTOR2(f * v.x, f * v.y);
	}

	BOOL operator == (const D3DXVECTOR2 &v) const
	{
		return x == v.x && y == v.y;
	}

	BOOL operator != (const D3DXVECTOR2 &v) const
	{
		return x != v.x || y != v.y;
	}
} D3DXVECTOR2, *LPD3DXVECTOR2;

// 3D Vector
typedef struct D3DXVECTOR3 : public CrossEngine::VEC3
{
public:
	D3DXVECTOR3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	D3DXVECTOR3(FLOAT fx, FLOAT fy, FLOAT fz)
	{
		x = fx;
		y = fy;
		z = fz;
	}

	// assignment operators
	D3DXVECTOR3& operator += (const D3DXVECTOR3 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	D3DXVECTOR3& operator -= (const D3DXVECTOR3 &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	D3DXVECTOR3& operator *= (FLOAT f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	D3DXVECTOR3& operator /= (FLOAT f)
	{
		FLOAT fInv = 1.0f / f;
		x *= fInv;
		y *= fInv;
		z *= fInv;
		return *this;
	}

	// unary operators
	D3DXVECTOR3 operator + () const
	{
		return *this;
	}

	D3DXVECTOR3 operator - () const
	{
		return D3DXVECTOR3(-x, -y, -z);
	}

	// binary operators
	D3DXVECTOR3 operator + (const D3DXVECTOR3 &v) const
	{
		return D3DXVECTOR3(x + v.x, y + v.y, z + v.z);
	}

	D3DXVECTOR3 operator - (const D3DXVECTOR3 &v) const
	{
		return D3DXVECTOR3(x - v.x, y - v.y, z - v.z);
	}

	D3DXVECTOR3 operator * (FLOAT f) const
	{
		return D3DXVECTOR3(x * f, y * f, z * f);
	}

	D3DXVECTOR3 operator / (FLOAT f) const
	{
		FLOAT fInv = 1.0f / f;
		return D3DXVECTOR3(x * fInv, y * fInv, z * fInv);
	}

	friend D3DXVECTOR3 operator * (FLOAT f, const struct D3DXVECTOR3 &v)
	{
		return D3DXVECTOR3(f * v.x, f * v.y, f * v.z);
	}

	BOOL operator == (const D3DXVECTOR3 &v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	BOOL operator != (const D3DXVECTOR3 &v) const
	{
		return x != v.x || y != v.y || z != v.z;
	}

} D3DXVECTOR3, *LPD3DXVECTOR3;

// 4D Vector
typedef struct D3DXVECTOR4 : public CrossEngine::VEC4
{
public:
	D3DXVECTOR4()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}

	D3DXVECTOR4(FLOAT fx, FLOAT fy, FLOAT fz, FLOAT fw)
	{
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}

	// assignment operators
	D3DXVECTOR4& operator += (const D3DXVECTOR4 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	D3DXVECTOR4& operator -= (const D3DXVECTOR4 &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	D3DXVECTOR4& operator *= (FLOAT f)
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}

	D3DXVECTOR4& operator /= (FLOAT f)
	{
		FLOAT fInv = 1.0f / f;
		x *= fInv;
		y *= fInv;
		z *= fInv;
		w *= fInv;
		return *this;
	}

	// unary operators
	D3DXVECTOR4 operator + () const
	{
		return *this;
	}

	D3DXVECTOR4 operator - () const
	{
		return D3DXVECTOR4(-x, -y, -z, -w);
	}

	// binary operators
	D3DXVECTOR4 operator + (const D3DXVECTOR4 &v) const
	{
		return D3DXVECTOR4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	D3DXVECTOR4 operator - (const D3DXVECTOR4 &v) const
	{
		return D3DXVECTOR4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	D3DXVECTOR4 operator * (FLOAT f) const
	{
		return D3DXVECTOR4(x * f, y * f, z * f, w * f);
	}

	D3DXVECTOR4 operator / (FLOAT f) const
	{
		FLOAT fInv = 1.0f / f;
		return D3DXVECTOR4(x * fInv, y * fInv, z * fInv, w * fInv);
	}

	friend D3DXVECTOR4 operator * (FLOAT f, const D3DXVECTOR4 &v)
	{
		return D3DXVECTOR4(f * v.x, f * v.y, f * v.z, f * v.w);
	}

	BOOL operator == (const D3DXVECTOR4 &v) const
	{
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}

	BOOL operator != (const D3DXVECTOR4 &v) const
	{
		return x != v.x || y != v.y || z != v.z || w != v.w;
	}
} D3DXVECTOR4, *LPD3DXVECTOR4;


//===========================================================================
//
// Matrices
//
//===========================================================================
typedef struct D3DXMATRIX : public CrossEngine::MATRIX4
{
public:
	D3DXMATRIX()
	{
		m00 = 1.0f; m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
		m10 = 0.0f; m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
		m20 = 0.0f; m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
		m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
	}

	D3DXMATRIX(
		FLOAT _m00, FLOAT _m01, FLOAT _m02, FLOAT _m03,
		FLOAT _m10, FLOAT _m11, FLOAT _m12, FLOAT _m13,
		FLOAT _m20, FLOAT _m21, FLOAT _m22, FLOAT _m23,
		FLOAT _m30, FLOAT _m31, FLOAT _m32, FLOAT _m33)
	{
		m00 = _m00; m01 = _m01; m02 = _m02; m03 = _m03;
		m10 = _m10; m11 = _m11; m12 = _m12; m13 = _m13;
		m20 = _m20; m21 = _m21; m22 = _m22; m23 = _m23;
		m30 = _m30; m31 = _m31; m32 = _m32; m33 = _m33;
	}

	// access grants
	FLOAT& operator () (UINT row, UINT col)
	{
		return m[row*4 + col];
	}

	FLOAT  operator () (UINT row, UINT col) const
	{
		return m[row*4 + col];
	}

	// assignment operators
	D3DXMATRIX& operator *= (const D3DXMATRIX &mtx)
	{
		D3DXMATRIX mtxIn = *this;
		D3DXMatrixMultiply(this, &mtxIn, &mtx);
		return *this;
	}

	D3DXMATRIX& operator += (const D3DXMATRIX &mtx)
	{
		m00 += mtx.m00; m01 += mtx.m01; m02 += mtx.m02; m03 += mtx.m03;
		m10 += mtx.m10; m11 += mtx.m11; m12 += mtx.m12; m13 += mtx.m13;
		m20 += mtx.m20; m21 += mtx.m21; m22 += mtx.m22; m23 += mtx.m23;
		m30 += mtx.m30; m31 += mtx.m31; m32 += mtx.m32; m33 += mtx.m33;
		return *this;
	}

	D3DXMATRIX& operator -= (const D3DXMATRIX &mtx)
	{
		m00 -= mtx.m00; m01 -= mtx.m01; m02 -= mtx.m02; m03 -= mtx.m03;
		m10 -= mtx.m10; m11 -= mtx.m11; m12 -= mtx.m12; m13 -= mtx.m13;
		m20 -= mtx.m20; m21 -= mtx.m21; m22 -= mtx.m22; m23 -= mtx.m23;
		m30 -= mtx.m30; m31 -= mtx.m31; m32 -= mtx.m32; m33 -= mtx.m33;
		return *this;
	}

	D3DXMATRIX& operator *= (FLOAT f)
	{
		m00 *= f; m01 *= f; m02 *= f; m03 *= f;
		m10 *= f; m11 *= f; m12 *= f; m13 *= f;
		m20 *= f; m21 *= f; m22 *= f; m23 *= f;
		m30 *= f; m31 *= f; m32 *= f; m33 *= f;
		return *this;
	}

	D3DXMATRIX& operator /= (FLOAT f)
	{
		FLOAT fInv = 1.0f / f;
		m00 *= fInv; m01 *= fInv; m02 *= fInv; m03 *= fInv;
		m10 *= fInv; m11 *= fInv; m12 *= fInv; m13 *= fInv;
		m20 *= fInv; m21 *= fInv; m22 *= fInv; m23 *= fInv;
		m30 *= fInv; m31 *= fInv; m32 *= fInv; m33 *= fInv;
		return *this;
	}

	// unary operators
	D3DXMATRIX operator + () const
	{
		return *this;
	}

	D3DXMATRIX operator - () const
	{
		return D3DXMATRIX(
			-m00, -m01, -m02, -m03,
			-m10, -m11, -m12, -m13,
			-m20, -m21, -m22, -m23,
			-m30, -m31, -m32, -m33);
	}

	// binary operators
	D3DXMATRIX operator * (const D3DXMATRIX &mtx) const
	{
		D3DXMATRIX mtxOut;
		D3DXMatrixMultiply(&mtxOut, this, &mtx);
		return mtxOut;
	}

	D3DXMATRIX operator + (const D3DXMATRIX &mtx) const
	{
		return D3DXMATRIX(
			m00 + mtx.m00, m01 + mtx.m01, m02 + mtx.m02, m03 + mtx.m03,
			m10 + mtx.m10, m11 + mtx.m11, m12 + mtx.m12, m13 + mtx.m13,
			m20 + mtx.m20, m21 + mtx.m21, m22 + mtx.m22, m23 + mtx.m23,
			m30 + mtx.m30, m31 + mtx.m31, m32 + mtx.m32, m33 + mtx.m33);
	}

	D3DXMATRIX operator - (const D3DXMATRIX &mtx) const
	{
		return D3DXMATRIX(
			m00 - mtx.m00, m01 - mtx.m01, m02 - mtx.m02, m03 - mtx.m03,
			m10 - mtx.m10, m11 - mtx.m11, m12 - mtx.m12, m13 - mtx.m13,
			m20 - mtx.m20, m21 - mtx.m21, m22 - mtx.m22, m23 - mtx.m23,
			m30 - mtx.m30, m31 - mtx.m31, m32 - mtx.m32, m33 - mtx.m33);
	}

	D3DXMATRIX operator * (FLOAT f) const
	{
		return D3DXMATRIX(
			m00 * f, m01 * f, m02 * f, m03 * f,
			m10 * f, m11 * f, m12 * f, m13 * f,
			m20 * f, m21 * f, m22 * f, m23 * f,
			m30 * f, m31 * f, m32 * f, m33 * f);
	}

	D3DXMATRIX operator / (FLOAT f) const
	{
		FLOAT fInv = 1.0f / f;
		return D3DXMATRIX(
			m00 * fInv, m01 * fInv, m02 * fInv, m03 * fInv,
			m10 * fInv, m11 * fInv, m12 * fInv, m13 * fInv,
			m20 * fInv, m21 * fInv, m22 * fInv, m23 * fInv,
			m30 * fInv, m31 * fInv, m32 * fInv, m33 * fInv);
	}

	friend D3DXMATRIX operator * (FLOAT f, const D3DXMATRIX &mtx)
	{
		return D3DXMATRIX(
			mtx.m00 * f, mtx.m01 * f, mtx.m02 * f, mtx.m03 * f,
			mtx.m10 * f, mtx.m11 * f, mtx.m12 * f, mtx.m13 * f,
			mtx.m20 * f, mtx.m21 * f, mtx.m22 * f, mtx.m23 * f,
			mtx.m30 * f, mtx.m31 * f, mtx.m32 * f, mtx.m33 * f);
	}

	BOOL operator == (const D3DXMATRIX &mtx) const
	{
		return 0 == memcmp(this, &mtx, sizeof(D3DXMATRIX));
	}

	BOOL operator != (const D3DXMATRIX &mtx) const
	{
		return 0 != memcmp(this, &mtx, sizeof(D3DXMATRIX));
	}

} D3DXMATRIX, *LPD3DXMATRIX;


//===========================================================================
//
//    Quaternions
//
//===========================================================================
typedef struct D3DXQUATERNION : public CrossEngine::QUAT
{
public:
	D3DXQUATERNION()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}

	D3DXQUATERNION(FLOAT fx, FLOAT fy, FLOAT fz, FLOAT fw)
	{
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}

	// assignment operators
	D3DXQUATERNION& operator += (const D3DXQUATERNION &q)
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;
		return *this;
	}

	D3DXQUATERNION& operator -= (const D3DXQUATERNION &q)
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;
		return *this;
	}

	D3DXQUATERNION& operator *= (const D3DXQUATERNION &q)
	{
		D3DXQUATERNION quatIn = *this;
		D3DXQuaternionMultiply(this, &quatIn, &q);
		return *this;
	}

	D3DXQUATERNION& operator *= (FLOAT f)
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}

	D3DXQUATERNION& operator /= (FLOAT f)
	{
		FLOAT fInv = 1.0f / f;
		x *= fInv;
		y *= fInv;
		z *= fInv;
		w *= fInv;
		return *this;
	}

	// unary operators
	D3DXQUATERNION  operator + () const
	{
		return *this;
	}

	D3DXQUATERNION  operator - () const
	{
		return D3DXQUATERNION(-x, -y, -z, -w);
	}

	// binary operators
	D3DXQUATERNION operator + (const D3DXQUATERNION &q) const
	{
		return D3DXQUATERNION(x + q.x, y + q.y, z + q.z, w + q.w);
	}

	D3DXQUATERNION operator - (const D3DXQUATERNION &q) const
	{
		return D3DXQUATERNION(x - q.x, y - q.y, z - q.z, w - q.w);
	}

	D3DXQUATERNION operator * (const D3DXQUATERNION &q) const
	{
		D3DXQUATERNION quatOut;
		D3DXQuaternionMultiply(&quatOut, this, &q);
		return quatOut;
	}

	D3DXQUATERNION operator * (FLOAT f) const
	{
		return D3DXQUATERNION(x * f, y * f, z * f, w * f);
	}

	D3DXQUATERNION operator / (FLOAT f) const
	{
		FLOAT fInv = 1.0f / f;
		return D3DXQUATERNION(x * fInv, y * fInv, z * fInv, w * fInv);
	}

	friend D3DXQUATERNION operator * (FLOAT f, const D3DXQUATERNION &q)
	{
		return D3DXQUATERNION(q.x * f, q.y * f, q.z * f, q.w * f);
	}

	BOOL operator == (const D3DXQUATERNION &q) const
	{
		return x == q.x && y == q.y && z == q.z && w == q.w;
	}

	BOOL operator != (const D3DXQUATERNION &q) const
	{
		return x != q.x || y != q.y || z != q.z || w != q.w;
	}
} D3DXQUATERNION, *LPD3DXQUATERNION;


//===========================================================================
//
// Colors
//
//===========================================================================
typedef struct D3DXCOLOR : public CrossEngine::VEC4
{
public:
	D3DXCOLOR()
	{
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
		a = 0.0f;
	}

	D3DXCOLOR(UINT argb)
	{
		const FLOAT f = 1.0f / 255.0f;
		r = f * (FLOAT) (BYTE) (argb >>  0);
		g = f * (FLOAT) (BYTE) (argb >>  8);
		b = f * (FLOAT) (BYTE) (argb >> 16);
		a = f * (FLOAT) (BYTE) (argb >> 24);
	}

	D3DXCOLOR(FLOAT fr, FLOAT fg, FLOAT fb, FLOAT fa)
	{
		r = fr;
		g = fg;
		b = fb;
		a = fa;
	}

	// casting
	operator UINT () const
	{
		UINT dwR = r >= 1.0f ? 0xff : r <= 0.0f ? 0x00 : (UINT) (r * 255.0f + 0.5f);
		UINT dwG = g >= 1.0f ? 0xff : g <= 0.0f ? 0x00 : (UINT) (g * 255.0f + 0.5f);
		UINT dwB = b >= 1.0f ? 0xff : b <= 0.0f ? 0x00 : (UINT) (b * 255.0f + 0.5f);
		UINT dwA = a >= 1.0f ? 0xff : a <= 0.0f ? 0x00 : (UINT) (a * 255.0f + 0.5f);

		return (dwA << 24) | (dwB << 16) | (dwG << 8) | (dwR << 0);
	}

	// assignment operators
	D3DXCOLOR& operator += (const D3DXCOLOR &c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
		a += c.a;
		return *this;
	}

	D3DXCOLOR& operator -= (const D3DXCOLOR &c)
	{
		r -= c.r;
		g -= c.g;
		b -= c.b;
		a -= c.a;
		return *this;
	}

	D3DXCOLOR& operator *= (FLOAT f)
	{
		r *= f;
		g *= f;
		b *= f;
		a *= f;
		return *this;
	}

	D3DXCOLOR& operator /= (FLOAT f)
	{
		FLOAT fInv = 1.0f / f;
		r *= fInv;
		g *= fInv;
		b *= fInv;
		a *= fInv;
		return *this;
	}

	// unary operators
	D3DXCOLOR operator + () const
	{
		return *this;
	}

	D3DXCOLOR operator - () const
	{
		return D3DXCOLOR(-r, -g, -b, -a);
	}

	// binary operators
	D3DXCOLOR operator + (const D3DXCOLOR &c) const
	{
		return D3DXCOLOR(r + c.r, g + c.g, b + c.b, a + c.a);
	}

	D3DXCOLOR operator - (const D3DXCOLOR &c) const
	{
		return D3DXCOLOR(r - c.r, g - c.g, b - c.b, a - c.a);
	}

	D3DXCOLOR operator * (FLOAT f) const
	{
		return D3DXCOLOR(r * f, g * f, b * f, a * f);
	}

	D3DXCOLOR operator / (FLOAT f) const
	{
		FLOAT fInv = 1.0f / f;
		return D3DXCOLOR(r * fInv, g * fInv, b * fInv, a * fInv);
	}

	friend D3DXCOLOR operator * (FLOAT f, const D3DXCOLOR &c)
	{
		return D3DXCOLOR(c.r * f, c.g * f, c.b * f, c.a * f);
	}

	BOOL operator == (const D3DXCOLOR &c) const
	{
		return r == c.r && g == c.g && b == c.b && a == c.a;
	}

	BOOL operator != (const D3DXCOLOR &c) const
	{
		return r != c.r || g != c.g || b != c.b || a != c.a;
	}

public:
	FLOAT r, g, b, a;
} D3DXCOLOR, *LPD3DXCOLOR;


//===========================================================================
//
// D3DX math functions:
//
// NOTE:
//  * All these functions can take the same object as in and out parameters.
//
//  * Out parameters are typically also returned as return values, so that
//    the output of one function may be used as a parameter to another.
//
//===========================================================================
//--------------------------
// 2D Vector
//--------------------------
inline FLOAT D3DXVec2Length(const D3DXVECTOR2 *pV)
{
	return sqrtf(pV->x * pV->x + pV->y * pV->y);
}

inline FLOAT D3DXVec2LengthSq(const D3DXVECTOR2 *pV)
{
	return pV->x * pV->x + pV->y * pV->y;
}

inline FLOAT D3DXVec2Dot(const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2)
{
	return pV1->x * pV2->x + pV1->y * pV2->y;
}

// Z component of ((x1,y1,0) cross (x2,y2,0))
inline FLOAT D3DXVec2CCW(const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2)
{
	return pV1->x * pV2->y - pV1->y * pV2->x;
}

inline D3DXVECTOR2* D3DXVec2Add(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2)
{
	pOut->x = pV1->x + pV2->x;
	pOut->y = pV1->y + pV2->y;
	return pOut;
}

inline D3DXVECTOR2* D3DXVec2Subtract(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2)
{
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	return pOut;
}

// Minimize each component. x = min(x1, x2), y = min(y1, y2)
inline D3DXVECTOR2* D3DXVec2Minimize(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2)
{
	pOut->x = pV1->x < pV2->x ? pV1->x : pV2->x;
	pOut->y = pV1->y < pV2->y ? pV1->y : pV2->y;
	return pOut;
}

// Maximize each component. x = max(x1, x2), y = max(y1, y2)
inline D3DXVECTOR2* D3DXVec2Maximize(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2)
{
	pOut->x = pV1->x > pV2->x ? pV1->x : pV2->x;
	pOut->y = pV1->y > pV2->y ? pV1->y : pV2->y;
	return pOut;
}

inline D3DXVECTOR2* D3DXVec2Scale(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV, FLOAT s)
{
	pOut->x = pV->x * s;
	pOut->y = pV->y * s;
	return pOut;
}

// Linear interpolation. V1 + s(V2-V1)
inline D3DXVECTOR2* D3DXVec2Lerp(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2, FLOAT s)
{
	pOut->x = pV1->x + s * (pV2->x - pV1->x);
	pOut->y = pV1->y + s * (pV2->y - pV1->y);
	return pOut;
}

inline D3DXVECTOR2* D3DXVec2Normalize(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV)
{
	FLOAT len = sqrtf(pV->x * pV->x + pV->y * pV->y);
	FLOAT invLen = 1.0f / len;

	pOut->x = pV->x * invLen;
	pOut->y = pV->y * invLen;

	return pOut;
}

// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
inline D3DXVECTOR2* D3DXVec2BaryCentric(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV1, const D3DXVECTOR2 *pV2, const D3DXVECTOR2 *pV3, FLOAT f, FLOAT g)
{
	pOut->x = pV1->x + f * (pV2->x - pV1->x) + g * (pV3->x - pV1->x);
	pOut->y = pV1->y + f * (pV2->y - pV1->y) + g * (pV3->y - pV1->y);
	return pOut;
}

// Transform (x, y, 0, 1) by matrix.
inline D3DXVECTOR4* D3DXVec2Transform(D3DXVECTOR4 *pOut, const D3DXVECTOR2 *pV, const D3DXMATRIX *pM)
{
	D3DXVECTOR4 vecOut;
	D3DXVECTOR4 vecIn(pV->x, pV->y, 0.0f, 1.0f);

	D3DXVec4Transform(&vecOut, &vecIn, pM);

	pOut->x = vecOut.x;
	pOut->y = vecOut.y;
	pOut->z = vecOut.z;
	pOut->w = vecOut.w;

	return pOut;
}

// Transform (x, y, 0, 1) by matrix, project result back into w=1.
inline D3DXVECTOR2* D3DXVec2TransformCoord(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV, const D3DXMATRIX *pM)
{
	D3DXVECTOR4 vecOut;
	D3DXVECTOR4 vecIn(pV->x, pV->y, 0.0f, 1.0f);

	D3DXVec4Transform(&vecOut, &vecIn, pM);

	pOut->x = vecOut.x / vecOut.w;
	pOut->y = vecOut.y / vecOut.w;

	return pOut;
}

// Transform (x, y, 0, 0) by matrix.
inline D3DXVECTOR2* D3DXVec2TransformNormal(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV, const D3DXMATRIX *pM)
{
	D3DXVECTOR4 vecOut;
	D3DXVECTOR4 vecIn(pV->x, pV->y, 0.0f, 0.0f);

	D3DXVec4Transform(&vecOut, &vecIn, pM);

	pOut->x = vecOut.x;
	pOut->y = vecOut.y;

	return pOut;
}

// Transform Array (x, y, 0, 1) by matrix.
inline D3DXVECTOR4* D3DXVec2TransformArray(D3DXVECTOR4 *pOut, UINT OutStride, const D3DXVECTOR2 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n)
{
	BYTE *pVInAddress = (BYTE *)pV;
	BYTE *pVOutAddress = (BYTE *)pOut;

	for (UINT indexVector = 0; indexVector < n; indexVector++) {
		D3DXVec2Transform((D3DXVECTOR4 *)pVOutAddress, (D3DXVECTOR2 *)pVInAddress, pM);
		pVInAddress += VStride;
		pVOutAddress += OutStride;
	}

	return pOut;
}

// Transform Array (x, y, 0, 1) by matrix, project result back into w=1.
inline D3DXVECTOR2* D3DXVec2TransformCoordArray(D3DXVECTOR2 *pOut, UINT OutStride, const D3DXVECTOR2 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n)
{
	BYTE *pVInAddress = (BYTE *)pV;
	BYTE *pVOutAddress = (BYTE *)pOut;

	for (UINT indexVector = 0; indexVector < n; indexVector++) {
		D3DXVec2TransformCoord((D3DXVECTOR2 *)pVOutAddress, (D3DXVECTOR2 *)pVInAddress, pM);
		pVInAddress += VStride;
		pVOutAddress += OutStride;
	}

	return pOut;
}

// Transform Array (x, y, 0, 0) by matrix.
inline D3DXVECTOR2* D3DXVec2TransformNormalArray(D3DXVECTOR2 *pOut, UINT OutStride, const D3DXVECTOR2 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n)
{
	BYTE *pVInAddress = (BYTE *)pV;
	BYTE *pVOutAddress = (BYTE *)pOut;

	for (UINT indexVector = 0; indexVector < n; indexVector++) {
		D3DXVec2TransformNormal((D3DXVECTOR2 *)pVOutAddress, (D3DXVECTOR2 *)pVInAddress, pM);
		pVInAddress += VStride;
		pVOutAddress += OutStride;
	}

	return pOut;
}


//--------------------------
// 3D Vector
//--------------------------
inline FLOAT D3DXVec3Length(const D3DXVECTOR3 *pV)
{
	return sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
}

inline FLOAT D3DXVec3LengthSq(const D3DXVECTOR3 *pV)
{
	return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z;
}

inline FLOAT D3DXVec3Dot(const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2)
{
	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

inline D3DXVECTOR3* D3DXVec3Cross(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2)
{
	D3DXVECTOR3 v;

	v.x = pV1->y * pV2->z - pV1->z * pV2->y;
	v.y = pV1->z * pV2->x - pV1->x * pV2->z;
	v.z = pV1->x * pV2->y - pV1->y * pV2->x;

	pOut->x = v.x;
	pOut->y = v.y;
	pOut->z = v.z;

	return pOut;
}

inline D3DXVECTOR3* D3DXVec3Add(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2)
{
	pOut->x = pV1->x + pV2->x;
	pOut->y = pV1->y + pV2->y;
	pOut->z = pV1->z + pV2->z;
	return pOut;
}

inline D3DXVECTOR3* D3DXVec3Subtract(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2)
{
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;
	return pOut;
}

// Minimize each component. x = min(x1, x2), y = min(y1, y2), ...
inline D3DXVECTOR3* D3DXVec3Minimize(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2)
{
	pOut->x = pV1->x < pV2->x ? pV1->x : pV2->x;
	pOut->y = pV1->y < pV2->y ? pV1->y : pV2->y;
	pOut->z = pV1->z < pV2->z ? pV1->z : pV2->z;
	return pOut;
}

// Maximize each component. x = max(x1, x2), y = max(y1, y2), ...
inline D3DXVECTOR3* D3DXVec3Maximize(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2)
{
	pOut->x = pV1->x > pV2->x ? pV1->x : pV2->x;
	pOut->y = pV1->y > pV2->y ? pV1->y : pV2->y;
	pOut->z = pV1->z > pV2->z ? pV1->z : pV2->z;
	return pOut;
}

inline D3DXVECTOR3* D3DXVec3Scale(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, FLOAT s)
{
	pOut->x = pV->x * s;
	pOut->y = pV->y * s;
	pOut->z = pV->z * s;
	return pOut;
}

// Linear interpolation. V1 + s(V2-V1)
inline D3DXVECTOR3* D3DXVec3Lerp(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2, FLOAT s)
{
	pOut->x = pV1->x + s * (pV2->x - pV1->x);
	pOut->y = pV1->y + s * (pV2->y - pV1->y);
	pOut->z = pV1->z + s * (pV2->z - pV1->z);
	return pOut;
}

inline D3DXVECTOR3* D3DXVec3Normalize(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV)
{
	FLOAT len = sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
	FLOAT invLen = 1.0f / len;

	pOut->x = pV->x * invLen;
	pOut->y = pV->y * invLen;
	pOut->z = pV->z * invLen;

	return pOut;
}

// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
inline D3DXVECTOR3* D3DXVec3BaryCentric(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2, const D3DXVECTOR3 *pV3, FLOAT f, FLOAT g)
{
	pOut->x = pV1->x + f * (pV2->x - pV1->x) + g * (pV3->x - pV1->x);
	pOut->y = pV1->y + f * (pV2->y - pV1->y) + g * (pV3->y - pV1->y);
	pOut->z = pV1->z + f * (pV2->z - pV1->z) + g * (pV3->z - pV1->z);
	return pOut;
}

// Transform (x, y, z, 1) by matrix.
inline D3DXVECTOR4* D3DXVec3Transform(D3DXVECTOR4 *pOut, const D3DXVECTOR3 *pV, const D3DXMATRIX *pM)
{
	D3DXVECTOR4 vecOut;
	D3DXVECTOR4 vecIn(pV->x, pV->y, pV->z, 1.0f);

	D3DXVec4Transform(&vecOut, &vecIn, pM);

	pOut->x = vecOut.x;
	pOut->y = vecOut.y;
	pOut->z = vecOut.z;
	pOut->w = vecOut.w;

	return pOut;
}

// Transform (x, y, z, 1) by matrix, project result back into w=1.
inline D3DXVECTOR3* D3DXVec3TransformCoord(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXMATRIX *pM)
{
	D3DXVECTOR4 vecOut;
	D3DXVECTOR4 vecIn(pV->x, pV->y, pV->z, 1.0f);

	D3DXVec4Transform(&vecOut, &vecIn, pM);

	pOut->x = vecOut.x / vecOut.w;
	pOut->y = vecOut.y / vecOut.w;
	pOut->z = vecOut.z / vecOut.w;

	return pOut;
}

// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
// non-affine matrix, the matrix you pass to this function should be the 
// transpose of the inverse of the matrix you would use to transform a coord.
inline D3DXVECTOR3* D3DXVec3TransformNormal(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXMATRIX *pM)
{
	D3DXVECTOR4 vecOut;
	D3DXVECTOR4 vecIn(pV->x, pV->y, pV->z, 0.0f);

	D3DXVec4Transform(&vecOut, &vecIn, pM);

	pOut->x = vecOut.x;
	pOut->y = vecOut.y;
	pOut->z = vecOut.z;

	return pOut;
}

// Transform Array (x, y, z, 1) by matrix. 
inline D3DXVECTOR4* D3DXVec3TransformArray(D3DXVECTOR4 *pOut, UINT OutStride, const D3DXVECTOR3 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n)
{
	BYTE *pVInAddress = (BYTE *)pV;
	BYTE *pVOutAddress = (BYTE *)pOut;

	for (UINT indexVector = 0; indexVector < n; indexVector++) {
		D3DXVec3Transform((D3DXVECTOR4 *)pVOutAddress, (D3DXVECTOR3 *)pVInAddress, pM);
		pVInAddress += VStride;
		pVOutAddress += OutStride;
	}

	return pOut;
}

// Transform Array (x, y, z, 1) by matrix, project result back into w=1.
inline D3DXVECTOR3* D3DXVec3TransformCoordArray(D3DXVECTOR3 *pOut, UINT OutStride, const D3DXVECTOR3 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n)
{
	BYTE *pVInAddress = (BYTE *)pV;
	BYTE *pVOutAddress = (BYTE *)pOut;

	for (UINT indexVector = 0; indexVector < n; indexVector++) {
		D3DXVec3TransformCoord((D3DXVECTOR3 *)pVOutAddress, (D3DXVECTOR3 *)pVInAddress, pM);
		pVInAddress += VStride;
		pVOutAddress += OutStride;
	}

	return pOut;
}

// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
// non-affine matrix, the matrix you pass to this function should be the 
// transpose of the inverse of the matrix you would use to transform a coord.
inline D3DXVECTOR3* D3DXVec3TransformNormalArray(D3DXVECTOR3 *pOut, UINT OutStride, const D3DXVECTOR3 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n)
{
	BYTE *pVInAddress = (BYTE *)pV;
	BYTE *pVOutAddress = (BYTE *)pOut;

	for (UINT indexVector = 0; indexVector < n; indexVector++) {
		D3DXVec3TransformNormal((D3DXVECTOR3 *)pVOutAddress, (D3DXVECTOR3 *)pVInAddress, pM);
		pVInAddress += VStride;
		pVOutAddress += OutStride;
	}

	return pOut;
}

// Project vector from object space into screen space
inline D3DXVECTOR3* D3DXVec3Project(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXMATRIX *pProjection, const D3DXMATRIX *pView, const D3DXMATRIX *pWorld, INT viewportX, INT viewportY, INT viewportWidth, INT viewportHeight)
{
	D3DXVECTOR3 screen;
	D3DXMATRIX mtxWorldToView;

	D3DXMatrixMultiply(&mtxWorldToView, pWorld, pView);
	CrossEngine::Project(&screen, pV, &mtxWorldToView, pProjection, viewportX, viewportY, viewportWidth, viewportHeight);

	pOut->x = screen.x;
	pOut->y = screen.y;
	pOut->z = screen.z;

	return pOut;
}

// Project vector Array from object space into screen space
inline D3DXVECTOR3* D3DXVec3ProjectArray(D3DXVECTOR3 *pOut, UINT OutStride,const D3DXVECTOR3 *pV, UINT VStride, const D3DXMATRIX *pProjection, const D3DXMATRIX *pView, const D3DXMATRIX *pWorld, INT viewportX, INT viewportY, INT viewportWidth, INT viewportHeight, UINT n)
{
	BYTE *pVInAddress = (BYTE *)pV;
	BYTE *pVOutAddress = (BYTE *)pOut;

	for (UINT indexVector = 0; indexVector < n; indexVector++) {
		D3DXVec3Project((D3DXVECTOR3 *)pVOutAddress, (D3DXVECTOR3 *)pVInAddress, pProjection, pView, pWorld, viewportX, viewportY, viewportWidth, viewportHeight);
		pVInAddress += VStride;
		pVOutAddress += OutStride;
	}

	return pOut;
}

// Project vector from screen space into object space
//inline D3DXVECTOR3* D3DXVec3Unproject(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DXMATRIX *pProjection, const D3DXMATRIX *pView, const D3DXMATRIX *pWorld, INT viewportWidth, INT viewportHeight)
//{
//
//}

// Project vector Array from screen space into object space
//inline D3DXVECTOR3* D3DXVec3UnprojectArray(D3DXVECTOR3 *pOut, UINT OutStride, const D3DXVECTOR3 *pV, UINT VStride, const D3DXMATRIX *pProjection, const D3DXMATRIX *pView, const D3DXMATRIX *pWorld, INT viewportWidth, INT viewportHeight, UINT n)
//{
//
//}


//--------------------------
// 4D Vector
//--------------------------
inline FLOAT D3DXVec4Length(const D3DXVECTOR4 *pV)
{
	return sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z + pV->w * pV->w);
}

inline FLOAT D3DXVec4LengthSq(const D3DXVECTOR4 *pV)
{
	return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z + pV->w * pV->w;
}

inline FLOAT D3DXVec4Dot(const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2)
{
	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z + pV1->w * pV2->w;
}

inline D3DXVECTOR4* D3DXVec4Add(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2)
{
	pOut->x = pV1->x + pV2->x;
	pOut->y = pV1->y + pV2->y;
	pOut->z = pV1->z + pV2->z;
	pOut->w = pV1->w + pV2->w;
	return pOut;
}

inline D3DXVECTOR4* D3DXVec4Subtract(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2)
{
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;
	pOut->w = pV1->w - pV2->w;
	return pOut;
}

// Minimize each component. x = min(x1, x2), y = min(y1, y2), ...
inline D3DXVECTOR4* D3DXVec4Minimize(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2)
{
	pOut->x = pV1->x < pV2->x ? pV1->x : pV2->x;
	pOut->y = pV1->y < pV2->y ? pV1->y : pV2->y;
	pOut->z = pV1->z < pV2->z ? pV1->z : pV2->z;
	pOut->w = pV1->w < pV2->w ? pV1->w : pV2->w;
	return pOut;
}

// Maximize each component. x = max(x1, x2), y = max(y1, y2), ...
inline D3DXVECTOR4* D3DXVec4Maximize(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2)
{
	pOut->x = pV1->x > pV2->x ? pV1->x : pV2->x;
	pOut->y = pV1->y > pV2->y ? pV1->y : pV2->y;
	pOut->z = pV1->z > pV2->z ? pV1->z : pV2->z;
	pOut->w = pV1->w > pV2->w ? pV1->w : pV2->w;
	return pOut;
}

inline D3DXVECTOR4* D3DXVec4Scale(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV, FLOAT s)
{
	pOut->x = pV->x * s;
	pOut->y = pV->y * s;
	pOut->z = pV->z * s;
	pOut->w = pV->w * s;
	return pOut;
}

// Linear interpolation. V1 + s(V2-V1)
inline D3DXVECTOR4* D3DXVec4Lerp(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2, FLOAT s)
{
	pOut->x = pV1->x + s * (pV2->x - pV1->x);
	pOut->y = pV1->y + s * (pV2->y - pV1->y);
	pOut->z = pV1->z + s * (pV2->z - pV1->z);
	pOut->w = pV1->w + s * (pV2->w - pV1->w);
	return pOut;
}

// Cross-product in 4 dimensions.
inline D3DXVECTOR4* D3DXVec4Normalize(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV)
{
	FLOAT len = sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z + pV->w * pV->w);
	FLOAT invLen = 1.0f / len;

	pOut->x = pV->x * invLen;
	pOut->y = pV->y * invLen;
	pOut->z = pV->z * invLen;
	pOut->w = pV->w * invLen;

	return pOut;
}

// Barycentric coordinates.  V1 + f(V2-V1) + g(V3-V1)
inline D3DXVECTOR4* D3DXVec4BaryCentric(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2, const D3DXVECTOR4 *pV3, FLOAT f, FLOAT g)
{
	pOut->x = pV1->x + f * (pV2->x - pV1->x) + g * (pV3->x - pV1->x);
	pOut->y = pV1->y + f * (pV2->y - pV1->y) + g * (pV3->y - pV1->y);
	pOut->z = pV1->z + f * (pV2->z - pV1->z) + g * (pV3->z - pV1->z);
	pOut->w = pV1->w + f * (pV2->w - pV1->w) + g * (pV3->w - pV1->w);
	return pOut;
}

// Transform vector by matrix.
inline D3DXVECTOR4* D3DXVec4Transform(D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV, const D3DXMATRIX *pM)
{
	pOut->x = pV->x*pM->m00 + pV->y*pM->m10 + pV->z*pM->m20 + pV->w*pM->m30;
	pOut->y = pV->x*pM->m01 + pV->y*pM->m11 + pV->z*pM->m21 + pV->w*pM->m31;
	pOut->z = pV->x*pM->m02 + pV->y*pM->m12 + pV->z*pM->m22 + pV->w*pM->m32;
	pOut->w = pV->x*pM->m03 + pV->y*pM->m13 + pV->z*pM->m23 + pV->w*pM->m33;
	return pOut;
}

// Transform vector array by matrix.
inline D3DXVECTOR4* D3DXVec4TransformArray(D3DXVECTOR4 *pOut, UINT OutStride, const D3DXVECTOR4 *pV, UINT VStride, const D3DXMATRIX *pM, UINT n)
{
	BYTE *pVInAddress = (BYTE *)pV;
	BYTE *pVOutAddress = (BYTE *)pOut;

	for (UINT indexVector = 0; indexVector < n; indexVector++) {
		D3DXVec4Transform((D3DXVECTOR4 *)pVOutAddress, (D3DXVECTOR4 *)pVInAddress, pM);
		pVInAddress += VStride;
		pVOutAddress += OutStride;
	}

	return pOut;
}


//--------------------------
// 4D Matrix
//--------------------------
inline D3DXMATRIX* D3DXMatrixIdentity(D3DXMATRIX *pOut)
{
	CrossEngine::MtxIdentity(pOut);
	return pOut;
}

inline BOOL D3DXMatrixIsIdentity(const D3DXMATRIX *pM)
{
	return 
		pM->m00 == 1.0f && pM->m01 == 0.0f && pM->m02 == 0.0f && pM->m03 == 0.0f &&
		pM->m10 == 0.0f && pM->m11 == 1.0f && pM->m12 == 0.0f && pM->m13 == 0.0f &&
		pM->m20 == 0.0f && pM->m21 == 0.0f && pM->m22 == 1.0f && pM->m23 == 0.0f &&
		pM->m30 == 0.0f && pM->m31 == 0.0f && pM->m32 == 0.0f && pM->m33 == 1.0f;
}

inline HRESULT D3DXMatrixDecompose(D3DXVECTOR3 *pOutScale, D3DXQUATERNION *pOutRotation, D3DXVECTOR3 *pOutTranslation, const D3DXMATRIX *pM)
{
	CrossEngine::MtxToScale(pOutScale, pM);
	CrossEngine::MtxToQuat(pOutRotation, pM);
	CrossEngine::MtxToTranslate(pOutTranslation, pM);
	return S_OK;
}

inline D3DXMATRIX* D3DXMatrixTranspose(D3DXMATRIX *pOut, const D3DXMATRIX *pM)
{
	CrossEngine::MtxTranspose(pOut, pM);
	return pOut;
}

// Matrix multiplication.  The result represents the transformation M2 followed by the transformation M1. (Out = M1 * M2)
inline D3DXMATRIX* D3DXMatrixMultiply(D3DXMATRIX *pOut, const D3DXMATRIX *pM1, const D3DXMATRIX *pM2)
{
	CrossEngine::MtxMul(pOut, pM1, pM2);
	return pOut;
}

// Matrix multiplication, followed by a transpose. (Out = T(M1 * M2))
inline D3DXMATRIX* D3DXMatrixMultiplyTranspose(D3DXMATRIX *pOut, const D3DXMATRIX *pM1, const D3DXMATRIX *pM2)
{
	D3DXMATRIX mtxOut;
	CrossEngine::MtxMul(&mtxOut, pM1, pM2);
	CrossEngine::MtxTranspose(pOut, &mtxOut);
	return pOut;
}

// Calculate inverse of matrix.  Inversion my fail, in which case NULL will
// be returned.  The determinant of pM is also returned it pfDeterminant
// is non-NULL.
inline D3DXMATRIX* D3DXMatrixInverse(D3DXMATRIX *pOut, FLOAT *pDeterminant, const D3DXMATRIX *pM)
{
	CrossEngine::MtxInverse(pOut, pM);
	return pOut;
}

// Build a matrix which scales by (sx, sy, sz)
inline D3DXMATRIX* D3DXMatrixScaling(D3DXMATRIX *pOut, FLOAT sx, FLOAT sy, FLOAT sz)
{
	CrossEngine::MtxDefScale(pOut, sx, sy, sz);
	return pOut;
}

// Build a matrix which translates by (x, y, z)
inline D3DXMATRIX* D3DXMatrixTranslation(D3DXMATRIX *pOut, FLOAT x, FLOAT y, FLOAT z)
{
	CrossEngine::MtxDefTranslate(pOut, x, y, z);
	return pOut;
}

// Build a matrix which rotates around the X axis
inline D3DXMATRIX* D3DXMatrixRotationX(D3DXMATRIX *pOut, FLOAT Angle)
{
	CrossEngine::MtxDefRotateAngleX(pOut, D3DXToRadian(Angle));
	return pOut;
}

// Build a matrix which rotates around the Y axis
inline D3DXMATRIX* D3DXMatrixRotationY(D3DXMATRIX *pOut, FLOAT Angle)
{
	CrossEngine::MtxDefRotateAngleY(pOut, D3DXToRadian(Angle));
	return pOut;
}

// Build a matrix which rotates around the Z axis
inline D3DXMATRIX* D3DXMatrixRotationZ(D3DXMATRIX *pOut, FLOAT Angle)
{
	CrossEngine::MtxDefRotateAngleZ(pOut, D3DXToRadian(Angle));
	return pOut;
}

// Build a matrix which rotates around an arbitrary axis
inline D3DXMATRIX* D3DXMatrixRotationAxis(D3DXMATRIX *pOut, const D3DXVECTOR3 *pV, FLOAT Angle)
{
	CrossEngine::MtxDefRotateAxisAngle(pOut, pV, D3DXToRadian(Angle));
	return pOut;
}

// Build a matrix from a quaternion
inline D3DXMATRIX* D3DXMatrixRotationQuaternion(D3DXMATRIX *pOut, const D3DXQUATERNION *pQ)
{
	CrossEngine::QuatToMtxRotation(pOut, pQ);
	return pOut;
}

// Yaw around the Y axis, a pitch around the X axis, and a roll around the Z axis.
inline D3DXMATRIX* D3DXMatrixRotationYawPitchRoll(D3DXMATRIX *pOut, FLOAT Yaw, FLOAT Pitch, FLOAT Roll)
{
	CrossEngine::MtxDefRotateAngleXYZ(pOut, D3DXToRadian(Pitch), D3DXToRadian(Yaw), D3DXToRadian(Roll));
	return pOut;
}


// Build transformation matrix.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
//inline D3DXMATRIX* D3DXMatrixTransformation( D3DXMATRIX *pOut, const D3DXVECTOR3 *pScalingCenter, const D3DXQUATERNION *pScalingRotation, const D3DXVECTOR3 *pScaling, const D3DXVECTOR3 *pRotationCenter, const D3DXQUATERNION *pRotation, const D3DXVECTOR3 *pTranslation)
//{
//
//}

// Build a lookat matrix. (right-handed)
inline D3DXMATRIX* D3DXMatrixLookAtRH(D3DXMATRIX *pOut, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp)
{
	//zaxis = normal(Eye - At)
	//xaxis = normal(cross(Up, zaxis))
	//yaxis = cross(zaxis, xaxis)

	//xaxis.x           yaxis.x           zaxis.x          0
	//xaxis.y           yaxis.y           zaxis.y          0
	//xaxis.z           yaxis.z           zaxis.z          0
	//-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye) 1

	D3DXVECTOR3 xaxis;
	D3DXVECTOR3 yaxis;
	D3DXVECTOR3 zaxis;

	zaxis = *pEye - *pAt;
	D3DXVec3Normalize(&zaxis, &zaxis);

	D3DXVec3Cross(&xaxis, pUp, &zaxis);
	D3DXVec3Normalize(&xaxis, &xaxis);

	D3DXVec3Cross(&yaxis, &zaxis, &xaxis);
	D3DXVec3Normalize(&yaxis, &yaxis);

	*pOut = D3DXMATRIX(
		xaxis.x,                    yaxis.x,                    zaxis.x,                    0.0f,
		xaxis.y,                    yaxis.y,                    zaxis.y,                    0.0f,
		xaxis.z,                    yaxis.z,                    zaxis.z,                    0.0f,
		-D3DXVec3Dot(&xaxis, pEye), -D3DXVec3Dot(&yaxis, pEye), -D3DXVec3Dot(&zaxis, pEye), 1.0f);

	return pOut;
}

// Build a lookat matrix. (left-handed)
inline D3DXMATRIX* D3DXMatrixLookAtLH(D3DXMATRIX *pOut, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp)
{
	//zaxis = normal(At - Eye)
	//xaxis = normal(cross(Up, zaxis))
	//yaxis = cross(zaxis, xaxis)

	//xaxis.x           yaxis.x           zaxis.x          0
	//xaxis.y           yaxis.y           zaxis.y          0
	//xaxis.z           yaxis.z           zaxis.z          0
	//-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye) 1

	D3DXVECTOR3 xaxis;
	D3DXVECTOR3 yaxis;
	D3DXVECTOR3 zaxis;

	zaxis = *pAt - *pEye;
	D3DXVec3Normalize(&zaxis, &zaxis);

	D3DXVec3Cross(&xaxis, pUp, &zaxis);
	D3DXVec3Normalize(&xaxis, &xaxis);

	D3DXVec3Cross(&yaxis, &zaxis, &xaxis);
	D3DXVec3Normalize(&yaxis, &yaxis);

	pOut->m00 = xaxis.x;                    pOut->m01 = yaxis.x;                    pOut->m02 = zaxis.x;                    pOut->m03 = 0.0f;
	pOut->m10 = xaxis.y;                    pOut->m11 = yaxis.y;                    pOut->m12 = zaxis.y;                    pOut->m13 = 0.0f;
	pOut->m20 = xaxis.z;                    pOut->m21 = yaxis.z;                    pOut->m22 = zaxis.z;                    pOut->m23 = 0.0f;
	pOut->m30 = -D3DXVec3Dot(&xaxis, pEye); pOut->m31 = -D3DXVec3Dot(&yaxis, pEye); pOut->m32 = -D3DXVec3Dot(&zaxis, pEye); pOut->m33 = 1.0f;

	return pOut;

}

// Build a perspective projection matrix. (right-handed)
inline D3DXMATRIX* D3DXMatrixPerspectiveRH(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf)
{
	//2*zn/w  0       0              0
	//0       2*zn/h  0              0
	//0       0       zf/(zn-zf)    -1
	//0       0       zn*zf/(zn-zf)  0

	pOut->m00 = 2.0f * zn / w; pOut->m01 = 0.0f;          pOut->m02 = 0.0f;                pOut->m03 =  0.0f;
	pOut->m10 = 0.0f;          pOut->m11 = 2.0f * zn / h; pOut->m12 = 0.0f;                pOut->m13 =  0.0f;
	pOut->m20 = 0.0f;          pOut->m21 = 0.0f;          pOut->m22 = zf / (zn - zf);      pOut->m23 = -1.0f;
	pOut->m30 = 0.0f;          pOut->m31 = 0.0f;          pOut->m32 = zn * zf / (zn - zf); pOut->m33 =  0.0f;

	return pOut;
}

// Build a perspective projection matrix. (left-handed)
inline D3DXMATRIX* D3DXMatrixPerspectiveLH(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf)
{
	//2*zn/w  0       0              0
	//0       2*zn/h  0              0
	//0       0       zf/(zf-zn)     1
	//0       0       zn*zf/(zn-zf)  0

	pOut->m00 = 2.0f * zn / w; pOut->m01 = 0.0f;          pOut->m02 = 0.0f;                pOut->m03 = 0.0f;
	pOut->m10 = 0.0f;          pOut->m11 = 2.0f * zn / h; pOut->m12 = 0.0f;                pOut->m13 = 0.0f;
	pOut->m20 = 0.0f;          pOut->m21 = 0.0f;          pOut->m22 = zf / (zf - zn);      pOut->m23 = 1.0f;
	pOut->m30 = 0.0f;          pOut->m31 = 0.0f;          pOut->m32 = zn * zf / (zn - zf); pOut->m33 = 0.0f;

	return pOut;
}

// Build a perspective projection matrix. (right-handed)
inline D3DXMATRIX* D3DXMatrixPerspectiveFovRH(D3DXMATRIX *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf)
{
	//xScale     0          0              0
	//0        yScale       0              0
	//0          0      zf/(zn-zf)        -1
	//0          0      zn*zf/(zn-zf)      0
	//where:
	//yScale = cot(fovY/2)
	//xScale = yScale / aspect ratio

	FLOAT yScale = 1.0f / tanf(D3DXToRadian(fovy / 2.0f));
	FLOAT xScale = yScale / Aspect;

	pOut->m00 = xScale; pOut->m01 = 0.0f;   pOut->m02 = 0.0f;                pOut->m03 =  0.0f;
	pOut->m10 = 0.0f;   pOut->m11 = yScale; pOut->m12 = 0.0f;                pOut->m13 =  0.0f;
	pOut->m20 = 0.0f;   pOut->m21 = 0.0f;   pOut->m22 = zf / (zn - zf);      pOut->m23 = -1.0f;
	pOut->m30 = 0.0f;   pOut->m31 = 0.0f;   pOut->m32 = zn * zf / (zn - zf); pOut->m33 =  0.0f;

	return pOut;
}

// Build a perspective projection matrix. (left-handed)
inline D3DXMATRIX* D3DXMatrixPerspectiveFovLH(D3DXMATRIX *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf)
{
	//xScale     0          0               0
	//0        yScale       0               0
	//0          0       zf/(zf-zn)         1
	//0          0       -zn*zf/(zf-zn)     0
	//where:
	//yScale = cot(fovY/2)
	//xScale = yScale / aspect ratio

	FLOAT yScale = 1.0f / tanf(D3DXToRadian(fovy / 2.0f));
	FLOAT xScale = yScale / Aspect;

	pOut->m00 = xScale; pOut->m01 = 0.0f;   pOut->m02 = 0.0f;                pOut->m03 = 0.0f;
	pOut->m10 = 0.0f;   pOut->m11 = yScale; pOut->m12 = 0.0f;                pOut->m13 = 0.0f;
	pOut->m20 = 0.0f;   pOut->m21 = 0.0f;   pOut->m22 = zf / (zf - zn);      pOut->m23 = 1.0f;
	pOut->m30 = 0.0f;   pOut->m31 = 0.0f;   pOut->m32 = zn * zf / (zn - zf); pOut->m33 = 0.0f;

	return pOut;
}

// Build an ortho projection matrix. (right-handed)
inline D3DXMATRIX* D3DXMatrixOrthoRH(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf)
{
	//2/w  0    0           0
	//0    2/h  0           0
	//0    0    1/(zn-zf)   0
	//0    0    zn/(zn-zf)  1

	pOut->m00 = 2.0f / w; pOut->m01 = 0.0f;     pOut->m02 = 0.0f;             pOut->m03 = 0.0f;
	pOut->m10 = 0.0f;     pOut->m11 = 2.0f / h; pOut->m12 = 0.0f;             pOut->m13 = 0.0f;
	pOut->m20 = 0.0f;     pOut->m21 = 0.0f;     pOut->m22 = 1.0f / (zn - zf); pOut->m23 = 0.0f;
	pOut->m30 = 0.0f;     pOut->m31 = 0.0f;     pOut->m32 = zn / (zn - zf);   pOut->m33 = 1.0f;

	return pOut;
}

// Build an ortho projection matrix. (left-handed)
inline D3DXMATRIX* D3DXMatrixOrthoLH(D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf)
{
	//2/w  0    0           0
	//0    2/h  0           0
	//0    0    1/(zf-zn)   0
	//0    0    zn/(zn-zf)  1

	pOut->m00 = 2.0f / w; pOut->m01 = 0.0f;     pOut->m02 = 0.0f;             pOut->m03 = 0.0f;
	pOut->m10 = 0.0f;     pOut->m11 = 2.0f / h; pOut->m12 = 0.0f;             pOut->m13 = 0.0f;
	pOut->m20 = 0.0f;     pOut->m21 = 0.0f;     pOut->m22 = 1.0f / (zf - zn); pOut->m23 = 0.0f;
	pOut->m30 = 0.0f;     pOut->m31 = 0.0f;     pOut->m32 = zn / (zn - zf);   pOut->m33 = 1.0f;

	return pOut;
}


//--------------------------
// Quaternion
//--------------------------
inline FLOAT D3DXQuaternionLength(const D3DXQUATERNION *pQ)
{
	return sqrtf(pQ->x * pQ->x + pQ->y * pQ->y + pQ->z * pQ->z + pQ->w * pQ->w);
}

// Length squared, or "norm"
inline FLOAT D3DXQuaternionLengthSq(const D3DXQUATERNION *pQ)
{
	return pQ->x * pQ->x + pQ->y * pQ->y + pQ->z * pQ->z + pQ->w * pQ->w;
}

inline FLOAT D3DXQuaternionDot(const D3DXQUATERNION *pQ1, const D3DXQUATERNION *pQ2)
{
	return pQ1->x * pQ2->x + pQ1->y * pQ2->y + pQ1->z * pQ2->z + pQ1->w * pQ2->w;
}

// (0, 0, 0, 1)
inline D3DXQUATERNION* D3DXQuaternionIdentity(D3DXQUATERNION *pOut)
{
	pOut->x = 0.0f;
	pOut->y = 0.0f;
	pOut->z = 0.0f;
	pOut->w = 1.0f;
	return pOut;
}

inline BOOL D3DXQuaternionIsIdentity(const D3DXQUATERNION *pQ)
{
	return pQ->x == 0.0f && pQ->y == 0.0f && pQ->z == 0.0f && pQ->w == 1.0f;
}

// (-x, -y, -z, w)
inline D3DXQUATERNION* D3DXQuaternionConjugate(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ)
{
	pOut->x = -pQ->x;
	pOut->y = -pQ->y;
	pOut->z = -pQ->z;
	pOut->w =  pQ->w;
	return pOut;
}

// Compute a quaternin's axis and angle of rotation. Expects unit quaternions.
//inline VOID D3DXQuaternionToAxisAngle(const D3DXQUATERNION *pQ, D3DXVECTOR3 *pAxis, FLOAT *pAngle)
//{
//
//}

// Build a quaternion from a rotation matrix.
inline D3DXQUATERNION* D3DXQuaternionRotationMatrix(D3DXQUATERNION *pOut, const D3DXMATRIX *pM)
{
	CrossEngine::MtxToQuat(pOut, pM);
	return pOut;
}

// Rotation about arbitrary axis.
inline D3DXQUATERNION* D3DXQuaternionRotationAxis(D3DXQUATERNION *pOut, const D3DXVECTOR3 *pV, FLOAT Angle)
{
	CrossEngine::QuatDefRotateAxisAngle(pOut, pV, D3DXToRadian(Angle));
	return pOut;
}

// Yaw around the Y axis, a pitch around the X axis, and a roll around the Z axis.
inline D3DXQUATERNION* D3DXQuaternionRotationYawPitchRoll(D3DXQUATERNION *pOut, FLOAT Yaw, FLOAT Pitch, FLOAT Roll)
{
	CrossEngine::QuatDefRotateAngleXYZ(pOut, D3DXToRadian(Pitch), D3DXToRadian(Yaw), D3DXToRadian(Roll));
	return pOut;
}

// Quaternion multiplication. The result represents the rotation Q2
// followed by the rotation Q1. (Out = Q2 * Q1)
inline D3DXQUATERNION* D3DXQuaternionMultiply(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ1, const D3DXQUATERNION *pQ2)
{
	CrossEngine::QuatMul(pOut, pQ2, pQ1);
	return pOut;
}

inline D3DXQUATERNION* D3DXQuaternionNormalize(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ)
{
	FLOAT len = sqrtf(pQ->x * pQ->x + pQ->y * pQ->y + pQ->z * pQ->z + pQ->w * pQ->w);
	FLOAT invLen = 1.0f / len;

	pOut->x = pQ->x * invLen;
	pOut->y = pQ->y * invLen;
	pOut->z = pQ->z * invLen;
	pOut->w = pQ->w * invLen;

	return pOut;
}

// Conjugate and re-norm
inline D3DXQUATERNION* D3DXQuaternionInverse(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ)
{
	CrossEngine::QuatInverse(pOut, pQ);
	return pOut;
}

// Expects unit quaternions.
// if q = (cos(theta), sin(theta) * v); ln(q) = (0, theta * v)
//inline D3DXQUATERNION* D3DXQuaternionLn(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ)
//{
//
//}

// Expects pure quaternions. (w == 0)  w is ignored in calculation.
// if q = (0, theta * v); exp(q) = (cos(theta), sin(theta) * v)
inline D3DXQUATERNION* D3DXQuaternionExp(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ)
{
	CrossEngine::QuatExp(pOut, pQ);
	return pOut;
}

// Spherical linear interpolation between Q1 (t == 0) and Q2 (t == 1).
// Expects unit quaternions.
inline D3DXQUATERNION* D3DXQuaternionSlerp(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ1, const D3DXQUATERNION *pQ2, FLOAT t)
{
	CrossEngine::QuatSlerp(pOut, pQ1, pQ2, t);
	return pOut;
}

// Spherical quadrangle interpolation.
// Slerp(Slerp(Q1, C, t), Slerp(A, B, t), 2t(1-t))
inline D3DXQUATERNION* D3DXQuaternionSquad(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ1, const D3DXQUATERNION *pA, const D3DXQUATERNION *pB, const D3DXQUATERNION *pC, FLOAT t)
{
	CrossEngine::QuatSquad(pOut, pQ1, pA, pB, pC, t);
	return pOut;
}

// Setup control points for spherical quadrangle interpolation
// from Q1 to Q2.  The control points are chosen in such a way 
// to ensure the continuity of tangents with adjacent segments.
//inline VOID D3DXQuaternionSquadSetup(D3DXQUATERNION *pAOut, D3DXQUATERNION *pBOut, D3DXQUATERNION *pCOut, const D3DXQUATERNION *pQ0, const D3DXQUATERNION *pQ1, const D3DXQUATERNION *pQ2, const D3DXQUATERNION *pQ3)
//{
//
//}

// Barycentric interpolation.
// Slerp(Slerp(Q1, Q2, f+g), Slerp(Q1, Q3, f+g), g/(f+g))
//inline D3DXQUATERNION* D3DXQuaternionBaryCentric(D3DXQUATERNION *pOut, const D3DXQUATERNION *pQ1, const D3DXQUATERNION *pQ2, const D3DXQUATERNION *pQ3, FLOAT f, FLOAT g)
//{
//
//}


//--------------------------
// Color
//--------------------------
// (1-r, 1-g, 1-b, a)
inline D3DXCOLOR* D3DXColorNegative(D3DXCOLOR *pOut, const D3DXCOLOR *pC)
{
	pOut->r = 1.0f - pC->r;
	pOut->g = 1.0f - pC->g;
	pOut->b = 1.0f - pC->b;
	pOut->a = pC->a;
	return pOut;
}

inline D3DXCOLOR* D3DXColorAdd(D3DXCOLOR *pOut, const D3DXCOLOR *pC1, const D3DXCOLOR *pC2)
{
	pOut->r = pC1->r + pC2->r;
	pOut->g = pC1->g + pC2->g;
	pOut->b = pC1->b + pC2->b;
	pOut->a = pC1->a + pC2->a;
	return pOut;
}

inline D3DXCOLOR* D3DXColorSubtract(D3DXCOLOR *pOut, const D3DXCOLOR *pC1, const D3DXCOLOR *pC2)
{
	pOut->r = pC1->r - pC2->r;
	pOut->g = pC1->g - pC2->g;
	pOut->b = pC1->b - pC2->b;
	pOut->a = pC1->a - pC2->a;
	return pOut;
}

inline D3DXCOLOR* D3DXColorScale(D3DXCOLOR *pOut, const D3DXCOLOR *pC, FLOAT s)
{
	pOut->r = pC->r * s;
	pOut->g = pC->g * s;
	pOut->b = pC->b * s;
	pOut->a = pC->a * s;
	return pOut;
}

// (r1*r2, g1*g2, b1*b2, a1*a2)
inline D3DXCOLOR* D3DXColorModulate(D3DXCOLOR *pOut, const D3DXCOLOR *pC1, const D3DXCOLOR *pC2)
{
	pOut->r = pC1->r * pC2->r;
	pOut->g = pC1->g * pC2->g;
	pOut->b = pC1->b * pC2->b;
	pOut->a = pC1->a * pC2->a;
	return pOut;
}

// Linear interpolation of r,g,b, and a. C1 + s(C2-C1)
inline D3DXCOLOR* D3DXColorLerp(D3DXCOLOR *pOut, const D3DXCOLOR *pC1, const D3DXCOLOR *pC2, FLOAT s)
{
	pOut->r = pC1->r + s * (pC2->r - pC1->r);
	pOut->g = pC1->g + s * (pC2->g - pC1->g);
	pOut->b = pC1->b + s * (pC2->b - pC1->b);
	pOut->a = pC1->a + s * (pC2->a - pC1->a);
	return pOut;
}

// Interpolate r,g,b between desaturated color and color.
// DesaturatedColor + s(Color - DesaturatedColor)
inline D3DXCOLOR* D3DXColorAdjustSaturation(D3DXCOLOR *pOut, const D3DXCOLOR *pC, FLOAT s)
{
	FLOAT grey = pC->r * 0.2125f + pC->g * 0.7154f + pC->b * 0.0721f;

	pOut->r = grey + s * (pC->r - grey);
	pOut->g = grey + s * (pC->g - grey);
	pOut->b = grey + s * (pC->b - grey);

	return pOut;
}

// Interpolate r,g,b between 50% grey and color.  Grey + s(Color - Grey)
inline D3DXCOLOR* D3DXColorAdjustContrast(D3DXCOLOR *pOut, const D3DXCOLOR *pC, FLOAT c)
{
	pOut->r = 0.5f + c * (pC->r - 0.5f);
	pOut->g = 0.5f + c * (pC->g - 0.5f);
	pOut->b = 0.5f + c * (pC->b - 0.5f);
	return pOut;
}

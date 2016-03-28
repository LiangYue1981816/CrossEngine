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


typedef char                           CHAR;     // 1BYTE
typedef short                          SHORT;    // 2BYTE
typedef int                            INT;      // 4BYTE
typedef unsigned int                   UINT;     // 4BYTE
typedef float                          FLOAT;    // 4BYTE
typedef double                         DOUBLE;   // 8BYTE

typedef long long                      INT64;    // 8BYTE
typedef unsigned long long             UINT64;   // 8BYTE

#ifdef WIN32
typedef long                           LONG;     // 4BYTE
typedef unsigned long                  ULONG;    // 4BYTE

typedef unsigned char                  BYTE;     // 1BYTE
typedef unsigned short                 WORD;     // 2BYTE
typedef unsigned long                  DWORD;    // 4BYTE
typedef unsigned long                  COLORREF; // 4BYTE
#else
typedef int                            LONG;     // 4BYTE
typedef unsigned int                   ULONG;    // 4BYTE

typedef unsigned char                  BYTE;     // 1BYTE
typedef unsigned short                 WORD;     // 2BYTE
typedef unsigned int                   DWORD;    // 4BYTE
typedef unsigned int                   COLORREF; // 4BYTE
#endif

#ifdef __LP64__
#define POINTER_SIZE                   8         // 8BYTE
#else
#define POINTER_SIZE                   4         // 4BYTE
#endif


#ifndef VOID
#define VOID                           void
#endif

#ifndef BOOL
#define BOOL                           INT
#endif

#ifndef TRUE
#define TRUE                           1
#endif

#ifndef FALSE
#define FALSE                          0
#endif

typedef LONG                           HRESULT;

#ifndef S_OK
#define S_OK                           ((HRESULT)0L)
#endif

#ifndef S_FALSE
#define S_FALSE                        ((HRESULT)1L)
#endif

#define SWAP(a, b, tmp)                { (tmp) = (a); (a) = (b); (b) = (tmp); }

#define ALIGN_4BYTE(a)                 ((((a) + 3) / 4) * 4)
#define ALIGN_16BYTE(a)                ((((a) + 15) / 16) * 16)

#define _MAX_STRING                    260

#ifndef FLT_MAX
#define FLT_MAX                        3.402823466e+38f
#endif

#ifndef FLT_MIN
#define FLT_MIN                        1.175494351e-38f
#endif

#ifndef INVALID_HASHNAME
#define INVALID_HASHNAME               0xffffffff
#endif

#ifndef INVALID_VALUE
#define INVALID_VALUE                  0xffffffff
#endif

#ifndef ASSERT
#define ASSERT                         assert
#endif

#ifndef _NLSCMP_DEFINED
#define _NLSCMPERROR                   2147483647  /* currently == INT_MAX */
#define _NLSCMP_DEFINED
#endif

#ifndef max
#define max(a,b)                       (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)                       (((a) < (b)) ? (a) : (b))
#endif

#define GET_RED(color)                 (BYTE)((color)  &  0xff)
#define GET_GRN(color)                 (BYTE)(((color) >> 0x08) & 0xff)
#define GET_BLU(color)                 (BYTE)(((color) >> 0x10) & 0xff)


typedef struct {
	FLOAT x;
	FLOAT y;
} FPOINT;

typedef struct {
	FLOAT left;
	FLOAT top;
	FLOAT right;
	FLOAT bottom;
} FRECT;


#ifdef _WINDOWS_
#define _CrossExport                   __declspec(dllexport)
#else
#define _CrossExport

#define NO_ERROR                       0

typedef DWORD                          HWND;
typedef DWORD                          HDC;
typedef DWORD                          HGLRC;
typedef VOID*                          LPVOID;
typedef wchar_t                        WCHAR;

#define BI_RGB                         0L
#define BI_RLE8                        1L
#define BI_RLE4                        2L
#define BI_BITFIELDS                   3L

typedef struct {
	LONG x;
	LONG y;
} POINT;

typedef struct {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT;

typedef struct {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQUAD;

#pragma pack(push, 1)

typedef struct { 
	BYTE rgbtBlue; 
	BYTE rgbtGreen; 
	BYTE rgbtRed; 
} RGBTRIPLE;

typedef struct { 
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER;

#pragma pack(pop)

typedef struct {
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct { 
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[1];
} BITMAPINFO;

typedef struct {
	LONG bmType; 
	LONG bmWidth; 
	LONG bmHeight; 
	LONG bmWidthBytes; 
	WORD bmPlanes; 
	WORD bmBitsPixel; 
	LPVOID bmBits; 
} BITMAP;
#endif

#ifndef RGB
#define RGB(r,g,b)                     ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

#ifndef RGBA
#define RGBA(r,g,b,a)                  ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24))
#endif

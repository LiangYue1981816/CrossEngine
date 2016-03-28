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

#include "CCDrawingPrimitives.h"
#include "ccTypes.h"
#include "ccMacros.h"
#include "CCGL.h"
#include "CCDirector.h"
#include <string.h>
#include <cmath>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

namespace cocos2d {

	void ccDrawPoint(const ccVertex2F& point, const ccColor4B &color)
	{
		ccVertex2F vertex = {point.x * CC_CONTENT_SCALE_FACTOR(), point.y * CC_CONTENT_SCALE_FACTOR()};

		// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
		// Needed states: GL_VERTEX_ARRAY, GL_COLOR_ARRAY
		// Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY
		CrossEngine::cglDisable(GL_TEXTURE_2D);
		CrossEngine::cglDisableClientState(GL_TEXTURE_COORD_ARRAY);

		CrossEngine::cglVertexPointer(2, GL_FLOAT, 0, &vertex);
		CrossEngine::cglColorPointer(4, GL_UNSIGNED_BYTE, 0, &color);
		CrossEngine::cglDrawArraysCompatible(GL_POINTS, 0, 1);

		// restore default state
		CrossEngine::cglEnableClientState(GL_TEXTURE_COORD_ARRAY);
		CrossEngine::cglEnable(GL_TEXTURE_2D);
	}

	void ccDrawPoints(const ccVertex2F *points, const ccColor4B *colors, unsigned int numberOfPoints)
	{
		ccVertex2F *newVertices = new ccVertex2F[numberOfPoints];

		for (unsigned int i= 0; i < numberOfPoints; i++)
		{
			newVertices[i].x = points[i].x * CC_CONTENT_SCALE_FACTOR();
			newVertices[i].y = points[i].y * CC_CONTENT_SCALE_FACTOR();
		}

		// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
		// Needed states: GL_VERTEX_ARRAY, GL_COLOR_ARRAY
		// Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY
		CrossEngine::cglDisable(GL_TEXTURE_2D);
		CrossEngine::cglDisableClientState(GL_TEXTURE_COORD_ARRAY);

		CrossEngine::cglVertexPointer(2, GL_FLOAT, 0, newVertices);
		CrossEngine::cglColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
		CrossEngine::cglDrawArraysCompatible(GL_POINTS, 0, (GLsizei)numberOfPoints);

		// restore default state
		CrossEngine::cglEnableClientState(GL_TEXTURE_COORD_ARRAY);
		CrossEngine::cglEnable(GL_TEXTURE_2D);

		delete[] newVertices;
	}

	void ccDrawLine(const ccVertex2F& origin, const ccColor4B &originColor, const ccVertex2F& destination, const ccColor4B &destinationColor)
	{
		ccVertex2F vertices[2] = 
		{
			{origin.x * CC_CONTENT_SCALE_FACTOR(), origin.y * CC_CONTENT_SCALE_FACTOR()},
			{destination.x * CC_CONTENT_SCALE_FACTOR(), destination.y * CC_CONTENT_SCALE_FACTOR()},
		};

		ccColor4B colors[2] = 
		{
			{originColor.r, originColor.g, originColor.b, originColor.a},
			{destinationColor.r, destinationColor.g, destinationColor.b, destinationColor.a},
		};

		// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
		// Needed states: GL_VERTEX_ARRAY, GL_COLOR_ARRAY
		// Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY
		CrossEngine::cglDisable(GL_TEXTURE_2D);
		CrossEngine::cglDisableClientState(GL_TEXTURE_COORD_ARRAY);

		CrossEngine::cglVertexPointer(2, GL_FLOAT, 0, vertices);
		CrossEngine::cglColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
		CrossEngine::cglDrawArraysCompatible(GL_LINES, 0, 2);

		// restore default state
		CrossEngine::cglEnableClientState(GL_TEXTURE_COORD_ARRAY);
		CrossEngine::cglEnable(GL_TEXTURE_2D);
	}

	void ccDrawPoly(const ccVertex2F *vertices, const ccColor4B *colors, int numberOfPoints, bool closePolygon)
	{
		ccDrawPoly(vertices, colors, numberOfPoints, closePolygon, false);
	}

	void ccDrawPoly(const ccVertex2F *vertices, const ccColor4B *colors, int numberOfPoints, bool closePolygon, bool fill)
	{
		ccVertex2F* newVertices = new ccVertex2F[numberOfPoints];

		for (int i=0; i<numberOfPoints;i++)
		{
			newVertices[i].x = vertices[i].x * CC_CONTENT_SCALE_FACTOR();
			newVertices[i].y = vertices[i].y * CC_CONTENT_SCALE_FACTOR();
		}

		// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
		// Needed states: GL_VERTEX_ARRAY, GL_COLOR_ARRAY
		// Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY
		CrossEngine::cglDisable(GL_TEXTURE_2D);
		CrossEngine::cglDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if (closePolygon)
		{
			CrossEngine::cglVertexPointer(2, GL_FLOAT, 0, newVertices);
			CrossEngine::cglColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
			CrossEngine::cglDrawArraysCompatible(fill? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, numberOfPoints);
		}
		else
		{
			CrossEngine::cglVertexPointer(2, GL_FLOAT, 0, newVertices);
			CrossEngine::cglColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
			CrossEngine::cglDrawArraysCompatible(fill? GL_TRIANGLE_FAN : GL_LINE_STRIP, 0, numberOfPoints);
		}

		// restore default state
		CrossEngine::cglEnableClientState(GL_TEXTURE_COORD_ARRAY);
		CrossEngine::cglEnable(GL_TEXTURE_2D);

		delete[] newVertices;
	}

	void ccDrawCircle(const ccVertex2F& center, float radius, float angle, int segments, bool drawLineToCenter, const ccColor4B &color)
	{
		ccColor4B *colors = new ccColor4B[segments + 2];
		ccVertex2F *vertices = new ccVertex2F[segments + 2];

		int additionalSegment = drawLineToCenter ? 2 : 1;
		const float coef = 2.0f * (float) (M_PI) /segments;

		for (int i=0;i<=segments;i++)
		{
			float rads = i*coef;
			float j = radius * cosf(rads + angle) + center.x;
			float k = radius * sinf(rads + angle) + center.y;

			vertices[i].x = j * CC_CONTENT_SCALE_FACTOR();
			vertices[i].y = k * CC_CONTENT_SCALE_FACTOR();

			colors[i] = color;
		}

		vertices[segments+1].x = center.x * CC_CONTENT_SCALE_FACTOR();
		vertices[segments+1].y = center.y * CC_CONTENT_SCALE_FACTOR();

		colors[segments+1] = color;

		// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
		// Needed states: GL_VERTEX_ARRAY, GL_COLOR_ARRAY
		// Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY
		CrossEngine::cglDisable(GL_TEXTURE_2D);
		CrossEngine::cglDisableClientState(GL_TEXTURE_COORD_ARRAY);

		CrossEngine::cglVertexPointer(2, GL_FLOAT, 0, vertices);
		CrossEngine::cglColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
		CrossEngine::cglDrawArraysCompatible(GL_LINE_STRIP, 0, (GLsizei) segments + additionalSegment);

		// restore default state
		CrossEngine::cglEnableClientState(GL_TEXTURE_COORD_ARRAY);
		CrossEngine::cglEnable(GL_TEXTURE_2D);

		delete[] vertices;
		delete[] colors;
	}

	void ccDrawQuadBezier(const ccVertex2F& origin, const ccVertex2F& control, const ccVertex2F& destination, int segments, const ccColor4B &color)
	{
		ccColor4B *colors = new ccColor4B[segments + 1];
		ccVertex2F *vertices = new ccVertex2F[segments + 1];

		float t = 0.0f;

		for (int i=0; i<segments; i++)
		{
			float x = powf(1 - t, 2) * origin.x + 2.0f * (1 - t) * t * control.x + t * t * destination.x;
			float y = powf(1 - t, 2) * origin.y + 2.0f * (1 - t) * t * control.y + t * t * destination.y;
			t += 1.0f / segments;

			vertices[i].x = x * CC_CONTENT_SCALE_FACTOR();
			vertices[i].y = y * CC_CONTENT_SCALE_FACTOR();

			colors[i] = color;
		}

		vertices[segments].x = destination.x * CC_CONTENT_SCALE_FACTOR();
		vertices[segments].y = destination.y * CC_CONTENT_SCALE_FACTOR();

		colors[segments] = color;

		// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
		// Needed states: GL_VERTEX_ARRAY, GL_COLOR_ARRAY
		// Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY
		CrossEngine::cglDisable(GL_TEXTURE_2D);
		CrossEngine::cglDisableClientState(GL_TEXTURE_COORD_ARRAY);

		CrossEngine::cglVertexPointer(2, GL_FLOAT, 0, vertices);
		CrossEngine::cglColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
		CrossEngine::cglDrawArraysCompatible(GL_LINE_STRIP, 0, (GLsizei) segments + 1);

		// restore default state
		CrossEngine::cglEnableClientState(GL_TEXTURE_COORD_ARRAY);
		CrossEngine::cglEnable(GL_TEXTURE_2D);

		delete[] vertices;
		delete[] colors;
	}

	void ccDrawCubicBezier(const ccVertex2F& origin, const ccVertex2F& control1, const ccVertex2F& control2, const ccVertex2F& destination, int segments, const ccColor4B &color)
	{
		ccColor4B *colors = new ccColor4B[segments + 1];
		ccVertex2F *vertices = new ccVertex2F[segments + 1];

		float t = 0.0f;

		for (int i = 0; i < segments; ++i)
		{
			float x = powf(1 - t, 3) * origin.x + 3.0f * powf(1 - t, 2) * t * control1.x + 3.0f * (1 - t) * t * t * control2.x + t * t * t * destination.x;
			float y = powf(1 - t, 3) * origin.y + 3.0f * powf(1 - t, 2) * t * control1.y + 3.0f * (1 - t) * t * t * control2.y + t * t * t * destination.y;
			t += 1.0f / segments;

			vertices[i].x = x * CC_CONTENT_SCALE_FACTOR();
			vertices[i].y = y * CC_CONTENT_SCALE_FACTOR();

			colors[i] = color;
		}

		vertices[segments].x = destination.x * CC_CONTENT_SCALE_FACTOR();
		vertices[segments].y = destination.y * CC_CONTENT_SCALE_FACTOR();

		colors[segments] = color;

		// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
		// Needed states: GL_VERTEX_ARRAY, GL_COLOR_ARRAY
		// Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY
		CrossEngine::cglDisable(GL_TEXTURE_2D);
		CrossEngine::cglDisableClientState(GL_TEXTURE_COORD_ARRAY);

		CrossEngine::cglVertexPointer(2, GL_FLOAT, 0, vertices);
		CrossEngine::cglColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
		CrossEngine::cglDrawArraysCompatible(GL_LINE_STRIP, 0, (GLsizei) segments + 1);

		// restore default state
		CrossEngine::cglEnableClientState(GL_TEXTURE_COORD_ARRAY);
		CrossEngine::cglEnable(GL_TEXTURE_2D);

		delete[] vertices;
		delete[] colors;
	}

}

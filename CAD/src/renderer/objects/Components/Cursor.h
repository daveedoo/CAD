#pragma once
#include <GL/gl.h>

struct Cursor
{
	GLfloat lineWidth;
	float lineLength;

	Cursor(GLfloat lineWidth = 2.f, float lineLength = 3.f)
		: lineWidth(lineWidth), lineLength(lineLength) {}
};


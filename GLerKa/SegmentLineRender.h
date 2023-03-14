#pragma once
#include "Depencies.h"
#include "LoaderColor.h"

#include <vector>
#include <map>

namespace glerka_lib
{
	// we have a distance between 0.0f and 1.0f
	static const float MIN_X = 0.3f;
	static const float MAX_X = 0.7f;
	static const float MIN_Y = 0.2f;
	static const float MID_Y = 0.55f;
	static const float MAX_Y = 0.8f;

	static const float SPACE = 0.6f;

	struct SegmentMatrix
	{
		bool led1 : 1;
		bool led2 : 1;
		bool led3 : 1;
		bool led4 : 1;
		bool led5 : 1;
		bool led6 : 1;
		bool led7 : 1;
	};

	SegmentMatrix ConvertToMatrix(char matrix) // 0b X000 0000
	{
		SegmentMatrix sm = *reinterpret_cast<SegmentMatrix*>(&matrix);
		return sm;
	}

	static const std::map<char, SegmentMatrix> SEGMENT_MAP = {
		{'0', ConvertToMatrix(0b1110111)},
		{'1', ConvertToMatrix(0b0000011)},
		{'2', ConvertToMatrix(0b0111110)},
		{'3', ConvertToMatrix(0b0011111)},
		{'4', ConvertToMatrix(0b1001011)},
		{'5', ConvertToMatrix(0b1011101)},
		{'6', ConvertToMatrix(0b1111101)},
		{'7', ConvertToMatrix(0b0010011)},
		{'8', ConvertToMatrix(0b1111111)},
		{'9', ConvertToMatrix(0b1011111)},
		{'a', ConvertToMatrix(0b1111011)},
		{'b', ConvertToMatrix(0b1101101)},
		{'c', ConvertToMatrix(0b0101100)},
		{'d', ConvertToMatrix(0b0101111)},
		{'e', ConvertToMatrix(0b1111100)},
		{'f', ConvertToMatrix(0b1111000)},
		{'g', ConvertToMatrix(0b1110101)},
		{'h', ConvertToMatrix(0b1101001)},
		{'i', ConvertToMatrix(0b0110000)},
		{'j', ConvertToMatrix(0b0010011)},
		{'k', ConvertToMatrix(0b1111001)},
		{'l', ConvertToMatrix(0b1100100)},
		{'m', ConvertToMatrix(0b0110001)},
		{'n', ConvertToMatrix(0b1110011)},
		{'o', ConvertToMatrix(0b0101101)},
		{'p', ConvertToMatrix(0b1111010)},
		{'q', ConvertToMatrix(0b1011011)},
		{'r', ConvertToMatrix(0b1110010)},
		{'s', ConvertToMatrix(0b1001101)},
		{'t', ConvertToMatrix(0b1101100)},
		{'u', ConvertToMatrix(0b1100111)},
		{'v', ConvertToMatrix(0b1000111)},
		{'w', ConvertToMatrix(0b1000110)},
		{'x', ConvertToMatrix(0b1101011)},
		{'y', ConvertToMatrix(0b1001111)},
		{'z', ConvertToMatrix(0b0011110)},
		{'A', ConvertToMatrix(0b1111011)},
		{'B', ConvertToMatrix(0b1101101)},
		{'C', ConvertToMatrix(0b0101100)},
		{'D', ConvertToMatrix(0b0101111)},
		{'E', ConvertToMatrix(0b1111100)},
		{'F', ConvertToMatrix(0b1111000)},
		{'G', ConvertToMatrix(0b1110101)},
		{'H', ConvertToMatrix(0b1101001)},
		{'I', ConvertToMatrix(0b0110000)},
		{'J', ConvertToMatrix(0b0010011)},
		{'K', ConvertToMatrix(0b1111001)},
		{'L', ConvertToMatrix(0b1100100)},
		{'M', ConvertToMatrix(0b0110001)},
		{'N', ConvertToMatrix(0b1110011)},
		{'O', ConvertToMatrix(0b0101101)},
		{'P', ConvertToMatrix(0b1111010)},
		{'Q', ConvertToMatrix(0b1011011)},
		{'R', ConvertToMatrix(0b1110010)},
		{'S', ConvertToMatrix(0b1001101)},
		{'T', ConvertToMatrix(0b1101100)},
		{'U', ConvertToMatrix(0b1100111)},
		{'V', ConvertToMatrix(0b1000111)},
		{'W', ConvertToMatrix(0b1000110)},
		{'X', ConvertToMatrix(0b1101011)},
		{'Y', ConvertToMatrix(0b1001111)},
		{'Z', ConvertToMatrix(0b0011110)},
		{'-', ConvertToMatrix(0b0001000)}
	};

	inline void RenderLine(float x1, float y1, float x2, float y2, Color col, float width)
	{
		glLineWidth(width);
		glBegin(GL_LINES);
		glColor3ub(col.r, col.g, col.b);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glEnd();
	}

	inline void RenderChar(char ch, Color col, float width)
	{
		SegmentMatrix matrix_map = {0};

		try
		{
			auto iter = SEGMENT_MAP.find(ch);
			if (iter == SEGMENT_MAP.end())
				throw 0;
			matrix_map = iter->second;
		}
		catch (...)
		{
			return;
		}

		if(matrix_map.led7)
			RenderLine(MIN_X, MAX_Y, MIN_X, MID_Y, col, width);
		if (matrix_map.led6)
			RenderLine(MIN_X, MID_Y, MIN_X, MIN_Y, col, width);
		if (matrix_map.led5)
			RenderLine(MIN_X, MAX_Y, MAX_X, MAX_Y, col, width);
		if (matrix_map.led4)
			RenderLine(MIN_X, MID_Y, MAX_X, MID_Y, col, width);
		if (matrix_map.led3)
			RenderLine(MIN_X, MIN_Y, MAX_X, MIN_Y, col, width);
		if (matrix_map.led2)
			RenderLine(MAX_X, MAX_Y, MAX_X, MID_Y, col, width);		
		if (matrix_map.led1)
			RenderLine(MAX_X, MID_Y, MAX_X, MIN_Y, col, width);
	}

	inline void RenderWord(std::string word, Color col, float width)
	{
		glPushMatrix();
		for (auto& ch : word)
		{
			RenderChar(ch, col, width);
			glTranslatef(SPACE, 0.0f, 0.0f);
		}
		glPopMatrix();
	}
}

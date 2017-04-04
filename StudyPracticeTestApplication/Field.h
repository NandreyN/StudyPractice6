#pragma once
#include <windows.h>
#include <vector>
#include "Cell.h"

class Field
{
public:
	Field();
	Field(int x, int y, int fieldDimension);
	void prepareBackground(HDC& hdc);
	int getClickedCellNumber(int, int);
private:
	std::vector<Cell> _cells;
	int _totalPixelsX, _totalPixelsY;
};


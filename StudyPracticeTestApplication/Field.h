#pragma once
#include <windows.h>
#include <vector>
#include "Cell.h"

struct Question;

class Field
{
public:
	Field();
	Field(int x, int y, int fieldDimension);
	void prepareBackground(HDC& hdc);
	int getClickedCellNumber(int, int);
	void markCellSeen(HDC&, int, COLORREF);
	void handleCellAction(HWND&, HDC&, int, Question);
	int getMark() const;
	bool isPassed() const;
private:
	std::vector<Cell> _cells;
	int _totalPixelsX, _totalPixelsY;
	std::vector<bool>  _arePassed;
	static BOOL CALLBACK TextItemDialog(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
};


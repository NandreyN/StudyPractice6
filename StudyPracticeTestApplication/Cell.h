#pragma once
#include <windows.h>

class Cell
{
public:
	Cell(int ,int,int,int,int);
	Cell(RECT& cellRect,int);
	int getNumber() const;
	RECT getRect() const;
private:
	RECT _cellRect;
	int _orderNumber;
};


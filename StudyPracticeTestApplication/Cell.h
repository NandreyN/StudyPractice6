#pragma once
#include <windows.h>

class Cell
{
public:
	Cell(int ,int,int,int,int);
	Cell(RECT& cellRect,int);
	int getNumber() const;
	RECT getRect() const;
	void setColor(long color);
	long getColor() const;
private:
	RECT _cellRect;
	int _orderNumber;
	long _color;
};


#include "Cell.h"

Cell::Cell(int left, int top, int right, int bottom, int number) 
{
	SetRect(&_cellRect,left,top, right, bottom);
	_orderNumber = number;
	_color = RGB(255, 255, 255);
}

Cell::Cell(RECT& cellRect, int number)
{
	_cellRect = cellRect;
	_orderNumber = number;
	_color = RGB(255, 255, 255);
}

int Cell::getNumber() const
{
	return _orderNumber;
}

RECT Cell::getRect() const
{
	return _cellRect;
}

void Cell::setColor(long color)
{
	_color = color;
}

long Cell::getColor() const
{
	return _color;
}
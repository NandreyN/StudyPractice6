#include "Cell.h"

Cell::Cell(int left, int top, int right, int bottom, int number) 
{
	SetRect(&_cellRect,left,top, right, bottom);
	_orderNumber = number;
}

Cell::Cell(RECT& cellRect, int number)
{
	_cellRect = cellRect;
	_orderNumber = number;
}

int Cell::getNumber() const
{
	return _orderNumber;
}

RECT Cell::getRect() const
{
	return _cellRect;
}
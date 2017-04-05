#include "Field.h"
#include <algorithm>
#define CHOSENCOLOR RGB(154,154,154)
using namespace std;

Field::Field()
{
	_totalPixelsX = 0;
	_totalPixelsY = 0;
}

Field::Field(int x, int y, int fieldDimension)
{
	_totalPixelsX = x;
	_totalPixelsY = y;

	int width = static_cast<double>(x) / fieldDimension, height = static_cast<double>(y) / fieldDimension;
	_cells.reserve(pow(fieldDimension, 2));

	int idCount = 0;
	for (int cellYNum = 0; cellYNum < fieldDimension; cellYNum++)
	{
		for (int cellXNum = 0; cellXNum < fieldDimension; cellXNum++)
		{
			RECT rectangle;
			SetRect(&rectangle, cellXNum*((double)_totalPixelsX / fieldDimension), cellYNum*((double)_totalPixelsY / fieldDimension),
				(cellXNum + 1)*((double)_totalPixelsX / fieldDimension), (cellYNum + 1)*((double)_totalPixelsY / fieldDimension));
			_cells.push_back(Cell(rectangle, idCount));
			++idCount;
		}
	}
}

int Field::getClickedCellNumber(int clickedX, int clickedY)
{
	int cellsDimensionX = ((double)_totalPixelsX / sqrt(_cells.size())),
		cellsDimensionY = ((double)_totalPixelsY / sqrt(_cells.size()));
	int xCount = (clickedX / cellsDimensionX),
		yCount = (clickedY / cellsDimensionY);
	int i = 0, j = 0, counter = 0;

	while (i != sqrt(_cells.size()) && j != sqrt(_cells.size()))
	{
		if (i == xCount && j == yCount) return counter;
		i++;
		counter++;
		if (i == sqrt(_cells.size()))
		{
			i = 0;
			j++;
		}
		if (j == sqrt(_cells.size()) - 1 && i == sqrt(_cells.size()) - 1)
			return counter;
	}
	return counter;
}

void Field::markCellSeen(HDC& hdc, int id)
{
	HBRUSH brush = CreateSolidBrush(CHOSENCOLOR);
	HBRUSH old = (HBRUSH)SelectObject(hdc, brush);

	FillRect(hdc, &_cells[id].getRect(), brush);
	_cells[id].setColor(CHOSENCOLOR);
	DeleteObject(brush);
	SelectObject(hdc, old);
}

void Field::prepareBackground(HDC& hdc)
{
	HPEN pen = CreatePen(PS_COSMETIC, 2, BLACK_PEN);
	HPEN old = (HPEN)SelectObject(hdc, pen);

	for_each(_cells.begin(), _cells.end(), [&hdc](const Cell& cell)
	{
		HBRUSH brush = CreateSolidBrush(cell.getColor());
		HBRUSH o = (HBRUSH)SelectObject(hdc, brush);

		Rectangle(hdc, cell.getRect().left, cell.getRect().top, cell.getRect().right, cell.getRect().bottom);
		DeleteObject(brush);
		SelectObject(hdc, o);
	});

	SelectObject(hdc, old);
	DeleteObject(pen);
}
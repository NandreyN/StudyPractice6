#pragma once
#define DIALOG 101
#define IDT_QTEXT 1001
#define IDG_GROUP 1003

#include <windows.h>
#include "Field.h"
#include <algorithm>
#define CHOSENCOLOR RGB(154,154,154)
#include <string>
#include "Question.h"
using namespace std;

static Question _globalQ;

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

BOOL Field::TextItemDialog(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static HWND Htext, HGroup;
	static Question q;

	switch (message)
	{
	case WM_INITDIALOG:
		q = _globalQ;
		Htext = GetDlgItem(hwnd, IDT_QTEXT);
		SetWindowText(Htext, q.text.data());
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam))
		{
		case IDOK:
			EndDialog(hwnd, 0);
			break;
		case IDCANCEL:
			EndDialog(hwnd, -1);
			break;
		}
		break;
	}
	return FALSE;
}

void Field::handleCellAction(HWND& hwnd, HDC &hdc, int id, Question question)
{
	_globalQ = question;
	int res = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(DIALOG), hwnd, Field::TextItemDialog);
	if (res != -1)
		this->markCellSeen(hdc, id);
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
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, to_string(cell.getNumber() + 1).data(), to_string(cell.getNumber()).size(), &cell.getRect(), DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		DeleteObject(brush);
		SelectObject(hdc, o);
	});

	SelectObject(hdc, old);
	DeleteObject(pen);
}
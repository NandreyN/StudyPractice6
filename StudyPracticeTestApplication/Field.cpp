#pragma once
#define DIALOG 101
#define IDT_QTEXT 1001
#define IDG_GROUP 1003
#define CHECKDIALOG 105
#define RADIODIOALOG 103
#define CHECK0 1010
#define RADIO0 1004
//#define CHECK1 1011
//#define CHECK2 1012
//#define CHECK3 1013
//#define CHECK4 1014
#define CTEXT 1015
#define  RTEXT 1009

#include <windows.h>
#include "Field.h"
#include <algorithm>
#define CHOSENCOLOR RGB(154,154,154)
#include <string>
#include "Question.h"
using namespace std;

static Question _globalQ;
static vector<int> _answers;

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
	static vector<int> answers;
	static int textBoxId, firstBId;
	switch (message)
	{
	case WM_INITDIALOG:
		q = _globalQ;

		switch (q.mode)
		{
		case 'c':
			textBoxId = CTEXT;
			firstBId = CHECK0;
			break;
		case 'r':
			textBoxId = RTEXT;
			firstBId = RADIO0;
		default:
			EndDialog(hwnd, -1);
			throw "Unknown task type";
		}

		if (q.variants.size() > 5)
		{
			EndDialog(hwnd, -1);
			throw "Too many variants";
		}

		int i; i = 0;
		for (; i < q.variants.size(); i++)
		{
			HWND handle = GetDlgItem(hwnd, firstBId + i);
			SetWindowText(handle, q.variants[i].data());
		}

		if (i <= 4)
			for (; i < 5; i++)
				ShowWindow(GetDlgItem(hwnd, firstBId + i), HIDE_WINDOW);

		SetWindowText(GetDlgItem(hwnd, textBoxId), q.text.data());
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam))
		{
		case IDOK:
			// To get marked 
			for (int j = 0; j < q.variants.size(); j++)
			{
				bool isMarked = IsDlgButtonChecked(hwnd, firstBId + j);
				if (isMarked) answers.push_back(j + 1);
			}
			_answers = answers;
			EndDialog(hwnd, 0);
			break;
		case IDCANCEL:
			EndDialog(hwnd, -2);
			break;
		}
		break;
	}
	return FALSE;
}

void Field::handleCellAction(HWND& hwnd, HDC &hdc, int id, Question question)
{
	_globalQ = question;
	int dialogID = (question.mode == 'c') ? CHECKDIALOG : RADIODIOALOG;
	_answers.clear();

	int res = -2;
	try
	{
		res = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(dialogID), hwnd, Field::TextItemDialog);
	}
	catch (char* errMsg)
	{
		MessageBox(NULL, "Error", errMsg, MB_OK);
		return;
	}
	if (res >= 0)
		this->markCellSeen(hdc, id);
	if (question.correct == _answers)
		MessageBox(NULL, "Correct", "Correct", MB_OK);
	if (res == 0)
		MessageBox(NULL, "Info", "Answered", MB_OK);
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
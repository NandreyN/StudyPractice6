#undef UNICODE
#include <windows.h>
#include  <math.h>
#include <vector>
#include "Question.h"
#include "Field.h"
#include "Cell.h"
#include "RParser.h"

using namespace std;

const int questionsCount = 3;
static vector<Question> _questions;

BOOL InitWnd1(HINSTANCE hinstance);
BOOL InitInstance1(HINSTANCE hinstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	if (!InitWnd1(hinstance))
	{
		MessageBox(NULL, "Unable to Init App", "Error", MB_OK);
		return FALSE;
	}

	if (!InitInstance1(hinstance, nCmdShow))
	{
		MessageBox(NULL, "Unable to Init Instance", "Error", MB_OK);
		return FALSE;
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOL InitWnd1(HINSTANCE hinstance)
{
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hinstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_CROSS);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "Test";
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, "Cannot register class", "Error", MB_OK);
		return FALSE;
	}
	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static int x, y;
	static bool isPassed;
	static HDC hdc;
	static Field field;
	PAINTSTRUCT ps;
	static RECT r;
	switch (message)
	{
	case WM_CREATE:
		isPassed = false;
		_questions = RParser::getQuestionsCollection(questionsCount);
		GetClientRect(hwnd, &r);
		x = r.right; y = r.bottom;
		field = Field(x, y, questionsCount);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		field.prepareBackground(hdc);
		EndPaint(hwnd, &ps);;
		break;
	case WM_SIZE:
		GetClientRect(hwnd, &r);
		x = r.right; y = r.bottom;
		field.prepareBackground(hdc);
		break;
	case WM_LBUTTONUP:
	{
		if (isPassed)
		{
			string msg = "You have passed the test, your mark is : " + to_string(field.getMark());
			MessageBox(NULL, msg.data(), "Info", MB_OK);
			return FALSE;
		};

		int k = field.getClickedCellNumber(LOWORD(lparam), HIWORD(lparam));
		field.handleCellAction(hwnd, hdc, k, _questions[k]);
		InvalidateRect(hwnd, NULL, true);
		isPassed = field.isPassed();
		if (isPassed)
		{
			string msg = "You have passed the test, your mark is : " + to_string(field.getMark());
			MessageBox(NULL, msg.data(), "Info", MB_OK);
			return FALSE;
		};
	}
	break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	return FALSE;
}

BOOL InitInstance1(HINSTANCE hinstance, int nCmdShow)
{
	HWND hwnd;
	hwnd = CreateWindow(
		"Test",
		"Test",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		NULL,
		NULL,
		hinstance,
		NULL);


	if (!hwnd)
		return FALSE;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return TRUE;
}
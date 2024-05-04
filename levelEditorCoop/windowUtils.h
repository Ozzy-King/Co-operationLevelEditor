#pragma once
#include <windows.h>

WNDCLASS ozCreateWindowClass(const wchar_t* winClassName, WNDPROC windProc) {
	WNDCLASS windowClass = { 0 };
	windowClass.hInstance = NULL;
	windowClass.lpfnWndProc = windProc;
	windowClass.lpszClassName = winClassName;
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if (!RegisterClass(&windowClass)) {
		MessageBox(NULL, L"Could not register class", L"Error", MB_OK);
	}
	return windowClass;
}

HWND ozCreateWindow(const wchar_t* winClassName, const wchar_t* title, int winPosX, int winPosY, int winWidth, int winHeight) {

	HWND windowHandle = CreateWindowEx(0, winClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		winPosX,
		winPosY,
		winWidth,
		winHeight,
		NULL,
		NULL,
		NULL,
		NULL);
	ShowWindow(windowHandle, SW_RESTORE);

	return windowHandle;
}

HWND ozCreateButton(HWND perantHwnd, const wchar_t* text, int xPos, int yPos, int width, int height, HMENU IDIndent) {
	HWND hwndButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		text,      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		xPos,         // x position 
		yPos,         // y position 
		width,        // Button width
		height,        // Button height
		perantHwnd,     // Parent window
		(HMENU)IDIndent,       // No menu.
		NULL,
		NULL);

	return hwndButton;
}

HWND ozCreateLabel(HWND perantHwnd, const wchar_t* text, int xPos, int yPos, int width, int height, HMENU IDIndent) {
	HWND hwndLabel = CreateWindow(
		L"static",
		text,
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		xPos,         // x position 
		yPos,         // y position 
		width,        // Button width
		height,        // Button height
		perantHwnd,     // Parent window
		(HMENU)IDIndent,       // No menu.
		NULL, 
		NULL);
	return hwndLabel;
}

HWND ozCreateTextBox(HWND perantHwnd, const wchar_t* text, int xPos, int yPos, int width, int height, HMENU IDIndent) {
	HWND hwndTextBox = CreateWindow(
		L"edit",
		text,
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
		xPos,
		yPos,
		width,
		height,
		perantHwnd,
		(HMENU)IDIndent,
		NULL,
		NULL);
	return hwndTextBox;
}

HWND ozCreateListBox(HWND perantHwnd, int xPos, int yPos, int width, int height, HMENU IDIndent) {
	HWND hwndListBox = CreateWindow(
		L"LISTBOX",
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_VSCROLL | LBS_NOTIFY,
		xPos,
		yPos,
		width,
		height,
		perantHwnd,
		(HMENU)IDIndent,
		NULL,
		NULL);
	return hwndListBox;
}

//got to test
//returns index of placement, -1 error occured, -2 insufficient space
int ozAddItemStringListBox(HWND windowHWND, int winID, char* text) {
	wchar_t* newtext = (wchar_t*)malloc((strlen(text) + 1)*sizeof(wchar_t));
	if (newtext == NULL) { return -1; }
	mbstowcs(newtext, text, strlen(text) + 1);

	HWND listBoxHwnd = GetDlgItem(windowHWND, winID);
	LRESULT result = SendMessage(listBoxHwnd, LB_ADDSTRING, NULL, (LPARAM)newtext);
	if (result == LB_ERR) {
		return -1;
	}
	else if ( result == LB_ERRSPACE) {
		return -2;
	}

	free(newtext);
	return result;
}
//remove item for list based on index
int ozRemoveItemListBox(HWND windowHWND, int winID, int index) {
	HWND listBoxHwnd = GetDlgItem(windowHWND, winID);
	LRESULT result = SendMessage(listBoxHwnd, LB_DELETESTRING, index, NULL);
	if (result == LB_ERR) {
		return 1;
	}
	return 0;
}

int ozSetDataItemListBox(HWND windowHWND, int winID, int index, int value) {
	HWND listBoxHwnd = GetDlgItem(windowHWND, winID);
	LRESULT result = SendMessage(listBoxHwnd, LB_SETITEMDATA, index, value);
	if (result == LB_ERR) {
		return 1;
	}
	return 0;
}

//returns what was set by dsetdataitemListBox at index of listbox (-1 fail, else suc)
int ozGetDataItemListBox(HWND windowHWND, int winID, int index) {
	HWND listBoxHwnd = GetDlgItem(windowHWND, winID);
	LRESULT result = SendMessage(listBoxHwnd, LB_GETITEMDATA, index, NULL);
	if (result == LB_ERR) {
		return -1;
	}
	return result;
}

//returns index of currently selected item
int ozGetSelItemIndexListBox(HWND windowHWND, int winID) {
	HWND listBoxHwnd = GetDlgItem(windowHWND, winID);
	LRESULT result = SendMessage(listBoxHwnd, LB_GETCURSEL, NULL, NULL);
	if (result == LB_ERR) {
		return -1;
	}
	return (int)result;
}
int ozSetSelItemIndexListBox(HWND windowHWND, int winID, int index) {
	HWND listBoxHwnd = GetDlgItem(windowHWND, winID);
	LRESULT result = SendMessage(listBoxHwnd, LB_SETCURSEL, index, NULL);
	if (result == LB_ERR) {
		return -1;
	}
	return 0;
}
//populates buffer with string and returns acquired strings length
int ozGetItemStringListBox(HWND windowHWND, int winID, int index, wchar_t* buffer, int bufferlen) {
	HWND listBoxHwnd = GetDlgItem(windowHWND, winID);
	LRESULT strLen = SendMessage(listBoxHwnd, LB_GETTEXTLEN, index, NULL);
	if (strLen == LB_ERR || strLen > bufferlen) { //error ccoured retiveing string len (invalid index, or buffer size is too small)
		return -1;
	}

	LRESULT result = SendMessage(listBoxHwnd, LB_GETTEXT, index, (LPARAM)buffer);
	if (result == LB_ERR) {
		return -1;
	}
	return (int)result;
}



//buffer is an out set up buffer before passing into here
int ozGetWindowText(HWND windowHWND, int winID, wchar_t* buffer, int bufferLen) {
	return GetWindowText(GetDlgItem(windowHWND, winID), buffer, bufferLen);
}
int ozGetWindowText(HWND windowHWND, wchar_t* buffer, int bufferLen) {
	return GetWindowText(windowHWND, buffer, bufferLen);
}
//buffer is an out set up buffer before passing into here
void ozSetWindowText(HWND windowHWND, int winID, const wchar_t* buffer) {
	SetWindowText(GetDlgItem(windowHWND, winID), buffer);
}
void ozSetWindowText(HWND windowHWND, const wchar_t* buffer) {
	SetWindowText(windowHWND, buffer);
}

int ozSetWindowText(HWND windowHWND, const char* text) {
	wchar_t* newtext = (wchar_t*)malloc((strlen(text) + 1) * sizeof(wchar_t));
	if (newtext == NULL) { return -1; }
	mbstowcs(newtext, text, strlen(text) + 1);

	SetWindowText(windowHWND, newtext);
	return 0;
}




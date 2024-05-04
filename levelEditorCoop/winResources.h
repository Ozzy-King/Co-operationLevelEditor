#pragma once
#include <string.h>
#include <cstdlib>
//IDA = An accelerator table resource
//IDB = A bitmap resource
//IDC = A command identifier
//IDD = A dialog box resource
//IDI = An icon resource
//IDM = A menu command identifier
//IDR = Multiple resource types, perhaps those common to an entire application or window
//IDS = A string resource
//ID = An unknown or custom resource

#define mapWidth 24
#define mapHeight 24

#define ObjPerTile 20

//40 max object to pick from
//const wchar_t objectList[][40] = { L"floor", L"brick_wall_e", L"brick_wall_n"};

//10000 possible tiles, 20 different object per tile
char tileList[mapWidth*mapHeight][ObjPerTile]; //will point to different items in objectList

//return -1 if failed else index of placement
int tileListAdd(int tileIndex, int value) {
	for (int i = 0; i < ObjPerTile; i++) {
		if (tileList[tileIndex][i] == -1) {
			tileList[tileIndex][i] = value;
			return i;
		}
	}
	return -1;
}

//need to free your self, returns null if nut successful
wchar_t* charToWchar(char* text) {
	wchar_t* newtext = (wchar_t*)malloc((strlen(text) + 1) * sizeof(wchar_t));
	if (newtext == NULL) { return NULL; }
	mbstowcs(newtext, text, strlen(text) + 1);
	return newtext;
}
char* wcharToChar(wchar_t* text, int textLen) {
	char* newtext = (char*)malloc((textLen + 1) * sizeof(char));
	if (newtext == NULL) { return NULL; }
	wcstombs(newtext, text, textLen + 1);
	return newtext;
}


//-------main map window
#define IDD_filePathTextBox 10012 //title of the file when exported
#define IDD_fileNameTextBox 10001 //title of the file when exported
#define IDD_fileNameObjDefTextBox 10013 //title of the file when exported
//reserve 1 -> 10,000 = all the tile button ids - 
#define	IDC_loadButton 10016
#define	IDC_saveButton 10003
#define IDC_saveDebugButton 10015

#define	IDC_loadObjsDefButton 10004
#define IDC_ObjListButton 10011
#define IDC_newMapButton 10014


//-------map tile window
#define IDS_tileTitleLabel 10005
#define ID_mapTileListBox 10006 //what objects are on the tile
#define IDC_addToTileButton 10007
#define IDC_removeFromTileButton 10008

//-------tile list
#define IDS_objectListTitleLabel 10009
#define ID_tileTypeListBox 10010 //types of object that can be added to tile
//filed with precedual generated list




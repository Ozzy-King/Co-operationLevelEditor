#define OLC_PGE_APPLICATION
//#define TEST 

//if testing build inclue test files
#ifdef TEST
#include "testPrintFunc.h"
#endif // TEST
#include "LogConsole.h"



#include "olcPixelGameEngine.h"
#include "fileUtils.h"
#include "mapUtils.h"

bool newfile = false, open = false, include = false;
std::string openFileName = "";
std::string newFileName = "";

std::vector<std::string> originalIncludes;

int currentItemPos;

int saveMap();

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	bool showItems = false;
	int tileSize = 20;
	Example()
	{
		// Name your application
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		
		// Called once per frame, draws random coloured pixels
		
		//Draw(rand() % ScreenWidth(), rand() % ScreenHeight(), olc::Pixel(rand() % 256, rand() % 256, rand() % 256));

		drawCurrentItem();
		if (GetMouse(olc::Mouse::MIDDLE).bReleased) {
			showItems = !showItems;
		}
		drawMap();

		if (showItems) {
			drawItems();
		}

		int x = (GetMouseX() / tileSize);
		int y = ((GetMouseY() < 21 ? 21 : GetMouseY()) / tileSize);

		//clamp x
		if (x >= 32) { x = 32; }
		if (x < 0) { x = 0; }
		if (y >= 32) { y = 32; }
		if (y < 0) { y = 0; }
		x = x * tileSize;
		y = y * tileSize;
		//std::cout << "x: " << x/tileSize << " - y: " << (y-20)/ tileSize << "\n";

		DrawRectDecal({ (float)x,(float)y }, { (float)tileSize,(float)tileSize }, olc::YELLOW);

		if (GetMouse(olc::Mouse::LEFT).bReleased) {
			int temp = getMapRowCol(objectNamesAtInt[currentItemPos], x / tileSize, (y - 20) / tileSize);
			setMapRowCol(objectNamesAtInt[currentItemPos], x / tileSize, (y-20)/ tileSize, !temp);
		}
		if (GetKey(olc::F1).bReleased) {
			saveMap();
		}

		return true;
	}

	bool drawMap() {
		if (Map.find(objectNamesAtInt[currentItemPos]) == Map.end()) {
			return true;
		}
		for (int i = 0; i < Map[objectNamesAtInt[currentItemPos]].size(); i++) {
			int row = Map[objectNamesAtInt[currentItemPos]][i];
			if (row == 0) { continue; }
			for (int x = 0; x < 32; x++) {
				if (getMapRowCol(objectNamesAtInt[currentItemPos], x, i)) {
					FillRectDecal({ ((float)x * tileSize), ((float)i* tileSize)+21 }, { (float)tileSize,(float)tileSize }, olc::GREEN);
				}
			}
		}
		return true;
	}

	bool drawCurrentItem() {
		//draw current selected object string
		FillRectDecal({ 0,0 }, {(float)ScreenWidth(),21}, olc::RED);
		DrawStringDecal({ 0,1 }, objectNamesAtInt[currentItemPos], olc::WHITE, {2.0,2.0});

		return true;
	}
	bool drawItems() {
		static int oldmouse;

		//move item select up or down
		if (GetMouseWheel() < 0) {
			currentItemPos += 1;
			if (currentItemPos >= objectCount) {
				currentItemPos = objectCount - 1;
			}
		}
		if (GetMouseWheel() > 0) {
			currentItemPos -= 1;
			if (currentItemPos < 0) {
				currentItemPos = 0;
			}
		}

		//if any ley is pressed got to nect object starting wiht that letter
		for (int i = 0; i <= 26; i ++) {
			if (GetKey((olc::Key)i).bPressed) {
				for (int j = currentItemPos+1; j < objectCount; j++) {
					if (objectNamesAtInt[j][0] == (char)(i+0x40) || objectNamesAtInt[j][0] == (char)(i + 0x60)) {
						currentItemPos = j;
						break;
					}
				}
			}
		}

		//draw 10 items after the selected
		float counter = 1;
		int mixCount = (objectCount > currentItemPos + 10 ? currentItemPos + 10 : objectCount);
		for (int i = currentItemPos+1; i < mixCount; i++) {
			DrawStringDecal({ 0,(counter * 21) }, objectNamesAtInt[i], olc::WHITE, {2.0,2.0});
			counter += 1;
		}

		return true;
	}
};
#ifndef TEST
// -o open, -i include, -n new
int main(int argc, char* argv[]) {

	//check argc argv
	if (argc < 2) {//less than two arguments are not allowed
		LOG::error("MAIN", "not enough paramerter entered.\n-n\tnew file\n-o\topen file\n-i\tinclude file");
		return 0;
	}
	for (int i = 1; i < argc; i++) {
		std::cout << i << "\n";
		std::string arg = std::string(argv[i]);
		if (arg == "-n") {
			if (!open) {
				newfile = true;
				if (i != argc-1) {
					newFileName = std::string(argv[i + 1]);
					i = i + 1;
				}
				else {
					LOG::error("MAIN", "need to supplie name with -n (new file)");	return 0;
				}
			} else {
				LOG::error("MAIN", "-n and -o were given");	return 0;
			}
		}
		if (arg == "-o") {
			if (!newfile) {
				open = true;
				if (i != argc - 1) {
					openFileName = std::string(argv[i + 1]);
					if (!fileExist(openFileName)) { LOG::error("MAIN", "file \"" + openFileName + "\" does not exist");	return 0; }
					i = i + 1;
				}
				else {
					LOG::error("MAIN", "need to supplie name with -o (open file)");	return 0;
				}
			}
			else {
				LOG::error("MAIN", "-n and -o were given");	return 0;
			}
		}
		if (arg == "-i") {
			include = true;
			if (i != argc - 1) {
				std::string includeFileName = std::string(argv[i + 1]);
				if (!fileExist(includeFileName)) { LOG::error("MAIN", "file \""+includeFileName+"\" does not exist");	return 0; }
				originalIncludes.push_back(includeFileName);
				i = i + 1;
			}
			else {
				LOG::error("MAIN", "need to supplie name with -o (open file)");	return 0;
			}
		}
	}
	//create new level
	if (newfile) {
		for (int i = 0; i < originalIncludes.size(); i++) {//add the original includes to the include list
			includeListAtInt.insert({ includeCount, originalIncludes[i] });
			includeListToInt.insert({ originalIncludes[i],includeCount });
			includeCount += 1;
		}
		//recursivly read headers headers and headers objects
		for (int i = 0; i < includeCount; i++) {
			readFullIncludeFile(includeListAtInt[i]);
		}
		if (objectCount == 0) { LOG::error("MAIN", "need atleast one object");  return 0; }
		currentItemPos = 0;
	
	}


	//read level if inputed NEED TO FINISH
	if (open) {
		std::ifstream openingFile;
		std::string line = "";
		if (OpenFile(openFileName, openingFile)) {
			LOG::error("MAIN", "could not open file \"" + openFileName + "\"");	return 0;
		}
		//read headers
		readLine(openingFile, line);
		int CountNote = originalIncludes.size();
		if (getIncludeFiles(line)) { LOG::error("MAIN", "error occured while getting includes");  return 0; }//gets the files in level file
		for (int i = 0; i < includeCount; i++) { //adds the leves includes to the original list
			originalIncludes.push_back(includeListAtInt[i]);
		}
		for (int i = 0; i < CountNote; i++) {//add the original includes to the include list
			includeListAtInt.insert({includeCount, originalIncludes[i]});
			includeListToInt.insert({ originalIncludes[i],includeCount });
			includeCount += 1;
		}
		//recursivly read headers headers and headers objects
		for (int i = 0; i < includeCount; i++) {
			readFullIncludeFile(includeListAtInt[i]);
		}

		//read map

		//read 
	}

	

	std::cout << "opened file: " << openFileName << "\n";
	std::cout << "new file: " << newFileName << "\n";
	for (int i = 0; i < originalIncludes.size(); i++) {
		std::cout << "ORIGINAL include: " << originalIncludes[i] << "\n";
	}
	for (int i = 0; i < includeCount; i++) {
		std::cout << "include: " << includeListAtInt[i] << "\n";
	}
	for (int i = 0; i < objectCount; i++) {
		std::cout << "object:\t" << objectNamesAtInt[i] << "\n";
	}

	Example demo;
	if (demo.Construct(520, 320, 1, 1))
		demo.Start();
	return 0;
}
#else
//if testing build run tests instead
int main()
{
	TESTtokenizeString();
	TESTgetIncludeFiles();
	TESTgetObjectsInIncludes();
	TESTgetTileDefinitions();
	TESTgetMapLayout();
}

#endif




int saveMap() {
	char validChar[] = "abcdefghijklmnopqrstuvwxyz123456789";

	std::ofstream theFile(newFileName, std::ios::out);

	//write original includes
	std::string includes = "include: [";
	for (int i = 0; i < originalIncludes.size(); i++) {
		includes += originalIncludes[i] + (i == originalIncludes.size() - 1 ? "]\n" : ",");
	}
	theFile << includes;

	//write fluff
	theFile << "fileProperties:\n";
	theFile << "  creatorName: OLACLevelEditor\n";
	theFile << "sceneName : OriginalWorld\n";
	theFile << "postProcessing :\n";
	theFile << "  depthOfField : { enabled: false, focusDistance : 58, focalLength : 300.0, aperture : 1.0 }\n";

	//write map
	theFile << "grid: |\n";
	std::string row = "  ";
	for (int y = 0; y < 32; y++) {
		row = "  ";
		for (int x = 0; x < 32; x++) {
			row += validChar[y];
			row += validChar[x];
			row += (x == 32 - 1 ? "\n" : ",");
		}
		theFile << row;
	}
	

	//write the tile definitons
	theFile << "gridObjects:\n";
	row = "  ";
	int x = 0;
	int y = 0;
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			row = "  ";
			std::string xytile = "";
			bool firstIter = true;
			xytile+= validChar[y];
			xytile += validChar[x];

			row += xytile;
			row += ": [";

			stringToXY(xytile, x, y);
			for (auto it = Map.begin(); it != Map.end(); ++it) {
				if (getMapRowCol(it->first, x, y)) {
					if (firstIter) {
						firstIter = false;
					}
					else {
						row += ",";
					}
					row += it->first;
				}
			}
			row += "]\n";
			theFile << row;
		}
	}

	theFile << "objectDefinitions:\n";
	theFile << "sounds:\n";
	theFile << "globalData:\n";

	//# override the definitions in LevelsShared for level specific adjustments

		




	return 0;
}
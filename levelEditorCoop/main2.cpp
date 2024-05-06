#define OLC_PGE_APPLICATION
//#define TEST 

//if testing build inclue test files
#ifdef TEST
#include "testPrintFunc.h"
#endif // TEST
#include "LogConsole.h"



#include "olcPixelGameEngine.h"
#include "fileUtils.h"



// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
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
		


		return true;
	}
};
#ifndef TEST
// -o open, -i include
int main(int argc, char* argv[]) {
	bool newfile = false, open = false, include = false;
	//check argc argv
	if (argc < 2) {//less than two arguments are not allowed
		LOG::error("MAIN", "not enough paramerter entered.\n-n\tnew file\n-o\topen file\n-i\tinclude file");
		return 0;
	}
	for (int i = 0; i < argc; i++) {
		if (argv[i] == "-n") {
			if (!open) {
				newfile = true;
			} else {
				LOG::error("MAIN", "-n and -o were given");	return 0;
			}
		}


	}


	Example demo;
	if (demo.Construct(256, 240, 4, 4))
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
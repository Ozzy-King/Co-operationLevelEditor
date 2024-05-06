#define OLC_PGE_APPLICATION
#define TEST 

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
int main()
{
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
}

#endif
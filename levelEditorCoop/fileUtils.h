#pragma once
#include <string>
#include <vector>
#include <map>

//workigns
//include [word.what]
//123456789		    0
//len = len - pos(9) - (len -pos(0))

int objectCount = 0;
std::map<std::string, int> objectNamesToInt;
std::map<int, std::string> objectNamesAtInt;

int includeCount = 0;
std::map<std::string, int> includeListToInt;
std::map<int, std::string> includeListAtInt;

std::vector<std::string> tokenizeString(std::string str, char ch, bool trim = false, char trimCh = ' ') {
	std::vector<std::string> output;
	std::string temp = "";
	int start = 0;
	int end = 0;

	while (end != str.size()) {
		
		end = (str.find(ch, start) != std::string::npos ? str.find(ch, start) : str.size());
		std::string tok = str.substr(start, end - start);
		if (trim) {
			while (tok[0] == trimCh) {
				tok = tok.substr(1);
			}
			while (tok[tok.size()-1] == trimCh) {
				tok.resize(tok.size()-2);
			}
		}
		output.push_back(tok);
		start = end+1;
	}
	return output;
}





int getIncludeFiles(std::string includeLine) {
	//check everything is valid
	size_t BrkOpen = includeLine.find('[');
	if (BrkOpen == std::string::npos) { return 1; }
	size_t BrkClose = includeLine.find(']');
	if (BrkClose == std::string::npos) { return 1; }
	size_t len = includeLine.size();
	if (len == 0) { return 1; }
	if (includeLine.find("include:") == std::string::npos) { return 1; }

	//get file names and split at ',' and trim spaces
	std::string names = includeLine.substr(BrkOpen+1, BrkClose - (BrkOpen + 1));
	std::vector<std::string> includeList = tokenizeString(names, ',', true);

	for (int i = 0; i < includeList.size(); i++) {
		if (includeListToInt.find(includeList[i]) != includeListToInt.end()) { continue; }
		includeListToInt.insert({ includeList[i], includeCount });
		includeListAtInt.insert({ includeCount, includeList[i] });
		includeCount++;
	}

	return 0;
}

int getObjectsInIncludes(std::vector<std::string> objectSelection) {
	if (objectSelection[0] != "objectDefinitions:") { return 1; }
	int colonPos;
	std::string getObj;
	for (int i = 1; i < objectSelection.size(); i++) {
		//isolate the name
		
		colonPos = objectSelection[i].find(':'); //get colon position
		if (colonPos == std::string::npos) { return 1; } //check colon has been found

		if (objectSelection[i][1] != ' ') { return 1; }//invalid as less than 2 spaces
		getObj = objectSelection[i].substr(2, colonPos-2);
		if (getObj[0] == ' ') { return 1; }//invalid as more than 2 spaces

		if (objectNamesToInt.find(getObj) != objectNamesToInt.end()) { continue; }
		
		objectNamesToInt.insert({getObj, objectCount});
		objectNamesAtInt.insert({ objectCount,getObj });
		objectCount++;
	}


	return 0;
}

//if testing build inclue test files
#ifdef TEST
	#include "fileUtilsTests.h"
#endif // TEST




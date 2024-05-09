#pragma once
#include <string>
#include <vector>
#include <map>

#include <fstream>


//file io fuinctions
int fileExist(std::string fileName) {
	std::ifstream file(fileName.c_str());
	if (file.is_open()) {
		file.close();
		return 1;
	}
	return 0;
}
int OpenFile(std::string fileName, std::ifstream& out) {
	out = std::ifstream(fileName.c_str());
	if (out.is_open()) {
		return 0;
	}
	return 1;
}
int readLine(std::ifstream& in, std::string& out) {
	if (std::getline(in, out)) {
		return 0;
	}
	return 1;
}

//workigns
//include [word.what]
//123456789		    0
//len = len - pos(9) - (len -pos(0))

//stores list of objs found as both: int	  -> obj name
								//   obj name -> int
int objectCount = 0;
std::map<std::string, int> objectNamesToInt;
std::map<int, std::string> objectNamesAtInt;

//stores the a list of includes as both: int	 -> include name
									//   include -> int
int includeCount = 0;
std::map<std::string, int> includeListToInt;
std::map<int, std::string> includeListAtInt;

std::map<std::string, std::vector<std::string>> tilesWithObjs; //stores the tile definition name along with a vector list of the object stored

std::vector<std::vector<std::string>> map;

//splitting string into tokens based on 
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
				tok.resize(tok.size()-1);
			}
		}
		if (tok != "") {//make sure theres something to push
			output.push_back(tok);
		}
		start = end+1;
	}
	return output;
}


int getIncludeFiles(std::string includeLine) {
	//check everything is valid
	size_t len = includeLine.size();
	if (len == 0) { LOG::error("getTileDefinition", "supplied empty line"); return 1; }
	if (includeLine.find("include:") == std::string::npos) { LOG::error("getTileDefinition", "supplied invalid line"); return 1; }

	size_t BrkOpen = includeLine.find('[');
	if (BrkOpen == std::string::npos) { LOG::error("getTileDefinition", "cant find open brack"); return 1; }
	size_t BrkClose = includeLine.find(']');
	if (BrkClose == std::string::npos) { LOG::error("getTileDefinition", "cant find close brack"); return 1; }

	//get file names and split at ',' and trim spaces
	std::string names = includeLine.substr(BrkOpen+1, BrkClose - (BrkOpen + 1));
	std::vector<std::string> includeList = tokenizeString(names, ',', true);

	for (int i = 0; i < includeList.size(); i++) {
		if (includeListToInt.find(includeList[i]) != includeListToInt.end()) { LOG::log("getTileDefinition", "\""+includeList[i] + "\"" + " already in list"); continue; }
		includeListToInt.insert({ includeList[i], includeCount });
		includeListAtInt.insert({ includeCount, includeList[i] });
		includeCount++;
	}

	return 0;
}

int getObjectsInIncludes(std::vector<std::string> objectSelection) {
	if (objectSelection.size() == 0) { LOG::error("getObjectsInIncludes", "supplied empty list"); return 1; }
	if (objectSelection[0] != "objectDefinitions:") { LOG::error("getObjectsInIncludes", "supplied invalid list"); return 1; }
	int colonPos;
	std::string getObj;
	for (int i = 1; i < objectSelection.size(); i++) {
		//isolate the name
		
		colonPos = objectSelection[i].find(':'); //get colon position
		if (colonPos == std::string::npos) { LOG::error("getObjectsInIncludes", "cant find colon on line: " + std::to_string(i)); continue; } //check colon has been found

		if (objectSelection[i][1] != ' ') { LOG::error("getObjectsInIncludes", "not enough spaces on line: " + std::to_string(i)); return 1; }//invalid as less than 2 spaces
		getObj = objectSelection[i].substr(2, colonPos-2);
		if (getObj[0] == '#') { LOG::log("getObjectsInIncludes", "comment on line:" + std::to_string(i)); continue; }//invalid as more than 2 spaces
		if (getObj[0] == ' ') { LOG::log("getObjectsInIncludes", "too many spaces on line: " + std::to_string(i)); continue; }//invalid as more than 2 spaces

		if (objectNamesToInt.find(getObj) != objectNamesToInt.end()) { 
			LOG::log("getObjectsInIncludes", "\""+getObj +"\"" + " already added"); 
			continue;
		}
		
		objectNamesToInt.insert({getObj, objectCount});
		objectNamesAtInt.insert({ objectCount,getObj });
		objectCount++;
	}


	return 0;
}

//need to finish. just finished getting thet itesm string - need to split and append to place
int getTileDefinitions(std::vector<std::string> tileDefinition) {
	if (tileDefinition.size() == 0) { LOG::error("getTileDefinition", "supplied empty list"); return 1; }
	if (tileDefinition[0] != "gridObjects:") { LOG::error("getTileDefinition", "supplied invalid list"); return 1;  }
	
	for (int i = 1; i < tileDefinition.size(); i++) {
		//split to name and objs
		std::vector<std::string> nameItems = tokenizeString(tileDefinition[i], ':');
		if (nameItems.size() < 2) { LOG::error("getTileDefinitions","invalid on line: " + i); return 1; }//isnt formated properly

		//------------------------for getting the tile defintion name
		//isolate name with out spaces// also check string
		if (nameItems[0].size() < 3) { LOG::error("getTileDefinitions", "invalid line: " + std::to_string(i)); return 1; }
		if (nameItems[0][1] != ' ') { LOG::error("getTileDefinitions", "not enough spaces on line: " + std::to_string(i)); return 1; }//invalid as less than 2 spaces
		nameItems[0] = nameItems[0].substr(2);
		if (nameItems[0][1] == ' ') { LOG::error("getTileDefinitions", "too many spaces on line: " + std::to_string(i)); return 1; }//invalid as more than 2 spaces

		//-----------------------for formatting the objects, spljting them up and 
		//get all the objects inside
		if (nameItems[1].size() < 1) { LOG::error("getTileDefinitions", "invalid line: " + std::to_string(i)); return 1; }
		if (nameItems[1].substr(0, 2) != " [") { LOG::error("getTileDefinitions", "line: " + std::to_string(i) + " formatted wrong"); return 1; } //if beigning isnt correct
		if (nameItems[1][nameItems[1].size() - 1] == '\n') { nameItems[1] = nameItems[1].substr(0, nameItems[1].size() - 1); } //remove trailing newline if needed
		if (nameItems[1][nameItems[1].size()-1] != ']') { LOG::error("getTileDefinitions", "line: " + std::to_string(i) + " formatted wrong"); return 1; } //if end isnt correct
		//skip unnessaray charaters
		nameItems[1] = nameItems[1].substr(2); //remove [
		nameItems[1] = nameItems[1].substr(0, nameItems[1].size()-1); //remove ]

		//split objects and place in maps
		std::vector<std::string> objsInTile = tokenizeString(nameItems[1], ',', true);
		if (tilesWithObjs.find(nameItems[0]) != tilesWithObjs.end()) { LOG::log("getTileDefinitions", nameItems[0] + " already defined on line: " + std::to_string(i)); return 1; }
		tilesWithObjs.insert({nameItems[0], objsInTile});
	}
	return 0;

}

int getMapLayout(std::vector<std::string> mapDefinition) {
	if (mapDefinition.size() == 0) { LOG::error("getObjectsInIncludes", "supplied empty map"); return 1; }
	if (mapDefinition[0] != "grid: |") { LOG::error("getMapLayout","supplied invalid map"); return 1; }

	for (int i = 1; i < mapDefinition.size(); i++) {
		if (mapDefinition[i].size() < 3) { LOG::error("getMapLayout", "invalid line: " + std::to_string(i)); return 1; }

		if (mapDefinition[i][1] != ' ') { LOG::error("getMapLayout", "not enough spaces on line: " + std::to_string(i)); return 1; }
		mapDefinition[i] = mapDefinition[i].substr(2); //trim spaces
		if (mapDefinition[i][mapDefinition.size() - 1] == '\n') { mapDefinition[i] = mapDefinition[i].substr(0, mapDefinition.size()-1); }
		if (mapDefinition[i][0] == ' ') { LOG::error("getMapLayout", "too many spaces on line: " + std::to_string(i)); return 1; }
		std::vector<std::string> mapLine = tokenizeString(mapDefinition[i], ',',true);
		map.push_back(mapLine);
	}
	return 0;
}

int readFullIncludeFile(std::string fileName) {
	//read includes
	std::ifstream openingFile;
	std::string line = "";
	if (OpenFile(fileName, openingFile)) {
		LOG::error("MAIN", "could not open include file \"" + fileName + "\"");	return 0;
	}
	//read include
	readLine(openingFile, line);
	if (getIncludeFiles(line)) { LOG::log("readFullIncludeFile", "error occured while getting includes"); }//gets the files in level file
	std::vector<std::string> objects;
	while (line != "objectDefinitions:") {
		if (readLine(openingFile, line)) { LOG::log("readFullIncludeFile", "could not find any objects in file \"" + fileName +"\"");  return 0; }
	}
	objects.push_back(line);
	while (!readLine(openingFile, line)) {
		if (line[0] == '\n' || line == "" || line[0] == '#') { continue; }
		if (line[0] != ' ') { break; }
		objects.push_back(line);
	}
	getObjectsInIncludes(objects);
	openingFile.close();
	return 0;
}



//if testing build inclue test files
#ifdef TEST
	#include "fileUtilsTests.h"
#endif // TEST




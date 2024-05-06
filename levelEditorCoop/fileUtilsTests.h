#pragma once


void TESTtokenizeString() {
	int error = 0;
	int totalTests = 0;
	std::cout << "testing tokenizeString\n";
	std::string testInput = "test1, test2, test3";
	
	//check that empty string arnt included
	std::string testInput2 = "test1,";
	std::vector<std::string> output2 = tokenizeString(testInput2, ',');
	
	totalTests += 1;
	error += TESTassert<int>(output2.size(), 1);
	
	//dont trim the spaces
	std::vector<std::string> testOutputNoTrim = { "test1"," test2"," test3" };
	std::vector<std::string> outputNoTrim = tokenizeString(testInput, ',');
	
	//trim the spaces
	std::vector<std::string> testOutputTrim = { "test1","test2","test3" };
	std::vector<std::string> outputTrim = tokenizeString(testInput, ',', true);


	for (int i = 0; i < testOutputNoTrim.size(); i++) {
		totalTests += 2;
		error += TESTassert<std::string>(outputTrim[i], testOutputTrim[i]);
		error += TESTassert<std::string>(outputNoTrim[i], testOutputNoTrim[i]);
	}
	TESTreportError(totalTests, error);
}

void TESTgetIncludeFiles() {
	int error = 0;
	int totalTests = 4;
	std::cout << "testing getIncludeFiles\n";

	std::vector<std::string> IncludeContained = {"levelsShared.yaml", "test.yaml", "test1.yaml"};
	std::vector<std::string> ShouldntContain = { "levels.yaml", "t.yaml" };

	std::string include1 = "include: [levelsShared.yaml, test.yaml]";
	std::string include2 = "include: [levelsShared.yaml,test1.yaml]";

	std::string include3 = "include: [levelsShared.yaml, t.yaml";
	std::string include4 = "include: levels.yaml, test.yaml]";

	int gotInclude1 = getIncludeFiles(include1);
	int gotInclude2 = getIncludeFiles(include2);
	int gotInclude3 = getIncludeFiles(include3);
	int gotInclude4 = getIncludeFiles(include4);
	//test if successfull
	error += TESTassert<int>(gotInclude1, 0);
	error += TESTassert<int>(gotInclude2, 0);
	error += TESTassert<int>(gotInclude3, 1);
	error += TESTassert<int>(gotInclude4, 1);

	//test if output the right thing
	for (int i = 0; i < IncludeContained.size(); i++) {
		totalTests += 1;
		error += TESTassert<bool>(includeListToInt.find(IncludeContained[i]) != includeListToInt.end(), true);
	}
	//should contain
	for (int i = 0; i < ShouldntContain.size(); i++) {
		totalTests += 1;
		error += TESTassert<bool>(includeListToInt.find(ShouldntContain[i]) == includeListToInt.end(), true);
	}


	TESTreportError(totalTests, error);
}

void TESTgetObjectsInIncludes() {
	int error = 0;
	int totalTests = 6;
	std::cout << "testing getObjectsInIncludes\n";

	std::vector<std::string> exspectedEndVals = { "obj1","obj2","obj3","blockmove","blockthrow","blockboth" };
	std::vector<std::string> shouldntContain = { "objectDefinitions", "noColon", "taht"};

	std::vector<std::string> objectsValid1 = {"objectDefinitions:","  obj1:","  obj2:","  obj3:","  obj3:"};
	std::vector<std::string> objectsMoreSpace2 = { "objectDefinitions:","  obj1:","  obj2:","  obj3:","  obj3:", "   3space:" };
	std::vector<std::string> objectsLessSpace3 = { "objectDefinitions:","  obj1:","  obj2:","  obj3:","  obj3:"," taht:" };
	std::vector<std::string> objectsNoColon4 = { "objectDefinitions:","  obj1:","  obj2:","  obj3:","  obj3:", "  noColon" };
	std::vector<std::string> objectsNoID5 = { "  obj1:","  obj2:","  obj3:","  obj3:" };
	std::vector<std::string> objectsfillTest6 = { "objectDefinitions:","  blockmove: { mapObject: BlankStatic, tags: [ \"blocksMove\" ] }","  blockboth: { mapObject: BlankStatic, tags: [ \"blocksThrow\", \"blocksMove\" ] }","  blockthrow: { mapObject: BlankStatic, tags: [\"blocksThrow\" ] }" };

	int suc1 = getObjectsInIncludes(objectsValid1);
	int suc2 = getObjectsInIncludes(objectsMoreSpace2);
	int suc3 = getObjectsInIncludes(objectsLessSpace3);
	int suc4 = getObjectsInIncludes(objectsNoColon4);
	int suc5 = getObjectsInIncludes(objectsNoID5);
	int suc6 = getObjectsInIncludes(objectsfillTest6);

	error += TESTassert<int>(suc1, 1);
	error += TESTassert<int>(suc2, 1);
	error += TESTassert<int>(suc3, 1);
	error += TESTassert<int>(suc4, 1);
	error += TESTassert<int>(suc5, 1);	
	error += TESTassert<int>(suc6, 0);

	error += TESTassert<int>(objectCount, (int)exspectedEndVals.size());

	//should contain
	for (int i = 0; i < exspectedEndVals.size(); i++) {
		totalTests += 1;
		error += TESTassert<bool>(objectNamesToInt.find(exspectedEndVals[i]) != objectNamesToInt.end(), true);
	}
	//shouldnt contain
	for (int i = 0; i < shouldntContain.size(); i++) {
		totalTests += 1;
		error += TESTassert<bool>(objectNamesToInt.find(shouldntContain[i]) == objectNamesToInt.end(), true);
	}

	TESTreportError(totalTests, error);
}

void TESTgetTileDefinitions() {
	int error = 0;
	int totalTests = 1;
	std::cout << "testing getTileDefinitions\n";

	std::vector<std::string> input = { "gridObjects:", "  AA: [ floor, te ]","  BB: [ floor ,te ]","  CC: [floor,te]","  DD: [ floor,te]","  EE: [floor,te]"};
	std::vector<std::string> input2 = { "gridObjects:", "  AA: [ floor,te ]","  BB: [ floor,te ]","  CC: [floor,te]","  DD: [ floor,te]","  EE: [floor,te]", " EE: [floor]" };
	std::vector<std::string> hasTiles = { "AA","BB","CC","DD","EE" };
	std::vector<std::string> hasObj = {"floor","te"};

	error += TESTassert<int>(getTileDefinitions(input), 0); //shoudl return 1
	error += TESTassert<int>(getTileDefinitions(input2), 1); //shoudl return 1

	//other checks
	for (int i = 0; i < hasTiles.size(); i++) {
		totalTests += 2;
		if (tilesWithObjs.find(hasTiles[i]) != tilesWithObjs.end()) {
			for (int j = 0; j < 2; j++) {
				error += TESTassert<std::string>(tilesWithObjs[hasTiles[i]][j], hasObj[j]);
			}
		}
		else {
			error += 2;
		}
	
	}

	TESTreportError(totalTests, error);
}
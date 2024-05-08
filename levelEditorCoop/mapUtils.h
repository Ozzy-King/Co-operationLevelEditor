#pragma once
#include <map>
#include <string>

//key eqauls object --> arry y index = h - int bit x = width
std::map<std::string, std::vector<int>> Map;
//layout
//aa ab ac ad --> 1111
//ba bb bc bd     1111
//ca cb cc cd     1111
//da db dc dd     1111
//
//


int getMapRowCol(std::string obj, int x, int y) {
	if (x >= 32 || y >= 32 || x < 0 || y < 0) {
		LOG::error("getMapRowCol","entered invalid coords x: " + std::to_string(x) + " - y: " + std::to_string(y));
		return 0;
	}
	if (Map.find(obj) == Map.end()) {
		return 0;
	}

	int mask = 1 << x;
	return Map[obj][y] & mask;
}

void setMapRowCol(std::string obj, int x, int y, int val) {
	if (x >= 32 || y >= 32 || x < 0 || y < 0) {
		LOG::error("getMapRowCol", "entered invalid coords x: " + std::to_string(x) + " - y: " + std::to_string(y));
		return;
	}
	if (Map.find(obj) == Map.end()) {
		Map.insert({ obj,{} });
		for (int i = 0; i < 32; i++) {
			Map[obj].push_back(0);
		}
	}

	int oval = Map[obj][y] & (1 << x);
	if (oval == val) { return; }
	if (oval == 0) {
		Map[obj][y] = Map[obj][y] | (1 << x);
	}
	else {
		Map[obj][y] = Map[obj][y] & (~(1 << x));
	}
}

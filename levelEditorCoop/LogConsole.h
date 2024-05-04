#pragma once
#include <iostream>

namespace LOG {
	int error(std::string source, std::string msg) {
		std::cout << "\033[31m" << source << " >>> " << msg << "\033[37m\n";
		return 0;
	}

	void log(std::string source, std::string msg) {
		std::cout << "\033[37m" << source << " >>> " << msg << "\n";
	}
}
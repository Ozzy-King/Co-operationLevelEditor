#pragma once
#include <iostream>

template<typename T>
int TESTassert(T got, T exspected) {
	if (got != exspected) {
		std::cout << "\033[31m" << "\texcepted: " << exspected << " -- Got: " << got << "\033[37m\n";
		return 1;
	}
	return 0;
}

void TESTreportError(int totalTests, int errors) {
	std::cout << "\033[34m" << "Total Tests:" << totalTests << " -- Errors: " << errors << "\033[37m\n";
}

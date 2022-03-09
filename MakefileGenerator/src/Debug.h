#ifdef _DEBUG
	#pragma once
	#include <iostream>
	#define DEBUG_PRINT(os)\
			std::cout << os;
	#define DEBUG_PRINT_NL(os)\
		std::cout << os << '\n';
#else
	#define DEBUG_PRINT(os)
	#define DEBUG_PRINT_NL(os)
#endif
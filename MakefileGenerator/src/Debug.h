#ifdef _DEBUG
	#pragma once
	#include <iostream>
	// the usage of flush and endl is intentional in order
	// to flush the output buffer and print the result at execution
	#define DEBUG_PRINT(os)\
			std::cout << os << std::flush;
	#define DEBUG_PRINT_NL(os)\
			std::cout << os << std::endl;
#else
	#define DEBUG_PRINT(os)
	#define DEBUG_PRINT_NL(os)
#endif
#ifdef _DEBUG
	#pragma once
	#include <iostream>
	// the usage of flush and endl is intentionall in order
	// to flush the output buffer and print the result at time
	#define DEBUG_PRINT(os)\
			std::cout << os << std::flush;
	#define DEBUG_PRINT_NL(os)\
			std::cout << os << std::endl;
#else
	#define DEBUG_PRINT(os)
	#define DEBUG_PRINT_NL(os)
#endif
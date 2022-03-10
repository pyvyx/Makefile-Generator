#include <exception>
#include "Debug.h"

#include "GUI/Application.h"

int main()
{
	// replace by returning bool
	try
	{
		App::StartApplication();
	}
	catch (const std::exception& ex)
	{
		DEBUG_PRINT_NL("Exception reason: " << ex.what())
		std::cin.get();
	}
}
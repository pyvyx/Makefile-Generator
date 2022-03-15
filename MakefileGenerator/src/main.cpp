#include <filesystem>

#include "Application.h"
#include "InputHandler.h"

void PrintHelpMessage(const char* fileName) {

	std::cout << "Usage: " << fileName << " <options>\n";
	std::cout << "       -g or -gui = start in GUI mode\n";
	std::cout << "       <filename> = load this config file\n";
	std::cout << "If you're providing a file name without using the GUI mode\n";
	std::cout << "the makefile will immediately be created\n";
}

int main(int argc, char** argv)
{
	bool usingGuiMode = true;
	std::string filePath;

	InputHandler ip(argc, argv);
	ip.lowerAll();
	if (ip.contains("-h")) {
		PrintHelpMessage(argv[0]);
		return 0;
	}
	if (ip.size() > 1)
	{
		if(!ip.contains("-g") && !ip.contains("-gui"))
			usingGuiMode = false;
		if (ip.size() > 2) {
			int location = ip.location("-g");
			if (location == -1)
				location = ip.location("-gui");
			if (location == 1) {
				filePath = ip[2];
			} else {
				filePath = ip[1];
			}
		} else if (!usingGuiMode) {
			filePath = ip[1];
		}
	}

	if (!App::StartApplication(usingGuiMode, filePath))
		return 1;
	return 0;
}
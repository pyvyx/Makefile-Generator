#include "Application.h"
#include "InputHandler.h"

#define EXIT_SUCCESS     0
#define EXIT_APP_ERROR   1
#define EXIT_INPUT_ERROR 2

int main(int argc, char** argv)
{
	Input ip = HandleInput(argc, argv);
	if (ip.error)
		return EXIT_INPUT_ERROR;

	if (!App::StartApplication(ip.guiMode, ip.filePath)) {
		std::cout << "[Error] Trying to start the application" << std::endl;
		return EXIT_APP_ERROR;
	}
	return EXIT_SUCCESS;
}
#include "Application.h"
#include "InputHandler.h"


int main(int argc, char** argv)
{
	Input ip = HandleInput(argc, argv);
	if (ip.error)
		return 1;

	if (!App::StartApplication(ip.guiMode, ip.filePath))
		return 1;
	return 0;
}
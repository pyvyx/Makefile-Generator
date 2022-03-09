#pragma once
#include <string>

#define MULTIPLE_FILE_DIALOG 100
#define FOLDER_SELECT_DIALOG 101

namespace FileDialog {

	bool handleFileDialog(char dialog, std::string* base_str, std::string* str, bool single_str = false);
	std::string SaveFileDialog();
	std::string FileSelectionDialog();
}
#pragma once
#include <string>


namespace FileDialog {

	bool MultiFileDialog(std::string* base_str);
	bool FolderSelectionDialog(std::string* base_str, std::string* str, bool single_str);
	std::string SaveFileDialog();
	std::string FileSelectionDialog();

}
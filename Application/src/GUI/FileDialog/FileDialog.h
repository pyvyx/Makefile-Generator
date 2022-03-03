#pragma once

#define MULTIPLE_FILE_DIALOG 100
#define FOLDER_SELECT_DIALOG 101

namespace FileDialog {

	void multiFileDialog();
	void folderSelectionDialog();
	void handleFileDialog(char dialog);

}
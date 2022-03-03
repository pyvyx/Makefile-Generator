#pragma once

#include <string>
#include <vector>

namespace FH {

#define ID_NOT_DELETED -1
#define ID_ALL_DELETED -10

	class FileEntry
	{
	private:
		static int sm_GlobalID;
		int m_ID;
		int m_DeletedID;
		bool m_Deleted;
		bool m_Shown;
		bool m_Selected;
		std::string m_FileName;
	public:
		FileEntry();
		FileEntry(const std::string& str);
		void setFileName(const std::string& str);
		void setDelete(bool toDelete, int id = ID_NOT_DELETED);
		void setSelected(bool selected);
		void setShown(bool shown);

		bool& getSelectedRef();
		bool isSelected();
		bool isShown();
		bool isDeleted();
		int ID();
		int deletedID();
		std::string& fileName();
	};

	using FileEntryVec = std::vector<FileEntry>;
	FileEntryVec* getFileEntriesPtr();

	void undoLastDelete(bool selectAllChecked);
	void deleteSelectedEntries();
	void setSelectAllEntries(bool state);
	void addEntry(const std::string& str);
	void filterFileEntries(std::string str);
}



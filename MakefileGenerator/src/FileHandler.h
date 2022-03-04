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
		std::string m_FileExtension;
	public:
		FileEntry();
		FileEntry(const std::string& str, const std::string& ext);
		void setFileName(const std::string& str);
		void setDelete(bool toDelete, int id = ID_NOT_DELETED);
		void setSelected(bool selected);
		void setShown(bool shown);
		void setFileExtension(const std::string& str);

		bool& getSelectedRef();
		bool isSelected();
		bool isShown();
		bool isDeleted();
		int ID();
		int deletedID();
		std::string& fileName();
		std::string& extension();
	};

	using FileEntryVec = std::vector<FileEntry>;
	FileEntryVec& getFileEntriesRef();

	void undoLastDelete(bool selectAllChecked);
	void deleteSelectedEntries();
	void setSelectAllEntries(bool state);
	void addEntry(const std::string& str);
	void filterFileEntries(std::string str);
	std::string getRelativePath(const std::string& base_str, const std::string& p);
}



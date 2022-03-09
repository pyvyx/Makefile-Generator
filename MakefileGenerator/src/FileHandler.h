#pragma once

#include <string>
#include <vector>

namespace FH {

#define ID_NOT_DELETED -1

	class FileEntry
	{
	private:
		int m_DeletedID = ID_NOT_DELETED;
		bool m_Deleted	= false;
		bool m_Shown	= true;
		bool m_Selected = false;
		std::string m_FileName;
		std::string m_FileExtension;
	public:
		FileEntry() = default;
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
		int deletedID();
		std::string& fileName();
		std::string& extension();
	};

	using FileEntryVec = std::vector<FileEntry>;
	FileEntryVec& GetFileEntriesRef();

	void UndoLastDelete(bool selectAllChecked);
	void DeleteSelectedEntries();
	void SetSelectAllEntries(bool state);
	void AddEntry(const std::string& str);
	void FilterFileEntries(std::string str);
	std::string GetRelativePath(const std::string* const base_str, const char* const relative_str);
}



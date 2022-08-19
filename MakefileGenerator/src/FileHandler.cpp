#include <iostream>
#include <algorithm>
#include <filesystem>
#include <unordered_set>

#include "FileHandler.h"
#include "Generator.h"

namespace FH {

	FileEntry::FileEntry(const std::string& str, const std::string& ext)
		:  m_FileName(str), m_FileExtension(ext)
		{}

	void FileEntry::setFileName(const std::string& str) { m_FileName = str; }
	void FileEntry::setDelete(bool toDelete, int id)        
	{
		if (toDelete)
		{
			if (id == ID_NOT_DELETED)
				return;
			m_DeletedID = id;
			m_Deleted = true;
			setSelected(false);
		}
		else
		{
			m_DeletedID = ID_NOT_DELETED;
			m_Deleted = false;
		}
	}
	void FileEntry::setSelected(bool selected)	{ m_Selected  = selected; }
	void FileEntry::setShown(bool shown)		{ m_Shown     = shown;    }
	void FileEntry::setFileExtension(const std::string& str) { m_FileExtension = str; }

	bool& FileEntry::getSelectedRef()			{ return m_Selected;      }
	bool FileEntry::isSelected() const			{ return m_Selected;      }
	bool FileEntry::isShown()    const			{ return m_Shown;         }
	bool FileEntry::isDeleted()	 const			{ return m_Deleted;       }
	int  FileEntry::deletedID()	 const			{ return m_DeletedID;     }
	std::string FileEntry::fileName()  const	{ return m_FileName;      }
	std::string FileEntry::extension() const	{ return m_FileExtension; }


	FileEntryVec& GetFileEntriesRef()
	{
		static FileEntryVec fileEntries;
		return fileEntries;
	}


	int GetDeletionID(bool getLastID)
	{
		static int s_DeletionID = 0;
		if(!getLastID)
			return s_DeletionID++;

		if (s_DeletionID == 0)
			return 0;
		return --s_DeletionID;
	}


	void UndoLastDelete(bool selectAllChecked)
	{
		int lastDeletionID = GetDeletionID(true);
		FileEntryVec& fileEntries = GetFileEntriesRef();

		for (size_t i = 0; i < fileEntries.size(); ++i)
		{
			if (fileEntries[i].deletedID() == lastDeletionID)
			{
				fileEntries[i].setDelete(false);
				fileEntries[i].setSelected(selectAllChecked);
			}
		}
	}


	void DeleteSelectedEntries()
	{
		int newDeleteId = GetDeletionID(false);
		FileEntryVec& fileEntries = GetFileEntriesRef();

		for (size_t i = 0; i < fileEntries.size(); ++i)
			if (fileEntries[i].isSelected())
				fileEntries[i].setDelete(true, newDeleteId);
	}


	void SetSelectAllEntries(bool state)
	{
		FileEntryVec& fileEntries = GetFileEntriesRef();
		for (size_t i = 0; i < fileEntries.size(); ++i)
			if(!fileEntries[i].isDeleted())
				fileEntries[i].setSelected(state);
	}


	void AddEntry(const std::string& str)
	{
		FileEntryVec& fileEntries = GetFileEntriesRef();
		for (size_t i = 0; i < fileEntries.size(); ++i)
		{
			//if (fileEntries[i].fileName().find(str) != std::string::npos)
			if (fileEntries[i].fileName() == str)
			{
				if (fileEntries[i].isDeleted())
				{
					fileEntries[i].setDelete(false);
					fileEntries[i].setSelected(false);
				}
				return;
			}
		}

		auto lowerCaseString = [=](std::string str) {
			std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
			return str;
		};

		std::string extension = std::filesystem::path(str).extension().generic_string();
		std::string lowerCased = lowerCaseString(extension);
		if (lowerCased == ".cpp" || lowerCased == ".cc" || lowerCased == ".cxx" || lowerCased == ".c++" || lowerCased == ".cp" || extension == ".C" || lowerCased == ".c" || lowerCased == ".res")
			fileEntries.push_back(FileEntry(str, extension));
	}


	void FilterFileEntries(std::string str)
	{
		FileEntryVec& fileEntries = GetFileEntriesRef();
		// lowercase string
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char c) { return std::tolower(c); });

		for (size_t i = 0; i < fileEntries.size(); ++i)
		{
			std::string fileNameLowerCased = fileEntries[i].fileName();
			// lowercase filename
			std::transform(fileNameLowerCased.begin(), fileNameLowerCased.end(), fileNameLowerCased.begin(),
				[](unsigned char c) { return std::tolower(c); });

			// didn't find
			if (fileNameLowerCased.find(str) == std::string::npos)
			{
				fileEntries[i].setShown(false);
				fileEntries[i].setSelected(false);
			}
			else
				fileEntries[i].setShown(true);
		}
	}


	std::string GetHardDrive(const std::string& str) {
		std::vector<std::string> filePath = MG::SplitStringByChar(str, ':');
		if (filePath.size() == 0)
			return "";
		return filePath[0];
	}


	bool HardDrivesDontMatch(const std::string* const base_str, const char* const relative_str) {
		#ifdef WINDOWS
			if (GetHardDrive(*base_str) != GetHardDrive(relative_str))
				return true;
		#endif
		return false;
	}


	std::string GetRelativePath(const std::string* const base_str, const char* const relative_str)
	{
		if (HardDrivesDontMatch(base_str, relative_str))
			return relative_str;

		std::filesystem::path base(*base_str);
		std::filesystem::path relative(relative_str);
		return std::filesystem::relative(relative, base).generic_string();
	}
}
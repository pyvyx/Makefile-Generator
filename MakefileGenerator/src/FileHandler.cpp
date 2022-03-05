#include <iostream>
#include <algorithm>
#include <filesystem>
#include <unordered_set>

#include "FileHandler.h"

namespace FH {

	static std::vector<FileEntry> sg_FileEntries;

	FileEntry::FileEntry()
		: m_ID(sm_GlobalID), m_DeletedID(ID_NOT_DELETED), m_Deleted(false), m_Shown(true), m_Selected(false)
	{
		++sm_GlobalID;
	}

	FileEntry::FileEntry(const std::string& str, const std::string& ext)
		: m_ID(sm_GlobalID), m_DeletedID(ID_NOT_DELETED), m_Deleted(false), m_Shown(true), m_Selected(false), m_FileName(str), m_FileExtension(ext)
	{
		++sm_GlobalID;
	}
	int FileEntry::sm_GlobalID = 0;

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
	bool FileEntry::isSelected()				{ return m_Selected;      }
	bool FileEntry::isShown()					{ return m_Shown;         }
	bool FileEntry::isDeleted()					{ return m_Deleted;       }
	int  FileEntry::ID()						{ return m_ID;		      }
	int  FileEntry::deletedID()					{ return m_DeletedID;     }
	std::string& FileEntry::fileName()			{ return m_FileName;      }
	std::string& FileEntry::extension()			{ return m_FileExtension; }


	FileEntryVec& getFileEntriesRef()
	{
		return sg_FileEntries;
	}


	int getDeletionID(bool last_one = false)
	{
		static int s_DeletionID = 0;
		if(!last_one)
			return s_DeletionID++;

		if (s_DeletionID == 0)
			return 0;
		return --s_DeletionID;
	}

	void undoLastDelete(bool selectAllChecked)
	{
		int lastDeletionID = getDeletionID(true);

		for (size_t i = 0; i < sg_FileEntries.size(); ++i)
		{
			if (sg_FileEntries[i].deletedID() == lastDeletionID)
			{
				sg_FileEntries[i].setDelete(false);
				sg_FileEntries[i].setSelected(selectAllChecked);
			}
		}
	}

	void deleteSelectedEntries()
	{
		int newDeleteId = getDeletionID();

		for (size_t i = 0; i < FH::sg_FileEntries.size(); ++i)
			if (FH::sg_FileEntries[i].isSelected())
				FH::sg_FileEntries[i].setDelete(true, newDeleteId);
	}

	void setSelectAllEntries(bool state)
	{
		for (size_t i = 0; i < FH::sg_FileEntries.size(); ++i)
			if(!FH::sg_FileEntries[i].isDeleted())
				FH::sg_FileEntries[i].setSelected(state);
	}

	void addEntry(const std::string& str)
	{
		for (size_t i = 0; i < sg_FileEntries.size(); ++i)
		{
			if (sg_FileEntries[i].fileName().find(str) != std::string::npos)
			{
				if (sg_FileEntries[i].isDeleted())
				{
					sg_FileEntries[i].setDelete(false);
					sg_FileEntries[i].setSelected(false);
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
			sg_FileEntries.push_back({ str, extension });
	}

	void filterFileEntries(std::string str)
	{
		// lowercase string
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char c) { return std::tolower(c); });

		for (size_t i = 0; i < sg_FileEntries.size(); ++i)
		{
			std::string fileNameLower = sg_FileEntries[i].fileName();
			// lowercase filename
			std::transform(fileNameLower.begin(), fileNameLower.end(), fileNameLower.begin(),
				[](unsigned char c) { return std::tolower(c); });

			// didn't find
			if (fileNameLower.find(str) == std::string::npos)
			{
				sg_FileEntries[i].setShown(false);
				sg_FileEntries[i].setSelected(false);
			}
			else
				sg_FileEntries[i].setShown(true);
		}
	}

	std::string getRelativePath(const std::string& base_str, const std::string& p)
	{
		std::filesystem::path base(base_str);
		std::filesystem::path p2(p);
		return std::filesystem::relative(p2, base).generic_string();
	}
}
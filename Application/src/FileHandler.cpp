#include <iostream>
#include <algorithm>

#include "FileHandler.h"

namespace FH {

	static std::vector<FileEntry> sg_FileEntries;

	FileEntry::FileEntry()
		: m_ID(sm_GlobalID), m_DeletedID(ID_NOT_DELETED), m_Deleted(false), m_Shown(true), m_Selected(false)
	{
		++sm_GlobalID;
	}

	FileEntry::FileEntry(const std::string& str)
		: m_ID(sm_GlobalID), m_DeletedID(ID_NOT_DELETED), m_Deleted(false), m_Shown(true), m_Selected(false), m_FileName(str)
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
			setSelected(true);
		}
	}
	void FileEntry::setSelected(bool selected)	{ m_Selected  = selected; }
	void FileEntry::setShown(bool shown)		{ m_Shown     = shown;    }

	bool& FileEntry::getSelectedRef()			{ return m_Selected; }
	bool FileEntry::isSelected()				{ return m_Selected; }
	bool FileEntry::isShown()					{ return m_Shown;    }
	bool FileEntry::isDeleted()					{ return m_Deleted;  }
	int  FileEntry::ID()						{ return m_ID;		 }
	int  FileEntry::deletedID()					{ return m_DeletedID;}
	std::string& FileEntry::fileName()			{ return m_FileName; }


	FileEntryVec* getFileEntriesPtr()
	{
		return &sg_FileEntries;
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

	void undoLastDelete()
	{
		int lastDeletionID = getDeletionID(true);

		for (size_t i = 0; i < sg_FileEntries.size(); ++i)
		{
			if (sg_FileEntries[i].deletedID() == lastDeletionID)
			{
				sg_FileEntries[i].setDelete(false);
				sg_FileEntries[i].setSelected(false);
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
		bool alreadyExists = false;
		for (size_t i = 0; i < sg_FileEntries.size(); ++i)
		{
			if (sg_FileEntries[i].fileName().find(str) != std::string::npos)
				alreadyExists = true;
		}
		if (!alreadyExists)
			sg_FileEntries.push_back(str);
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
}
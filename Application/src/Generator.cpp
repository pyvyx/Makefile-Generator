#include <vector>
#include <sstream>
#include <set>

#include "Generator.h"
#include "FileHandler.h"

#define CC_GCC     0
#define CC_GPP     1
#define CC_CLANG   2
#define CC_CLANGPP 3
#define CC_OTHER   4

namespace MG {

	std::vector<std::string> splitStringByChar(const std::string& str, char to_split)
	{
		std::stringstream test(str);
		std::string segment;
		std::vector<std::string> seglist;

		while (std::getline(test, segment, to_split))
		{
			seglist.push_back(segment);
		}
		return seglist;
	}

	std::string getCompiler(int selectedCompiler)
	{
		switch (selectedCompiler)
		{
		case CC_GCC:
			return "gcc";
		case CC_GPP:
			return "g++";
		case CC_CLANG:
			return "clang";
		case CC_CLANGPP:
			return "clang++";
		case CC_OTHER:
		default:
			return "";
		}
	}

	std::string createDirectoryPaths(const std::string& include_dirs, const std::string& identifier)
	{
		std::string result;
		std::vector<std::string> includeDirs = splitStringByChar(include_dirs, ';');
		for (size_t i = 0; i < includeDirs.size(); ++i)
			result.append("-" + identifier + includeDirs[i] + " ");

		return result;
	}

	std::string generateTarget(const std::string& targetName, const std::string& dependencies, const std::string& command)
	{
		std::string target = targetName + ": " + dependencies + "\n" + command;
		return target;
	}

	std::string createFiles(FH::FileEntryVec& vec)
	{
		std::string result;
		for (size_t i = 0; i < vec.size(); ++i)
		{
			if (!vec[i].isDeleted())
				result += "File" + std::to_string(i) + "=" + vec[i].fileName() + "\n";
		}
		return result;
	}

	std::string getDirName(const std::string& fname)
	{
		size_t pos = fname.find_last_of("\\/");
		return (std::string::npos == pos) ? "" : fname.substr(0, pos);
	}

	std::set<std::string> getAllSourceDirectories(FH::FileEntryVec& vec)
	{
		std::set<std::string> sourceDirs;
		for (size_t i = 0; i < vec.size(); ++i)
		{
			if (!vec[i].isDeleted())
				sourceDirs.insert(getDirName(vec[i].fileName()));
		}
		return sourceDirs;
	}

	void GenerateMakeFile(GeneratorInfo info)
	{
		std::string compiler = getCompiler(info.selectedCompiler);
		std::string includeDirectories = createDirectoryPaths(info.includeDirs, "I");
		std::string libraryDirectories = createDirectoryPaths(info.includeDirs, "L");
		std::set<std::string> sourceDirs = getAllSourceDirectories(info.files);

		std::cout << generateTarget(info.outputDir + "/" + "%.o", *sourceDirs.begin() + "%.cpp", "\ttest");
	}

}
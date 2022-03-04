#include <vector>
#include <sstream>
#include <unordered_set>
#include <fstream>

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

	std::pair<std::string, std::string> createFileVariables(FH::FileEntryVec& vec, const std::string& outputDir)
	{
		std::string result = "SOURCE_FILES=";
		for (size_t i = 0; i < vec.size(); ++i)
		{
			if (!vec[i].isDeleted())
				result += vec[i].fileName() + " ";
		}

		std::string resultOBJ = "OBJ_FILES=$(addprefix " + outputDir + "/, $(notdir $(SOURCE_FILES:.cpp=.o)))";
		return {result, resultOBJ};
	}

	std::string getDirName(const std::string& fname)
	{
		size_t pos = fname.find_last_of("\\/");
		return (std::string::npos == pos) ? "" : fname.substr(0, pos);
	}

	std::unordered_set<std::string> getAllSourceDirectories(FH::FileEntryVec& vec)
	{
		std::unordered_set<std::string> sourceDirs;
		for (size_t i = 0; i < vec.size(); ++i)
		{
			if (!vec[i].isDeleted())
			{
				std::string dirName = getDirName(vec[i].fileName());
				if(dirName.size() > 0)
					sourceDirs.insert(dirName + "/");
				else
					sourceDirs.insert("");
			}
		}
		return sourceDirs;
	}

	void GenerateMakeFile(GeneratorInfo info)
	{
		if (info.outputDir == "")
			info.outputDir = "out";

		std::string compiler = getCompiler(info.selectedCompiler);
		std::string includeDirectories = createDirectoryPaths(info.includeDirs, "I");
		std::string libraryDirectories = createDirectoryPaths(info.includeDirs, "L");

		std::string buildObjFilesCommand = compiler + " " + info.compilerFlags + " -c $< -o $@ " + includeDirectories;
		std::string sourceFiles = createFileVariables(info.files, info.outputDir).first;
		std::string objectFiles = createFileVariables(info.files, info.outputDir).second;

		std::unordered_set<std::string> sourceDirs = getAllSourceDirectories(info.files);

		std::string objectTargets;
		for (const auto& i : sourceDirs)
			objectTargets += generateTarget("out/%.o", i + "%.c", "\t" + buildObjFilesCommand + "\n\n");
			//objectTargets += generateTarget("out/%.o", i + "%.c", "\tgcc -D _GLFW_WIN32 -c $< -o $@ " + includeDirectories + "\n\n");

		std::cout << objectTargets << std::endl;
		std::string buildTarget = generateTarget("Build", "$(OBJ_FILES)", "");

		std::ofstream makeFile(info.makeFileOutput + "/makefile");
		
		makeFile << sourceFiles << '\n';
		makeFile << objectFiles << '\n';
		makeFile << buildTarget << '\n';
		makeFile << objectTargets << '\n';
		
		makeFile.close();

		//std::unordered_set<std::string> sourceDirs = getAllSourceDirectories(info.files);
		//std::cout << generateTarget(info.outputDir + "/" + "%.o", *sourceDirs.begin() + "%.cpp", "\ttest");
	}

}
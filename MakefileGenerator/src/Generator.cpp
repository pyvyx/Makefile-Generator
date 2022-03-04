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

	struct MakeFileVariable
	{
		MakeFileVariable() = default;
		MakeFileVariable(const std::string& mv, const std::string& va)
			: makeVariable(mv), value(va) {}
		std::string makeVariable;
		std::string value;
	};

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


	MakeFileVariable getCompiler(int selectedCompiler)
	{
		switch (selectedCompiler)
		{
		case CC_GCC:
			return {"CXXCOMP", "gcc" };
		case CC_GPP:
			return { "CXXCOMP", "g++" };
		case CC_CLANG:
			return { "CXXCOMP", "clang" };
		case CC_CLANGPP:
			return { "CXXCOMP", "clang++" };
		case CC_OTHER:
		default:
			return { "CXXCOMP", "" };
		}
	}


	MakeFileVariable createDirectoryPaths(const std::string& include_dirs, const std::string& identifier)
	{
		std::string result;
		std::vector<std::string> includeDirs = splitStringByChar(include_dirs, ';');
		for (size_t i = 0; i < includeDirs.size(); ++i)
			result.append("-" + identifier + includeDirs[i] + "/ ");

		if(identifier == "I")
			return { "INCLUDEDIRS", result };
		if (identifier == "L")
			return { "LIBRARYDIRS", result };

		return { "", result };
	}


	std::string generateTarget(const std::string& targetName, const std::string& dependencies, const std::string& command)
	{
		std::string target = targetName + ": " + dependencies + "\n" + command;
		return target;
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


	std::unordered_set<std::string> GetAllFileExtensions(FH::FileEntryVec& vec)
	{
		std::unordered_set<std::string> fileExtensions;
		for (auto& i : vec)
			if (!i.isDeleted())
				fileExtensions.insert(i.extension());
		return fileExtensions;
	}


	std::string GetMakeVariable(const std::string& makeVar)
	{
		return "$(" + makeVar + ")";
	}

	std::string GetMakeVariable(const MakeFileVariable& makeVar)
	{
		return makeVar.makeVariable + "=" + makeVar.value;
	}


	struct BuildTargets
	{
		BuildTargets(const std::string& file, const MakeFileVariable& objfile, const std::string target)
			: srcfiles(file), objfiles(objfile), targets(target) {}
		std::string srcfiles;
		MakeFileVariable objfiles;
		std::string targets;
	};


	struct FileData
	{
		FileData(const MakeFileVariable& compiler, const MakeFileVariable& compilerFlags,
			const MakeFileVariable& includeDirs, const MakeFileVariable& libraryDirs, const MakeFileVariable& libraries,
			const MakeFileVariable& intFolder, const MakeFileVariable& outFolder, const MakeFileVariable& outFile)
				: compiler(compiler), compilerFlags(compilerFlags), includeDirs(includeDirs), libraryDirs(libraryDirs),
				libraries(libraries), intFolder(intFolder), outFolder(outFolder), outFile(outFile) {}

		MakeFileVariable compiler;
		MakeFileVariable compilerFlags;
		MakeFileVariable includeDirs;
		MakeFileVariable libraryDirs;
		MakeFileVariable libraries;
		MakeFileVariable intFolder;
		MakeFileVariable outFolder;
		MakeFileVariable outFile;
		int buildMode = 0;
	};


	std::vector<BuildTargets> CreateFileVariables(const FileData& fd, FH::FileEntryVec& vec)
	{
		std::unordered_set<std::string> extensions = GetAllFileExtensions(vec);
		std::vector<BuildTargets> ve;
		std::string targets;

		for (const auto& i : extensions)
		{
			std::string sourceFiles = "SRCFILES" + i.substr(1, i.size() - 1);
			MakeFileVariable objFiles("OBJFILES" + i.substr(1, i.size() - 1), "$(addprefix " + GetMakeVariable(fd.intFolder.makeVariable) + "/, $(notdir $(" + sourceFiles + ":" + i + "=.o)))");
			sourceFiles += "=";

			for (auto& j : vec)
			{
				if (j.extension() == i && !j.isDeleted())
					sourceFiles += j.fileName() + " ";
			}

			std::unordered_set<std::string> sourceDirs = getAllSourceDirectories(vec);
			for (auto& j : sourceDirs)
			{
				std::string buildTarget = GetMakeVariable(fd.intFolder.makeVariable) + "/%.o: " + j + "%" + i + "\n\t" + GetMakeVariable(fd.compiler.makeVariable) + " " +
					GetMakeVariable(fd.compilerFlags.makeVariable) + " -c $< -o $@ " + GetMakeVariable(fd.includeDirs.makeVariable) +
					" " + GetMakeVariable(fd.libraryDirs.makeVariable) + " " + GetMakeVariable(fd.libraries.makeVariable);
				targets += buildTarget + "\n\n";
			}
			ve.push_back(BuildTargets(sourceFiles, objFiles, targets));
			targets.clear();
		}
		return ve;
	}


	std::string CreateFileBuildTarget(const std::vector<BuildTargets>& vec, const FileData& fd)
	{
		std::string target = GetMakeVariable(fd.outFolder.makeVariable) + "/" + GetMakeVariable(fd.outFile.makeVariable) + ": ";
		for (auto& i : vec)
		{
			target += GetMakeVariable(i.objfiles.makeVariable) + " ";
		}
		target += "\n\t";

		if (fd.buildMode == BuildModes::Application)
		{
			target += GetMakeVariable(fd.compiler.makeVariable) + " " + GetMakeVariable(fd.compilerFlags.makeVariable) + " ";
			for (auto& i : vec)
				target += GetMakeVariable(i.objfiles.makeVariable) + " ";
			target += " -o " + GetMakeVariable(fd.outFolder.makeVariable) + "/" + GetMakeVariable(fd.outFile.makeVariable) + " ";
			target += GetMakeVariable(fd.includeDirs.makeVariable) + " " + GetMakeVariable(fd.libraryDirs.makeVariable) + " ";
			target += GetMakeVariable(fd.libraries.makeVariable);
			target += " " + GetMakeVariable(fd.libraryDirs.makeVariable) + " " + GetMakeVariable(fd.libraries.makeVariable);
		}
		else if (fd.buildMode == BuildModes::StaticLibrary)
		{
			target += "ar rcs " + GetMakeVariable(fd.outFolder.makeVariable) + "/lib" + GetMakeVariable(fd.outFile.makeVariable) + ".a";
			target += " ";
			for (auto& i : vec)
				target += GetMakeVariable(i.objfiles.makeVariable) + " ";
		}
		else if (fd.buildMode == BuildModes::DynamicLibrary)
		{
			target += GetMakeVariable(fd.compiler.makeVariable) + " -shared ";
			for (auto& i : vec)
				target += GetMakeVariable(i.objfiles.makeVariable) + " ";
			target += " -o " + GetMakeVariable(fd.outFolder.makeVariable) + "/lib" + GetMakeVariable(fd.outFile.makeVariable) + ".so";
			target += " " + GetMakeVariable(fd.libraryDirs.makeVariable) + " " + GetMakeVariable(fd.libraries.makeVariable);
		}
		return target;
	}


	void WriteMakeFile(const FileData& fd, const std::vector<BuildTargets>& buildTargets, const std::string& makefileOutput)
	{
		std::ofstream makeFile(makefileOutput + "/makefile");
		if (!makeFile)
		{
			return;
		}

		makeFile << GetMakeVariable(fd.compiler) << '\n';
		makeFile << GetMakeVariable(fd.compilerFlags) << '\n';
		makeFile << GetMakeVariable(fd.includeDirs) << '\n';
		makeFile << GetMakeVariable(fd.libraryDirs) << '\n';
		makeFile << GetMakeVariable(fd.libraries) << '\n';
		makeFile << GetMakeVariable(fd.outFile) << '\n';
		makeFile << GetMakeVariable(fd.outFolder) << '\n';
		makeFile << GetMakeVariable(fd.intFolder) << "\n\n";

		for (auto& i : buildTargets)
		{
			makeFile << i.srcfiles << std::endl;
			makeFile << i.objfiles.makeVariable << "=" << i.objfiles.value << std::endl;
		}

		makeFile << "\n#Do not edit below this line\n";
		makeFile << "Build: " << GetMakeVariable(fd.outFolder.makeVariable) << " " << GetMakeVariable(fd.intFolder.makeVariable) << " " << GetMakeVariable(fd.outFolder.makeVariable) << "/";
		makeFile << GetMakeVariable(fd.outFile.makeVariable) << "\n\n";
		makeFile << CreateFileBuildTarget(buildTargets, fd) << "\n\n";

		for (auto& i : buildTargets)
		{
			makeFile << i.targets << '\n';
		}

		makeFile << generateTarget(GetMakeVariable(fd.outFolder.makeVariable), "", "\tmkdir " + GetMakeVariable(fd.outFolder.makeVariable)) << '\n';
		makeFile << generateTarget(GetMakeVariable(fd.intFolder.makeVariable), "", "\tmkdir " + GetMakeVariable(fd.intFolder.makeVariable)) << "\n\n";
		makeFile << generateTarget("clean", "", "\trmdir /s /q " + GetMakeVariable(fd.outFolder.makeVariable));
		makeFile.close();
	}


	void GenerateMakeFile(GeneratorInfo info)
	{
		MakeFileVariable compiler = getCompiler(info.selectedCompiler);
		MakeFileVariable compilerFlags("CXXFLAGS", info.compilerFlags);
		MakeFileVariable includeDirectories = createDirectoryPaths(info.includeDirs, "I");
		MakeFileVariable libraryDirectories = createDirectoryPaths(info.libraryDirs, "L");
		MakeFileVariable libraries("LIBRARIES", info.linkLibraries);
		MakeFileVariable outFile("EXE", info.outFileName == "" ? "MyOutput" : info.outFileName);

		std::string outDir = info.outputDir == "" ? "Out" : info.outputDir;
		MakeFileVariable outFolder("OUTPUTFOLDER", outDir);
		MakeFileVariable intFolder("INTFOLDER", outDir + "\\BIN");

		FileData fd(compiler, compilerFlags, includeDirectories, libraryDirectories, libraries, intFolder, outFolder, outFile);
		fd.buildMode = info.selectedBinaryFormat;

		std::vector<BuildTargets> buildTargets = CreateFileVariables(fd, info.files);
		WriteMakeFile(fd, buildTargets, info.makeFileOutput);
	}
		/*
		std::ofstream makeFile(info.makeFileOutput + "/makefile");

		makeFile << GetMakeVariable(compiler) << '\n';
		makeFile << GetMakeVariable(compilerFlags) << '\n';
		makeFile << GetMakeVariable(includeDirectories) << '\n';
		makeFile << GetMakeVariable(libraryDirectories) << '\n';
		makeFile << GetMakeVariable(libraries) << '\n';

		for (auto& i : buildTargets)
		{
			makeFile << i.srcfiles << std::endl;
			makeFile << i.objfiles.makeVariable << "=" << i.objfiles.value << std::endl;
		}
		makeFile << "#Do not edit below this line\n";
		makeFile << "Build: out/" + outputFile.value + " ";
		for (auto& i : buildTargets)
		{
			makeFile << GetMakeVariable(i.objfiles.makeVariable) << " ";
		}
		makeFile << std::endl << std::endl;

		makeFile << exe << std::endl << std::endl;

		for (auto& i : buildTargets)
		{
			makeFile << i.targets << std::endl;
		}

		makeFile.close();
		*/
		
		//for (auto& i : fileExtensions)
		//{
		//	std::cout << i << std::endl;
		//}


		/*
		if (info.outputDir == "")
			info.outputDir = "out";

		std::string compiler = getCompiler(info.selectedCompiler);
		std::string includeDirectories = createDirectoryPaths(info.includeDirs, "I");
		std::string libraryDirectories = createDirectoryPaths(info.libraryDirs, "L");

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
		*/
		//std::unordered_set<std::string> sourceDirs = getAllSourceDirectories(info.files);
		//std::cout << generateTarget(info.outputDir + "/" + "%.o", *sourceDirs.begin() + "%.cpp", "\ttest");
}
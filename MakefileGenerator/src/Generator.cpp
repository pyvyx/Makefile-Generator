#include <vector>
#include <sstream>
#include <unordered_set>
#include <fstream>
#include <functional>
#include <algorithm>

#include "Generator.h"
#include "FileHandler.h"

namespace MG {

	std::string WIN_OR_LINUX(const std::string& windows_case, const std::string& linux_case)
	{
		#ifdef _WIN32
			return windows_case;
		#endif
		#if defined linux || defined __APPLE__
			return linux_case;
		#endif
	}

	struct MakeFileVariable
	{
		MakeFileVariable() = default;
		MakeFileVariable(const std::string& mv, const std::string& var, const std::string& va)
			: makeVariable(mv), variable(var), value(va) {}
		std::string makeVariable;
		std::string variable;
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


	std::pair<MakeFileVariable, MakeFileVariable> getCompiler(int selectedCompiler)
	{
		switch (selectedCompiler)
		{
		case Compiler::GCC_GPP:
			return { {"$(CCOMP)", "CCOMP", "gcc"    }, {"$(CXXCOMP)", "CXXCOMP", "g++"    } };
		case Compiler::Clang_ClangPP:
			return { {"$(CCOMP)", "CCOMP", "clang"  }, {"$(CXXCOMP)", "CXXCOMP", "clang++"} };
		case Compiler::GCC:
			return { {"$(CCOMP)", "CCOMP", "gcc"    }, {"$(CXXCOMP)", "CXXCOMP", "gcc"    } };
		case Compiler::GPP:
			return { {"$(CCOMP)", "CCOMP", "g++"    }, {"$(CXXCOMP)", "CXXCOMP", "g++"    } };
		case Compiler::Clang:
			return { {"$(CCOMP)", "CCOMP", "clang"  }, {"$(CXXCOMP)", "CXXCOMP", "clang"  } };
		case Compiler::ClangPP:
			return { {"$(CCOMP)", "CCOMP", "clang++"}, {"$(CXXCOMP)", "CXXCOMP", "clang++"} };
		default:
			return { {"$(CCOMP)", "CCOMP", ""       }, {"$(CXXCOMP)", "CXXCOMP", ""       } };
		}
	}


	MakeFileVariable createDirectoryPaths(const std::string& include_dirs, const std::string& identifier)
	{
		std::string result;
		std::vector<std::string> includeDirs = splitStringByChar(include_dirs, ';');
		for (size_t i = 0; i < includeDirs.size(); ++i)
			result.append("-" + identifier + includeDirs[i] + "/ ");

		if(identifier == "I")
			return { "$(INCLUDEDIRS)", "INCLUDEDIRS", result };
		if (identifier == "L")
			return { "$(LIBRARYDIRS)", "LIBRARYDIRS", result };

		return { "", "", result };
	}


	std::string generateTarget(const std::string& targetName, const std::string& dependencies, const std::string& command)
	{
		std::string target = targetName + ": " + dependencies + "\n" + command;
		return target;
	}


	std::string GetFilePath(const std::string& fname)
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
				std::string dirName = GetFilePath(vec[i].fileName());
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
		FileData(const MakeFileVariable& ccompiler, const MakeFileVariable& ccompilerFlags,
			const MakeFileVariable& cppcompiler, const MakeFileVariable& cppcompilerFlags,
			const MakeFileVariable& includeDirs, const MakeFileVariable& libraryDirs, const MakeFileVariable& libraries,
			const MakeFileVariable& intFolder, const MakeFileVariable& outFolder, const MakeFileVariable& outFile
		)
			: ccompiler(ccompiler), ccompilerFlags(ccompilerFlags), cppcompiler(cppcompiler), cppcompilerFlags(cppcompilerFlags),
			includeDirs(includeDirs), libraryDirs(libraryDirs), libraries(libraries), intFolder(intFolder),
			outFolder(outFolder), outFile(outFile) {}

		MakeFileVariable ccompiler;
		MakeFileVariable ccompilerFlags;
		MakeFileVariable cppcompiler;
		MakeFileVariable cppcompilerFlags;
		MakeFileVariable includeDirs;
		MakeFileVariable libraryDirs;
		MakeFileVariable libraries;
		MakeFileVariable intFolder;
		MakeFileVariable outFolder;
		MakeFileVariable outFile;
		int buildMode = 0;
	};


	std::pair<std::vector<BuildTargets>, MakeFileVariable> CreateFileVariables(const FileData& fd, FH::FileEntryVec& vec)
	{
		std::unordered_set<std::string> extensions = GetAllFileExtensions(vec);
		std::vector<BuildTargets> ve;
		std::string targets;

		auto lowerCaseString = [=](std::string str) {
			std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
			return str;
		};
		MakeFileVariable resFiles("$(RESFILES)", "RESFILES", "");

		for (const auto& i : extensions)
		{
			if (lowerCaseString(i) == ".res")
			{
				for (auto& j : vec)
				{
					if (j.extension() == i && !j.isDeleted())
						resFiles.value += j.fileName() + " ";
				}
				continue;
			}

			std::string sourceFiles = "SRCFILES" + i.substr(1, i.size() - 1);
			MakeFileVariable objFiles("$(OBJFILES" + i.substr(1, i.size() - 1) + ")", "OBJFILES" + i.substr(1, i.size() - 1), "$(addprefix " + fd.intFolder.makeVariable + "/, $(notdir $(" + sourceFiles + ":" + i + "=.o)))");
			sourceFiles += "=";

			for (auto& j : vec)
			{
				if (j.extension() == i && !j.isDeleted())
					sourceFiles += j.fileName() + " ";
			}

			std::unordered_set<std::string> sourceDirs = getAllSourceDirectories(vec);
			for (auto& j : sourceDirs)
			{
				std::string compiler = i == ".c" ? fd.ccompiler.makeVariable : fd.cppcompiler.makeVariable;
				std::string compilerFlags = i == ".c" ? fd.ccompilerFlags.makeVariable : fd.cppcompilerFlags.makeVariable;

				std::string buildTarget = fd.intFolder.makeVariable + "/%.o: " + j + "%" + i + "\n\t" + compiler + " " +
					compilerFlags + " -c $< -o $@ " + fd.includeDirs.makeVariable;
				targets += buildTarget + "\n\n";
			}
			ve.push_back(BuildTargets(sourceFiles, objFiles, targets));
			targets.clear();
		}
		return { ve, resFiles };
	}


	std::string CreateFileBuildTarget(const std::vector<BuildTargets>& vec, const FileData& fd, const MakeFileVariable resFiles)
	{
		std::string target = fd.outFolder.makeVariable + "/" + fd.outFile.makeVariable + ": ";
		for (auto& i : vec)
		{
			target += i.objfiles.makeVariable + " ";
		}
		target += "\n\t";

		if (fd.buildMode == BuildModes::Application)
		{
			target += fd.cppcompiler.makeVariable + " " + fd.cppcompilerFlags.makeVariable + " ";
			for (auto& i : vec)
				target += i.objfiles.makeVariable + " ";
			target += resFiles.makeVariable;
			target += " -o " + fd.outFolder.makeVariable + "/" + fd.outFile.makeVariable + " ";
			target += fd.libraryDirs.makeVariable + " " + fd.libraries.makeVariable;
		}
		else if (fd.buildMode == BuildModes::StaticLibrary)
		{
			target += "ar rcs " + fd.outFolder.makeVariable + "/" + fd.outFile.makeVariable + "";
			target += " ";
			for (auto& i : vec)
				target += i.objfiles.makeVariable + " ";
		}
		else if (fd.buildMode == BuildModes::DynamicLibrary)
		{
			target += fd.cppcompiler.makeVariable + " -shared ";
			for (auto& i : vec)
				target += i.objfiles.makeVariable + " ";
			target += "-o " + fd.outFolder.makeVariable + "/" + fd.outFile.makeVariable + "";
			target += " " + fd.libraryDirs.makeVariable + " " + fd.libraries.makeVariable;
		}
		return target;
	}


	void WriteMakeFile(const FileData& fd, const std::string& makefileOutput, FH::FileEntryVec& vec)
	{
		std::ofstream makeFile(makefileOutput + "/makefile");
		if (!makeFile)
		{
			return;
		}

		std::pair<std::vector<BuildTargets>, MakeFileVariable> targets = CreateFileVariables(fd, vec);
		std::vector<BuildTargets> buildTargets = targets.first;
		MakeFileVariable resFiles = targets.second;

		std::function<std::string(const MakeFileVariable&)> 
			getMakeVariable = [&](const MakeFileVariable& makeVar) -> std::string{ return makeVar.variable + "=" + makeVar.value; };

		makeFile << getMakeVariable(fd.ccompiler) << '\n';
		makeFile << getMakeVariable(fd.ccompilerFlags) << '\n';
		makeFile << getMakeVariable(fd.cppcompiler) << '\n';
		makeFile << getMakeVariable(fd.cppcompilerFlags) << '\n';
		makeFile << getMakeVariable(fd.includeDirs) << '\n';
		makeFile << getMakeVariable(fd.libraryDirs) << '\n';
		makeFile << getMakeVariable(fd.libraries) << '\n';
		makeFile << getMakeVariable(fd.outFile) << '\n';
		makeFile << getMakeVariable(fd.outFolder) << '\n';
		makeFile << getMakeVariable(fd.intFolder) << '\n';
		makeFile << getMakeVariable(resFiles) << "\n\n";

		std::string extensionTargets;
		extensionTargets.reserve(buildTargets.size() * buildTargets[0].targets.size() + 10); // 10 is an arbitrary number
		for (auto& i : buildTargets)
		{
			makeFile << i.srcfiles << std::endl;
			makeFile << i.objfiles.variable << "=" << i.objfiles.value << std::endl;
			extensionTargets += i.targets + "\n";
		}

		makeFile << "\n#Do not edit below this line\n";
		makeFile << ".PHONY: build clean rebuild\n\n";
		makeFile << "build: " << fd.outFolder.makeVariable << " " << fd.intFolder.makeVariable << " " << fd.outFolder.makeVariable << "/";
		makeFile << fd.outFile.makeVariable << "\n\n";
		makeFile << "rebuild: clean build\n\n";
		makeFile << CreateFileBuildTarget(buildTargets, fd, resFiles) << "\n\n\n";

		makeFile << extensionTargets;
		//for (auto& i : buildTargets)
		//{
		//	makeFile << i.targets << '\n';
		//}

		makeFile << generateTarget(fd.outFolder.makeVariable, "", WIN_OR_LINUX("\tmkdir ", "\tmkdir -p") + fd.outFolder.makeVariable) << '\n';
		makeFile << generateTarget(fd.intFolder.makeVariable, "", WIN_OR_LINUX("\tmkdir ", "\tmkdir -p") + fd.intFolder.makeVariable) << "\n\n";
		makeFile << generateTarget("clean", "", WIN_OR_LINUX("\trmdir /s /q ", "\trm -r ") + fd.outFolder.makeVariable);
		makeFile.close();
	}


	void GenerateMakeFile(GeneratorInfo info)
	{
		std::pair<MakeFileVariable, MakeFileVariable> compiler = getCompiler(info.selectedCompiler);
		MakeFileVariable ccompiler = compiler.first;
		MakeFileVariable ccompilerFlags("$(CFLAGS)", "CFLAGS", info.ccompilerFlags);
		MakeFileVariable cppcompiler = compiler.second;
		MakeFileVariable cppcompilerFlags("$(CXXFLAGS)", "CXXFLAGS", info.cppcompilerFlags);
		MakeFileVariable includeDirectories = createDirectoryPaths(info.includeDirs, "I");
		MakeFileVariable libraryDirectories = createDirectoryPaths(info.libraryDirs, "L");
		MakeFileVariable libraries("$(LIBRARIES)", "LIBRARIES", info.linkLibraries);
		MakeFileVariable outFile("$(EXE)", "EXE", info.outFileName == "" ? "MyOutput" : info.outFileName);

		std::string outDir = info.outputDir == "" ? "Out" : info.outputDir;
		MakeFileVariable outFolder("$(OUTPUTFOLDER)", "OUTPUTFOLDER", outDir);
		MakeFileVariable intFolder("$(INTFOLDER)", "INTFOLDER", outDir + WIN_OR_LINUX("\\BIN", "/BIN"));

		FileData fd(ccompiler, ccompilerFlags, cppcompiler, cppcompilerFlags, includeDirectories, libraryDirectories, libraries, intFolder, outFolder, outFile);
		fd.buildMode = info.selectedBinaryFormat;

		WriteMakeFile(fd, info.makeFileOutput, info.files);
	}
}
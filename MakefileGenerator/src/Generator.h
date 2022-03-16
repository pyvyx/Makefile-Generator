#pragma once
#include <string>
#include <iostream>

#include "FileHandler.h"

namespace MG {

    enum Compiler : char
    {
        GCC_GPP,
        Clang_ClangPP,
        GCC,
        GPP,
        Clang,
        ClangPP
    };

    enum BuildMode : char
    {
        Application,
        StaticLibrary,
        DynamicLibrary
    };

    struct GeneratorInfo
    {
        int selectedCompiler = Compiler::GCC_GPP;
        int selectedBinaryFormat = BuildMode::Application;
        bool usePIL = true;
        std::string ccompilerFlags;
        std::string cppcompilerFlags;
        std::string linkLibraries;
        std::string makeFileOutput;
        std::string outputDir;
        std::string includeDirs;
        std::string libraryDirs;
        std::string dllFileName;
        std::string outFileName;
        std::vector<FH::FileEntry> files;
    };


    std::vector<std::string> SplitStringByChar(const std::string& str, char to_split);
    void GenerateMakeFile(const GeneratorInfo& info);
    void SaveConfigFile(const GeneratorInfo& info, const std::string& file_path);
    GeneratorInfo LoadConfigFile(const std::string& file_path);
}
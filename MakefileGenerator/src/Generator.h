#pragma once
#include <string>
#include <iostream>

#include "FileHandler.h"

namespace MG {

    struct GeneratorInfo
    {
        int selectedCompiler = 0;
        int selectedBinaryFormat = 0;
        bool selectedAll = false;
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

    void GenerateMakeFile(const GeneratorInfo& info);
    void SaveConfigFile(const GeneratorInfo& info, const std::string& file_path);
    GeneratorInfo LoadConfigFile(const std::string& file_path);
}
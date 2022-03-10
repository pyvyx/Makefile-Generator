#pragma once
#include <string>
#include <iostream>

#include "FileHandler.h"

namespace MG {

    struct GeneratorInfo
    {
        int selectedCompiler = 0;
        bool usePIL = true;
        std::string ccompilerFlags;
        std::string cppcompilerFlags;
        std::string linkLibraries;
        std::string makeFileOutput;
        std::string outputDir;
        std::string includeDirs;
        std::string libraryDirs;
        std::vector<FH::FileEntry> files;
        int selectedBinaryFormat = 0;
        std::string dllFileName;
        std::string outFileName;

        void print()
        {
            std::cout << "Selected compiler : " << selectedCompiler << std::endl;
            std::cout << "C compiler Flags  : " << ccompilerFlags << std::endl;
            std::cout << "C++ compiler Flags: " << cppcompilerFlags << std::endl;
            std::cout << "Link libraries    : " << linkLibraries << std::endl;
            std::cout << "Makefile output   : " << makeFileOutput << std::endl;
            std::cout << "Output directory  : " << outputDir << std::endl;
            std::cout << "Include directory : " << includeDirs << std::endl;
            std::cout << "Library directory : " << libraryDirs << std::endl;
            std::cout << "Selected bin form : " << selectedBinaryFormat << std::endl;
            std::cout << "Files             : " << files.size() << std::endl;
        }
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

    void GenerateMakeFile(GeneratorInfo info);
    void SaveConfigFile(GeneratorInfo info, const std::string& file_path, bool selectedAll);
    void LoadConfigFile(const std::string& file_path);
}
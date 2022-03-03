#pragma once
#include <string>
#include <iostream>

#include "FileHandler.h"

namespace MG {

    struct GeneratorInfo
    {
        int selectedCompiler = 0;  // done
        std::string compilerFlags; // done
        std::string linkLibraries; // done
        std::string makeFileOutput;
        std::string outputDir;
        std::string includeDirs;
        std::string libraryDirs;
        std::vector<FH::FileEntry> files;
        int selectedBinaryFormat = 0;

        void print()
        {
            std::cout << "Selected compiler: " << selectedCompiler << std::endl;
            std::cout << "Compiler Flags   : " << compilerFlags << std::endl;
            std::cout << "Link libraries   : " << linkLibraries << std::endl;
            std::cout << "Makefile output  : " << makeFileOutput << std::endl;
            std::cout << "Output directory : " << outputDir << std::endl;
            std::cout << "Include directory: " << includeDirs << std::endl;
            std::cout << "Library directory: " << libraryDirs << std::endl;
            std::cout << "Selected bin form: " << selectedBinaryFormat << std::endl;
            std::cout << "Files            : " << files.size() << std::endl;
        }
    };

    void GenerateMakeFile(GeneratorInfo info);

    //void GenerateMakeFile(cstring& compilerFlags, int selectedCompiler,
    //    cstring& link_libs, cstring& makeFileDir, cstring& binaryOutputDir, cstring& includeDirs,
    //    cstring& libraryDirs, const std::vector<FH::FileEntry>& files, int selectedBinaryFormat);

    //MG::GenerateMakeFile(compiler_flags, selected_compiler,
        //    linked_libs, outputfile, output_dir, include_dirs,
        //    library_dirs, files, selected_binary);

}
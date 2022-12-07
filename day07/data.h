#pragma once

#include "types.h"
#include <memory>
#include <vector>
#include <string>

namespace AoC
{
    const char* s_testInputData
    {
        "$ cd /\n"
        "$ ls\n"
        "dir a\n"
        "14848514 b.txt\n"
        "8504156 c.dat\n"
        "dir d\n"
        "$ cd a\n"
        "$ ls\n"
        "dir e\n"
        "29116 f\n"
        "2557 g\n"
        "62596 h.lst\n"
        "$ cd e\n"
        "$ ls\n"
        "584 i\n"
        "$ cd ..\n"
        "$ cd ..\n"
        "$ cd d\n"
        "$ ls\n"
        "4060174 j\n"
        "8033020 d.log\n"
        "5626152 d.ext\n"
        "7214296 k"
    };

    struct File
    {
        ~File()
        {
            for (File* subFile : SubFiles)
            {
                delete subFile;
            }
        }

        std::string Name;
        std::vector<File*> SubFiles;
        u64 Size;
    };

    struct InputData
    {
        File RootDirectory;
    };

    struct OutputData
    {
        const File* RootDirectory;
        u64 TotalDirectorySizesBelowLimit;
        u64 DirectorySizeToRemove;
    };
}
#include "data.h"
#include "aoc.h"

#include <numeric>

namespace AoC
{
    namespace Internal
    {
        void PrintFileSystem(const File& file, u32 indentation)
        {
            static constexpr u32 indentationStep{ 2 };
            u32 subFileIndentation{ indentation + indentationStep };

            fmt::print("{:{}}- {} (size:{})\n", "", indentation, file.Name, file.Size);
            for (const File* subFile : file.SubFiles)
            {
                PrintFileSystem(*subFile, subFileIndentation);
            }
        }

        void PreComputeDirectorySize(File& file)
        {
            if (!file.SubFiles.empty())
            {
                u64 directorySize{};
                for (File* subFile : file.SubFiles)
                {
                    PreComputeDirectorySize(*subFile);
                    directorySize += subFile->Size;
                }
                file.Size = directorySize;
            }
        }

        File* FindSubFile(const File& file, const std::string& fileName)
        {
            auto findByName{ [&fileName](File* subFile) { return subFile->Name == fileName; } };
            auto foundIt{ std::find_if(file.SubFiles.begin(), file.SubFiles.end(), findByName) };
            return foundIt != file.SubFiles.end() ? *foundIt : nullptr;
        }

        u64 ComputeTotalDirectoriesBelowSize(const File& file, u64 sizeLimit)
        {
            u64 total{};
            if (!file.SubFiles.empty())
            {
                if (file.Size <= sizeLimit)
                    total += file.Size;

                for (const File* subFile : file.SubFiles)
                {
                    total += ComputeTotalDirectoriesBelowSize(*subFile, sizeLimit);
                }
            }
            return total;
        }

        u64 ComputeSmallestDirectoryAboveSize(const File& file, u64 sizeLimit)
        {
            u64 minSize { std::numeric_limits<u64>::max() };
            if (!file.SubFiles.empty() && file.Size >= sizeLimit)
            {
                minSize = file.Size;

                for (const File* subFile : file.SubFiles)
                {
                    minSize = std::min(minSize, ComputeSmallestDirectoryAboveSize(*subFile, sizeLimit));
                }
            }
            return minSize;
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        inputData.RootDirectory.Name = "/";
        std::vector<File*> fileBreadcrums{};
        fileBreadcrums.push_back(&inputData.RootDirectory);

        static constexpr u32 bufferSize{ 20 };
        char fileNameBuffer[bufferSize + 1] { '\0' };
        u64 fileSize{};
        std::string line{};
        while (std::getline(inputStream, line))
        {
            if (sscanf_s(line.c_str(), "$ cd %s", fileNameBuffer, bufferSize) != 0)
            {
                std::string fileName{ fileNameBuffer };
                if (fileName == "/")
                {
                    fileBreadcrums.resize(1);
                }
                else if (fileName == "..")
                {
                    if (!fileBreadcrums.empty())
                    {
                        fileBreadcrums.pop_back();
                    }
                }
                else
                {
                    File* foundFile{ Internal::FindSubFile(*fileBreadcrums.back(), fileName) };
                    if (foundFile == nullptr)
                    {
                        foundFile = new File();
                        foundFile->Name = fileName;
                        fileBreadcrums.back()->SubFiles.push_back(foundFile);
                    }
                    fileBreadcrums.push_back(foundFile);
                }
            }
            else if (sscanf_s(line.c_str(), "dir %s", fileNameBuffer, bufferSize) != 0)
            {
                std::string fileName{ fileNameBuffer };
                File* foundFile{ Internal::FindSubFile(*fileBreadcrums.back(), fileName) };
                if (foundFile == nullptr)
                {
                    foundFile = new File();
                    foundFile->Name = fileName;
                    fileBreadcrums.back()->SubFiles.push_back(foundFile);
                }
            }
            else if (sscanf_s(line.c_str(), "%lld %s", &fileSize, fileNameBuffer, bufferSize) != 0)
            {
                std::string fileName{ fileNameBuffer };
                File* foundFile{ Internal::FindSubFile(*fileBreadcrums.back(), fileName) };
                if (foundFile == nullptr)
                {
                    foundFile = new File();
                    foundFile->Name = fileName;
                    foundFile->Size = fileSize;
                    fileBreadcrums.back()->SubFiles.push_back(foundFile);
                }
            }
        }

        Internal::PreComputeDirectorySize(inputData.RootDirectory);

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        output.RootDirectory = &input.RootDirectory;
        output.TotalDirectorySizesBelowLimit = Internal::ComputeTotalDirectoriesBelowSize(input.RootDirectory, 100000);

        static constexpr u64 totalSpace{ 70000000ULL };
        static constexpr u64 requiredSpace{ 30000000ULL };
        u64 freeSpace{ totalSpace - input.RootDirectory .Size };
        output.DirectorySizeToRemove = Internal::ComputeSmallestDirectoryAboveSize(input.RootDirectory, requiredSpace - freeSpace);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        static constexpr u64 part1TestResult{ 95437 };
        didTestsPass &= output.TotalDirectorySizesBelowLimit == part1TestResult;

        static constexpr u64 part2TestResult{ 24933642 };
        didTestsPass &= output.DirectorySizeToRemove == part2TestResult;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        Internal::PrintFileSystem(*output.RootDirectory, 0);
        fmt::print("\n");
        fmt::print("Total Directory Sizes below limit: {}\n", output.TotalDirectorySizesBelowLimit);
        fmt::print("Directory Size To Remove: {}\n", output.DirectorySizeToRemove);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
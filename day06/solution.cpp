#include "data.h"
#include "aoc.h"

namespace AoC
{
    namespace Internal
    {
        u32 ComputeFirstNUniqueCharactersPosition(const std::string& dataStream, u32 n)
        {
            u32 i{};
            u32 maxIndex{ (u32)dataStream.size() };
            u32 charUseMap[26]{ 0 };
            for (i = 0; i < n; ++i)
            {
                ++charUseMap[dataStream[i] - 'a'];
            }

            bool areAllCharUnique{};
            auto checkCharUnique{ [](u32 count) { return count <= 1; } };
            while (!areAllCharUnique && i < maxIndex)
            {
                areAllCharUnique = std::all_of(std::begin(charUseMap), std::end(charUseMap), checkCharUnique);
                --charUseMap[dataStream[i - n] - 'a'];
                ++charUseMap[dataStream[i] - 'a'];
                ++i;
            }

            return i - 1;
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData)
    {
        inputStream >> inputData.DataStream;
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        output.FirstUnique4CharPosition = Internal::ComputeFirstNUniqueCharactersPosition(input.DataStream, 4);
        output.FirstUnique14CharPosition = Internal::ComputeFirstNUniqueCharactersPosition(input.DataStream, 14);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        static constexpr u32 part1TestSolution{ 11 };
        didTestsPass &= output.FirstUnique4CharPosition == part1TestSolution;

        static constexpr u32 part2TestSolution{ 26 };
        didTestsPass &= output.FirstUnique14CharPosition == part2TestSolution;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("First Unique 4 Char Position: {}\n", output.FirstUnique4CharPosition);
        fmt::print("First Unique 14 Char Position: {}\n", output.FirstUnique14CharPosition);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
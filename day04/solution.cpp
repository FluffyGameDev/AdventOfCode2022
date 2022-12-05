#include "data.h"
#include "aoc.h"

namespace AoC
{
    bool ReadInput(std::istream& inputStream, InputData& inputData)
    {
        RangePair pair{};
        while (!inputStream.eof())
        {
            char ignore;
            inputStream >> pair.Range1.Min >> ignore >> pair.Range1.Max >> ignore;
            inputStream >> pair.Range2.Min >> ignore >> pair.Range2.Max;
            inputData.Ranges.push_back(pair);
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        auto isRangeContained{ [](const RangePair& pair)
            { return pair.Range1.IsInRange(pair.Range2) || pair.Range2.IsInRange(pair.Range1); } };
        output.ContainedRangeCount = (u32)std::count_if(input.Ranges.begin(), input.Ranges.end(), isRangeContained);

        auto doRangesOverlap{ [](const RangePair& pair)
            { return pair.Range1.DoesRangeOverlap(pair.Range2); } };
        output.OverlappingRangeCount = (u32)std::count_if(input.Ranges.begin(), input.Ranges.end(), doRangesOverlap);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        static constexpr u32 part1ExpectedValue{ 2 };
        didTestsPass &= output.ContainedRangeCount == part1ExpectedValue;

        static constexpr u32 part2ExpectedValue{ 4 };
        didTestsPass &= output.OverlappingRangeCount == part2ExpectedValue;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Contained Range Count: {}\n", output.ContainedRangeCount);
        fmt::print("Overlapping Range Count: {}\n", output.OverlappingRangeCount);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
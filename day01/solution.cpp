#include "data.h"
#include "aoc.h"

#include <algorithm>
#include <numeric>
#include <vector>

namespace AoC
{
    namespace Internal
    {
        u32 ComputeMaxCalories(const std::vector<u32>& caloriesPerElf)
        {
            auto foundIt = std::max_element(caloriesPerElf.begin(), caloriesPerElf.end());
            return foundIt != caloriesPerElf.end() ? *foundIt : 0;
        }

        u32 ComputeTopNCalories(std::vector<u32>& caloriesPerElf, u32 n)
        {
            auto endIt{ caloriesPerElf.begin() + n };
            std::nth_element(caloriesPerElf.begin(), endIt, caloriesPerElf.end(), std::greater());
            u32 total{ std::accumulate(caloriesPerElf.begin(), endIt, 0U) };
            return total;
        }
    }

    bool ReadInput(std::istream& inputStream, std::vector<u32>& caloriesPerElf)
    {
        u32 currentElfCalories{};
        std::string currentLine{};
        while (std::getline(inputStream, currentLine))
        {
            if (currentLine.size() == 0)
            {
                caloriesPerElf.push_back(currentElfCalories);
                currentElfCalories = 0;
            }
            else
            {
                std::stringstream lineBuffer{ currentLine };
                u32 calories{};
                lineBuffer >> calories;
                currentElfCalories += calories;
            }
        }

        if (currentElfCalories > 0)
        {
            caloriesPerElf.push_back(currentElfCalories);
        }
        return true;
    }

    void ComputeOutput(std::vector<u32>& caloriesPerElf, OutputData& output)
    {
        output.maxCalories = Internal::ComputeMaxCalories(caloriesPerElf);
        output.total3Calories = Internal::ComputeTopNCalories(caloriesPerElf, 3);
    }

    bool ValidateTestOutput(const OutputData& outputData)
    {
        static const u32 part1ExpectedResult{ 24000 };
        static const u32 part2ExpectedResult{ 45000 };

        bool didTestsPass{ true };

        didTestsPass &= outputData.maxCalories == part1ExpectedResult;
        didTestsPass &= outputData.total3Calories == part2ExpectedResult;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Max Calories {}.\n", output.maxCalories);
        fmt::print("Top 3 Elves Calories {}.\n", output.total3Calories);
    }
}

void main()
{
    AoC::Run<std::vector<AoC::u32>, AoC::OutputData>(AoC::s_testInputData);
}
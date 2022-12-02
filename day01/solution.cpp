#include "data.h"
#include "aoc.h"

#include <algorithm>
#include <numeric>
#include <vector>

namespace AoC
{
    namespace Internal
    {
        std::uint32_t ComputeMaxCalories(const std::vector<std::uint32_t>& caloriesPerElf)
        {
            auto foundIt = std::max_element(caloriesPerElf.begin(), caloriesPerElf.end());
            return foundIt != caloriesPerElf.end() ? *foundIt : 0;
        }

        std::uint32_t ComputeTopNCalories(std::vector<std::uint32_t>& caloriesPerElf, std::uint32_t n)
        {
            auto endIt{ caloriesPerElf.begin() + n };
            std::nth_element(caloriesPerElf.begin(), endIt, caloriesPerElf.end(), std::greater());
            std::uint32_t total{ std::accumulate(caloriesPerElf.begin(), endIt, 0U) };
            return total;
        }
    }

    bool ReadInput(std::istream& inputStream, std::vector<std::uint32_t>& caloriesPerElf)
    {
        std::uint32_t currentElfCalories{};
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
                std::uint32_t calories{};
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

    void ComputeOutput(std::vector<std::uint32_t>& caloriesPerElf, OutputData& output)
    {
        output.maxCalories = Internal::ComputeMaxCalories(caloriesPerElf);
        output.total3Calories = Internal::ComputeTopNCalories(caloriesPerElf, 3);
    }

    bool ValidateTestOutput(const OutputData& outputData)
    {
        static const std::uint32_t part1ExpectedResult{ 24000 };
        static const std::uint32_t part2ExpectedResult{ 45000 };

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
    AoC::Run<std::vector<std::uint32_t>, AoC::OutputData>(AoC::s_testInputData);
}
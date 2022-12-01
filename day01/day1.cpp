#include "day1.h"

#include <algorithm>
#include <fstream>
#include <numeric>
#include <sstream>
#include <vector>


namespace Day1
{
    bool ReadInputValues(std::vector<std::uint32_t>& caloriesPerElf)
    {
        static const char* inputFile{ "input.txt" };
        std::ifstream inputStream{ inputFile };

        bool readSucceeded{ inputStream.is_open() };
        if (readSucceeded)
        {
            FillCaloriesPerElfList(inputStream, caloriesPerElf);
            inputStream.close();
        }

        return readSucceeded;
    }

    void FillCaloriesPerElfList(std::istream& inputStream, std::vector<std::uint32_t>& caloriesPerElf)
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
    }

    std::uint32_t ComputeMaxCalories(const std::vector<std::uint32_t>& caloriesPerElf)
    {
        auto foundIt = std::max_element(caloriesPerElf.begin(), caloriesPerElf.end());
        return foundIt != caloriesPerElf.end() ? *foundIt : 0;
    }

    std::uint32_t ComputeTopNCalories(std::vector<std::uint32_t>& caloriesPerElf, std::uint32_t n)
    {
        auto endIt{ caloriesPerElf.begin() + n };
        std::nth_element(caloriesPerElf.begin(), endIt, caloriesPerElf.end(), std::greater());
        uint32_t total{ std::accumulate(caloriesPerElf.begin(), endIt, 0U) };
        return total;
    }
}
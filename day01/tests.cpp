#include "tests.h"

#include <sstream>
#include "day1.h"

namespace Day1::Tests
{
    static const char* ms_inputData{
        "1000\n"
        "2000\n"
        "3000\n"
        "\n"
        "4000\n"
        "\n"
        "5000\n"
        "6000\n"
        "\n"
        "7000\n"
        "8000\n"
        "9000\n"
        "\n"
        "10000"
    };

    static const std::uint32_t ms_Part1ExpectedResult{ 24000 };
    static const std::uint32_t ms_Part2ExpectedResult{ 45000 };

    bool ValidateTests()
    {
        bool didTestsPass{ true };

        std::istringstream input{ ms_inputData };
        std::vector<std::uint32_t> caloriesPerElf;
        FillCaloriesPerElfList(input, caloriesPerElf);
        std::uint32_t maxCalories{ Day1::ComputeMaxCalories(caloriesPerElf) };
        didTestsPass &= maxCalories == ms_Part1ExpectedResult;

        std::uint32_t total3Calories{ Day1::ComputeTopNCalories(caloriesPerElf, 3) };
        didTestsPass &= total3Calories == ms_Part2ExpectedResult;

        return didTestsPass;
    }
}
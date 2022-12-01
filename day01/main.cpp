#include <fmt/core.h>

#include "day1.h"
#include "tests.h"

void main()
{
    if (Day1::Tests::ValidateTests())
    {
        std::vector<std::uint32_t> caloriesPerElf;
        if (Day1::ReadInputValues(caloriesPerElf))
        {
            std::uint32_t maxCalories{ Day1::ComputeMaxCalories(caloriesPerElf) };
            std::uint32_t total3Calories{ Day1::ComputeTopNCalories(caloriesPerElf, 3) };

            fmt::print("Max Calories {}.\n", maxCalories);
            fmt::print("Top 3 Elves Calories {}.\n", total3Calories);
        }
        else
        {
            fmt::print("Failed to open input file.\n");
        }
    }
    else
    {
        fmt::print("Tests have failed.\n");
    }
}
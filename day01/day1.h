#pragma once

#include <cstdint>
#include <istream>
#include <vector>

namespace Day1
{
    bool ReadInputValues(std::vector<std::uint32_t>& caloriesPerElf);
    void FillCaloriesPerElfList(std::istream& inputStream, std::vector<std::uint32_t>& caloriesPerElf);
    std::uint32_t ComputeMaxCalories(const std::vector<std::uint32_t>& caloriesPerElf);
    std::uint32_t ComputeTopNCalories(std::vector<std::uint32_t>& caloriesPerElf, std::uint32_t n);
}
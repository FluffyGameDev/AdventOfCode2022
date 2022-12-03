#pragma once

#include <cstdint>

namespace AoC
{
    const char* s_testInputData
    {
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

    struct OutputData
    {
        std::uint32_t maxCalories;
        std::uint32_t total3Calories;
    };
}
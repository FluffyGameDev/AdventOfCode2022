#pragma once

#include "vector2.h"
#include <vector>

namespace AoC
{
    const char* s_testInputData
    {
        "Sabqponm\n"
        "abcryxxl\n"
        "accszExk\n"
        "acctuvwj\n"
        "abdefghi"
    };

    struct InputData
    {
        std::vector<u8> HeightMap{};
        Vector2 MapSize{};
        Vector2 StartPosition{};
        Vector2 EndPosition{};
    };

    struct OutputData
    {
        u32 MinStepsPart1{};
        u32 MinStepsPart2{};
    };
}
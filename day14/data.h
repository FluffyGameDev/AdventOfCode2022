#pragma once

#include "vector2.h"
#include <vector>

namespace AoC
{
    const char* s_testInputData
    {
        "498,4 -> 498,6 -> 496,6\n"
        "503,4 -> 502,4 -> 502,9 -> 494,9"
    };

    struct RockPath
    {
        std::vector<Vector2> Path;
    };

    struct InputData
    {
        Vector2 MinBound{};
        Vector2 MaxBound{};
        std::vector<RockPath> Paths;
    };

    struct OutputData
    {
        u32 MaxSandUnits{};
        u32 MaxSandUnitsWithFloor{};
    };
}
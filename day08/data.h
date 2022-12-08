#pragma once

#include "types.h"
#include <vector>

namespace AoC
{
    const char* s_testInputData
    {
        "30373\n"
        "25512\n"
        "65332\n"
        "33549\n"
        "35390"
    };

    enum class VisibilityFlags : u8
    {
        NotVisible = 0,
        FromTop = 0x01,
        FromBottom = 0x02,
        FromLeft = 0x04,
        FromRight = 0x08,
        FromAllSides = 0x0f,
    };

    struct VisibilityData
    {
        VisibilityFlags Flags{ VisibilityFlags::FromAllSides };
        u8 HighestTop{};
        u8 HighestBottom{};
        u8 HighestLeft{};
        u8 HighestRight{};
    };

    struct ScenicData
    {
        u32 TopScore{};
        u32 BottomScore{};
        u32 LeftScore{};
        u32 RightScore{};
    };

    struct InputData
    {
        u32 MapWidth;
        std::vector<u8> TreeHeightMap;
    };

    struct OutputData
    {
        std::vector<VisibilityData> VisibilityMap;
        u32 VisibleTrees;
        u64 BestScenicScore;
    };
}
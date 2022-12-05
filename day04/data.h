#pragma once

#include <vector>

#include "types.h"
#include "range.h"

namespace AoC
{
    const char* s_testInputData
    {
        "2-4,6-8\n"
        "2-3,4-5\n"
        "5-7,7-9\n"
        "2-8,3-7\n"
        "6-6,4-6\n"
        "2-6,4-8"
    };

    struct RangePair
    {
        Range Range1;
        Range Range2;
    };

    struct InputData
    {
        std::vector<RangePair> Ranges;
    };

    struct OutputData
    {
        u32 ContainedRangeCount;
        u32 OverlappingRangeCount;
    };
}
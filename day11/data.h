#pragma once

#include "types.h"
#include <vector>

namespace AoC
{
    const char* s_testInputData { "" };

    struct MonkeyData
    {
        std::vector<u32> StartingItems{};
        std::vector<u8> BytecodeBuffer{};
        u32 DivisibilityCondition{};
        u32 ConditionTrueMonkey{};
        u32 ConditionFalseMonkey{};
    };

    struct InputData
    {
        std::vector<MonkeyData> Monkeys;
    };

    struct OutputData
    {
        std::vector<u32> InspectionCounters;
        u32 Part1Result;
    };
}
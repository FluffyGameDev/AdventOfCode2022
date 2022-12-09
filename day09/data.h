#pragma once

#include "types.h"
#include <set>
#include <vector>

namespace AoC
{
    const char* s_testInputData
    {
        "R 4\n"
        "U 4\n"
        "L 3\n"
        "D 1\n"
        "R 4\n"
        "D 1\n"
        "L 5\n"
        "R 2"
    };

    struct Command
    {
        char CommandId;
        u32 ExecutionCount;
    };

    struct InputData
    {
        std::vector<Command> CommandSequence;
    };

    struct OutputData
    {
        std::set<u64> VisitedCellsPart1;
        std::set<u64> VisitedCellsPart2;
    };
}
#pragma once

#include <string>
#include <vector>

#include "types.h"

namespace AoC
{
    const char* s_testInputData
    {
        "    [D]    \n"
        "[N] [C]    \n"
        "[Z] [M] [P]\n"
        " 1   2   3 \n"
        "\n"
        "move 1 from 2 to 1\n"
        "move 3 from 1 to 3\n"
        "move 2 from 2 to 1\n"
        "move 1 from 1 to 2"
    };

    struct Command
    {
        u32 Quantity;
        u32 SourceId;
        u32 DestinationId;
    };

    struct InputData
    {
        std::vector<std::vector<char>> Stacks;
        std::vector<Command> Commands;
    };

    struct OutputData
    {
        std::string Part1TopLine;
        std::string Part2TopLine;
    };
}
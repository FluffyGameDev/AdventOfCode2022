#pragma once

#include <string>
#include <vector>

#include "types.h"

namespace AoC
{
    const char* s_testInputData
    {
        "vJrwpWtwJgWrhcsFMMfFFhFp\n"
        "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL\n"
        "PmmdzqPrVvPwwTWBwg\n"
        "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn\n"
        "ttgJtRGJQctTZtZT\n"
        "CrZsJsPPZsGzwwsLwLmpwMDw"
    };

    struct InputData
    {
        std::vector<std::string> BagContents;
    };

    struct OutputData
    {
        u32 TotalItemPriorities;
        u32 TotalBadgePriorities;
    };
}
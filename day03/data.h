#pragma once

#include <string>
#include <vector>

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
        std::uint32_t TotalItemPriorities;
        std::uint32_t TotalBadgePriorities;
    };
}
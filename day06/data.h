#pragma once

#include "types.h"
#include <string>

namespace AoC
{
    const char* s_testInputData
    {
        "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw"
    };

    struct InputData
    {
        std::string DataStream;
    };

    struct OutputData
    {
        u32 FirstUnique4CharPosition;
        u32 FirstUnique14CharPosition;
    };
}
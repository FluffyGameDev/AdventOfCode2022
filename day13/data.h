#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace AoC
{
    const char* s_testInputData
    {
        "[1,1,3,1,1]\n"
        "[1,1,5,1,1]\n"
        "\n"
        "[[1],[2,3,4]]\n"
        "[[1],4]\n"
        "\n"
        "[9]\n"
        "[[8,7,6]]\n"
        "\n"
        "[[4,4],4,4]\n"
        "[[4,4],4,4,4]\n"
        "\n"
        "[7,7,7,7]\n"
        "[7,7,7]\n"
        "\n"
        "[]\n"
        "[3]\n"
        "\n"
        "[[[]]]\n"
        "[[]]\n"
        "\n"
        "[1,[2,[3,[4,[5,6,7]]]],8,9]\n"
        "[1,[2,[3,[4,[5,6,0]]]],8,9]"
    };

    struct RawPacketPairs
    {
        std::string RawPacket1{};
        std::string RawPacket2{};
    };

    struct InputData
    {
        std::vector<RawPacketPairs> RawPacketPairs;
    };

    struct OutputData
    {
        u32 SumRightPairIndices{};
        u32 DividerIndicesProduct{};
    };
}
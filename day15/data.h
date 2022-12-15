#pragma once

#include "vector2.h"
#include <vector>

namespace AoC
{
    const char* s_testInputData
    {
        "Sensor at x=2, y=18: closest beacon is at x=-2, y=15\n"
        "Sensor at x=9, y=16: closest beacon is at x=10, y=16\n"
        "Sensor at x=13, y=2: closest beacon is at x=15, y=3\n"
        "Sensor at x=12, y=14: closest beacon is at x=10, y=16\n"
        "Sensor at x=10, y=20: closest beacon is at x=10, y=16\n"
        "Sensor at x=14, y=17: closest beacon is at x=10, y=16\n"
        "Sensor at x=8, y=7: closest beacon is at x=2, y=10\n"
        "Sensor at x=2, y=0: closest beacon is at x=2, y=10\n"
        "Sensor at x=0, y=11: closest beacon is at x=2, y=10\n"
        "Sensor at x=20, y=14: closest beacon is at x=25, y=17\n"
        "Sensor at x=17, y=20: closest beacon is at x=21, y=22\n"
        "Sensor at x=16, y=7: closest beacon is at x=15, y=3\n"
        "Sensor at x=14, y=3: closest beacon is at x=15, y=3\n"
        "Sensor at x=20, y=1: closest beacon is at x=15, y=3"
    };

    struct SensorData
    {
        Vector2 SensorPosition;
        Vector2 BeaconPosition;
    };

    struct InputData
    {
        s32 InspectedRow{};
        std::vector<SensorData> Sensors{};
    };

    struct OutputData
    {
        u32 BeaconlessPositionCountPart1{};
    };
}
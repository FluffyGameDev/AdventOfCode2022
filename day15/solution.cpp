#include "data.h"
#include "aoc.h"

#include "range.h"
#include <numeric>

namespace AoC
{
    namespace Internal
    {
        s32 ComputeSensorManhattanDistance(const SensorData& sensor)
        {
            return std::abs(sensor.BeaconPosition.x - sensor.SensorPosition.x) +
                   std::abs(sensor.BeaconPosition.y - sensor.SensorPosition.y);
        }

        void ComputeInspectedRowRanges(const InputData& input, std::vector<Range>& sensorRangesOnRow)
        {
            for (const SensorData& sensor : input.Sensors)
            {
                s32 sensorDistance{ ComputeSensorManhattanDistance(sensor) };
                s32 distanceToRow{ std::abs(sensor.SensorPosition.y - input.InspectedRow) };
                if (distanceToRow <= sensorDistance)
                {
                    s32 rowHalfWidth{ sensorDistance - distanceToRow };
                    Range& range{ sensorRangesOnRow.emplace_back() };
                    range.Min = sensor.SensorPosition.x - rowHalfWidth;
                    range.Max = sensor.SensorPosition.x + rowHalfWidth;
                }
            }
        }

        void ComputeBeaconPositions(const std::vector<SensorData>& sensors, std::vector<Vector2>& beacons)
        {
            for (const SensorData& sensor : sensors)
            {
                if (!std::any_of(beacons.begin(), beacons.end(), [&sensor](const Vector2& beacon)
                    { return beacon.x == sensor.BeaconPosition.x && beacon.y == sensor.BeaconPosition.y; }))
                {
                    beacons.push_back(sensor.BeaconPosition);
                }
            }
        }

        u32 ComputePositionCountInRanges(const std::vector<Range>& ranges)
        {
            auto computeRangeWidth{ [](u32 total, const Range& range) { return total + range.Max - range.Min + 1; } };
            return std::accumulate(ranges.begin(), ranges.end(), 0U, computeRangeWidth);
        }

        u32 ComputeBeaconsOnRowCount(const std::vector<Vector2>& beacons, s32 row)
        {
            auto isOnRow{ [row](const Vector2& beacon) { return beacon.y == row; } };
            return (u32)std::count_if(beacons.begin(), beacons.end(), isOnRow);
        }

        bool MergeRanges(Range& target, const Range& range)
        {
            bool didMerge{};
            if (target.DoesRangeOverlap(range))
            {
                target.Min = std::min(target.Min, range.Min);
                target.Max = std::max(target.Max, range.Max);
                didMerge = true;
            }
            return didMerge;
        }

        void MergeRanges(std::vector<Range>& ranges)
        {
            for (s32 index1 = (s32)ranges.size() - 1; index1 >= 0; --index1)
            {
                bool hasMerged{ false };
                for (s32 index2 = index1 - 1; index2 >= 0; --index2)
                {
                    hasMerged |= MergeRanges(ranges[index2], ranges[index1]);
                }

                if (hasMerged)
                {
                    ranges.erase(std::remove(ranges.begin(), ranges.end(), ranges[index1]));
                }
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        std::string line{};
        while (std::getline(inputStream, line))
        {
            SensorData& sensor{ inputData.Sensors.emplace_back() };
            sscanf_s(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
                    &sensor.SensorPosition.x, &sensor.SensorPosition.y,
                    &sensor.BeaconPosition.x, &sensor.BeaconPosition.y);
        }

        inputData.InspectedRow = (step == AoCStep::Test ? 10 : 2000000);

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        std::vector<Range> sensorRangesOnRow{};
        Internal::ComputeInspectedRowRanges(input, sensorRangesOnRow);
        Internal::MergeRanges(sensorRangesOnRow);

        std::vector<Vector2> beacons{};
        Internal::ComputeBeaconPositions(input.Sensors, beacons);
        u32 beaconsOnRowCount{ Internal::ComputeBeaconsOnRowCount(beacons, input.InspectedRow) };

        output.BeaconlessPositionCountPart1 = Internal::ComputePositionCountInRanges(sensorRangesOnRow) - beaconsOnRowCount;
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.BeaconlessPositionCountPart1 == 26;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Beaconless Position Count Part 1: {}\n", output.BeaconlessPositionCountPart1);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
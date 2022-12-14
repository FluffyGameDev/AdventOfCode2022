#include "data.h"
#include "aoc.h"

#include "mathutils.h"

#include <chrono>
#include <thread>

namespace AoC
{
    namespace Internal
    {
        static constexpr Vector2 SandOrigin{ 500, 0 };

        enum class CaveCell : u8
        {
            Air,
            Rock,
            Sand,
            SandSource
        };

        void AddPointToBounds(const Vector2& point, Vector2& minBound, Vector2& maxBound)
        {
            maxBound.x = std::max(maxBound.x, point.x);
            maxBound.y = std::max(maxBound.y, point.y);
            minBound.x = std::min(minBound.x, point.x);
            minBound.y = std::min(minBound.y, point.y);
        }

        Vector2 ConvertAbsoluteToRelative(const Vector2& point, const Vector2& worldOffset)
        {
            return { point.x - worldOffset.x, point.y - worldOffset.y };
        }

        Vector2 ConvertRelativeToAbsolute(const Vector2& point, const Vector2& worldOffset)
        {
            return { point.x + worldOffset.x, point.y + worldOffset.y };
        }

        s32 ConvertPositionToIndex(s32 x, s32 y, const Vector2& caveDimensions)
        {
            return x + y * caveDimensions.x;
        }

        s32 ConvertPositionToIndex(const Vector2& point, const Vector2& caveDimensions)
        {
            return ConvertPositionToIndex(point.x, point.y, caveDimensions);
        }

        void DrawPointOnMap(const Vector2& point, CaveCell cellValue, std::vector<CaveCell>& caveMap, const Vector2& caveDimensions)
        {
            s32 sandOriginIndex{ ConvertPositionToIndex(point, caveDimensions) };
            caveMap[sandOriginIndex] = cellValue;
        }

        void DrawLineOnMap(const Vector2& lineStart, const Vector2& lineEnd, std::vector<CaveCell>& caveMap, const Vector2& caveDimensions)
        {
            Vector2 direction{};
            if (lineStart.x == lineEnd.x) // Vertical Line
            {
                direction.y = Math::Sign(lineEnd.y - lineStart.y);
            }
            else if (lineStart.y == lineEnd.y) // Horizontal Line
            {
                direction.x = Math::Sign(lineEnd.x - lineStart.x);
            }
            else
            {
                FMT_ASSERT(false, "Unhandled case.");
            }

            Vector2 currentCell{ lineStart };
            while (currentCell.x != lineEnd.x || currentCell.y != lineEnd.y)
            {
                DrawPointOnMap(currentCell, CaveCell::Rock, caveMap, caveDimensions);
                currentCell.x += direction.x;
                currentCell.y += direction.y;
            }
            DrawPointOnMap(lineEnd, CaveCell::Rock, caveMap, caveDimensions);
        }

        void GenerateCaveMap(const std::vector<RockPath>& paths, const Vector2& worldMin, const Vector2& worldMax, std::vector<CaveCell>& caveMap, Vector2& caveDimensions)
        {
            caveDimensions = { worldMax.x - worldMin.x + 1, worldMax.y - worldMin.y + 1 };
            caveMap.resize((u64)caveDimensions.x * caveDimensions.y, CaveCell::Air);
            for (const RockPath& path : paths)
            {
                for (u32 i = 1; i < path.Path.size(); ++i)
                {
                    Vector2 lineStart{ ConvertAbsoluteToRelative(path.Path[i - 1], worldMin) };
                    Vector2 lineEnd{ ConvertAbsoluteToRelative(path.Path[i], worldMin) };
                    DrawLineOnMap(lineStart, lineEnd, caveMap, caveDimensions);
                }
            }

            Vector2 relativeSandOrigin{ ConvertAbsoluteToRelative(SandOrigin, worldMin) };
            DrawPointOnMap(relativeSandOrigin, CaveCell::SandSource, caveMap, caveDimensions);
        }

        bool IsPositionValid(const Vector2& point, const Vector2& min, const Vector2& max)
        {
            return point.x >= min.x && point.y >= min.y
                && point.x <= max.x && point.y <= max.y;
        }

        bool SimulateSandUnitStep(const std::vector<CaveCell>& caveMap, const Vector2& caveDimensions, Vector2& sandPosition)
        {
            s32 index{ ConvertPositionToIndex(sandPosition.x, sandPosition.y + 1, caveDimensions) };
            if (sandPosition.y >= caveDimensions.y - 1 || caveMap[index] == CaveCell::Air)
            {
                ++sandPosition.y;
                return true;
            }

            index = ConvertPositionToIndex(sandPosition.x - 1, sandPosition.y + 1, caveDimensions);
            if (sandPosition.x <= 0 || caveMap[index] == CaveCell::Air)
            {
                --sandPosition.x;
                ++sandPosition.y;
                return true;
            }

            index = ConvertPositionToIndex(sandPosition.x + 1, sandPosition.y + 1, caveDimensions);
            if (sandPosition.x >= caveDimensions.x - 1 || caveMap[index] == CaveCell::Air)
            {
                ++sandPosition.x;
                ++sandPosition.y;
                return true;
            }

            return false;
        }

        void SimulateSandUnit(const Vector2& worldOffset, const std::vector<CaveCell>& caveMap, const Vector2& caveDimensions, Vector2& sandPosition)
        {
            Vector2 position{ ConvertAbsoluteToRelative(sandPosition, worldOffset) };
            Vector2 zeroVector{};

            while (IsPositionValid(position, zeroVector, caveDimensions) && SimulateSandUnitStep(caveMap, caveDimensions, position))
            {
            }

            sandPosition = ConvertRelativeToAbsolute(position, worldOffset);
        }

        u32 SimulateSandFlowUntilImpossible(const Vector2& worldMin, const Vector2& worldMax, std::vector<CaveCell>& caveMap, const Vector2& caveDimensions)
        {
            Vector2 sandOriginRelativePosition{ ConvertAbsoluteToRelative(SandOrigin, worldMin) };
            s32 sandOriginRelativeIndex{ ConvertPositionToIndex(sandOriginRelativePosition, caveDimensions) };

            u32 createSandUnits{};
            Vector2 sandPosition{};
            while (true)
            {
                sandPosition = SandOrigin;
                SimulateSandUnit(worldMin, caveMap, caveDimensions, sandPosition);

                if (IsPositionValid(sandPosition, worldMin, worldMax))
                {
                    Vector2 relationPosition{ ConvertAbsoluteToRelative(sandPosition, worldMin) };
                    DrawPointOnMap(relationPosition, CaveCell::Sand, caveMap, caveDimensions);
                }
                else
                {
                    break;
                }

                ++createSandUnits;

                if (caveMap[sandOriginRelativeIndex] != CaveCell::SandSource)
                {
                    break;
                }
            }
            return createSandUnits;
        }

        void PrintCaveMap(const std::vector<CaveCell>& caveMap, const Vector2& caveDimensions)
        {
            for (s32 j = 0; j < caveDimensions.y; ++j)
            {
                for (s32 i = 0; i < caveDimensions.x; ++i)
                {
                    s32 index{ ConvertPositionToIndex(i, j, caveDimensions) };
                    CaveCell cell{ caveMap[index] };
                    char displayedChar{};
                    switch (cell)
                    {
                        case AoC::Internal::CaveCell::Air: { displayedChar = '.'; break; }
                        case AoC::Internal::CaveCell::Rock: { displayedChar = '#'; break; }
                        case AoC::Internal::CaveCell::Sand: { displayedChar = 'o'; break; }
                        case AoC::Internal::CaveCell::SandSource: { displayedChar = '+'; break; }
                    }

                    fmt::print("{}", displayedChar);
                }
                fmt::print("\n");
            }
            fmt::print("\n");
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        std::string line{};
        std::string coordPair{};

        inputData.MaxBound = { std::numeric_limits<s32>::min(), std::numeric_limits<s32>::min() };
        inputData.MinBound = { std::numeric_limits<s32>::max(), std::numeric_limits<s32>::max() };

        while (std::getline(inputStream, line))
        {
            std::stringstream lineStream{ line };
            RockPath& newPath{ inputData.Paths.emplace_back() };

            while (lineStream >> coordPair)
            {
                lineStream.ignore(4); // size of " -> ";

                Vector2& pathPoint{ newPath.Path.emplace_back() };
                sscanf_s(coordPair.c_str(), "%d,%d", &pathPoint.x, &pathPoint.y);
                Internal::AddPointToBounds(pathPoint, inputData.MinBound, inputData.MaxBound);
            }
        }

        Internal::AddPointToBounds(Internal::SandOrigin, inputData.MinBound, inputData.MaxBound);

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        Vector2 caveDimensions{};
        std::vector<Internal::CaveCell> caveMap{};
        Internal::GenerateCaveMap(input.Paths, input.MinBound, input.MaxBound, caveMap, caveDimensions);
        output.MaxSandUnits = Internal::SimulateSandFlowUntilImpossible(input.MinBound, input.MaxBound, caveMap, caveDimensions);


        Vector2 floorPointLeft{ input.MinBound.x - input.MaxBound.y, input.MaxBound.y + 2 };
        Vector2 floorPointRight{ input.MaxBound.x + input.MaxBound.y , input.MaxBound.y + 2 };
        std::vector<RockPath> extendedPaths{ input.Paths };
        RockPath& bottomLine{ extendedPaths.emplace_back() };
        bottomLine.Path.push_back(floorPointLeft);
        bottomLine.Path.push_back(floorPointRight);

        Vector2 extendedMinBound{ input.MinBound };
        Vector2 extendedMaxBound{ input.MaxBound };
        Internal::AddPointToBounds(floorPointLeft, extendedMinBound, extendedMaxBound);
        Internal::AddPointToBounds(floorPointRight, extendedMinBound, extendedMaxBound);

        Vector2 extendedCaveDimensions{};
        std::vector<Internal::CaveCell> extendedCaveMap{};
        Internal::GenerateCaveMap(extendedPaths, extendedMinBound, extendedMaxBound, extendedCaveMap, extendedCaveDimensions);
        output.MaxSandUnitsWithFloor = Internal::SimulateSandFlowUntilImpossible(extendedMinBound, extendedMaxBound, extendedCaveMap, extendedCaveDimensions);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.MaxSandUnits == 24;
        didTestsPass &= output.MaxSandUnitsWithFloor == 93;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Max Sand Units: {}\n", output.MaxSandUnits);
        fmt::print("Max Sand Units with Floor: {}\n", output.MaxSandUnitsWithFloor);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
#include "data.h"
#include "aoc.h"

#include <queue>
#include <string>

namespace AoC
{
    namespace Internal
    {
        struct Cell
        {
            s32 distanceToEnd{ -1 };
            u8 height{};
            const Cell* PreviousCell{};
        };

        struct CellVisit
        {
            const Cell* PreviousCell{};
            Vector2 VisitedCellCoord{};
        };

        u32 CoordToIndex(const Vector2& mapSize, const Vector2& coord)
        {
            return (u32)(coord.x + coord.y * mapSize.x);
        }

        void TryVisitCell(const InputData& input, const CellVisit& visit, std::vector<Cell>& cells, std::queue<CellVisit>& cellsToVisit)
        {
            s32 x{ visit.VisitedCellCoord.x };
            s32 y{ visit.VisitedCellCoord.y };
            if (x >= 0 && y >= 0 && x < input.MapSize.x && y < input.MapSize.y)
            {
                u32 cellIndex{ CoordToIndex(input.MapSize, visit.VisitedCellCoord) };
                Cell* currentCell{ &cells[cellIndex] };
                if (currentCell->distanceToEnd < 0)
                {
                    u8 previousHeight{ visit.PreviousCell != nullptr ? visit.PreviousCell->height : (u8)0 };
                    u8 currentHeight{ input.HeightMap[cellIndex] };
                    if (currentHeight >= previousHeight - 1)
                    {
                        currentCell->height = currentHeight;
                        currentCell->distanceToEnd = visit.PreviousCell != nullptr ? visit.PreviousCell->distanceToEnd + 1 : 0;
                        currentCell->PreviousCell = visit.PreviousCell;

                        cellsToVisit.push({ currentCell, { x - 1, y } });
                        cellsToVisit.push({ currentCell, { x + 1, y } });
                        cellsToVisit.push({ currentCell, { x, y - 1 } });
                        cellsToVisit.push({ currentCell, { x, y + 1 } });
                    }
                }
            }
        }

        void ComputePath(const InputData& input, std::vector<Cell>& cells)
        {
            std::queue<CellVisit> cellsToVisit{};
            cellsToVisit.push({ nullptr, input.EndPosition });

            while (!cellsToVisit.empty())
            {
                CellVisit visit{ cellsToVisit.front() };
                cellsToVisit.pop();
                TryVisitCell(input, visit, cells, cellsToVisit);
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        std::string line{};
        while (inputStream >> line)
        {
            s32 x{};
            for (char cell : line)
            {
                if (cell == 'S')
                {
                    inputData.StartPosition = { x, inputData.MapSize.y };
                    inputData.HeightMap.push_back(0);
                }
                else if (cell == 'E')
                {
                    inputData.EndPosition = { x, inputData.MapSize.y };
                    inputData.HeightMap.push_back(25);
                }
                else
                {
                    inputData.HeightMap.push_back((u8)(cell - 'a'));
                }

                ++x;
            }

            inputData.MapSize.x = (u32)line.size();
            ++inputData.MapSize.y;
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        std::vector<Internal::Cell> cells(input.HeightMap.size());
        Internal::ComputePath(input, cells);
        output.MinStepsPart1 = cells[Internal::CoordToIndex(input.MapSize, input.StartPosition)].distanceToEnd;

        output.MinStepsPart2 = std::numeric_limits<u32>::max();
        for (const Internal::Cell& cell : cells)
        {
            if (cell.height == 0 && cell.distanceToEnd < output.MinStepsPart2)
            {
                output.MinStepsPart2 = cell.distanceToEnd;
            }
        }
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.MinStepsPart1 == 31;
        didTestsPass &= output.MinStepsPart2 == 29;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Min Steps Part 1: {}\n", output.MinStepsPart1);
        fmt::print("Min Steps Part 2: {}\n", output.MinStepsPart2);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
#include "data.h"
#include "aoc.h"

#include "mathutils.h"
#include "vector2.h"

namespace AoC
{
    namespace Internal
    {
        void MoveTowards(const Vector2& dst, Vector2& pos)
        {
            if (Vector2::SquareDistance(pos, dst) >= 4)
            {
                pos.x += Math::Sign(dst.x - pos.x);
                pos.y += Math::Sign(dst.y - pos.y);
            }
        }

        void ExecuteCommand(char commandId, std::vector<Vector2>& rope)
        {
            switch (commandId)
            {
                case 'U': ++rope.front().y; break;
                case 'D': --rope.front().y; break;
                case 'L': --rope.front().x; break;
                case 'R': ++rope.front().x; break;
            }

            for (u32 i = 1; i < rope.size(); ++i)
            {
                MoveTowards(rope[i - 1], rope[i]);
            }
        }

        u64 ConvertPositionToCellId(const Vector2& v)
        {
            return *reinterpret_cast<const u64*>(&v);
        }

        void ExecuteCommandSequence(const std::vector<Command>& sequence, std::vector<Vector2>& rope, std::set<u64>& visitedCells)
        {
            for (const Command& command : sequence)
            {
                for (u32 i = 0; i < command.ExecutionCount; ++i)
                {
                    ExecuteCommand(command.CommandId, rope);
                    visitedCells.insert(ConvertPositionToCellId(rope.back()));
                }
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        Command nextCommand;
        while (inputStream >> nextCommand.CommandId >> nextCommand.ExecutionCount)
        {
            inputData.CommandSequence.push_back(nextCommand);
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        std::vector<Vector2> rope(2, { 0, 0 });
        Internal::ExecuteCommandSequence(input.CommandSequence, rope, output.VisitedCellsPart1);

        std::vector<Vector2> ropeLength10(10, { 0, 0 });
        Internal::ExecuteCommandSequence(input.CommandSequence, ropeLength10, output.VisitedCellsPart2);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        static constexpr u32 part1TestResult{ 13 };
        didTestsPass &= output.VisitedCellsPart1.size() == part1TestResult;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Visited Cells Part 1: {}\n", output.VisitedCellsPart1.size());
        fmt::print("Visited Cells Part 2: {}\n", output.VisitedCellsPart2.size());
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
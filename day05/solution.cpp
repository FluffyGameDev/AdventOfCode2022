#include "data.h"
#include "aoc.h"

namespace AoC
{
    namespace Internal
    {
        void SimulateCommands(std::vector<std::vector<char>>& stacks, const std::vector<Command>& commands, bool reverseMovedItems)
        {
            for (const Command& command : commands)
            {
                auto& sourceStack{ stacks[command.SourceId - 1] };
                auto& destinationStack{ stacks[command.DestinationId - 1] };

                destinationStack.resize(destinationStack.size() + command.Quantity);
                std::copy(sourceStack.end() - command.Quantity, sourceStack.end(), destinationStack.end() - command.Quantity);
                sourceStack.resize(sourceStack.size() - command.Quantity);

                if (reverseMovedItems)
                {
                    std::reverse(destinationStack.end() - command.Quantity, destinationStack.end());
                }
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData)
    {
        char ignore{};
        char crate{};
        std::string currentLine{};
        do
        {
            std::getline(inputStream, currentLine);
            std::stringstream lineStream{ currentLine };

            u32 stackIndex{ 0 };
            while (!lineStream.eof())
            {
                lineStream.read(&ignore, 1);
                lineStream.read(&crate, 1);
                lineStream.read(&ignore, 1);
                lineStream.read(&ignore, 1);

                if (std::isalpha(crate))
                {
                    if (stackIndex >= inputData.Stacks.size())
                    {
                        inputData.Stacks.resize((std::uint64_t)stackIndex + 1);
                    }

                    inputData.Stacks[stackIndex].push_back(crate);
                }

                ++stackIndex;
            }

        } while (currentLine.size() > 0);

        for (auto& s : inputData.Stacks)
        {
            std::reverse(s.begin(), s.end());
        }

        Command readCommand{};
        while (std::getline(inputStream, currentLine))
        {
            sscanf_s(currentLine.c_str(), "move %d from %d to %d", &readCommand.Quantity, &readCommand.SourceId, &readCommand.DestinationId);
            inputData.Commands.push_back(readCommand);
        }

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        auto getTopItem{ [](const std::vector<char>& s) { return s.back(); } };

        std::vector<std::vector<char>> stacksCopyPart1(input.Stacks);
        Internal::SimulateCommands(stacksCopyPart1, input.Commands, true);
        output.Part1TopLine.resize(stacksCopyPart1.size());
        std::transform(stacksCopyPart1.begin(), stacksCopyPart1.end(), output.Part1TopLine.begin(), getTopItem);

        std::vector<std::vector<char>> stacksCopyPart2(input.Stacks);
        Internal::SimulateCommands(stacksCopyPart2, input.Commands, false);
        output.Part2TopLine.resize(stacksCopyPart2.size());
        std::transform(stacksCopyPart2.begin(), stacksCopyPart2.end(), output.Part2TopLine.begin(), getTopItem);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        constexpr char* part1ExpectedResult = "CMZ";
        didTestsPass &= output.Part1TopLine == part1ExpectedResult;

        constexpr char* part2ExpectedResult = "MCD";
        didTestsPass &= output.Part2TopLine == part2ExpectedResult;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Part 1 Top Line: {}\n", output.Part1TopLine);
        fmt::print("Part 2 Top Line: {}\n", output.Part2TopLine);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
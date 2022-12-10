#include "data.h"
#include "aoc.h"

#include <numeric>
#include <string>
#include <unordered_map>

namespace AoC
{
    namespace Internal
    {
        struct Context
        {
            s32 RegisterValue{ 1 };
            s32 CycleCount{ 0 };
        };

        static constexpr s32 SpriteWidth{ 3 };
        static constexpr s32 ScreenWidth{ 40 };
        static constexpr s32 ScreenHeight{ 6 };
        static constexpr char SpritePixel{ (char)219 };

        void CheckCycleMeasure(const Context& context, OutputData& output)
        {
            if ((context.CycleCount - ScreenWidth / 2) % ScreenWidth == 0)
            {
                MeasureData measure{ context.CycleCount, context.RegisterValue };
                output.Measures.push_back(measure);
            }
        }

        void PrintPixel(Context& context, OutputData& output)
        {
            s32 pixelX{ context.CycleCount % ScreenWidth + 1 };
            if (pixelX >= context.RegisterValue && pixelX < context.RegisterValue + SpriteWidth)
            output.ScreenBuffer[context.CycleCount] = SpritePixel;
        }

        void IncrementCycle(Context& context, OutputData& output)
        {
            PrintPixel(context, output);
            ++context.CycleCount;
            CheckCycleMeasure(context, output);
        }

        void ExecuteNoopCommand(Context& context, OutputData& output)
        {
            IncrementCycle(context, output);
        }

        void ExecuteAddXCommand(s32 argument, Context& context, OutputData& output)
        {
            IncrementCycle(context, output);
            IncrementCycle(context, output);
            context.RegisterValue += argument;
        }

        void SimulateCommands(const InputData& input, OutputData& output)
        {
            Context context{};
            output.ScreenBuffer.resize(ScreenWidth * ScreenHeight, '.');
            for (const Command& currentCommand : input.Commands)
            {
                switch (currentCommand.Id)
                {
                    case CommandId::Noop: ExecuteNoopCommand(context, output); break;
                    case CommandId::AddX: ExecuteAddXCommand(currentCommand.Argument, context, output); break;
                }
            }
        }

        void PrintScreen(const std::string& screenBuffer)
        {
            for (u32 j = 0; j < ScreenHeight; ++j)
            {
                for (u32 i = 0; i < ScreenWidth; ++i)
                {
                    fmt::print("{}", screenBuffer[i + j * ScreenWidth]);
                }
                fmt::print("\n");
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData)
    {
        struct CommandDescription
        {
            CommandId Id;
            bool HasArgument;
        };
        static const std::unordered_map<std::string, CommandDescription> commandDescriptions
        {
            { "noop", { CommandId::Noop, false } },
            { "addx", { CommandId::AddX, true } }
        };

        std::string commandName{};
        Command command{};
        while (inputStream >> commandName)
        {
            const CommandDescription& description{ commandDescriptions.at(commandName) };
            command.Id = description.Id;
            if (description.HasArgument)
            {
                inputStream >> command.Argument;
            }
            inputData.Commands.push_back(command);
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        Internal::SimulateCommands(input, output);
        auto computeSignalStrength{ [](s32 total, const MeasureData& data)
            { return total + data.CycleNumber * data.RegisterValue; } };
        output.SignalStrengthsSum = std::accumulate(output.Measures.begin(), output.Measures.end(), 0, computeSignalStrength);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.Measures.size() == 6;
        if (didTestsPass)
        {
            didTestsPass &= output.Measures[0].RegisterValue == 21;
            didTestsPass &= output.Measures[1].RegisterValue == 19;
            didTestsPass &= output.Measures[2].RegisterValue == 18;
            didTestsPass &= output.Measures[3].RegisterValue == 21;
            didTestsPass &= output.Measures[4].RegisterValue == 16;
            didTestsPass &= output.Measures[5].RegisterValue == 18;
            didTestsPass &= output.SignalStrengthsSum == 13140;
        }

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Signal Strengths Sum: {}\n\n", output.SignalStrengthsSum);
        Internal::PrintScreen(output.ScreenBuffer);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
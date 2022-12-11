#include "data.h"
#include "aoc.h"

#include "bytecode.h"
#include <numeric>

namespace AoC
{
    namespace Internal
    {
        struct MonkeySimulationData
        {
            std::vector<u32> HeldItems{};
            u32 ItemInspectionCount{};
        };

        void InitMonkeySimulationData(const std::vector<MonkeyData>& monkeys, std::vector<MonkeySimulationData>& monkeySimulationData)
        {
            monkeySimulationData.resize(monkeys.size());
            for (u32 i = 0; i < monkeys.size(); ++i)
            {
                monkeySimulationData[i].HeldItems = monkeys[i].StartingItems;
            }
        }

        void SimulateMonkey(const std::vector<MonkeyData>& monkeys, std::vector<MonkeySimulationData>& monkeySimulationData,
                BytecodeInterpreter& interpreter, const MonkeyData& currentMonkeyData, MonkeySimulationData& currentMonkey)
        {
            currentMonkey.ItemInspectionCount += (u32)currentMonkey.HeldItems.size();

            for (u32 itemValue : currentMonkey.HeldItems)
            {
                interpreter.SetRegisterA(itemValue);
                interpreter.RunBytecode(currentMonkeyData.BytecodeBuffer);
                itemValue = interpreter.GetRegisterA() / 3;

                u32 targetMonkeyId{ (itemValue % currentMonkeyData.DivisibilityCondition == 0) ?
                    currentMonkeyData.ConditionTrueMonkey : currentMonkeyData.ConditionFalseMonkey };
                monkeySimulationData[targetMonkeyId].HeldItems.push_back(itemValue);
            }

            currentMonkey.HeldItems.clear();
        }

        void SimulateNRounds(const std::vector<MonkeyData>& monkeys, std::vector<MonkeySimulationData>& monkeySimulationData, u32 roundCount)
        {
            BytecodeInterpreter interpreter{};
            for (u32 i = 0; i < roundCount; ++i)
            {
                for (u32 monkeyId = 0; monkeyId < monkeySimulationData.size(); ++monkeyId)
                {
                    const MonkeyData& currentMonkeyData{ monkeys[monkeyId] };
                    MonkeySimulationData& currentMonkey{ monkeySimulationData[monkeyId] };
                    SimulateMonkey(monkeys, monkeySimulationData, interpreter, currentMonkeyData, currentMonkey);
                }
            }
        }

        u32 ComputeTopNMonkeyBusiness(const std::vector<u32>& inspectionCounters, u32 monkeyCount)
        {
            std::vector<u32> countersCopy{ inspectionCounters };
            auto nthIterator{ countersCopy.begin() + monkeyCount };
            std::nth_element(countersCopy.begin(), nthIterator, countersCopy.end(), std::greater());
            return std::accumulate(countersCopy.begin(), nthIterator, 1U,
                [](u32 total, u32 value) { return total * value; });
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        if (step == AoCStep::Test)
        {
            inputData.Monkeys =
            {
                {
                    { 79, 98 },
                    { (u8)Bytecode::PushRegA, (u8)Bytecode::PushConst, (u8)19, (u8)0, (u8)0, (u8)0, (u8)Bytecode::Mul, (u8)Bytecode::PopRegA },
                    23, 2, 3
                },
                {
                    { 54, 65, 75, 74 },
                    { (u8)Bytecode::PushRegA, (u8)Bytecode::PushConst, (u8)6, (u8)0, (u8)0, (u8)0, (u8)Bytecode::Add, (u8)Bytecode::PopRegA },
                    19, 2, 0
                },
                {
                    { 79, 60, 97 },
                    { (u8)Bytecode::PushRegA, (u8)Bytecode::PushRegA, (u8)Bytecode::Mul, (u8)Bytecode::PopRegA },
                    13, 1, 3
                },
                {
                    { 74 },
                    { (u8)Bytecode::PushRegA, (u8)Bytecode::PushConst, (u8)3, (u8)0, (u8)0, (u8)0, (u8)Bytecode::Add, (u8)Bytecode::PopRegA },
                    17, 0, 1
                }
            };
        }
        else if (step == AoCStep::RealData)
        {
            inputData.Monkeys =
            {
                /*
                Monkey 0:
                  Starting items: 83, 62, 93
                  Operation: new = old * 17
                  Test: divisible by 2
                    If true: throw to monkey 1
                    If false: throw to monkey 6
                */
                {
                    { 83, 62, 93 },
                    { (u8)Bytecode::PushRegA, (u8)Bytecode::PushConst, (u8)17, (u8)0, (u8)0, (u8)0, (u8)Bytecode::Mul, (u8)Bytecode::PopRegA },
                    2, 1, 6
                },

                /*
                Monkey 1:
                  Starting items: 90, 55
                  Operation: new = old + 1
                  Test: divisible by 17
                    If true: throw to monkey 6
                    If false: throw to monkey 3
                */
                {
                    { 90, 55 },
                    { (u8)Bytecode::PushRegA, (u8)Bytecode::PushConst, (u8)1, (u8)0, (u8)0, (u8)0, (u8)Bytecode::Add, (u8)Bytecode::PopRegA },
                    17, 6, 3
                },

                /*
                Monkey 2:
                  Starting items: 91, 78, 80, 97, 79, 88
                  Operation: new = old + 3
                  Test: divisible by 19
                    If true: throw to monkey 7
                    If false: throw to monkey 5
                */
                {
                    { 91, 78, 80, 97, 79, 88 },
                    { (u8)Bytecode::PushRegA, (u8)Bytecode::PushConst, (u8)3, (u8)0, (u8)0, (u8)0, (u8)Bytecode::Add, (u8)Bytecode::PopRegA },
                    19, 7, 5
                },

                /*
                Monkey 3:
                  Starting items: 64, 80, 83, 89, 59
                  Operation: new = old + 5
                  Test: divisible by 3
                    If true: throw to monkey 7
                    If false: throw to monkey 2
                */
                {
                    { 64, 80, 83, 89, 59 },
                    { (u8)Bytecode::PushRegA, (u8)Bytecode::PushConst, (u8)5, (u8)0, (u8)0, (u8)0, (u8)Bytecode::Add, (u8)Bytecode::PopRegA },
                    3, 7, 2
                },

                /*
                Monkey 4:
                  Starting items: 98, 92, 99, 51
                  Operation: new = old * old
                  Test: divisible by 5
                    If true: throw to monkey 0
                    If false: throw to monkey 1
                */
                {
                    { 98, 92, 99, 51 },
                    { (u8)Bytecode::PushRegA, (u8)Bytecode::PushRegA, (u8)Bytecode::Mul, (u8)Bytecode::PopRegA },
                    5, 0, 1
                },

                /*
                Monkey 5:
                  Starting items: 68, 57, 95, 85, 98, 75, 98, 75
                  Operation: new = old + 2
                  Test: divisible by 13
                    If true: throw to monkey 4
                    If false: throw to monkey 0
                */
                {
                    { 68, 57, 95, 85, 98, 75, 98, 75 },
                    { (u8)Bytecode::PushRegA, (u8)Bytecode::PushConst, (u8)2, (u8)0, (u8)0, (u8)0, (u8)Bytecode::Add, (u8)Bytecode::PopRegA },
                    13, 4, 0
                },

                /*
                Monkey 6:
                  Starting items: 74
                  Operation: new = old + 4
                  Test: divisible by 7
                    If true: throw to monkey 3
                    If false: throw to monkey 2
                */
                {
                    { 74 },
                    { (u8)Bytecode::PushRegA, (u8)Bytecode::PushConst, (u8)4, (u8)0, (u8)0, (u8)0, (u8)Bytecode::Add, (u8)Bytecode::PopRegA },
                    7, 3, 2
                },

                /*
                Monkey 7:
                  Starting items: 68, 64, 60, 68, 87, 80, 82
                  Operation: new = old * 19
                  Test: divisible by 11
                    If true: throw to monkey 4
                    If false: throw to monkey 5
                */
                {
                    { 68, 64, 60, 68, 87, 80, 82 },
                    { (u8)Bytecode::PushRegA, (u8)Bytecode::PushConst, (u8)19, (u8)0, (u8)0, (u8)0, (u8)Bytecode::Mul, (u8)Bytecode::PopRegA },
                    11, 4, 5
                }
            };
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        static constexpr u32 roundsToSimulate{ 20 };
        static constexpr u32 topMonkeysCount{ 2 };

        std::vector<Internal::MonkeySimulationData> monkeySimulationData{};
        Internal::InitMonkeySimulationData(input.Monkeys, monkeySimulationData);
        Internal::SimulateNRounds(input.Monkeys, monkeySimulationData, roundsToSimulate);

        output.InspectionCounters.resize(monkeySimulationData.size());
        std::transform(monkeySimulationData.begin(), monkeySimulationData.end(), output.InspectionCounters.begin(),
            [](const Internal::MonkeySimulationData& data) { return data.ItemInspectionCount; });

        output.Part1Result = Internal::ComputeTopNMonkeyBusiness(output.InspectionCounters, topMonkeysCount);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.Part1Result == 10605;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Part 1 Result: {}\n", output.Part1Result);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
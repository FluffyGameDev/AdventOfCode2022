#include "data.h"
#include "aoc.h"

#include "bytecode.h"
#include <functional>
#include <numeric>
#include <sstream>
#include <string>

namespace AoC
{
    namespace Internal
    {
        struct MonkeySimulationData
        {
            std::vector<u64> HeldItems{};
            u64 ItemInspectionCount{};
        };

        using PostOperationAction = std::function<u64(u64)>;

        void InitMonkeySimulationData(const std::vector<MonkeyData>& monkeys, std::vector<MonkeySimulationData>& monkeySimulationData)
        {
            monkeySimulationData.resize(monkeys.size());
            for (u64 i = 0; i < monkeys.size(); ++i)
            {
                monkeySimulationData[i].HeldItems = monkeys[i].StartingItems;
            }
        }

        void SimulateMonkey(const std::vector<MonkeyData>& monkeys, std::vector<MonkeySimulationData>& monkeySimulationData,
                BytecodeInterpreter& interpreter, const MonkeyData& currentMonkeyData, MonkeySimulationData& currentMonkey,
                const PostOperationAction& postOperationAction)
        {
            currentMonkey.ItemInspectionCount += (u64)currentMonkey.HeldItems.size();

            for (u64 itemValue : currentMonkey.HeldItems)
            {
                interpreter.Reset();
                interpreter.SetRegisterA(itemValue);
                interpreter.RunBytecode(currentMonkeyData.BytecodeBuffer);
                itemValue = postOperationAction(interpreter.GetRegisterA());

                u64 targetMonkeyId{ (itemValue % currentMonkeyData.DivisibilityCondition == 0) ?
                    currentMonkeyData.ConditionTrueMonkey : currentMonkeyData.ConditionFalseMonkey };
                monkeySimulationData[targetMonkeyId].HeldItems.push_back(itemValue);
            }

            currentMonkey.HeldItems.clear();
        }

        void SimulateRound(const std::vector<MonkeyData>& monkeys, std::vector<MonkeySimulationData>& monkeySimulationData, BytecodeInterpreter& interpreter, const PostOperationAction& postOperationAction)
        {
            for (u64 monkeyId = 0; monkeyId < monkeySimulationData.size(); ++monkeyId)
            {
                const MonkeyData& currentMonkeyData{ monkeys[monkeyId] };
                MonkeySimulationData& currentMonkey{ monkeySimulationData[monkeyId] };
                SimulateMonkey(monkeys, monkeySimulationData, interpreter, currentMonkeyData, currentMonkey, postOperationAction);
            }
        }

        void SimulateNRounds(const std::vector<MonkeyData>& monkeys, std::vector<MonkeySimulationData>& monkeySimulationData, u64 roundCount, const PostOperationAction& postOperationAction)
        {
            BytecodeInterpreter interpreter{};
            for (u64 i = 0; i < roundCount; ++i)
            {
                SimulateRound(monkeys, monkeySimulationData, interpreter, postOperationAction);
            }
        }

        u64 ComputeTopNMonkeyBusiness(const std::vector<u64>& inspectionCounters, u64 monkeyCount)
        {
            std::vector<u64> countersCopy{ inspectionCounters };
            auto nthIterator{ countersCopy.begin() + monkeyCount };
            std::nth_element(countersCopy.begin(), nthIterator, countersCopy.end(), std::greater());
            return std::accumulate(countersCopy.begin(), nthIterator, 1ULL,
                [](u64 total, u64 value) { return total * value; });
        }

        u64 ComputeItemModulus(const InputData& input)
        {
            u64 result{ 1 };
            for (const MonkeyData& monkeyData : input.Monkeys)
            {
                result *= monkeyData.DivisibilityCondition;
            }
            return result;
        }

        void CompileBytecode(const std::string& inputLine, std::vector<u8>& bytecodeBuffer)
        {
            std::stringstream operationStream{ inputLine };
            std::string operand1Text{};
            std::string operand2Text{};
            std::string operatorText{};
            std::string ignore{};
            operationStream >> ignore >> ignore >> ignore >> operand1Text >> operatorText >> operand2Text;
            if (operand1Text == "old")
            {
                bytecodeBuffer.push_back((u8)Bytecode::PushRegA);
            }
            else
            {
                bytecodeBuffer.push_back((u8)Bytecode::PushConst);
                u64 value{ (u64)std::atoll(operand1Text.c_str()) };
                char* valueCharBuffer{ reinterpret_cast<char*>(&value) };
                for (u64 i = 0; i < sizeof(u64); ++i)
                {
                    bytecodeBuffer.push_back((u8)valueCharBuffer[i]);
                }
            }

            if (operand2Text == "old")
            {
                bytecodeBuffer.push_back((u8)Bytecode::PushRegA);
            }
            else
            {
                bytecodeBuffer.push_back((u8)Bytecode::PushConst);
                u64 value{ (u64)std::atoi(operand2Text.c_str()) };
                char* valueCharBuffer{ reinterpret_cast<char*>(&value) };
                for (u64 i = 0; i < sizeof(u64); ++i)
                {
                    bytecodeBuffer.push_back((u8)valueCharBuffer[i]);
                }
            }

            if (operatorText == "*") { bytecodeBuffer.push_back((u8)Bytecode::Mul); }
            else if (operatorText == "+") { bytecodeBuffer.push_back((u8)Bytecode::Add); }

            bytecodeBuffer.push_back((u8)Bytecode::PopRegA);
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        std::string line{};
        std::string ignore{};
        while (!inputStream.eof())
        {
            MonkeyData data{};
            std::getline(inputStream, line);

            //Starting items: ???
            u64 itemId{};
            std::getline(inputStream, line);
            std::stringstream itemsStream{ line };
            itemsStream >> ignore >> ignore;
            while (itemsStream >> itemId)
            {
                data.StartingItems.push_back(itemId);
                itemsStream >> ignore;
            }

            //Operation: new = ???
            std::getline(inputStream, line);
            Internal::CompileBytecode(line, data.BytecodeBuffer);

            //Test: divisible by ???
            std::getline(inputStream, line);
            sscanf_s(line.c_str(), "  Test: divisible by %lld", &data.DivisibilityCondition);

            //If true: throw to monkey ???
            std::getline(inputStream, line);
            sscanf_s(line.c_str(), "    If true: throw to monkey %lld", &data.ConditionTrueMonkey);

            //If false: throw to monkey ???
            std::getline(inputStream, line);
            sscanf_s(line.c_str(), "    If false: throw to monkey %lld", &data.ConditionFalseMonkey);

            inputData.Monkeys.push_back(data);
            std::getline(inputStream, line);
        }

        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        static constexpr u64 roundsToSimulatePart1{ 20 };
        static constexpr u64 roundsToSimulatePart2{ 10000 };
        static constexpr u64 topMonkeysCount{ 2 };
        auto getInspectionCount{ [](const Internal::MonkeySimulationData& data) { return data.ItemInspectionCount; } };

        std::vector<Internal::MonkeySimulationData> monkeySimulationDataPart1{};
        auto part1PostOp{ [](u64 item) { return item / 3; } };
        Internal::InitMonkeySimulationData(input.Monkeys, monkeySimulationDataPart1);
        Internal::SimulateNRounds(input.Monkeys, monkeySimulationDataPart1, roundsToSimulatePart1, part1PostOp);
        std::vector<u64> inspectionCountersPart1(monkeySimulationDataPart1.size(), 0);
        std::transform(monkeySimulationDataPart1.begin(), monkeySimulationDataPart1.end(), inspectionCountersPart1.begin(), getInspectionCount);
        output.Part1Result = Internal::ComputeTopNMonkeyBusiness(inspectionCountersPart1, topMonkeysCount);


        u64 itemModulus{ Internal::ComputeItemModulus(input) };
        std::vector<Internal::MonkeySimulationData> monkeySimulationDataPart2{};
        auto part2PostOp{ [itemModulus](u64 item) { return item % itemModulus; } };
        Internal::InitMonkeySimulationData(input.Monkeys, monkeySimulationDataPart2);
        Internal::SimulateNRounds(input.Monkeys, monkeySimulationDataPart2, roundsToSimulatePart2, part2PostOp);
        std::vector<u64> inspectionCountersPart2(monkeySimulationDataPart2.size(), 0);
        std::transform(monkeySimulationDataPart2.begin(), monkeySimulationDataPart2.end(), inspectionCountersPart2.begin(), getInspectionCount);
        output.Part2Result = Internal::ComputeTopNMonkeyBusiness(inspectionCountersPart2, topMonkeysCount);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.Part1Result == 10605ULL;
        didTestsPass &= output.Part2Result == 2713310158ULL;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Part 1 Result: {}\n", output.Part1Result);
        fmt::print("Part 2 Result: {}\n", output.Part2Result);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
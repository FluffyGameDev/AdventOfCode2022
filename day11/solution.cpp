#include "data.h"
#include "aoc.h"

#include "bytecode.h"
#include <numeric>
#include <sstream>
#include <string>

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
                interpreter.Reset();
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
                u32 value{ (u32)std::atoi(operand1Text.c_str()) };
                char* valueCharBuffer{ reinterpret_cast<char*>(&value) };
                for (u32 i = 0; i < sizeof(u32); ++i)
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
                u32 value{ (u32)std::atoi(operand2Text.c_str()) };
                char* valueCharBuffer{ reinterpret_cast<char*>(&value) };
                for (u32 i = 0; i < sizeof(u32); ++i)
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
            u32 itemId{};
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
            sscanf_s(line.c_str(), "  Test: divisible by %d", &data.DivisibilityCondition);

            //If true: throw to monkey ???
            std::getline(inputStream, line);
            sscanf_s(line.c_str(), "    If true: throw to monkey %d", &data.ConditionTrueMonkey);

            //If false: throw to monkey ???
            std::getline(inputStream, line);
            sscanf_s(line.c_str(), "    If false: throw to monkey %d", &data.ConditionFalseMonkey);

            inputData.Monkeys.push_back(data);
            std::getline(inputStream, line);
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
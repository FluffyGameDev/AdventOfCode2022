#include "data.h"
#include "aoc.h"

namespace AoC
{
    namespace Internal
    {
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData)
    {
        return true;
    }

    void ComputeOutput(const InputData& inputData, OutputData& output)
    {
    }

    bool ValidateTestOutput(const OutputData& outputData)
    {
        bool didTestsPass{ true };

        //TODO

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
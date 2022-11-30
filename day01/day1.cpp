#include <algorithm>
#include <fstream>
#include <numeric>
#include <vector>

#include <fmt/core.h>

bool ReadInputValues(/*TODO: output data*/)
{
    static const char* inputFile{ "input.txt" };
    std::ifstream inputStream{ inputFile };

    bool readSucceeded{ inputStream.is_open() };
    if (readSucceeded)
    {
        while (!inputStream.eof())
        {
            std::uint32_t readValue{};
            inputStream >> readValue; //TODO: use some other struct
        }

        inputStream.close();
    }

    return readSucceeded;
}

int main()
{
    if (ReadInputValues())
    {
        //TODO
    }
    else
    {
        fmt::print("Failed to open input file.\n");
    }

    return 0;
}
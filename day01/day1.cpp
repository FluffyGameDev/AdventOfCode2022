#include "day1.h"

#include <algorithm>
#include <fstream>
#include <numeric>
#include <vector>


namespace Day1
{
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
}
#include <fmt/core.h>

#include "day1.h"
#include "tests.h"

void main()
{
    if (Day1::Tests::ValidateTests())
    {
        if (Day1::ReadInputValues())
        {
            //TODO
            fmt::print("Done.\n");
        }
        else
        {
            fmt::print("Failed to open input file.\n");
        }
    }
    else
    {
        fmt::print("Tests have failed.\n");
    }
}
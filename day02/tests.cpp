#include "tests.h"

#include <sstream>
#include "day2.h"

namespace Day2::Tests
{
    static const char* ms_inputData{
        "A Y\n"
        "B X\n"
        "C Z"
    };

    static const std::uint32_t ms_Part1ExpectedResult{ 15 };
    static const std::uint32_t ms_Part2ExpectedResult{ 12 };

    bool ValidateTests()
    {
        bool didTestsPass{ true };

        std::istringstream input{ ms_inputData };

        TournamentMovesSummary tournamentMovesSummary;
        ReadTournamentMovesSummary(input, tournamentMovesSummary);

        TournamentResults results{};
        ComputeTournamentResults(tournamentMovesSummary, results);

        didTestsPass &= results.TheoreticalTotalScore == ms_Part1ExpectedResult;
        didTestsPass &= results.RealTotalScore == ms_Part2ExpectedResult;

        return didTestsPass;
    }
}
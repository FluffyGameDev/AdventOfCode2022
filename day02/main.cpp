#include <fmt/core.h>

#include "day2.h"
#include "tests.h"

void main()
{
    if (Day2::Tests::ValidateTests())
    {
        Day2::TournamentMovesSummary tournamentMovesSummary;
        if (Day2::ReadInputValues(tournamentMovesSummary))
        {
            Day2::TournamentResults results{};
            Day2::ComputeTournamentResults(tournamentMovesSummary, results);

            fmt::print("Theoretical Strategy Score {}.\n", results.TheoreticalTotalScore);
            fmt::print("Real Strategy Score {}.\n", results.RealTotalScore);
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
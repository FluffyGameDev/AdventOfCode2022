#pragma once

#include "types.h"

namespace AoC
{
    const char* s_testInputData
    {
        "A Y\n"
        "B X\n"
        "C Z"
    };

    static constexpr u32 PossibleMoveCount{ 3 };
    static constexpr u32 PossibleMoveCombinations{ PossibleMoveCount * PossibleMoveCount };

    struct TournamentMovesSummary
    {
        u32 CountPerMoves[PossibleMoveCombinations]{0};
    };

    struct TournamentResults
    {
        u32 TheoreticalTotalScore;
        u32 RealTotalScore;
    };
}
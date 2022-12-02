#pragma once

#include <cstdint>

namespace AoC
{
    const char* s_testInputData
    {
        "A Y\n"
        "B X\n"
        "C Z"
    };

    static constexpr std::uint32_t PossibleMoveCount{ 3 };
    static constexpr std::uint32_t PossibleMoveCombinations{ PossibleMoveCount * PossibleMoveCount };

    struct TournamentMovesSummary
    {
        std::uint32_t CountPerMoves[PossibleMoveCombinations]{0};
    };

    struct TournamentResults
    {
        std::uint32_t TheoreticalTotalScore;
        std::uint32_t RealTotalScore;
    };
}
#pragma once

#include <array>
#include <cstdint>
#include <istream>
#include <vector>

namespace Day2
{
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

    bool ReadInputValues(TournamentMovesSummary& tournamentMovesSummary);
    void ReadTournamentMovesSummary(std::istream& inputStream, TournamentMovesSummary& tournamentMovesSummary);
    void ComputeTournamentResults(const TournamentMovesSummary& movesSummary, TournamentResults& TournamentResults);
}
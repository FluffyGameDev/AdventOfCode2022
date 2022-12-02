#include "day2.h"

#include <algorithm>
#include <fstream>
#include <numeric>
#include <sstream>
#include <vector>


namespace Day2
{
    bool ReadInputValues(TournamentMovesSummary& tournamentMovesSummary)
    {
        static const char* inputFile{ "input.txt" };
        std::ifstream inputStream{ inputFile };

        bool readSucceeded{ inputStream.is_open() };
        if (readSucceeded)
        {
            ReadTournamentMovesSummary(inputStream, tournamentMovesSummary);
            inputStream.close();
        }

        return readSucceeded;
    }

    void ReadTournamentMovesSummary(std::istream& inputStream, TournamentMovesSummary& tournamentMovesSummary)
    {
        char opponentMove{};
        char playerMove{};
        while (inputStream >> opponentMove >> playerMove)
        {
            std::uint32_t movesId{ (std::uint32_t)((opponentMove - 'A') * PossibleMoveCount + (playerMove - 'X')) };
            ++tournamentMovesSummary.CountPerMoves[movesId];
        }
    }

    enum class MatchMove
    {
        Rock = 1,
        Paper = 2,
        Scissors = 3
    };

    enum class MatchOutcome
    {
        Loss = 0,
        Draw = 3,
        Win = 6
    };

    static constexpr std::uint32_t PossibleOutcomeCount{ 3 };
    using CorrespondingMoves = std::array<MatchMove, PossibleMoveCount>;
    using CorrespondingOutcomes = std::array<MatchOutcome, PossibleOutcomeCount>;

    MatchOutcome ComputeMatchOutcome(MatchMove playerMove, MatchMove opponentMove)
    {
        MatchOutcome outcome{ MatchOutcome::Draw };

        if (playerMove != opponentMove)
        {
            switch (opponentMove)
            {
                case MatchMove::Rock: { outcome = (playerMove == MatchMove::Paper ? MatchOutcome::Win : MatchOutcome::Loss); break; }
                case MatchMove::Paper: { outcome = (playerMove == MatchMove::Scissors ? MatchOutcome::Win : MatchOutcome::Loss); break; }
                case MatchMove::Scissors: { outcome = (playerMove == MatchMove::Rock ? MatchOutcome::Win : MatchOutcome::Loss); break; }
            }
        }

        return outcome;
    }

    std::tuple<MatchOutcome, MatchMove> ComputeTheoreticalMatchOutcome(std::uint32_t matchId)
    {
        static constexpr Day2::CorrespondingMoves correspondingMoves{ MatchMove::Rock, MatchMove::Paper, MatchMove::Scissors };

        MatchMove opponentMove{ correspondingMoves[matchId / PossibleMoveCount] };
        MatchMove playerMove{ correspondingMoves[matchId % PossibleMoveCount] };
        MatchOutcome outcome{ ComputeMatchOutcome(playerMove, opponentMove) };

        return { outcome, playerMove };
    }

    MatchMove ComputeNeededMove(MatchMove opponentMove, MatchOutcome desiredOutcome)
    {
        MatchMove playerMove{ opponentMove };

        if (desiredOutcome != MatchOutcome::Draw)
        {
            switch (opponentMove)
            {
                case MatchMove::Rock: { playerMove = (desiredOutcome == MatchOutcome::Win ? MatchMove::Paper : MatchMove::Scissors); break; }
                case MatchMove::Paper: { playerMove = (desiredOutcome == MatchOutcome::Win ? MatchMove::Scissors : MatchMove::Rock); break; }
                case MatchMove::Scissors: { playerMove = (desiredOutcome == MatchOutcome::Win ? MatchMove::Rock : MatchMove::Paper); break; }
            }
        }

        return playerMove;
    }

    std::tuple<MatchOutcome, MatchMove> ComputeRealMatchOutcome(std::uint32_t matchId)
    {
        static constexpr Day2::CorrespondingMoves correspondingMoves{ MatchMove::Rock, MatchMove::Paper, MatchMove::Scissors };
        static constexpr Day2::CorrespondingOutcomes correspondingOutcomes{ MatchOutcome::Loss, MatchOutcome::Draw, MatchOutcome::Win };

        MatchMove opponentMove{ correspondingMoves[matchId / PossibleMoveCount] };
        MatchOutcome outcome{ correspondingOutcomes[matchId % PossibleMoveCount] };
        MatchMove playerMove{ ComputeNeededMove(opponentMove, outcome) };

        return { outcome, playerMove };
    }

    void ComputeTournamentResults(const TournamentMovesSummary& movesSummary, TournamentResults& tournamentResults)
    {
        for (std::uint32_t i = 0; i < PossibleMoveCombinations; ++i)
        {
            std::uint32_t matchCount{ movesSummary.CountPerMoves[i] };
            if (matchCount > 0)
            {
                {
                    auto [outcome, move] { ComputeTheoreticalMatchOutcome(i) };
                    std::uint32_t theoreticalScore{ (std::uint32_t)outcome + (std::uint32_t)move };
                    tournamentResults.TheoreticalTotalScore += theoreticalScore * matchCount;
                }

                {
                    auto [outcome, move] { ComputeRealMatchOutcome(i) };
                    std::uint32_t realScore{ (std::uint32_t)outcome + (std::uint32_t)move };
                    tournamentResults.RealTotalScore += realScore * matchCount;
                }
            }
        }
    }
}
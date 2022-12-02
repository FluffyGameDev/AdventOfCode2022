#include "data.h"
#include "aoc.h"

#include <algorithm>
#include <array>


namespace AoC
{
    namespace Internal
    {
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
            static constexpr CorrespondingMoves correspondingMoves{ MatchMove::Rock, MatchMove::Paper, MatchMove::Scissors };

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
            static constexpr CorrespondingMoves correspondingMoves{ MatchMove::Rock, MatchMove::Paper, MatchMove::Scissors };
            static constexpr CorrespondingOutcomes correspondingOutcomes{ MatchOutcome::Loss, MatchOutcome::Draw, MatchOutcome::Win };

            MatchMove opponentMove{ correspondingMoves[matchId / PossibleMoveCount] };
            MatchOutcome outcome{ correspondingOutcomes[matchId % PossibleMoveCount] };
            MatchMove playerMove{ ComputeNeededMove(opponentMove, outcome) };

            return { outcome, playerMove };
        }
    }

    bool ReadInput(std::istream& inputStream, TournamentMovesSummary& movesSummary)
    {
        char opponentMove{};
        char playerMove{};
        while (inputStream >> opponentMove >> playerMove)
        {
            std::uint32_t movesId{ (std::uint32_t)((opponentMove - 'A') * AoC::PossibleMoveCount + (playerMove - 'X')) };
            ++movesSummary.CountPerMoves[movesId];
        }
        return true;
    }

    void ComputeOutput(const TournamentMovesSummary& movesSummary, TournamentResults& tournamentResults)
    {
        for (std::uint32_t i = 0; i < PossibleMoveCombinations; ++i)
        {
            std::uint32_t matchCount{ movesSummary.CountPerMoves[i] };
            if (matchCount > 0)
            {
                {
                    auto [outcome, move] { Internal::ComputeTheoreticalMatchOutcome(i) };
                    std::uint32_t theoreticalScore{ (std::uint32_t)outcome + (std::uint32_t)move };
                    tournamentResults.TheoreticalTotalScore += theoreticalScore * matchCount;
                }

                {
                    auto [outcome, move] { Internal::ComputeRealMatchOutcome(i) };
                    std::uint32_t realScore{ (std::uint32_t)outcome + (std::uint32_t)move };
                    tournamentResults.RealTotalScore += realScore * matchCount;
                }
            }
        }
    }

    bool ValidateTestOutput(const TournamentResults& output)
    {
        static const std::uint32_t part1ExpectedResult{ 15 };
        static const std::uint32_t part2ExpectedResult{ 12 };

        bool didTestsPass{ true };

        didTestsPass &= output.TheoreticalTotalScore == part1ExpectedResult;
        didTestsPass &= output.RealTotalScore == part2ExpectedResult;

        return didTestsPass;
    }

    void PrintOutput(const TournamentResults& output)
    {
        fmt::print("Theoretical Strategy Score {}.\n", output.TheoreticalTotalScore);
        fmt::print("Real Strategy Score {}.\n", output.RealTotalScore);
    }

}

void main()
{
    AoC::Run<AoC::TournamentMovesSummary, AoC::TournamentResults>(AoC::s_testInputData);
}
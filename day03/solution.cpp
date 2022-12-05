#include "data.h"
#include "aoc.h"

#include <numeric>
#include <set>
#include <string_view>

namespace AoC
{
    namespace Internal
    {
        void FindCommonCompartmentItems(const std::string_view& leftBag
                                      , const std::string_view& rightBag
                                      , std::string& commonItems)
        {
            std::string leftBagCopy{ leftBag };
            std::string rightBagCopy{ rightBag };
            std::set<char> itemSet{};

            std::sort(leftBagCopy.begin(), leftBagCopy.end());
            std::sort(rightBagCopy.begin(), rightBagCopy.end());

            std::set_intersection(leftBagCopy.begin(), leftBagCopy.end(),
                                  rightBagCopy.begin(), rightBagCopy.end(),
                                  std::inserter(itemSet, std::end(itemSet)));
            commonItems.resize(itemSet.size());
            std::copy(itemSet.begin(), itemSet.end(), commonItems.begin());
        }

        u32 ComputeItemListPriority(const std::string& commonItems)
        {
            auto accumulateItemPriority{ [](u32 currentTotal, char item)
                { return currentTotal + (std::islower(item) ? item - 'a' + 1 : item - 'A' + 27); } };
            return std::accumulate(commonItems.begin(), commonItems.end(), 0U, accumulateItemPriority);
        }

        u32 ComputeTotalPriorities(const InputData& inputData)
        {
            u32 totalItemPriorities{};
            std::string commonItems{};
            for (const auto& bagContent : inputData.BagContents)
            {
                commonItems.clear();
                std::size_t leftSize{ bagContent.size() / 2 };
                std::string_view leftCompartment{ bagContent.c_str(), leftSize };
                std::string_view rightCompartment{ bagContent.c_str() + leftSize, bagContent.size() - leftSize };
                Internal::FindCommonCompartmentItems(leftCompartment, rightCompartment, commonItems);
                totalItemPriorities += Internal::ComputeItemListPriority(commonItems);
            }
            return totalItemPriorities;
        }

        u32 ComputeTotalBadgePriorities(const InputData& inputData)
        {
            static constexpr u32 elvesPerTeam{ 3 };

            u32 totalBadgePriorities{};
            std::string commonItemsPass1{};
            std::string commonItemsPass2{};
            for (std::size_t teamStartIndex = 0; teamStartIndex < inputData.BagContents.size(); teamStartIndex += elvesPerTeam)
            {
                commonItemsPass1.clear();
                commonItemsPass2.clear();
                const std::string& bag1{ inputData.BagContents[teamStartIndex] };
                const std::string& bag2{ inputData.BagContents[teamStartIndex + 1] };
                const std::string& bag3{ inputData.BagContents[teamStartIndex + 2] };
                Internal::FindCommonCompartmentItems(bag1, bag2, commonItemsPass1);
                Internal::FindCommonCompartmentItems(commonItemsPass1, bag3, commonItemsPass2);
                totalBadgePriorities += Internal::ComputeItemListPriority(commonItemsPass2);
            }
            return totalBadgePriorities;
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData)
    {
        std::string line{};
        while (!inputStream.eof())
        {
            inputStream >> line;
            inputData.BagContents.push_back(line);
        }
        return true;
    }

    void ComputeOutput(const InputData& inputData, OutputData& output)
    {
        output.TotalItemPriorities = Internal::ComputeTotalPriorities(inputData);
        output.TotalBadgePriorities = Internal::ComputeTotalBadgePriorities(inputData);
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        static const u32 part1TestSolution{ 157 };
        didTestsPass &= output.TotalItemPriorities == part1TestSolution;

        static const u32 part2TestSolution{ 70 };
        didTestsPass &= output.TotalBadgePriorities == part2TestSolution;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Total Item Priorities {}\n", output.TotalItemPriorities);
        fmt::print("Total Badge Priorities {}\n", output.TotalBadgePriorities);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
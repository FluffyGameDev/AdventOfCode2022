#include "data.h"
#include "aoc.h"

#include <algorithm>
#include <string>

namespace AoC
{
    namespace Internal
    {
        void InitVisibilityData(const InputData& input, std::vector<VisibilityData>& visibilityMap)
        {
            u32 width{ input.MapWidth };
            u32 height{ (u32)input.TreeHeightMap.size() / input.MapWidth };
            for (u32 i = 0; i < width; ++i)
            {
                u32 topIndex{ i };
                u32 bottomIndex{ (u32)input.TreeHeightMap.size() - i - 1 };
                visibilityMap[topIndex].HighestTop = input.TreeHeightMap[topIndex];
                visibilityMap[bottomIndex].HighestBottom = input.TreeHeightMap[bottomIndex];
            }
            for (u32 i = 0; i < height; ++i)
            {
                u32 leftIndex{ i * input.MapWidth };
                u32 rightIndex{ leftIndex + input.MapWidth - 1 };
                visibilityMap[leftIndex].HighestLeft = input.TreeHeightMap[leftIndex];
                visibilityMap[rightIndex].HighestRight = input.TreeHeightMap[rightIndex];
            }
        }

        void ComputeVisibilityForwardPass(const InputData& input, std::vector<VisibilityData>& visibilityMap)
        {
            u32 width{ input.MapWidth };
            u32 height{ (u32)input.TreeHeightMap.size() / input.MapWidth };
            for (u32 j = 1; j < height - 1; ++j)
            {
                for (u32 i = 1; i < width - 1; ++i)
                {
                    u32 index{ i + j * width };
                    u8 visibility{ (u8)VisibilityFlags::NotVisible };

                    u8 currentTreeHeight{ input.TreeHeightMap[index] };
                    u8 topTreeHeight{ visibilityMap[index - width].HighestTop };
                    u8 leftTreeHeight{ visibilityMap[index - 1].HighestLeft };
                    u8 topTreeVisibility{ (u8)visibilityMap[index - width].Flags };
                    u8 leftTreeVisibility{ (u8)visibilityMap[index - 1].Flags };

                    if (topTreeHeight < currentTreeHeight)
                        visibility |= (u8)VisibilityFlags::FromTop;

                    if (leftTreeHeight < currentTreeHeight)
                        visibility |= (u8)VisibilityFlags::FromLeft;

                    visibilityMap[index].HighestTop = (currentTreeHeight > topTreeHeight ? currentTreeHeight : topTreeHeight);
                    visibilityMap[index].HighestLeft = (currentTreeHeight > leftTreeHeight ? currentTreeHeight : leftTreeHeight);
                    visibilityMap[index].Flags = (VisibilityFlags)visibility;
                }
            }
        }

        void ComputeVisibilityBackwardPass(const InputData& input, std::vector<VisibilityData>& visibilityMap)
        {
            u32 width{ input.MapWidth };
            u32 height{ (u32)input.TreeHeightMap.size() / input.MapWidth };
            for (u32 j = height - 2; j >= 1; --j)
            {
                for (u32 i = width - 2; i >= 1; --i)
                {
                    u32 index{ i + j * width };
                    u8 visibility{ (u8)visibilityMap[index].Flags };

                    u8 currentTreeHeight{ input.TreeHeightMap[index] };
                    u8 bottomTreeHeight{ visibilityMap[index + width].HighestBottom };
                    u8 rightTreeHeight{ visibilityMap[index + 1].HighestRight };
                    u8 bottomTreeVisibility{ (u8)visibilityMap[index + width].Flags };
                    u8 rightTreeVisibility{ (u8)visibilityMap[index + 1].Flags };

                    if (bottomTreeHeight < currentTreeHeight)
                        visibility |= (u8)VisibilityFlags::FromBottom;

                    if (rightTreeHeight < currentTreeHeight)
                        visibility |= (u8)VisibilityFlags::FromRight;

                    visibilityMap[index].HighestBottom = (currentTreeHeight > bottomTreeHeight ? currentTreeHeight : bottomTreeHeight);
                    visibilityMap[index].HighestRight = (currentTreeHeight > rightTreeHeight ? currentTreeHeight : rightTreeHeight);
                    visibilityMap[index].Flags = (VisibilityFlags)visibility;
                }
            }
        }

        void ComputeScenicDataAtCoord(const InputData& input, u32 index, u32 x, u32 y, ScenicData& scenicData)
        {
            u8 treeHeight{ input.TreeHeightMap[index] };

            u32 distLeft{ 1 };
            u32 maxDistLeft{ x };
            while (distLeft < maxDistLeft && input.TreeHeightMap[index - distLeft] < treeHeight) { ++distLeft; }

            u32 distRight{ 1 };
            u32 maxDistRight{ input.MapWidth - x - 1 };
            while (distRight < maxDistRight && input.TreeHeightMap[index + distRight] < treeHeight) { ++distRight; }

            u32 distTop{ 1 };
            u32 maxDistTop{ y };
            while (distTop < maxDistTop && input.TreeHeightMap[index - distTop * input.MapWidth] < treeHeight) { ++distTop; }

            u32 distBottom{ 1 };
            u32 maxDistBottom{ input.MapWidth - y - 1 };
            while (distBottom < maxDistBottom && input.TreeHeightMap[index + distBottom * input.MapWidth] < treeHeight) { ++distBottom; }

            scenicData.LeftScore = distLeft;
            scenicData.RightScore = distRight;
            scenicData.TopScore = distTop;
            scenicData.BottomScore = distBottom;
        }

        void ComputeScenicData(const InputData& input, std::vector<ScenicData>& scenicData)
        {
            scenicData.resize(input.TreeHeightMap.size());

            u32 width{ input.MapWidth };
            u32 height{ (u32)input.TreeHeightMap.size() / input.MapWidth };
            for (u32 j = 1; j < height - 1; ++j)
            {
                for (u32 i = 1; i < width - 1; ++i)
                {
                    u32 index{ i + j * width };
                    ComputeScenicDataAtCoord(input, index, i, j, scenicData[index]);
                }
            }
        }

        u32 ComputeScenicScore(const ScenicData& scenicData)
        {
            return scenicData.TopScore * scenicData.BottomScore * scenicData.LeftScore * scenicData.RightScore;
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        std::string line;
        while (inputStream >> line)
        {
            inputData.MapWidth = (u32)line.size();
            u32 oldSize{ (u32)inputData.TreeHeightMap.size() };
            inputData.TreeHeightMap.resize(oldSize + line.size());
            std::transform(line.begin(), line.end(), inputData.TreeHeightMap.begin() + oldSize,
                [](char c) { return (u8)(c - '0'); });
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        output.VisibilityMap = std::vector<VisibilityData>(input.TreeHeightMap.size());
        Internal::InitVisibilityData(input, output.VisibilityMap);
        Internal::ComputeVisibilityForwardPass(input, output.VisibilityMap);
        Internal::ComputeVisibilityBackwardPass(input, output.VisibilityMap);
        output.VisibleTrees = (u32)std::count_if(output.VisibilityMap.begin(), output.VisibilityMap.end(),
            [](const VisibilityData& data) { return data.Flags != VisibilityFlags::NotVisible; });

        std::vector<ScenicData> scenicData{};
        Internal::ComputeScenicData(input, scenicData);

        output.BestScenicScore = 0;
        for (const ScenicData& data : scenicData)
        {
            u32 score{ Internal::ComputeScenicScore(data) };
            if (score > output.BestScenicScore)
            {
                output.BestScenicScore = score;
            }
        }
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        static constexpr u32 part1TestResult{ 21 };
        didTestsPass &= output.VisibleTrees == part1TestResult;

        static constexpr u32 part2TestResult{ 8 };
        didTestsPass &= output.BestScenicScore == part2TestResult;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Visible Trees: {}\n", output.VisibleTrees);
        fmt::print("Best Scenic Score: {}\n", output.BestScenicScore);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
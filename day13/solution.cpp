#include "data.h"
#include "aoc.h"

namespace AoC
{
    namespace Internal
    {
        static constexpr s32 NoValuePacket{ -1 };

        struct PacketNode
        {
            std::vector<PacketNode> SubNodes{};
            s32 Value{ NoValuePacket };
        };

        void BuildPacketNode(std::string_view& rawData, PacketNode& packetNode)
        {
            char nextChar{ rawData[0] };
            if (nextChar == '[')
            {
                rawData.remove_prefix(1);
                while (nextChar != ']')
                {
                    PacketNode& subNode{ packetNode.SubNodes.emplace_back() };
                    BuildPacketNode(rawData, subNode);

                    nextChar = rawData[0];
                    rawData.remove_prefix(1);
                }
            }
            else if (std::isdigit(nextChar))
            {
                //TODO: use strtol?
                std::string_view digitBuffer{ rawData };
                while (std::isdigit(rawData[0]))
                {
                    rawData.remove_prefix(1);
                }

                packetNode.Value = std::atoi(digitBuffer.data());
            }
            else
            {
                //Error
            }
        }

        s32 ComparePacketNodePair(const PacketNode& node1, const PacketNode& node2)
        {
            s32 result{};

            if (node1.SubNodes.empty() != node2.SubNodes.empty())
            {
                if (node1.SubNodes.empty())
                {
                    if (node1.Value != NoValuePacket)
                    {
                        PacketNode tmpNode{};
                        tmpNode.SubNodes.push_back(node1);
                        result = ComparePacketNodePair(tmpNode, node2);
                    }
                    else
                    {
                        result = (s32)(node1.SubNodes.size() - node2.SubNodes.size());
                    }
                }
                else
                {
                    if (node2.Value != NoValuePacket)
                    {
                        PacketNode tmpNode{};
                        tmpNode.SubNodes.push_back(node2);
                        result = ComparePacketNodePair(node1, tmpNode);
                    }
                    else
                    {
                        result = (s32)(node1.SubNodes.size() - node2.SubNodes.size());
                    }
                }
            }
            else if (node1.SubNodes.empty() && node2.SubNodes.empty())
            {
                result = node1.Value - node2.Value;
            }
            else
            {
                u32 i{};
                u32 minLength{ (u32)std::min(node1.SubNodes.size(), node2.SubNodes.size()) };
                while (i < minLength && result == 0)
                {
                    result = ComparePacketNodePair(node1.SubNodes[i], node2.SubNodes[i]);
                    ++i;
                }

                if (result == 0)
                {
                    result = (s32)(node1.SubNodes.size() - node2.SubNodes.size());
                }
            }

            return result;
        }

        void PrintPacketNode(const PacketNode& node)
        {
            if (node.SubNodes.empty())
            {
                if (node.Value != NoValuePacket)
                {
                    fmt::print("{}", node.Value);
                }
            }
            else
            {
                u32 index{};
                fmt::print("[");
                for (const PacketNode& subNode : node.SubNodes)
                {
                    if (index != 0)
                        fmt::print(",");
                    PrintPacketNode(subNode);
                    ++index;
                }
                fmt::print("]");
            }
        }
    }

    bool ReadInput(std::istream& inputStream, InputData& inputData, AoCStep step)
    {
        RawPacketPairs pair{};
        while (inputStream >> pair.RawPacket1 >> pair.RawPacket2)
        {
            inputData.RawPacketPairs.push_back(pair);
        }
        return true;
    }

    void ComputeOutput(const InputData& input, OutputData& output)
    {
        std::vector<Internal::PacketNode*> packets{};

        u32 index{ 1 };
        for (const RawPacketPairs& packetPair : input.RawPacketPairs)
        {
            Internal::PacketNode* node1{ new Internal::PacketNode() };
            Internal::PacketNode* node2{ new Internal::PacketNode() };
            packets.push_back(node1);
            packets.push_back(node2);

            Internal::BuildPacketNode(std::string_view(packetPair.RawPacket1), *node1);
            Internal::BuildPacketNode(std::string_view(packetPair.RawPacket2), *node2);

            if (Internal::ComparePacketNodePair(*node1, *node2) <= 0)
            {
                output.SumRightPairIndices += index;
            }

            ++index;
        }

        Internal::PacketNode* divider1{ new Internal::PacketNode() };
        Internal::PacketNode* divider2{ new Internal::PacketNode() };
        divider1->SubNodes.emplace_back().SubNodes.emplace_back().Value = 2;
        divider2->SubNodes.emplace_back().SubNodes.emplace_back().Value = 6;
        packets.push_back(divider1);
        packets.push_back(divider2);

        auto comparePackets{ [](const Internal::PacketNode* lhs, const Internal::PacketNode* rhs)
            { return Internal::ComparePacketNodePair(*lhs, *rhs) < 0; } };
        std::sort(packets.begin(), packets.end(), comparePackets);

        u32 divider1Index{ (u32)std::distance(packets.begin(), std::find(packets.begin(), packets.end(), divider1)) + 1 };
        u32 divider2Index{ (u32)std::distance(packets.begin(), std::find(packets.begin(), packets.end(), divider2)) + 1 };
        output.DividerIndicesProduct = divider1Index * divider2Index;

        for (Internal::PacketNode* packet : packets)
            delete packet;
        packets.clear();
    }

    bool ValidateTestOutput(const OutputData& output)
    {
        bool didTestsPass{ true };

        didTestsPass &= output.SumRightPairIndices == 13;
        didTestsPass &= output.DividerIndicesProduct == 140;

        return didTestsPass;
    }

    void PrintOutput(const OutputData& output)
    {
        fmt::print("Sum of Right Pair Indices: {}\n", output.SumRightPairIndices);
        fmt::print("Divider Indices Product: {}\n", output.DividerIndicesProduct);
    }
}

void main()
{
    AoC::Run<AoC::InputData, AoC::OutputData>(AoC::s_testInputData);
}
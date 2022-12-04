#pragma once

#include <cstdint>

namespace AoC
{
    class Range
    {
    public:
        Range()
            : Range(0, 0)
        {
        }

        Range(std::uint32_t min, std::uint32_t max)
        {
            Min = min;
            Max = max;
        }

        bool IsInRange(std::uint32_t value) const
        {
            return (Min <= value && value <= Max);
        }

        bool IsInRange(const Range& range) const
        {
            return IsInRange(range.Min) && IsInRange(range.Max);
        }

        bool DoesRangeOverlap(const Range& range) const
        {
            return IsInRange(range.Min) || IsInRange(range.Max) || range.IsInRange(Min) || range.IsInRange(Max);
        }

        std::uint32_t Min;
        std::uint32_t Max;
    };
}
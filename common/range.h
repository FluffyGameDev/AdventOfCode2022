#pragma once

#include "types.h"

namespace AoC
{
    class Range
    {
    public:
        Range()
            : Range(0, 0)
        {
        }

        Range(u32 min, u32 max)
        {
            Min = min;
            Max = max;
        }

        bool IsInRange(u32 value) const
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

        u32 Min;
        u32 Max;
    };
}
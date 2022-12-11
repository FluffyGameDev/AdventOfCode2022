#pragma once

#include "types.h"
#include <vector>

namespace AoC
{
    enum class Bytecode : u8
    {
        PushConst,
        PushRegA,
        PopRegA,
        Add,
        Mul
    };

    class BytecodeInterpreter
    {
    public:
        void RunBytecode(const std::vector<u8>& buffer);
        void Reset();

        inline u64 GetRegisterA() const { return m_RegisterA; }
        inline void SetRegisterA(u64 value) { m_RegisterA = value; }

    private:
        void Execute(Bytecode operation, const std::vector<u8>& buffer, u32& instructionPointer);
        void Execute_PushConst(const std::vector<u8>& buffer, u32& instructionPointer);
        void Execute_PushRegA(const std::vector<u8>& buffer, u32& instructionPointer);
        void Execute_PopRegA(const std::vector<u8>& buffer, u32& instructionPointer);
        void Execute_Add(const std::vector<u8>& buffer, u32& instructionPointer);
        void Execute_Mul(const std::vector<u8>& buffer, u32& instructionPointer);

        std::vector<u64> m_Stack;
        u64 m_RegisterA;
    };
}
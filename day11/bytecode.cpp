#include "bytecode.h"

namespace AoC
{
    void BytecodeInterpreter::RunBytecode(const std::vector<u8>& buffer)
    {
        u32 endProgram{ (u32)buffer.size() };
        u32 instructionPointer{ 0 };

        while (instructionPointer < endProgram)
        {
            Bytecode currentOp{ (Bytecode)buffer[instructionPointer] };
            ++instructionPointer;
            Execute(currentOp, buffer, instructionPointer);
        }
    }

    void BytecodeInterpreter::Reset()
    {
        m_Stack.clear();
        m_RegisterA = 0;
    }


    void BytecodeInterpreter::Execute(Bytecode operation, const std::vector<u8>& buffer, u32& instructionPointer)
    {
        switch (operation)
        {
            case Bytecode::PushConst: Execute_PushConst(buffer, instructionPointer); break;
            case Bytecode::PushRegA: Execute_PushRegA(buffer, instructionPointer); break;
            case Bytecode::PopRegA: Execute_PopRegA(buffer, instructionPointer); break;
            case Bytecode::Add: Execute_Add(buffer, instructionPointer); break;
            case Bytecode::Mul: Execute_Mul(buffer, instructionPointer); break;
        }
    }

    void BytecodeInterpreter::Execute_PushConst(const std::vector<u8>& buffer, u32& instructionPointer)
    {
        u32 constantValue{ *reinterpret_cast<const u32*>(&buffer[instructionPointer]) };
        instructionPointer += sizeof(u32);
        m_Stack.push_back(constantValue);
    }

    void BytecodeInterpreter::Execute_PushRegA(const std::vector<u8>& buffer, u32& instructionPointer)
    {
        m_Stack.push_back(m_RegisterA);
    }

    void BytecodeInterpreter::Execute_PopRegA(const std::vector<u8>& buffer, u32& instructionPointer)
    {
        m_RegisterA = m_Stack.back();
        m_Stack.pop_back();
    }

    void BytecodeInterpreter::Execute_Add(const std::vector<u8>& buffer, u32& instructionPointer)
    {
        u32 valueA{ m_Stack.back() };
        m_Stack.pop_back();
        u32 valueB{ m_Stack.back() };
        m_Stack.pop_back();
        m_Stack.push_back(valueA + valueB);
    }

    void BytecodeInterpreter::Execute_Mul(const std::vector<u8>& buffer, u32& instructionPointer)
    {
        u32 valueA{ m_Stack.back() };
        m_Stack.pop_back();
        u32 valueB{ m_Stack.back() };
        m_Stack.pop_back();
        m_Stack.push_back(valueA * valueB);
    }
}
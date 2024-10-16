// Test task for Quantum Brains, 2024.
// All rights are either reserved, not reserved or reserved a little.
// ========================================================================== //
// Stack virtual machine class
// Author: Ksenia Dolgorukova
// ========================================================================== //
#pragma once
#include <vector>
#include <cstddef>
#include <string>

namespace stackVM {

// Instructions mnemonics
enum Instructions{
  PUSH = 0,
  POP = 1,
  SWAP = 2,
  ADD = 3,
  SUB = 4,
  MUL = 5,
  DIV = 6,
  RET = 7,
  JMP = 8,
  CALL = 9,
  FRET = 10,
  JNE = 11,
  JEQ = 12,
  JGT = 13,
  JGE = 14,
  JLS = 15,
  JLE = 16,
  InstructionNumber = 17
};

const std::string InstructionNames[Instructions::InstructionNumber] = {
  "push",
  "pop",
  "swap",
  "add",
  "sub",
  "mul",
  "div",
  "ret",
  "jump",
  "call",
  "fret",
  "jne",
  "jeq",
  "jgt",
  "jge",
  "jls",
  "jle"
};

// Error codes
enum class ErrorCode{
  Success = 0,
  StackIsEmpty = 1,
  StackIsFull = 2,
  SegmentationFault = 3,
  UnknownInstruction = 4,
  DivisionByZero = 5,
  TooManyFuncParams = 6,
  ErrorNumber = 7
};

// ========================================================================== //
// This class implements a stack virtual machine with basic instruction set   //
// ========================================================================== //
class StackVirtualMachine{
public:
  StackVirtualMachine();

  // Modifiers
  void AddInstruction(Instructions instruction, unsigned int param = 0);
  void Clear();
  void ClearStackAndPC(); //for debugging

  // The main method runs the machine. Copies top of the stack to the topStack parameter
  // Normally returns 0, othervise returns error code
  ErrorCode Interpret(int& topStack);
  bool InterpretOneStep(int& topStack, ErrorCode& errCode);

  // Debugging
  void DumpStack();
  void PrintState();
private:
  // ----------------------------------------------------------------- //
  // Instructions implementation, every instruction returns error code
  // ----------------------------------------------------------------- //

  // Working with the stack
  ErrorCode push(int arg);   // pushes an element arg to the stack top
  ErrorCode pop();       // removes an element from the stack top
  ErrorCode swap();      // swaps two top elements

  // Arithmetics, leave results on the top of the stack
  ErrorCode add();
  ErrorCode sub();
  ErrorCode mul();
  ErrorCode div();

  // Jumps and returns, all of them affect PC
  ErrorCode ret();             // stops the machine, returns the top stack element
  ErrorCode jump();            // unconditional jump to the address on the top of the stack
  ErrorCode call(unsigned int paramsNo); // calls the function which parameters and address are on the top of the stack
  ErrorCode functionReturn();      // clears stack from the function call stack, sets the result to the top of the stack and returns
  ErrorCode jumpIfEqual();
  ErrorCode jumpIfNotEqual();
  ErrorCode jumpIfGreater();
  ErrorCode jumpIfGreaterOrEqual();
  ErrorCode jumpIfLess();
  ErrorCode jumpIfLessOrEqual();

  // ------------------------------- //
  // Diagnostics
  // ------------------------------- //
  void error(ErrorCode errorCode);

  // Utility functions
  ErrorCode internalPop(int& top); // extracts an element from the stack top
  ErrorCode internalJump(bool (*compare)(int, int));
private:
  std::vector<int> stack;            // stack memory, stack counter is always set to stack.size()
  std::vector<unsigned long int> instructions; // instructions memory
  size_t pc;           // program counter
};

}// namespace stackVM

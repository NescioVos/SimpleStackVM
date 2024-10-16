// Test task for Quantum Brains, 2024.
// All rights are either reserved, not reserved or reserved a little.
// ========================================================================== //
// Stack virtual machine debugger class
// Author: Ksenia Dolgorukova
// ========================================================================== //
#pragma once
#include "StackVirtualMachine.h"

namespace stackVM {

class StackMachineStepDebugger{
public:
  // The main method, runs the debugging loop
  void RunDebug();
private:
  bool parseNextCommand();
  void interpretStepByStep();

  StackVirtualMachine vm;
};

}// namespace stackVM

// Test task for Quantum Brains, 2024.
// All rights are either reserved, not reserved or reserved a little.
// ========================================================================== //
// Main file runs step debugger for StackVirtualMachine
// Author: Ksenia Dolgorukova
// ========================================================================== //

#include "StackVMDebugger.h"

int main(){
  // Start the session for debugger
  stackVM::StackMachineStepDebugger debugger;
  debugger.RunDebug();
  return 0;
}


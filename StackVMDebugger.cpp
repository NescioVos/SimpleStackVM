// Test task for Quantum Brains, 2024.
// All rights are either reserved, not reserved or reserved a little.
// ========================================================================== //
// Stack virtual machine debugger class
// Author: Ksenia Dolgorukova
// ========================================================================== //

#include "StackVMDebugger.h"

#include <iostream>

namespace stackVM {

static bool parseString(const std::string& cmdString, Instructions& cmd, int& param){
  bool hasParam = false;
  unsigned int i = 0;
  for(; i < Instructions::InstructionNumber; ++i){
    if(cmdString == InstructionNames[i]){
      cmd = (Instructions)i;
      break;
    }
  }
  if(i == Instructions::InstructionNumber)
    return false;
  
  hasParam = cmd == Instructions::CALL || cmd == Instructions::PUSH;

  if(hasParam){
    std::cin >> param;
  }
  return true;
}

void StackMachineStepDebugger::interpretStepByStep(){
  std::cout<<"Start running the code step by step\n";
  std::cout<<"Lookup commands:\n\tn - next\n\tdump - dump the stack\n\texit - exit debugging"<<std::endl;
  vm.ClearStackAndPC();
  int topStack;
  ErrorCode err;
  do{
    vm.PrintState();
    std::string lookupCommand;
    std::cin >> lookupCommand;
    while( lookupCommand != "n" && lookupCommand != "exit"){
      if( lookupCommand == "dump" || lookupCommand == "d"){
        vm.DumpStack();
      } else {
        std::cout << "Unknown lookup command" << std::endl;
      }
      std::cin >> lookupCommand;
    }
    if(lookupCommand == "exit")
      return;
  } while( vm.InterpretOneStep(topStack, err) );
  std::cout << "The vm exited with code " << ((unsigned int)err) << "\n";
  std::cout << "Top of the stack is " << topStack << "\n";
}

bool StackMachineStepDebugger::parseNextCommand(){
  std::string str;
  std::cin >> str;
  if(str == "exit"){
    return false;
  }
  if(str == "clear"){
    vm.Clear();
    std::cout<<"Cleaning the commands memory\n";
    return true;
  }

  if(str == "run" || str == "r"){
    interpretStepByStep();
    return true;
  }

  Instructions cmd;
  int param = 0;
  if(parseString(str, cmd, param)){
    vm.AddInstruction(cmd, param);
  } else {
    std::cout << "Step debugger error: unknown instruction\n";
  }
  return true;
}

void StackMachineStepDebugger::RunDebug(){
  std::cout << "-----------------------------------------------------------------------\n";
  std::cout << "Runing stack virtual machine in step-by-step debugging.\n"
    <<"Instructions available:\n";
  for(unsigned int i = 0; i < Instructions::InstructionNumber; ++i){
    std::cout << "\t" << InstructionNames[i] << "\t";
  }
  std::cout << "\n\tpush and call take 1 argument, others don't require any.\n"
    <<"\tpush needs a value to push it on the stack, call needs number of params\n\n"
    <<"Utility commands:\n"
    <<"\texit - exit the debugger\n"
    <<"\trun -- run the code step-by-step\n"
    <<"\tclear -- clears the code\n";
  std::cout << "-----------------------------------------------------------------------\n";
  while(parseNextCommand()){}
}

}// namespace stackVM

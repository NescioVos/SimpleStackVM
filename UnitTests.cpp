// Test task for Quantum Brains, 2024.
// All rights are either reserved, not reserved or reserved a little.
// ========================================================================== // 
// Unit tests for stack virtual machine
// Author: Ksenia Dolgorukova
// ========================================================================== //
#include "UnitTests.h"

#include <iostream>

namespace stackVMUnitTests{

static bool checkAssert(int value, int etalon){
  if(value != etalon){
    std::cout << std::dec << "UNIT TEST ASSERT: Return value is " << value
      << ", but " << etalon << " is expected.\n";
    return false;
  }
  return true;
}

void StackVMUnitTests::interpretAndCheckNormal(int etalon){
  int result = 0;
  stackVM::ErrorCode retCode = vm.Interpret(result);
  if( checkAssert( static_cast<int>(retCode), static_cast<int>(stackVM::ErrorCode::Success) ) && checkAssert(result, etalon) )
    std::cout << "[SUCCESS]\n";
  else
    std::cout << "[FAILED]\n";
}

void StackVMUnitTests::interpretAndCheckError(stackVM::ErrorCode errCode){
  int result = 0;
  stackVM::ErrorCode retCode = vm.Interpret(result);
  if( checkAssert( static_cast<int>(retCode), static_cast<int>(errCode) ) )
    std::cout << "[SUCCESS]\n";
  else
    std::cout << "[FAILED]\n";
}

// --------------------------------------------------------------------------- //
// Normal cases
// --------------------------------------------------------------------------- //
void StackVMUnitTests::TestAddNormal(){
  std::cout << "Running test TestAddNormal:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 5);  
  vm.AddInstruction(stackVM::Instructions::PUSH, 885);
  vm.AddInstruction(stackVM::Instructions::ADD);
  vm.AddInstruction(stackVM::Instructions::RET);
  
  interpretAndCheckNormal(890);
}

void StackVMUnitTests::TestSubNormal(){
  std::cout << "Running test TestSubNormal:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);  
  vm.AddInstruction(stackVM::Instructions::PUSH, 100);
  vm.AddInstruction(stackVM::Instructions::SUB);
  vm.AddInstruction(stackVM::Instructions::RET);

  interpretAndCheckNormal(-92);
}

void StackVMUnitTests::TestMulNormal(){
  std::cout << "Running test TestMulNormal:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 25);  
  vm.AddInstruction(stackVM::Instructions::PUSH, 4);
  vm.AddInstruction(stackVM::Instructions::MUL);
  vm.AddInstruction(stackVM::Instructions::RET);

  interpretAndCheckNormal(100);
}

void StackVMUnitTests::TestDivNormal(){
  std::cout << "Running test TestDivNormal:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 38);  
  vm.AddInstruction(stackVM::Instructions::PUSH, 2);
  vm.AddInstruction(stackVM::Instructions::DIV);
  vm.AddInstruction(stackVM::Instructions::RET);

  interpretAndCheckNormal(19);
}

void StackVMUnitTests::TestJumpNormal(){
  std::cout << "Running test TestJumpNormal:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 4); //address to jump
  vm.AddInstruction(stackVM::Instructions::JMP);
  vm.AddInstruction(stackVM::Instructions::POP);
  vm.AddInstruction(stackVM::Instructions::POP);
  vm.AddInstruction(stackVM::Instructions::PUSH, 333);
  vm.AddInstruction(stackVM::Instructions::RET);

  interpretAndCheckNormal(333);
}

void StackVMUnitTests::TestCallNormal(){
  std::cout << "Running test TestCallNormal:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 31); //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, 2);  //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 5); //function address
  vm.AddInstruction(stackVM::Instructions::CALL, 2);
  vm.AddInstruction(stackVM::Instructions::RET);
  vm.AddInstruction(stackVM::Instructions::MUL);   //the function body
  vm.AddInstruction(stackVM::Instructions::FRET);

  interpretAndCheckNormal(62);
}

void StackVMUnitTests::TestJEQNormal1(){
  std::cout << "Running test TestJEQNormal1:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 23); //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, 23); //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 7);  //function address
  vm.AddInstruction(stackVM::Instructions::JEQ);
  vm.AddInstruction(stackVM::Instructions::PUSH, -1);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::JMP);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::RET);

  interpretAndCheckNormal(8);
}

void StackVMUnitTests::TestJEQNormal2(){
  std::cout << "Running test TestJEQNormal2:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 23);  //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, -23); //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 7);   //function address
  vm.AddInstruction(stackVM::Instructions::JEQ);
  vm.AddInstruction(stackVM::Instructions::PUSH, -1);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::JMP);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::RET);

  interpretAndCheckNormal(-1);
}

void StackVMUnitTests::TestJNENormal1(){
  std::cout << "Running test TestJNENormal1:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 23); //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, -2); //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 7);  //function address
  vm.AddInstruction(stackVM::Instructions::JNE);
  vm.AddInstruction(stackVM::Instructions::PUSH, -1);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::JMP);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);  //if jump label
  vm.AddInstruction(stackVM::Instructions::RET);    //else exit label

  interpretAndCheckNormal(8);
}

void StackVMUnitTests::TestJNENormal2(){
  std::cout << "Running test TestJNENormal2:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 20);  //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, 20);  //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 7);   //function address
  vm.AddInstruction(stackVM::Instructions::JNE);
  vm.AddInstruction(stackVM::Instructions::PUSH, -1);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::JMP);    
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);  //if jump label
  vm.AddInstruction(stackVM::Instructions::RET);    //else exit label

  interpretAndCheckNormal(-1);
}

void StackVMUnitTests::TestJGTNormal1(){
  std::cout << "Running test TestJGTNormal1:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 15);   //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, -200); //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 7);  //function address
  vm.AddInstruction(stackVM::Instructions::JGT);
  vm.AddInstruction(stackVM::Instructions::PUSH, -1);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::JMP);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);  //if jump label
  vm.AddInstruction(stackVM::Instructions::RET);    //else exit label

  interpretAndCheckNormal(8);
}

void StackVMUnitTests::TestJGTNormal2(){
  std::cout << "Running test TestJGTNormal2:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 6); //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, 6769);   //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 7);   //function address
  vm.AddInstruction(stackVM::Instructions::JGT);
  vm.AddInstruction(stackVM::Instructions::PUSH, -1);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::JMP);    
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);  //if jump label
  vm.AddInstruction(stackVM::Instructions::RET);    //else exit label

  interpretAndCheckNormal(-1);
}

void StackVMUnitTests::TestJGENormal1(){
  std::cout << "Running test TestJGENormal1:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 15);   //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, 15); //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 7);  //function address
  vm.AddInstruction(stackVM::Instructions::JGE);
  vm.AddInstruction(stackVM::Instructions::PUSH, -1);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::JMP);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);  //if jump label
  vm.AddInstruction(stackVM::Instructions::RET);    //else exit label

  interpretAndCheckNormal(8);
}

void StackVMUnitTests::TestJGENormal2(){
  std::cout << "Running test TestJGENormal2:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 0); //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, 6);   //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 7);   //function address
  vm.AddInstruction(stackVM::Instructions::JGE);
  vm.AddInstruction(stackVM::Instructions::PUSH, -1);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::JMP);    
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);  //if jump label
  vm.AddInstruction(stackVM::Instructions::RET);    //else exit label

  interpretAndCheckNormal(-1);
}
void StackVMUnitTests::TestJLSNormal1(){
  std::cout << "Running test TestJLSNormal1:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 14);   //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, 15);   //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 7);  //function address
  vm.AddInstruction(stackVM::Instructions::JLS);
  vm.AddInstruction(stackVM::Instructions::PUSH, -1);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::JMP);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);  //if jump label
  vm.AddInstruction(stackVM::Instructions::RET);    //else exit label

  interpretAndCheckNormal(8);
}

void StackVMUnitTests::TestJLSNormal2(){
  std::cout << "Running test TestJGLSNormal2:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 300); //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, 6);   //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 7);   //function address
  vm.AddInstruction(stackVM::Instructions::JLS);
  vm.AddInstruction(stackVM::Instructions::PUSH, -1);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::JMP);    
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);  //if jump label
  vm.AddInstruction(stackVM::Instructions::RET);    //else exit label

  interpretAndCheckNormal(-1);
}

void StackVMUnitTests::TestJLENormal1(){
  std::cout << "Running test TestJLENormal1:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 14);   //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, 14);   //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 7);  //function address
  vm.AddInstruction(stackVM::Instructions::JLE);
  vm.AddInstruction(stackVM::Instructions::PUSH, -1);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::JMP);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);  //if jump label
  vm.AddInstruction(stackVM::Instructions::RET);    //else exit label

  interpretAndCheckNormal(8);
}

void StackVMUnitTests::TestJLENormal2(){
  std::cout << "Running test TestJLENormal2:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 300); //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, 6);   //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 7);   //function address
  vm.AddInstruction(stackVM::Instructions::JLE);
  vm.AddInstruction(stackVM::Instructions::PUSH, -1);
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);
  vm.AddInstruction(stackVM::Instructions::JMP);    
  vm.AddInstruction(stackVM::Instructions::PUSH, 8);  //if jump label
  vm.AddInstruction(stackVM::Instructions::RET);    //else exit label

  interpretAndCheckNormal(-1);
}

// --------------------------------------------------------------------------- //
// Error cases
// --------------------------------------------------------------------------- //
void StackVMUnitTests::TestPopEmpty(){
  std::cout << "Running test TestPopEmpty:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::POP);
  vm.AddInstruction(stackVM::Instructions::RET);

  interpretAndCheckError(stackVM::ErrorCode::StackIsEmpty);
}

void StackVMUnitTests::TestRetEmpty(){
  std::cout << "Running test TestRetEmpty:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::RET);

  interpretAndCheckError(stackVM::ErrorCode::StackIsEmpty);
}

void StackVMUnitTests::TestSwapOfOne(){
  std::cout << "Running test TestSwapOfOne:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 666);
  vm.AddInstruction(stackVM::Instructions::SWAP);
  vm.AddInstruction(stackVM::Instructions::RET);

  interpretAndCheckError(stackVM::ErrorCode::StackIsEmpty);
}

void StackVMUnitTests::TestDivByZero(){
  std::cout << "Running test TestDivByZero:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 321);
  vm.AddInstruction(stackVM::Instructions::PUSH, 0);
  vm.AddInstruction(stackVM::Instructions::DIV);
  vm.AddInstruction(stackVM::Instructions::RET);

  interpretAndCheckError(stackVM::ErrorCode::DivisionByZero);
}

void StackVMUnitTests::TestJumpOutOfScope(){
  std::cout << "Running test TestJumpOutOfScope:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 23); //address to jump
  vm.AddInstruction(stackVM::Instructions::JMP);
  vm.AddInstruction(stackVM::Instructions::POP);
  vm.AddInstruction(stackVM::Instructions::POP);
  vm.AddInstruction(stackVM::Instructions::PUSH, 333);
  vm.AddInstruction(stackVM::Instructions::RET);

  interpretAndCheckError(stackVM::ErrorCode::SegmentationFault);
}

void StackVMUnitTests::TestTooManyParamsCall(){
  std::cout << "Running test TestTooManyParamsCall:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::PUSH, 31); //param 1
  vm.AddInstruction(stackVM::Instructions::PUSH, 2);  //param 2
  vm.AddInstruction(stackVM::Instructions::PUSH, 5);  //function address
  vm.AddInstruction(stackVM::Instructions::CALL, 4);
  vm.AddInstruction(stackVM::Instructions::RET);
  vm.AddInstruction(stackVM::Instructions::MUL);   //the function body
  vm.AddInstruction(stackVM::Instructions::FRET);

  interpretAndCheckError(stackVM::ErrorCode::TooManyFuncParams);
}

void StackVMUnitTests::TestUnknownInstruction(){
  std::cout << "Running test TestRetEmpty:\n";
  vm.Clear();
  vm.AddInstruction(stackVM::Instructions::InstructionNumber);
  vm.AddInstruction(stackVM::Instructions::RET);

  interpretAndCheckError(stackVM::ErrorCode::UnknownInstruction);
}


void StackVMUnitTests::RunAllTests(){
  std::cout << "-------------------------------------------\n";
  std::cout << "Running normal cases for virtual machine\n";
  std::cout << "-------------------------------------------\n";
  TestAddNormal();
  TestSubNormal();
  TestMulNormal();
  TestJumpNormal();
  TestCallNormal();
  TestDivNormal();
  TestJEQNormal1();
  TestJEQNormal2();
  TestJNENormal1();
  TestJNENormal2();
  TestJGTNormal1();
  TestJGTNormal2();
  TestJGENormal1();
  TestJGENormal2();
  TestJLSNormal1();
  TestJLSNormal2();
  TestJLENormal1();
  TestJLENormal2();

  std::cout << "\n-------------------------------------------\n";
  std::cout << "Running error cases for virtual machine\n";
  std::cout << "-------------------------------------------\n";
  TestPopEmpty();
  TestRetEmpty();
  TestSwapOfOne();
  TestDivByZero();
  TestJumpOutOfScope();
  TestTooManyParamsCall();
  TestUnknownInstruction();
  std::cout << "-------------------------------------------\n";
  std::cout << "DONE" << std::endl;
  std::cout << "-------------------------------------------\n";
}

}// namespace stackVMUnitTests

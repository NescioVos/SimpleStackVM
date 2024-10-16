
// Test task for Quantum Brains, 2024.
// All rights are either reserved, not reserved or reserved a little.
// ========================================================================== //
// Unit tests for stack virtual machine
// Author: Ksenia Dolgorukova
// ========================================================================== //
#pragma once
#include "StackVirtualMachine.h"

namespace stackVMUnitTests{

class StackVMUnitTests{
public:
    void RunAllTests();

    // Separate tests
    // Normal cases
    void TestAddNormal();
    void TestSubNormal();
    void TestMulNormal();
    void TestJumpNormal();
    void TestCallNormal();
    void TestDivNormal();
    //conditional jumps
    void TestJEQNormal1();
	void TestJEQNormal2();
	void TestJNENormal1();
	void TestJNENormal2();
    void TestJGTNormal1();
    void TestJGTNormal2();
    void TestJGENormal1();
    void TestJGENormal2();
    void TestJLSNormal1();
    void TestJLSNormal2();
    void TestJLENormal1();
    void TestJLENormal2();

    // Error cases
    void TestPopEmpty();
    void TestRetEmpty();
    void TestSwapOfOne();
    void TestDivByZero();
    void TestJumpOutOfScope();
    void TestTooManyParamsCall();
    void TestUnknownInstruction();
private:
    void interpretAndCheckNormal(int etalon);
    void interpretAndCheckError(stackVM::ErrorCode errCode);
private:
    stackVM::StackVirtualMachine vm;
};

}//namespace stackVMUnitTests
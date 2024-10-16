# SimpleStackVM
Simple stack virtual machine.


Supports following instructions.

Basic stack commands: **push, pop, swap**

Simple arithmetics: **add, sub,  mul, div**

Return from the program: **ret**

Unconditional jump: **jump**

Conditional jumps: **jne, jeq, jgt, jge, jls, jle**

All jumps require an address pushed on top of the stack before the call; conditional jumps require two operands on the stack before the address.

Function call and return: **call, fret**

Function **call** takes 1 parameter -- the number of arguments.

# Debugger
There is a debugger to try it out: **svm_dbg**. Enter instructions and run them with **run** command. The program will be executed step-by-step. If you want to clear the command buffer, enter **clear**.

In the executing mode, there are several commands available. Press **n** for continue and enter **dump** to dump out the current stack state.

Type **exit** to exit the debugger.

# Unit tests
Unit tests are made to test the virtual machine. You can run them with **svm_ut** tool.

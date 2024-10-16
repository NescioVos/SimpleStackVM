// Test task for Quantum Brains, 2024.
// All rights are either reserved, not reserved or reserved a little.
// ========================================================================== // 
// Stack virtual machine class implementation
// Author: Ksenia Dolgorukova
// ========================================================================== //

#include "StackVirtualMachine.h"

#include <iostream>

namespace stackVM {

// Constants
const unsigned short BYTE_SIZE = 8;
const unsigned short INSTRUCTION_CODE_SIZE = 5;
const unsigned int INSTRUCTION_OFFSET = sizeof(unsigned long int) * BYTE_SIZE - INSTRUCTION_CODE_SIZE;
const unsigned long int PARAMETER_MASK = (1ul << (sizeof(unsigned int) * BYTE_SIZE)) - 1;
const unsigned long int INSTRUCTION_MASK = (1ul << INSTRUCTION_CODE_SIZE) - 1;

// ========================================================================== //
// StackVirtualMachine class implementation                                   //
// ========================================================================== //
StackVirtualMachine::StackVirtualMachine(): pc(0){ }

void StackVirtualMachine::AddInstruction(Instructions instruction, unsigned int param){
    instructions.push_back((((unsigned long)instruction) << INSTRUCTION_OFFSET) | param);
}

void StackVirtualMachine::Clear(){
    instructions.clear();
    stack.clear();
    pc = 0;
}

void StackVirtualMachine::ClearStackAndPC(){
    stack.clear();
    pc = 0;
}

// --------------------------------------------------------------------------- //
// Instructions
// --------------------------------------------------------------------------- //

// Working with the stack
ErrorCode StackVirtualMachine::internalPop(int& top){
    if(stack.empty())
		return ErrorCode::StackIsEmpty;
    top = stack.back();
	stack.pop_back();
	return ErrorCode::Success;
}

ErrorCode StackVirtualMachine::push(int arg){
    int oldSize = stack.size();
    stack.push_back(arg);
    if(oldSize == stack.size())
        return ErrorCode::StackIsFull;
    return ErrorCode::Success;
}

ErrorCode StackVirtualMachine::pop(){
    if(stack.empty())
		return ErrorCode::StackIsEmpty;
    stack.pop_back();
    return ErrorCode::Success;
}

// Fast swap
static void swapOnPlace(int& a, int& b){
    a ^= b;
    b ^= a;
    a ^= b;
}

ErrorCode StackVirtualMachine::swap(){
    size_t stackSize = stack.size();
    if(stackSize < 2)
        return ErrorCode::StackIsEmpty;
    
    swapOnPlace(stack[stackSize - 1], stack[stackSize - 2]);
    return ErrorCode::Success;
}

// Simple wrapper around instruction calls for usage by other instructions
#define safeInstructionCall(func, param) { \
    ErrorCode retCode = func(param);       \
    if( retCode != ErrorCode::Success ){   \
        return retCode;                    \
    }                                      \
}

// Arithmetics
ErrorCode StackVirtualMachine::add(){
    int op1, op2;
    safeInstructionCall(internalPop, op1);    
    safeInstructionCall(internalPop, op2);
	return push(op1 + op2);
}

// stack[sp-1] - stack[sp]
ErrorCode StackVirtualMachine::sub(){
    int op1, op2;
    safeInstructionCall(internalPop, op1);  
    safeInstructionCall(internalPop, op2);
	return push(op2 - op1);
}

ErrorCode StackVirtualMachine::mul(){
    int op1, op2;
    safeInstructionCall(internalPop, op1);  
    safeInstructionCall(internalPop, op2);    
	return push(op2 * op1);
}

// stack[sp-1] / stack[sp]
ErrorCode StackVirtualMachine::div(){
    int op1, op2;
    safeInstructionCall(internalPop, op1);
    
    if(op1 == 0){
        return ErrorCode::DivisionByZero;
    }
    
    safeInstructionCall(internalPop, op2);   
	return push(op2 / op1);
}

// Actually, nothing to do
ErrorCode StackVirtualMachine::ret(){
    return ErrorCode::Success;
}

// Unconditional jump to the address on the top of the stack
ErrorCode StackVirtualMachine::jump(){
    int stackTop = 0;
    safeInstructionCall(internalPop, stackTop);
    pc = (unsigned int) stackTop;
    return ErrorCode::Success;
}

// Calls the function which parameters and address are on the top of the stack
ErrorCode StackVirtualMachine::call(unsigned int paramsNo){
	int callAddress = 0;
    safeInstructionCall(internalPop, callAddress);
	
	unsigned int returnAddress = pc + 1;

	// Move returnAddress before parameters  	
	if( paramsNo > stack.size()){
		return ErrorCode::TooManyFuncParams;
	}
	stack.insert(stack.end() - paramsNo, returnAddress);    

	safeInstructionCall(push, callAddress);
	return jump();
}

// Clears stack from the function call stack, set the result to the top of the stack and return
ErrorCode StackVirtualMachine::functionReturn(){
    // Get return value from the stack
	int retVal = 0;
    safeInstructionCall(internalPop, retVal);
	
	// Get return address
	int retAddress = 0;
    safeInstructionCall(internalPop, retAddress);
	
	// Push the result
	safeInstructionCall(push, retVal);
	
	// Jump back to the next after call instruction
	safeInstructionCall(push, retAddress);
	return jump();
}

ErrorCode StackVirtualMachine::internalJump(bool (*compare)(int, int)){
    int address, op1, op2;
    safeInstructionCall(internalPop, address);  
    safeInstructionCall(internalPop, op1);  
    safeInstructionCall(internalPop, op2);
    if(compare(op2, op1)){ //NOTE: we are reversing params here
        safeInstructionCall(push, address);
        return jump();
    }
    ++pc;
    return ErrorCode::Success;
}

ErrorCode StackVirtualMachine::jumpIfEqual(){
    return internalJump( [](int a, int b){ return a == b; } );
}

ErrorCode StackVirtualMachine::jumpIfNotEqual(){
    return internalJump( [](int a, int b){ return a != b; } );
}

ErrorCode StackVirtualMachine::jumpIfGreater(){
    return internalJump( [](int a, int b){ return a > b; } );
}

ErrorCode StackVirtualMachine::jumpIfGreaterOrEqual(){
    return internalJump( [](int a, int b){ return a >= b; } );
}

ErrorCode StackVirtualMachine::jumpIfLess(){
    return internalJump( [](int a, int b){ return a < b; } );
}

ErrorCode StackVirtualMachine::jumpIfLessOrEqual(){
    return internalJump( [](int a, int b){ return a <= b; } );
}

void StackVirtualMachine::error(ErrorCode errorCode){
    if(errorCode == ErrorCode::Success)
        return;
    
    std::string msg;
    switch(errorCode){
        case ErrorCode::StackIsEmpty: msg = "Stack is empty"; break;
        case ErrorCode::StackIsFull: msg = "Stack is full"; break;
        case ErrorCode::SegmentationFault: msg = "Segmentation fault"; break;
        case ErrorCode::UnknownInstruction: msg = "Unknown instruction"; break;
        case ErrorCode::DivisionByZero: msg = "Division by zero"; break;
        case ErrorCode::TooManyFuncParams: msg = "Too many function params"; break;
        default:
            msg = "Unknown error"; break;
    }
    std::cout<<"VIRTUAL MACHINE ERROR: "<<msg<<"\n";
}

void StackVirtualMachine::DumpStack(){
    std::cout << "Stack from top to bottom:\n";
    for(int i = stack.size() - 1; i >= 0; --i){
        std::cout << "\t" << stack[i] << "\n";
    }
}

void StackVirtualMachine::PrintState(){
    std::cout << "Program counter position = "<< pc <<"\n";
    if( pc >= 0 && pc < instructions.size() ){
        unsigned int code = ((instructions[pc] >> INSTRUCTION_OFFSET) & INSTRUCTION_MASK);
        unsigned int parameter = (instructions[pc] & PARAMETER_MASK);
        std::cout << "Instruction code = " << (code < Instructions::InstructionNumber? InstructionNames[code] : "UNKNOWN") 
            << ", parameter is "<< parameter <<"\n";
    } else {
        std::cout << "Instruction is out of memory scope\n";
    }    
}

bool StackVirtualMachine::InterpretOneStep(int& topStack, ErrorCode& retCode){
    if( pc >= instructions.size() ){
		error(ErrorCode::SegmentationFault);
        retCode = ErrorCode::SegmentationFault;
        return false;
	}
	bool isJmp = false, isRet = false;
    retCode = ErrorCode::Success;
	switch((instructions[pc]>>INSTRUCTION_OFFSET) & INSTRUCTION_MASK){
	    // Basic stack imstructions
		case Instructions::PUSH: retCode = push(instructions[pc] & PARAMETER_MASK); break;
		case Instructions::POP: retCode = pop(); break;
		case Instructions::SWAP: retCode = swap(); break;

		// Basic arithmetics
		case Instructions::ADD: retCode = add(); break;
		case Instructions::SUB: retCode = sub(); break;
		case Instructions::MUL: retCode = mul(); break;
		case Instructions::DIV: retCode = div(); break;

		// Return and jumps
		case Instructions::RET: retCode = ret(); isRet = true; break;
		case Instructions::JMP: retCode = jump(); isJmp = true; break;
		case Instructions::CALL: retCode = call(instructions[pc] & PARAMETER_MASK); isJmp = true; break;
		case Instructions::FRET: retCode = functionReturn(); isJmp = true; break;
        case Instructions::JEQ: retCode = jumpIfEqual(); isJmp = true; break;
        case Instructions::JNE: retCode = jumpIfNotEqual(); isJmp = true; break;
        case Instructions::JGT: retCode = jumpIfGreater(); isJmp = true; break;
        case Instructions::JGE: retCode = jumpIfGreaterOrEqual(); isJmp = true; break;
        case Instructions::JLS: retCode = jumpIfLess(); isJmp = true; break;
        case Instructions::JLE: retCode = jumpIfLessOrEqual(); isJmp = true; break;
		default:
			retCode = ErrorCode::UnknownInstruction;
	};
     
    if(retCode != ErrorCode::Success){
#ifdef DEBUG
        PrintState();
        DumpStack();
#endif
        error(retCode);
        return false;
    }

    if(isRet){
        retCode = internalPop(topStack);
        if( retCode != ErrorCode::Success)
            error(retCode);
        return false;
    }

	if(!isJmp)
		++pc;
    return true;
}

// The main interpretation loop
// Reads and interprets instructions one-by-one
ErrorCode StackVirtualMachine::Interpret(int& topStack){
    ErrorCode retCode;
	while( InterpretOneStep(topStack, retCode) ){ };
    return retCode;
}


}// namespace stackVM
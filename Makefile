unitTests:
	g++ StackVirtualMachine.cpp UnitTests.cpp main_ut.cpp -o svm_ut

stepDebugger:
	g++ StackVirtualMachine.cpp UnitTests.cpp main_dbg.cpp StackVMDebugger.cpp -o svm_dbg

all: stepDebugger unitTests

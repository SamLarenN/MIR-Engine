#pragma once
class PeMutator
{
public:

	bool Mutate(BYTE* inBuf, DWORD SizeToMut, bool MutAll);
	int OperationCalculation(DWORD &FirstOP, DWORD SecondOP, int &FirstIMM, int SecondIMM);

private:
	int MutationCount = 0;
	int InstructionCount = 0;
};

extern PeMutator* PeMut;